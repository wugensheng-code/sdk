/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_iic_hw.h"

AlAxiIic_HwConfigStruct AlAxiIic_HwConfig[AL_AXI_IIC_NUM_INSTANCE] =
{
    {
        .DeviceId       = AXI_IIC0_DEVICE_ID,
        .BaseAddress    = AXI_I2C0_BASE_ADDR,
        .IntrId         = AXI_IIC0_IRQn,
    },

    {
        .DeviceId       = AXI_IIC1_DEVICE_ID,
        .BaseAddress    = AXI_I2C1_BASE_ADDR,
        .IntrId         = AXI_IIC1_IRQn,
    },
};
