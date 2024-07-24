/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_can_plat.h"
#include "al_can_hw.h"

AL_CAN_HwConfigStruct AlCan_HwConfig[AL_CAN_NUM_INSTANCE] = {
#ifdef CAN0PS_ENABLE
    {
        .DeviceId       = 0,
        .BaseAddress    = CAN0__BASE_ADDR,
        .IntrId         = SOC_CAN0_IRQn,
        .InputClk       = CAN_CLOCK,
        .Type           = CAN0_TYPE,
        .SlowBitRate    = CAN0_SLOW_RATE,
        .FastBitRate    = CAN0_FAST_RATE
    },
#endif
#ifdef CAN1PS_ENABLE
    {
        .DeviceId       = 1,
        .BaseAddress    = CAN1__BASE_ADDR,
        .IntrId         = SOC_CAN1_IRQn,
        .InputClk       = CAN_CLOCK,
        .Type           = CAN1_TYPE,
        .SlowBitRate    = CAN1_SLOW_RATE,
        .FastBitRate    = CAN1_FAST_RATE
    }
#endif
};