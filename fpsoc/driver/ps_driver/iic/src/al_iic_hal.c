/***************************** Include Files *********************************/
#include "al_iic_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
#ifdef USE_RTOS
static AL_S32 AlIic_Hal_WaitMasterTxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

static AL_S32 AlIic_Hal_WaitMasterRxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

static AL_S32 AlIic_Hal_WaitSlaveTxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

static AL_S32 AlIic_Hal_WaitSlaveRxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

#define AL_UART_HAL_LOCK(Handle)        do {} while (0)
#define AL_UART_HAL_UNLOCK(Handle)      do {} while (0)

#else

extern AL_BOOL AlIic_Dev_IsTxBusy(AL_IIC_DevStruct *Iic);
extern AL_BOOL AlIic_Dev_IsRxBusy(AL_IIC_DevStruct *Iic);

static AL_S32 AlIic_Hal_WaitMasterTxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_U32 Timeout)
{
    while(AlIic_Dev_IsTxBusy(Handle->Dev));

    if (Handle->Dev->CmdOption == AL_IIC_CMD_OPTION_STOP) {
        while(AlIic_ll_IsMasterActivity(Handle->Dev->HwConfig.BaseAddress));
    }

    return AL_OK;
}

static AL_S32 AlIic_Hal_WaitMasterRxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_U32 Timeout)
{
    while(AlIic_Dev_IsRxBusy(Handle->Dev));

    if (Handle->Dev->CmdOption == AL_IIC_CMD_OPTION_STOP) {
        while(AlIic_ll_IsMasterActivity(Handle->Dev->HwConfig.BaseAddress));
    }

    return AL_OK;
}

static AL_S32 AlIic_Hal_WaitSlaveTxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_U32 Timeout)
{
    while(AlIic_Dev_IsTxBusy(Handle->Dev));
    while(AlIic_ll_IsSlaveActivity(Handle->Dev->HwConfig.BaseAddress));

    return AL_OK;
}

static AL_S32 AlIic_Hal_WaitSlaveRxDoneOrTimeout(AL_IIC_HalStruct *Handle, AL_U32 Timeout)
{
    while(AlIic_Dev_IsRxBusy(Handle->Dev));
    while(AlIic_ll_IsSlaveActivity(Handle->Dev->HwConfig.BaseAddress));

    return AL_OK;
}

#define AL_IIC_HAL_LOCK(Handle)          do {} while (0)
#define AL_IIC_HAL_UNLOCK(Handle)        do {} while (0)

#endif /* USE_RTOS */

AL_IIC_DevStruct AL_IIC_DevInstance[AL_IIC_NUM_INSTANCE];

static AL_VOID AlIic_DefEventCallBack(AL_IIC_EventStruct *IicEvent, void *CallbackRef)
{
    switch (IicEvent->Event) {
        case AL_IIC_EVENT_RX_UNDER:
        break;

        case AL_IIC_EVENT_RX_OVER:
        break;

        case AL_IIC_EVENT_TX_OVER:
        break;

        case AL_IIC_EVENT_RD_REQ:
        break;

        case AL_IIC_EVENT_TX_ABRT:
        printf("AL_IIC_EVENT_TX_ABRT source:0x%x\r\n", IicEvent->EventData);
        break;

        case AL_IIC_EVENT_RX_DONE:
        break;

        case AL_IIC_EVENT_ACTIVITY:
        break;

        case AL_IIC_EVENT_STOP_DET:
        break;

        case AL_IIC_EVENT_START_DET:
        break;

        case AL_IIC_EVENT_GEN_CALL:
        break;
    }
}

AL_S32 AlIic_Hal_Init(AL_IIC_HalStruct *Handle, AL_U32 DevId,
                      AL_IIC_InitStruct *InitConfig, AL_IIC_EventCallBack Callback)
{
    AL_S32 Ret = AL_OK;
    AL_IIC_HwConfigStruct *HwConfig = AL_NULL;

    if (Handle == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_LOCK(Handle);

    HwConfig = AlIic_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        Handle->Dev = &AL_IIC_DevInstance[DevId];
    } else {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    Ret = AlIic_Dev_Init(Handle->Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        AL_IIC_HAL_UNLOCK(Handle);
        return Ret;
    }

    if(AL_NULL == Callback) {
        Ret = AlIic_Dev_RegisterEventCallBack(Handle->Dev, AlIic_DefEventCallBack, (void *)Handle);
    } else {
        Ret = AlIic_Dev_RegisterEventCallBack(Handle->Dev, Callback, (void *)Handle);
    }
    if (Ret != AL_OK) {
        AL_IIC_HAL_UNLOCK(Handle);
        return Ret;
    }

    (AL_VOID)AlIntr_RegHandler(Handle->Dev->HwConfig.IntrId, AL_NULL, AlIic_Dev_IntrHandler, Handle->Dev);

    AL_IIC_HAL_UNLOCK(Handle);

    return AL_OK;
}

AL_S32 AlIic_Hal_MasterSendDataBlock(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_LOCK(Handle);

    Ret = AlIic_Dev_MasterSendData(Handle->Dev, SlaveAddr, Data, Size);
    if (Ret != AL_OK) {
        AL_IIC_HAL_UNLOCK(Handle);
        return Ret;
    }

    /*
     * wait until data send done
     */
    Ret = AlIic_Hal_WaitMasterTxDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlIic_Hal_MasterRecvDataBlock(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    /* check only Handle, more checks in AlIic_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_LOCK(Handle);

    Ret = AlIic_Dev_MasterRecvData(Handle->Dev, SlaveAddr, Data, Size);
    if (Ret != AL_OK) {
        AL_IIC_HAL_UNLOCK(Handle);
        return Ret;
    }

    /* wait until data receive done */
    Ret = AlIic_Hal_WaitMasterRxDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlIic_Hal_SlaveSendDataBlock(AL_IIC_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    /*
     * check only Handle, more checks in AlIic_Dev_Init function;
    */
    if (Handle == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_LOCK(Handle);

    Ret = AlIic_Dev_SlaveSendData(Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        AL_IIC_HAL_UNLOCK(Handle);
        return Ret;
    }

    /*
     * wait until data send done
    */
    Ret = AlIic_Hal_WaitSlaveTxDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlIic_Hal_SlaveRecvDataBlock(AL_IIC_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    /* check only Handle, more checks in AlIic_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_LOCK(Handle);

    Ret = AlIic_Dev_SlaveRecvData(Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        AL_IIC_HAL_UNLOCK(Handle);
        return Ret;
    }

    /* wait until data receive done */
    Ret = AlIic_Hal_WaitSlaveRxDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlIic_Hal_MasterSetCmdOption(AL_IIC_HalStruct *Handle, AL_IIC_CmdOptionEnum CmdOption)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_LOCK(Handle);

    Ret = AlIic_Dev_MasterSetCmdOption(Handle->Dev, CmdOption);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Dev_MasetSetCmdOption error:%d\r\n", Ret);
    }

    AL_IIC_HAL_UNLOCK(Handle);

    return Ret;
}

AL_IIC_CmdOptionEnum AlIic_Hal_MastertGetCmdOption(AL_IIC_HalStruct *Handle)
{
    AL_IIC_CmdOptionEnum CmdOption;

    if (Handle == AL_NULL) {
        return AL_IIC_CMD_OPTION_NONE;
    }

    AL_IIC_HAL_LOCK(Handle);

    CmdOption = AlIic_Dev_MasterGetCmdOption(Handle->Dev);

    AL_IIC_HAL_UNLOCK(Handle);

    return CmdOption;
}

AL_S32 AlIic_Hal_IoCtl(AL_IIC_HalStruct *Handle, AL_IIC_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_IIC_ERR_ILLEGAL_PARAM;
    }

    AL_IIC_HAL_LOCK(Handle);

    Ret = AlIic_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlIic_Dev_IoCtl error:%d\r\n", Ret);
    }

    AL_IIC_HAL_UNLOCK(Handle);

    return Ret;
}
