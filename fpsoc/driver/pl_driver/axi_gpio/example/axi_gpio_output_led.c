/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_gpio_hal.h"

#define USE_BIT

static AL_VOID ReleasePorts(AL_VOID)
{
    AL_REG32_SET_BIT(0xF8800080, 1, 0);
    AL_REG32_SET_BIT(0xF8801078, 0, 1);
    AL_REG32_SET_BIT(0xF8801078, 4, 1);
}

int main()
{
    AL_U32 Bit_Val = 0;
    AL_U32 Val = 0xffffffff;
    AL_S32 ret = AL_OK;

    ReleasePorts();

    AL_LOG(AL_LOG_LEVEL_INFO, "Axi gpio output test!");

    /* Instantiate a axi gpio and init it */
    AlAxiGpio_Hal_Struct *Gpio0;
    AlAxiGpio_Hal_Struct *Gpio1;

    ret = AlAxiGpio_Hal_Init(&Gpio0, AXI_GPIO0_DEVICE_ID, AL_NULL);
    if (ret != AL_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Axi gpio0 init failed!");
        return ret;
    }

    ret = AlAxiGpio_Hal_Init(&Gpio1, AXI_GPIO1_DEVICE_ID, AL_NULL);
    if (ret != AL_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Axi gpio1 init failed!");
        return ret;
    }

    while (1)
    {
    #ifdef USE_BIT
        /* Toggle led */
        Bit_Val = ~Bit_Val;
        AlAxiGpio_Hal_WritePin(Gpio0, AL_AXI_GPIO_CHANNEL1, 15, Bit_Val);
        AlAxiGpio_Hal_WritePin(Gpio1, AL_AXI_GPIO_CHANNEL2, 14, Bit_Val);
    #else
        Val = ~Val;
        AlAxiGpio_Hal_WriteChannel(Gpio0, AL_AXI_GPIO_CHANNEL1, Val);
        AlAxiGpio_Hal_WriteChannel(Gpio1, AL_AXI_GPIO_CHANNEL2, Val);
    #endif
        AlSys_MDelay(500);
    }

    return 0;
}