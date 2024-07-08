/*
 * Copyright (c) 2015-2021, ARM Limited and Contributors. All rights reserved.
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_GICV3_PRIVATE_H_
#define __AL_GICV3_PRIVATE_H_

#include <assert.h>
#include <stdint.h>
#include "al_reg_io.h"
#include "al_type.h"
#include "al_gicv3.h"

/*******************************************************************************
 * GICv3 private macro definitions
 ******************************************************************************/

/* Constants to indicate the status of the RWP bit */
#define RWP_TRUE    U(1)
#define RWP_FALSE   U(0)

/* Calculate GIC register bit number corresponding to its interrupt ID */
#define BIT_NUM(REG, Id)     ((Id) & ((1U << REG##R_SHIFT) - 1U))

/*
 * Calculate 8, 32 and 64-bit GICD register offset
 * corresponding to its interrupt ID
 */
#define GICD_OFFSET_8(REG, Id)      (GICD_##REG##R + (AL_UINTPTR)(Id))

#define GICD_OFFSET(REG, Id)        (GICD_##REG##R + (((AL_UINTPTR)(Id) >> REG##R_SHIFT) << 2))

#define GICD_OFFSET_64(REG, Id)     (GICD_##REG##R + (((AL_UINTPTR)(Id) >> REG##R_SHIFT) << 3))

/*
 * Read/Write 8, 32 and 64-bit GIC Distributor register
 * corresponding to its interrupt ID
 */
#define GICD_READ(REG, Base, Id)            AL_REG32_READ((Base) + GICD_OFFSET(REG, (Id)))

#define GICD_READ_64(REG, Base, Id)         AL_REG64_READ((Base) + GICD_OFFSET_64(REG, (Id)))

#define GICD_WRITE_8(REG, Base, Id, Val)    AL_REG8_WRITE((Base) + GICD_OFFSET_8(REG, (Id)), (Val))

#define GICD_WRITE(REG, Base, Id, Val)      AL_REG32_WRITE((Base) + GICD_OFFSET(REG, (Id)), (Val))

#define GICD_WRITE_64(REG, Base, Id, Val)   AL_REG64_WRITE((Base) + GICD_OFFSET_64(REG, (Id)), (Val))

/*
 * Bit operations on GIC Distributor register corresponding
 * to its interrupt ID
 */
/* Get bit in GIC Distributor register */
#define GICD_GET_BIT(REG, Base, Id)     ((AL_REG32_READ((Base) + GICD_OFFSET(REG, (Id))) >> \
                                          BIT_NUM(REG, (Id))) & 1U)

/* Set bit in GIC Distributor register */
#define GICD_SET_BIT(REG, Base, Id)     AL_REG32_SET_BIT((Base) + GICD_OFFSET(REG, (Id)), \
                                                         BIT_NUM(REG, (Id)), 1)

/* Clear bit in GIC Distributor register */
#define GICD_CLR_BIT(REG, Base, Id)     AL_REG32_SET_BIT((Base) + GICD_OFFSET(REG, (Id)), \
                                                         BIT_NUM(REG, (Id)), 0)

/* Write bit in GIC Distributor register */
#define GICD_WRITE_BIT(REG, Base, Id)   AL_REG32_WRITE((Base) + GICD_OFFSET(REG, (Id)), \
                                                       ((AL_U32)1 << BIT_NUM(REG, (Id))))

/*
 * Calculate 8 and 32-bit GICR register offset
 * corresponding to its interrupt ID
 */
#define GICR_OFFSET_8(REG, Id)  (GICR_##REG##R + (AL_UINTPTR)(Id))

#define GICR_OFFSET(REG, Id)    (GICR_##REG##R + (((AL_UINTPTR)(Id) >> REG##R_SHIFT) << 2))

/* Read/Write GIC Redistributor register corresponding to its interrupt ID */
#define GICR_READ(REG, Base, Id)    AL_REG32_READ((Base) + GICR_OFFSET(REG, (Id)))

#define GICR_WRITE_8(REG, Base, Id, Val)    AL_REG8_WRITE((Base) + GICR_OFFSET_8(REG, (Id)), (Val))

#define GICR_WRITE(REG, Base, Id, Val)      AL_REG32_WRITE((Base) + GICR_OFFSET(REG, (Id)), (Val))

/*
 * Bit operations on GIC Redistributor register
 * corresponding to its interrupt ID
 */
/* Get bit in GIC Redistributor register */
#define GICR_GET_BIT(REG, Base, Id)     ((AL_REG32_READ((Base) + GICR_OFFSET(REG, (Id))) >> \
                                          BIT_NUM(REG, (Id))) & 1U)

/* Write bit in GIC Redistributor register */
#define GICR_WRITE_BIT(REG, Base, Id)   AL_REG32_WRITE((Base) + GICR_OFFSET(REG, (Id)), \
                                                       ((AL_U32)1 << BIT_NUM(REG, (Id))))

/* Set bit in GIC Redistributor register */
#define GICR_SET_BIT(REG, Base, Id)     AL_REG32_SET_BIT((Base) + GICD_OFFSET(REG, (Id)), \
                                                         BIT_NUM(REG, (Id)), 1)

/* Clear bit in GIC Redistributor register */
#define GICR_CLR_BIT(REG, Base, Id)     AL_REG32_SET_BIT((Base) + GICD_OFFSET(REG, (Id)),    \
                                                         BIT_NUM(REG, (Id)), 0)

/*
 * Macro to convert a GICR_TYPER affinity value into a MPIDR value. Bits[31:24]
 * are zeroes.
 */
#ifdef __aarch64__
static inline AL_REG MpidrFromGicrTyper(AL_U64 TyperVal)
{
    return (((TyperVal >> 56) & MPIDR_AFFLVL_MASK) << MPIDR_AFF3_SHIFT) |
        ((TyperVal >> 32) & U(0xffffff));
}
#else
static inline AL_REG MpidrFromGicrTyper(AL_U64 TyperVal)
{
    return (((TyperVal) >> 32) & U(0xffffff));
}
#endif

/*******************************************************************************
 * GICv3 private global variables declarations
 ******************************************************************************/
extern const AL_GICV3_DrvDataStruct *Gicv3DrvData;

/*******************************************************************************
 * Private GICv3 helper function prototypes
 ******************************************************************************/
AL_U32 AlGicv3_GetSpiLimit(AL_UINTPTR GicdBase);
AL_VOID AlGicv3_SpisConfigDefaults(AL_UINTPTR GicdBase);
AL_VOID AlGicv3_PpiSgiConfigDefaults(AL_UINTPTR GicrBase);
AL_VOID AlGicv3_Rdist_BaseAddrsProbe(AL_UINTPTR *RdistBaseAddrs, AL_U32 RdistNum, AL_UINTPTR GicrBase,
                                     MpidrHashFn MpidrToCorePos);
AL_VOID AlGicv3_Rdist_MarkCoreAwake(AL_UINTPTR GicrBase);
AL_VOID AlGicv3_Rdist_MarkCoreAsleep(AL_UINTPTR GicrBase);
AL_VOID AlGicv3_Rdist_WaitMasterAwake(AL_UINTPTR GicrBase);
AL_U32 AlGicv3_GetComponentPartnum(const AL_UINTPTR GicFrame);

#endif /* AL_GICV3_PRIVATE_H */
