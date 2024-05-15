
/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/********************************* including Files *********************************/
#include "al_hal.h"
#include "al_ttc_hw.h"

/********************************* Constant Definitions *****************************/

AL_TTC_HwConfigStruct AlTtc_HwConfig[AL_TTC_NUM_INSTANCE] =
{
#ifdef TTC0PS_ENABLE
    {
        .DevId              = 0,
        .BaseAddress        = TTC0__TC0_BASE_ADDR,
        .InputClockHz       = TTC_CLOCK,
        .IntrNum            = SOC_TTC0_TC0_IRQn,
    },
    {
        .DevId              = 1,
        .BaseAddress        = TTC0__TC1_BASE_ADDR,
        .InputClockHz       = TTC_CLOCK,
        .IntrNum            = SOC_TTC0_TC1_IRQn,
    },
    {
        .DevId              = 2,
        .BaseAddress        = TTC0__TC2_BASE_ADDR,
        .InputClockHz       = TTC_CLOCK,
        .IntrNum            = SOC_TTC0_TC2_IRQn,
    },
#endif

#ifdef TTC1PS_ENABLE
    {
        .DevId              = 3,
        .BaseAddress        = TTC1__TC0_BASE_ADDR,
        .InputClockHz       = TTC_CLOCK,
        .IntrNum            = SOC_TTC1_TC0_IRQn,
    },
    {
        .DevId              = 4,
        .BaseAddress        = TTC1__TC1_BASE_ADDR,
        .InputClockHz       = TTC_CLOCK,
        .IntrNum            = SOC_TTC1_TC1_IRQn,
    },
    {
        .DevId              = 5,
        .BaseAddress        = TTC1__TC2_BASE_ADDR,
        .InputClockHz       = TTC_CLOCK,
        .IntrNum            = SOC_TTC1_TC2_IRQn,
    },
#endif
};