/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    uart polling_example file
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-05-17
 * @brief   uart intr_example file
 */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_uart_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_BUFFER_SIZE 32

static AL_U8 SendBuffer[TEST_BUFFER_SIZE];	/* Buffer for Transmitting Data */
static AL_U8 RecvBuffer[TEST_BUFFER_SIZE];	/* Buffer for Receiving Data */

volatile AL_U32 TotalReceivedCount;
volatile AL_U32 TotalSentCount;

AL_S32 UartIntrExample();
static AL_VOID AlUart_Handler(AL_UART_EventStruct UartEvent, AL_VOID *CallbackRef);

static AL_UART_InitStruct UART_InitStruct = {
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,
    };

AL_S32 main(void)
{
    AL_S32 Status;
    AL_S32 Ret;

    Status = UartIntrExample();

    if (Status != AL_OK) {
        printf("Uart Intr Example Failed\r\n");
        return Status;
    }

    printf("Successfully run Uart Intr Example\r\n");

    return Status;
}

AL_S32 UartIntrExample(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U16 Index;
    AL_UART_IoctlParamUnion UartIoctlParam = {0};

    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct, AL_NULL);
    if (ret != AL_OK) {
        printf("AlUart_Hal_Init Error\r\n");
        return ret;
    }

    ret = AlUart_Dev_RegisterEventCallBack(uart0_hal.Dev, AlUart_Handler, &uart0_hal);
    if (ret != AL_OK) {
        return ret;
    }

    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    UartIoctlParam.LoopBack = AL_FUNC_ENABLE;
    AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_SET_LOOPBACK , &UartIoctlParam);

    /*
    * Initialize the send buffer bytes with a pattern and zero out
    * the receive buffer.
    */
    for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
        SendBuffer[Index] = '0' + Index;
        RecvBuffer[Index] = 0;
    }

    ret = AlUart_Hal_RecvData(&uart0_hal, RecvBuffer, TEST_BUFFER_SIZE);
    if (ret != AL_OK) {
        printf("AlUart_Hal_RecvData Error\r\n");
        return ret;
    }

    ret = AlUart_Hal_SendData(&uart0_hal, SendBuffer, TEST_BUFFER_SIZE);
    if (ret != AL_OK) {
        printf("AlUart_Hal_SendData Error\r\n");
        return ret;
    }

    while (1) {
        if((TotalReceivedCount== TEST_BUFFER_SIZE) &&
        (TotalSentCount == TEST_BUFFER_SIZE)) {
            break;
        }
    }

    /* Verify the entire receive buffer was successfully received */
    for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
        if (uart0_hal.Dev->SendBuffer.BufferPtr[Index] != uart0_hal.Dev->RecvBuffer.BufferPtr[Index]) {
            return -1;
        }
    }

    UartIoctlParam.LoopBack = AL_FUNC_DISABLE;
    AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_SET_LOOPBACK , &UartIoctlParam);

    return AL_OK;
}

static AL_VOID AlUart_Handler(AL_UART_EventStruct UartEvent, AL_VOID *CallbackRef)
{
    AL_UART_HalStruct *Handle = (AL_UART_HalStruct *)CallbackRef;

    /* All of the data has been sent */
    if (UartEvent.Events == AL_UART_EVENT_SEND_DONE) {
        TotalSentCount = UartEvent.EventData;
    }

    /* All of the data has been received */
    if (UartEvent.Events == AL_UART_EVENT_RECEIVE_DONE) {
        TotalReceivedCount = UartEvent.EventData;
    }

    /*
     * Data was received, but not the expected number of bytes, a
     * timeout just indicates the data stopped for 8 character times
     */
    if (UartEvent.Events == AL_UART_EVENT_CHAR_TIMEOUT) {
        TotalReceivedCount = UartEvent.EventData;
    }

}


#ifdef __cplusplus
}
#endif
