/*
 * Copyright (c) 2015-2022, Arm Limited and Contributors. All rights reserved.
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include "al_aarch64_core.h"
#include "al_barrier.h"

#include "al_gicv3_private.h"
#include "al_gicv3_dist.h"
#include "al_gicv3_rdist.h"
#include "al_gicv3.h"

#include "al_type.h"

const AL_GICV3_DrvDataStruct *Gicv3DrvData;

/* Check interrupt ID for SGI/(E)PPI and (E)SPIs */
static AL_BOOL AlGicv3_IsSgiPpi(AL_U32 Id);

/*******************************************************************************
 * This function initialises the ARM GICv3 driver in EL3 with provided platform
 * inputs.
 ******************************************************************************/
AL_VOID AlGicv3_DriverInit(const AL_GICV3_DrvDataStruct *DrvData)
{
    AL_U32 GicVer;

    assert(DrvData != NULL);
    assert(DrvData->CpuId != NULL);
    assert(DrvData->GicdBase != 0U);
    assert(DrvData->RdistNum != 0U);
    assert(DrvData->RdistBaseAddrs != NULL);

    GicVer = Gicd_ReadPidr2(DrvData->GicdBase);
    GicVer >>= PIDR2_ARCH_REV_SHIFT;
    GicVer &= PIDR2_ARCH_REV_MASK;

    if (DrvData->GicrBase != 0U) {
        /*
         * Find the base address of each implemented Redistributor interface.
         * The number of interfaces should be equal to the number of CPUs in the
         * system. The memory for saving these addresses has to be allocated by
         * the platform port
         */
        AlGicv3_Rdist_BaseAddrsProbe(DrvData->RdistBaseAddrs, DrvData->RdistNum,
                                     DrvData->GicrBase, DrvData->MpidrToCorePos);
    }

    Gicv3DrvData = DrvData;
}

/*******************************************************************************
 * This function initialises the GIC distributor interface based upon the data
 * provided by the platform while initialising the driver.
 ******************************************************************************/
AL_VOID AlGicv3_DistInit(AL_VOID)
{
    AL_U32 BitMap = 0;

    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);

    /*
     * Clear the "enable" bits for G0/G1S/G1NS interrupts before configuring
     * the ARE_S bit. The Distributor might generate a system error
     * otherwise.
     */
    Gicd_ClrCtlr(Gicv3DrvData->GicdBase, CTLR_ENABLE_G0_BIT | CTLR_ENABLE_G1S_BIT |
                                         CTLR_ENABLE_G1NS_BIT, RWP_TRUE);

    /* Set the ARE_S and ARE_NS bit now that interrupts have been disabled */
    Gicd_SetCtlr(Gicv3DrvData->GicdBase, CTLR_ARE_S_BIT | CTLR_ARE_NS_BIT, RWP_TRUE);

    /* Set the default attribute of all (E)SPIs */
    AlGicv3_SpisConfigDefaults(Gicv3DrvData->GicdBase);

    #ifdef SUPPORT_NONSECURE
    BitMap |= CTLR_ENABLE_G1NS_BIT;
    #else
    BitMap |= CTLR_ENABLE_G1S_BIT;
    #endif

    /* Enable the secure (E)SPIs now that they have been configured */
    Gicd_SetCtlr(Gicv3DrvData->GicdBase, BitMap, RWP_TRUE);
}

/*******************************************************************************
 * This function initialises the GIC Redistributor interface of the calling CPU
 * (identified by the 'ProcNum' parameter) based upon the data provided by the
 * platform while initialising the driver.
 ******************************************************************************/
AL_VOID AlGicv3_RdistInit(AL_U32 ProcNum)
{
    AL_UINTPTR GicrBase;
    AL_U32 BitMap = 0;
    AL_U32 Ctlr;

    assert(Gicv3DrvData != NULL);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);

    Ctlr = Gicd_ReadCtlr(Gicv3DrvData->GicdBase);
    assert((Ctlr & CTLR_ARE_S_BIT) != 0U);

    GicrBase = Gicv3DrvData->RdistBaseAddrs[ProcNum];
    assert(GicrBase != 0U);

    /* Set the default attribute of all SGIs and (E)PPIs */
    AlGicv3_PpiSgiConfigDefaults(GicrBase);

    #ifdef SUPPORT_NONSECURE
    BitMap |= CTLR_ENABLE_G1NS_BIT;
    #else
    BitMap |= CTLR_ENABLE_G1S_BIT;
    #endif

    /* Enable interrupt groups as required, if not already */
    if ((Ctlr & BitMap) != BitMap) {
        Gicd_SetCtlr(Gicv3DrvData->GicdBase, BitMap, RWP_TRUE);
    }
}

/*******************************************************************************
 * This function enables the GIC CPU interface of the calling CPU using only
 * system register accesses.
 ******************************************************************************/
AL_VOID AlGicv3_CpuIfEnable(AL_U32 ProcNum)
{
    AL_UINTPTR GicrBase;
    AL_U32 IccSreEl1;

    assert(Gicv3DrvData != NULL);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    /* Mark the connected core as awake */
    GicrBase = Gicv3DrvData->RdistBaseAddrs[ProcNum];
    AlGicv3_Rdist_MarkCoreAwake(GicrBase);

    /* Has been init in al_aarch64_startup.S */
    /* Disable the legacy interrupt bypass */
    IccSreEl1 = ICC_SRE_DIB_BIT | ICC_SRE_DFB_BIT;

    /*
     * Enable system register access for EL3 and allow lower exception
     * levels to configure the same for themselves. If the legacy mode is
     * not supported, the SRE bit is RAO/WI
     */
    IccSreEl1 |= (ICC_SRE_SRE_BIT);
    ARCH_SYSREG_WRITE(icc_sre_el1, (ARCH_SYSREG_READ(icc_sre_el1) | IccSreEl1));

    /* NS support in al_aarch64_startup.S */
    // ScrEl3 = read_scr_el3();

    // /*
    //  * Switch to NS state to write Non secure ICC_SRE_EL1 and
    //  * ICC_SRE_EL2 registers.
    //  */
    // write_scr_el3(ScrEl3 | SCR_NS_BIT);
    // ISB();

    // write_icc_sre_el2(read_icc_sre_el2() | IccSreEl1);
    // write_icc_sre_el1(ICC_SRE_SRE_BIT);
    // ISB();

    // /* Switch to secure state. */
    // write_scr_el3(ScrEl3 & (~SCR_NS_BIT));
    // ISB();

    // /* Write the secure ICC_SRE_EL1 register */
    // write_icc_sre_el1(ICC_SRE_SRE_BIT);
    ISB();

    /* Program the idle Priority in the PMR */
    ARCH_SYSREG_WRITE(icc_pmr_el1, GIC_PRI_MASK);

    /* Enable Group0 interrupts */
    // write_icc_igrpen0_el1(IGRPEN1_EL1_ENABLE_G0_BIT);

    // /* Enable Group1 Secure interrupts */
    // write_icc_igrpen1_el3(read_icc_igrpen1_el3() |
    //             IGRPEN1_EL3_ENABLE_G1S_BIT);

    ARCH_SYSREG_WRITE(icc_bpr1_el1, 7);
    ARCH_SYSREG_WRITE(icc_ctlr_el1, 0);

    ISB();

    /* Enable Group1 Secure interrupts */
    ARCH_SYSREG_WRITE(icc_igrpen1_el1, 1);

    /* Add DSB to ensure visibility of System register writes */
    DSB();
}

/*******************************************************************************
 * This function disables the GIC CPU interface of the calling CPU using
 * only system register accesses.
 ******************************************************************************/
AL_VOID AlGicv3_CpuIfDisable(AL_U32 ProcNum)
{
    AL_UINTPTR GicrBase;

    assert(Gicv3DrvData != NULL);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    /* Disable legacy interrupt bypass */
    ARCH_SYSREG_WRITE(icc_sre_el1, (ARCH_SYSREG_READ(icc_sre_el1) |
              (ICC_SRE_DIB_BIT | ICC_SRE_DFB_BIT)));

    /* Disable Group0 interrupts */
    ARCH_SYSREG_WRITE(icc_igrpen0_el1, (ARCH_SYSREG_READ(icc_igrpen0_el1) &
                  ~IGRPEN1_EL1_ENABLE_G0_BIT));

    /* Disable Group1 Secure and Non-Secure interrupts */
    ARCH_SYSREG_WRITE(icc_igrpen1_el3, (ARCH_SYSREG_READ(icc_igrpen1_el3) &
                  ~(IGRPEN1_EL3_ENABLE_G1NS_BIT |
                  IGRPEN1_EL3_ENABLE_G1S_BIT)));

    /* Synchronise accesses to group enable registers */
    ISB();
    /* Add DSB to ensure visibility of System register writes */
    DSB();

    GicrBase = Gicv3DrvData->RdistBaseAddrs[ProcNum];
    assert(GicrBase != 0UL);

    /* Mark the connected core as asleep */
    AlGicv3_Rdist_MarkCoreAsleep(GicrBase);
}

/*******************************************************************************
 * This function returns the id of the highest Priority pending interrupt at
 * the GIC cpu interface.
 ******************************************************************************/
AL_U32 AlGicv3_GetPendingInterruptId(AL_VOID)
{
    AL_U32 Id;

    Id = (AL_U32)ARCH_SYSREG_READ(icc_hppir0_el1) & HPPIR0_EL1_INTID_MASK;

    /*
     * If the ID is special identifier corresponding to G1S or G1NS
     * interrupt, then read the highest pending group 1 interrupt.
     */
    if ((Id == PENDING_G1S_INTID) || (Id == PENDING_G1NS_INTID)) {
        return (AL_U32)ARCH_SYSREG_READ(icc_hppir1_el1) & HPPIR1_EL1_INTID_MASK;
    }

    return Id;
}

/*******************************************************************************
 * This function returns the type of the highest Priority pending interrupt at
 * the GIC cpu interface. The return values can be one of the following :
 *   PENDING_G1S_INTID  : The interrupt type is secure Group 1.
 *   PENDING_G1NS_INTID : The interrupt type is non secure Group 1.
 *   0 - 1019           : The interrupt type is secure Group 0.
 *   GIC_SPURIOUS_INTERRUPT : there is no pending interrupt with
 *                            sufficient Priority to be signaled
 ******************************************************************************/
AL_U32 AlGicv3_GetPendingInterruptType(AL_VOID)
{
    return (AL_U32)ARCH_SYSREG_READ(icc_hppir0_el1) & HPPIR0_EL1_INTID_MASK;
}

/*******************************************************************************
 * This function returns the type of the interrupt id depending upon the group
 * this interrupt has been configured under by the interrupt controller i.e.
 * group0 or group1 Secure / Non Secure. The return value can be one of the
 * following :
 *    INTR_GROUP0  : The interrupt type is a Secure Group 0 interrupt
 *    INTR_GROUP1S : The interrupt type is a Secure Group 1 secure interrupt
 *    INTR_GROUP1NS: The interrupt type is a Secure Group 1 non secure
 *                   interrupt.
 ******************************************************************************/
AL_U32 AlGicv3_GetInterruptType(AL_U32 Id, AL_U32 ProcNum)
{
    AL_U32 Igroup, Grpmodr;
    AL_UINTPTR GicrBase;

    assert(Gicv3DrvData != NULL);

    /* Ensure the parameters are valid */
    assert((Id < PENDING_G1S_INTID) || (Id >= MIN_LPI_ID));
    assert(ProcNum < Gicv3DrvData->RdistNum);

    /* All LPI interrupts are Group 1 non secure */
    if (Id >= MIN_LPI_ID) {
        return INTR_GROUP1NS;
    }

    /* Check interrupt ID */
    if (AlGicv3_IsSgiPpi(Id)) {
        /* SGIs: 0-15, PPIs: 16-31, EPPIs: 1056-1119 */
        assert(Gicv3DrvData->RdistBaseAddrs != NULL);
        GicrBase = Gicv3DrvData->RdistBaseAddrs[ProcNum];
        Igroup = Gicr_GetIgroupr(GicrBase, Id);
        Grpmodr = Gicr_GetIgrpmodr(GicrBase, Id);
    } else {
        /* SPIs: 32-1019, ESPIs: 4096-5119 */
        assert(Gicv3DrvData->GicdBase != 0U);
        Igroup = Gicd_GetIgroupr(Gicv3DrvData->GicdBase, Id);
        Grpmodr = Gicd_GetIgrpmodr(Gicv3DrvData->GicdBase, Id);
    }

    /*
     * If the IGROUP bit is set, then it is a Group 1 Non secure
     * interrupt
     */
    if (Igroup != 0U) {
        return INTR_GROUP1NS;
    }

    /* If the GRPMOD bit is set, then it is a Group 1 Secure interrupt */
    if (Grpmodr != 0U) {
        return INTR_GROUP1S;
    }

    /* Else it is a Group 0 Secure interrupt */
    return INTR_GROUP0;
}

/*******************************************************************************
 * This function gets the Priority of the interrupt the processor is currently
 * servicing.
 ******************************************************************************/
AL_U32 AlGicv3_GetRunningPriority(AL_VOID)
{
    return (AL_U32)ARCH_SYSREG_READ(icc_rpr_el1);
}

/*******************************************************************************
 * This function checks if the interrupt identified by id is active (whether the
 * state is either active, or active and pending). The ProcNum is used if the
 * interrupt is SGI or (E)PPI and programs the corresponding Redistributor
 * interface.
 ******************************************************************************/
AL_U32 AlGicv3_GetInterruptActive(AL_U32 Id, AL_U32 ProcNum)
{
    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    /* Check interrupt ID */
    if (AlGicv3_IsSgiPpi(Id)) {
        /* For SGIs: 0-15, PPIs: 16-31 and EPPIs: 1056-1119 */
        return Gicr_GetIsactiver(
            Gicv3DrvData->RdistBaseAddrs[ProcNum], Id);
    }

    /* For SPIs: 32-1019 and ESPIs: 4096-5119 */
    return Gicd_GetIsactiver(Gicv3DrvData->GicdBase, Id);
}

/*******************************************************************************
 * This function enables the interrupt identified by id. The ProcNum
 * is used if the interrupt is SGI or PPI, and programs the corresponding
 * Redistributor interface.
 ******************************************************************************/
AL_VOID AlGicv3_EnableInterrupt(AL_U32 Id, AL_U32 ProcNum)
{
    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    /*
     * Ensure that any shared variable updates depending on out of band
     * interrupt trigger are observed before enabling interrupt.
     */
    DSB();

    /* Check interrupt ID */
    if (AlGicv3_IsSgiPpi(Id)) {
        /* For SGIs: 0-15, PPIs: 16-31 and EPPIs: 1056-1119 */
        Gicr_SetIsenabler(
            Gicv3DrvData->RdistBaseAddrs[ProcNum], Id);
    } else {
        /* For SPIs: 32-1019 and ESPIs: 4096-5119 */
        Gicd_SetIsenabler(Gicv3DrvData->GicdBase, Id);
    }
}

/*******************************************************************************
 * This function disables the interrupt identified by id. The ProcNum
 * is used if the interrupt is SGI or PPI, and programs the corresponding
 * Redistributor interface.
 ******************************************************************************/
AL_VOID AlGicv3_DisableInterrupt(AL_U32 Id, AL_U32 ProcNum)
{
    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    /*
     * Disable interrupt, and ensure that any shared variable updates
     * depending on out of band interrupt trigger are observed afterwards.
     */

    /* Check interrupt ID */
    if (AlGicv3_IsSgiPpi(Id)) {
        /* For SGIs: 0-15, PPIs: 16-31 and EPPIs: 1056-1119 */
        Gicr_SetIcenabler(
            Gicv3DrvData->RdistBaseAddrs[ProcNum], Id);

        /* Write to clear enable requires waiting for pending writes */
        Gicr_WaitForPendingWrite(
            Gicv3DrvData->RdistBaseAddrs[ProcNum]);
    } else {
        /* For SPIs: 32-1019 and ESPIs: 4096-5119 */
        Gicd_SetIcenabler(Gicv3DrvData->GicdBase, Id);

        /* Write to clear enable requires waiting for pending writes */
        Gicd_WaitForPendingWrite(Gicv3DrvData->GicdBase);
    }

    DSB();
}

/*******************************************************************************
 * This function sets the interrupt Priority as supplied for the given interrupt
 * id.
 ******************************************************************************/
AL_VOID AlGicv3_SetInterruptPriority(AL_U32 Id, AL_U32 ProcNum, AL_U32 Priority)
{
    AL_UINTPTR GicrBase;

    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    /* Check interrupt ID */
    if (AlGicv3_IsSgiPpi(Id)) {
        /* For SGIs: 0-15, PPIs: 16-31 and EPPIs: 1056-1119 */
        GicrBase = Gicv3DrvData->RdistBaseAddrs[ProcNum];
        Gicr_SetIpriorityr(GicrBase, Id, Priority);
    } else {
        /* For SPIs: 32-1019 and ESPIs: 4096-5119 */
        Gicd_SetIpriorityr(Gicv3DrvData->GicdBase, Id, Priority);
    }
}

/*******************************************************************************
 * This function sets the interrupt triggered mode as supplied for the given interrupt
 * id.
 ******************************************************************************/
AL_VOID AlGicv3_SetInterruptTriggerMode(AL_U32 Id, AL_U32 ProcNum, AL_U32 TriggerMode)
{
    AL_UINTPTR GicrBase;

    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    /* Check interrupt ID */
    if (AlGicv3_IsSgiPpi(Id)) {
        /* For SGIs: 0-15, PPIs: 16-31 and EPPIs: 1056-1119 */
        GicrBase = Gicv3DrvData->RdistBaseAddrs[ProcNum];
        Gicr_SetIcfgr(GicrBase, Id, TriggerMode);
    } else {
        /* For SPIs: 32-1019 and ESPIs: 4096-5119 */
        Gicd_SetIcfgr(Gicv3DrvData->GicdBase, Id, TriggerMode);
    }
}

/*******************************************************************************
 * This function assigns group for the interrupt identified by id. The ProcNum
 * is used if the interrupt is SGI or (E)PPI, and programs the corresponding
 * Redistributor interface. The group can be any of GICV3_INTR_GROUP*
 ******************************************************************************/
AL_VOID AlGicv3_SetInterruptType(AL_U32 Id, AL_U32 ProcNum, AL_U32 Type)
{
    AL_BOOL Igroup = false, Grpmod = false;
    AL_UINTPTR GicrBase;

    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    switch (Type) {
    case INTR_GROUP1S:
        Igroup = false;
        Grpmod = true;
        break;
    case INTR_GROUP0:
        Igroup = false;
        Grpmod = false;
        break;
    case INTR_GROUP1NS:
        Igroup = true;
        Grpmod = false;
        break;
    default:
        assert(false);
        break;
    }

    /* Check interrupt ID */
    if (AlGicv3_IsSgiPpi(Id)) {
        /* For SGIs: 0-15, PPIs: 16-31 and EPPIs: 1056-1119 */
        GicrBase = Gicv3DrvData->RdistBaseAddrs[ProcNum];

        Igroup ? Gicr_SetIgroupr(GicrBase, Id) :
             Gicr_ClrIgroupr(GicrBase, Id);
        Grpmod ? Gicr_SetIgrpmodr(GicrBase, Id) :
             Gicr_ClrIgrpmodr(GicrBase, Id);
    } else {
        /* For SPIs: 32-1019 and ESPIs: 4096-5119 */
        Igroup ? Gicd_SetIgroupr(Gicv3DrvData->GicdBase, Id) :
             Gicd_ClrIgroupr(Gicv3DrvData->GicdBase, Id);
        Grpmod ? Gicd_SetIgrpmodr(Gicv3DrvData->GicdBase, Id) :
             Gicd_ClrIgrpmodr(Gicv3DrvData->GicdBase, Id);
    }
}

/*******************************************************************************
 * This function raises the specified SGI of the specified group.
 *
 * The target parameter must be a valid MPIDR in the system.
 ******************************************************************************/
AL_VOID AlGicv3_RaiseSgi(AL_U32 SgiNum, AL_GICV3_IrqGroupEnum Group, AL_REG Target)
{
    AL_U32 Tgt, Aff3, Aff2, Aff1, Aff0;
    AL_U64 SgiVal;

    /* Verify interrupt number is in the SGI range */
    assert((SgiNum >= MIN_SGI_ID) && (SgiNum < MIN_PPI_ID));

    /* Extract affinity fields from target */
    Aff0 = MPIDR_AFFLVL0_VAL(Target);
    Aff1 = MPIDR_AFFLVL1_VAL(Target);
    Aff2 = MPIDR_AFFLVL2_VAL(Target);
    Aff3 = MPIDR_AFFLVL3_VAL(Target);

    /*
     * Make target list from affinity 0, and ensure GICv3 SGI can target
     * this PE.
     */
    assert(Aff0 < GICV3_MAX_SGI_TARGETS);
    Tgt = BIT_32(Aff0);

    /* Raise SGI to PE specified by its affinity */
    SgiVal = GICV3_SGIR_VALUE(Aff3, Aff2, Aff1, SgiNum, SGIR_IRM_TO_AFF,
            Tgt);

    /*
     * Ensure that any shared variable updates depending on out of band
     * interrupt trigger are observed before raising SGI.
     */
    DSB();

    switch (Group) {
    case AL_GICV3_G0:
        ARCH_SYSREG_WRITE(icc_sgi0r_el1, SgiVal);
        break;
    case AL_GICV3_G1NS:
        #ifdef SUPPORT_NONSECURE
        ARCH_SYSREG_WRITE(icc_sgi1r, SgiVal);
        #else
        ARCH_SYSREG_WRITE(icc_asgi1r, SgiVal);
        #endif
        break;
    case AL_GICV3_G1S:
        #ifdef SUPPORT_NONSECURE
        ARCH_SYSREG_WRITE(icc_asgi1r, SgiVal);
        #else
        ARCH_SYSREG_WRITE(icc_sgi1r, SgiVal);
        #endif
        break;
    default:
        assert(false);
        break;
    }

    ISB();
}

/*******************************************************************************
 * This function sets the interrupt routing for the given (E)SPI interrupt id.
 * The interrupt routing is specified in routing mode and mpidr.
 *
 * The routing mode can be either of:
 *  - GICV3_IRM_ANY
 *  - GICV3_IRM_PE
 *
 * The mpidr is the affinity of the PE to which the interrupt will be routed,
 * and is ignored for routing mode GICV3_IRM_ANY.
 ******************************************************************************/
AL_VOID AlGicv3_SetSpiRouting(AL_U32 Id, AL_U32 Irm, AL_REG Mpidr)
{
    unsigned long long Aff;

    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);

    assert((Irm == GICV3_IRM_ANY) || (Irm == GICV3_IRM_PE));

    assert(IS_SPI(Id));

    Aff = Gicd_IrouterValFromMpidr(Mpidr, Irm);
    Gicd_WriteIrouter(Gicv3DrvData->GicdBase, Id, Aff);
}

/*******************************************************************************
 * This function clears the pending status of an interrupt identified by id.
 * The ProcNum is used if the interrupt is SGI or (E)PPI, and programs the
 * corresponding Redistributor interface.
 ******************************************************************************/
AL_VOID AlGicv3_ClearInterruptPending(AL_U32 Id, AL_U32 ProcNum)
{
    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    /*
     * Clear pending interrupt, and ensure that any shared variable updates
     * depending on out of band interrupt trigger are observed afterwards.
     */

    /* Check interrupt ID */
    if (AlGicv3_IsSgiPpi(Id)) {
        /* For SGIs: 0-15, PPIs: 16-31 and EPPIs: 1056-1119 */
        Gicr_SetIcpendr(
            Gicv3DrvData->RdistBaseAddrs[ProcNum], Id);
    } else {
        /* For SPIs: 32-1019 and ESPIs: 4096-5119 */
        Gicd_SetIcpendr(Gicv3DrvData->GicdBase, Id);
    }

    DSB();
}

/*******************************************************************************
 * This function sets the pending status of an interrupt identified by id.
 * The ProcNum is used if the interrupt is SGI or PPI and programs the
 * corresponding Redistributor interface.
 ******************************************************************************/
AL_VOID AlGicv3_SetInterruptPending(AL_U32 Id, AL_U32 ProcNum)
{
    assert(Gicv3DrvData != NULL);
    assert(Gicv3DrvData->GicdBase != 0U);
    assert(ProcNum < Gicv3DrvData->RdistNum);
    assert(Gicv3DrvData->RdistBaseAddrs != NULL);

    /*
     * Ensure that any shared variable updates depending on out of band
     * interrupt trigger are observed before setting interrupt pending.
     */
    DSB();

    /* Check interrupt ID */
    if (AlGicv3_IsSgiPpi(Id)) {
        /* For SGIs: 0-15, PPIs: 16-31 and EPPIs: 1056-1119 */
        Gicr_SetIspendr(
            Gicv3DrvData->RdistBaseAddrs[ProcNum], Id);
    } else {
        /* For SPIs: 32-1019 and ESPIs: 4096-5119 */
        Gicd_SetIspendr(Gicv3DrvData->GicdBase, Id);
    }
}

/*******************************************************************************
 * This function sets the PMR register with the supplied value. Returns the
 * original PMR.
 ******************************************************************************/
AL_U32 AlGicv3_SetPmr(AL_U32 Mask)
{
    AL_U32 OldMask;

    OldMask = (AL_U32)ARCH_SYSREG_READ(icc_pmr_el1);

    /*
     * Order memory updates w.r.t. PMR write, and ensure they're visible
     * before potential out of band interrupt trigger because of PMR update.
     * PMR system register writes are self-synchronizing, so no ISB required
     * thereafter.
     */
    DSB();
    ARCH_SYSREG_WRITE(icc_pmr_el1, Mask);

    return OldMask;
}

/******************************************************************************
 * This function checks the interrupt ID and returns true for SGIs and (E)PPIs
 * and false for (E)SPIs IDs.
 *****************************************************************************/
static AL_BOOL AlGicv3_IsSgiPpi(AL_U32 Id)
{
    if (IS_SGI_PPI(Id)) {       /* SGIs: 0-15, PPIs: 16-31, EPPIs: 1056-1119 */
        return true;
    } else if (IS_SPI(Id)) {    /* SPIs: 32-1019, ESPIs: 4096-5119 */
        return false;
    } else {
        return false;
    }
}
