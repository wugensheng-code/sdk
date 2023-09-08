/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_uart_blocked.c
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-09-01
 * @brief   uart blocked example
 */

/***************************** Include Files *********************************/
#include <string.h>
#include "al_uart_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define BUF_SIZE 16
#define AL_UART_TIME_OUT_MS 10000
#define AL_UART_DEVID 1

/************************** Variable Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/
static AL_UART_InitStruct UART_InitStruct = {
        .BaudRate           = 115200,
        .Parity             = AL_UART_NO_PARITY,
        .WordLength         = AL_UART_CHAR_8BITS,
        .StopBits           = AL_UART_STOP_1BIT,
        .HwFlowCtl          = AL_FALSE,
        .CharTimeoutEnable  = AL_TRUE
    };


/************************** Function Prototypes ******************************/
static AL_S32 AlUart_Test_RecvAndSendNonBlock(AL_VOID);

/************************** Function Definitions ******************************/

AL_S32 main(AL_VOID)
{
    AL_S32 Ret = AL_OK;

    AL_LOG(AL_LOG_LEVEL_INFO, "Uart Nonblocked test\r\n");

    Ret = AlUart_Test_RecvAndSendNonBlock();
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Nonblocked test failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Uart Nonblocked test success\r\n");

    return Ret;
}

static AL_S32 AlUart_Test_RecvAndSendNonBlock(AL_VOID)
{

    AL_UART_HalStruct *UartHandle;
    AL_U32 RecvSize;

    AL_U8 *Data = (AL_U8 *)malloc(BUF_SIZE);
    memset(Data, 0, (sizeof(AL_U8) * BUF_SIZE));

    AL_S32 Ret = AlUart_Hal_Init(&UartHandle, AL_UART_DEVID, &UART_InitStruct, AL_NULL);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Hal Init error:0x%x\r\n", Ret);
        return Ret;
    }
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_LOG(AL_LOG_LEVEL_INFO, "Send less than %d Bytes data and will show back\r\n", BUF_SIZE);

    while (1) {
        Ret = AlUart_Hal_RecvData(UartHandle, Data, BUF_SIZE);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart_Hal_RecvData Error\r\n");
            return Ret;
        }

        while (AlUart_Dev_IsRxBusy(UartHandle->Dev));

        Ret = AlUart_Hal_SendData(UartHandle, Data, UartHandle->Dev.RecvBuffer.HandledCnt);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlUart_Hal_SendData Error\r\n");
            return Ret;
        }
        AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");
    }


}
