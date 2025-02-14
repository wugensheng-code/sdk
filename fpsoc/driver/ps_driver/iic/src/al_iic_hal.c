/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_iic_hal.h"
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

AL_IIC_HalStruct AlIicHandle[AL_IIC_NUM_INSTANCE];

AL_DMACAHB_ChInitStruct     Iic0TxDmacChConfig;
AL_DMACAHB_HalStruct        *Iic0TxDmacHandle = AL_NULL;
AL_DMACAHB_ChInitStruct     Iic0RxDmacChConfig;
AL_DMACAHB_HalStruct        *Iic0RxDmacHandle = AL_NULL;

AL_DMACAHB_ChInitStruct     Iic1TxDmacChConfig;
AL_DMACAHB_HalStruct        *Iic1TxDmacHandle = AL_NULL;
AL_DMACAHB_ChInitStruct     Iic1RxDmacChConfig;
AL_DMACAHB_HalStruct        *Iic1RxDmacHandle = AL_NULL;

/**
 *
 * This function is called when an IIC event occurs. It handles various IIC events such as
 * receive underflow, receive overflow, transmit overflow, read request, transmit abort,
 * receive done, transmit done, activity, stop detection, start detection, and general call.
 * For most events, it sends the event data to the TxRxEventQueue. For the TX abort event,
 * it also logs an error message with the abort source.
 *
 * @param IicEvent Pointer to an AL_IIC_EventStruct structure that contains event data.
 * @param CallbackRef Pointer to a user-defined callback reference, typically an instance of AL_IIC_HalStruct.
 *
 * @return None.
 *
 */
static AL_VOID AlIic_DefEventCallBack(AL_IIC_EventStruct *IicEvent, void *CallbackRef)
{
    AL_IIC_HalStruct *Handle = (AL_IIC_HalStruct *)CallbackRef;

    switch (IicEvent->Events) {
        case AL_IIC_EVENT_RX_UNDER:
        case AL_IIC_EVENT_RX_OVER:
        case AL_IIC_EVENT_TX_OVER:
        AlOsal_Mb_Send(&Handle->TxRxEventQueue, IicEvent);
        break;

        case AL_IIC_EVENT_RD_REQ:
        break;

        case AL_IIC_EVENT_TX_ABRT:
        AlOsal_Mb_Send(&Handle->TxRxEventQueue, IicEvent);
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_IIC_EVENT_TX_ABRT source:0x%x\r\n", IicEvent->EventData);
        break;

        case AL_IIC_EVENT_RX_DONE:
        AlOsal_Mb_Send(&Handle->TxRxEventQueue, IicEvent);
        break;

        case AL_IIC_EVENT_TX_DONE:
        AlOsal_Mb_Send(&Handle->TxRxEventQueue, IicEvent);
        break;

        case AL_IIC_EVENT_ACTIVITY:
        case AL_IIC_EVENT_STOP_DET:
        case AL_IIC_EVENT_START_DET:
        case AL_IIC_EVENT_GEN_CALL:
        break;

        default:
        break;
    }
}

/**
 *
 * This function waits for a master transmit operation to complete. It checks for the completion
 * of the operation by receiving an event from the TxRxEventQueue. If the device is configured
 * to stop after the operation, it waits until the master activity flag is cleared.
 *
 * @param Handle Pointer to a AL_IIC_HalStruct structure that contains the IIC device instance.
 * @param Event Pointer to an AL_IIC_EventStruct structure to receive the event data.
 * @param Timeout Timeout duration in block mode.
 *
 * @return AL_OK if the operation was successful, otherwise an error code.
 *
 */
static AL_S32 AlIic_Hal_WaitMasterTxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_IIC_EventStruct *Event, AL_U32 Timeout)
{
    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->TxRxEventQueue, Event, Timeout);

    if (Handle->Dev.CmdOption == AL_IIC_CMD_OPTION_STOP) {
        while(AlIic_ll_IsMasterActivity(Handle->Dev.HwConfig.BaseAddress));
    }

    return Ret;
}

/**
 *
 * Similar to AlIic_Hal_WaitMasterTxDoneOrTimeout, but for receive operations.
 *
 * @param Handle Pointer to a AL_IIC_HalStruct structure that contains the IIC device instance.
 * @param Event Pointer to an AL_IIC_EventStruct structure to receive the event data.
 * @param Timeout Timeout duration in block mode.
 *
 * @return AL_OK if the operation was successful, otherwise an error code.
 *
 */
static AL_S32 AlIic_Hal_WaitMasterRxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_IIC_EventStruct *Event, AL_U32 Timeout)
{
    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->TxRxEventQueue, Event, Timeout);

    if (Handle->Dev.CmdOption == AL_IIC_CMD_OPTION_STOP) {
        while(AlIic_ll_IsMasterActivity(Handle->Dev.HwConfig.BaseAddress));
    }

    return Ret;
}

/**
 *
 * This function waits for a slave transmit operation to complete by checking the slave activity flag.
 *
 * @param Handle Pointer to a AL_IIC_HalStruct structure that contains the IIC device instance.
 * @param Event Pointer to an AL_IIC_EventStruct structure to receive the event data.
 * @param Timeout Timeout duration in block mode.
 *
 * @return AL_OK if the operation was successful, otherwise an error code.
 *
 */
static AL_S32 AlIic_Hal_WaitSlaveTxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_IIC_EventStruct *Event, AL_U32 Timeout)
{
    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->TxRxEventQueue, Event, Timeout);

    while(AlIic_ll_IsSlaveActivity(Handle->Dev.HwConfig.BaseAddress));

    return Ret;
}

/**
 *
 * Similar to AlIic_Hal_WaitSlaveTxDoneOrTimeout, but for receive operations.
 *
 * @param Handle Pointer to a AL_IIC_HalStruct structure that contains the IIC device instance.
 * @param Event Pointer to an AL_IIC_EventStruct structure to receive the event data.
 * @param Timeout Timeout duration in block mode.
 *
 * @return AL_OK if the operation was successful, otherwise an error code.
 *
 */
static AL_S32 AlIic_Hal_WaitSlaveRxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_IIC_EventStruct *Event, AL_U32 Timeout)
{
    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->TxRxEventQueue, Event, Timeout);

    while(AlIic_ll_IsSlaveActivity(Handle->Dev.HwConfig.BaseAddress));

    return Ret;
}

/**
 *
 * This function initializes the IIC module based on the provided hardware configuration and
 * initialization parameters. It also registers an event callback function for IIC events.
 *
 * @param Handle Double pointer to a AL_IIC_HalStruct structure for returning the initialized IIC device instance.
 * @param DevId Hardware module ID.
 * @param InitConfig Pointer to an AL_IIC_InitStruct structure containing the configuration information.
 * @param Callback Event callback function to be registered.
 *
 * @return AL_OK if the initialization was successful, otherwise an error code.
 *
 *
 */
AL_S32 AlIic_Hal_Init(AL_IIC_HalStruct **Handle, AL_U32 DevId,
                      AL_IIC_InitStruct *InitConfig, AL_IIC_EventCallBack Callback)
{
    AL_S32 Ret = AL_OK;
    AL_IIC_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    HwConfig = AlIic_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AlIicHandle[DevId];
    } else {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    Ret = AlIic_Dev_Init(&(*Handle)->Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    if(AL_NULL == Callback) {
        Ret = AlIic_Dev_RegisterEventCallBack(&(*Handle)->Dev, AlIic_DefEventCallBack, (void *)*Handle);
    } else {
        Ret = AlIic_Dev_RegisterEventCallBack(&(*Handle)->Dev, Callback, (void *)*Handle);
    }
    if (Ret != AL_OK) {
        return Ret;
    }

    (AL_VOID)AlIntr_RegHandler((*Handle)->Dev.HwConfig.IntrId, AL_NULL, AlIic_Dev_IntrHandler, &(*Handle)->Dev);

    Ret = AlOsal_Lock_Init(&(*Handle)->Lock, "Iic-Lock");
    if (Ret != AL_OK) {
        return Ret;
    }

    AlOsal_Mb_Init(&(*Handle)->TxRxEventQueue, "IIC_TXRXDONE");

    return AL_OK;
}

/**
 *
 * This function sends data to a specified slave address in either blocking or interrupt mode.
 * It waits for the transmit operation to complete or timeout.
 *
 * @param Handle Pointer to a AL_IIC_HalStruct structure that contains the IIC device instance.
 * @param SlaveAddr Slave address.
 * @param Data Pointer to the data buffer to be sent.
 * @param Size Amount of data to be sent.
 * @param Timeout Timeout duration.
 *
 * @return AL_OK if the operation was successful, otherwise an error code.
 *
 */
AL_S32 AlIic_Hal_MasterSendDataBlock(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_IIC_EventStruct IicEvent = {0};

    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlIic_Dev_MasterSendData(&Handle->Dev, SlaveAddr, Data, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);
        return Ret;
    }

    /*
     * wait until data send done
     */
    Ret = AlIic_Hal_WaitMasterTxDoneOrTimeout(Handle, &IicEvent, Timeout);
    if (Ret != AL_OK) {
        AlIic_Dev_StopMasterSend(&Handle->Dev);
        (AL_VOID)AlOsal_Mb_Receive(&Handle->TxRxEventQueue, &IicEvent, AL_WAITING_NO);
    } else {
        AlIic_Dev_StopMasterSend(&Handle->Dev);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    if (Ret == AL_OK && (IicEvent.Events == AL_IIC_EVENT_TX_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_IIC_EVENTS_TO_ERRS(IicEvent.Events);
}

/**
 *
 * This function master send an amount of data in DMA & blocking & interrupt mode
 *
 * @param Handle Pointer to a AL_IIC_HalStruct structure that contains the IIC device instance.
 * @param SlaveAddr Slave address.
 * @param Data Pointer to the data buffer to be sent.
 * @param Size Amount of data to be sent.
 * @param Timeout Timeout duration.
 *
 * @return AL_OK if the operation was successful, otherwise an error code.
 *
*/
AL_S32 AlIic_Hal_MasterDmaSendDataBlock(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U32 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    AL_U32                      DmacDevId = 0;
    AL_DMACAHB_ChTransStruct    *IicTxDmacChTrans;
    AL_DMACAHB_ChInitStruct     *IicTxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *IicTxDmacHandlePtr;

    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    if (Handle->Dev.HwConfig.BaseAddress < I2C1__BASE_ADDR) {
        IicTxDmacChConfigPtr = &Iic0TxDmacChConfig;
        IicTxDmacHandlePtr = Iic0TxDmacHandle;
    } else {
        IicTxDmacChConfigPtr = &Iic1TxDmacChConfig;
        IicTxDmacHandlePtr = Iic1TxDmacHandle;
    }

    Ret = AlIic_Dev_MasterDmaSendData(&Handle->Dev, SlaveAddr);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);
        return Ret;
    }

    if (IicTxDmacHandlePtr == NULL) {
        IicTxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_6;
        IicTxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        IicTxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        IicTxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR;
        IicTxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_32;
        IicTxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_32;
        IicTxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        IicTxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        IicTxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        IicTxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;

        IicTxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_MEM2PER;
        if (Handle->Dev.HwConfig.BaseAddress < I2C1__BASE_ADDR) {
            IicTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_I2C0_TX;
        } else {
            IicTxDmacChConfigPtr->HandShaking.DstPer = AL_DMACAHB_PER_I2C1_TX;
        }
        IicTxDmacChConfigPtr->HandShaking.DstHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        IicTxDmacChConfigPtr->HandShaking.DstHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

        IicTxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        IicTxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        IicTxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_6;
        IicTxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        IicTxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        IicTxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        IicTxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;

        Ret = AlDmacAhb_Hal_Init(&IicTxDmacHandlePtr, DmacDevId, IicTxDmacChConfigPtr, AL_NULL);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "IicTx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);
            return Ret;
        }
        if (Handle->Dev.HwConfig.BaseAddress < I2C1__BASE_ADDR) {
            Iic0TxDmacHandle = IicTxDmacHandlePtr;
        } else {
            Iic1TxDmacHandle = IicTxDmacHandlePtr;
        }
    }

    IicTxDmacChTrans = &(IicTxDmacHandlePtr->Channel.Trans);

    IicTxDmacChTrans->SrcAddr        = (AL_REG)Data;
    IicTxDmacChTrans->DstAddr        = Handle->Dev.HwConfig.BaseAddress + I2C__IC_DATA_CMD__OFFSET;
    IicTxDmacChTrans->TransSize      = Size;

    Ret = AlDmacAhb_Hal_StartBlock(IicTxDmacHandlePtr, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IicTx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return Ret;
}

/**
 *
 * This function sends data to the IIC bus in master mode using polling.
 *
 * @param Handle Pointer to the AL_IIC_HalStruct structure.
 * @param SlaveAddr The address of the slave device.
 * @param Data Pointer to the data buffer to be sent.
 * @param Size The size of the data buffer.
 *
 * @return Returns AL_OK if successful, otherwise an error code.
 *
 */
AL_S32 AlIic_Hal_MasterSendDataPolling(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlIic_Dev_MasterSendDataPolling(&Handle->Dev, SlaveAddr, Data, Size);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return Ret;
}

/**
 *
 * This function is used to receive a block of data from an I2C slave device in master mode.
 *
 * @param Handle Pointer to the AL_IIC_HalStruct representing the I2C handle.
 * @param SlaveAddr The 7-bit I2C slave address.
 * @param Data Pointer to the buffer where the received data will be stored.
 * @param Size The number of bytes to receive.
 * @param Timeout The timeout value in milliseconds.
 *
 * @return Returns AL_OK if the operation is successful. Otherwise, it returns an error code.
 *
 */
AL_S32 AlIic_Hal_MasterRecvDataBlock(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_IIC_EventStruct IicEvent = {0};

    /* check only Handle, more checks in AlIic_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlIic_Dev_MasterRecvData(&Handle->Dev, SlaveAddr, Data, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);
        return Ret;
    }

    /* wait until data receive done */
    Ret = AlIic_Hal_WaitMasterRxDoneOrTimeout(Handle, &IicEvent, Timeout);
    if (Ret != AL_OK) {
        AlIic_Dev_StopMasterRecv(&Handle->Dev);
        (AL_VOID)AlOsal_Mb_Receive(&Handle->TxRxEventQueue, &IicEvent, AL_WAITING_NO);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    if (Ret == AL_OK && (IicEvent.Events == AL_IIC_EVENT_RX_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_IIC_EVENTS_TO_ERRS(IicEvent.Events);
}


/**
 *
 * This function receives a block of data from the specified slave device using DMA transfer.
 *
 * @param Handle Pointer to the AL_IIC_HalStruct structure representing the I2C controller handle.
 * @param SlaveAddr The address of the slave device.
 * @param Data Pointer to the buffer where the received data will be stored.
 * @param Size The size of the data block to receive.
 * @param Timeout The timeout value in milliseconds for the operation.
 *
 * @return Returns AL_OK if the operation is successful, or an error code if it fails.
 *
 */
AL_S32 AlIic_Hal_MasterDmaRecvDataBlock(AL_IIC_HalStruct *Handle,AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    AL_DMACAHB_ChTransStruct    *IicRxDmacChTrans;
    AL_U32                      DmacDevId = 0;

    AL_DMACAHB_ChInitStruct     *IicRxDmacChConfigPtr;
    AL_DMACAHB_HalStruct        *IicRxDmacHandlePtr;

    /* check only Handle, more checks in AlIic_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    if (Handle->Dev.HwConfig.BaseAddress < I2C1__BASE_ADDR) {
        IicRxDmacChConfigPtr = &Iic0RxDmacChConfig;
        IicRxDmacHandlePtr = Iic0RxDmacHandle;
    } else {
        IicRxDmacChConfigPtr = &Iic1RxDmacChConfig;
        IicRxDmacHandlePtr = Iic1RxDmacHandle;
    }

    Ret = AlOsal_Lock_Take(&Handle->Lock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    if (IicRxDmacHandlePtr == NULL) {
        IicRxDmacChConfigPtr->Id = AL_DMACAHB_CHANNEL_7;
        IicRxDmacChConfigPtr->TransType = AL_DMACAHB_TRANS_TYPE_1;
        IicRxDmacChConfigPtr->Intr.IsIntrEn = AL_TRUE;
        IicRxDmacChConfigPtr->Intr.IntrUnMask = AL_DMACAHB_CH_INTR_TFR | AL_DMACAHB_CH_INTR_ERR | AL_DMACAHB_CH_INTR_SRCT | AL_DMACAHB_CH_INTR_DSTT;
        IicRxDmacChConfigPtr->SrcTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        IicRxDmacChConfigPtr->DstTransWidth = AL_DMACAHB_TRANS_WIDTH_8;
        IicRxDmacChConfigPtr->SrcAddrIncMode = AL_DMACAHB_ADDR_INC_NO0;
        IicRxDmacChConfigPtr->DstAddrIncMode = AL_DMACAHB_ADDR_INC_INC;
        IicRxDmacChConfigPtr->SrcBurstLength = AL_DMACAHB_MSIZE_1;
        IicRxDmacChConfigPtr->DstBurstLength = AL_DMACAHB_MSIZE_1;
        IicRxDmacChConfigPtr->Direction = AL_DMACAHB_TT_FC_PER2MEM;
        if (Handle->Dev.HwConfig.BaseAddress < I2C1__BASE_ADDR) {
            IicRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_I2C0_RX;
        } else {
            IicRxDmacChConfigPtr->HandShaking.SrcPer = AL_DMACAHB_PER_I2C1_RX;
        }
        IicRxDmacChConfigPtr->HandShaking.SrcHsSel = AL_DMACAHB_HAND_SHAKING_HARDWARE;
        IicRxDmacChConfigPtr->HandShaking.SrcHsPol = AL_DMACAHB_HS_POL_ACTIVE_HI;

        IicRxDmacChConfigPtr->SrcMasterSel = AL_DMACAHB_MASTER_SEL_1;
        IicRxDmacChConfigPtr->DstMasterSel = AL_DMACAHB_MASTER_SEL_2;
        IicRxDmacChConfigPtr->ChPrior = AL_DMACAHB_CH_PRIOR_7;
        IicRxDmacChConfigPtr->FifoMode = AL_DMACAHB_FIFO_MODE_0;
        IicRxDmacChConfigPtr->ProtCtl = AL_DMACAHB_PROT_0;
        IicRxDmacChConfigPtr->SgrDsr.IsSrcGatherEn = AL_FALSE;
        IicRxDmacChConfigPtr->SgrDsr.IsDstScatterEn = AL_FALSE;

        Ret = AlDmacAhb_Hal_Init(&IicRxDmacHandlePtr, DmacDevId, IicRxDmacChConfigPtr, AL_NULL);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "IicRx Dmacahb hal Init error:0x%x\r\n", Ret);
            (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);
            return Ret;
        }

        if (Handle->Dev.HwConfig.BaseAddress < I2C1__BASE_ADDR) {
            Iic0RxDmacHandle = IicRxDmacHandlePtr;
        } else {
            Iic1RxDmacHandle = IicRxDmacHandlePtr;
        }
    }

    IicRxDmacChTrans = &(IicRxDmacHandlePtr->Channel.Trans);
    IicRxDmacChTrans->SrcAddr        = Handle->Dev.HwConfig.BaseAddress + I2C__IC_DATA_CMD__OFFSET;
    IicRxDmacChTrans->DstAddr        = (AL_REG)Data;
    IicRxDmacChTrans->TransSize      = Size;

    Ret = AlIic_Dev_MasterDmaRecvData(&Handle->Dev, SlaveAddr, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);
        return Ret;
    }

    Ret = AlDmacAhb_Hal_StartBlock(IicRxDmacHandlePtr, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IICRx Dmacahb hal Start Block error:0x%x\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return Ret;
}


/**
 *
 * This function is responsible for performing a polling-based master receive operation on the IIC bus.
 * It locks the handle, calls the AlIic_Dev_MasterRecvDataPolling function to perform the receive operation,
 * and then releases the lock before returning the result.
 *
 * @param Handle Pointer to the AL_IIC_HalStruct handle structure.
 * @param SlaveAddr The address of the slave device.
 * @param Data Pointer to the buffer where the received data will be stored.
 * @param Size The number of bytes to receive.
 *
 * @return Returns AL_OK if the operation is successful, otherwise returns an error code.
 *
 */
AL_S32 AlIic_Hal_MasterRecvDataPolling(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    /* check only Handle, more checks in AlIic_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlIic_Dev_MasterRecvDataPolling(&Handle->Dev, SlaveAddr, Data, Size);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return Ret;
}


/**
 *
 * This function sends a data block to the IIC slave device specified by the given handle.
 *
 * @param Handle Pointer to the AL_IIC_HalStruct representing the IIC handle.
 * @param Data Pointer to the data block to be sent.
 * @param Size Size of the data block in bytes.
 * @param Timeout Timeout value in milliseconds.
 *
 * @return Returns AL_OK if the data block was sent successfully, or an error code if an error occurred.
 *
 */
AL_S32 AlIic_Hal_SlaveSendDataBlock(AL_IIC_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_IIC_EventStruct IicEvent = {0};

    /*
     * check only Handle, more checks in AlIic_Dev_Init function;
    */
    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlIic_Dev_SlaveSendData(&Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);
        return Ret;
    }

    /*
     * wait until data send done
    */
    Ret = AlIic_Hal_WaitSlaveTxDoneOrTimeout(Handle, &IicEvent, Timeout);
    if (Ret != AL_OK) {
        AlIic_Dev_StopSlaveSend(&Handle->Dev);
        (AL_VOID)AlOsal_Mb_Receive(&Handle->TxRxEventQueue, &IicEvent, AL_WAITING_NO);
    }
    if (Ret == AL_OK && (IicEvent.Events == AL_IIC_EVENT_TX_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_IIC_EVENTS_TO_ERRS(IicEvent.Events);
}


/**
 *
 * This function receives a block of data from the I2C slave device specified by the given handle.
 * It locks the handle, calls the AlIic_Dev_SlaveRecvData function to receive the data, and then waits
 * until the data receive is done or a timeout occurs. If a timeout occurs, the function stops the
 * slave receive operation and releases the lock. The function returns the status of the operation.
 *
 * @param Handle Pointer to the AL_IIC_HalStruct representing the I2C handle.
 * @param Data Pointer to the buffer where the received data will be stored.
 * @param Size The size of the data to receive, in bytes.
 * @param Timeout The timeout value in milliseconds.
 *
 * @return The status of the operation. Returns AL_OK if the data receive is successful, or an error code
 *         if an error occurs.
 *
 */
AL_S32 AlIic_Hal_SlaveRecvDataBlock(AL_IIC_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_IIC_EventStruct IicEvent = {0};

    /* check only Handle, more checks in AlIic_Dev_Init function */
    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlIic_Dev_SlaveRecvData(&Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);
        return Ret;
    }

    /* wait until data receive done */
    Ret = AlIic_Hal_WaitSlaveRxDoneOrTimeout(Handle, &IicEvent, Timeout);
    if (Ret != AL_OK) {
        AlIic_Dev_StopSlaveRecv(&Handle->Dev);
        (AL_VOID)AlOsal_Mb_Receive(&Handle->TxRxEventQueue, &IicEvent, AL_WAITING_NO);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    if (Ret == AL_OK && (IicEvent.Events == AL_IIC_EVENT_RX_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_IIC_EVENTS_TO_ERRS(IicEvent.Events);

    return Ret;
}


/**
 *
 * This function sets the command option for the IIC master specified by the given handle.
 *
 * @param Handle Pointer to the AL_IIC_HalStruct representing the IIC master.
 * @param CmdOption The command option to be set.
 *
 * @return AL_S32 Returns AL_OK if successful, otherwise returns an error code.
 *
 */
AL_S32 AlIic_Hal_MasterSetCmdOption(AL_IIC_HalStruct *Handle, AL_IIC_CmdOptionEnum CmdOption)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlIic_Dev_MasterSetCmdOption(&Handle->Dev, CmdOption);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Dev_MasetSetCmdOption error:%d\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return Ret;
}

/**
 *
 * This function retrieves the command option for the IIC master.
 *
 * @param Handle Pointer to the AL_IIC_HalStruct instance.
 *
 * @return The command option for the IIC master.
 *
 */
AL_IIC_CmdOptionEnum AlIic_Hal_MastertGetCmdOption(AL_IIC_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;
    AL_IIC_CmdOptionEnum CmdOption = AL_IIC_CMD_OPTION_NONE;

    AL_ASSERT((Handle != AL_NULL), AL_IIC_CMD_OPTION_NONE);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return AL_IIC_CMD_OPTION_NONE;
    }

    CmdOption = AlIic_Dev_MasterGetCmdOption(&Handle->Dev);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return CmdOption;
}


/**
 *
 * This function performs an I2C HAL IOCTL operation specified by the given command and data.
 *
 * @param Handle Pointer to the AL_IIC_HalStruct representing the I2C HAL handle.
 * @param Cmd The I2C IOCTL command to be executed.
 * @param Data Pointer to the data associated with the IOCTL command.
 *
 * @return Returns AL_OK if the operation is successful, otherwise returns an error code.
 *
 */
AL_S32 AlIic_Hal_IoCtl(AL_IIC_HalStruct *Handle, AL_IIC_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL), AL_IIC_ERR_ILLEGAL_PARAM);

    Ret = AlOsal_Lock_Take(&Handle->Lock, AL_WAITFOREVER);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlIic_Dev_IoCtl(&Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Dev_IoCtl error:%d\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return Ret;
}
