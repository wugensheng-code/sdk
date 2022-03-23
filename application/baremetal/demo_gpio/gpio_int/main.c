/*
 * main.c
 *
 *  Created on: Dec.22.2021
 *      Author: haitao.liu
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"


#define GPIO_NUM  32

static uint32_t GPIO_PIN_ARRAY[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

#define INT_MASK  	1
#define INT_UNMASK  0

int gpio0_irq_count=0;
int gpio1_irq_count=0;
int gpio2_irq_count=0;
int gpio3_irq_count=0;

uint8_t i=0;

//IN RTL- only GPIO0 set as input; GPIO1,2,3 set as out
// thus here only test GPIO0.
void gpio0_handler(void)
{
    printf("Enter gpio0_handler\r\n");
    gpio0_irq_count++;
    do
    {
    	i++;
    	printf("i=%d\r\n", i);
    }while(i<10);
    gpio_int_disable(GPIO0,(0x1<<GPIO_PIN_ARRAY[0]));
    printf("Exit gpio0_handler\r\n");
}

//void gpio1_handler(void)
//{
//    printf("Enter gpio1_handler\r\n");
//    gpio1_irq_count++;
//    gpio_int_disable(GPIO0,(0x1<<GPIO_PIN_ARRAY[1]));
//}


//void gpio2_handler(void)
//{
//    printf("Enter gpio2_handler\r\n");
//    gpio2_irq_count++;
//    if(gpio2_irq_count==3)
//    {
//		gpio2_irq_count=0;
//		gpio_int_disable(GPIO2,(0x1<<0));
//    }
//}
//
//
//void gpio3_handler(void)
//{
//    printf("Enter gpio3_handler\r\n");
//    gpio3_irq_count++;
//    if(gpio3_irq_count==3)
//    {
//        gpio3_irq_count=0;
//        gpio_int_disable(GPIO3,(0x1<<0));
//    }
//}


void gpio_config(void)
{	//caution:int cfg must before int enable
	gpio_enable_input(GPIO0,(0x1<<GPIO_PIN_ARRAY[0]));
	gpio_inttype_cfg(GPIO0,(0x1<<GPIO_PIN_ARRAY[0]),GPIO_INT_HIGH);
//	gpio_intmask_cfg(GPIO0,(0x1<<GPIO_PIN_ARRAY[0]),INT_MASK); // mask int
	gpio_int_enable(GPIO0,(0x1<<GPIO_PIN_ARRAY[0]));

//	//caution:int cfg must before int enable
//	gpio_enable_input(GPIO0,(0x1<<GPIO_PIN_ARRAY[1]));
//	gpio_inttype_cfg(GPIO0,(0x1<<GPIO_PIN_ARRAY[1]),GPIO_INT_LOW);
//	gpio_int_enable(GPIO0,(0x1<<GPIO_PIN_ARRAY[1]));

//	//caution:int cfg must before int enable
//	gpio_enable_input(GPIO2,(0x1<<GPIO_PIN_ARRAY[0]));
//	gpio_inttype_cfg(GPIO2,(0x1<<GPIO_PIN_ARRAY[0]),GPIO_INT_RISE);
//	gpio_int_enable(GPIO2,(0x1<<GPIO_PIN_ARRAY[0]));
//
//	//caution:int cfg must before int enable
//	gpio_enable_input(GPIO3,(0x1<<GPIO_PIN_ARRAY[0]));
//	gpio_inttype_cfg(GPIO3,(0x1<<GPIO_PIN_ARRAY[0]),GPIO_INT_FALL);
//	gpio_int_enable(GPIO3,(0x1<<GPIO_PIN_ARRAY[0]));
}


int main(void)
{
	printf("gpio interrupt test start:\r\n");
    __enable_irq();
	ECLIC_Register_IRQ(GPIO0_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
                                    ECLIC_LEVEL_TRIGGER, 1, 1,
                                    gpio0_handler);

//    ECLIC_Register_IRQ(GPIO1_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
//                                    ECLIC_LEVEL_TRIGGER, 1, 1,
//                                    gpio1_handler);

//    ECLIC_Register_IRQ(GPIO2_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
//                                    ECLIC_LEVEL_TRIGGER, 1, 1,
//                                    gpio2_handler);
//
//    ECLIC_Register_IRQ(GPIO3_IRQn, ECLIC_NON_VECTOR_INTERRUPT,
//                                    ECLIC_LEVEL_TRIGGER, 1, 1,
//                                    gpio3_handler);


    gpio_config();
    printf("gpio interrupt test finish.\r\n");
    while(1);
}
