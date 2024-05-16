/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_hal.h"
#include "al_can_hw.h"

AL_CAN_HwConfigStruct AlCan_HwConfig[AL_CAN_NUM_INSTANCE] = {
#ifdef CAN0PS_ENABLE
    {
        .DeviceId       = 0,
        .BaseAddress    = CAN0__BASE_ADDR,
        .IntrId         = SOC_CAN0_IRQn
    },
#endif
#ifdef CAN1PS_ENABLE

    {
        .DeviceId       = 1,
        .BaseAddress    = CAN1__BASE_ADDR,
        .IntrId         = SOC_CAN1_IRQn
    }
#endif
};