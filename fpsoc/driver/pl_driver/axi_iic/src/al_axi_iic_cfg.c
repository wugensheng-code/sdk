/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_iic_hw.h"

AlAxiIic_HwConfigStruct AlAxiIic_HwConfig[AL_AXI_IIC_NUM_INSTANCE] = 
{
    {
        .DeviceId       = 0,
        .BaseAddress    = AXI_I2C0__BASE_ADDR,
        .IntrId         = AXI_IIC0_IRQn,
    },

    {
        .DeviceId       = 1,
        .BaseAddress    = AXI_I2C1__BASE_ADDR,
        .IntrId         = AXI_IIC1_IRQn,
    },
};
