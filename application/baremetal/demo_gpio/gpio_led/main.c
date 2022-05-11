/*
 * main.c
 *
 *  Created on: Dec.16.2021
 *      Author: haitao.liu
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"


#define LEDn  2
static uint32_t LED_CLORK[] = {SOC_LED_RED_GPIO_MASK, SOC_LED_GREEN_GPIO_MASK};

void led_config(void)
{
    for(int i=0; i<LEDn; i++)
    {
        //bank0
        AlGpio_EnableOutput(GPIO0,LED_CLORK[i]);
        AlGpio_Write(GPIO0,LED_CLORK[i],1);
        //bank1
        AlGpio_EnableOutput(GPIO1,LED_CLORK[i]);
        AlGpio_Write(GPIO1,LED_CLORK[i],1);
        //bank2
        AlGpio_EnableOutput(GPIO2,LED_CLORK[i]);
        AlGpio_Write(GPIO2,LED_CLORK[i],1);
        //bank3
        AlGpio_EnableOutput(GPIO3,LED_CLORK[i]);
        AlGpio_Write(GPIO3,LED_CLORK[i],1);
    }
}

int main(void)
{
	led_config();
    while(1)
    {
        for(int i=0; i<LEDn; i++)
        {
        	//bank0
            AlGpio_Toggle(GPIO0,LED_CLORK[i]);
            AlGpio_OutputRead(GPIO0,LED_CLORK[i]);
        	//bank1
            AlGpio_Toggle(GPIO1,LED_CLORK[i]);
            AlGpio_OutputRead(GPIO1,LED_CLORK[i]);
        	//bank2
            AlGpio_Toggle(GPIO2,LED_CLORK[i]);
            AlGpio_OutputRead(GPIO2,LED_CLORK[i]);
        	//bank3
            AlGpio_Toggle(GPIO3,LED_CLORK[i]);
            AlGpio_OutputRead(GPIO3,LED_CLORK[i]);
//            delay_1ms(1000);
            printf("AlGpio_Toggle\r\n");
//            anl_printf("AlGpio_Toggle\r\n");
        }
    }
}
