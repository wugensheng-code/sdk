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

static void IntrHandler(AL_VOID *CallBackRef, AL_U32 Bank, AL_U32 Status) {
    AL_GPIO_DevStruct *GPIO = (AL_GPIO_DevStruct *)CallBackRef;
    printf("Enter CallbackHandler!\r\n");
}

AL_GPIO_HalStruct GPIO;
AL_S32 PinVal[4];

int main(void)
{
    //*(AL_U32 *)(0xf8803054u) =0x4;
    printf("test start\r\n");
    AL_S32 IntrVal;

    AL_S32 ret = AlGpio_Hal_Init(&GPIO, 0);
    printf("ret is %x\r\n", ret);
    if(&GPIO == AL_NULL) {
        printf("No &GPIO\r\n");
    }
        
    // test ddr, dr register and outputread,writePin (already ok!)
    AlGpio_Hal_WritePin(&GPIO,3,0x1);
    AlGpio_Hal_WritePin(&GPIO,99,0x1);  
    PinVal[0] = AlGpio_Hal_OutputReadPin(&GPIO,3);
    PinVal[1] = AlGpio_Hal_OutputReadPin(&GPIO,99);
    printf("GPIO 3 output data value is %x\r\n", PinVal[0]);
    printf("GPIO 99 output data value is %x\r\n", PinVal[1]);

#if 0
    //test eoi register
    printf("GPIO 1 input data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,1));
    printf("GPIO 100 input data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,100));
#endif
    
    // test intr
    AlGpio_Hal_IntrCallbackHandler(&GPIO, (void *)(&GPIO), (Gpio_Handler)IntrHandler);
    
    printf("GPIO 21 input data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,21));
    
    printf("=====> Enabling interrupts on bank ...\n");
    
    AlGpio_Hal_IntrHandler(&GPIO, 21, GPIO_INTR_TYPE_EDGE_RISING);

    while(1);

    return 0;
}


#if 0
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <al_type.h>
#include "nuclei_sdk_soc.h"

extern char *_heap_start;
extern char *_heap_end;

int main(void)
{
    AL_U8 *str = "Hello World From Anlogic!";
    AL_U8 *p;
    AL_U32 Size = 0x1;

    //test printf
    for (int i = 0; i < 20; i ++) {
        printf("str = %s %p %p \r\n", str, _heap_start, _heap_end);
    }

    //test malloc & free
    for (int i = 0; i < 20; i ++)
    {
        p = malloc(Size);
        if (p == NULL) {
            printf("malloc failed");
        } else {
            printf("malloc succeed");
            
        }
        Size <<= 1;
        free(p);
    }

    return 0;
}
#endif
