/*
 * Copyright (c) 2016-2018, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef GICV3_DIST_H
#define GICV3_DIST_H

#include <stdint.h>
#include "al_reg_io.h"
#include "al_gicv3_common.h"
#include "al_gicv3_private.h"
#include "al_gicv3.h"

static inline AL_U32 Gicd_ReadCtlr(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICD_CTLR);
}

static inline AL_VOID Gicd_WriteCtlr(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICD_CTLR, Val);
}

/*
 * Wait for updates to:
 * GICD_CTLR[2:0] - the Group Enables
 * GICD_CTLR[7:4] - the ARE bits, E1NWF bit and DS bit
 * GICD_ICENABLER<n> - the clearing of enable state for SPIs
 */
static inline AL_VOID Gicd_WaitForPendingWrite(AL_UINTPTR GicdBase)
{
    while ((Gicd_ReadCtlr(GicdBase) & GICD_CTLR_RWP_BIT) != 0U) {
    }
}

static inline AL_VOID Gicd_ClrCtlr(AL_UINTPTR Base, AL_U32 Bitmap, AL_U32 Rwp)
{
    Gicd_WriteCtlr(Base, Gicd_ReadCtlr(Base) & ~Bitmap);
    if (Rwp != 0U) {
        Gicd_WaitForPendingWrite(Base);
    }
}

static inline AL_VOID Gicd_SetCtlr(AL_UINTPTR Base, AL_U32 Bitmap, AL_U32 Rwp)
{
    Gicd_WriteCtlr(Base, Gicd_ReadCtlr(Base) | Bitmap);
    if (Rwp != 0U) {
        Gicd_WaitForPendingWrite(Base);
    }
}

static inline AL_U32 Gicd_ReadTyper(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICD_TYPER);
}

static inline AL_U32 Gicd_ReadIidr(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICD_IIDR);
}

/*
 * Accessors to read/write the GIC Distributor IGROUPR and IGROUPRE
 * corresponding to the interrupt ID, 32 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIgroupr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(IGROUP, Base, Id);
}

static inline AL_VOID Gicd_WriteIgroupr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(IGROUP, Base, Id, Val);
}

/*
 * Accessors to get/set/clear the bit corresponding to interrupt ID
 * in GIC Distributor IGROUPR and IGROUPRE.
 */
static inline AL_U32 Gicd_GetIgroupr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_GET_BIT(IGROUP, Base, Id);
}

static inline AL_VOID Gicd_SetIgroupr(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_SET_BIT(IGROUP, Base, Id);
}

static inline AL_VOID Gicd_ClrIgroupr(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_CLR_BIT(IGROUP, Base, Id);
}

/*
 * Accessors to read/write the GIC Distributor ISENABLER and ISENABLERE
 * corresponding to the interrupt ID, 32 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIsenabler(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(ISENABLE, Base, Id);
}

static inline AL_VOID Gicd_WriteIsenabler(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(ISENABLE, Base, Id, Val);
}

/*
 * Accessors to set the bit corresponding to interrupt ID
 * in GIC Distributor ISENABLER and ISENABLERE.
 */
static inline AL_VOID Gicd_SetIsenabler(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_WRITE_BIT(ISENABLE, Base, Id);
}

/*
 * Accessor to read/write the GIC Distributor ICENABLER corresponding to the
 * interrupt `Id`, 32 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIcenabler(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(ICENABLE, Base, Id);
}

static inline AL_VOID Gicd_WriteIcenabler(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(ICENABLE, Base, Id, Val);
}

/*
 * Accessors to set the bit corresponding to interrupt ID
 * in GIC Distributor ICENABLER and ICENABLERE.
 */
static inline AL_VOID Gicd_SetIcenabler(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_WRITE_BIT(ICENABLE, Base, Id);
}

/*
 * Accessors to read/write the GIC Distributor ISPENDR and ISPENDRE
 * corresponding to the interrupt ID, 32 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIspendr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(ISPEND, Base, Id);
}

static inline AL_VOID Gicd_WriteIspendr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(ISPEND, Base, Id, Val);
}

/*
 * Accessors to set the bit corresponding to interrupt ID
 * in GIC Distributor ISPENDR and ISPENDRE.
 */
static inline AL_VOID Gicd_SetIspendr(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_WRITE_BIT(ISPEND, Base, Id);
}

/*
 * Accessor to read/write the GIC Distributor ICPENDR corresponding to the
 * interrupt `Id`, 32 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIcpendr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(ICPEND, Base, Id);
}

static inline AL_VOID Gicd_WriteIcpendr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(ICPEND, Base, Id, Val);
}

/*
 * Accessors to set the bit corresponding to interrupt ID
 * in GIC Distributor ICPENDR and ICPENDRE.
 */
static inline AL_VOID Gicd_SetIcpendr(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_WRITE_BIT(ICPEND, Base, Id);
}

/*
 * Accessors to read/write the GIC Distributor ISACTIVER and ISACTIVERE
 * corresponding to the interrupt ID, 32 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIsactiver(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(ISACTIVE, Base, Id);
}

static inline AL_VOID Gicd_WriteIsactiver(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(ISACTIVE, Base, Id, Val);
}

/*
 * Accessors to get/set the bit corresponding to interrupt ID
 * in GIC Distributor ISACTIVER and ISACTIVERE.
 */
static inline AL_U32 Gicd_GetIsactiver(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_GET_BIT(ISACTIVE, Base, Id);
}

static inline AL_VOID Gicd_SetIsactiver(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_WRITE_BIT(ISACTIVE, Base, Id);
}

/*
 * Accessor to read the GIC Distributor ICACTIVER corresponding to the
 * interrupt `Id`, 32 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIcactiver(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(ICACTIVE, Base, Id);
}

static inline AL_VOID Gicd_WriteIcactiver(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(ICACTIVE, Base, Id, Val);
}

/*
 * Accessors to get/set the bit corresponding to interrupt ID
 * in GIC Distributor ICACTIVER and ICACTIVERE.
 */
static inline AL_U32 Gicd_GetIcactiver(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_GET_BIT(ICACTIVE, Base, Id);
}

static inline AL_VOID Gicd_SetIcactiver(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_WRITE_BIT(ICACTIVE, Base, Id);
}

/*
 * Accessors to read/write the GIC Distributor IPRIORITYR and IPRIORITYRE
 * corresponding to the interrupt ID, 4 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIpriorityr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(IPRIORITY, Base, Id);
}

static inline AL_VOID Gicd_WriteIpriorityr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(IPRIORITY, Base, Id, Val);
}

/*
 * Accessors to set the bit corresponding to interrupt ID
 * in GIC Distributor IPRIORITYR and IPRIORITYRE.
 */
static inline AL_VOID Gicd_SetIpriorityr(AL_UINTPTR Base, AL_U32 Id, AL_U32 pri)
{
    GICD_WRITE_8(IPRIORITY, Base, Id, (uint8_t)(pri & GIC_PRI_MASK));
}

/*
 * Accessors to read/write the GIC Distributor ICGFR and ICGFRE
 * corresponding to the interrupt ID, 16 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIcfgr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(ICFG, Base, Id);
}

static inline AL_VOID Gicd_WriteIcfgr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(ICFG, Base, Id, Val);
}

/*
 * Accessors to set the bits corresponding to interrupt ID
 * in GIC Distributor ICFGR and ICFGRE.
 */
static inline AL_VOID Gicd_SetIcfgr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Cfg)
{
    /* Interrupt configuration is a 2-bit field */
    AL_U32 bit_shift = BIT_NUM(ICFG, Id) << 1U;

    /* Clear the field, and insert required configuration */
    AL_REG32_SET_BITS(Base + GICD_OFFSET(ICFG, Id), bit_shift, GIC_CFG_MASK_SIZE, Cfg);
}

/*
 * Accessors to read/write the GIC Distributor IGRPMODR and IGRPMODRE
 * corresponding to the interrupt ID, 32 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadIgrpmodr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(IGRPMOD, Base, Id);
}

static inline AL_VOID Gicd_WriteIgrpmodr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(IGRPMOD, Base, Id, Val);
}

/*
 * Accessors to get/set/clear the bit corresponding to interrupt ID
 * in GIC Distributor IGRPMODR and IGRPMODRE.
 */
static inline AL_U32 Gicd_GetIgrpmodr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_GET_BIT(IGRPMOD, Base, Id);
}

static inline AL_VOID Gicd_SetIgrpmodr(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_SET_BIT(IGRPMOD, Base, Id);
}

static inline AL_VOID Gicd_ClrIgrpmodr(AL_UINTPTR Base, AL_U32 Id)
{
    GICD_CLR_BIT(IGRPMOD, Base, Id);
}

/*
 * Accessors to read/write the GIC Distributor NSACR and NSACRE
 * corresponding to the interrupt ID, 16 interrupt IDs at a time.
 */
static inline AL_U32 Gicd_ReadNsacr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICD_READ(NSAC, Base, Id);
}

static inline AL_VOID Gicd_WriteNsacr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICD_WRITE(NSAC, Base, Id, Val);
}

static inline AL_U64 Gicd_ReadIrouter(AL_UINTPTR Base, AL_U32 Id)
{
    assert(Id >= MIN_SPI_ID);
    return GICD_READ_64(IROUTE, Base, Id);
}

static inline AL_VOID Gicd_WriteIrouter(AL_UINTPTR Base, AL_U32 Id, AL_U64 Affinity)
{
    assert(Id >= MIN_SPI_ID);
    GICD_WRITE_64(IROUTE, Base, Id, Affinity);
}

/*
 * Macro to convert an mpidr to a value suitable for programming into a
 * GICD_IROUTER. Bits[31:24] in the MPIDR are cleared as they are not relevant
 * to GICv3.
 */
static inline AL_REG Gicd_IrouterValFromMpidr(AL_REG Mpidr, AL_U32 Irm)
{
    return (Mpidr & MPIDR_AFFINITY_MASK) |
        ((Irm & IROUTER_IRM_MASK) << IROUTER_IRM_SHIFT);
}

static inline AL_U32 Gicd_ReadPidr2(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICD_PIDR2_GICV3);
}

#endif /* GICV3_DIST_H */
