#include "al_uart_hal.h"

static AL_UART_HalStruct uart0_hal;

AL_S32 AlLog_Init()
{
    AL_UART_InitStruct UART_InitStruct = {
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,
    };

    return AlUart_Hal_Init(&uart0_hal, &UART_InitStruct, AL_NULL, 0);
}

AL_U32 AlLog_Write(const void* Data, AL_U32 Len)
{
    AL_S32 ret = AlUart_Hal_SendDataBlock(&uart0_hal, Data, Len, WAIT_FOREVER);

    /*
     * fixme
     */
    return Len;
}
