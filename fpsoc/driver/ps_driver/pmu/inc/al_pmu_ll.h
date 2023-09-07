/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_PMU_LL_H
#define AL_PMU_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_core.h"
#include "al_pmu_hw.h"

typedef enum
{
    AL_PMU_ERR_DDR_ECC,
    AL_PMU_ERR_OCM_ECC,
    AL_PMU_ERR_RPU_FATAL,
    AL_PMU_ERR_RPU_NORM,
    AL_PMU_ERR_APU_ECC,
    AL_PMU_ERR_WDT0_OT,
    AL_PMU_ERR_WDT1_OT,
    AL_PMU_ERR_WDT2_OT,
    AL_PMU_ERR_CLK_DETEC,
    AL_PMU_ERR_BUS_TIMEOUT,
    AL_PMU_ERR_CPU_PLL_LOCK,
    AL_PMU_ERR_IO_PLL_LOCK,
    AL_PMU_ERR_DDR_PLL_LOCK
} AL_PMU_ErrEnum;

typedef enum
{
    AL_PMU_SHACK_PROC_DONE,
    AL_PMU_SHACK_FIQ_ACK,
    AL_PMU_SHACK_HDRST_CNT_CLR
} AL_PMU_ShackEnum;

static const AL_U32 PmuErrShift[] = {PMU_DDR_ECC_SHIFT, PMU_OCM_ECC_SHIFT,
                                           PMU_RPU_FATAL_SHIFT, PMU_RPU_NORM_SHIFT,
                                           PMU_APU_ECC_SHIFT, PMU_WDT0_OT_SHIFT,
                                           PMU_WDT1_OT_SHIFT, PMU_WDT2_OT_SHIFT,
                                           PMU_CLK_DETEC_SHIFT, PMU_BUS_TIMEOUT_SHIFT,
                                           PMU_CPU_PLL_LOCK_SHIFT, PMU_IO_PLL_LOCK_SHIFT,
                                           PMU_DDR_PLL_LOCK_SHIFT};

static inline AL_VOID AlPmu_SetHwEn(AL_PMU_ErrEnum Err)
{
    AL_REG32_SET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_HW_EN0_SET_ADDR_OFFSET :
                                      PMU_HW_EN1_SET_ADDR_OFFSET), PmuErrShift[Err], AL_TRUE);
}

static inline AL_VOID AlPmu_ClrHwEn(AL_PMU_ErrEnum Err)
{
    AL_REG32_SET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_HW_EN0_CLR_ADDR_OFFSET :
                                      PMU_HW_EN1_CLR_ADDR_OFFSET), PmuErrShift[Err], AL_TRUE);
}

static inline AL_BOOL AlPmu_GetHwEn(AL_PMU_ErrEnum Err)
{
    return AL_REG32_GET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_HW_EN0_SET_ADDR_OFFSET :
                                             PMU_HW_EN1_SET_ADDR_OFFSET), PmuErrShift[Err]);
}

static inline AL_VOID AlPmu_SetIntEn(AL_PMU_ErrEnum Err)
{
    AL_REG32_SET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_INT_EN0_SET_ADDR_OFFSET :
                                      PMU_INT_EN1_SET_ADDR_OFFSET), PmuErrShift[Err], AL_TRUE);
}

static inline AL_VOID AlPmu_ClrIntEn(AL_PMU_ErrEnum Err)
{
    AL_REG32_SET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_INT_EN0_CLR_ADDR_OFFSET :
                                      PMU_INT_EN1_CLR_ADDR_OFFSET), PmuErrShift[Err], AL_TRUE);
}

static inline AL_BOOL AlPmu_GetIntEn(AL_PMU_ErrEnum Err)
{
    return AL_REG32_GET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_INT_EN0_SET_ADDR_OFFSET :
                                             PMU_INT_EN1_SET_ADDR_OFFSET), PmuErrShift[Err]);
}

static inline AL_VOID AlPmu_SetIntMask(AL_PMU_ErrEnum Err)
{
    AL_REG32_SET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_INT_MSK0_SET_ADDR_OFFSET :
                                      PMU_INT_MSK1_SET_ADDR_OFFSET), PmuErrShift[Err], AL_TRUE);
}

static inline AL_VOID AlPmu_ClrIntMask(AL_PMU_ErrEnum Err)
{
    AL_REG32_SET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_INT_MSK0_CLR_ADDR_OFFSET :
                                      PMU_INT_MSK1_CLR_ADDR_OFFSET), PmuErrShift[Err], AL_TRUE);
}

static inline AL_BOOL AlPmu_GetIntMask(AL_PMU_ErrEnum Err)
{
    return AL_REG32_GET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_INT_MSK0_SET_ADDR_OFFSET :
                                             PMU_INT_MSK1_SET_ADDR_OFFSET), PmuErrShift[Err]);
}

static inline AL_VOID AlPmu_ClrIntState(AL_PMU_ErrEnum Err)
{
    AL_REG32_SET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_INT_CLR0_ADDR_OFFSET :
                                      PMU_INT_CLR1_ADDR_OFFSET), PmuErrShift[Err], AL_TRUE);
}

static inline AL_BOOL AlPmu_GetIntState(AL_PMU_ErrEnum Err)
{
    return AL_REG32_GET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_INT_STATE0_ADDR_OFFSET :
                                             PMU_INT_STATE1_ADDR_OFFSET), PmuErrShift[Err]);
}

/* Read clear */
static inline AL_BOOL AlPmu_GetRawHistoryState(AL_PMU_ErrEnum Err)
{
    return AL_REG32_GET_BIT(PMU_CTRL_ADDR + ((Err <= AL_PMU_ERR_BUS_TIMEOUT) ? PMU_RAW_HIS0_ADDR_OFFSET :
                                             PMU_RAW_HIS1_ADDR_OFFSET), PmuErrShift[Err]);
}

static inline AL_VOID AlPmu_SetRpuNmiEn(AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT(PMU_CTRL_ADDR + PMU_NMI_EN_ADDR_OFFSET, PMU_NMI_EN_RPU_SHIFT, IsEnable);
}

static inline AL_VOID AlPmu_Shack(AL_PMU_ShackEnum Shack)
{
    AL_REG32_SET_BIT(PMU_CTRL_ADDR + PMU_SW_PMU_SHACK_ADDR_OFFSET, Shack, AL_TRUE);
}

#ifdef __cplusplus
}
#endif

#endif
