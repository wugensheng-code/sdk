/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_gpio_intr.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-08
 * @brief   gpio intrrupt example
 */
#include "al_gpio_hal.h"

#define IntrPin 21

AL_S32 AlGpio_Hal_Intr_Example()
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

    /* 2、Test InputRead EOI register */
    AlGpio_Hal_ReadPin(GPIO,IntrPin);

    /* 3、Test intr */
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    AlGpio_Hal_IntrCfg(GPIO, IntrPin, GPIO_INTR_TYPE_LEVEL_LOW);
    while(1);

    return AL_OK;
}


AL_S32 main(void) {
    AL_LOG(AL_LOG_LEVEL_INFO, "[TEST]AlGpio_Hal_Test start");
    AlGpio_Hal_Intr_Example();

    return AL_OK;
}
