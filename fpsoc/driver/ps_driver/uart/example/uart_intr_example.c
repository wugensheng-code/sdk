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

AL_S32 UartIntrExample();

static AL_UART_InitStruct UART_InitStruct = {
        .BaudRate     = 115200,
        .Parity       = UART_NO_PARITY,
        .WordLength   = UART_CHAR_8BITS,
        .StopBits     = UART_STOP_1BIT,
    };

AL_S32 main(void)
{
    AL_S32 Status;

    Status = UartIntrExample();

    if (Status != AL_OK) {
        printf("Uart Intr Example Failed\r\n");
    }

    printf("Successfully run Uart Intr Example\r\n");

    return Status;
}

AL_S32 UartIntrExample(AL_VOID)
{
    AL_UART_HalStruct uart0_hal;
    AL_U16 Index;
    
    AL_S32 ret = AlUart_Hal_Init(&uart0_hal, 0, &UART_InitStruct);
    if (ret != AL_OK) {
        printf("AlUart_Hal_Init Error\r\n");
        return ret;
    }

    AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_SET_LOOPBACK , AL_FUNC_ENABLE);

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
        printf("AlUart_Hal_RecvDataPolling Error\r\n");
        return ret;
    }

    ret = AlUart_Hal_SendData(&uart0_hal, SendBuffer, TEST_BUFFER_SIZE);
    if (ret != AL_OK) {
        printf("AlUart_Hal_SendDataPolling Error\r\n");
        return ret;
    }

/*
* Check the receive buffer against the send buffer and verify the
* data was correctly received
*/
    for (Index = 0; Index < TEST_BUFFER_SIZE; Index++) {
        if (SendBuffer[Index] != RecvBuffer[Index]) {
            return -1;
        }
    }

    AlUart_Hal_IoCtl(&uart0_hal, AL_UART_IOCTL_SET_LOOPBACK , AL_FUNC_DISABLE);

    return AL_OK;
}


#ifdef __cplusplus
}
#endif
