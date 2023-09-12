/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_iic_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

AL_IIC_HalStruct AlIicHandle[AL_IIC_NUM_INSTANCE];

/**
 * This function action when receive or send data.
 * @param   IicEvent Pointer to AL_IIC_EventStruct contains event datas
 * @param   CallbackRef Pointer to a AL_IIC_HalStruct structure that contains iic dev instance
 * @return
 * @note
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
 * This function action when master send data.
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic dev instance
 * @param   Event Pointer to AL_IIC_EventStruct contains event datas
 * @param   Timeout in block mode
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
 * This function action when master receive data.
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic dev instance
 * @param   Event Pointer to AL_IIC_EventStruct contains event datas
 * @param   Timeout in block mode
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
 * This function action when slave send data.
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic dev instance
 * @param   Event Pointer to AL_IIC_EventStruct contains event datas
 * @param   Timeout in block mode
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
static AL_S32 AlIic_Hal_WaitSlaveTxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_IIC_EventStruct *Event, AL_U32 Timeout)
{
    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->TxRxEventQueue, Event, Timeout);

    while(AlIic_ll_IsSlaveActivity(Handle->Dev.HwConfig.BaseAddress));

    return Ret;
}

/**
 * This function action when slave receive data.
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic dev instance
 * @param   Event Pointer to AL_IIC_EventStruct contains event datas
 * @param   Timeout in block mode
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
static AL_S32 AlIic_Hal_WaitSlaveRxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_IIC_EventStruct *Event, AL_U32 Timeout)
{
    AL_S32 Ret;
    Ret = AlOsal_Mb_Receive(&Handle->TxRxEventQueue, Event, Timeout);

    while(AlIic_ll_IsSlaveActivity(Handle->Dev.HwConfig.BaseAddress));

    return Ret;
}

/**
 * This function initialize the IIC mode according to the specified
 *          parameters in the AL_IIC_InitStruct and initialize the associated handle.
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic dev instance
 * @param   DevId is hardware module id
 * @param   InitConfig pointer to a AL_IIC_InitStruct structure
 *          that contains the configuration information for the specified IIC peripheral
 * @return
 *          - AL_OK for function success
 *          - Other for function failuregit
 * @note
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
 * This function master send an amount of data in blocking & interrupt mode
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic device instance
 * @param   SlaveAddr Slave address
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @param   Timeout Timeout duration
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
    }

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    if (Ret == AL_OK && (IicEvent.Events == AL_IIC_EVENT_TX_DONE))
        return AL_OK;
    else
        return (Ret != AL_OK) ? Ret : AL_IIC_EVENTS_TO_ERRS(IicEvent.Events);
}

/**
 * This function master send an amount of data in polling(non interrupt) & blocking mode.
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic device instance
 * @param   SlaveAddr Slave address
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
 * This function master receive an amount of data in blocking & interrupt mode
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic device instance
 * @param   SlaveAddr Slave address
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @param   Timeout Timeout duration
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
 * This function master receive an amount of data in polling(non interrupt) & blocking mode.
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic device instance
 * @param   SlaveAddr Slave address
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
 * This function slave send an amount of data in blocking & interrupt mode
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @param   Timeout Timeout duration
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
 * This function slave receive an amount of data in blocking & interrupt mode
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @param   Timeout Timeout duration
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
        AlIic_Dev_StopSlaveSend(&Handle->Dev);
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
 * This function master set command option, like stop、restart etc
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic device instance
 * @param   CmdOption Command to be set
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
 * This function master get current command option, like stop、restart etc
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic device instance
 * @return
 *          - Command for function success
 *          - AL_IIC_CMD_OPTION_NONE for function failure
 * @note
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
 * This function excute operations to set
 * @param   Handle Pointer to a AL_IIC_HalStruct structure that contains iic device instance
 * @param   Cmd is io ctl cmd to AL_IIC_IoCtlCmdEnum
 * @param   Data Pointer to cmd args
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
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
