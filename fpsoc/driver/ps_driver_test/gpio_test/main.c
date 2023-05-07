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

int main(void)
{
    //*(AL_U32 *)(0xf8803054u) =0x4;
    printf("test start\r\n");

    AL_GPIO_InitStruct GPIO_InitStruct ={
        .BaseAddr       = GPIO_BASE_ADDR,
    };

    AL_S32 ret = AlGpio_Hal_Init(&GPIO, &GPIO_InitStruct, 0);
    printf("ret is %x\r\n", ret);
    if(&GPIO == AL_NULL) {
        printf("No &GPIO\r\n");
    }
        
    // test ddr, dr register and outputread,writePin (already ok!)
    AlGpio_Hal_WritePin(&GPIO,1,0x1);
    AlGpio_Hal_WritePin(&GPIO,100,0x1);  
    
    printf("GPIO 1 output data value is %x\r\n", AlGpio_Hal_OutputReadPin(&GPIO,1));
    printf("GPIO 100 output data value is %x\r\n", AlGpio_Hal_OutputReadPin(&GPIO,100));

    //test eoi register
    printf("GPIO 1 input data value is %x\r\n", AlGpio_Hal_ReadEXTRegisterOut(&GPIO,1));
    printf("GPIO 100 input data value is %x\r\n", AlGpio_Hal_ReadEXTRegisterOut(&GPIO,100));

    // test inputread  
    //printf("GPIO 21 input data value is %x\r\n", AlGpio_Hal_ReadEXTRegisterIn(&GPIO,21));
    //printf("GPIO 22 input data value is %x\r\n", AlGpio_Hal_ReadEXTRegisterIn(&GPIO,22));
        
    return 0;
}
