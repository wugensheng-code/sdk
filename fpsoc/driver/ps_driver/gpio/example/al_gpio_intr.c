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

#define PS_KEY 10
#define PL_KEY 54
#define AL_GPIO_DEVICE_ID 0


AL_S32 AlGpio_Hal_Intr_Example()
{
    AL_GPIO_HalStruct *GPIO;
    AL_S32 i = 0;

    /* 1、Test AlGpio_Hal_Init */
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, AL_GPIO_DEVICE_ID, AL_NULL);

    if (ret == AL_OK) {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init success");
    }
    else {
        AL_LOG(AL_LOG_LEVEL_INFO, "[TEST] APU AlGpio_Hal_Init failed");
    }

    /* 2、Test InputRead EXT register */
    for(i = 0; i < 6; i++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "GPIO Pin %d input data value is 0x%x", PL_KEY, AlGpio_Hal_ReadPinFromEXT(GPIO,PL_KEY));
    }

    /* 3、Test intr */
    AlGpio_Hal_IntrCfg(GPIO, PS_KEY, GPIO_INTR_TYPE_LEVEL_HIGH);
    AlGpio_Hal_IntrCfg(GPIO, PL_KEY, GPIO_INTR_TYPE_EDGE_FALLING);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    while(1);

    return AL_OK;
}


AL_S32 main(void) {
    AL_LOG(AL_LOG_LEVEL_INFO, "[TEST]AlGpio_Hal_Test start");
    AlGpio_Hal_Intr_Example();

    return AL_OK;
}
