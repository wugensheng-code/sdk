/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_qspi_hal.h"
#include "al_intr.h"
#include "al_dmacahb_hal.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
AL_QSPI_HalStruct AlQspiHandle[AL_QSPI_NUM_INSTANCE];

AL_DMACAHB_ChInitStruct     QspiTxDmacChConfig;
AL_DMACAHB_HalStruct        *QspiTxDmacHandle = AL_NULL;
AL_DMACAHB_ChInitStruct     QspiRxDmacChConfig;
AL_DMACAHB_HalStruct        *QspiRxDmacHandle = AL_NULL;

/********************************************************/
/**
 *
 * This function waits for the QSPI transmission to complete or until the specified timeout period elapses.
 * It checks the QSPI status register for the busy flag to determine if the transmission is still ongoing.
 * If the transmission completes within the timeout period, it disables the slave select signal.
 *
 * @param Handle Pointer to the AL_QSPI_HalStruct structure that holds the QSPI device's configuration and state.
 * @param Event Pointer to the AL_QSPI_EventStruct structure where the event details will be stored.
 * @param Timeout Maximum time to wait for the transmission to complete, in milliseconds.
 * @return Returns AL_OK if the transmission completes successfully within the timeout period, otherwise returns an error code indicating the type of failure (e.g., timeout).
 *
 */
static AL_S32 AlQspi_Hal_WaitTxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_QSPI_EventStruct *Event, AL_U32 Timeout)
{
#ifdef QSPI_DEBUG
    AlQspi_Dev_DumpReg(Handle->Dev.HwConfig.BaseAddress);
#endif
    AL_S32 Ret;

    Ret = AlOsal_Mb_Receive(&Handle->TxEventQueue, Event, Timeout);
    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));

    AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, QSPI_SER_SS_DIS);

    return Ret;
}

/**
 *
 * Similar to AlQspi_Hal_WaitTxDoneOrTimeout, this function waits for the QSPI reception to complete or until the specified timeout period elapses.
 * It monitors the QSPI status register for the busy flag to check if the reception is still in progress.
 * Upon completion or timeout, it disables the slave select signal.
 *
 * @param Handle Pointer to the AL_QSPI_HalStruct structure that holds the QSPI device's configuration and state.
 * @param Event Pointer to the AL_QSPI_EventStruct structure where the event details will be stored.
 * @param Timeout Maximum time to wait for the reception to complete, in milliseconds.
 * @return Returns AL_OK if the reception completes successfully within the timeout period, otherwise returns an error code indicating the type of failure (e.g., timeout).
 */
static AL_S32 AlQspi_Hal_WaitRxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_QSPI_EventStruct *Event, AL_U32 Timeout)
{
#ifdef QSPI_DEBUG
    AlQspi_Dev_DumpReg(Handle->Dev.HwConfig.BaseAddress);
#endif
    AL_S32 Ret;

    Ret = AlOsal_Mb_Receive(&Handle->RxEventQueue, Event, Timeout);
    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));

    AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, QSPI_SER_SS_DIS);

    return Ret;
}

/**
 *
 * This function waits for both QSPI transmission and reception to complete or until the specified timeout period elapses.
 * It first waits for the transmission to complete and then for the reception to complete, each within the specified timeout period.
 * If either operation does not complete within its timeout period, it disables the slave select signal and returns an error.
 *
 * @param Handle Pointer to the AL_QSPI_HalStruct structure that holds the QSPI device's configuration and state.
 * @param Event Pointer to the AL_QSPI_EventStruct structure where the event details will be stored.
 * @param Timeout Maximum time to wait for both transmission and reception to complete, in milliseconds.
 * @return Returns AL_OK if both transmission and reception complete successfully within their respective timeout periods, otherwise returns an error code indicating the type of failure (e.g., timeout).
 * @note This function is static and intended for internal use within the QSPI HAL module.
 */
static AL_S32 AlQspi_Hal_WaitTxRxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_QSPI_EventStruct *Event, AL_U32 Timeout)
{
#ifdef QSPI_DEBUG
    AlQspi_Dev_DumpReg(Handle->Dev.HwConfig.BaseAddress);
#endif
    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->TxEventQueue, Event, Timeout);
    if (AL_OK != Ret) {
        AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, QSPI_SER_SS_DIS);
        return Ret;
    }

    Ret = AlOsal_Mb_Receive(&Handle->RxEventQueue, Event, Timeout);
    if (AL_OK != Ret) {
        AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, QSPI_SER_SS_DIS);
        return Ret;
    }

    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));

    AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, QSPI_SER_SS_DIS);

    return Ret;
}


/**
 *
 * This function is the default event callback that is called upon the occurrence of a QSPI event.
 * It handles various QSPI events such as send done, send timeout, receive done, and receive timeout by posting these events to the respective event queues.
 *
 * @param QspiEvent The QSPI event structure that contains details about the occurred event.
 * @param CallbackRef User-defined callback reference, which in this case is expected to be a pointer to the AL_QSPI_HalStruct structure.
 * @return Always returns AL_OK.
 */
static AL_VOID AlQspi_DefEventCallBack(AL_QSPI_EventStruct QspiEvent, void *CallbackRef)
{
    AL_QSPI_HalStruct *Handle = (AL_QSPI_HalStruct *)CallbackRef;

    switch (QspiEvent.Events) {
    case AL_QSPI_SEND_DONE:
    case AL_QSPI_SEND_TIMEOUT:
    case AL_QSPI_TX_FO:
    case AL_QSPI_TX_FIFO_EMPTY:
        AlOsal_Mb_Send(&Handle->TxEventQueue, &QspiEvent);
        break;
    case AL_QSPI_RECEIVE_DONE:
    case AL_QSPI_RECEIVE_TIMEOUT:
    case AL_QSPI_RX_FO:
    case AL_QSPI_RX_FU:
    case AL_QSPI_RX_FIFO_FULL:
        AlOsal_Mb_Send(&Handle->RxEventQueue, &QspiEvent);
        break;
    default:
        break;
    }
}

/**
 *
 * This function initializes the QSPI peripheral with the specified configurations.
 * It looks up the hardware configuration based on the provided device ID, initializes the QSPI device structure,
 * and registers the provided callback function for QSPI events.
 * If no callback is provided, it registers a default event callback function.
 *
 * @param Handle Double pointer to the AL_QSPI_HalStruct structure that will be initialized and represent the QSPI device.
 * @param InitConfig Pointer to the AL_QSPI_ConfigsStruct structure containing the desired QSPI configurations.
 * @param Callback Function pointer to the QSPI event callback function. If NULL, a default callback is used.
 * @param DevId Hardware module ID of the QSPI peripheral to be initialized.
 * @return Returns AL_OK if the initialization is successful, otherwise returns an error code indicating the failure reason.
 */
AL_S32 AlQspi_Hal_Init(AL_QSPI_HalStruct **Handle, AL_QSPI_ConfigsStruct *InitConfig, QSPI_EventCallBack Callback, AL_U32 DevId)
{
    AL_S32 Ret = AL_OK;
    AL_QSPI_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT((Handle != AL_NULL), AL_QSPI_ERR_ILLEGAL_PARAM);

    HwConfig = AlQspi_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlQspiHandle[DevId];
    } else {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    Ret = AlQspi_Dev_Init(&(*Handle)->Dev, HwConfig, InitConfig);
    if (AL_OK != Ret) {
        return Ret;
    }
    (*Handle)->Dev.Configs.Trans.SlvSelEnum = HwConfig->CsSel;

    if (AL_NULL == Callback) {
        Ret = AlQspi_Dev_RegisterIntrCallBack(&(*Handle)->Dev, AlQspi_DefEventCallBack, (void *)*Handle);
    } else {
        Ret = AlQspi_Dev_RegisterIntrCallBack(&(*Handle)->Dev, Callback, (void *)*Handle);
    }

    (AL_VOID)AlIntr_RegHandler(HwConfig->InterrupId, AL_NULL, AlQspi_Dev_IntrHandler, &(*Handle)->Dev);

    Ret = AlOsal_Lock_Init(&(*Handle)->QspiLock, "QspiLock");
    if (AL_OK != Ret) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&(*Handle)->TxEventQueue, "QSPI_TXDONE");
    if (AL_OK != Ret) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&(*Handle)->RxEventQueue, "QSPI_RXDONE");

    return Ret;
}

/**
 *
 * This function sends data through the QSPI peripheral in a blocking manner, meaning it waits until the data is fully sent or a timeout occurs.
 * It takes the QSPI lock before sending the data and releases it afterward.
 * If the data is sent successfully within the timeout period, it waits for a send done event.
 *
 * @param Handle Pointer to the AL_QSPI_HalStruct structure representing the QSPI device.
 * @param Data Pointer to the data buffer to be sent.
 * @param Size Size of the data to be sent, in bytes.
 * @param Timeout Maximum time to wait for the data to be sent, in milliseconds.
 * @return Returns AL_OK if the data is sent successfully within the timeout period, otherwise returns an error code indicating the failure reason.
*/
AL_S32 AlQspi_Hal_SendDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_QSPI_EventStruct QspiEvent = {0};

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_QSPI_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->QspiLock, Timeout);
    if (AL_OK != Ret) {
        return Ret;
    }

    Ret = AlQspi_Dev_SendData(&Handle->Dev, Data, Size);
    if (AL_OK != Ret) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
        return Ret;
    }

    /* wait until data send done */
    Ret = AlQspi_Hal_WaitTxDoneOrTimeout(Handle, &QspiEvent, Timeout);
    if (AL_OK != Ret) {
        Ret = AL_QSPI_ERR_TIMEOUT;
        AlQspi_ll_MaskIntr(Handle->Dev.HwConfig.BaseAddress, QSPI_TXEIM);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);

    if (AL_OK ==  Ret&& (QspiEvent.Events == AL_QSPI_SEND_DONE))
        return AL_OK;
    else
        return (AL_OK != Ret) ? Ret : AL_QSPI_EVENTS_TO_ERRS(QspiEvent.Events);
}

/**
 *
 * Similar to AlQspi_Hal_SendDataBlock, this function receives data through the QSPI peripheral in a blocking manner.
 * It waits until the specified amount of data is received or a timeout occurs.
 * It ensures mutual exclusive access to the QSPI peripheral by taking a lock before receiving the data and releasing it afterward.
 *
 * @param Handle Pointer to the AL_QSPI_HalStruct structure representing the QSPI device.
 * @param Data Pointer to the buffer where the received data will be stored.
 * @param Size Size of the data to be received, in bytes.
 * @param Timeout Maximum time to wait for the data to be received, in milliseconds.
 * @return Returns AL_OK if the data is received successfully within the timeout period, otherwise returns an error code indicating the failure reason.
 */
AL_S32 AlQspi_Hal_RecvDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_QSPI_EventStruct QspiEvent = {0};

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_QSPI_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->QspiLock, Timeout);
    if (AL_OK != Ret) {
        return Ret;
    }

    Ret = AlQspi_Dev_RecvData(&Handle->Dev, Data, Size);
    if (AL_OK != Ret) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
        return Ret;
    }

    /* wait until data receive done */
    Ret = AlQspi_Hal_WaitRxDoneOrTimeout(Handle, &QspiEvent, Timeout);
    if (AL_OK != Ret) {
        Ret = AL_QSPI_ERR_TIMEOUT;
        AlQspi_ll_MaskIntr(Handle->Dev.HwConfig.BaseAddress, QSPI_RXFIM);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);

    if (Ret == AL_OK && (QspiEvent.Events == AL_QSPI_RECEIVE_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_QSPI_EVENTS_TO_ERRS(QspiEvent.Events);
}

/**
 *
 * This function initiates a transfer of data blocks over QSPI. It first takes a lock on the QSPI device to ensure
 * exclusive access, then sends the data. It waits for the transmission to complete or timeout before releasing the lock.
 *
 * @param Handle Pointer to the QSPI handle which contains the device configuration and state.
 * @param SendData Pointer to the buffer containing the data to be sent.
 * @param SendSize Size of the data to be sent.
 * @param RecvData Pointer to the buffer where received data will be stored.
 * @param RecvSize Size of the receive buffer.
 * @param Timeout Time in milliseconds to wait for the operation to complete.
 * @return AL_S32 Returns AL_OK on success, AL_QSPI_ERR_TIMEOUT if the operation times out, or other error codes on failure.
 */
AL_S32 AlQspi_Hal_TranferDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                   AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_QSPI_EventStruct QspiEvent = {0};

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_QSPI_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->QspiLock, Timeout);
    if (AL_OK != Ret) {
        return Ret;
    }

    Ret = AlQspi_Dev_TranferData(&Handle->Dev, SendData, SendSize, RecvData, RecvSize);
    if (AL_OK != Ret) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
        return Ret;
    }

    /* wait until data done */
    Ret = AlQspi_Hal_WaitTxRxDoneOrTimeout(Handle, &QspiEvent, Timeout);
    if (AL_OK != Ret) {
        Ret = AL_QSPI_ERR_TIMEOUT;
        AlQspi_ll_MaskIntr(Handle->Dev.HwConfig.BaseAddress, QSPI_TXEIM | QSPI_RXFIM);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);

    if (Ret == AL_OK && (QspiEvent.Events == AL_QSPI_RECEIVE_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_QSPI_EVENTS_TO_ERRS(QspiEvent.Events);
}

/**
 *
 * This function configures the DMA controller for sending data over QSPI. It sets up the DMA channel, including
 * the source and destination addresses, transfer size, and other parameters. It then starts the DMA transfer and waits
 * for it to complete or timeout.
 *
 * @param Handle Pointer to the QSPI handle which contains the device configuration and state.
 * @param SendData Pointer to the data to be sent.
 * @param InstAndAddr Pointer to the instruction and address bytes to be sent before the data.
 * @param SendDataSize Size of the data to be sent.
 * @param Timeout Time in milliseconds to wait for the operation to complete.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlQspi_Hal_DmaStartBlockSend(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8 *InstAndAddr, AL_U32 SendDataSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChTransStruct    *QspiTxDmacChTrans;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    AL_DMACAHB_ChInitStruct     *QspiTxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *QspiTxDmacHandlePtr;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_QSPI_ERR_ILLEGAL_PARAM);

    QspiTxDmacChConfigPtr = &QspiTxDmacChConfig;
    QspiTxDmacHandlePtr = QspiTxDmacHandle;

    Ret = AlOsal_Lock_Take(&Handle->QspiLock, Timeout);
    if (AL_OK != Ret) {
        return Ret;
    }

    Handle->Dev.SendBuffer.BufferPtr = InstAndAddr;

    Ret = AlQspi_Dev_DmaSendData(&Handle->Dev, SendDataSize);
    if (AL_OK != Ret) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
        return Ret;
    }

    if (SendDataSize > 0) {
        if (QspiTxDmacHandlePtr == NULL) {
            QspiTxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_6;
            QspiTxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
            QspiTxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
            QspiTxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
            QspiTxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
            QspiTxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
            QspiTxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
            QspiTxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
            QspiTxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
            QspiTxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;

            QspiTxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_MEM2PER;
            QspiTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_QSPI_TX;

            QspiTxDmacChConfigPtr->HandShaking.DstHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
            QspiTxDmacChConfigPtr->HandShaking.DstHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

            QspiTxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
            QspiTxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
            QspiTxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_6;
            QspiTxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
            QspiTxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
            QspiTxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
            QspiTxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;
            Ret = AlDmacAhb_Hal_Init(&QspiTxDmacHandlePtr, DmacDevId, QspiTxDmacChConfigPtr, AL_NULL);
            if (AL_OK != Ret) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "QspiTx Dmacahb hal Init error:0x%x\r\n", Ret);
                (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
                return Ret;
            }
            QspiTxDmacHandle = QspiTxDmacHandlePtr;
        }
        QspiTxDmacChTrans = &(QspiTxDmacHandlePtr->Channel.Trans);
        QspiTxDmacChTrans->SrcAddr        = (AL_REG)(SendData);
        QspiTxDmacChTrans->DstAddr        = Handle->Dev.HwConfig.BaseAddress + QSPI_DR0_OFFSET;
        QspiTxDmacChTrans->TransSize      = SendDataSize;

        /* Set cs to start transfer */
        AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, Handle->Dev.Configs.Trans.SlvSelEnum);
        Ret = AlDmacAhb_Hal_StartBlock(QspiTxDmacHandlePtr, Timeout);
        if (AL_OK != Ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiTx Dmacahb hal Start Block error:0x%x\r\n", Ret);
        }
    } else {
        /* Set cs to start transfer */
        AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, Handle->Dev.Configs.Trans.SlvSelEnum);
    }

    while(QSPI_SR_TXFIFO_NOTEMPTY == AlQspi_ll_IsTxFifoEmpty(Handle->Dev.HwConfig.BaseAddress));
    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));
    AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, QSPI_SER_SS_DIS);
    (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);

    return Ret;
}


/**
 *
 * This function prepares the DMA controller for receiving data over QSPI. It sets up the DMA channel for reception,
 * including the destination address and size. It then initiates the DMA transfer and waits for it to complete or timeout.
 *
 * @param Handle Pointer to the QSPI handle which contains the device configuration and state.
 * @param RecvData Pointer to the buffer where the received data will be stored.
 * @param RecvSize Size of the data to be received.
 * @param Timeout Time in milliseconds to wait for the operation to complete.
 * @return AL_S32 Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlQspi_Hal_DmaStartBlockReceive(AL_QSPI_HalStruct *Handle, AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChTransStruct    *QspiRxDmacChTrans;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    AL_DMACAHB_ChInitStruct     *QspiRxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *QspiRxDmacHandlePtr;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_QSPI_ERR_ILLEGAL_PARAM);

    QspiRxDmacChConfigPtr = &QspiRxDmacChConfig;
    QspiRxDmacHandlePtr = QspiRxDmacHandle;

    Ret = AlOsal_Lock_Take(&Handle->QspiLock, Timeout);
    if (AL_OK != Ret) {
        return Ret;
    }

    Ret = AlQspi_Dev_DmaRecvData(&Handle->Dev, RecvSize);
    if (AL_OK != Ret) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
        return Ret;
    }

    if (QspiRxDmacHandlePtr == NULL) {
        QspiRxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_7;
        QspiRxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        QspiRxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        QspiRxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        QspiRxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        QspiRxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        QspiRxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        QspiRxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        QspiRxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        QspiRxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;
        QspiRxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_PER2MEM;
        QspiRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_QSPI_RX;

        QspiRxDmacChConfigPtr->HandShaking.SrcHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        QspiRxDmacChConfigPtr->HandShaking.SrcHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

        QspiRxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        QspiRxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        QspiRxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_7;
        QspiRxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        QspiRxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        QspiRxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        QspiRxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;

        Ret = AlDmacAhb_Hal_Init(&QspiRxDmacHandlePtr, DmacDevId, QspiRxDmacChConfigPtr, AL_NULL);
        if (AL_OK != Ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiRx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
            return Ret;
        }
        QspiRxDmacHandle = QspiRxDmacHandlePtr;
    }

    QspiRxDmacChTrans = &(QspiRxDmacHandlePtr->Channel.Trans);
    QspiRxDmacChTrans->SrcAddr        = Handle->Dev.HwConfig.BaseAddress + QSPI_DR0_OFFSET;
    QspiRxDmacChTrans->DstAddr        = (AL_REG)RecvData;
    QspiRxDmacChTrans->TransSize      = RecvSize;

    /* Set cs to start transfer */
    AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, Handle->Dev.Configs.Trans.SlvSelEnum);

    Ret = AlDmacAhb_Hal_StartBlock(QspiRxDmacHandlePtr, Timeout);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "QspiRx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);

    return Ret;
}



/**
 *
 * This function configures the DMA channels for both transmission and reception, initializes
 * the DMA with the given parameters, and starts the transfer. It handles the locking mechanism
 * to ensure thread safety and checks for valid input parameters. The function calculates the
 * temporary send size based on the address and instruction length from the QSPI configuration,
 * sets up the DMA channel configurations for both sending and receiving, and initializes the
 * DMA channels. If any step fails, it releases the acquired lock and returns an error code.
 *
 * @param Handle Pointer to the QSPI handle which contains configuration and state information.
 * @param SendData Pointer to the buffer containing the data to be sent.
 * @param SendSize Size of the data to be sent.
 * @param RecvData Pointer to the buffer where received data will be stored.
 * @param RecvSize Size of the data to be received.
 * @param Timeout Time in milliseconds to wait for the lock before starting the transfer.
 * @return Returns AL_OK on success or an error code on failure.
 */
AL_S32 AlQspi_Hal_DmaStartBlockTranfer(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                       AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChTransStruct    *QspiTxDmacChTrans;
    AL_DMACAHB_ChTransStruct    *QspiRxDmacChTrans;

    AL_DMACAHB_ChInitStruct     *QspiTxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *QspiTxDmacHandlePtr;
    AL_DMACAHB_ChInitStruct     *QspiRxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *QspiRxDmacHandlePtr;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;
    AL_U32 TempSendSize = (Handle->Dev.Configs.Trans.EnSpiCfg.AddrLength + Handle->Dev.Configs.Trans.EnSpiCfg.InstLength) / 2;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_QSPI_ERR_ILLEGAL_PARAM);

    QspiTxDmacChConfigPtr = &QspiTxDmacChConfig;
    QspiTxDmacHandlePtr = QspiTxDmacHandle;
    QspiRxDmacChConfigPtr = &QspiRxDmacChConfig;
    QspiRxDmacHandlePtr = QspiRxDmacHandle;

    Ret = AlOsal_Lock_Take(&Handle->QspiLock, Timeout);
    if (AL_OK != Ret) {
        return Ret;
    }

    Handle->Dev.SendBuffer.BufferPtr = SendData;
    Ret = AlQspi_Dev_DmaTranferData(&Handle->Dev, SendSize, RecvSize);
    if (AL_OK != Ret) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
        return Ret;
    }

    if ((QspiTxDmacHandlePtr == NULL) && (SendSize > TempSendSize)) {
        QspiTxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_6;
        QspiTxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        QspiTxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        QspiTxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        QspiTxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        QspiTxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        QspiTxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        QspiTxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        QspiTxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        QspiTxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;

        QspiTxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_MEM2PER;
        QspiTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_QSPI_TX;

        QspiTxDmacChConfigPtr->HandShaking.DstHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        QspiTxDmacChConfigPtr->HandShaking.DstHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

        QspiTxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        QspiTxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        QspiTxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_6;
        QspiTxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        QspiTxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        QspiTxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        QspiTxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;
        Ret = AlDmacAhb_Hal_Init(&QspiTxDmacHandlePtr, DmacDevId, QspiTxDmacChConfigPtr, AL_NULL);
        if (AL_OK != Ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiTx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
            return Ret;
        }

        QspiTxDmacChTrans = &(QspiTxDmacHandlePtr->Channel.Trans);
        QspiTxDmacChTrans->SrcAddr        = (AL_REG)(SendData + TempSendSize);
        QspiTxDmacChTrans->DstAddr        = Handle->Dev.HwConfig.BaseAddress + QSPI_DR0_OFFSET;
        QspiTxDmacChTrans->TransSize      = SendSize - TempSendSize;
        QspiTxDmacHandle = QspiTxDmacHandlePtr;
    }

    if (QspiRxDmacHandlePtr == NULL) {
        QspiRxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_7;
        QspiRxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        QspiRxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        QspiRxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        QspiRxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        QspiRxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        QspiRxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        QspiRxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        QspiRxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        QspiRxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;
        QspiRxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_PER2MEM;
        QspiRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_QSPI_RX;

        QspiRxDmacChConfigPtr->HandShaking.SrcHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        QspiRxDmacChConfigPtr->HandShaking.SrcHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

        QspiRxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        QspiRxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        QspiRxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_7;
        QspiRxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        QspiRxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        QspiRxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        QspiRxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;
        Ret = AlDmacAhb_Hal_Init(&QspiRxDmacHandlePtr, DmacDevId, QspiRxDmacChConfigPtr, AL_NULL);
        if (AL_OK != Ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiRx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
            return Ret;
        }
        QspiRxDmacHandle = QspiRxDmacHandlePtr;
    }

    QspiRxDmacChTrans = &(QspiRxDmacHandlePtr->Channel.Trans);
    QspiRxDmacChTrans->SrcAddr        = Handle->Dev.HwConfig.BaseAddress + QSPI_DR0_OFFSET;
    QspiRxDmacChTrans->DstAddr        = (AL_REG)RecvData;
    QspiRxDmacChTrans->TransSize      = RecvSize;

    Ret = AlDmacAhb_Hal_Start(QspiRxDmacHandlePtr);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "QspiRx Dmacahb hal Start error:0x%x\r\n", Ret);
        (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);
        return Ret;
    }

    if (SendSize > TempSendSize) {
        /* Set cs to start transfer */
        AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, Handle->Dev.Configs.Trans.SlvSelEnum);

        Ret = AlDmacAhb_Hal_StartBlock(QspiTxDmacHandlePtr, Timeout);
        if (AL_OK != Ret) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiTx Dmacahb hal Start Block error:0x%x\r\n", Ret);
        }
        AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, QSPI_SER_SS_DIS);
    } else {
        /* Set cs to start transfer */
        AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, Handle->Dev.Configs.Trans.SlvSelEnum);

        AL_DMACAHB_EventStruct Event;
        Ret = AlDmacAhb_Hal_WaitTransDoneOrTimeout(QspiRxDmacHandlePtr, &Event, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiRx Dmacahb hal Wait Trans error:0x%x\r\n", Ret);
            return Ret;
        }
        while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));
        AlQspi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, QSPI_SER_SS_DIS);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);

    return Ret;
}

/**
 *
 * This function is responsible for executing a variety of control commands on the QSPI device. It first ensures
 * that the handle provided is not NULL, indicating a valid QSPI device. Then, it attempts to take a lock on the
 * device to ensure exclusive access during the operation, waiting up to a specified timeout. If the lock is successfully
 * acquired, the function proceeds to execute the specified IOCTL command by calling another function and passing
 * the command and data. Regardless of the success or failure of the command execution, it releases the lock on the
 * device before returning. If any step fails, the function returns an error code.
 *
 * @param Handle Pointer to the QSPI handle, representing the QSPI device.
 * @param Cmd The IOCTL command to execute.
 * @param Data Pointer to the data required for the command, if any.
 * @param Timeout Maximum time in milliseconds to wait for the lock before proceeding with the command.
 * @return AL_S32 Returns AL_OK on successful execution of the command, or an error code if the operation fails.
 */
AL_S32 AlQspi_Hal_IoCtl(AL_QSPI_HalStruct *Handle, AL_Qspi_IoCtlCmdEnum Cmd, AL_VOID *Data, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_QSPI_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->QspiLock, Timeout);
    if (AL_OK != Ret) {
        return Ret;
    }

    Ret = AlQspi_Dev_IoCtl(&Handle->Dev, Cmd, Data);
    if (AL_OK != Ret) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Qspi io ctl cmd error:%d\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->QspiLock);

    return Ret;
}


