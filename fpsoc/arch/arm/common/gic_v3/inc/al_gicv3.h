/*
 * Copyright (c) 2015-2022, Arm Limited and Contributors. All rights reserved.
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_GICV3_H_
#define __AL_GICV3_H_

#include <stdbool.h>
#include <stdint.h>

#include "al_aarch64_core.h"
#include "al_utils_def.h"
#include "al_type.h"
#include "al_gicv3_common.h"

/*******************************************************************************
 * GICv3 and 3.1 miscellaneous definitions
 ******************************************************************************/
/* Interrupt Group definitions */
#define INTR_GROUP1S    U(0)
#define INTR_GROUP0     U(1)
#define INTR_GROUP1NS   U(2)

/* Interrupt IDs reported by the HPPIR and IAR registers */
#define PENDING_G1S_INTID   U(1020)
#define PENDING_G1NS_INTID  U(1021)

/* Constant to categorize LPI interrupt */
#define MIN_LPI_ID  U(8192)

/* GICv3 can only target up to 16 PEs with SGI */
#define GICV3_MAX_SGI_TARGETS   U(16)

/* PPIs INTIDs 16-31 */
#define MAX_PPI_ID  U(31)

/* Total number of GICv3 PPIs */
#define TOTAL_PRIVATE_INTR_NUM  TOTAL_PCPU_INTR_NUM

/* Total number of GICv3 SPIs */
#define TOTAL_SHARED_INTR_NUM   TOTAL_SPI_INTR_NUM

/* SGIs: 0-15 */
#define IS_SGI(Id)      ((Id) <= GICV3_MAX_SGI_TARGETS)

/* PPIs: 16-31 */
#define IS_PPI(Id)      (((Id) <= MAX_PPI_ID) && ((Id) > GICV3_MAX_SGI_TARGETS))

/* SGIs: 0-15, PPIs: 16-31 */
#define IS_SGI_PPI(Id)  ((Id) <= MAX_PPI_ID)

/* SPIs: 32-1019 */
#define IS_SPI(Id)      (((Id) >= MIN_SPI_ID) && ((Id) <= MAX_SPI_ID))

#define GIC_REV(r, p)   ((r << 4) | p)

/*******************************************************************************
 * GICv3 and 3.1 specific Distributor interface register offsets and constants
 ******************************************************************************/
#define GICD_TYPER2         U(0x0c)
#define GICD_STATUSR        U(0x10)
#define GICD_SETSPI_NSR     U(0x40)
#define GICD_CLRSPI_NSR     U(0x48)
#define GICD_SETSPI_SR      U(0x50)
#define GICD_CLRSPI_SR      U(0x58)
#define GICD_IGRPMODR       U(0xd00)
#define GICD_IGROUPRE       U(0x1000)
#define GICD_ISENABLERE     U(0x1200)
#define GICD_ICENABLERE     U(0x1400)
#define GICD_ISPENDRE       U(0x1600)
#define GICD_ICPENDRE       U(0x1800)
#define GICD_ISACTIVERE     U(0x1a00)
#define GICD_ICACTIVERE     U(0x1c00)
#define GICD_IPRIORITYRE    U(0x2000)
#define GICD_ICFGRE         U(0x3000)
#define GICD_IGRPMODRE      U(0x3400)
#define GICD_NSACRE         U(0x3600)
/*
 * GICD_IROUTER<n> register is at 0x6000 + 8n, where n is the interrupt ID
 * and n >= 32, making the effective offset as 0x6100
 */
#define GICD_IROUTER        U(0x6000)
#define GICD_IROUTERE       U(0x8000)

#define GICD_PIDR0_GICV3    U(0xffe0)
#define GICD_PIDR1_GICV3    U(0xffe4)
#define GICD_PIDR2_GICV3    U(0xffe8)

#define IGRPMODR_SHIFT          5

/* GICD_CTLR bit definitions */
#define CTLR_ENABLE_G1NS_SHIFT  1
#define CTLR_ENABLE_G1S_SHIFT   2
#define CTLR_ARE_S_SHIFT        4
#define CTLR_ARE_NS_SHIFT       5
#define CTLR_DS_SHIFT           6
#define CTLR_E1NWF_SHIFT        7
#define GICD_CTLR_RWP_SHIFT     31

#define CTLR_ENABLE_G1NS_MASK   U(0x1)
#define CTLR_ENABLE_G1S_MASK    U(0x1)
#define CTLR_ARE_S_MASK         U(0x1)
#define CTLR_ARE_NS_MASK        U(0x1)
#define CTLR_DS_MASK            U(0x1)
#define CTLR_E1NWF_MASK         U(0x1)
#define GICD_CTLR_RWP_MASK      U(0x1)

#define CTLR_ENABLE_G1NS_BIT        BIT_32(CTLR_ENABLE_G1NS_SHIFT)
#define CTLR_ENABLE_G1S_BIT        BIT_32(CTLR_ENABLE_G1S_SHIFT)
#define CTLR_ARE_S_BIT            BIT_32(CTLR_ARE_S_SHIFT)
#define CTLR_ARE_NS_BIT            BIT_32(CTLR_ARE_NS_SHIFT)
#define CTLR_DS_BIT            BIT_32(CTLR_DS_SHIFT)
#define CTLR_E1NWF_BIT            BIT_32(CTLR_E1NWF_SHIFT)
#define GICD_CTLR_RWP_BIT        BIT_32(GICD_CTLR_RWP_SHIFT)

/* GICD_IROUTER shifts and masks */
#define IROUTER_SHIFT       0
#define IROUTER_IRM_SHIFT   31
#define IROUTER_IRM_MASK    U(0x1)

#define GICV3_IRM_PE        U(0)
#define GICV3_IRM_ANY       U(1)

#define NUM_OF_DIST_REGS    30

/* GICD_TYPER shifts and masks */
#define    TYPER_ESPI               U(1 << 8)
#define    TYPER_DVIS               U(1 << 18)
#define    TYPER_ESPI_RANGE_MASK    U(0x1f)
#define    TYPER_ESPI_RANGE_SHIFT   U(27)
#define    TYPER_ESPI_RANGE         U(TYPER_ESPI_MASK << TYPER_ESPI_SHIFT)

/*******************************************************************************
 * Common GIC Redistributor interface registers & constants
 ******************************************************************************/
#define GICR_V4_PCPUBASE_SHIFT  0x12
#define GICR_V3_PCPUBASE_SHIFT  0x11
#define GICR_SGIBASE_OFFSET     U(65536)    /* 64 KB */
#define GICR_CTLR               U(0x0)
#define GICR_IIDR               U(0x04)
#define GICR_TYPER              U(0x08)
#define GICR_STATUSR            U(0x10)
#define GICR_WAKER              U(0x14)
#define GICR_PROPBASER          U(0x70)
#define GICR_PENDBASER          U(0x78)
#define GICR_IGROUPR0           (GICR_SGIBASE_OFFSET + U(0x80))
#define GICR_ISENABLER0         (GICR_SGIBASE_OFFSET + U(0x100))
#define GICR_ICENABLER0         (GICR_SGIBASE_OFFSET + U(0x180))
#define GICR_ISPENDR0           (GICR_SGIBASE_OFFSET + U(0x200))
#define GICR_ICPENDR0           (GICR_SGIBASE_OFFSET + U(0x280))
#define GICR_ISACTIVER0         (GICR_SGIBASE_OFFSET + U(0x300))
#define GICR_ICACTIVER0         (GICR_SGIBASE_OFFSET + U(0x380))
#define GICR_IPRIORITYR         (GICR_SGIBASE_OFFSET + U(0x400))
#define GICR_ICFGR0             (GICR_SGIBASE_OFFSET + U(0xc00))
#define GICR_ICFGR1             (GICR_SGIBASE_OFFSET + U(0xc04))
#define GICR_IGRPMODR0          (GICR_SGIBASE_OFFSET + U(0xd00))
#define GICR_NSACR              (GICR_SGIBASE_OFFSET + U(0xe00))

#define GICR_IGROUPR            GICR_IGROUPR0
#define GICR_ISENABLER          GICR_ISENABLER0
#define GICR_ICENABLER          GICR_ICENABLER0
#define GICR_ISPENDR            GICR_ISPENDR0
#define GICR_ICPENDR            GICR_ICPENDR0
#define GICR_ISACTIVER          GICR_ISACTIVER0
#define GICR_ICACTIVER          GICR_ICACTIVER0
#define GICR_ICFGR              GICR_ICFGR0
#define GICR_IGRPMODR           GICR_IGRPMODR0

/* GICR_CTLR bit definitions */
#define GICR_CTLR_UWP_SHIFT     31
#define GICR_CTLR_UWP_MASK      U(0x1)
#define GICR_CTLR_UWP_BIT       BIT_32(GICR_CTLR_UWP_SHIFT)
#define GICR_CTLR_DPG1S_SHIFT   26
#define GICR_CTLR_DPG1S_MASK    U(0x1)
#define GICR_CTLR_DPG1S_BIT     BIT_32(GICR_CTLR_DPG1S_SHIFT)
#define GICR_CTLR_DPG1NS_SHIFT  25
#define GICR_CTLR_DPG1NS_MASK   U(0x1)
#define GICR_CTLR_DPG1NS_BIT    BIT_32(GICR_CTLR_DPG1NS_SHIFT)
#define GICR_CTLR_DPG0_SHIFT    24
#define GICR_CTLR_DPG0_MASK     U(0x1)
#define GICR_CTLR_DPG0_BIT      BIT_32(GICR_CTLR_DPG0_SHIFT)
#define GICR_CTLR_RWP_SHIFT     3
#define GICR_CTLR_RWP_MASK      U(0x1)
#define GICR_CTLR_RWP_BIT       BIT_32(GICR_CTLR_RWP_SHIFT)
#define GICR_CTLR_EN_LPIS_BIT   BIT_32(0)

/* GICR_WAKER bit definitions */
#define WAKER_CA_SHIFT          2
#define WAKER_PS_SHIFT          1

#define WAKER_CA_MASK           U(0x1)
#define WAKER_PS_MASK           U(0x1)

#define WAKER_CA_BIT            BIT_32(WAKER_CA_SHIFT)
#define WAKER_PS_BIT            BIT_32(WAKER_PS_SHIFT)

/* GICR_TYPER bit definitions */
#define TYPER_AFF_VAL_SHIFT     32
#define TYPER_PROC_NUM_SHIFT    8
#define TYPER_LAST_SHIFT        4
#define TYPER_VLPI_SHIFT        1

#define TYPER_AFF_VAL_MASK      U(0xffffffff)
#define TYPER_PROC_NUM_MASK     U(0xffff)
#define TYPER_LAST_MASK         U(0x1)

#define TYPER_LAST_BIT          BIT_32(TYPER_LAST_SHIFT)
#define TYPER_VLPI_BIT          BIT_32(TYPER_VLPI_SHIFT)

#define TYPER_PPI_NUM_SHIFT     U(27)
#define TYPER_PPI_NUM_MASK      U(0x1f)

/* GICR_IIDR bit definitions */
#define IIDR_PRODUCT_ID_MASK    U(0xff)
#define IIDR_VARIANT_MASK       U(0xf)
#define IIDR_REV_MASK           U(0xf)
#define IIDR_IMPLEMENTER_MASK   U(0xfff)
#define IIDR_PRODUCT_ID_SHIFT   24
#define IIDR_VARIANT_SHIFT      16
#define IIDR_REV_SHIFT          12
#define IIDR_IMPLEMENTER_SHIFT  0
#define IIDR_PRODUCT_ID_BIT     BIT_32(IIDR_PRODUCT_ID_SHIFT)
#define IIDR_VARIANT_BIT        BIT_32(IIDR_VARIANT_SHIFT)
#define IIDR_REV_BIT            BIT_32(IIDR_REVISION_SHIFT)
#define IIDR_IMPLEMENTER_BIT    BIT_32(IIDR_IMPLEMENTER_SHIFT)

#define IIDR_MODEL_MASK         (IIDR_PRODUCT_ID_MASK << IIDR_PRODUCT_ID_SHIFT | \
                                 IIDR_IMPLEMENTER_MASK << IIDR_IMPLEMENTER_SHIFT)

/*******************************************************************************
 * GICv3 and 3.1 CPU interface registers & constants
 ******************************************************************************/
/* ICC_SRE bit definitions */
#define ICC_SRE_EN_BIT          BIT_32(3)
#define ICC_SRE_DIB_BIT         BIT_32(2)
#define ICC_SRE_DFB_BIT         BIT_32(1)
#define ICC_SRE_SRE_BIT         BIT_32(0)

/* ICC_IGRPEN1_EL3 bit definitions */
#define IGRPEN1_EL3_ENABLE_G1NS_SHIFT   0
#define IGRPEN1_EL3_ENABLE_G1S_SHIFT    1

#define IGRPEN1_EL3_ENABLE_G1NS_BIT     BIT_32(IGRPEN1_EL3_ENABLE_G1NS_SHIFT)
#define IGRPEN1_EL3_ENABLE_G1S_BIT      BIT_32(IGRPEN1_EL3_ENABLE_G1S_SHIFT)

/* ICC_IGRPEN0_EL1 bit definitions */
#define IGRPEN1_EL1_ENABLE_G0_SHIFT     0
#define IGRPEN1_EL1_ENABLE_G0_BIT       BIT_32(IGRPEN1_EL1_ENABLE_G0_SHIFT)

/* ICC_HPPIR0_EL1 bit definitions */
#define HPPIR0_EL1_INTID_SHIFT          0
#define HPPIR0_EL1_INTID_MASK           U(0xffffff)

/* ICC_HPPIR1_EL1 bit definitions */
#define HPPIR1_EL1_INTID_SHIFT          0
#define HPPIR1_EL1_INTID_MASK           U(0xffffff)

/* ICC_IAR0_EL1 bit definitions */
#define IAR0_EL1_INTID_SHIFT            0
#define IAR0_EL1_INTID_MASK             U(0xffffff)

/* ICC_IAR1_EL1 bit definitions */
#define IAR1_EL1_INTID_SHIFT            0
#define IAR1_EL1_INTID_MASK             U(0xffffff)

/* ICC SGI macros */
#define SGIR_TGT_MASK               ULL(0xffff)
#define SGIR_AFF1_SHIFT             16
#define SGIR_INTID_SHIFT            24
#define SGIR_INTID_MASK             ULL(0xf)
#define SGIR_AFF2_SHIFT             32
#define SGIR_IRM_SHIFT              40
#define SGIR_IRM_MASK               ULL(0x1)
#define SGIR_AFF3_SHIFT             48
#define SGIR_AFF_MASK               ULL(0xff)

#define SGIR_IRM_TO_AFF             U(0)

#define GICV3_SGIR_VALUE(_aff3, _aff2, _aff1, _intid, _irm, _tgt) \
    ((((AL_U64) (_aff3) & SGIR_AFF_MASK) << SGIR_AFF3_SHIFT) | \
     (((AL_U64) (_irm) & SGIR_IRM_MASK) << SGIR_IRM_SHIFT) | \
     (((AL_U64) (_aff2) & SGIR_AFF_MASK) << SGIR_AFF2_SHIFT) | \
     (((_intid) & SGIR_INTID_MASK) << SGIR_INTID_SHIFT) | \
     (((_aff1) & SGIR_AFF_MASK) << SGIR_AFF1_SHIFT) | \
     ((_tgt) & SGIR_TGT_MASK))


typedef enum {
    AL_GICV3_G1S,
    AL_GICV3_G1NS,
    AL_GICV3_G0
} AL_GICV3_IrqGroupEnum;

static inline AL_UINTPTR AlGicv3_RedistSize(AL_VOID)
{
    return 1U << GICR_V3_PCPUBASE_SHIFT;
}

static inline bool AlGicv3_IsIntrIdSpecialIdentifier(AL_U32 Id)
{
    return (Id >= PENDING_G1S_INTID) && (Id <= GIC_SPURIOUS_INTERRUPT);
}

/*******************************************************************************
 * Helper GICv3 and 3.1 macros for SEL1
 ******************************************************************************/
static inline AL_U32 AlGicv3_AckIntrSel1(AL_VOID)
{
    return (AL_U32)ARCH_SYSREG_READ(icc_iar1_el1) & IAR1_EL1_INTID_MASK;
}

static inline AL_U32 AlGicv3_GetPendingIntrIdSel1(AL_VOID)
{
    return (AL_U32)ARCH_SYSREG_READ(icc_hppir1_el1) & HPPIR1_EL1_INTID_MASK;
}

static inline AL_VOID AlGicv3_EndOfIntrSel1(AL_U32 Id)
{
    /*
     * Interrupt request deassertion from peripheral to GIC happens
     * by clearing interrupt condition by a write to the peripheral
     * register. It is desired that the write transfer is complete
     * before the core tries to change GIC state from 'AP/Active' to
     * a new state on seeing 'EOI write'.
     * Since ICC interface writes are not ordered against Device
     * memory writes, a barrier is required to ensure the ordering.
     * The dsb will also ensure *completion* of previous writes with
     * DEVICE nGnRnE attribute.
     */
    DSB();
    ARCH_SYSREG_WRITE(icc_eoir1_el1, Id);
}

/*******************************************************************************
 * Helper GICv3 macros for EL3
 ******************************************************************************/
static inline AL_U32 AlGicv3_AckIntr(AL_VOID)
{
    return (AL_U32)ARCH_SYSREG_READ(icc_iar0_el1) & IAR0_EL1_INTID_MASK;
}

static inline AL_VOID AlGicv3_EndOfIntr(AL_U32 Id)
{
    /*
     * Interrupt request deassertion from peripheral to GIC happens
     * by clearing interrupt condition by a write to the peripheral
     * register. It is desired that the write transfer is complete
     * before the core tries to change GIC state from 'AP/Active' to
     * a new state on seeing 'EOI write'.
     * Since ICC interface writes are not ordered against Device
     * memory writes, a barrier is required to ensure the ordering.
     * The dsb will also ensure *completion* of previous writes with
     * DEVICE nGnRnE attribute.
     */
    ISB();
    return ARCH_SYSREG_WRITE(icc_eoir0_el1, Id);
}

/*
 * This macro returns the total number of GICD/GICR registers corresponding to
 * the register name
 */
#define GICD_NUM_REGS(RegName)  DIV_ROUND_UP_2EVAL(TOTAL_SHARED_INTR_NUM, (1 << RegName##_SHIFT))

#define GICR_NUM_REGS(RegName)  DIV_ROUND_UP_2EVAL(TOTAL_PRIVATE_INTR_NUM, (1 << RegName##_SHIFT))

/* Interrupt ID mask for HPPIR, AHPPIR, IAR and AIAR CPU Interface registers */
#define INT_ID_MASK     U(0xffffff)

/*******************************************************************************
 * This structure describes some of the implementation defined attributes of the
 * GICv3 IP. It is used by the platform port to specify these attributes in order
 * to initialise the GICV3 driver. The attributes are described below.
 *
 * The 'GicdBase' field contains the base address of the Distributor interface
 * programmer's view.
 *
 * The 'gicr_base' field contains the base address of the Re-distributor
 * interface programmer's view.
 *
 * The 'interrupt_props' field is a pointer to an array that enumerates secure
 * interrupts and their properties. If this field is not NULL, both
 * 'g0_interrupt_array' and 'g1s_interrupt_array' fields are ignored.
 *
 * The 'interrupt_props_num' field contains the number of entries in the
 * 'interrupt_props' array. If this field is non-zero, both 'g0_interrupt_num'
 * and 'g1s_interrupt_num' are ignored.
 *
 * The 'rdistif_num' field contains the number of Redistributor interfaces the
 * GIC implements. This is equal to the number of CPUs or CPU interfaces
 * instantiated in the GIC.
 *
 * The 'RdistBaseAddrs' field is a pointer to an array that has an entry for
 * storing the base address of the Redistributor interface frame of each CPU in
 * the system. The size of the array = 'rdistif_num'. The base addresses are
 * detected during driver initialisation.
 *
 * The 'MpidrToCorePos' field is a pointer to a hash function which the
 * driver will use to convert an MPIDR value to a linear core index. This index
 * will be used for accessing the 'RdistBaseAddrs' array. This is an
 * optional field. A GICv3 implementation maps each MPIDR to a linear core index
 * as well. This mapping can be found by reading the "Affinity Value" and
 * "Processor Number" fields in the GICR_TYPER. It is IMP. DEF. if the
 * "Processor Numbers" are suitable to index into an array to access core
 * specific information. If this not the case, the platform port must provide a
 * hash function. Otherwise, the "Processor Number" field will be used to access
 * the array elements.
 ******************************************************************************/
typedef AL_U32 (*MpidrHashFn)(AL_UINTPTR Mpidr);

typedef struct Gicv3DrvData {
    AL_U32          *CpuId;
    AL_UINTPTR      GicdBase;
    AL_UINTPTR      GicrBase;
    AL_U32          RdistNum;
    AL_UINTPTR      *RdistBaseAddrs;
    MpidrHashFn     MpidrToCorePos;
} AL_GICV3_DrvDataStruct;

/*******************************************************************************
 * GICv3 EL3 driver API
 ******************************************************************************/
AL_VOID AlGicv3_DriverInit(const AL_GICV3_DrvDataStruct *DrvData);
AL_VOID AlGicv3_DistInit(AL_VOID);
AL_VOID AlGicv3_DisableOwnSpiInterrupt(AL_VOID);
AL_VOID AlGicv3_RdistInit(AL_U32 ProcNum);
AL_VOID AlGicv3_CpuIfEnable(AL_U32 ProcNum);
AL_VOID AlGicv3_CpuIfDisable(AL_U32 ProcNum);
AL_U32 AlGicv3_GetPendingInterruptType(AL_VOID);
AL_U32 AlGicv3_GetPendingInterruptId(AL_VOID);
AL_U32 AlGicv3_GetInterruptType(AL_U32 Id, AL_U32 ProcNum);
AL_U32 AlGicv3_GetRunningPriority(AL_VOID);
AL_U32 AlGicv3_GetInterruptActive(AL_U32 Id, AL_U32 ProcNum);
AL_VOID AlGicv3_EnableInterrupt(AL_U32 Id, AL_U32 ProcNum);
AL_VOID AlGicv3_DisableInterrupt(AL_U32 Id, AL_U32 ProcNum);
AL_VOID AlGicv3_SetInterruptPriority(AL_U32 Id, AL_U32 ProcNum, AL_U32 Priority);
AL_VOID AlGicv3_SetInterruptTriggerMode(AL_U32 Id, AL_U32 ProcNum, AL_U32 TriggerMode);
AL_VOID AlGicv3_SetInterruptType(AL_U32 Id, AL_U32 ProcNum, AL_U32 type);
AL_VOID AlGicv3_RaiseSgi(AL_U32 SgiNum, AL_GICV3_IrqGroupEnum Group, AL_REG Target);
AL_VOID AlGicv3_SetSpiRouting(AL_U32 Id, AL_U32 Irm, AL_REG Mpidr);
AL_VOID AlGicv3_SetInterruptPending(AL_U32 Id, AL_U32 ProcNum);
AL_VOID AlGicv3_ClearInterruptPending(AL_U32 Id, AL_U32 ProcNum);
AL_U32 AlGicv3_SetPmr(AL_U32 Mask);
AL_VOID AlGicv3_GetComponentProdidRev(const AL_UINTPTR GicdBase, AL_U32 *GicProdId, AL_U8 *GicRev);

#endif /* AL_GICV3_H */
