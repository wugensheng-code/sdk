/***************************** Include Files *********************************/
#include "al_dmacahb_hal.h"
#include "al_dmacahb_dev.h"
#include "al_dmacahb_ll.h"
#include "al_errno.h"
/* TODO: Remove after irq driver done */
#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

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
    AL_DMACAHB_TransTypeEnum Type = Handle->Channel->Config.TransType;
    AL_DMACAHB_ChStateEnum State;

    extern AL_BOOL AlDmacAhb_Dev_GetState(AL_DMACAHB_ChStruct *Channel, AL_DMACAHB_ChStateEnum State);

    if (Type == AL_DMACAHB_TRANS_TYPE_1 || Type == AL_DMACAHB_TRANS_TYPE_5) {
        State = AL_DMACAHB_STATE_TRANS_BUSY;
    } else {
        State = AL_DMACAHB_STATE_BLOCK_TRANS_BUSY;
    }

    while(AlDmacAhb_Dev_GetState(Handle->Channel, State) && Timeout);

    if (Timeout == 0) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "DmacAhb wait send done time out!\r\n");
        return AL_DMACAHB_ERR_TIMEOUT;
    }

    return AL_OK;
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

    if (InitConfig->Id > HwConfig->ChannelNum) {
        AL_DMACAHB_HAL_UNLOCK(Handle);
        return AL_DMACAHB_ERR_INVALID_CH_ID;
    }

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
    /* TODO: replace intr handler reference function with al_irq.h api */
    if (Handle->Channel->Dmac->State.IntrEn == AL_FALSE) {
        Handle->Channel->Dmac->State.IntrEn = AL_TRUE;
        interrupt_table IntrCallBack;
        IntrCallBack.handler    = AlDmacAhb_Dev_IntrHandler;
        IntrCallBack.ref        = Handle->Channel->Dmac;
        ECLIC_Register_IRQ(HwConfig->IntrId, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1, &IntrCallBack);
        __enable_irq();
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
 * @param   SrcAddr is data source address
 * @param   DstAddr is data destination address
 * @param   TransSize is data size in source trans width
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlDmacAhb_Hal_Start(AL_DMACAHB_HalStruct *Handle, AL_REG SrcAddr, AL_REG DstAddr, AL_U32 TransSize)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    AL_DMACAHB_HAL_LOCK(Handle);

    Ret = AlDmacAhb_Dev_Start(Handle->Channel, SrcAddr, DstAddr, TransSize);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Dmacahb start error:%x\r\n", Ret);
    }

    AL_DMACAHB_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function start llp mode dma
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @param   Lli is pointer to AL_DMACAHB_LliStruct
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlDmacAhb_Hal_LlpModeStart(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_LliStruct *Lli)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    AL_DMACAHB_HAL_LOCK(Handle);

    AlDmacAhb_Dev_FillLliWithCtl(Handle->Channel, Lli);

    Ret = AlDmacAhb_Dev_LlpModeStart(Handle->Channel, Lli);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Dmacahb llp mode start error:%x\r\n", Ret);
    }

    AL_DMACAHB_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function start dma blocked
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @param   SrcAddr is data source address
 * @param   DstAddr is data destination address
 * @param   TransSize is data size in source trans width
 * @param   Timeout is max wait time for dma done
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlDmacAhb_Hal_StartBlock(AL_DMACAHB_HalStruct *Handle, AL_REG SrcAddr, AL_REG DstAddr, AL_U32 TransSize,
                                AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    AL_DMACAHB_HAL_LOCK(Handle);

    Ret = AlDmacAhb_Dev_Start(Handle->Channel, SrcAddr, DstAddr, TransSize);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Dmacahb start block error:0x%x\r\n", Ret);
        AL_DMACAHB_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlDmacAhb_Hal_WaitTransDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Dmacahb wait trans done error:%x\r\n", Ret);
    }

    AL_DMACAHB_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function start llp mode dma blocked
 * @param   Handle is pointer to AL_DMACAHB_HalStruct
 * @param   Lli is pointer to AL_DMACAHB_LliStruct
 * @param   Timeout is max wait time for dma done
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlDmacAhb_Hal_LlpModeStartBlock(AL_DMACAHB_HalStruct *Handle, AL_DMACAHB_LliStruct *Lli, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_DMACAHB_ERR_NULL_PTR;
    }

    AL_DMACAHB_HAL_LOCK(Handle);

    Ret = AlDmacAhb_Dev_LlpModeStart(Handle->Channel, Lli);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Dmacahb llp mode start error:%x\r\n", Ret);
    }

    Ret = AlDmacAhb_Hal_WaitTransDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Dmacahb llp mode wait trans done error:%x\r\n", Ret);
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


