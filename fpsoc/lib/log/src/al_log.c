#include "al_uart_hal.h"
#include "al_log.h"


#if ((LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1))
AL_UART_HalStruct AlLog;
#endif

AL_S32 AlLog_Init()
{
#if ((LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1))
    AL_UART_InitStruct UART_InitStruct = {
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,
    };

    return AlUart_Hal_Init(&AlLog, LOG_DEV, &UART_InitStruct, AL_NULL);
#else
    return AL_OK;
#endif
}

AL_U32 AlLog_Write(const void* Data, AL_U32 Len)
{
#if (LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1)
    return AlUart_Hal_SendDataPolling(&AlLog, (AL_U8 *)Data, Len);
#else
    (AL_VOID)Data;
    (AL_VOID)Len;
    return Len;
#endif
}
