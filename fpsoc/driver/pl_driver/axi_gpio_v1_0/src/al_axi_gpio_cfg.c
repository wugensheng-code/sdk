/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_axi_gpio_hw.h"

/************************** Variable Prototypes ******************************/
AlAxiGpio_HwConfigStruct AlAxiGpio_HwCfg[AL_AXI_GPIO_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .BaseAddress        = AXI_GPIO0__BASE_ADDR,
        .IntrId             = AXI_GPIO0_IRQn,
    },
    {
        .DeviceId           = 1,
        .BaseAddress        = AXI_GPIO1__BASE_ADDR,
        .IntrId             = AXI_GPIO1_IRQn,
    }
};