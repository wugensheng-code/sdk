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
        gpio_enable_output(GPIO0,LED_CLORK[i]);
        gpio_write(GPIO0,LED_CLORK[i],1);
        //bank1
        gpio_enable_output(GPIO1,LED_CLORK[i]);
        gpio_write(GPIO1,LED_CLORK[i],1);
        //bank2
        gpio_enable_output(GPIO2,LED_CLORK[i]);
        gpio_write(GPIO2,LED_CLORK[i],1);
        //bank3
        gpio_enable_output(GPIO3,LED_CLORK[i]);
        gpio_write(GPIO3,LED_CLORK[i],1);
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
            gpio_toggle(GPIO0,LED_CLORK[i]);
            gpio_output_read(GPIO0,LED_CLORK[i]);
        	//bank1
            gpio_toggle(GPIO1,LED_CLORK[i]);
            gpio_output_read(GPIO1,LED_CLORK[i]);
        	//bank2
            gpio_toggle(GPIO2,LED_CLORK[i]);
            gpio_output_read(GPIO2,LED_CLORK[i]);
        	//bank3
            gpio_toggle(GPIO3,LED_CLORK[i]);
            gpio_output_read(GPIO3,LED_CLORK[i]);
//            delay_1ms(1000);
            printf("gpio_toggle\r\n");
//            anl_printf("gpio_toggle\r\n");
        }
    }
}
