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

static void IntrHandler(AL_VOID *CallBackRef, AL_U32 Bank, AL_U32 Status)
{
    AL_GPIO_DevStruct *GPIO = (AL_GPIO_DevStruct *)CallBackRef;
    printf("Enter CallbackHandler!\r\n");
}

AL_GPIO_HalStruct GPIO;

int main(void)
{
    printf("dev test start\r\n");
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, 0);
    printf("ret is %x\r\n", ret);
    if((&GPIO)->Dev == AL_NULL) {
        printf("No &GPIO\r\n");
    }

#if 0
    AlGpio_Dev_WritePin((&GPIO)->Dev, 13, 0x1);
    printf("GPIO Pin 13 output data value is %x\r\n", AlGpio_Dev_OutputReadPin((&GPIO)->Dev, 13));
    AlGpio_Dev_WritePin((&GPIO)->Dev, 13, 0x0);
    printf("GPIO Pin 13 output data value is %x\r\n", AlGpio_Dev_OutputReadPin((&GPIO)->Dev, 13));

    printf("-------------------------------------------------------\r\n");
    AlGpio_Dev_WriteBank((&GPIO)->Dev, 1, 0xfed0);
    printf("GPIO Bank 1 output data value is %x\r\n", AlGpio_Dev_OutputRead((&GPIO)->Dev, 1));
    AlGpio_Dev_WriteBank((&GPIO)->Dev, 1, 0x10);
    printf("GPIO Bank 1 output data value is %x\r\n", AlGpio_Dev_OutputRead((&GPIO)->Dev, 1));
#endif

#if 0
    printf("GPIO Pin 17 input data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,17));
    printf("GPIO Pin 15 input data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,15));
    printf("GPIO Bank 0 input data value is %x\r\n", AlGpio_Dev_InputRead((&GPIO)->Dev, 0));
#endif



    AlGpio_Dev_IntrCallbackHandler((&GPIO)->Dev, (void *)(&GPIO), (Gpio_Handler)IntrHandler);
    AlGpio_Dev_SetDirectionPin((&GPIO)->Dev, 0, GPIO_INPUT);
    AlGpio_Dev_IntrSetTypePin((&GPIO)->Dev, 0, GPIO_INTR_TYPE_LEVEL_LOW);
    AlGpio_Dev_IntrEnablePin((&GPIO)->Dev, 0);

    while(1);

    return 0;
}

#if 0
static void IntrHandler(AL_VOID *CallBackRef, AL_U32 Bank, AL_U32 Status)
{
    AL_GPIO_DevStruct *GPIO = (AL_GPIO_DevStruct *)CallBackRef;
    printf("Enter CallbackHandler!\r\n");
}

AL_GPIO_HalStruct GPIO;
AL_S32 PinVal[4];

int main(void)
{
    printf("hal test start\r\n");
    AL_S32 IntrVal;
    AL_S32 ret = AlGpio_Hal_Init(&GPIO, 0);
    printf("ret is %x\r\n", ret);
    if(&GPIO == AL_NULL) {
        printf("No &GPIO\r\n");
    }

    // test outputread,writePin (already ok!)
    AlGpio_Hal_WritePin(&GPIO,3,0x1);
    AlGpio_Hal_WritePin(&GPIO,99,0x1);
    PinVal[0] = AlGpio_Hal_OutputReadPin(&GPIO,3);
    PinVal[1] = AlGpio_Hal_OutputReadPin(&GPIO,99);
    printf("GPIO 3 output data value is %x\r\n", PinVal[0]);
    printf("GPIO 99 output data value is %x\r\n", PinVal[1]);

    // test eoi register (already ok!)
    for(int i = 0;i < 15;i++)
    {
        printf("GPIO 21 input data value is %x\r\n", AlGpio_Hal_InputReadPin(&GPIO,21));
    }

    // test intr
    AlGpio_Dev_IntrCallbackHandler((&GPIO)->Dev, (void *)(&GPIO), (Gpio_Handler)IntrHandler);
    printf("config BaseAddress is %x\r\n", (&GPIO)->Dev->HwConfig.BaseAddress);
    printf("=====> Enabling interrupts on bank ...\n");
    AlGpio_Hal_IntrCfg(&GPIO, 21, GPIO_INTR_TYPE_LEVEL_HIGH);

    while(1);

    return 0;
}
#endif


#if 0
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <al_type.h>
#include "nuclei_sdk_soc.h"
#include <sys/errno.h>

static int errors = 0;
static void merror(const char *msg)
{
    ++errors;
    printf("Error: %s\r\n", msg);
}

int main(void)
{
    void *p;
    int save;

    errno = 0;

    printf("----------------以下malloc(-1)-------------------\r\n");
    p = malloc(-1);
    save = errno;

    if (p != NULL)
        merror("malloc (-1) succeeded.");

    if (p == NULL && save != ENOMEM)
        merror("errno is not set correctly");

    if(p == NULL)
        merror("malloc (-1) returns NULL");


    printf("----------------以下malloc(10)-------------------\r\n");
//第一次内存分配
    p = malloc(10);
    if (p == NULL)
        merror("malloc (10) failed.");
	memcpy(p, "anlogic", 10);
	printf("first: p value is %s, p address is %p\r\n", p,p);

//重新分配内存
    printf("---------------以下realloc(p, 20)-------------------\r\n");
    /* realloc (p, 0) == free (p).  */
    p = realloc(p, 20);
    if (p != NULL)
        merror("realloc (p, 0) failed.");
	//check contents existed
	printf("second: p value is %s, p address is %p\r\n", p,p);
	free(p);

	printf("first case end\r\n");

	//p = realloc(NULL, 10);
	//write
	//strcpy(p, ".com");
	//read
	//printf("p value is %s, p address is %p\r\n", p,p);
	//free(p);


printf("---------------以下malloc(0)-------------------\r\n");
    p = malloc(0);
    if (p == NULL)
    {
        printf("malloc(0) returns NULL\r\n");
    } else {
        printf("malloc(0) is not NULL\r\n");
    }


printf("---------------以下realloc(p, 0)-------------------\r\n");
    p = realloc(p, 0);
    if (p != NULL) {
        merror("realloc (p, 0) failed.\r\n");
    } else {
        merror("realloc (p, 0) succeed.\r\n");
    }


    return errors != 0;
}
#endif
