/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_cipher_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
/* Static CIPHER controller instance */
static AL_CIPHER_HalStruct AL_CIPHER_HalInstance[AL_CIPHER_NUM_INSTANCE];

/************************** Function Prototypes ******************************/
extern AL_BOOL AlCipher_Dev_GetState(AL_CIPHER_DevStruct *Dev, AL_CIPHER_StateEnum State);

/************************** Function Definitions ******************************/

/**
 * This function wait for ack or timeout
 * @param   Handle is pointer to AL_CIPHER_HalStruct
 * @param   TimeoutMs is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlCipher_Hal_WaitAckOrTimeout(AL_CIPHER_HalStruct *Handle, AL_CIPHER_EventStruct *Event, AL_U32 TimeoutMs)
{
    AL_ASSERT(Handle != AL_NULL, AL_CIPHER_ERR_NULL_PTR);

    return AlOsal_Mb_Receive(&Handle->StartEventQueue, Event, TimeoutMs);
}

/**
 * This function is intr handler call back function
 * @param   Event is pointer to AL_CIPHER_EventStruct
 * @param   CallBackRef
 * @return
 * @note
*/
static AL_S32 AlCipher_Hal_DefEventCallBack(AL_CIPHER_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_ASSERT(Event != AL_NULL, AL_CIPHER_ERR_NULL_PTR);

    AL_CIPHER_HalStruct *Handle = (AL_CIPHER_HalStruct *)CallBackRef;

    switch (Event->EventId)
    {
    case AL_CIPHER_EVENT_DONE:
        AlOsal_Mb_Send(&Handle->StartEventQueue, Event);
        break;
    default:
        break;
    }

    return AL_OK;
}

static inline AL_S32 AlCipher_Hal_HandleInit(AL_CIPHER_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    Ret = AlOsal_Lock_Init(&Handle->StartLock, "Cipher-StartLock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlOsal_Mb_Init(&Handle->StartEventQueue, "Cipher-Done");
    if (Ret != AL_OK) {
        return Ret;
    }

    return Ret;
}

/**
 * This function init CIPHER module
 * @param   Handle is pointer to AL_CIPHER_HalStruct
 * @param   InitConfig is module config structure with AL_CIPHER_InitStruct
 * @param   CallBack is call back struct with AL_CIPHER_EventCallBack
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCipher_Hal_Init(AL_CIPHER_HalStruct **Handle, AL_U32 DevId, AL_CIPHER_EventCallBack CallBack)
{
    AL_S32 Ret = AL_OK;
    AL_CIPHER_DevStruct *Dev = AL_NULL;
    AL_CIPHER_HwConfigStruct *HwConfig = AL_NULL;

    AL_ASSERT(Handle != AL_NULL, AL_CIPHER_ERR_NULL_PTR);

    /* 1. look up hardware config */
    HwConfig = AlCipher_Dev_LookupConfig(DevId);
    if (HwConfig == AL_NULL) {
        return AL_CIPHER_ERR_NULL_PTR;
    }

    *Handle = &AL_CIPHER_HalInstance[DevId];
    Dev = &(*Handle)->Dev;

    /* 2. Init IP */
    Ret = AlCipher_Dev_Init(Dev, HwConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* 3. register callback */
    if (CallBack == AL_NULL) {
        Ret = AlCipher_Dev_RegisterEventCallBack(Dev, AlCipher_Hal_DefEventCallBack, *Handle);
    } else {
        Ret = AlCipher_Dev_RegisterEventCallBack(Dev, CallBack, *Handle);
    }
    AL_ASSERT(Ret == AL_OK, Ret);

    /* 4. register intr */
    AL_INTR_AttrStrct IntrAttr = {
        .Priority = 1,
        .TrigMode = POSTIVE_EDGE_TRIGGER,
    };
    AlIntr_RegHandler(HwConfig->AckIntrId, &IntrAttr, AlCipher_Dev_IntrHandler, Dev);

    Ret = AlCipher_Hal_HandleInit(*Handle);

    return Ret;
}

/**
 * This function start cipher operation
 * @param   Handle is pointer to AL_CIPHER_HalStruct
 * @param   Cmd is index to AL_CIPHER_CmdEnum
 * @param   Config is pointer to AL_CIPHER_ConfigUnion
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCipher_Hal_Start(AL_CIPHER_HalStruct *Handle, AL_CIPHER_CmdEnum Cmd, AL_CIPHER_ConfigUnion *Config)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Handle != AL_NULL) && (Config != AL_NULL), AL_CIPHER_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->StartLock), 0);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&(Handle->StartLock));
        return Ret;
    }

    Ret = AlCipher_Dev_Start(&(Handle->Dev), Cmd, Config);

    (AL_VOID)AlOsal_Lock_Release(&(Handle->StartLock));

    return Ret;
}

/**
 * This function start cipher operation blocked
 * @param   Handle is pointer to AL_CIPHER_HalStruct
 * @param   Cmd is index to AL_CIPHER_CmdEnum
 * @param   Config is pointer to AL_CIPHER_ConfigUnion
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCipher_Hal_StartBlock(AL_CIPHER_HalStruct *Handle, AL_CIPHER_CmdEnum Cmd, AL_CIPHER_ConfigUnion *Config,
                               AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    AL_CIPHER_EventStruct Event = {0};

    AL_ASSERT(Handle != AL_NULL, AL_CIPHER_ERR_NULL_PTR);

    Ret = AlOsal_Lock_Take(&(Handle->StartLock), Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlCipher_Dev_Start(&(Handle->Dev), Cmd, Config);
    if (Ret != AL_OK) {
        (AL_VOID)AlOsal_Lock_Release(&(Handle->StartLock));
        return Ret;
    }

    Ret = AlCipher_Hal_WaitAckOrTimeout(Handle, &Event, Timeout);

    (AL_VOID)AlOsal_Lock_Release(&(Handle->StartLock));

    return (Ret != AL_OK) ? Ret : Event.EventData;
}