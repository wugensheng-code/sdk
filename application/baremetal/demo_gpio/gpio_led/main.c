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
static uint32_t LED_CLORK[2] = {SOC_LED_RED_GPIO_MASK, SOC_LED_GREEN_GPIO_MASK};

void led_config(void)
{
    //for(int i=0; i<LEDn; i++)

        //bank0
        AlGpio_EnableOutput(GPIO0,1<<19);
        AlGpio_Write(GPIO0,LED_CLORK[0],19);
        //bank1
        //AlGpio_EnableOutput(GPIO1,LED_CLORK[i]);
        //AlGpio_Write(GPIO1,LED_CLORK[i],19);
        //bank2
        //AlGpio_EnableOutput(GPIO2,LED_CLORK[i]);
        //AlGpio_Write(GPIO2,LED_CLORK[i],19);
        //bank3
       // AlGpio_EnableOutput(GPIO3,LED_CLORK[i]);
        //AlGpio_Write(GPIO3,LED_CLORK[i],19);

}

int main(void)
{
	Enablepinmux1();
    AlGpio_EnableOutput(GPIO0,1<<20);
    while(1)
    {
    	AlGpio_Write(GPIO0,(uint32_t)(1<<20),19);
        _delay_ms(1000);
        AlGpio_WriteLow(GPIO0,(uint32_t)(1<<20),19);
        _delay_ms(1000);
        printf("AlGpio_Toggle\r\n");
//            anl_printf("AlGpio_Toggle\r\n");

    }
}
