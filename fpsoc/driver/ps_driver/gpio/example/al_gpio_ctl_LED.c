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
#define KEY 10
#define LED_IN_BANK0          0x4000
#define AL_GPIO_DEVICE_ID     0
#define AL_GPIO_DELAY_20MS    20
#define AL_GPIO_DELAY_2000MS  2000


/**
 *
 * This function demonstrates how to initialize the GPIO, write to a GPIO bank,
 * read the value of a GPIO pin, and perform GPIO polling to control an LED.
 *
 * @return AL_OK if the function executes successfully.
 *
 */
AL_S32 AlGpio_Hal_Ctl_LED_Example()
{
    AL_GPIO_HalStruct *GPIO;
    AL_S32 LedValue = 0;
    AL_S32 KeyValue = 0;

    /* 1、Test AlGpio_Hal_Init */
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, AL_GPIO_DEVICE_ID, AL_NULL);

    if (ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init success");
    }
    else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init failed");
    }


    /* 2、Test Gpio function through Bank. */
    AlGpio_Hal_WriteBank(GPIO, AL_GPIO_BANK0, LED_IN_BANK0);
    AlSys_MDelay(AL_GPIO_DELAY_2000MS);
    AlGpio_Hal_WriteBank(GPIO, AL_GPIO_BANK0, 0x0);
    AlSys_MDelay(AL_GPIO_DELAY_2000MS);


    /* 3、Test Gpio polling */
    LedValue = AlGpio_Hal_ReadPinOutput(GPIO, LED);
    AL_LOG(AL_LOG_LEVEL_INFO, "GPIO led value is 0x%x", LedValue);

    while(1)
    {
        KeyValue = AlGpio_Hal_ReadPinInput(GPIO, KEY);
        if(KeyValue == 0){
            AlSys_MDelay(AL_GPIO_DELAY_20MS);
            if (KeyValue == 0) {
                AlGpio_Hal_WritePin(GPIO, LED, ~LedValue);
                AlSys_MDelay(AL_GPIO_DELAY_20MS);
                AlGpio_Hal_WritePin(GPIO, LED, LedValue);
                AlSys_MDelay(AL_GPIO_DELAY_20MS);
            }
        }
    }

    return AL_OK;
}


AL_S32 main(void) {
    AL_LOG(AL_LOG_LEVEL_INFO, "[TEST]AlGpio_Hal_CTL_LED_Test start");
    AlGpio_Hal_Ctl_LED_Example();

    return AL_OK;
}