#include <stdio.h>
#include <stdlib.h>

#include "al_uart_dev.h"
#include "al_uart_hal.h"
#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

AL_U8 tx_buffer[50] ;
AL_U8 rx_buffer[50];


AL_VOID main()
{
    AL_UART_HalStruct uart0_hal;

    AL_UART_InitStruct UART_InitStruct = {
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,
    };

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, &UART_InitStruct, AL_NULL, 0);
    AL_U32 *RecvSize;

    while (1) {
        ret = AlUart_Hal_RecvDataBlock(&uart0_hal, rx_buffer, 50, RecvSize, 50);
        ret = AlUart_Hal_SendDataBlock(&uart0_hal, tx_buffer, 50, 50);
    }
}