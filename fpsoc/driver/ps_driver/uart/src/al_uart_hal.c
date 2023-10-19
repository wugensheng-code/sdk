/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_uart_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_UART_HalStruct AlUartHandle[AL_UART_NUM_INSTANCE];
/********************************************************/

/**
 * This function action when receive or send data down.
 * @param   UartEvent Pointer to AL_UART_EventStruct contains event datas
 * @param   CallbackRef Pointer to a AL_UART_HalStruct structure that contains uart dev instance
 * @return
 * @note
*/
static AL_VOID AlUart_Hal_DefEventHandler(AL_UART_EventStruct UartEvent, AL_VOID *CallbackRef)
{
    AL_UART_HalStruct *Handle = (AL_UART_HalStruct *)CallbackRef;
    AL_S32 Ret = AL_OK;

    switch (UartEvent.Events)
    {
    case AL_UART_EVENT_SEND_DONE:
    case AL_UART_EVENT_BUSY_DETECT_TX:
         Ret = AlOsal_Mb_Send(&Handle->TxEventQueue, &UartEvent);
        break;

    case AL_UART_EVENT_RECEIVE_DONE:
    case AL_UART_EVENT_CHAR_TIMEOUT:
    case AL_UART_EVENT_OVER_RUN_ERR:
    case AL_UART_EVENT_BUSY_DETECT_RX:
        Ret = AlOsal_Mb_Send(&Handle->RxEventQueue, &UartEvent);
        break;

    case AL_UART_EVENT_MODEM_STATUS_INTR:
        AL_LOG(AL_LOG_LEVEL_INFO, "Get AL_UART_MODEM_STATUS_INTR \r\n");
        break;

    case AL_UART_EVENT_READY_TO_RECEIVE:
        break;

    case AL_UART_EVENT_READY_TO_SEND:
       break;

    default:
        break;
    }

    if (UartEvent.Events & (AL_UART_EVENT_PARITY_ERR | AL_UART_EVENT_FRAMING_ERR | AL_UART_EVENT_BREAK_INTR)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Get uart event: 0x%x", AL_UART_EVENTS_TO_ERRS(AL_UART_EVENT_PARITY_ERR));
        Ret = AlOsal_Mb_Send(&Handle->RxEventQueue, &UartEvent);
    }

    BUG_ON(Ret != AL_OK);
}

/**
 * This function initialize the UART mode according to the specified
 *          parameters in the AL_UART_InitStruct and initialize the associated handle.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart dev instance
 * @param   DevId is hardware module id
 * @param   InitConfig pointer to a AL_UART_InitStruct structure
 *          that contains the configuration information for the specified UART peripheral
 * @return
 *          - AL_OK for function success
 *          - Other for function failuregit
 * @note
*/
AL_S32 AlUart_Hal_Init(AL_UART_HalStruct **Handle, AL_U32 DevId,
                       AL_UART_InitStruct *InitConfig, AL_Uart_EventCallBack Callback)
{
    AL_S32 Ret = AL_OK;
    AL_UART_HwConfigStruct *HwConfig = NULL;

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    HwConfig = AlUart_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlUartHandle[DevId];
    } else {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    Ret = AlUart_Dev_Init(&(*Handle)->Dev, DevId, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    if(Callback == AL_NULL) {
        Ret = AlUart_Dev_RegisterEventCallBack(&(*Handle)->Dev, AlUart_Hal_DefEventHandler, (AL_VOID *)*Handle);
    } else {
        Ret = AlUart_Dev_RegisterEventCallBack(&(*Handle)->Dev, Callback, (AL_VOID *)*Handle);
    }
    if (Ret != AL_OK) {
        return Ret;
    }

    (AL_VOID)AlIntr_RegHandler((*Handle)->Dev.IntrNum, AL_NULL, AlUart_Dev_IntrHandler, &(*Handle)->Dev);

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

/**
 * This function send an amount of data in polling (non-interrupt) & blocking mode
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_SendDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;
    AL_S32 Timeout = AL_WAITFOREVER;

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->TxLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlUart_Dev_SendDataPolling(&Handle->Dev, Data, Size);

    (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);

    return Ret;
}

/**
 * This function receive an amount of data in polling(non interrupt) & blocking mode.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to receive
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_RecvDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->RxLock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlUart_Dev_RecvDataPolling(&Handle->Dev, Data, Size);
    (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);

    return Ret;
}

/**
 * This function send an amount of data in blocking & interrupt mode
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @param   Timeout Timeout duration
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_SendDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_UART_EventStruct UartEvent = {0};

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->TxLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlUart_Dev_SendData(&Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);
        return Ret;
    }

    Ret = AlOsal_Mb_Receive(&Handle->TxEventQueue, &UartEvent, Timeout);
    if (Ret != AL_OK) {
        AlUart_Dev_StopSend(&Handle->Dev);
        (AL_VOID)AlOsal_Mb_Receive(&Handle->TxEventQueue, &UartEvent, AL_WAITING_NO);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);

    if (Ret == AL_OK && (UartEvent.Events == AL_UART_EVENT_SEND_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_UART_EVENTS_TO_ERRS(UartEvent.Events);
}

/**
 * This function receive an amount of data in blocking mode, interrupt mode
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be received
 * @param   RecvSize Pointer to Amount how much data has been received
 * @param   Timeout Timeout duration
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_RecvDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 *RecvSize, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_UART_EventStruct UartEvent = {0};

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->RxLock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlUart_Dev_RecvData(&Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);
        return Ret;
    }

    Ret = AlOsal_Mb_Receive(&Handle->RxEventQueue, &UartEvent, Timeout);
    if (Ret != AL_OK) {
        AlUart_Dev_StopReceive(&Handle->Dev);
        (AL_VOID)AlOsal_Mb_Receive(&Handle->RxEventQueue, &UartEvent, AL_WAITING_NO);
    }

    *RecvSize =  UartEvent.EventData;

    (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);

    if (Ret == AL_OK && (UartEvent.Events == AL_UART_EVENT_RECEIVE_DONE ||
                         UartEvent.Events == AL_UART_EVENT_CHAR_TIMEOUT))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_UART_EVENTS_TO_ERRS(UartEvent.Events);
}

/**
 * This function send an amount of data in non-blocking, interrupt mode;
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_SendData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->TxLock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlUart_Dev_SendData(&Handle->Dev, Data, Size);

    (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);

    return Ret;
}

/**
 * This function receive an amount of data in non-blocking mode, interrupt mode
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be received
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_RecvData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->RxLock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlUart_Dev_RecvData(&Handle->Dev, Data, Size);

    (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);

    return Ret;
}


/**
 * get the last event for nonblock receive
 * @note
*/
AL_S32 AlUart_Hal_TryGetRecvLastEvent(AL_UART_HalStruct *Handle, AL_UART_EventStruct *Events)
{
    return AlOsal_Mb_Receive(&Handle->RxEventQueue, Events, 0);
}

/**
 * get the last event for nonblock send
 * @note
*/
AL_S32 AlUart_Hal_TryGetSendLastEvent(AL_UART_HalStruct *Handle, AL_UART_EventStruct *Events)
{
    return AlOsal_Mb_Send(&Handle->TxEventQueue, Events);
}


/**
 * This function excute operations to set or check uart status.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Cmd is io ctl cmd to AL_UART_IoCtlCmdEnum
 * @param   Data Pointer to cmd args
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_IoCtl(AL_UART_HalStruct *Handle, AL_UART_IoCtlCmdEnum Cmd, AL_UART_IoctlParamUnion *IoctlParam)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->RxLock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        goto ERR_1;
    }

    Ret = AlOsal_Lock_Take(&Handle->TxLock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        goto ERR_0;
    }

    Ret = AlUart_Dev_IoCtl(&Handle->Dev, Cmd, IoctlParam);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart Ioctl cmd error:%d\r\n", Ret);
    }

ERR_0:
    (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);

ERR_1:
    (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);

    return Ret;
}
