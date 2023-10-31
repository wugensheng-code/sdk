/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include <stdlib.h>
#include "al_can_hal.h"
#include "al_dmacahb_hal.h"
#include "al_osal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
/* Static CAN controller instance */
static AL_CAN_HalStruct AL_CAN_HalInstance[AL_CAN_NUM_INSTANCE];

/************************** Function Prototypes ******************************/
extern AL_BOOL AlCan_Dev_GetState(AL_CAN_DevStruct *Dev, AL_CAN_StateEnum State);

/************************** Function Definitions ******************************/
/**
 * This function wait for frame send done or timeout
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   TimeoutMs is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlCan_Hal_WaitSendDoneOrTimeout(AL_CAN_HalStruct *Handle, AL_CAN_EventStruct *Event, AL_U32 TimeoutMs)
{
    return AlOsal_Mb_Receive(&(Handle->TxEventQueue), Event, TimeoutMs);
}

/**
 * This function wait for frame recv done or timeout
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   TimeoutMs is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlCan_Hal_WaitRecvNotEmptyOrTimeout(AL_CAN_HalStruct *Handle, AL_CAN_EventStruct *Event, AL_U32 TimeoutMs)
{
    return AlOsal_Mb_Receive(&(Handle->RxEventQueue), Event, TimeoutMs);
}

/**
 * This function is intr handler call back function
 * @param   Event is pointer to AL_CAN_EventStruct
 * @param   CallBackRef
 * @return
 * @note
*/
static AL_VOID AlCan_Hal_DefEventCallBack(AL_CAN_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_CAN_HalStruct *Handle = (AL_CAN_HalStruct *)CallBackRef;

    switch (Event->EventId)
    {
    case AL_CAN_EVENT_SEND_DONE:
        AlOsal_Mb_Send(&(Handle->TxEventQueue), Event);
        break;
    case AL_CAN_EVENT_RECV_DONE:
        AlOsal_Mb_Send(&(Handle->RxEventQueue), Event);
        break;
    case AL_CAN_EVENT_RBUFF_ALMOST_FULL:
    case AL_CAN_EVENT_RBUFF_FULL:
    case AL_CAN_EVENT_RBUFF_OVERFLOW:
        /* User need to generate a Sem call for recv */
        break;
    case AL_CAN_EVENT_ABORT:
    case AL_CAN_EVENT_ERR:
    case AL_CAN_EVENT_BUS_ERR:
    case AL_CAN_EVENT_ARBITRATION_LOST:
    case AL_CAN_EVENT_ERR_PASSIVE:
        AlOsal_Mb_Send(&(Handle->TxEventQueue), Event);
        break;
    default:
        break;
    }
}

static inline AL_S32 AlCan_Hal_HandleInit(AL_CAN_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    Ret = AlOsal_Lock_Init(&(Handle->TxLock), "Can-TxLock");
    AL_ASSERT(Ret == AL_OK, Ret);

    Ret = AlOsal_Lock_Init(&(Handle->RxLock), "Can-RxLock");
    AL_ASSERT(Ret == AL_OK, Ret);

    AlOsal_Mb_Init(&(Handle->TxEventQueue), "Can-TxDone");
    AL_ASSERT(Ret == AL_OK, Ret);

    AlOsal_Mb_Init(&(Handle->RxEventQueue), "Can-RxNoEmpty");
    AL_ASSERT(Ret == AL_OK, Ret);

    return Ret;
}

/**
 * This function init CAN module
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   InitConfig is module config structure with AL_CAN_InitStruct
 * @param   CallBack is call back struct with AL_CAN_EventCallBack
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_Init(AL_CAN_HalStruct **Handle, AL_U32 DevId, AL_CAN_InitStruct *InitConfig,
                      AL_CAN_EventCallBack CallBack)
{
    AL_S32 Ret = AL_OK;
    AL_CAN_DevStruct *Dev = AL_NULL;
    AL_CAN_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_CAN_ERR_NULL_PTR);

    /* 1. look up hardware config */
    HwConfig = AlCan_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        *Handle = &AL_CAN_HalInstance[DevId];
        Dev = &((*Handle)->Dev);
    } else {
        return AL_CAN_ERR_NULL_PTR;
    }

    /* 2. Init IP */
    Ret = AlCan_Dev_Init(Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* 3. register callback */
    if (CallBack == AL_NULL) {
        Ret = AlCan_Dev_RegisterEventCallBack(Dev, AlCan_Hal_DefEventCallBack, *Handle);
    } else {
        Ret = AlCan_Dev_RegisterEventCallBack(Dev, CallBack, *Handle);
    }
    AL_ASSERT(Ret == AL_OK, Ret);

    /* 4. register intr */
    if (Dev->Config.RunMode & (AL_CAN_RUN_INTR_DMA | AL_CAN_RUN_INTR)) {
        AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlCan_Dev_IntrHandler, Dev);
    }

    Ret = AlCan_Hal_HandleInit(*Handle);

    return Ret;
}

/**
 * This function send frame
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Frame is tx buffer data with AL_CAN_FrameStruct
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_SendFrame(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL) && (Frame != AL_NULL), AL_CAN_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->TxLock), 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlCan_Dev_SendFrame(&(Handle->Dev), Frame);

    (AL_VOID)AlOsal_Lock_Release(&(Handle->TxLock));

    return Ret;
}

/**
 * This function recv frame
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Frame is tx buffer data with AL_CAN_FrameStruct
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_RecvFrame(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame)
{
    AL_S32 Ret = AL_OK;
    AL_CAN_EventStruct Event = {0};

    AL_ASSERT((Handle != AL_NULL) && (Frame != AL_NULL), AL_CAN_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->RxLock), 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlCan_Hal_WaitRecvNotEmptyOrTimeout(Handle, &Event, 0);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&(Handle->RxLock));
        return Ret;
    }

    Ret = AlCan_Dev_RecvFrame(&(Handle->Dev), Frame);

    /* Check recv buffer slots not empty, release RxQueue Sem */
    if (!AlCan_Dev_GetState(&(Handle->Dev), AL_CAN_STATE_RECV_EMPTY)) {
        /* Pre frame event covered by latest frame's */
        AlOsal_Mb_Send(&(Handle->RxEventQueue), &Event);
    }

    (AL_VOID)AlOsal_Lock_Release(&(Handle->RxLock));

    return Ret;
}

/**
 * This function send frame blocked
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Frame is tx buffer data with AL_CAN_FrameStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_SendFrameBlock(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_CAN_EventStruct Event = {0};

    AL_ASSERT((Handle != AL_NULL) && (Frame != AL_NULL), AL_CAN_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->TxLock), Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlCan_Dev_SendFrame(&(Handle->Dev), Frame);
    if (Ret != AL_OK) {
        if (Ret == AL_CAN_ERR_IN_STANDBY_MODE) {
            Event.EventId = AL_CAN_EVENT_SEND_DONE;
            AlOsal_Mb_Send(&(Handle->TxEventQueue), &Event);
        } else {
            (AL_VOID)AlOsal_Lock_Release(&(Handle->TxLock));
            return Ret;
        }
    }

    Ret = AlCan_Hal_WaitSendDoneOrTimeout(Handle, &Event, Timeout);

    (AL_VOID)AlOsal_Lock_Release(&(Handle->TxLock));

    if ((Ret == AL_OK) && (Event.EventId == AL_CAN_EVENT_SEND_DONE)) {
        return Ret;
    } else {
        return (Ret != AL_OK) ? Ret : AL_CAN_EVENTS_TO_ERRS(Event.EventId);
    }
}

/**
 * This function recv frame blocked
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Frame is tx buffer data with AL_CAN_FrameStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_RecvFrameBlock(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_CAN_EventStruct Event = {0};

    AL_ASSERT((Handle != AL_NULL) && (Frame != AL_NULL), AL_CAN_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->RxLock), Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlCan_Hal_WaitRecvNotEmptyOrTimeout(Handle, &Event, Timeout);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&(Handle->RxLock));
        Ret = AL_CAN_ERR_TIMEOUT;
    }

    Ret = AlCan_Dev_RecvFrame(&(Handle->Dev), Frame);

    if (!AlCan_Dev_GetState(&(Handle->Dev), AL_CAN_STATE_RECV_EMPTY)) {
        AlOsal_Mb_Send(&(Handle->RxEventQueue), &Event);
    }

    (AL_VOID)AlOsal_Lock_Release(&(Handle->RxLock));

    if (Ret == AL_OK && (Event.EventId == AL_CAN_EVENT_RECV_DONE)) {
        return Ret;
    } else {
        return (Ret != AL_OK) ? Ret : AL_CAN_EVENTS_TO_ERRS(Event.EventId);
    }
}

/**
 * This function recv frame blocked
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Frame is rx buffer data with AL_CAN_FrameStruct
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_RecvFrameDma(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL) && (Frame != AL_NULL), AL_CAN_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->RxLock), 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    /*TODO: dma recv operation*/

    (AL_VOID)AlOsal_Lock_Release(&(Handle->RxLock));

    return Ret;
}

/**
 * This function recv frame blocked
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Cmd is io ctl cmd to AL_CAN_IoCtlCmdEnum
 * @param   Data is pointer to cmd args
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_IoCtl(AL_CAN_HalStruct *Handle, AL_CAN_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL) && (Data != AL_NULL), AL_CAN_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->TxLock), 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Lock_Take(&(Handle->RxLock), 0);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&(Handle->TxLock));
        return Ret;
    }

    Ret = AlCan_Dev_IoCtl(&(Handle->Dev), Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can io ctl cmd error:%d\r\n", Ret);
    }

    (AL_VOID)AlOsal_Lock_Release(&(Handle->TxLock));
    (AL_VOID)AlOsal_Lock_Release(&(Handle->RxLock));

    return Ret;
}