/*
 * main.c
 *
 *  Created on: Dec.16.2021
 *      Author: haitao.liu
 */


#include <stdio.h>
#include <stdlib.h>
#include "al_gpio_hw.h"
#include "al_gpio_ll.h"
#include "al_gpio_dev.h"
#include "al_gpio_hal.h"

AL_GPIO_HalStruct GPIO;

static void IntrHandler(AL_VOID *CallBackRef, AL_U32 Bank, AL_U32 Status)
{
    AL_GPIO_DevStruct *GPIO = (AL_GPIO_DevStruct *)CallBackRef;
    printf("Enter CallbackHandler!\r\n");
}


AlGpio_Hal_Test()
{
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, 0);

    // 1、Test AlGpio_Hal_Init
    if (ret == AL_OK)
        printf("[TEST] APU AlGpio_Hal_Init success\r\n");
    else
        printf("[TEST] APU AlGpio_Hal_Init failed\r\n");

    // 2、Test Write and Read DR register
    AlGpio_Hal_WritePin(&GPIO,19,0x1);
    _delay_ms(2000);
    printf("GPIO 19 output data value is %x\r\n", AlGpio_Hal_OutputReadPin(&GPIO,19));
    AlGpio_Hal_WritePin(&GPIO,19,0x0);
    _delay_ms(2000);
    printf("GPIO 19 output data value is %x\r\n", AlGpio_Hal_OutputReadPin(&GPIO,19));


    // 3、Test InputRead EOI register
    for(int i = 0;i < 15;i++)
    {
        printf("GPIO 21 input data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,21));
    }

    // 4、Test intr
    AlGpio_Dev_IntrCallbackHandler((&GPIO)->Dev, (void *)(&GPIO), (Gpio_Handler)IntrHandler);
    AlGpio_Hal_IntrCfg(&GPIO, 21, GPIO_INTR_TYPE_LEVEL_HIGH);
    while(1);

    return 0;
}


AlGpio_Dev_Test()
{
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, 0);

    // 1.1、Test Write and Read DR register (Pin)
    AlGpio_Dev_WritePin((&GPIO)->Dev, 13, 0x1);
    printf("GPIO Pin 13 output data value is %x\r\n", AlGpio_Dev_OutputReadPin((&GPIO)->Dev, 13));
    AlGpio_Dev_WritePin((&GPIO)->Dev, 13, 0x0);
    printf("GPIO Pin 13 output data value is %x\r\n", AlGpio_Dev_OutputReadPin((&GPIO)->Dev, 13));

    // 1.2、Test Write and Read DR register (Bank)
    AlGpio_Dev_WriteBank((&GPIO)->Dev, 1, 0xfed0);
    printf("GPIO Bank 1 output data value is %x\r\n", AlGpio_Dev_OutputRead((&GPIO)->Dev, 1));
    AlGpio_Dev_WriteBank((&GPIO)->Dev, 1, 0x10);
    printf("GPIO Bank 1 output data value is %x\r\n", AlGpio_Dev_OutputRead((&GPIO)->Dev, 1));


    // 2.1、Test InputRead EOI register (Pin)
    printf("GPIO Pin 15 input data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,15));

    // 2.2、Test InputRead EOI register (Bank)
    printf("GPIO Bank 0 input data value is %x\r\n", AlGpio_Dev_InputRead((&GPIO)->Dev, 0));


    // 3、Test intr
    AlGpio_Dev_IntrCallbackHandler((&GPIO)->Dev, (void *)(&GPIO), (Gpio_Handler)IntrHandler);
    AlGpio_Dev_SetDirectionPin((&GPIO)->Dev, 0, GPIO_INPUT);
    AlGpio_Dev_IntrSetTypePin((&GPIO)->Dev, 0, GPIO_INTR_TYPE_LEVEL_LOW);
    AlGpio_Dev_IntrEnablePin((&GPIO)->Dev, 0);

    while(1);

    return 0;
}

int main(void)
{
#if 1
    printf("[TEST]AlGpio_Hal_Test start\r\n");
    AlGpio_Hal_Test();
#endif

#if 0
    printf("[TEST]AlGpio_Dev_Test start\r\n");
    AlGpio_Dev_Test();
#endif

    return 0;
}
