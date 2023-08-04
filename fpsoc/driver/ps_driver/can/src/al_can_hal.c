/***************************** Include Files *********************************/
#include "al_can_hal.h"
#include "al_dmacahb_hal.h"
#include "al_osal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
/* Static CAN controller instance */
static AL_CAN_DevStruct AL_CAN_DevInstance[AL_CAN_NUM_INSTANCE];

/************************** Function Prototypes ******************************/
extern AL_BOOL AlCan_Dev_GetState(AL_CAN_DevStruct *Dev, AL_CAN_StateEnum State);

/************************** Function Definitions ******************************/
/**
 * This function wait for frame send done or timeout
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlCan_Hal_WaitSendDoneOrTimeout(AL_CAN_HalStruct *Handle, AL_U32 Timeout, AL_CAN_EventStruct *Event)
{
    return Al_OSAL_Mb_Recive(&Handle->TxEventQueue[Handle->CurTxMode], Event, Timeout);
}

/**
 * This function wait for frame recv done or timeout
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlCan_Hal_WaitRecvNotEmptyOrTimeout(AL_CAN_HalStruct *Handle, AL_U32 Timeout, AL_CAN_EventStruct *Event)
{
    return Al_OSAL_Mb_Recive(&Handle->RxEventQueue, Event, Timeout);
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
    case AL_CAN_EVENT_SEND_READY:
        Handle->CurTxMode = Handle->ReqTxMode;
        break;
    case AL_CAN_EVENT_SEND_DONE:
        Al_OSAL_Mb_Send(&Handle->TxEventQueue[Handle->CurTxMode], Event);
        break;
    case AL_CAN_EVENT_RECV_DONE:
        Al_OSAL_Mb_Send(&Handle->RxEventQueue, Event);
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
        /* ???????? */
        break;
    default:
        break;
    }
}

/**
 * This function init CAN module
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   InitConfig is module config structure with AL_CAN_InitStruct
 * @param   CallBack is call back struct with AL_CAN_CallBackStruct
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_Init(AL_CAN_HalStruct *Handle, AL_CAN_InitStruct *InitConfig, AL_CAN_CallBackStruct *CallBack,
                      AL_U32 DevId)
{
    AL_S32 Ret = AL_OK;
    AL_CAN_HwConfigStruct *HwConfig;
    AL_CAN_CallBackStruct EventCallBack;

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    /* 1. look up hardware config */
    HwConfig = AlCan_Dev_LookupConfig(DevId);
    if (HwConfig == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }
    Handle->Dev = &AL_CAN_DevInstance[DevId];

    /* 2. Init IP */
    Ret = AlCan_Dev_Init(Handle->Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* 3. register callback */
    if (CallBack == AL_NULL) {
        EventCallBack.Func  = AlCan_Hal_DefEventCallBack;
        EventCallBack.Ref   = Handle;
    } else {
        EventCallBack.Func  = CallBack->Func;
        EventCallBack.Ref   = CallBack->Ref;
    }

    Ret = AlCan_Dev_RegisterEventCallBack(Handle->Dev, &EventCallBack);
    if (Ret != AL_OK) {
        return Ret;
    }

    /* 4. register intr */
    /* replace intr handler reference function with al_intr.h api */
    if (Handle->Dev->Config.RunMode & (AL_CAN_RUN_INTR_DMA | AL_CAN_RUN_INTR)) {
        AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlCan_Dev_IntrHandler, Handle->Dev);
        /* Already call in AlIntr_RegHandler */
        // AlIntr_SetInterrupt(HwConfig->IntrId, AL_FUNC_ENABLE);
    }

    Ret = Al_OSAL_Lock_Init(&Handle->TxLock, "Can-TxLock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = Al_OSAL_Lock_Init(&Handle->RxLock, "Can-RxLock");
    if (Ret != AL_OK) {
        return Ret;
    }

    Al_OSAL_Mb_Init(&Handle->TxEventQueue[CAN_BLOCK], "Can-TxDone");
    Al_OSAL_Mb_Init(&Handle->RxEventQueue, "Can-RxNoEmpty");
    Al_OSAL_Mb_Init(&Handle->TxEventQueue[CAN_NONBLOCK], "Can-TxDone");

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

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    Ret = Al_OSAL_Lock_Take(&Handle->TxLock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Handle->ReqTxMode = CAN_NONBLOCK;

    Ret = AlCan_Dev_SendFrame(Handle->Dev, Frame);

    (AL_VOID)Al_OSAL_Lock_Release(&Handle->TxLock);

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

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    Ret = Al_OSAL_Lock_Take(&Handle->RxLock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlCan_Hal_WaitRecvNotEmptyOrTimeout(Handle, 0, &Event);
    if (Ret != AL_OK) {
        (AL_VOID)Al_OSAL_Lock_Release(&Handle->TxLock);
        return Ret;
    }

    Ret = AlCan_Dev_RecvFrame(Handle->Dev, Frame);

    /* Check recv buffer slots not empty, release RxQueue Sem */
    if (!AlCan_Dev_GetState(Handle->Dev, AL_CAN_STATE_RECV_EMPTY)) {
        /* Pre frame event covered by latest frame's */
        Al_OSAL_Mb_Send(&Handle->RxEventQueue, &Event);
    }

    (AL_VOID)Al_OSAL_Lock_Release(&Handle->RxLock);

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

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    Ret = Al_OSAL_Lock_Take(&Handle->TxLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Handle->ReqTxMode = CAN_BLOCK;

    Ret = AlCan_Dev_SendFrame(Handle->Dev, Frame);
    if (Ret != AL_OK) {
        (AL_VOID)Al_OSAL_Lock_Release(&Handle->TxLock);
        return Ret;
    }

    Ret = AlCan_Hal_WaitSendDoneOrTimeout(Handle, Timeout, &Event);

    (AL_VOID)Al_OSAL_Lock_Release(&Handle->TxLock);

    if (Ret == AL_OK && (Event.EventId == AL_CAN_EVENT_SEND_DONE)) {
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

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    Ret = Al_OSAL_Lock_Take(&Handle->RxLock, Timeout);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlCan_Hal_WaitRecvNotEmptyOrTimeout(Handle, Timeout, &Event);
    if (Ret != AL_OK) {
        (AL_VOID)Al_OSAL_Lock_Release(&Handle->TxLock);
        Ret = AL_CAN_ERR_TIMEOUT;
    }

    Ret = AlCan_Dev_RecvFrame(Handle->Dev, Frame);

    if (!AlCan_Dev_GetState(Handle->Dev, AL_CAN_STATE_RECV_EMPTY)) {
        Al_OSAL_Mb_Send(&Handle->RxEventQueue, &Event);
    }

    (AL_VOID)Al_OSAL_Lock_Release(&Handle->RxLock);

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

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    Ret = Al_OSAL_Lock_Take(&Handle->RxLock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    /*TODO: dma recv operation*/

    (AL_VOID)Al_OSAL_Lock_Release(&Handle->RxLock);

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

    if (Handle == AL_NULL || Data == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    Ret = Al_OSAL_Lock_Take(&Handle->TxLock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = Al_OSAL_Lock_Take(&Handle->RxLock, 0);
    if (Ret != AL_OK) {
        return Ret;
    }

    Ret = AlCan_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can io ctl cmd error:%d\r\n", Ret);
    }

    (AL_VOID)Al_OSAL_Lock_Release(&Handle->TxLock);
    (AL_VOID)Al_OSAL_Lock_Release(&Handle->RxLock);

    return Ret;
}