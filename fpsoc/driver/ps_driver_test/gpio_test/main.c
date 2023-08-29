/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gpio_hal.h"

AL_GPIO_HalStruct GPIO;


AlGpio_Hal_Test()
{
    /* 1、Test AlGpio_Hal_Init */
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, 0, AL_NULL);

    if (ret == AL_OK)
        printf("[TEST] APU AlGpio_Hal_Init success\r\n");
    else
        printf("[TEST] APU AlGpio_Hal_Init failed\r\n");

    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);


    /* 2、Test Write and Read DR register */
    AlGpio_Hal_WritePin(&GPIO,19,0x1);
    AlSys_MDelay(2000);
    printf("GPIO 19 output data value is %x\r\n", AlGpio_Hal_OutputReadPin(&GPIO,19));
    AlGpio_Hal_WritePin(&GPIO,19,0x0);
    AlSys_MDelay(2000);
    printf("GPIO 19 output data value is %x\r\n", AlGpio_Hal_OutputReadPin(&GPIO,19));


    /* 3、Test InputRead EOI register */
    for(int i = 0;i < 15;i++)
    {
        printf("GPIO 21 input data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,21));
    }

    /* 4、Test intr */
    AlGpio_Hal_IntrCfg(&GPIO, 21, GPIO_INTR_TYPE_LEVEL_LOW);
    while(1);

    return 0;
}


int main(void)
{
    printf("[TEST]AlGpio_Hal_Test start\r\n");
    AlGpio_Hal_Test();

    return 0;
}
