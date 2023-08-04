/***************************** Include Files *********************************/
#include "al_cipher_hal.h"
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
/* Static CIPHER controller instance */
static AL_CIPHER_DevStruct AL_CIPHER_DevInstance[AL_CIPHER_NUM_INSTANCE];

/************************** Function Prototypes ******************************/
extern AL_BOOL AlCipher_Dev_GetState(AL_CIPHER_DevStruct *Dev, AL_CIPHER_StateEnum State);

/************************** Function Definitions ******************************/

/**
 * This function wait for ack or timeout
 * @param   Handle is pointer to AL_CIPHER_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlCipher_Hal_WaitAckOrTimeout(AL_CIPHER_HalStruct *Handle, AL_U32 Timeout, AL_CIPHER_EventStruct *Event)
{
    return Al_OSAL_Mb_Recive(&Handle->StartEventQueue[Handle->CurStartMode], Event, Timeout);
}

/**
 * This function is intr handler call back function
 * @param   Event is pointer to AL_CIPHER_EventStruct
 * @param   CallBackRef
 * @return
 * @note
*/
static AL_VOID AlCipher_Hal_DefEventCallBack(AL_CIPHER_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_CIPHER_HalStruct *Handle = (AL_CIPHER_HalStruct *)CallBackRef;

    switch (Event->EventId)
    {
    case AL_CIPHER_EVENT_READY:
        Handle->CurStartMode = Handle->ReqStartMode;
        break;
    case AL_CIPHER_EVENT_DONE:
        Al_OSAL_Mb_Send(&Handle->StartEventQueue[Handle->CurStartMode], Event);
        break;
    default:
        break;
    }
}

/**
 * This function init CIPHER module
 * @param   Handle is pointer to AL_CIPHER_HalStruct
 * @param   InitConfig is module config structure with AL_CIPHER_InitStruct
 * @param   CallBack is call back struct with AL_CIPHER_CallBackStruct
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCipher_Hal_Init(AL_CIPHER_HalStruct *Handle, AL_CIPHER_CallBackStruct *CallBack, AL_U32 DevId)
{
    AL_S32 Ret = AL_OK;
    AL_CIPHER_HwConfigStruct *HwConfig;
    AL_CIPHER_CallBackStruct EventCallBack;

    if (Handle == AL_NULL) {
        return AL_CIPHER_ERR_NULL_PTR;
    }

    /* 1. look up hardware config */
    HwConfig = AlCipher_Dev_LookupConfig(DevId);
    if (HwConfig == AL_NULL) {
        return AL_CIPHER_ERR_NULL_PTR;
    }
    Handle->Dev = &AL_CIPHER_DevInstance[DevId];

    /* 2. Init IP */
    Ret = AlCipher_Dev_Init(Handle->Dev, HwConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* 3. register callback */
    if (CallBack == AL_NULL) {
        EventCallBack.Func  = AlCipher_Hal_DefEventCallBack;
        EventCallBack.Ref   = Handle->Dev;
    } else {
        EventCallBack.Func  = CallBack->Func;
        EventCallBack.Ref   = CallBack->Ref;
    }

    Ret = AlCipher_Dev_RegisterEventCallBack(Handle->Dev, &EventCallBack);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* 4. register intr */
    AL_INTR_AttrStrct IntrAttr = {
        .Priority = 1,
        .TrigMode = POSTIVE_EDGE_TRIGGER,
    };
    AlIntr_RegHandler(HwConfig->AckIntrId, &IntrAttr, AlCipher_Dev_IntrHandler, Handle->Dev);

    Ret = Al_OSAL_Lock_Init(&Handle->StartLock, "Cipher-StartLock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = Al_OSAL_Lock_Init(&Handle->AckLock, "Cipher-AckLock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Al_OSAL_Mb_Init(&Handle->StartEventQueue[CIPHER_BLOCK], "Cipher-Done");
    Al_OSAL_Mb_Init(&Handle->StartEventQueue[CIPHER_NONBLOCK], "Cipher-Done");

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

    if (Handle == AL_NULL) {
        return AL_CIPHER_ERR_NULL_PTR;
    }

    Ret = Al_OSAL_Lock_Take(&Handle->StartLock, 0);
    if (Ret != AL_OK) {
        (AL_VOID)Al_OSAL_Lock_Release(&Handle->StartLock);
        return Ret;
    }

    Handle->ReqStartMode = CIPHER_NONBLOCK;

    Ret = AlCipher_Dev_Start(Handle->Dev, Cmd, Config);

    (AL_VOID)Al_OSAL_Lock_Release(&Handle->StartLock);

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

    if (Handle == AL_NULL) {
        return AL_CIPHER_ERR_NULL_PTR;
    }

    Ret = Al_OSAL_Lock_Take(&Handle->StartLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Handle->ReqStartMode = CIPHER_BLOCK;

    Ret = AlCipher_Dev_Start(Handle->Dev, Cmd, Config);
    if (Ret != AL_OK) {
        (AL_VOID)Al_OSAL_Lock_Release(&Handle->StartLock);
        return Ret;
    }

    Ret = AlCipher_Hal_WaitAckOrTimeout(Handle, Timeout, &Event);

    (AL_VOID)Al_OSAL_Lock_Release(&Handle->StartLock);

    return (Ret != AL_OK) ? Ret : AL_CIPHER_EVENTS_TO_ERRS(Event.EventId);
}