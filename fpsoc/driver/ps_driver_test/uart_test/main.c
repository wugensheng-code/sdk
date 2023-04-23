#include <stdio.h>
#include <stdlib.h>

#include "al_uart_dev.h"
#include "al_uart_hal.h"
#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

AL_U8 tx_buffer[] = "ABCDEFGH12345678\r\n";



void main()
{
    AL_UART_HalStruct uart0_hal;

    AL_UART_InitStruct UART_InitStruct ={
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,
    };

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal,&UART_InitStruct,1);


}