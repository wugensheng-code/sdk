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
    printf("test start\r\n");

    AL_GPIO_InitStruct GPIO_InitStruct ={
        .BaseAddr       = GPIO_BASE_ADDR,
    };

    AL_S32 ret = AlGpio_Hal_Init(&GPIO, &GPIO_InitStruct, 0);
    printf("ret is %x\r\n", ret);
    if(&GPIO == AL_NULL) {
        printf("No &GPIO\r\n");
    }
    
    //test ddr, dr register and outputread,writePin
    AlGpio_Hal_WritePin(&GPIO,1,0x1);
    AlGpio_Hal_WritePin(&GPIO,100,0x1);  
    
    printf("GPIO 1 output data value is %x\r\n", AlGpio_Hal_OutputReadPin(&GPIO,1));//0
    printf("GPIO 3 output data value is %x\r\n", AlGpio_Hal_OutputReadPin(&GPIO,3));//0
    printf("GPIO 100 output data value is %x\r\n", AlGpio_Hal_OutputReadPin(&GPIO,100));//0
    
    ////test eoi register
    printf("GPIO 1 input data value is %x\r\n", AlGpio_Hal_ReadEOIRegisterOut(&GPIO,1));//
    printf("GPIO 3 input data value is %x\r\n", AlGpio_Hal_ReadEOIRegisterOut(&GPIO,3));//
    
    // test inputread  
    //printf("GPIO 21 output data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,21));//0
    //printf("GPIO 22 output data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,22));//1
        
     
    

    
    return 0;
}
