/***************************** Include Files *********************************/
#include "al_can_hal.h"
#include "al_dmacahb_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
/* Static CAN controller instance */
static AL_CAN_DevStruct AL_CAN_DevInstance[AL_CAN_NUM_INSTANCE];

/************************** Function Prototypes ******************************/
extern AL_BOOL AlCan_Dev_GetState(AL_CAN_DevStruct *Dev, AL_CAN_StateEnum State);

/************************** Function Definitions ******************************/

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
    while(AlCan_Dev_GetState(Handle->Dev, AL_CAN_STATE_SEND_BUSY) && Timeout);

    if (Timeout == 0) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can wait send done time out!\r\n");
        return AL_CAN_ERR_TIMEOUT;
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
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Can wait recv done time out!\r\n");
        return AL_CAN_ERR_TIMEOUT;
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

    AL_CAN_HAL_LOCK(Handle);

    /* 1. look up hardware config */
    HwConfig = AlCan_Dev_LookupConfig(DevId);
    if (HwConfig == AL_NULL) {
        AL_CAN_HAL_UNLOCK(Handle);
        return AL_CAN_ERR_NULL_PTR;
    }
    Handle->Dev = &AL_CAN_DevInstance[DevId];

    /* 2. Init IP */
    Ret = AlCan_Dev_Init(Handle->Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        /* TODO: Err return */
        AL_CAN_HAL_UNLOCK(Handle);
        return Ret;
    }

    /* 3. register callback */
    if (CallBack == AL_NULL) {
        EventCallBack.Func  = AlCan_Hal_DefEventCallBack;
        EventCallBack.Ref   = Handle->Dev;
    } else {
        EventCallBack.Func  = CallBack->Func;
        EventCallBack.Ref   = CallBack->Ref;
    }

    Ret = AlCan_Dev_RegisterEventCallBack(Handle->Dev, &EventCallBack);
    if (Ret != AL_OK) {
        /* TODO: Err return */
        AL_CAN_HAL_UNLOCK(Handle);
        return Ret;
    }

    /* 4. register intr */
    /* replace intr handler reference function with al_intr.h api */
    if (Handle->Dev->Config.RunMode & (AL_CAN_RUN_INTR_DMA | AL_CAN_RUN_INTR)) {
        AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlCan_Dev_IntrHandler, Handle->Dev);
        AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);
        /* Already call in AlIntr_RegHandler */
        // AlIntr_SetInterrupt(HwConfig->IntrId, AL_FUNC_ENABLE);
    }

    AL_CAN_HAL_UNLOCK(Handle);

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
 * @param   Handle is pointer to AL_CAN_HalStruct
 * @param   Frame is tx buffer data with AL_CAN_FrameStruct
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlCan_Hal_RecvFrame(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
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

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    AL_CAN_HAL_LOCK(Handle);

    Ret = AlCan_Dev_SendFrame(Handle->Dev, Frame);
    if (Ret != AL_OK) {
        AL_CAN_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlCan_Hal_WaitSendDoneOrTimeout(Handle, Timeout);

    AL_CAN_HAL_UNLOCK(Handle);

    return Ret;
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

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    AL_CAN_HAL_LOCK(Handle);

    Ret = AlCan_Hal_WaitRecvNotEmptyOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_CAN_ERR_TIMEOUT;
    }

    Ret = AlCan_Dev_RecvFrame(Handle->Dev, Frame);

    AL_CAN_HAL_UNLOCK(Handle);

    return Ret;
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

    AL_CAN_HAL_LOCK(Handle);

    /*TODO: dma recv operation*/

    AL_CAN_HAL_UNLOCK(Handle);

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

    if (Handle == AL_NULL) {
        return AL_CAN_ERR_NULL_PTR;
    }

    AL_CAN_HAL_LOCK(Handle);

    Ret = AlCan_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Can io ctl cmd error:%d\r\n", Ret);
    }

    AL_CAN_HAL_UNLOCK(Handle);

    return Ret;
}