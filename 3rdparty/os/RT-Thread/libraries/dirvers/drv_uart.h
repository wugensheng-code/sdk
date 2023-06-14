#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"

/* config class */
struct al_uart_config
{
    const char *name;
    AL_UART_InitStruct UART_InitStruct;
    AL_U32 DevId;
    AL_S32 IntrId; 
};

/* hbird uart dirver class */
struct al_uart
{
    AL_UART_HalStruct handle;
    struct al_uart_config *config;
    struct rt_serial_device serial;
};

int rt_hw_uart_init(void);

#endif /* __DRV_USART_H__ */

/******************* end of file *******************/