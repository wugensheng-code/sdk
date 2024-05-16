/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_hal.h"
#include "al_iic_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_IIC_HwConfigStruct AlIic_HwConfig[AL_IIC_NUM_INSTANCE] =
{
#ifdef I2C0PS_ENABLE
    {
        .DeviceId       = 0,
        .BaseAddress    = I2C0__BASE_ADDR,
        .ClockHz        = I2C_CLOCK,
        .RateHz         = I2C0_RATE,
        .IntrId         = SOC_IIC0_IRQn,
    },
#endif

#ifdef I2C1PS_ENABLE
    {
        .DeviceId       = 1,
        .BaseAddress    = I2C1__BASE_ADDR,
        .ClockHz        = I2C_CLOCK,
        .RateHz         = I2C1_RATE,
        .IntrId         = SOC_IIC1_IRQn
    },
#endif
};