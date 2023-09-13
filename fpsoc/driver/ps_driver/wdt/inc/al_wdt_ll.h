/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_WDT_LL_H
#define AL_WDT_LL_H

#include "al_type.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "al_wdt_hw.h"
#include "al_reg_io.h"

#define WDT_RESTART_CMD               (0x76)

typedef enum
{
    WDT_RPL_PCLK_CYCLES_2             = 0x0,
    WDT_RPL_PCLK_CYCLES_4             = 0x1,
    WDT_RPL_PCLK_CYCLES_8             = 0x2,
    WDT_RPL_PCLK_CYCLES_16            = 0x3,
    WDT_RPL_PCLK_CYCLES_32            = 0x4,
    WDT_RPL_PCLK_CYCLES_64            = 0x5,
    WDT_RPL_PCLK_CYCLES_128           = 0x6,
    WDT_RPL_PCLK_CYCLES_256           = 0x7
} AL_WDT_RestPluseLenEnum;

typedef enum
{
    WDT_RESET_MODE                    = 0x0,
    WDT_INTR_MODE                     = 0x1
} AL_WDT_ResponseMode;

typedef enum 
{
    WDT_TIMEOUT_PERIOD_64K_CLOCKS      = 0x0, 
    WDT_TIMEOUT_PERIOD_128K_CLOCKS     = 0x1, 
    WDT_TIMEOUT_PERIOD_256K_CLOCKS     = 0x2, 
    WDT_TIMEOUT_PERIOD_512K_CLOCKS     = 0x3, 
    WDT_TIMEOUT_PERIOD_1M_CLOCKS       = 0x4, 
    WDT_TIMEOUT_PERIOD_2M_CLOCKS       = 0x5, 
    WDT_TIMEOUT_PERIOD_4M_CLOCKS       = 0x6, 
    WDT_TIMEOUT_PERIOD_8M_CLOCKS       = 0x7, 
    WDT_TIMEOUT_PERIOD_16M_CLOCKS      = 0x8, 
    WDT_TIMEOUT_PERIOD_32M_CLOCKS      = 0x9, 
    WDT_TIMEOUT_PERIOD_64M_CLOCKS      = 0xa, 
    WDT_TIMEOUT_PERIOD_128M_CLOCKS     = 0xb, 
    WDT_TIMEOUT_PERIOD_256M_CLOCKS     = 0xc, 
    WDT_TIMEOUT_PERIOD_512M_CLOCKS     = 0xd, 
    WDT_TIMEOUT_PERIOD_1G_CLOCKS       = 0xe, 
    WDT_TIMEOUT_PERIOD_2G_CLOCKS       = 0xf
} AL_WDT_TIMEOUT_PERIOD; 

static inline AL_U32 AlWdt_ll_ClearIntr(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + WDT__WDT_EOI__ADDR__OFFSET , WDT__WDT_EOI__WDT_EOI__SHIFT);
}

static inline AL_VOID AlWdt_ll_Enable(AL_REG BaseAddr, AL_FUNCTION State)
{
    AL_REG32_SET_BIT(BaseAddr + WDT__WDT_CR__ADDR__OFFSET , WDT__WDT_CR__WDT_EN__SHIFT, State);
}

static inline AL_VOID AlWdt_ll_Set_RestPulseLen(AL_REG BaseAddr, AL_WDT_RestPluseLenEnum RestPluseLen)
{
    AL_REG32_SET_BITS(BaseAddr + WDT__WDT_CR__ADDR__OFFSET , WDT__WDT_CR__RPL__SHIFT, WDT__WDT_CR__RPL__SIZE, RestPluseLen);
}

static inline AL_VOID AlWdt_ll_Set_ResopnseMode(AL_REG BaseAddr, AL_WDT_ResponseMode Mode)
{
    AL_REG32_SET_BIT(BaseAddr + WDT__WDT_CR__ADDR__OFFSET , WDT__WDT_CR__RMOD__SHIFT, Mode);  
}

static inline AL_VOID AlWdt_ll_Set_TimeoutPeriod(AL_REG BaseAddr, AL_WDT_TIMEOUT_PERIOD TimeOutPeriod)
{
    AL_REG32_SET_BITS(BaseAddr + WDT__WDT_TORR__ADDR__OFFSET, WDT__WDT_TORR__TOP__SHIFT, WDT__WDT_TORR__TOP__SIZE, TimeOutPeriod);
}

static inline AL_VOID AlWdt_ll_CounterRestart(AL_REG BaseAddr)
{
    AL_REG32_SET_BITS(BaseAddr + WDT__WDT_CRR__ADDR__OFFSET, WDT__WDT_CRR__WDT_CRR__SHIFT, WDT__WDT_CRR__WDT_CRR__SIZE, WDT_RESTART_CMD);
}

static inline AL_VOID AlWdt_ll_Pause(AL_REG BaseAddr, AL_FUNCTION State)
{
    AL_REG32_SET_BIT(WDT_TOP_CFG_CTRL_ADDR, ((BaseAddr == WDT0__BASE_ADDR) ? WDT_TOP_CFG_CTRL_ADDR_WDT0_PAUSE_SHIFT :
            WDT_TOP_CFG_CTRL_ADDR_WDT1_PAUSE_SHIFT), State);
}
#endif
