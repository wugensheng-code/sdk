/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/********************************* including Files *********************************/
#include "al_wdt_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_WDT_HwConfigStruct AlWdt_HwConfig[AL_WDT_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .BaseAddress        = WDT0__BASE_ADDR,
        .InputClockHz       = WDT0_CLOCK,
        .IntrNum            = SOC_WDT0_IRQn
    },

    {
        .DeviceId           = 1,
        .BaseAddress        = WDT1__BASE_ADDR,
        .InputClockHz       = WDT1_CLOCK,
        .IntrNum            = SOC_WDT1_IRQn
    }
};
