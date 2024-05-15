/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_hal.h"
#include "al_gbe_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_GBE_HwConfigStruct AlGbe_HwConfig[AL_GBE_NUM_INSTANCE] =
{
#ifdef GBE0PS_ENABLE
    {
        .DeviceId       = 0,
        .BaseAddress    = GBE0__BASE_ADDR,
        .InputClockHz   = GBE_CLOCK,
        .PtpInputClockHz= GBE0_PTP_CLOCK,
        .IntrNum        = SOC_GBE0_IRQn,
    },
#endif

#ifdef GBE1PS_ENABLE
    {
        .DeviceId       = 1,
        .BaseAddress    = GBE1__BASE_ADDR,
        .InputClockHz   = GBE_CLOCK,
        .PtpInputClockHz= GBE1_PTP_CLOCK,
        .IntrNum        = SOC_GBE1_IRQn
    },
#endif
};