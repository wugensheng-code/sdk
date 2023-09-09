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
 * This function wait for frame send done or timeout
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlDmacAhb_Hal_WaitTransDoneOrTimeout(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_EventStruct *Event, AL_U32 TimeoutMs)
{
    return AlOsal_Mb_Receive(&Handle->EventQueue, Event, TimeoutMs);
}

/**
 * This function is intr handler call back function
 * @param   Event is pointer to AL_DMACAHB_EventStruct
 * @param   CallBackRef
 * @return
 * @note
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

    return Ret;
}

/**
 * This function init DMACAHB module
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @param   InitConfig is module config structure with AL_DMACAHB_ChInitStruct
 * @param   CallBack is call back struct with AL_DMACAHB_ChCallBackStruct
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK
 * @note
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
 * This function init DMACAHB module
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @return
 *          - AL_OK
 * @note
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
 * This function start dma
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @return
 *          - AL_OK
 * @note
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
 * This function start dma blocked
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @param   Timeout is max wait time for dma done
 * @return
 *          - AL_OK
 * @note
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
 * This function recv frame blocked
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @param   Cmd is io ctl cmd to AL_DMACAHB_IoCtlCmdEnum
 * @param   Data is pointer to cmd args
 * @return
 *          - AL_OK
 * @note
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


