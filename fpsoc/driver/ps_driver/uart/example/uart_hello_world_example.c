/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/***************************** Include Files *********************************/
#include "al_uart_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AL_S32 UartHelloWorldExample();

static AL_UART_InitStruct UART_InitStruct = {
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,
    };

AL_S32 main(void)
{
    AL_S32 Status;

    Status = UartHelloWorldExample();

    if (Status != AL_OK) {
        printf("Uart Hello World Example Failed\r\n");
        return Status;
    }

    printf("Successfully run Uart Hello World Example\r\n");

    return Status;
}

AL_S32 UartHelloWorldExample(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    int SentCount = 0;
    AL_U8 HelloWorld[] = "Hello World";

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct, AL_NULL);
    if (ret != AL_OK) {
        printf("AlUart_Hal_Init error\r\n");
        return ret;
    }

    AL_S32 ret = AlUart_Hal_SendDataPolling(&uart0_hal, HelloWorld, sizeof(HelloWorld));
    if (ret != AL_OK) {
        printf("AlUart_Hal_SendDataPolling error\r\n");
        return ret;
    }

    printf("Hello Test Passed\r\n");

    return AL_OK;
}
