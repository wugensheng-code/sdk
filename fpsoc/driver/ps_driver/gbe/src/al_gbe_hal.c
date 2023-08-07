#include "al_gbe_hal.h"

#define AL_GBE_HAL_LOCK(Handle)          do {} while (0)
#define AL_GBE_HAL_UNLOCK(Handle)        do {} while (0)

AL_GBE_DevStruct AL_GBE_DevInstance[AL_GBE_NUM_INSTANCE];

static AL_VOID AlGbe_DefEventCallBack(AL_GBE_EventStruct *GbeEvent, void *CallbackRef)
{

}

static AL_S32 AlGbe_Hal_WaitTxDoneOrTimeout(AL_GBE_HalStruct *Handle, AL_U32 Timeout)
{

    return AL_OK;
}

static AL_S32 AlGbe_Hal_WaitRxDoneOrTimeout(AL_GBE_HalStruct *Handle, AL_U32 Timeout)
{

    return AL_OK;
}

AL_S32 AlGbe_Hal_Init(AL_GBE_HalStruct *Handle, AL_U32 DevId, AL_GBE_InitStruct *InitConfig,
                      AL_GBE_MacDmaConfigStruct *MacDmaConfig, AL_GBE_EventCallBack Callback)
{
    AL_S32 Ret = AL_OK;
    AL_GBE_HwConfigStruct *HwConfig = AL_NULL;

    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    HwConfig = AlGbe_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        Handle->Dev = &AL_GBE_DevInstance[DevId];
    } else {
        return AL_GBE_ERR_INVALID_DEVICE_ID;
    }

    Ret = AlGbe_Dev_Init(Handle->Dev, HwConfig, InitConfig, MacDmaConfig);
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    if(AL_NULL == Callback) {
        Ret = AlGbe_Dev_RegisterEventCallBack(Handle->Dev, AlGbe_DefEventCallBack, (void *)Handle);
    } else {
        Ret = AlGbe_Dev_RegisterEventCallBack(Handle->Dev, Callback, (void *)Handle);
    }
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    AlIntr_RegHandler(Handle->Dev->HwConfig.IntrNum, AL_NULL, AlGbe_Dev_IntrHandler, Handle->Dev);

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

AL_S32 AlGbe_Hal_ConfigRxDescBuffer(AL_GBE_HalStruct *Handle, AL_U32 DescIndex, AL_U8 *Buffer1Addr, AL_U8 *Buffer2Addr)
{
    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    AlGbe_Dev_ConfigRxDescBuffer(Handle->Dev, DescIndex, Buffer1Addr, Buffer2Addr);

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

AL_S32 AlGbe_Hal_PhyInit(AL_GBE_HalStruct *Handle, AL_U8 PhyAddr)
{
    AL_S32 Ret;

    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_PhyInit(Handle->Dev, PhyAddr);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlGbe_Hal_ConfigDuplexAndSpeed(AL_GBE_HalStruct *Handle)
{
    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    AlGbe_Dev_ConfigDuplexAndSpeed(Handle->Dev);

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

AL_S32 AlGbe_Hal_StartMacDma(AL_GBE_HalStruct *Handle)
{
    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    AlGbe_Dev_StartMacDma(Handle->Dev);

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

AL_S32 AlGbe_Hal_StartMacDmaIntr(AL_GBE_HalStruct *Handle)
{
    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    AlGbe_Dev_StartMacDmaIntr(Handle->Dev);

    AL_GBE_HAL_UNLOCK(Handle);

    return AL_OK;
}

AL_S32 AlGbe_Hal_TransmitBlock(AL_GBE_HalStruct *Handle, AL_GBE_TxDescConfigStruct *TxConfig, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_Transmit(Handle->Dev, TxConfig);
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    /*
     * wait until frame send done
     */
    Ret = AlGbe_Hal_WaitTxDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        Ret = AL_GBE_ERR_TIMEOUT;
    }

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlGbe_Hal_Transmit(AL_GBE_HalStruct *Handle, AL_GBE_TxDescConfigStruct *TxConfig)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_TransmitPolling(Handle->Dev, TxConfig);
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlGbe_Hal_GetRxDataBuffer(AL_GBE_HalStruct *Handle, AL_GBE_BufferStruct *RxBuffer)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_GetRxDataBuffer(Handle->Dev, RxBuffer);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlGbe_Hal_GetRxDataLength(AL_GBE_HalStruct *Handle, AL_U32 *Length)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_GetRxDataLength(Handle->Dev, Length);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlGbe_Hal_BuildRxDescriptors(AL_GBE_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_BuildRxDescriptors(Handle->Dev);

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlGbe_Hal_ReleaseTxPacket(AL_GBE_HalStruct *Handle)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_HAL_LOCK(Handle);

    Ret = AlGbe_Dev_ReleaseTxPacket(Handle->Dev);
    if (Ret != AL_OK) {
        AL_GBE_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_GBE_HAL_UNLOCK(Handle);

    return Ret;
}