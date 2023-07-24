/***************************** Include Files *********************************/
#include "al_dmacahb_hal.h"
#include "al_dmacahb_dev.h"
#include "al_dmacahb_ll.h"
#include "al_core.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_DMACAHB_ChStruct AL_DMACAHB_ChInstance[AL_DMACAHB_NUM_INSTANCE][AL_DMACAHB_CHANNEL_NUM];

/************************** Function Prototypes ******************************/
extern AL_BOOL AlDmacAhb_Dev_GetState(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_ChStateEnum State);

/************************** Function Definitions ******************************/
#ifdef USE_RTOS
static AL_S32 AlDmacAhb_Hal_WaitSendDoneOrTimeout(AL_DMACAHB_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

static AL_S32 AlDmacAhb_Hal_WaitRecvNotEmptyOrTimeout(AL_DMACAHB_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}


static AlDmacAhb_Hal_IntrHandler(AL_DMACAHB_HalStruct *Handle, AL_U32 Event, AL_U32 EventData)
{
    /*
     * TODO:send event
    */
    (void) Handle;
    (void) Event;
    (void) EventData;
}

#define AL_DMACAHB_HAL_LOCK(Handle)        do {} while (0)

#define AL_DMACAHB_HAL_UNLOCK(Handle)      do {} while (0)

#else

/**
 * This function wait for frame send done or timeout
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlDmacAhb_Hal_WaitTransDoneOrTimeout(AL_DMACAHB_HalStruct *Handle, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;
    extern AL_BOOL AlDmacAhb_Dev_GetState(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_ChStateEnum State);

    AL_DMACAHB_ChStateEnum State;

    Ret = AlDmacAhb_Dev_TransTypeToState(Handle->Channel->Config.TransType, &State);
    if (Ret != AL_OK) {
        return Ret;
    }

    while(AlDmacAhb_Dev_GetState(Handle->Channel, State) && Timeout);

    if (Timeout == 0) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "DmacAhb wait send done time out!\r\n");
        return AL_DMACAHB_ERR_TIMEOUT;
    }

    return Ret;
}

#define AL_DMACAHB_HAL_LOCK(Handle)          do {} while (0)

#define AL_DMACAHB_HAL_UNLOCK(Handle)        do {} while (0)

#endif

/**
 * This function is intr handler call back function
 * @param   Event is pointer to AL_DMACAHB_EventStruct
 * @param   CallBackRef
 * @return
 * @note
*/
static AL_VOID AlDmacAhb_Hal_DefChEventCallBack(AL_DMACAHB_EventStruct *Event, AL_VOID *CallBackRef)
{

}

/**
 * This function init CAN module
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @param   InitConfig is module config structure with AL_DMACAHB_ChInitStruct
 * @param   CallBack is call back struct with AL_DMACAHB_ChCallBackStruct
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlDmacAhb_Hal_Init(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_ChInitStruct *InitConfig,
                          AL_DMACAHB_ChCallBackStruct *CallBack, AL_U32 DevId)
{
    AL_S32 Ret = AL_OK;
    AL_DMACAHB_HwConfigStruct   *HwConfig;
    AL_DMACAHB_ChCallBackStruct EventCallBack;
    AL_DMACAHB_ChIdEnum AvailableId;

    if (Handle == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    AL_DMACAHB_HAL_LOCK(Handle);

    /* 1. look up hardware config */
    HwConfig = AlDmacAhb_Dev_LookupConfig(DevId);
    if (HwConfig == AL_NULL) {
        AL_DMACAHB_HAL_UNLOCK(Handle);
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    Ret = AlDmacAhb_Dev_RequestCh(HwConfig, InitConfig->Id, &AvailableId);
    if (Ret != AL_OK) {
        AL_DMACAHB_HAL_UNLOCK(Handle);
        return Ret;
    }

    InitConfig->Id = AvailableId;
    Handle->Channel = &AL_DMACAHB_ChInstance[DevId][InitConfig->Id];

    /* 2. Init IP */
    Ret = AlDmacAhb_Dev_Init(Handle->Channel, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        /* TODO: Err return */
        AL_DMACAHB_HAL_UNLOCK(Handle);
        return Ret;
    }

    /* 3. register callback */
    if (CallBack == AL_NULL) {
        EventCallBack.Func  = AlDmacAhb_Hal_DefChEventCallBack;
        EventCallBack.Ref   = Handle->Channel;
    } else {
        EventCallBack.Func  = CallBack->Func;
        EventCallBack.Ref   = CallBack->Ref;
    }

    AlDmacAhb_Dev_RegisterChEventCallBack(Handle->Channel, &EventCallBack);

    /* 4. register intr */
    /* TODO: replace intr handler reference function with al_intr.h api */
    if (Handle->Channel->Dmac->State.IntrEn == AL_FALSE) {
        Handle->Channel->Dmac->State.IntrEn = AL_TRUE;
        AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlDmacAhb_Dev_IntrHandler, Handle->Channel->Dmac);
    }

    AL_DMACAHB_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function init CAN module
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlDmacAhb_Hal_DeInit(AL_DMACAHB_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    if (Handle->Channel == AL_NULL) {
        return AL_DMACAHB_ERR_HANDLE_WITHOUT_CH;
    }

    AL_DMACAHB_HAL_LOCK(Handle);

    if (Handle->Channel->Dmac->State.IntrEn == AL_TRUE) {
        /* TODO: unregister intr */
    }

    Ret = AlDmacAhb_Dev_DeInit(Handle->Channel);
    if (Ret != AL_OK) {
        /* TODO: Err return */
        AL_DMACAHB_HAL_UNLOCK(Handle);
        return Ret;
    }

    Handle->Channel = AL_NULL;

    AL_DMACAHB_HAL_UNLOCK(Handle);

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

    if (Handle == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    AL_DMACAHB_HAL_LOCK(Handle);

    Ret = AlDmacAhb_Dev_Start(Handle->Channel);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Dmacahb start error:%x\r\n", Ret);
    }

    AL_DMACAHB_HAL_UNLOCK(Handle);

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

    if (Handle == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    AL_DMACAHB_HAL_LOCK(Handle);

    Ret = AlDmacAhb_Dev_Start(Handle->Channel);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Dmacahb start block error:0x%x\r\n", Ret);
        AL_DMACAHB_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlDmacAhb_Hal_WaitTransDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "Dmacahb wait trans done error:%x\r\n", Ret);
    }

    AL_DMACAHB_HAL_UNLOCK(Handle);

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

    if (Handle == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    AL_DMACAHB_HAL_LOCK(Handle);

    AlDmacAhb_Dev_IoCtl(Handle->Channel, Cmd, Data);

    AL_DMACAHB_HAL_UNLOCK(Handle);

    return Ret;
}


