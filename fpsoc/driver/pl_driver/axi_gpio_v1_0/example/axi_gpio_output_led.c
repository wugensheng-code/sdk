/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_gpio_hal.h"

int main()
{
    AL_U32 Val = 0;
    AL_S32 ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Axi gpio output test!");

    /* Instantiate a axi gpio and init it */
    AlAxiGpio_Hal_Struct *Gpio;
    ret = AlAxiGpio_Hal_Init(&Gpio, 0, AL_NULL);
    if (ret != AL_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Axi gpio init failed!");
        return ret;
    }

    while (1)
    {
        /* Toggle led */
        Val = ~Val;
        AlAxiGpio_Hal_WritePin(&Gpio, AL_AXI_GPIO_CHANNEL1, 0, Val);
        AlSys_MDelay(200);
    }
    
    return 0;
}