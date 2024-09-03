/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_axi_uart_lite_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

static Al_AxiUartLite_HalStruct AlAxiUartLiteHandle[AL_UART_NUM_INSTANCE];

static AL_VOID AlAxiUartLite_Hal_DefEventHandler(Al_AxiUartLite_EventStruct UartEvent, AL_VOID *CallbackRef)
{
    Al_AxiUartLite_HalStruct *Handle = (Al_AxiUartLite_HalStruct *)CallbackRef;
    switch (UartEvent.Events)
    {
    case AL_AXI_UART_LITE_EVENT_SEND_DONE:
        AlOsal_Mb_Send(&Handle->TxEventQueue, &UartEvent);
        break;

    case AL_AXI_UART_LITE_EVENT_RECEIVE_DONE:
        AlOsal_Mb_Send(&Handle->RxEventQueue, &UartEvent);
        break;

    default:
        break;
    }
}

AL_S32 AlAxiUartLite_Hal_Init(Al_AxiUartLite_HalStruct **Handle, AL_U32 DevId, Al_AxiUartLite_EventCallBack Callback)
{
    AL_S32 Ret = AL_OK;
    AlAxiUartLite_HwConfigStruct *HwConfig = NULL;

    AL_ASSERT((Handle != AL_NULL), AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM);

    AL_INTR_AttrStrct AxiUartLiteAttr = {POSTIVE_EDGE_TRIGGER, 137};

    HwConfig = AlAxiUartLite_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlAxiUartLiteHandle[DevId];
    } else {
        return AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM;
    }

    Ret = AlAxiUartLite_Dev_Init(&(*Handle)->Dev, DevId);
    if (Ret != AL_OK) {
        return Ret;
    }

    if(Callback == AL_NULL) {
        Ret = AlAxiUartLite_Dev_RegisterEventCallBack(&(*Handle)->Dev, AlAxiUartLite_Hal_DefEventHandler, (AL_VOID *)*Handle);
    } else {
        Ret = AlAxiUartLite_Dev_RegisterEventCallBack(&(*Handle)->Dev, Callback, (AL_VOID *)*Handle);
    }
    if (Ret != AL_OK) {
        return Ret;
    }

    (AL_VOID)AlIntr_RegHandler((*Handle)->Dev.HwConfig.IntrId, &AxiUartLiteAttr, AlAxiUartLite_Dev_IntrHandler, &(*Handle)->Dev);

    Ret = AlOsal_Lock_Init(&(*Handle)->TxLock, "Uart-TxLock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Lock_Init(&(*Handle)->RxLock, "Uart-RxLock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&(*Handle)->TxEventQueue, "UART_TXDONE");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&(*Handle)->RxEventQueue, "UART_RXDONE");
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}


AL_S32 AlAxiUartLite_Hal_SendDataPolling(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM);

    Ret = AlAxiUartLite_Dev_SendDataPolling(&Handle->Dev, Data, Size);

    return Ret;
}

AL_S32 AlAxiUartLite_Hal_RecvDataPolling(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM);

    Ret = AlAxiUartLite_Dev_RecvDataPolling(&Handle->Dev, Data, Size);

    return Ret;
}

AL_S32 AlAxiUartLite_Hal_SendDataBlock(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    Al_AxiUartLite_EventStruct UartEvent = {0};

    AL_ASSERT((Handle != AL_NULL), AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->TxLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlAxiUartLite_Dev_SendData(&Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);
        return Ret;
    }
    Ret = AlOsal_Mb_Receive(&Handle->TxEventQueue, &UartEvent, Timeout);
    if (Ret != AL_OK) {
        AlAxiUartLite_EnableIntr(&Handle->Dev.HwConfig.BaseAddress, AL_FALSE);
        (AL_VOID)AlOsal_Mb_Receive(&Handle->TxEventQueue, &UartEvent, AL_WAITING_NO);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);

    if (Ret == AL_OK && (UartEvent.Events == AL_AXI_UART_LITE_EVENT_SEND_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_AXI_UART_LITE_EVENTS_TO_ERRS(UartEvent.Events);

    return Ret;
}

AL_S32 AlAxiUartLite_Hal_RecvDataBlock(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 RequestSize, AL_U32 *ReceiveSize, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    Al_AxiUartLite_EventStruct UartEvent = {0};

    AL_ASSERT((Handle != AL_NULL), AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->RxLock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlAxiUartLite_Dev_RecvData(&Handle->Dev, Data, RequestSize);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);
        return Ret;
    }

    Ret = AlOsal_Mb_Receive(&Handle->RxEventQueue, &UartEvent, Timeout);
    if (Ret != AL_OK) {
        AlAxiUartLite_EnableIntr(&Handle->Dev.HwConfig.BaseAddress, AL_FALSE);
        (AL_VOID)AlOsal_Mb_Receive(&Handle->RxEventQueue, &UartEvent, AL_WAITING_NO);
    }

    *ReceiveSize =  UartEvent.EventData;

    (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);

    if (Ret == AL_OK && (UartEvent.Events == AL_AXI_UART_LITE_EVENT_RECEIVE_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_AXI_UART_LITE_EVENTS_TO_ERRS(UartEvent.Events);
}

AL_S32 AlAxiUartLite_Hal_SendData(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->TxLock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlAxiUartLite_Dev_SendData(&Handle->Dev, Data, Size);

    (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);

    return Ret;
}

AL_S32 AlAxiUartLite_Hal_RecvData(Al_AxiUartLite_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->RxLock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlAxiUartLite_Dev_RecvData(&Handle->Dev, Data, Size);

    (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);

    return Ret;
}