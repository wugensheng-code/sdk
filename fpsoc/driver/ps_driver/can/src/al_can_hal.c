#include "al_can_hal.h"
#include "al_can_dev.h"
#include "al_can_ll.h"
#include "al_errno.h"
#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

static AL_CAN_DevStruct AL_CAN_DevInstance[AL_CAN_NUM_INSTANCE];

extern AL_BOOL AlCan_Dev_GetState(AL_CAN_DevStruct *Dev, AL_CAN_StateEnum State);

#ifdef USE_RTOS
static AL_S32 AlCan_Hal_WaitSendDoneOrTimeout(AL_CAN_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

static AL_S32 AlCan_Hal_WaitRecvNotEmptyOrTimeout(AL_CAN_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}


static AlCan_Hal_IntrHandler(AL_CAN_HalStruct *Handle, AL_U32 Event, AL_U32 EventData)
{
    /*
     * TODO:send event
    */
    (void) Handle;
    (void) Event;
    (void) EventData;
}

#define AL_CAN_HAL_LOCK(Handle)        do {} while (0)

#define AL_CAN_HAL_UNLOCK(Handle)      do {} while (0)

#else

/**
 * This function wait for frame send done or timeout
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlCan_Hal_WaitSendDoneOrTimeout(AL_CAN_HalStruct *Handle, AL_U32 Timeout)
{
    while(AlCan_Dev_GetState(Handle->Dev, AL_CAN_STATE_SEND_BUSY) & Timeout);

    if (Timeout == 0) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can wait send done time out!\r\n");
        return AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT);
    }

    return AL_OK;
}

/**
 * This function wait for frame recv done or timeout
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlCan_Hal_WaitRecvNotEmptyOrTimeout(AL_CAN_HalStruct *Handle, AL_U32 Timeout)
{
    while(AlCan_Dev_GetState(Handle->Dev, AL_CAN_STATE_RECV_EMPTY) & Timeout);

    if (Timeout == 0) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Can wait recv done time out!\r\n");
        return AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT);
    }

    return AL_OK;
}


#define AL_CAN_HAL_LOCK(Handle)          do {} while (0)

#define AL_CAN_HAL_UNLOCK(Handle)        do {} while (0)

#endif

/**
 * This function is intr handler call back function
 * @param   Event is pointer to AL_CAN_EventStruct
 * @param   CallBackRef
 * @return
 * @note
*/
static AL_VOID AlCan_Hal_DefEventCallBack(AL_CAN_EventStruct *Event, AL_VOID *CallBackRef)
{
    AL_CAN_DevStruct *Dev = CallBackRef;
    switch (Event->EventId)
    {
    case AL_CAN_EVENT_SEND_DONE:
        break;
    case AL_CAN_EVENT_RECV_DONE:
#ifdef USE_RTOS
        /* TODO: Set event touch a block section */
#else
        AL_CAN_FrameStruct *Frame = (AL_CAN_FrameStruct *)&(Dev->RecvBuffer);
        AL_LOG(AL_ERR_LEVEL_DEBUG, "can recv done!\r\n");
#ifdef CAN_DEBUG
        AlCan_Dev_DisplayFrame(Frame);
#endif
#endif
        break;
    case AL_CAN_EVENT_RBUFF_ALMOST_FULL:
        AL_LOG(AL_ERR_LEVEL_DEBUG, "can rbuffer almost full!\r\n");
        break;
    case AL_CAN_EVENT_RBUFF_OVERFLOW:
        AL_LOG(AL_ERR_LEVEL_DEBUG, "can rbuffer overflow!\r\n");
        break;
    case AL_CAN_EVENT_ABORT:
        AL_LOG(AL_ERR_LEVEL_DEBUG, "can abort!\r\n");
        break;
    case AL_CAN_EVENT_ERR:
        AL_LOG(AL_ERR_LEVEL_DEBUG, "can error!\r\n");
        break;
    case AL_CAN_EVENT_BUS_ERR:
        AL_LOG(AL_ERR_LEVEL_DEBUG, "can bus error!\r\n");

        break;
    case AL_CAN_EVENT_ARBITRATION_LOST:
        AL_LOG(AL_ERR_LEVEL_DEBUG, "can arbitration lost!\r\n");
        break;
    case AL_CAN_EVENT_ERR_PASSIVE:
        AL_LOG(AL_ERR_LEVEL_DEBUG, "can error passive!\r\n");
        break;
    default:
        break;
    }
}

/**
 * This function init CAN module
 * @param   AL_CAN_HalStruct is pointer to AL_CAN_HalStruct
 * @param   InitConfig is module config structure with AL_CAN_InitStruct
 * @param   CallBack is call back struct with AL_CAN_CallBackStruct
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_Init(AL_CAN_HalStruct *Handle, AL_CAN_InitStruct *InitConfig, \
    AL_CAN_CallBackStruct *CallBack, AL_U32 DevId)
{
    AL_S32 Ret = AL_OK;
    AL_CAN_HwConfigStruct *HwConfig;
    AL_CAN_CallBackStruct EventCallBack;

    if (Handle == AL_NULL) {
        return AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AL_CAN_HAL_LOCK(Handle);

    /* 1. look up hardware config */
    HwConfig = AlCan_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        Handle->Dev = &AL_CAN_DevInstance[DevId-1];
    }

    /* 2. Init IP */
    Ret = AlCan_Dev_Init(Handle->Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        /* TODO: Err return */
    }


    /* 3. register callback */
    if (CallBack == AL_NULL) {
        EventCallBack.Func  = AlCan_Hal_DefEventCallBack;
        EventCallBack.Ref   = Handle->Dev;
    } else {
        EventCallBack.Func  = CallBack->Func;
        EventCallBack.Ref   = CallBack->Ref;
    }

    AlCan_Dev_RegisterEventCallBack(Handle->Dev, &EventCallBack);

    /* 4. register intr */
    /* TODO: replace intr handler reference function with al_irq.h api */
    if (Handle->Dev->Config.RunMode & (AL_CAN_RUN_INTR_DMA | AL_CAN_RUN_INTR)) {
        interrupt_table callback;
        callback.handler    = AlCan_Dev_IntrHandler;
        callback.ref        = Handle->Dev;
        ECLIC_Register_IRQ(SOC_INT92_IRQn , ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,&callback);
        __enable_irq();
    }

    AL_CAN_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function send frame
 * @param   AL_CAN_HalStruct is pointer to AL_CAN_HalStruct
 * @param   Frame is tx buffer data with AL_CAN_FrameStruct
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_SendFrame(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AL_CAN_HAL_LOCK(Handle);

    Ret = AlCan_Dev_SendFrame(Handle->Dev, Frame);
    if (Ret != AL_OK) {
        AL_CAN_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_CAN_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function recv frame
 * @param   AL_CAN_HalStruct is pointer to AL_CAN_HalStruct
 * @param   Frame is tx buffer data with AL_CAN_FrameStruct
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_RecvFrame(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AL_CAN_HAL_LOCK(Handle);

    Ret = AlCan_Dev_RecvFrame(Handle->Dev, Frame);
    if (Ret != AL_OK) {
        AL_CAN_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_CAN_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function send frame blocked
 * @param   AL_CAN_HalStruct is pointer to AL_CAN_HalStruct
 * @param   Frame is tx buffer data with AL_CAN_FrameStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_SendFrameBlock(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AL_CAN_HAL_LOCK(Handle);

    Ret = AlCan_Dev_SendFrame(Handle->Dev, Frame);
    if (Ret != AL_OK) {
        AL_CAN_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlCan_Hal_WaitSendDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT);
    }

    AL_CAN_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function recv frame blocked
 * @param   AL_CAN_HalStruct is pointer to AL_CAN_HalStruct
 * @param   Frame is tx buffer data with AL_CAN_FrameStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_RecvFrameBlock(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AL_CAN_HAL_LOCK(Handle);

    Ret = AlCan_Hal_WaitRecvNotEmptyOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT);
    }

    Ret = AlCan_Dev_RecvFrame(Handle->Dev, Frame);
    if (Ret != AL_OK) {
        AL_CAN_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_CAN_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlCan_Hal_RecvFrameDma(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DEF_ERR(AL_CAN, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    AL_CAN_HAL_LOCK(Handle);

    /*TODO: dma recv operation*/

    AL_CAN_HAL_UNLOCK(Handle);

    return Ret;
}
