/*
 * Copyright (c) 2015-2022, Arm Limited and Contributors. All rights reserved.
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include "al_gicv3_private.h"
#include "al_gicv3_dist.h"
#include "al_gicv3_rdist.h"
#include "al_gicv3.h"

/******************************************************************************
 * This function marks the core as awake in the re-distributor and
 * ensures that the interface is active.
 *****************************************************************************/
AL_VOID AlGicv3_Rdist_MarkCoreAwake(AL_UINTPTR GicrBase)
{
    /*
     * The WAKER_PS_BIT should be changed to 0
     * only when WAKER_CA_BIT is 1.
     */
    if ((Gicr_ReadWaker(GicrBase) & WAKER_CA_BIT) != 0U) {
        /* Mark the connected core as awake */
        Gicr_WriteWaker(GicrBase, Gicr_ReadWaker(GicrBase) & ~WAKER_PS_BIT);

        /* Wait till the WAKER_CA_BIT changes to 0 */
        while ((Gicr_ReadWaker(GicrBase) & WAKER_CA_BIT) != 0U) {
        }
    }
}

/******************************************************************************
 * This function marks the core as asleep in the re-distributor and ensures
 * that the interface is quiescent.
 *****************************************************************************/
AL_VOID AlGicv3_Rdist_MarkCoreAsleep(AL_UINTPTR GicrBase)
{
    /* Mark the connected core as asleep */
    Gicr_WriteWaker(GicrBase, Gicr_ReadWaker(GicrBase) | WAKER_PS_BIT);

    /* Wait till the WAKER_CA_BIT changes to 1 */
    while ((Gicr_ReadWaker(GicrBase) & WAKER_CA_BIT) == 0U) {
    }
}

/*******************************************************************************
 * This function probes the Redistributor frames when the driver is initialised
 * and saves their base addresses. These base addresses are used later to
 * initialise each Redistributor interface.
 ******************************************************************************/
AL_VOID AlGicv3_Rdist_BaseAddrsProbe(AL_UINTPTR *RdistBaseAddrs, AL_U32 RdistNum, AL_UINTPTR GicrBase,
                                     MpidrHashFn MpidrToCorePos)
{
    AL_REG Mpdir;
    AL_U32 ProcNum;
    AL_U64 TyperVal;
    AL_UINTPTR RdistifBase = GicrBase;

    assert(RdistBaseAddrs != NULL);

    /*
     * Iterate over the Redistributor frames. Store the base address of each
     * frame in the platform provided array. Use the "Processor Number"
     * field to index into the array if the platform has not provided a hash
     * function to convert an MPIDR (obtained from the "Affinity Value"
     * field into a linear index.
     */
    do {
        TyperVal = Gicr_ReadTyper(RdistifBase);
        if (MpidrToCorePos != NULL) {
            Mpdir = MpidrFromGicrTyper(TyperVal);
            ProcNum = MpidrToCorePos(Mpdir);
        } else {
            ProcNum = (TyperVal >> TYPER_PROC_NUM_SHIFT) &
                TYPER_PROC_NUM_MASK;
        }

        if (ProcNum < RdistNum) {
            RdistBaseAddrs[ProcNum] = RdistifBase;
        }
        RdistifBase += AlGicv3_RedistSize();
    } while ((TyperVal & TYPER_LAST_BIT) == 0U);
}

/*******************************************************************************
 * Helper function to get the maximum SPI INTID + 1.
 ******************************************************************************/
AL_U32 AlGicv3_GetSpiLimit(AL_UINTPTR GicdBase)
{
    AL_U32 SpiLimit;
    AL_U32 typer_reg = Gicd_ReadTyper(GicdBase);

    /* (maximum SPI INTID + 1) is equal to 32 * (GICD_TYPER.ITLinesNumber+1) */
    SpiLimit = ((typer_reg & TYPER_IT_LINES_NO_MASK) + 1U) << 5;

    /* Filter out special INTIDs 1020-1023 */
    if (SpiLimit > (MAX_SPI_ID + 1U)) {
        return MAX_SPI_ID + 1U;
    }

    return SpiLimit;
}

/*******************************************************************************
 * Helper function to configure the default attributes of (E)SPIs.
 ******************************************************************************/
AL_VOID AlGicv3_SpisConfigDefaults(AL_UINTPTR GicdBase)
{
    AL_U32 i, NumInts;

    NumInts = AlGicv3_GetSpiLimit(GicdBase);

    /* Complete all interrupt */
    for (i = MIN_SPI_ID; i < NumInts; i++) {
        AlGicv3_EndOfIntrSel1(i);
    }

    /* Treat all (E)SPIs as G1NS by default. We do 32 at a time. */
    for (i = MIN_SPI_ID; i < NumInts; i += (1U << IGROUPR_SHIFT)) {
        #ifdef SUPPORT_NONSECURE
        Gicd_WriteIgroupr(GicdBase, i, ~0U);
        Gicd_WriteIgrpmodr(GicdBase, i, 0U);
        #else
        Gicd_WriteIgroupr(GicdBase, i, 0U);
        Gicd_WriteIgrpmodr(GicdBase, i, ~0U);
        #endif
    }

    /* Setup the default (E)SPI priorities doing four at a time */
    for (i = MIN_SPI_ID; i < NumInts; i += (1U << IPRIORITYR_SHIFT)) {
        Gicd_WriteIpriorityr(GicdBase, i, GICD_IPRIORITYR_DEF_VAL);
    }

    /*
     * Treat all (E)SPIs as level triggered by default, write 16 at a time
     */
    for (i = MIN_SPI_ID; i < NumInts; i += (1U << ICFGR_SHIFT)) {
        Gicd_WriteIcfgr(GicdBase, i, 0U);
    }

    /*
     * Treat all (E)SPIs as router by current cpu, write 16 at a time
     */
    for (i = MIN_SPI_ID; i < NumInts; i += (1U << IROUTER_SHIFT)) {
        Gicd_WriteIrouter(GicdBase, i, *(Gicv3DrvData->CpuId));
    }

    /* Disable all spi */
    for (i = MIN_SPI_ID; i < NumInts; i += (1U << ICENABLER_SHIFT)) {
        Gicd_WriteIcenabler(GicdBase, i, ~0U);
        Gicd_WriteIcactiver(GicdBase, i, ~0U);
        Gicd_WriteIcpendr(GicdBase, i, ~0U);
    }
}

/*******************************************************************************
 * Helper function to configure the default attributes of (E)PPIs/SGIs
 ******************************************************************************/
AL_VOID AlGicv3_PpiSgiConfigDefaults(AL_UINTPTR GicrBase)
{
    AL_U32 i, PpiRegsNum, RegsNum;

    PpiRegsNum = 1U;

    Gicr_WriteIcenabler0(GicrBase, ~0U);
    Gicr_WriteIcpendr0(GicrBase, ~0U);

    /* Wait for pending writes to GICR_ICENABLER */
    Gicr_WaitForPendingWrite(GicrBase);

    /* 32 interrupt IDs per GICR_IGROUPR register */
    #ifdef SUPPORT_NONSECURE
    Gicr_WriteIgroupr0(GicrBase, ~0U);
    Gicr_WriteIgrpmodr0(GicrBase, 0U);
    #else
    Gicr_WriteIgroupr0(GicrBase, 0U);
    Gicr_WriteIgrpmodr0(GicrBase, ~0U);
    /* Non-secure writes to are permitted to generate a Secure Group1 SGI */
    Gicr_WriteNsacr(GicrBase, 0xAAAAAAAA);
    #endif

    /* 4 interrupt IDs per GICR_IPRIORITYR register */
    RegsNum = PpiRegsNum << 3;
    for (i = 0U; i < RegsNum; ++i) {
        /* Setup the default (E)PPI/SGI priorities doing 4 at a time */
        Gicr_WriteIpriorityr(GicrBase, i << 2, GICD_IPRIORITYR_DEF_VAL);
    }

    /* 16 interrupt IDs per GICR_ICFGR register */
    RegsNum = PpiRegsNum << 1;
    for (i = (MIN_PPI_ID >> ICFGR_SHIFT); i < RegsNum; ++i) {
        /* Configure all (E)PPIs as level triggered by default */
        Gicr_WriteIcfgr(GicrBase, i, 0U);
    }
}

AL_U32 AlGicv3_GetComponentPartnum(const AL_UINTPTR GicFrame)
{
    AL_U32 PartId;

    /*
     * The lower 8 bits of PIDR0, complemented by the lower 4 bits of
     * PIDR1 contain a part number identifying the GIC component at a
     * particular base address.
     */
    PartId = AL_REG32_READ(GicFrame + GICD_PIDR0_GICV3) & 0xff;
    PartId |= (AL_REG32_READ(GicFrame + GICD_PIDR1_GICV3) << 8) & 0xf00;

    return PartId;
}

/*******************************************************************************
 * Helper function to return product ID and revision of GIC
 * @GicdBase:   base address of the GIC distributor
 * @GicProdId: retrieved product id of GIC
 * @GicRev:     retrieved revision of GIC
 ******************************************************************************/
AL_VOID AlGicv3_GetComponentProdidRev(const AL_UINTPTR GicdBase, AL_U32 *GicProdId, AL_U8 *GicRev)
{
    AL_U32 GicdIidr;
    AL_U8 GicVariant;

    GicdIidr = Gicd_ReadIidr(GicdBase);
    *GicProdId = GicdIidr >> IIDR_PRODUCT_ID_SHIFT;
    *GicProdId &= IIDR_PRODUCT_ID_MASK;

    GicVariant = GicdIidr >> IIDR_VARIANT_SHIFT;
    GicVariant &= IIDR_VARIANT_MASK;

    *GicRev = GicdIidr >> IIDR_REV_SHIFT;
    *GicRev &= IIDR_REV_MASK;

    /*
     * pack gic variant and GicRev in 1 byte
     * GicRev = GicVariant[7:4] and GicRev[0:3]
     */
    *GicRev = *GicRev | GicVariant << 0x4;

}
