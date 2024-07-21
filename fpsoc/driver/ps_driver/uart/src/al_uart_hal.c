/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_uart_hal.h"
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_UART_HalStruct AlUartHandle[AL_UART_NUM_INSTANCE];

AL_DMACAHB_HalStruct        *UartTxDmacHandle = AL_NULL;
AL_DMACAHB_ChInitStruct     UartTxDmacChConfig;
AL_DMACAHB_HalStruct        *UartRxDmacHandle = AL_NULL;
AL_DMACAHB_ChInitStruct     UartRxDmacChConfig;

/********************************************************/
/**
 * This function is called back when a UART event occurs and handles the event by sending it to the
 * appropriate message queue based on the type of event.
 *
 * @param UartEvent The UART event structure containing details about the event.
 * @param CallbackRef A void pointer to the UART handle structure.
 * @return void
 */
static AL_VOID AlUart_Hal_DefEventHandler(AL_UART_EventStruct UartEvent, AL_VOID *CallbackRef)
{
    AL_UART_HalStruct *Handle = (AL_UART_HalStruct *)CallbackRef;

    switch (UartEvent.Events)
    {
    case AL_UART_EVENT_SEND_DONE:
    case AL_UART_EVENT_BUSY_DETECT_TX:
        AlOsal_Mb_Send(&Handle->TxEventQueue, &UartEvent);
        break;

    case AL_UART_EVENT_RECEIVE_DONE:
    case AL_UART_EVENT_CHAR_TIMEOUT:
    case AL_UART_EVENT_OVER_RUN_ERR:
    case AL_UART_EVENT_BUSY_DETECT_RX:
        AlOsal_Mb_Send(&Handle->RxEventQueue, &UartEvent);
        break;

    case AL_UART_EVENT_MODEM_STATUS_INTR:
        AL_LOG(AL_LOG_LEVEL_INFO, "Uart experienced a modem status interrupt. \r\n");
        break;

    case AL_UART_EVENT_READY_TO_RECEIVE:
        break;

    case AL_UART_EVENT_READY_TO_SEND:
       break;

    default:
        break;
    }

    if (UartEvent.Events & (AL_UART_EVENT_PARITY_ERR | AL_UART_EVENT_FRAMING_ERR | AL_UART_EVENT_BREAK_INTR)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Uart receive error 0x%x .\r\n", AL_UART_EVENTS_TO_ERRS(UartEvent.Events));
        AlOsal_Mb_Send(&Handle->RxEventQueue, &UartEvent);
    }
}

/**
 * This function sets up the hardware configuration for the UART, initializes locks and message queues for TX
 * and RX, and registers the event callback function.
 *
 * @param Handle A pointer to a pointer of the UART handle structure to be initialized.
 * @param DevId The device ID of the UART to be initialized.
 * @param InitConfig A pointer to the UART initialization structure.
 * @param Callback The event callback function to be registered.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
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
 * This function sends data by polling  * the UART device until all data is sent or an error occurs.
 *
 * @param Handle A pointer to the UART handle structure.
 * @param Data A pointer to the data buffer to be sent.
 * @param Size The size of the data buffer to be sent.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlUart_Hal_SendDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->TxLock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlUart_Dev_SendDataPolling(&Handle->Dev, Data, Size);

    (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);

    return Ret;
}

/**
 * This function receives data by polling the UART device until the specified amount of data is received or an error occurs.
 *
 * @param Handle A pointer to the UART handle structure.
 * @param Data A pointer to the buffer where received data will be stored.
 * @param Size The size of the data buffer to be received.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
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
 * This function sends data and waits for a send complete event or an error event within a specified timeout period.
 *
 * @param Handle A pointer to the UART handle structure.
 * @param Data A pointer to the data buffer to be sent.
 * @param Size The size of the data buffer to be sent.
 * @param Timeout The timeout period in milliseconds.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
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
 * This function receives data and waits for a receive complete event or an error event within a specified timeout period.
 *
 * @param Handle A pointer to the UART handle structure.
 * @param Data A pointer to the buffer where received data will be stored.
 * @param RequestSize The size of the data buffer to be received.
 * @param ReceiveSize A pointer to a variable where the actual size of received data will be stored.
 * @param Timeout The timeout period in milliseconds.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlUart_Hal_RecvDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 RequestSize, AL_U32 *ReceiveSize, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_UART_EventStruct UartEvent = {0};

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->RxLock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlUart_Dev_RecvData(&Handle->Dev, Data, RequestSize);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);
        return Ret;
    }

    Ret = AlOsal_Mb_Receive(&Handle->RxEventQueue, &UartEvent, Timeout);
    if (Ret != AL_OK) {
        AlUart_Dev_StopReceive(&Handle->Dev);
        (AL_VOID)AlOsal_Mb_Receive(&Handle->RxEventQueue, &UartEvent, AL_WAITING_NO);
    }

    *ReceiveSize =  UartEvent.EventData;

    (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);

    if (Ret == AL_OK && (UartEvent.Events == AL_UART_EVENT_RECEIVE_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_UART_EVENTS_TO_ERRS(UartEvent.Events);
}

/**
 * This function sends data using the UART device.
 *
 * @param Handle A pointer to the UART handle structure.
 * @param Data A pointer to the data buffer to be sent.
 * @param Size The size of the data buffer to be sent.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
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
 * This function receives data using the UART device.
 *
 * @param Handle A pointer to the UART handle structure.
 * @param Data A pointer to the buffer where received data will be stored.
 * @param Size The size of the data buffer to be received.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
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
 * This function configures DMA for UART TX, sends data, and waits for a send complete event or an error event within a specified timeout period.
 *
 * @param Handle A pointer to the UART handle structure.
 * @param Data A pointer to the data buffer to be sent.
 * @param Size The size of the data buffer to be sent.
 * @param Timeout The timeout period in milliseconds.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlUart_Hal_SendDataDmaBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    AL_S32 Ret = AL_OK;
    AL_U32 DmacDevId = 0;
    AL_DMACAHB_HalStruct        *UartTxDmacHandlePtr;
    AL_DMACAHB_ChInitStruct     *UartTxDmacChConfigPtr;
    AL_DMACAHB_ChTransStruct    *UartTxDmacChTrans;

    UartTxDmacHandlePtr = UartTxDmacHandle;
    UartTxDmacChConfigPtr = &UartTxDmacChConfig;

    Ret = AlOsal_Lock_Take(&Handle->TxLock, Timeout);
    if (AL_OK != Ret) {
        return Ret;
    }

    if (UartTxDmacHandlePtr == NULL) {
        UartTxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_0;
        UartTxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        UartTxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        UartTxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        UartTxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        UartTxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        UartTxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        UartTxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        UartTxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        UartTxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;
        UartTxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_MEM2PER;
        if (Handle->Dev.DevId == 0) {
            UartTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_UART0_TX;
        } else {
            UartTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_UART1_TX;
        }
        UartTxDmacChConfigPtr->HandShaking.DstHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        UartTxDmacChConfigPtr->HandShaking.DstHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;
        UartTxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        UartTxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        UartTxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_6;
        UartTxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        UartTxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        UartTxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        UartTxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;

        Ret = AlDmacAhb_Hal_Init(&UartTxDmacHandlePtr, DmacDevId, UartTxDmacChConfigPtr, AL_NULL);
        if (AL_OK != Ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "UartTx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);
            return Ret;
        }
        UartTxDmacHandle = UartTxDmacHandlePtr;
    }

    UartTxDmacChTrans = &(UartTxDmacHandlePtr->Channel.Trans);
    UartTxDmacChTrans->SrcAddr        = (AL_REG)(Data);
    UartTxDmacChTrans->DstAddr        = Handle->Dev.BaseAddr + UART__RBR__THR__DLL__OFFSET;
    UartTxDmacChTrans->TransSize      = Size / (1 << UartTxDmacChConfigPtr->SrcTransWidth);

    Ret = AlDmacAhb_Hal_StartBlock(UartTxDmacHandlePtr, Timeout);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "UartTx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->TxLock);

    return Ret;
}

/**
 * This function receives data from UART using DMA in a blocking manner.
 *
 * @param Handle Pointer to the UART handle structure.
 * @param Data Pointer to the buffer where received data will be stored.
 * @param Size The size of the data to be received.
 * @param Timeout Timeout duration in milliseconds.
 * @return AL_S32 Returns AL_OK on success, error code otherwise.
 */
AL_S32 AlUart_Hal_RecvDataDmaBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{

    AL_ASSERT((Handle != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    AL_S32 Ret = AL_OK;
    AL_U32 DmacDevId = 0;
    AL_DMACAHB_HalStruct        *UartRxDmacHandlePtr;
    AL_DMACAHB_ChInitStruct     *UartRxDmacChConfigPtr;
    AL_DMACAHB_ChTransStruct    *UartRxDmacChTrans;

    UartRxDmacHandlePtr = UartRxDmacHandle;
    UartRxDmacChConfigPtr = &UartRxDmacChConfig;

    Ret = AlOsal_Lock_Take(&Handle->RxLock, Timeout);
    if (AL_OK != Ret) {
        return Ret;
    }

    if (UartRxDmacHandlePtr == NULL) {
        UartRxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_1;
        UartRxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        UartRxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        UartRxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        UartRxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        UartRxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        UartRxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        UartRxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        UartRxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_32;
        UartRxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;
        UartRxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_PER2MEM;
        if (Handle->Dev.DevId == 0) {
            UartRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_UART0_RX;
        } else {
            UartRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_UART1_RX;
        }
        UartRxDmacChConfigPtr->HandShaking.DstHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        UartRxDmacChConfigPtr->HandShaking.DstHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;
        UartRxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        UartRxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_1;
        UartRxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_6;
        UartRxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        UartRxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        UartRxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        UartRxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;

        Ret = AlDmacAhb_Hal_Init(&UartRxDmacHandlePtr, DmacDevId, UartRxDmacChConfigPtr, AL_NULL);
        if (AL_OK != Ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "UartRx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);
            return Ret;
        }
        UartRxDmacHandle = UartRxDmacHandlePtr;
    }

    UartRxDmacChTrans = &(UartRxDmacHandlePtr->Channel.Trans);
    UartRxDmacChTrans->SrcAddr        = Handle->Dev.BaseAddr + UART__RBR__THR__DLL__OFFSET;
    UartRxDmacChTrans->DstAddr        = (AL_REG)(Data);
    UartRxDmacChTrans->TransSize      = Size / (1 << UartRxDmacChConfigPtr->SrcTransWidth);

    Ret = AlDmacAhb_Hal_StartBlock(UartRxDmacHandlePtr, Timeout);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "UartRx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->RxLock);

    return Ret;
}


/**
 * This function performs an IOCTL command on the UART device.
 *
 * @param Handle Pointer to the UART handle structure.
 * @param Cmd The IOCTL command to be performed.
 * @param IoctlParam Pointer to the IOCTL parameter union.
 * @return AL_S32 Returns AL_OK on success, error code otherwise.
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
