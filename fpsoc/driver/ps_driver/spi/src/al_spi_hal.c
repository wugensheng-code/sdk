/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_spi_hal.h"
#include "al_intr.h"
#include "al_dmacahb_hal.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
AL_SPI_HalStruct AlSpiHandle[AL_SPI_NUM_INSTANCE];

AL_DMACAHB_ChInitStruct     Spi0TxDmacChConfig;
AL_DMACAHB_HalStruct        *Spi0TxDmacHandle = AL_NULL;
AL_DMACAHB_ChInitStruct     Spi0RxDmacChConfig;
AL_DMACAHB_HalStruct        *Spi0RxDmacHandle = AL_NULL;

AL_DMACAHB_ChInitStruct     Spi1TxDmacChConfig;
AL_DMACAHB_HalStruct        *Spi1TxDmacHandle = AL_NULL;
AL_DMACAHB_ChInitStruct     Spi1RxDmacChConfig;
AL_DMACAHB_HalStruct        *Spi1RxDmacHandle = AL_NULL;

/********************************************************/
/**
 *
 * This function waits for the transmission to complete or times out, using the specified timeout value.
 *
 * @param Handle Pointer to the AL_SPI_HalStruct structure.
 * @param Event Pointer to the AL_SPI_EventStruct structure to store the event information.
 * @param Timeout Timeout value in milliseconds.
 *
 * @return The return value indicates the status of the function call.
 *         - Returns a negative value if an error occurred.
 *         - Returns 0 if the transmission completed successfully.
 *         - Returns a positive value if the function timed out.
 *
 */
static AL_S32 AlSpi_Hal_WaitTxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_SPI_EventStruct *Event, AL_U32 Timeout)
{

#ifdef SPI_DEBUG
    AlSpi_Dev_DumpReg(Handle->Dev.HwConfig.BaseAddress);
#endif

    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->TxEventQueue, Event, Timeout);

    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));

    AlSpi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, SPI_SER_SS_DIS);
    return Ret;
}

/**
 *
 * This function waits for the Rx done event to occur or for a timeout to happen.
 *
 * @param Handle Pointer to the AL_SPI_HalStruct structure.
 * @param Event Pointer to the AL_SPI_EventStruct structure to store the event information.
 * @param Timeout Timeout value in milliseconds.
 *
 * @return The return value of AlOsal_Mb_Receive function.
 *
 */
static AL_S32 AlSpi_Hal_WaitRxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_SPI_EventStruct *Event, AL_U32 Timeout)
{
#ifdef SPI_DEBUG
    AlSpi_Dev_DumpReg(Handle->Dev.HwConfig.BaseAddress);
#endif
    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->RxEventQueue, Event, Timeout);
    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));

    AlSpi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, SPI_SER_SS_DIS);

    return Ret;
}

/**
 *
 * This function waits for the transmit and receive operations of the SPI handle to complete
 * or until the specified timeout period is reached. It uses the event queues to receive
 * the transmit and receive events. If the timeout period is reached before the operations
 * complete, the slave select line is disabled and the function returns the appropriate error code.
 *
 * @param Handle Pointer to the AL_SPI_HalStruct representing the SPI handle.
 * @param Event Pointer to the AL_SPI_EventStruct to store the received events.
 * @param Timeout Timeout period in milliseconds.
 *
 * @return Returns AL_OK if the transmit and receive operations complete successfully,
 *         otherwise returns the appropriate error code.
 *
 */
static AL_S32 AlSpi_Hal_WaitTxRxDoneOrTimeout(AL_SPI_HalStruct *Handle, AL_SPI_EventStruct *Event, AL_U32 Timeout)
{
#ifdef SPI_DEBUG
    AlSpi_Dev_DumpReg(Handle->Dev.HwConfig.BaseAddress);
#endif

    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->TxEventQueue, Event, Timeout);
    if (AL_OK != Ret) {
        AlSpi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, SPI_SER_SS_DIS);
        return Ret;
    }

    Ret = AlOsal_Mb_Receive(&Handle->RxEventQueue, Event, Timeout);
    if (AL_OK != Ret) {
        AlSpi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, SPI_SER_SS_DIS);
        return Ret;
    }

    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));

    AlSpi_ll_SetSlvSel(Handle->Dev.HwConfig.BaseAddress, SPI_SER_SS_DIS);

    return Ret;
}

/**
 *
 * This function is called when a SPI event occurs. It handles different types of SPI events
 * and sends them to the corresponding event queues.
 *
 * @param SpiEvent The SPI event structure containing information about the event.
 * @param CallbackRef A reference to the SPI handle structure.
 *
 * @return None.
 *
 */
static AL_VOID AlSpi_DefEventCallBack(AL_SPI_EventStruct SpiEvent, void *CallbackRef)
{
    AL_SPI_HalStruct *Handle = (AL_SPI_HalStruct *)CallbackRef;

    switch (SpiEvent.Events) {
    case AL_SPI_SEND_DONE:
    case AL_SPI_SEND_TIMEOUT:
    case AL_SPI_TX_FO:
    case AL_SPI_TX_FIFO_EMPTY:
        AlOsal_Mb_Send(&Handle->TxEventQueue, &SpiEvent);
        break;
    case AL_SPI_RECEIVE_DONE:
    case AL_SPI_RECEIVE_TIMEOUT:
    case AL_SPI_RX_FO:
    case AL_SPI_RX_FU:
    case AL_SPI_RX_FIFO_FULL:
        AlOsal_Mb_Send(&Handle->RxEventQueue, &SpiEvent);
        break;
    default:
        break;
    }
}

/**
 *
 * This function initializes the SPI hardware and driver based on the provided configuration.
 *
 * @param Handle Pointer to the SPI handle structure.
 * @param InitConfig Pointer to the SPI configuration structure.
 * @param Callback Callback function for SPI events.
 * @param DevId ID of the SPI device.
 *
 * @return Returns AL_OK on success, otherwise an error code.
 *
 */
AL_S32 AlSpi_Hal_Init(AL_SPI_HalStruct **Handle, AL_SPI_ConfigsStruct *InitConfig, SPI_EventCallBack Callback, AL_U32 DevId)
{
    AL_S32 Ret = AL_OK;
    AL_SPI_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT((Handle != AL_NULL), AL_SPI_ERR_ILLEGAL_PARAM);

    HwConfig = AlSpi_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlSpiHandle[DevId];
    } else {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    Ret = AlSpi_Dev_Init(&(*Handle)->Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    (*Handle)->Dev.Configs.Trans.SlvSelEnum = HwConfig->CsSel;

    if(AL_NULL == Callback) {
        Ret = AlSpi_Dev_RegisterIntrCallBack(&(*Handle)->Dev, AlSpi_DefEventCallBack, (void *)*Handle);
    } else {
        Ret = AlSpi_Dev_RegisterIntrCallBack(&(*Handle)->Dev, Callback, (void *)*Handle);
    }

    (AL_VOID)AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlSpi_Dev_IntrHandler, &(*Handle)->Dev);

    Ret = AlOsal_Lock_Init(&(*Handle)->SpiLock, "SpiLock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&(*Handle)->TxEventQueue, "SPI_TXDONE");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&(*Handle)->RxEventQueue, "SPI_RXDONE");

    return Ret;
}

/**
 *
 * This function sends a block of data over SPI using the provided handle.
 *
 * @param Handle Pointer to the SPI handle structure.
 * @param Data Pointer to the data buffer to be sent.
 * @param Size Size of the data buffer in bytes.
 * @param Timeout Timeout value in milliseconds.
 *
 * @return Returns AL_OK if the data was sent successfully, otherwise returns an error code.
 *
 */
AL_S32 AlSpi_Hal_SendDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_SPI_EventStruct SpiEvent = {0};

    /* check only Handle,Handle->Dev more checks in AlSpi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_SPI_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->SpiLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlSpi_Dev_SendData(&Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
        return Ret;
    }

    Ret = AlSpi_Hal_WaitTxDoneOrTimeout(Handle, &SpiEvent, Timeout);
    if (Ret != AL_OK) {
        /*
        * if timeout, mask SPI_TXEIM interrupt
        */
        AlSpi_ll_MaskIntr(Handle->Dev.HwConfig.BaseAddress, SPI_TXEIM);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);

    if (Ret == AL_OK && (SpiEvent.Events == AL_SPI_SEND_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_SPI_EVENTS_TO_ERRS(SpiEvent.Events);
}

/**
 *
 * This function receives a block of data from the SPI device specified by the given handle.
 *
 * @param Handle Pointer to the AL_SPI_HalStruct representing the SPI device.
 * @param Data Pointer to the buffer where the received data will be stored.
 * @param Size The size of the data block to receive, in bytes.
 * @param Timeout The maximum time to wait for the receive operation to complete, in milliseconds.
 *
 * @return AL_OK if the receive operation is successful, otherwise an error code.
 *
 */
AL_S32 AlSpi_Hal_RecvDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_SPI_EventStruct SpiEvent = {0};

    /* check only Handle,Handle->Dev more checks in AlSpi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_SPI_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->SpiLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlSpi_Dev_RecvData(&Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
        return Ret;
    }

    /* wait until data receive done */
    Ret = AlSpi_Hal_WaitRxDoneOrTimeout(Handle, &SpiEvent, Timeout);
    if (Ret != AL_OK) {
        AlSpi_ll_MaskIntr(Handle->Dev.HwConfig.BaseAddress, SPI_RXFIM);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);

    if (Ret == AL_OK && (SpiEvent.Events == AL_SPI_RECEIVE_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_SPI_EVENTS_TO_ERRS(SpiEvent.Events);
}

/**
 *
 * This function transfers a block of data over SPI using the provided SPI handle.
 * It sends the data in the `SendData` buffer of size `SendSize` and receives the data
 * in the `RecvData` buffer of size `RecvSize`. The transfer is performed with a timeout
 * value specified in milliseconds.
 *
 * @param Handle Pointer to the SPI handle structure.
 * @param SendData Pointer to the buffer containing the data to be sent.
 * @param SendSize Size of the data to be sent in bytes.
 * @param RecvData Pointer to the buffer to store the received data.
 * @param RecvSize Size of the buffer to store the received data in bytes.
 * @param Timeout Timeout value in milliseconds for the transfer operation.
 *
 * @return Returns AL_OK if the transfer is successful, otherwise returns an error code.
 *
 */
AL_S32 AlSpi_Hal_TranferDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                   AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_SPI_EventStruct SpiEvent = {0};

    /* check only Handle,Handle->Dev more checks in AlSpi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_SPI_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->SpiLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlSpi_Dev_TranferData(&Handle->Dev, SendData, SendSize, RecvData, RecvSize);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
        return Ret;
    }

    /* wait until data done */
    Ret = AlSpi_Hal_WaitTxRxDoneOrTimeout(Handle, &SpiEvent, Timeout);
    if (Ret != AL_OK) {
        AlSpi_ll_MaskIntr(Handle->Dev.HwConfig.BaseAddress, SPI_TXEIM | SPI_RXFIM);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);

    if (Ret == AL_OK && (SpiEvent.Events == AL_SPI_RECEIVE_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_SPI_EVENTS_TO_ERRS(SpiEvent.Events);
}

/**
 *
 * This function starts a DMA block send operation on the SPI interface. It transfers the data from the provided
 * buffer to the SPI data register using DMA. The function first checks the handle and then initializes the DMA
 * channel configuration based on the SPI base address. It then locks the SPI interface, initiates the DMA send
 * operation, and starts the DMA transfer. Finally, it waits for the transmit FIFO to be empty and for the SPI
 * interface to be idle before releasing the lock and returning the result.
 *
 * @param Handle Pointer to the SPI HAL structure.
 * @param SendData Pointer to the buffer containing the data to be sent.
 * @param SendSize Size of the data to be sent in bytes.
 * @param Timeout Timeout value in milliseconds.
 *
 * @return Returns AL_OK if the DMA block send operation is successful, or an error code if it fails.
 *
 */
AL_S32 AlSpi_Hal_DmaStartBlockSend(AL_SPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChTransStruct    *SpiTxDmacChTrans;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    AL_DMACAHB_ChInitStruct     *SpiTxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *SpiTxDmacHandlePtr;

    /* check only Handle,Handle->Dev more checks in AlSpi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_SPI_ERR_ILLEGAL_PARAM);

    if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
        SpiTxDmacChConfigPtr = &Spi0TxDmacChConfig;
        SpiTxDmacHandlePtr = Spi0TxDmacHandle;
    } else {
        SpiTxDmacChConfigPtr = &Spi1TxDmacChConfig;
        SpiTxDmacHandlePtr = Spi1TxDmacHandle;
    }

    Ret = AlOsal_Lock_Take(&Handle->SpiLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlSpi_Dev_DmaSendData(&Handle->Dev);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
        return Ret;
    }

    if (SpiTxDmacHandlePtr == NULL) {
        SpiTxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_6;
        SpiTxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        SpiTxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        SpiTxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        SpiTxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        SpiTxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        SpiTxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        SpiTxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        SpiTxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        SpiTxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;
        SpiTxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_MEM2PER;

        if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
            SpiTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_SPI0_TX;
        } else {
            SpiTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_SPI1_TX;
        }

        SpiTxDmacChConfigPtr->HandShaking.DstHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        SpiTxDmacChConfigPtr->HandShaking.DstHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

        SpiTxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        SpiTxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        SpiTxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_6;
        SpiTxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        SpiTxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        SpiTxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        SpiTxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;

        Ret = AlDmacAhb_Hal_Init(&SpiTxDmacHandlePtr, DmacDevId, SpiTxDmacChConfigPtr, AL_NULL);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "SpiTx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
            return Ret;
        }
        if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
            Spi0TxDmacHandle = SpiTxDmacHandlePtr;
        } else {
            Spi1TxDmacHandle = SpiTxDmacHandlePtr;
        }
    }

    SpiTxDmacChTrans = &(SpiTxDmacHandlePtr->Channel.Trans);
    SpiTxDmacChTrans->SrcAddr        = (AL_REG)SendData;
    SpiTxDmacChTrans->DstAddr        = Handle->Dev.HwConfig.BaseAddress + SPI_DR0_MST_OFFSET;
    SpiTxDmacChTrans->TransSize      = SendSize;

    Ret = AlDmacAhb_Hal_StartBlock(SpiTxDmacHandlePtr, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "SpiTx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    while(SPI_SR_TXFIFO_NOTEMPTY == AlSpi_ll_IsTxFifoEmpty(Handle->Dev.HwConfig.BaseAddress));
    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));

    (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);

    return Ret;
}

/**
 *
 * This function starts a DMA block receive operation on the SPI interface. It receives data from the SPI interface
 * and stores it in the provided buffer using DMA. The function takes a handle to the SPI interface, the buffer to
 * store the received data, the size of the data to receive, and a timeout value.
 *
 * @param Handle Pointer to the SPI handle structure.
 * @param RecvData Pointer to the buffer to store the received data.
 * @param RecvSize Size of the data to receive.
 * @param Timeout Timeout value in milliseconds.
 *
 * @return Returns AL_OK if the operation is successful, or an error code if the operation fails.
 *
 */
AL_S32 AlSpi_Hal_DmaStartBlockReceive(AL_SPI_HalStruct *Handle, AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChTransStruct    *SpiRxDmacChTrans;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    AL_DMACAHB_ChInitStruct     *SpiRxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *SpiRxDmacHandlePtr;

    /* check only Handle,Handle->Dev more checks in AlSpi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_SPI_ERR_ILLEGAL_PARAM);

    if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
        SpiRxDmacChConfigPtr = &Spi0RxDmacChConfig;
        SpiRxDmacHandlePtr = Spi0RxDmacHandle;
    } else {
        SpiRxDmacChConfigPtr = &Spi1RxDmacChConfig;
        SpiRxDmacHandlePtr = Spi1RxDmacHandle;
    }

    Ret = AlOsal_Lock_Take(&Handle->SpiLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlSpi_Dev_DmaRecvData(&Handle->Dev, RecvSize);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
        return Ret;
    }

    if (SpiRxDmacHandlePtr == NULL) {
        SpiRxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_7;
        SpiRxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        SpiRxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        SpiRxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        SpiRxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        SpiRxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        SpiRxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        SpiRxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        SpiRxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        SpiRxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;
        SpiRxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_PER2MEM;
        if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
            SpiRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_SPI0_RX;
        } else {
            SpiRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_SPI1_RX;
        }
        SpiRxDmacChConfigPtr->HandShaking.SrcHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        SpiRxDmacChConfigPtr->HandShaking.SrcHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

        SpiRxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        SpiRxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        SpiRxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_7;
        SpiRxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        SpiRxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        SpiRxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        SpiRxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;

        Ret = AlDmacAhb_Hal_Init(&SpiRxDmacHandlePtr, DmacDevId, SpiRxDmacChConfigPtr, AL_NULL);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "SpiRx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
            return Ret;
        }

        if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
            Spi0RxDmacHandle = SpiRxDmacHandlePtr;
        } else {
            Spi1RxDmacHandle = SpiRxDmacHandlePtr;
        }
    }

    SpiRxDmacChTrans = &(SpiRxDmacHandlePtr->Channel.Trans);
    SpiRxDmacChTrans->SrcAddr        = Handle->Dev.HwConfig.BaseAddress + SPI_DR0_MST_OFFSET;
    SpiRxDmacChTrans->DstAddr        = (AL_REG)RecvData;
    SpiRxDmacChTrans->TransSize      = RecvSize;

    Ret = AlDmacAhb_Hal_StartBlock(SpiRxDmacHandlePtr, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "SpiRx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);

    return Ret;
}

/**
 *
 * This function starts a DMA block transfer for SPI communication. It transfers data from the
 * SendData buffer to the SPI transmit register and from the SPI receive register to the RecvData buffer.
 *
 * @param Handle Pointer to the SPI HAL structure.
 * @param SendData Pointer to the buffer containing the data to be transmitted.
 * @param SendSize Size of the data to be transmitted in bytes.
 * @param RecvData Pointer to the buffer to store the received data.
 * @param RecvSize Size of the buffer to store the received data in bytes.
 * @param Timeout Timeout value in milliseconds.
 *
 * @return Returns AL_OK if the DMA block transfer was started successfully, or an error code if an error occurred.
 *
 */
AL_S32 AlSpi_Hal_DmaStartBlockTranfer(AL_SPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                       AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChTransStruct    *SpiTxDmacChTrans;
    AL_DMACAHB_ChTransStruct    *SpiRxDmacChTrans;

    AL_DMACAHB_ChInitStruct     *SpiTxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *SpiTxDmacHandlePtr;
    AL_DMACAHB_ChInitStruct     *SpiRxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *SpiRxDmacHandlePtr;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    /* check only Handle,Handle->Dev more checks in AlSpi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_SPI_ERR_ILLEGAL_PARAM);

    if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
        SpiTxDmacChConfigPtr = &Spi0TxDmacChConfig;
        SpiTxDmacHandlePtr = Spi0TxDmacHandle;
        SpiRxDmacChConfigPtr = &Spi0RxDmacChConfig;
        SpiRxDmacHandlePtr = Spi0RxDmacHandle;
    } else {
        SpiTxDmacChConfigPtr = &Spi1TxDmacChConfig;
        SpiTxDmacHandlePtr = Spi1TxDmacHandle;
        SpiRxDmacChConfigPtr = &Spi1RxDmacChConfig;
        SpiRxDmacHandlePtr = Spi1RxDmacHandle;
    }

    Ret = AlOsal_Lock_Take(&Handle->SpiLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlSpi_Dev_DmaTranferData(&Handle->Dev, RecvSize);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
        return Ret;
    }

    if (SpiTxDmacHandlePtr == NULL) {
        SpiTxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_6;
        SpiTxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        SpiTxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        SpiTxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        SpiTxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        SpiTxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        SpiTxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        SpiTxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        SpiTxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        SpiTxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;
        SpiTxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_MEM2PER;

        if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
            SpiTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_SPI0_TX;
        } else {
            SpiTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_SPI1_TX;
        }

        SpiTxDmacChConfigPtr->HandShaking.DstHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        SpiTxDmacChConfigPtr->HandShaking.DstHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

        SpiTxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        SpiTxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        SpiTxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_6;
        SpiTxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        SpiTxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        SpiTxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        SpiTxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;

        Ret = AlDmacAhb_Hal_Init(&SpiTxDmacHandlePtr, DmacDevId, SpiTxDmacChConfigPtr, AL_NULL);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "SpiTx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
            return Ret;
        }

        if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
            Spi0TxDmacHandle = SpiTxDmacHandlePtr;
        } else {
            Spi1TxDmacHandle = SpiTxDmacHandlePtr;
        }
    }

    if (SpiRxDmacHandlePtr == NULL) {
        SpiRxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_7;
        SpiRxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        SpiRxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        SpiRxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        SpiRxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        SpiRxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        SpiRxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        SpiRxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        SpiRxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        SpiRxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;
        SpiRxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_PER2MEM;

        if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
            SpiRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_SPI0_RX;
        } else {
            SpiRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_SPI1_RX;
        }

        SpiRxDmacChConfigPtr->HandShaking.SrcHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        SpiRxDmacChConfigPtr->HandShaking.SrcHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

        SpiRxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        SpiRxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        SpiRxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_7;
        SpiRxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        SpiRxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        SpiRxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        SpiRxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;

        Ret = AlDmacAhb_Hal_Init(&SpiRxDmacHandlePtr, DmacDevId, SpiRxDmacChConfigPtr, AL_NULL);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "SpiRx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
            return Ret;
        }

        if (Handle->Dev.HwConfig.BaseAddress < SPI1_BASE_ADDR) {
            Spi0RxDmacHandle = SpiRxDmacHandlePtr;
        } else {
            Spi1RxDmacHandle = SpiRxDmacHandlePtr;
        }
    }

    SpiTxDmacChTrans = &(SpiTxDmacHandlePtr->Channel.Trans);
    SpiTxDmacChTrans->SrcAddr        = (AL_REG)SendData;
    SpiTxDmacChTrans->DstAddr        = Handle->Dev.HwConfig.BaseAddress + SPI_DR0_MST_OFFSET;
    SpiTxDmacChTrans->TransSize      = SendSize;

    SpiRxDmacChTrans = &(SpiRxDmacHandlePtr->Channel.Trans);
    SpiRxDmacChTrans->SrcAddr        = Handle->Dev.HwConfig.BaseAddress + SPI_DR0_MST_OFFSET;
    SpiRxDmacChTrans->DstAddr        = (AL_REG)RecvData;
    SpiRxDmacChTrans->TransSize      = RecvSize;

    Ret = AlDmacAhb_Hal_Start(SpiRxDmacHandlePtr);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "SpiRx Dmacahb hal Start error:0x%x\r\n", Ret);
        (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);
        return Ret;
    }

    Ret = AlDmacAhb_Hal_StartBlock(SpiTxDmacHandlePtr, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "SpiTx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }
    while(SPI_SR_TXFIFO_NOTEMPTY == AlSpi_ll_IsTxFifoEmpty(Handle->Dev.HwConfig.BaseAddress));
    while(SPI_SR_BUSY == AlSpi_ll_IsBusy(Handle->Dev.HwConfig.BaseAddress));
    (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);

    return Ret;
}

/**
 *
 * This function allows the user to perform various control operations on the SPI HAL.
 *
 * @param Handle Pointer to the AL_SPI_HalStruct representing the SPI HAL handle.
 * @param Cmd The I/O control command to be executed.
 * @param Data Pointer to the data associated with the I/O control command.
 * @param Timeout The timeout value for the I/O control operation.
 *
 * @return Returns AL_OK if the I/O control operation is successful, otherwise returns an error code.
 *
 */
AL_S32 AlSpi_Hal_IoCtl(AL_SPI_HalStruct *Handle, AL_Spi_IoCtlCmdEnum Cmd, AL_VOID *Data, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    /* check only Handle,Handle->Dev more checks in AlSpi_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_SPI_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->SpiLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlSpi_Dev_IoCtl(&Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Spi io ctl cmd error:%d\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->SpiLock);

    return Ret;
}


