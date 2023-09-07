/*
 * Copyright (c) 2016-2018, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_GICV3_RDIST_H_
#define __AL_GICV3_RDIST_H_

#include <stdint.h>
#include "al_reg_io.h"
#include "al_gicv3_common.h"
#include "al_gicv3_private.h"
#include "al_gicv3.h"

/*******************************************************************************
 * GIC Redistributor interface accessors
 ******************************************************************************/
static inline AL_U32 Gicr_ReadCtlr(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_CTLR);
}

static inline AL_VOID Gicr_WriteCtlr(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_CTLR, Val);
}

/*
 * Wait for updates to:
 * GICR_ICENABLER0
 * GICR_CTLR.DPG1S
 * GICR_CTLR.DPG1NS
 * GICR_CTLR.DPG0
 * GICR_CTLR, which clears EnableLPIs from 1 to 0
 */
static inline AL_VOID Gicr_WaitForPendingWrite(AL_UINTPTR GicrBase)
{
    while ((Gicr_ReadCtlr(GicrBase) & GICR_CTLR_RWP_BIT) != 0U) {
    }
}

static inline AL_VOID Gicr_WaitForUpstreamPendingWrite(AL_UINTPTR GicrBase)
{
    while ((Gicr_ReadCtlr(GicrBase) & GICR_CTLR_UWP_BIT) != 0U) {
    }
}

static inline AL_U64 Gicr_ReadIidr(AL_UINTPTR Base)
{
    return AL_REG64_READ(Base + GICR_IIDR);
}

static inline AL_U64 Gicr_ReadTyper(AL_UINTPTR Base)
{
    return AL_REG64_READ(Base + GICR_TYPER);
}

static inline AL_U32 Gicr_ReadWaker(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_WAKER);
}

static inline AL_VOID Gicr_WriteWaker(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_WAKER, Val);
}

/*
 * Accessors to read/write GIC Redistributor PROPBASER register
 */
static inline AL_U64 Gicr_ReadPropbaser(AL_UINTPTR Base)
{
    return AL_REG64_READ(Base + GICR_PROPBASER);
}

static inline AL_VOID Gicr_WritePropbaser(AL_UINTPTR Base, AL_U64 Val)
{
    AL_REG64_WRITE(Base + GICR_PROPBASER, Val);
}

/*
 * Accessors to read/write GIC Redistributor PENDBASER register
 */
static inline AL_U64 Gicr_ReadPendbaser(AL_UINTPTR Base)
{
    return AL_REG64_READ(Base + GICR_PENDBASER);
}

static inline AL_VOID Gicr_WritePendbaser(AL_UINTPTR Base, AL_U64 Val)
{
    AL_REG64_WRITE(Base + GICR_PENDBASER, Val);
}

/*
 * Accessors to read/write GIC Redistributor IGROUPR0 register
 */
static inline AL_U32 Gicr_ReadIgroupr0(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_IGROUPR0);
}

static inline AL_VOID Gicr_WriteIgroupr0(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_IGROUPR0, Val);
}

/*
 * Accessors to get/set/clear the bit corresponding to interrupt `Id`
 * from GIC Redistributor IGROUPR0 and IGROUPRE
 */
static inline AL_U32 Gicr_GetIgroupr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICR_GET_BIT(IGROUP, Base, Id);
}

static inline AL_VOID Gicr_SetIgroupr(AL_UINTPTR Base, AL_U32 Id)
{
    GICR_SET_BIT(IGROUP, Base, Id);
}

static inline AL_VOID Gicr_ClrIgroupr(AL_UINTPTR Base, AL_U32 Id)
{
    GICR_CLR_BIT(IGROUP, Base, Id);
}

/*
 * Accessors to read/write GIC Redistributor ISENABLER0 register
 */
static inline AL_U32 Gicr_ReadIsenabler0(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_ISENABLER0);
}

static inline AL_VOID Gicr_WriteIsenabler0(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_ISENABLER0, Val);
}

/*
 * Accessor to write the bit corresponding to interrupt `Id`
 * in GIC Redistributor ISENABLER0 and ISENABLERE
 */
static inline AL_VOID Gicr_SetIsenabler(AL_UINTPTR Base, AL_U32 Id)
{
    GICR_WRITE_BIT(ISENABLE, Base, Id);
}

/*
 * Accessors to read/write GIC Redistributor ICENABLER0 register
 */
static inline AL_U32 Gicr_ReadIcenabler0(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_ICENABLER0);
}

static inline AL_VOID Gicr_WriteIcenabler0(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_ICENABLER0, Val);
}

/*
 * Accessor to write the bit corresponding to interrupt `Id`
 * in GIC Redistributor ICENABLER0 and ICENABLERE
 */
static inline AL_VOID Gicr_SetIcenabler(AL_UINTPTR Base, AL_U32 Id)
{
    GICR_WRITE_BIT(ICENABLE, Base, Id);
}

/*
 * Accessors to read/write GIC Redistributor ISPENDR0 register
 */
static inline AL_U32 Gicr_ReadIspendr0(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_ISPENDR0);
}

static inline AL_VOID Gicr_WriteIspendr0(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_ISPENDR0, Val);
}

/*
 * Accessor to write the bit corresponding to interrupt `Id`
 * in GIC Redistributor ISPENDR0 and ISPENDRE
 */
static inline AL_VOID Gicr_SetIspendr(AL_UINTPTR Base, AL_U32 Id)
{
    GICR_WRITE_BIT(ISPEND, Base, Id);
}

/*
 * Accessor to write GIC Redistributor ICPENDR0 register
 */
static inline AL_VOID Gicr_WriteIcpendr0(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_ICPENDR0, Val);
}

/*
 * Accessor to clear the bit corresponding to interrupt `Id`
 * in GIC Redistributor ICPENDR0 and ICPENDRE
 */
static inline AL_VOID Gicr_SetIcpendr(AL_UINTPTR Base, AL_U32 Id)
{
    GICR_WRITE_BIT(ICPEND, Base, Id);
}

/*
 * Accessors to read/write GIC Redistributor ISACTIVER0 register
 */
static inline AL_U32 Gicr_ReadIsactiver0(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_ISACTIVER0);
}

static inline AL_VOID Gicr_WriteIsactiver0(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_ISACTIVER0, Val);
}

/*
 * Accessor to get the bit corresponding to interrupt `Id`
 * in GIC Redistributor ISACTIVER0 and ISACTIVERE
 */
static inline AL_U32 Gicr_GetIsactiver(AL_UINTPTR Base, AL_U32 Id)
{
    return    GICR_GET_BIT(ISACTIVE, Base, Id);
}

/*
 * Accessors to read/write the GIC Redistributor IPRIORITYR and IPRIORITYRE
 * register corresponding to the interrupt `Id`, 4 interrupts IDs at a time.
 */
static inline AL_U32 Gicr_ReadIpriorityr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICR_READ(IPRIORITY, Base, Id);
}

static inline AL_VOID Gicr_WriteIpriorityr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Val)
{
    GICR_WRITE(IPRIORITY, Base, Id, Val);
}

/*
 * Accessor to set the byte corresponding to interrupt `Id`
 * in GIC Redistributor IPRIORITYR and IPRIORITYRE.
 */
static inline AL_VOID Gicr_SetIpriorityr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Pri)
{
    GICR_WRITE_8(IPRIORITY, Base, Id, (uint8_t)(Pri & GIC_PRI_MASK));
}

/*
 * Accessors to read/write GIC Redistributor ICFGR0, ICFGR1 registers
 */
static inline AL_U32 Gicr_ReadIcfgr0(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_ICFGR0);
}

static inline AL_U32 Gicr_ReadIcfgr1(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_ICFGR1);
}

static inline AL_VOID Gicr_WriteIcfgr0(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_ICFGR0, Val);
}

static inline AL_VOID Gicr_WriteIcfgr1(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_ICFGR1, Val);
}

/*
 * Accessors to read/write GIC Redistributor ICFGR0, ICFGR1 and ICFGRE
 * register corresponding to its number
 */
static inline AL_U32 Gicr_ReadIcfgr(AL_UINTPTR Base, AL_U32 RegNum)
{
    return AL_REG32_READ(Base + GICR_ICFGR + (RegNum << 2));
}

static inline AL_VOID Gicr_WriteIcfgr(AL_UINTPTR Base, AL_U32 RegNum,
                    AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_ICFGR + (RegNum << 2), Val);
}

/*
 * Accessor to set the bit fields corresponding to interrupt `Id`
 * in GIC Redistributor ICFGR0, ICFGR1 and ICFGRE
 */
static inline AL_VOID Gicr_SetIcfgr(AL_UINTPTR Base, AL_U32 Id, AL_U32 Cfg)
{
    GICR_WRITE(ICFG, Base, Id, Cfg);
}

/*
 * Accessors to read/write GIC Redistributor IGRPMODR0 register
 */
static inline AL_U32 Gicr_ReadIgrpmodr0(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_IGRPMODR0);
}

static inline AL_VOID Gicr_WriteIgrpmodr0(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_IGRPMODR0, Val);
}

/*
 * Accessors to get/set/clear the bit corresponding to interrupt `Id`
 * from GIC Redistributor IGRPMODR0 and IGRPMODRE
 */
static inline AL_U32 Gicr_GetIgrpmodr(AL_UINTPTR Base, AL_U32 Id)
{
    return GICR_GET_BIT(IGRPMOD, Base, Id);
}

static inline AL_VOID Gicr_SetIgrpmodr(AL_UINTPTR Base, AL_U32 Id)
{
    GICR_SET_BIT(IGRPMOD, Base, Id);
}

static inline AL_VOID Gicr_ClrIgrpmodr(AL_UINTPTR Base, AL_U32 Id)
{
    GICR_CLR_BIT(IGRPMOD, Base, Id);
}

/*
 * Accessors to read/write GIC Redistributor NSACR register
 */
static inline AL_U32 Gicr_ReadNsacr(AL_UINTPTR Base)
{
    return AL_REG32_READ(Base + GICR_NSACR);
}

static inline AL_VOID Gicr_WriteNsacr(AL_UINTPTR Base, AL_U32 Val)
{
    AL_REG32_WRITE(Base + GICR_NSACR, Val);
}

#endif /* AL_GICV3_RDIST_H */
