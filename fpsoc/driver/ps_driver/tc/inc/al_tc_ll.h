/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_TC_LL_H_
#define __AL_TC_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_tc_hw.h"

/**************************** Type Definitions ******************************/
typedef enum
{
    AL_TC_TC0    = 0,
    AL_TC_TC1    = 1,
    AL_TC_TC2    = 2
} AL_TC_TcEnum;

typedef enum
{
    AL_TC_PCLK      = 0,
    AL_TC_EXTCLK    = 1
} AL_TC_ClkSrcEnum;

typedef enum
{
    AL_TC_Posedge    = 0,
    AL_TC_Negedge    = 1
} AL_TC_ClkEdgeEnum;

typedef enum
{
    AL_TC_OverflowMode    = 0,
    AL_TC_IntervalMode    = 1
} AL_TC_WorkModeEnum;

typedef enum
{
    AL_TC_HighLevel    = 0,
    AL_TC_LowLevel     = 1
} AL_TC_LevelEnum;

typedef enum
{
    AL_TC_Match1    = 0,
    AL_TC_Match2    = 1,
    AL_TC_Match3    = 2
} AL_TC_MatchNumEnum;

typedef enum
{
    AL_TC_IntrInterval         = 0,
    AL_TC_IntrMatch1           = 1,
    AL_TC_IntrMatch2           = 2,
    AL_TC_IntrMatch3           = 3,
    AL_TC_IntrOverflow         = 4,
    AL_TC_IntrEventTimer       = 5,
} AL_TC_IntrTypeEnum;

typedef enum
{
    AL_TC_CountUp      = 0,
    AL_TC_CountDown    = 1
} AL_TC_CountDec;

/***************** Macros (Inline Functions) Definitions ********************/

static inline AL_U16 AlTc_ll_GetEventTimerVal(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + TC__EVENT_REG_1__OFFSET, TC__EVENT_REG_1__EVENT__SHIFT, TC__EVENT_REG_1__EVENT__SIZE);
}

static inline AL_VOID AlTc_ll_EnableEventTimerMode(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TC__EVENT_CTRL_1__OFFSET, TC__EVENT_CTRL_1__E_EN__SHIFT, State);
}

static inline AL_VOID AlTc_ll_SetEventTimerLevel(AL_REG BaseAddr, AL_TC_LevelEnum Level)
{
    AL_REG32_SET_BIT(BaseAddr + TC__EVENT_CTRL_1__OFFSET, TC__EVENT_CTRL_1__E_LO__SHIFT, Level);
}

static inline AL_VOID AlTc_ll_SetEventTimerOvState(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TC__EVENT_CTRL_1__OFFSET, TC__EVENT_CTRL_1__E_OV__SHIFT, State);
}

static inline AL_VOID AlTc_ll_EnbaleCounter(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CNTR_CTRL_REG_1__ADDR, TC__CNTR_CTRL_REG_1__DIS__SHIFT, ~State);
}

static inline AL_VOID AlTc_ll_EnbaleOverflowMode(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CNTR_CTRL_REG_1__ADDR, TC__CNTR_CTRL_REG_1__INT__SHIFT, AL_FALSE);
}

static inline AL_VOID AlTc_ll_EnbaleIntervalMode(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CNTR_CTRL_REG_1__ADDR, TC__CNTR_CTRL_REG_1__INT__SHIFT, AL_TRUE);
}

static inline AL_VOID AlTc_ll_SetCountDec(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CNTR_CTRL_REG_1__ADDR, TC__CNTR_CTRL_REG_1__DEC__SHIFT, State);
}

static inline AL_VOID AlTc_ll_EnableMatchMode(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CNTR_CTRL_REG_1__ADDR, TC__CNTR_CTRL_REG_1__MATCH__SHIFT, State);
}

static inline AL_VOID AlTc_ll_ResetCounter(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CNTR_CTRL_REG_1__ADDR, TC__CNTR_CTRL_REG_1__RST__SHIFT, AL_TRUE);
}

static inline AL_VOID AlTc_ll_EnableWaveOutput(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CNTR_CTRL_REG_1__ADDR, TC__CNTR_CTRL_REG_1__WAVE_EN__SHIFT, ~State);
}

static inline AL_VOID AlTc_ll_SetWaveformPolarity(AL_REG BaseAddr, AL_TC_ClkEdgeEnum ClkEdge)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CNTR_CTRL_REG_1__ADDR, TC__CNTR_CTRL_REG_1__WAVE_POL__SHIFT, ClkEdge);
}

static inline AL_VOID AlTc_ll_EnbalePresacle(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CLK_CTRL_REG_1__OFFSET, TC__CLK_CTRL_REG_1__PS_EN__SHIFT, State);
}

static inline AL_VOID AlTc_ll_SetPresacleVal(AL_REG BaseAddr, AL_U8 Div)
{
    AL_REG32_SET_BITS(BaseAddr + TC__CLK_CTRL_REG_1__OFFSET, TC__CLK_CTRL_REG_1__PS_V__SHIFT, TC__CLK_CTRL_REG_1__PS_V__SIZE, Div);
}

static inline AL_VOID AlTc_ll_SetClkSrc(AL_REG BaseAddr, AL_TC_ClkSrcEnum ClkSrc)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CLK_CTRL_REG_1__OFFSET, TC__CLK_CTRL_REG_1__C_SRC__SHIFT, ClkSrc);
}

static inline AL_VOID AlTc_ll_SetExtClkEdge(AL_REG BaseAddr, AL_TC_ClkEdgeEnum ClkEdge)
{
    AL_REG32_SET_BIT(BaseAddr + TC__CLK_CTRL_REG_1__OFFSET, TC__CLK_CTRL_REG_1__EX_E__SHIFT, ClkEdge);
}

static inline AL_VOID AlTc_ll_SetIntervalMaxVal(AL_REG BaseAddr, AL_U16 Value)
{
    AL_REG32_SET_BITS(BaseAddr + TC__INTERVAL_REG_1__OFFSET, TC__INTERVAL_REG_1__INTERVAL__SHIFT, TC__INTERVAL_REG_1__INTERVAL__SIZE, Value);
}

static inline AL_U32 AlTc_ll_GetCounterVal(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + TC__CNTR_VAL_REG_1__OFFSET, TC__CNTR_VAL_REG_1__VALUE__SHIFT, TC__CNTR_VAL_REG_1__VALUE__SIZE);
}

static inline AL_VOID AlTc_ll_SetMatchVal(AL_REG BaseAddr, AL_U32 DevId, AL_TC_MatchNumEnum MatchNum, AL_U16 Value)
{
    if (DevId > 2)
        DevId = DevId - 3;
    AL_REG32_SET_BITS(BaseAddr + TC__MATCH_1_REG_1__OFFSET + (DevId * 0x8) +  (MatchNum * 0x4), TC__MATCH_1_REG_1__MATCH__SHIFT, TC__MATCH_1_REG_1__MATCH__SIZE, Value);
}

static inline AL_VOID AlTc_ll_EnableIntr(AL_REG BaseAddr, AL_TC_IntrTypeEnum IntrType, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TC__IRQ_EN_REG_1__OFFSET, IntrType, State);
}

static inline AL_VOID AlTc_ll_DisableAllIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BITS(BaseAddr + TC__IRQ_EN_REG_1__OFFSET, TC__IRQ_EN_REG_1__IEN__SHIFT, TC__IRQ_EN_REG_1__IEN__SIZE, 0x0);
}

static inline AL_U32 AlTc_ll_GetIntrtype(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + TC__IRQ_REG_1__OFFSET);
}


#ifdef __cplusplus
}
#endif

#endif
