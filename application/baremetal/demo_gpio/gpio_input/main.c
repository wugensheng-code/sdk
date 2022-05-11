/*
 * main.c
 *
 *  Created on: Dec.22.2021
 *      Author: Administrator
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"


#define LEDn  2

//static uint32_t GPIO_CLORK[] = {2,3,4,5,6,7,8,9,10,11};
static uint32_t GPIO_CLORK[] = {0,1};
void gpio_config(void)
{
    for(int i=0; i<LEDn; i++)
    {
        AlGpio_EnableInput(GPIO0,1<<GPIO_CLORK[i]);
    }
}

int main(void)
{
    gpio_config();
    printf("test start \r\n");
    while(1)
    {
        for(int i=0; i<LEDn; i++)
        {
            if(0!=AlGpio_InputRead(GPIO0,1<<GPIO_CLORK[i]))
            {
                printf("GPIO[%d] input data value is %d\r\n",GPIO_CLORK[i],AlGpio_InputRead(GPIO0,1<<GPIO_CLORK[i]));
            }
        }
    }
    return 0;
}

