/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_gpio_ctl_LED.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-08
 * @brief   control LED example
 */

#include "al_gpio_hal.h"

#define LED 14

AL_S32 AlGpio_Hal_Ctl_LED_Example()
{
    AL_GPIO_HalStruct *GPIO;

    /* 1、Test AlGpio_Hal_Init */
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, 0, AL_NULL);

    if (ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init success");
    }
    else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init failed");
    }


    /* 2、Test LED function */
    while(1) {
        AlGpio_Hal_WritePin(GPIO, LED, 0x0);
        AlSys_MDelay(2000);
        AlGpio_Hal_WritePin(GPIO, LED, 0x1);
        AlSys_MDelay(2000);
    }

    return AL_OK;
}


AL_S32 main(void) {
    AL_LOG(AL_LOG_LEVEL_INFO, "[TEST]AlGpio_Hal_CTL_LED_Test start");
    AlGpio_Hal_Ctl_LED_Example();

    return AL_OK;
}