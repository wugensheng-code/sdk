/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_dmacahb_hal.h"
#include "al_dmacahb_dev.h"
#include "al_dmacahb_ll.h"
#include "al_core.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_DMACAHB_HalStruct AL_DMACAHB_HalInstance[AL_DMACAHB_NUM_INSTANCE][AL_DMACAHB_CHANNEL_NUM];

/************************** Function Prototypes ******************************/

/************************** Function Definitions ******************************/
/**
 * This function waits for a transaction to complete or timeout.
 *
 * @param Handle Pointer to the DMA controller handle.
 * @param Event Pointer to the event structure where the event details will be stored.
 * @param TimeoutMs Timeout in milliseconds.
 * @return AL_S32 Status of the operation. Returns 0 (AL_OK) if successful.
 */
AL_S32 AlDmacAhb_Hal_WaitTransDoneOrTimeout(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_EventStruct *Event, AL_U32 TimeoutMs)
{
    return AlOsal_Mb_Receive(&Handle->EventQueue, Event, TimeoutMs);
}

/**
 * This function waits for a source transaction to complete or timeout.
 *
 * @param Handle Pointer to the DMA controller handle.
 * @param Event Pointer to the event structure where the event details will be stored.
 * @param TimeoutMs Timeout in milliseconds.
 * @return AL_S32 Status of the operation. Returns 0 (AL_OK) if successful.
 */
AL_S32 AlDmacAhb_Hal_WaitSrcTransDoneOrTimeout(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_EventStruct *Event, AL_U32 TimeoutMs)
{
    return AlOsal_Mb_Receive(&Handle->SrcEventQueue, Event, TimeoutMs);
}

/**
 * Default channel event callback function.
 *
 * @param Event Pointer to the event structure containing the event details.
 * @param CallBackRef User-defined callback reference.
 * @return AL_S32 Returns 0 (AL_OK) if successful.
 */
static AL_S32 AlDmacAhb_Hal_DefChEventCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_DMACAHB_HalStruct *Handle = (AL_DMACAHB_HalStruct *)CallBackRef;

    AL_ASSERT((Event != AL_NULL) && (CallBackRef != AL_NULL), AL_DMACAHB_ERR_NULL_PTR);

    switch (Event->EventId)
    {
    case AL_DMACAHB_EVENT_TRANS_COMP:
    case AL_DMACAHB_EVENT_BLOCK_TRANS_COMP:
        AlOsal_Mb_Send(&Handle->EventQueue, Event);
        break;
    case AL_DMACAHB_EVENT_SRC_TRANS_COMP:
        AlOsal_Mb_Send(&Handle->SrcEventQueue, Event);
        break;
    case AL_DMACAHB_EVENT_DST_TRANS_COMP:
        /* Not use, do nothing here */
        break;
    case AL_DMACAHB_EVENT_ERR:
        /* ???????? */
        break;
    case AL_DMACAHB_EVENT_RELOAD:
        /* Use for forever reload mode, user do something here */
        AL_LOG(AL_LOG_LEVEL_INFO, "Reload cnt: %d\r\n", Event->EventData);
        break;
    default:
        break;
    }
    return AL_OK;
}

/**
 * This function initializes the DMA controller handle.
 *
 * @param Handle Pointer to the DMA controller handle.
 * @return AL_S32 Returns 0 (AL_OK) if successful.
 */
static inline AL_S32 AlDmacahb_Hal_HandleInit(AL_DMACAHB_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    Ret = AlOsal_Lock_Init(&Handle->Lock, "Dmacahb-Lock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&Handle->EventQueue, "Dmacahb-Queue");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&Handle->SrcEventQueue, "Dmacahb-SrcQueue");
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}

/**
 * This function initializes the DMA controller with the specified configuration.
 *
 * @param Handle Double pointer to the DMA controller handle to be initialized.
 * @param DevId Device ID for the DMA controller.
 * @param InitConfig Pointer to the initialization configuration structure.
 * @param CallBack Event callback function.
 * @return AL_S32 Returns 0 (AL_OK) if successful.
 */
AL_S32 AlDmacAhb_Hal_Init(AL_DMACAHB_HalStruct **Handle, AL_U32 DevId, AL_DMACAHB_ChInitStruct *InitConfig,
                          AL_DMACAHB_ChEventCallBack CallBack)
{
    AL_S32 Ret = AL_OK;
    AL_DMACAHB_ChStruct *Channel;
    AL_DMACAHB_ChIdEnum AvailableId = AL_DMACAHB_CHANNEL_UNAVAILABLE;
    AL_DMACAHB_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_DMACAHB_ERR_NULL_PTR);

    /* 1. look up hardware config */
    HwConfig = AlDmacAhb_Dev_LookupConfig(DevId);
    if (HwConfig == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    Ret = AlDmacAhb_Dev_ClrChannelState(HwConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlDmacAhb_Dev_RequestCh(HwConfig, InitConfig->Id, &AvailableId);
    if (Ret != AL_OK) {
        return Ret;
    }

    InitConfig->Id = AvailableId;
    *Handle = &AL_DMACAHB_HalInstance[DevId][InitConfig->Id];
    Channel = &((*Handle)->Channel);

    /* 2. Init IP */
    Ret = AlDmacAhb_Dev_Init(Channel, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* 3. register callback */
    if (CallBack == AL_NULL) {
        AlDmacAhb_Dev_RegisterChEventCallBack(Channel, AlDmacAhb_Hal_DefChEventCallBack, *Handle);
    } else {
        AlDmacAhb_Dev_RegisterChEventCallBack(Channel, CallBack, *Handle);
    }

    /* 4. register intr */
    if (Channel->Dmac->State.IntrEn == AL_FALSE) {
        Channel->Dmac->State.IntrEn = AL_TRUE;
        AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlDmacAhb_Dev_IntrHandler, Channel->Dmac);
    }

    Ret = AlDmacahb_Hal_HandleInit(*Handle);

    return Ret;
}

/**
 * This function deinitializes the DMA controller.
 *
 * @param Handle Pointer to the DMA controller handle.
 * @return AL_S32 Returns 0 (AL_OK) if successful.
 */
AL_S32 AlDmacAhb_Hal_DeInit(AL_DMACAHB_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT(Handle != AL_NULL, AL_DMACAHB_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->Lock), 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* TODO: Need consider on share with linux, do not shut down intr */
    // if ((Handle->Channel->Dmac->State.IntrEn == AL_TRUE) &&
    //     ((Handle->Channel->Dmac->State.ChEn & (Handle->Channel->Dmac->State.ChEn - 1)) == 0)) {
    //     /* Only disable intr but not unregister intr handler */
    //     AlIntr_SetInterrupt(Handle->Channel->Dmac->IntrId, AL_FUNC_DISABLE);
    // }

    Ret = AlDmacAhb_Dev_DeInit(&(Handle->Channel));
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&(Handle->Lock));
        return Ret;
    }

    (AL_VOID)AlOsal_Lock_Release(&(Handle->Lock));

    return Ret;
}

/**
 * This function starts a DMA transaction.
 *
 * @param Handle Pointer to the DMA controller handle.
 * @return AL_S32 Returns 0 (AL_OK) if successful.
 */
AL_S32 AlDmacAhb_Hal_Start(AL_DMACAHB_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT(Handle != AL_NULL, AL_DMACAHB_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->Lock), 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlDmacAhb_Dev_Start(&(Handle->Channel));
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Dmacahb start error:%x\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&(Handle->Lock));

    return Ret;
}

/**
 * This function starts a DMA transaction and waits for it to complete or timeout.
 *
 * @param Handle Pointer to the DMA controller handle.
 * @param Timeout Timeout in milliseconds.
 * @return AL_S32 Returns 0 (AL_OK) if successful, otherwise returns an error code.
 */
AL_S32 AlDmacAhb_Hal_StartBlock(AL_DMACAHB_HalStruct *Handle, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_DMACAHB_EventStruct Event = {0};

    AL_ASSERT(Handle != AL_NULL, AL_DMACAHB_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->Lock), Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlDmacAhb_Dev_Start(&(Handle->Channel));
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Dmacahb start block error:0x%x\r\n", Ret);
        (AL_VOID)AlOsal_Lock_Release(&(Handle->Lock));
        return Ret;
    }

    Ret = AlDmacAhb_Hal_WaitTransDoneOrTimeout(Handle, &Event, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Dmacahb wait trans done error:%x\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&(Handle->Lock));

    if (Ret == AL_OK && ((Event.EventId == AL_DMACAHB_EVENT_TRANS_COMP) ||
        (Event.EventId == AL_DMACAHB_EVENT_BLOCK_TRANS_COMP))) {
        return Ret;
    } else {
        return (Ret != AL_OK) ? Ret : AL_DMACAHB_EVENTS_TO_ERRS(Event.EventId);
    }

    return Ret;
}

/**
 * This function performs an IOCTL command on the DMA controller.
 *
 * @param Handle Pointer to the DMA controller handle.
 * @param Cmd IOCTL command to be performed.
 * @param Data Pointer to the data required for the IOCTL command.
 * @return AL_S32 Returns 0 (AL_OK) if successful.
 */
AL_S32 AlDmacAhb_Hal_IoCtl(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL) && (Data != AL_NULL), AL_DMACAHB_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&Handle->Lock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    AlDmacAhb_Dev_IoCtl(&(Handle->Channel), Cmd, Data);

    (AL_VOID)AlOsal_Lock_Release(&Handle->Lock);

    return Ret;
}
