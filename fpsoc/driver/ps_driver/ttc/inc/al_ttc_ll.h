/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_TTC_LL_H_
#define __AL_TTC_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_ttc_hw.h"

/**************************** Type Definitions ******************************/
typedef enum
{
    AL_TTC_TC0    = 0,
    AL_TTC_TC1    = 1,
    AL_TTC_TC2    = 2
} AL_TTC_TcEnum;

typedef enum
{
    AL_TTC_PCLK      = 0,
    AL_TTC_EXTCLK    = 1
} AL_TTC_ClkSrcEnum;

typedef enum
{
    AL_TTC_Posedge    = 0,
    AL_TTC_Negedge    = 1
} AL_TTC_ClkEdgeEnum;

typedef enum
{
    AL_TTC_OverflowMode    = 0,
    AL_TTC_IntervalMode    = 1
} AL_TTC_WorkModeEnum;

typedef enum
{
    AL_TTC_HighLevel    = 0,
    AL_TTC_LowLevel     = 1
} AL_TTC_LevelEnum;

typedef enum
{
    AL_TTC_Match1    = 0,
    AL_TTC_Match2    = 1,
    AL_TTC_Match3    = 2
} AL_TTC_MatchNumEnum;

typedef enum
{
    AL_TTC_IntrInterval         = 0,
    AL_TTC_IntrMatch1           = 1,
    AL_TTC_IntrMatch2           = 2,
    AL_TTC_IntrMatch3           = 3,
    AL_TTC_IntrOverflow         = 4,
    AL_TTC_IntrEventTimer       = 5,
} AL_TTC_IntrTypeEnum;

typedef enum
{
    AL_TTC_CountUp      = 0,
    AL_TTC_CountDown    = 1
} AL_TTC_CountDec;

/***************** Macros (Inline Functions) Definitions ********************/

static inline AL_U16 AltTtc_ll_GetEventTimerVal(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + TTC__EVENT_REG_1__OFFSET, TTC__EVENT_REG_1__EVENT__SHIFT, TTC__EVENT_REG_1__EVENT__SIZE);
}

static inline AL_VOID AlTtc_ll_EnableEventTimerMode(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__EVENT_CTRL_1__OFFSET, TTC__EVENT_CTRL_1__E_EN__SHIFT, AL_TRUE);
}

static inline AL_VOID AlTtc_ll_DisableEventTimerMode(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__EVENT_CTRL_1__OFFSET, TTC__EVENT_CTRL_1__E_EN__SHIFT, AL_FALSE);
}

static inline AL_VOID AlTtc_ll_SetEventTimerLevel(AL_REG BaseAddr, AL_TTC_LevelEnum Level)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__EVENT_CTRL_1__OFFSET, TTC__EVENT_CTRL_1__E_LO__SHIFT, Level);
}

static inline AL_VOID AlTtc_ll_EventTimerOv(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__EVENT_CTRL_1__OFFSET, TTC__EVENT_CTRL_1__E_OV__SHIFT, State);
}

static inline AL_VOID AlTtc_ll_EnbaleCounter(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__DIS__SHIFT, AL_FALSE);
}

static inline AL_VOID AlTtc_ll_DisbaleCounter(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__DIS__SHIFT, AL_TRUE);
}

static inline AL_VOID AlTtc_ll_EnbaleOverflowMode(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__INT__SHIFT, AL_FALSE);
}

static inline AL_VOID AlTtc_ll_EnbaleIntervalMode(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__INT__SHIFT, AL_TRUE);
}

static inline AL_VOID AlTtc_ll_CountDec(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__DEC__SHIFT, State);
}

static inline AL_VOID AlTtc_ll_EnableMatchMode(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__MATCH__SHIFT, AL_TRUE);
}

static inline AL_VOID AlTtc_ll_DisableMatchMode(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__MATCH__SHIFT, AL_FALSE);
}

static inline AL_VOID AlTtc_ll_ResetCounter(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__RST__SHIFT, AL_TRUE);
}

static inline AL_VOID AlTtc_ll_EnableWaveOutput(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__WAVE_EN__SHIFT, AL_FALSE);
}

static inline AL_VOID AlTtc_ll_DisableWaveOutput(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__WAVE_EN__SHIFT, AL_TRUE);
}

static inline AL_VOID AlTtc_ll_SetWaveformPolarity(AL_REG BaseAddr, AL_TTC_ClkEdgeEnum ClkEdge)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CNTR_CTRL_REG_1__ADDR, TTC__CNTR_CTRL_REG_1__WAVE_POL__SHIFT, ClkEdge);
}

static inline AL_VOID AlTtc_ll_EnbalePresacle(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CLK_CTRL_REG_1__OFFSET, TTC__CLK_CTRL_REG_1__PS_EN__SHIFT, AL_TRUE);
}

static inline AL_VOID AlTtc_ll_DisbalePresacle(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CLK_CTRL_REG_1__OFFSET, TTC__CLK_CTRL_REG_1__PS_EN__SHIFT, AL_FALSE);
}

static inline AL_VOID AlTtc_ll_SetPresacleVal(AL_REG BaseAddr, AL_U8 Div)
{
    AL_REG32_SET_BITS(BaseAddr + TTC__CLK_CTRL_REG_1__OFFSET, TTC__CLK_CTRL_REG_1__PS_V__SHIFT, TTC__CLK_CTRL_REG_1__PS_V__SIZE, Div);
}

static inline AL_VOID AlTtc_ll_SetClkSrc(AL_REG BaseAddr, AL_TTC_ClkSrcEnum ClkSrc)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CLK_CTRL_REG_1__OFFSET, TTC__CLK_CTRL_REG_1__C_SRC__SHIFT, ClkSrc);
}

static inline AL_VOID AlTtc_ll_SelExtClkEdge(AL_REG BaseAddr, AL_TTC_ClkEdgeEnum ClkEdge)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__CLK_CTRL_REG_1__OFFSET, TTC__CLK_CTRL_REG_1__EX_E__SHIFT, ClkEdge);
}

static inline AL_VOID AlTtc_ll_SetIntervalMaxVal(AL_REG BaseAddr, AL_U16 Value)
{
    AL_REG32_SET_BITS(BaseAddr + TTC__INTERVAL_REG_1__OFFSET, TTC__INTERVAL_REG_1__INTERVAL__SHIFT, TTC__INTERVAL_REG_1__INTERVAL__SIZE, Value);
}

static inline AL_U32 AltTtc_ll_GetCounterVal(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + TTC__CNTR_VAL_REG_1__OFFSET, TTC__CNTR_VAL_REG_1__VALUE__SHIFT, TTC__CNTR_VAL_REG_1__VALUE__SIZE);
}

static inline AL_VOID AlTtc_ll_SetMatchVal(AL_REG BaseAddr, AL_U32 DevId, AL_TTC_MatchNumEnum MatchNum, AL_U16 Value)
{
    if (DevId > 2)
        DevId = DevId - 3;
    AL_REG32_SET_BITS(BaseAddr + TTC__MATCH_1_REG_1__OFFSET + (DevId * 0x8) +  (MatchNum * 0x4), TTC__MATCH_1_REG_1__MATCH__SHIFT, TTC__MATCH_1_REG_1__MATCH__SIZE, Value);
}

static inline AL_VOID AlTtc_ll_EnableIntr(AL_REG BaseAddr, AL_TTC_IntrTypeEnum IntrType)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__IRQ_EN_REG_1__OFFSET, TTC__IRQ_EN_REG_1__IEN__SHIFT, IntrType << 1);
}

static inline AL_VOID AlTtc_ll_DisableIntr(AL_REG BaseAddr, AL_TTC_IntrTypeEnum IntrType)
{
    AL_REG32_SET_BIT(BaseAddr + TTC__IRQ_EN_REG_1__OFFSET, TTC__IRQ_EN_REG_1__IEN__SHIFT, IntrType << 0);
}

static inline AL_VOID AlTtc_ll_DisableAllIntr(AL_REG BaseAddr)
{
    AL_REG32_SET_BITS(BaseAddr + TTC__IRQ_EN_REG_1__OFFSET, TTC__IRQ_EN_REG_1__IEN__SHIFT, TTC__IRQ_EN_REG_1__IEN__SIZE, 0x0);
}

static inline AL_U32 AltTtc_ll_GetIntrtype(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + TTC__IRQ_REG_1__OFFSET);
}


#ifdef __cplusplus
}
#endif

#endif
