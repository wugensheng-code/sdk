
/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/********************************* including Files *********************************/
#include "al_hal.h"
#include "al_tc_hw.h"

/********************************* Constant Definitions *****************************/

AL_TC_HwConfigStruct AlTc_HwConfig[AL_TC_NUM_INSTANCE] =
{
#ifdef TC0PS_ENABLE
    {
        .DevId              = 0,
        .BaseAddress        = TC0__TC0_BASE_ADDR,
        .InputClockHz       = TC_CLOCK,
        .IntrNum            = SOC_TC0_TC0_IRQn,
    },
    {
        .DevId              = 1,
        .BaseAddress        = TC0__TC1_BASE_ADDR,
        .InputClockHz       = TC_CLOCK,
        .IntrNum            = SOC_TC0_TC1_IRQn,
    },
    {
        .DevId              = 2,
        .BaseAddress        = TC0__TC2_BASE_ADDR,
        .InputClockHz       = TC_CLOCK,
        .IntrNum            = SOC_TC0_TC2_IRQn,
    },
#endif

#ifdef TC1PS_ENABLE
    {
        .DevId              = 3,
        .BaseAddress        = TC1__TC0_BASE_ADDR,
        .InputClockHz       = TC_CLOCK,
        .IntrNum            = SOC_TC1_TC0_IRQn,
    },
    {
        .DevId              = 4,
        .BaseAddress        = TC1__TC1_BASE_ADDR,
        .InputClockHz       = TC_CLOCK,
        .IntrNum            = SOC_TC1_TC1_IRQn,
    },
    {
        .DevId              = 5,
        .BaseAddress        = TC1__TC2_BASE_ADDR,
        .InputClockHz       = TC_CLOCK,
        .IntrNum            = SOC_TC1_TC2_IRQn,
    },
#endif
};