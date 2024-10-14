/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_UART_LITE_HAL_H_
#define AL_AXI_UART_LITE_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_axi_uart_lite_dev.h"


typedef struct
{
    Al_AxiUartLite_DevStruct         Dev;
    AL_Lock                      TxLock;
    AL_Lock                      RxLock;
    AL_MailBox                   TxEventQueue;
    AL_MailBox                   RxEventQueue;
} Al_AxiUartLite_HalStruct;

AL_S32 AlAxiUartLite_Hal_Init(Al_AxiUartLite_HalStruct **Handle, AL_U32 DevId, Al_AxiUartLite_EventCallBack Callback);
AL_S32 AlAxiUartLite_Hal_SendDataBlock(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_S32 AlAxiUartLite_Hal_RecvDataBlock(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 RequestSize, AL_U32 *ReceiveSize, AL_U32 Timeout);
AL_S32 AlAxiUartLite_Hal_SendDataPolling(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);
AL_S32 AlAxiUartLite_Hal_RecvDataPolling(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);
AL_S32 AlAxiUartLite_Hal_SendData(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);
AL_S32 AlAxiUartLite_Hal_RecvData(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

#ifdef __cplusplus
}
#endif

#endif /* AL_AXI_UART_LITE_HAL_H_ */
