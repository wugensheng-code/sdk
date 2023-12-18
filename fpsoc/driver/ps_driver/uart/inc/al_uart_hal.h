/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_UART_HAL_H_
#define __AL_UART_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_uart_dev.h"

typedef enum {
    AL_UART_TX_BLOCK    = 0,
    AL_UART_TX_NONBLOCK = 1,
} AL_UART_TxModeEnum;


typedef enum {
    AL_UART_RECV_BLOCK    = 0,
    AL_UART_RECV_NONBLOCK = 1,
} AL_UART_RxModeEnum;


typedef struct
{
    AL_UART_DevStruct            Dev;
    AL_Lock                      TxLock;
    AL_Lock                      RxLock;
    AL_MailBox                   TxEventQueue;
    AL_MailBox                   RxEventQueue;
} AL_UART_HalStruct;


AL_S32 AlUart_Hal_Init(AL_UART_HalStruct **Handle, AL_U32 DevId,
                       AL_UART_InitStruct *InitConfig, AL_Uart_EventCallBack Callback);

AL_S32 AlUart_Hal_SendData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_RecvData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_SendDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlUart_Hal_RecvDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 NeedSize, AL_U32 *RealSize, AL_U64 Timeout);

AL_S32 AlUart_Hal_SendDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_RecvDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_SendDataDmaBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlUart_Hal_RecvDataDmaBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlUart_Hal_IoCtl(AL_UART_HalStruct *Handle, AL_UART_IoCtlCmdEnum Cmd, AL_UART_IoctlParamUnion *IoctlParam);


#ifdef __cplusplus
}
#endif

#endif
