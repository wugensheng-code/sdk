/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_log.h"
#include "al_uart_hal.h"

#if ((LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1) || (LOG_DEV == AL_LOG_NONE))
AL_UART_HalStruct *AlLog;
#endif

AL_S32 AlLog_Init()
{
#ifdef LOG_DEV

#if ((LOG_DEV == AL_LOG_NONE))
    return AL_OK;

#elif ((LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1))
    AL_UART_InitStruct UART_InitStruct = {.BaudRate = 115200,
                                          .Parity = AL_UART_NO_PARITY,
                                          .WordLength = AL_UART_CHAR_8BITS,
                                          .StopBits = AL_UART_STOP_1BIT,
                                          .HwFlowCtl = AL_FALSE,
                                          .CharTimeoutEnable = AL_TRUE};

#if (LOG_DEV == AL_LOG_UART0)
#ifdef UART0_BAUDRATE
    UART_InitStruct.BaudRate = UART0_BAUDRATE;
#endif
#endif
#if (LOG_DEV == AL_LOG_UART1)
#ifdef UART1_BAUDRATE
    UART_InitStruct.BaudRate = UART1_BAUDRATE;
#endif
#endif

    return AlUart_Hal_Init(&AlLog, LOG_DEV, &UART_InitStruct, AL_NULL);

#endif
#endif
    return AL_OK;
}

AL_U32 AlLog_Write(const void *Data, AL_U32 Len)
{
#if (LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1)
    AL_U32 i;
    for (i = 0; i < Len; i++)
    {
        AlUart_Dev_SendByte(&(AlLog->Dev), ((const AL_S8 *)Data)[i]);
    }

    return Len;

#else
    (AL_VOID) Data;
    (AL_VOID) Len;
    return Len;
#endif
}

AL_VOID AlLog_WriteByte(AL_S8 Data)
{
#if (LOG_DEV == AL_LOG_UART0) || (LOG_DEV == AL_LOG_UART1)
    AlUart_Dev_SendByte(&(AlLog->Dev), Data);
#else
    (AL_VOID) Data;
#endif
}
