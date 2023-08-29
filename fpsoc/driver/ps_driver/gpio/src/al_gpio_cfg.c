/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */



/***************************** Include Files *********************************/
#include "al_gpio_hw.h"

/************************** Variable Prototypes ******************************/
AL_GPIO_HwConfigStruct AlGpio_HwCfg[AL_GPIO_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .BaseAddress        = GPIO_BASE_ADDR,
        .MaxBanks           = GPIO_MAX_BANKS,
        .BankMaxPins        = GPIO_BANK_MAX_PINS,
    }
};