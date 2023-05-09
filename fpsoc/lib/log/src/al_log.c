#include "al_log.h"

#ifndef LOG_DEV
#define LOG_DEV AL_LOG_UART0
#endif

#if (LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1)

#include "al_uart_hal.h"
static AL_UART_HalStruct Log;

#else
#endif


AL_S32 AlLog_Init()
{
#if (LOG_DEV == AL_LOG_UART0)
    AL_UART_InitStruct UART_InitStruct = {
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,
    };

    return AlUart_Hal_Init(&Log, AL_LOG_UART0, &UART_InitStruct);
#else
    return AL_OK;
#endif
}

AL_U32 AlLog_Write(const void* Data, AL_U32 Len)
{
#if (LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1)
    AL_S32 ret = AlUart_Hal_SendDataPolling(&Log, Data, Len);
#else
    (AL_VOID)Data;
    (AL_VOID)Len;

#endif

    return Len;
}
