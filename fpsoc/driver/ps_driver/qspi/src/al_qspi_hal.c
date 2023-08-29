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
static AL_QSPI_DevStruct AL_QSPI_DevInstance[AL_QSPI_NUM_INSTANCE];

AL_DMACAHB_ChInitStruct     QspiTxDmacChConfig;
AL_DMACAHB_HalStruct        QspiTxDmacHandle;
AL_DMACAHB_ChInitStruct     QspiRxDmacChConfig;
AL_DMACAHB_HalStruct        QspiRxDmacHandle;

/********************************************************/
#ifdef USE_RTOS
/**
 * This function
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitTxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

/**
 * This function
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send or receive timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitRxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO:wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

/**
 * This function is wait receive done and send done or timeout
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send and receive timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitTxRxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO:wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}


/**
 * This function
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param
 * @param
 * @return
 * @note    None
*/
static AlQspi_Hal_IntrHandler(AL_QSPI_HalStruct *Handle, AL_U32 Event, AL_U32 EventData)
{
    /*
     * TODO:send event
    */
    (void) Handle;
    (void) Event;
    (void) EventData;
}

#define AL_QSPI_HAL_LOCK(Handle)        do {} while (0)

#define AL_QSPI_HAL_UNLOCK(Handle)      do {} while (0)

#else

/**
 * This function is wait send done or timeout
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitTxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlQspi_Dev_IsTxBusy(AL_QSPI_DevStruct *Qspi);

#ifdef QSPI_DEBUG
    AlQspi_Dev_DumpReg(Handle->Dev->BaseAddr);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_Dev_IsTxBusy:%d\r\n", AlQspi_Dev_IsTxBusy(Handle->Dev));
#endif

    while(AlQspi_Dev_IsTxBusy(Handle->Dev));
    // while(QSPI_SR_TXFIFO_NOTEMPTY == AlQspi_ll_IsTxFifoEmpty(Handle->Dev->BaseAddr));
    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);

    return AL_OK;
}

/**
 * This function is wait receive done or timeout
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to receive timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitRxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlQspi_Dev_IsRxBusy(AL_QSPI_DevStruct *Qspi);

#ifdef QSPI_DEBUG
    AlQspi_Dev_DumpReg(Handle->Dev->BaseAddr);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_Dev_IsRxBusy:%d\r\n", AlQspi_Dev_IsRxBusy(Handle->Dev));
#endif

    while(AlQspi_Dev_IsRxBusy(Handle->Dev));
    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);

    return AL_OK;
}

/**
 * This function is wait receive done and send done or timeout
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send and receive timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitTxRxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlQspi_Dev_IsRxBusy(AL_QSPI_DevStruct *Qspi);
    extern AL_BOOL AlQspi_Dev_IsTxBusy(AL_QSPI_DevStruct *Qspi);

#ifdef QSPI_DEBUG
    AlQspi_Dev_DumpReg(Handle->Dev->BaseAddr);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_Dev_IsRxBusy:%d\r\n", AlQspi_Dev_IsRxBusy(Handle->Dev));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_Dev_IsTxBusy:%d\r\n", AlQspi_Dev_IsTxBusy(Handle->Dev));
#endif

    while(AlQspi_Dev_IsRxBusy(Handle->Dev) || AlQspi_Dev_IsTxBusy(Handle->Dev));
    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);

    return AL_OK;
}



#define AL_QSPI_HAL_LOCK(Handle)          do {} while (0)

#define AL_QSPI_HAL_UNLOCK(Handle)        do {} while (0)

#endif

/**
 * This is default event callback function
 * @param   QspiEvent is a AL_QSPI_EventStruct struct
 * @param   CallbackRef is parameter of callback function
 * @return  AL_OK
 * @note    None
*/
static AL_S32 AlQspi_DefEventCallBack(AL_QSPI_EventStruct QspiEvent, void *CallbackRef)
{
    switch (QspiEvent.Event) {
    case AL_QSPI_SEND_DONE:
#ifdef QSPI_DEBUG
       AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack AL_QSPI_SEND_DONE\r\n");
#endif
        break;
    case AL_QSPI_RECEIVE_DONE:
#ifdef QSPI_DEBUG
       AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack AL_QSPI_RECEIVE_DONE\r\n");
#endif
        break;
    case AL_QSPI_SEND_TIMEOUT:
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack AL_QSPI_SEND_TIMEOUT\r\n");
#endif
        break;
    case AL_QSPI_RECEIVE_TIMEOUT:
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack AL_QSPI_RECEIVE_TIMEOUT\r\n");
#endif
        break;
    default:
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack default\r\n");
#endif
        break;
    }

    return AL_OK;
}

/**
 * This function initialize the qspi according to the specified
 *          parameters in the AL_QSPI_ConfigsStruct and initialize the associated handle.
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   InitConfig pointer to a AL_QSPI_ConfigsStruct structure
 *          that contains the configuration information for the specified qspi peripheral
 * @param   Callback is a function pointer to qspi event callback function
 * @param   CallbackRef is parameter of callback function
 * @param   DevId is hardware module id
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_Init(AL_QSPI_HalStruct *Handle, AL_QSPI_ConfigsStruct *InitConfig, QSPI_EventCallBack Callback,
                      AL_VOID *CallbackRef, AL_U32 DevId)
{
    AL_S32 ret = AL_OK;
    AL_QSPI_HwConfigStruct *CfgPtr = AL_NULL;

    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    CfgPtr = AlQspi_Dev_LookupConfig(DevId);
    Handle->Dev = &AL_QSPI_DevInstance[CfgPtr->DeviceId];
    Handle->Dev->BaseAddr = CfgPtr->BaseAddress;
    Handle->Dev->Fifolen  = CfgPtr->FifoLen;

    ret = AlQspi_Dev_Init(Handle->Dev, InitConfig);
    if (ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return ret;
    }

    // if (AL_QSPI_USE_DMA != InitConfig->IsUseDma) {
        if(AL_NULL == Callback) {
            ret = AlQspi_Dev_RegisterIntrCallBack(Handle->Dev, AlQspi_DefEventCallBack, AL_NULL);
        } else {
            ret = AlQspi_Dev_RegisterIntrCallBack(Handle->Dev, Callback, CallbackRef);
        }
        // AlIntr_RegHandler(CfgPtr->InterrupId + 13, AL_NULL, AlQspi_Dev_IntrHandler, Handle->Dev);
        AlIntr_RegHandler(CfgPtr->InterrupId, AL_NULL, AlQspi_Dev_IntrHandler, Handle->Dev);
    // } else {
    //     AlIntr_SetInterrupt(CfgPtr->InterrupId + 13, AL_FUNC_DISABLE);
    //     // AlIntr_RegHandler(CfgPtr->InterrupId + 13, AL_NULL, AlQspi_Dev_IntrHandler, Handle->Dev);
    // }

    AL_QSPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function is qspi blocking send data
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Data is pointer to send data
 * @param   Size is send data size
 * @param   Timeout is max wait time for send done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_SendDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    ret = AlQspi_Dev_SendData(Handle->Dev, Data, Size);
    if (ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return ret;
    }

    /* wait until data send done */
    ret = AlQspi_Hal_WaitTxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_QSPI_ERR_TIMEOUT;
        AlQspi_ll_MaskIntr(Handle->Dev->BaseAddr, QSPI_TXEIM);
    }

    AL_QSPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function is qspi blocking receive data
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Data is pointer to receive data
 * @param   Size is receive data size
 * @param   Timeout is max wait time for receive done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_RecvDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    ret = AlQspi_Dev_RecvData(Handle->Dev, Data, Size);
    if (ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return ret;
    }

    /* wait until data receive done */
    ret = AlQspi_Hal_WaitRxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_QSPI_ERR_TIMEOUT;
        AlQspi_ll_MaskIntr(Handle->Dev->BaseAddr, QSPI_RXFIM);
    }

    AL_QSPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   SendData is pointer to send data
 * @param   SendSize is send data size
 * @param   RecvData is pointer to receive data
 * @param   RecvSize is receive data size
 * @param   Timeout is max wait time for send done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_TranferDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                   AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    ret = AlQspi_Dev_TranferData(Handle->Dev, SendData, SendSize, RecvData, RecvSize);
    if (ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return ret;
    }

    /* wait until data done */
    ret = AlQspi_Hal_WaitTxRxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_QSPI_ERR_TIMEOUT;
        AlQspi_ll_MaskIntr(Handle->Dev->BaseAddr, QSPI_TXEIM | QSPI_RXFIM);
    }

    AL_QSPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function is qspi dma blocking send data
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Data is pointer to send data
 * @param   Size is send data size
 * @param   Timeout is max wait time for send done
 * @return  The state of function execution
 * @note    TransSize(AL_DMACAHB_ChTransStruct) * SrcTransWidth = Data Size
 *          Data Size % SrcBurstLength = 0
*/
AL_S32 AlQspi_Hal_DmaStartBlockSend(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChTransStruct    *QspiTxDmacChTrans;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    AL_DMACAHB_ChInitStruct     *QspiTxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *QspiTxDmacHandlePtr;
    AL_U32 TempSendSize = (Handle->Dev->Configs.EnSpiCfg.AddrLength + Handle->Dev->Configs.EnSpiCfg.InstLength) / 2;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    QspiTxDmacChConfigPtr = &QspiTxDmacChConfig;
    QspiTxDmacHandlePtr = &QspiTxDmacHandle;

    AL_QSPI_HAL_LOCK(Handle);

    Handle->Dev->SendBuffer.BufferPtr = SendData;

    Ret = AlQspi_Dev_DmaSendData(Handle->Dev, SendSize);
    if (Ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return Ret;
    }
// printf("n if(SendSize > TempSendSize)");
    if(SendSize > TempSendSize) {
        if(QspiTxDmacHandlePtr->Channel == NULL) {
            QspiTxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_6;
            QspiTxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
            QspiTxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
            QspiTxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
            QspiTxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
            QspiTxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
            QspiTxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
            //QspiTxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
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
// printf("n AlDmacAhb_Hal_Init");
            Ret = AlDmacAhb_Hal_Init(QspiTxDmacHandlePtr, QspiTxDmacChConfigPtr, AL_NULL, DmacDevId);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "QspiTx Dmacahb hal Init error:0x%x\r\n", Ret);
                AL_QSPI_HAL_UNLOCK(Handle);
                return Ret;
            }
        }
        // printf("\n QspiTxDmacChTrans");
        QspiTxDmacChTrans = &(QspiTxDmacHandlePtr->Channel->Trans);
        QspiTxDmacChTrans->SrcAddr        = (AL_REG)(SendData + TempSendSize);
        QspiTxDmacChTrans->DstAddr        = Handle->Dev->BaseAddr + QSPI_DR0_OFFSET;
        QspiTxDmacChTrans->TransSize      = SendSize - TempSendSize;

        /* Set cs to start transfer */
        AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, Handle->Dev->Configs.SlvSelEnum);
        // printf("n AlQspi_ll_SetSlvSel\r\n");
        Ret = AlDmacAhb_Hal_StartBlock(QspiTxDmacHandlePtr, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiTx Dmacahb hal Start Block error:0x%x\r\n", Ret);
        }
    } else {
        /* Set cs to start transfer */
        AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, Handle->Dev->Configs.SlvSelEnum);
        // while(QSPI_SR_TXFIFO_NOTEMPTY == AlQspi_ll_IsTxFifoEmpty(Handle->Dev->BaseAddr));
        // while(AlQspi_Dev_IsTxBusy(Handle->Dev));
        // AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);

    }

    Ret = AlQspi_Hal_WaitTxDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_QSPI_ERR_TIMEOUT;
    }

    AL_QSPI_HAL_UNLOCK(Handle);

    return Ret;
}


/**
 * This function is qspi dma blocking receive data
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Data is pointer to receive data
 * @param   Size is receive data size
 * @param   Timeout is max wait time for receive done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_DmaStartBlockReceive(AL_QSPI_HalStruct *Handle, AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_DMACAHB_ChTransStruct    *QspiRxDmacChTrans;

    AL_U32                      DmacDevId = 0;
    AL_S32                      Ret = AL_OK;

    AL_DMACAHB_ChInitStruct     *QspiRxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *QspiRxDmacHandlePtr;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    QspiRxDmacChConfigPtr = &QspiRxDmacChConfig;
    QspiRxDmacHandlePtr = &QspiRxDmacHandle;

    AL_QSPI_HAL_LOCK(Handle);

    Ret = AlQspi_Dev_DmaRecvData(Handle->Dev, RecvSize);
    if (Ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    if(QspiRxDmacHandlePtr->Channel == NULL) {
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

        Ret = AlDmacAhb_Hal_Init(QspiRxDmacHandlePtr, QspiRxDmacChConfigPtr, AL_NULL, DmacDevId);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiRx Dmacahb hal Init error:0x%x\r\n", Ret);
            AL_QSPI_HAL_UNLOCK(Handle);
            return Ret;
        }
    }

    QspiRxDmacChTrans = &(QspiRxDmacHandlePtr->Channel->Trans);
    QspiRxDmacChTrans->SrcAddr        = Handle->Dev->BaseAddr + QSPI_DR0_OFFSET;
    QspiRxDmacChTrans->DstAddr        = (AL_REG)RecvData;
    QspiRxDmacChTrans->TransSize      = RecvSize;

    /* Set cs to start transfer */
    AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, Handle->Dev->Configs.SlvSelEnum);

    Ret = AlDmacAhb_Hal_StartBlock(QspiRxDmacHandlePtr, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "QspiRx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    AL_QSPI_HAL_UNLOCK(Handle);

    return Ret;
}



/**
 * This function is qspi dma blocking tranfer data
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   SendData is pointer to send data
 * @param   SendSize is send data size
 * @param   RecvData is pointer to receive data
 * @param   RecvSize is receive data size
 * @param   Timeout is max wait time for send done
 * @return  The state of function execution
 * @note    None
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
    AL_U32 TempSendSize = (Handle->Dev->Configs.EnSpiCfg.AddrLength + Handle->Dev->Configs.EnSpiCfg.InstLength) / 2;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    QspiTxDmacChConfigPtr = &QspiTxDmacChConfig;
    QspiTxDmacHandlePtr = &QspiTxDmacHandle;
    QspiRxDmacChConfigPtr = &QspiRxDmacChConfig;
    QspiRxDmacHandlePtr = &QspiRxDmacHandle;

    AL_QSPI_HAL_LOCK(Handle);

    Handle->Dev->SendBuffer.BufferPtr = SendData;
    Ret = AlQspi_Dev_DmaTranferData(Handle->Dev, SendSize, RecvSize);
    if (Ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    if((QspiTxDmacHandlePtr->Channel == NULL) && (SendSize > TempSendSize)) {
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

        Ret = AlDmacAhb_Hal_Init(QspiTxDmacHandlePtr, QspiTxDmacChConfigPtr, AL_NULL, DmacDevId);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiTx Dmacahb hal Init error:0x%x\r\n", Ret);
            AL_QSPI_HAL_UNLOCK(Handle);
            return Ret;
        }

        QspiTxDmacChTrans = &(QspiTxDmacHandlePtr->Channel->Trans);
        QspiTxDmacChTrans->SrcAddr        = (AL_REG)(SendData + TempSendSize);
        QspiTxDmacChTrans->DstAddr        = Handle->Dev->BaseAddr + QSPI_DR0_OFFSET;
        QspiTxDmacChTrans->TransSize      = SendSize - TempSendSize;
    }

    if(QspiRxDmacHandlePtr->Channel == NULL) {
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

        Ret = AlDmacAhb_Hal_Init(QspiRxDmacHandlePtr, QspiRxDmacChConfigPtr, AL_NULL, DmacDevId);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiRx Dmacahb hal Init error:0x%x\r\n", Ret);
            AL_QSPI_HAL_UNLOCK(Handle);
            return Ret;
        }
    }

    QspiRxDmacChTrans = &(QspiRxDmacHandlePtr->Channel->Trans);
    QspiRxDmacChTrans->SrcAddr        = Handle->Dev->BaseAddr + QSPI_DR0_OFFSET;
    QspiRxDmacChTrans->DstAddr        = (AL_REG)RecvData;
    QspiRxDmacChTrans->TransSize      = RecvSize;

    Ret = AlDmacAhb_Hal_Start(QspiRxDmacHandlePtr);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "QspiRx Dmacahb hal Start error:0x%x\r\n", Ret);
        AL_QSPI_HAL_UNLOCK(Handle);
        return Ret;
    }

    if(SendSize > TempSendSize) {
        /* Set cs to start transfer */
        AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, Handle->Dev->Configs.SlvSelEnum);

        Ret = AlDmacAhb_Hal_StartBlock(QspiTxDmacHandlePtr, Timeout);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "QspiTx Dmacahb hal Start Block error:0x%x\r\n", Ret);
        }
        AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);
    } else {
        /* Set cs to start transfer */
        AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, Handle->Dev->Configs.SlvSelEnum);
        Ret = AlQspi_Hal_WaitTxDoneOrTimeout(Handle, Timeout);
        if (Ret != AL_OK) {
            Ret = AL_QSPI_ERR_TIMEOUT;
        }
    }

    AL_QSPI_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function is excute operations to set or check qspi configuration status.
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_IoCtl(AL_QSPI_HalStruct *Handle, AL_Qspi_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    Ret = AlQspi_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Qspi io ctl cmd error:%d\r\n", Ret);
    }

    AL_QSPI_HAL_LOCK(Handle);

    return Ret;
}


