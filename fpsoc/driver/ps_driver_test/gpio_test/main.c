/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gpio_hal.h"

#define LED 14
#define IntrPin 21

int AlGpio_Hal_Test()
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


    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);


    /* 2、Test LED function */
    AlGpio_Hal_WritePin(GPIO, LED, 0x0);
    AlGpio_Hal_ReadPinOutput(GPIO, LED);
    AlGpio_Hal_WritePin(GPIO, LED, 0x1);
    AlGpio_Hal_ReadPinOutput(GPIO, LED);

    /* 3、Test InputRead EOI register */
    for(int i = 0;i < 15;i++) {
        AL_LOG(AL_LOG_LEVEL_INFO, "GPIO IntrPin input data value is %x", AlGpio_Hal_ReadPinInput(GPIO,IntrPin));
    }

    /* 4、Test intr */
    AlGpio_Hal_IntrCfg(GPIO, IntrPin, GPIO_INTR_TYPE_LEVEL_LOW);
    while(1);

    return 0;
}


int main(void) {
    AL_LOG(AL_LOG_LEVEL_INFO, "[TEST]AlGpio_Hal_Test start");
    AlGpio_Hal_Test();

    return 0;
}
