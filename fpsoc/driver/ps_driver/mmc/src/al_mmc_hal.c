#include "al_mmc_hal.h"

static AL_MMC_DevStruct AL_MMC_DevInstance[AL_MMC_NUM_INSTANCE];

extern AL_BOOL AlMmc_Dev_GetState(AL_MMC_DevStruct *Dev, AL_MMC_StateEnum State);

#ifdef USE_RTOS
static AL_S32 AlMmc_Hal_WaitWriteDoneOrTimeout(AL_MMC_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

static AL_S32 AlMmc_Hal_WaitReadDoneOrTimeout(AL_MMC_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}


static AlMmc_Hal_IntrHandler(AL_MMC_HalStruct *Handle, AL_U32 Event, AL_U32 EventData)
{
    /*
     * TODO:send event
    */
    (void) Handle;
    (void) Event;
    (void) EventData;
}

#define AL_MMC_HAL_LOCK(Handle)        do {} while (0)

#define AL_MMC_HAL_UNLOCK(Handle)      do {} while (0)

#else

/**
 * This function wait for frame write done or timeout
 * @param   Handle is pointer to AL_MMC_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlMmc_Hal_WaitWriteDoneOrTimeout(AL_MMC_HalStruct *Handle, AL_U32 Timeout)
{
    while(AlMmc_Dev_GetState(Handle->Dev, AL_MMC_STATE_WRITE_BUSY) && Timeout);

    if (Timeout == 0) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Mmc wait send done time out!\r\n");
        return AL_MMC_ERR_TIMEOUT;
    }

    return AL_OK;
}

/**
 * This function wait for frame read done or timeout
 * @param   Handle is pointer to AL_MMC_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
 * @note
*/
static AL_S32 AlMmc_Hal_WaitReadDoneOrTimeout(AL_MMC_HalStruct *Handle, AL_U32 Timeout)
{
    while(AlMmc_Dev_GetState(Handle->Dev, AL_MMC_STATE_READ_BUSY) && Timeout);

    if (Timeout == 0) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Mmc wait recv done time out!\r\n");
        return AL_MMC_ERR_TIMEOUT;
    }

    return AL_OK;
}


#define AL_MMC_HAL_LOCK(Handle)          do {} while (0)

#define AL_MMC_HAL_UNLOCK(Handle)        do {} while (0)

#endif

/**
 * This function is intr handler call back function
 * @param   Event is pointer to AL_MMC_EventStruct
 * @param   CallBackRef
 * @return
 * @note
*/
static AL_VOID AlMmc_Hal_DefEventCallBack(AL_MMC_EventStruct *Event, AL_VOID *CallBackRef)
{

}

/**
 * This function init MMC module
 * @param   Handle is pointer to AL_MMC_HalStruct
 * @param   InitConfig is module config structure with AL_MMC_InitStruct
 * @param   CallBack is call back struct with AL_MMC_CallBackStruct
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlMmc_Hal_Init(AL_MMC_HalStruct *Handle, AL_MMC_InitStruct *InitConfig, AL_MMC_CallBackStruct *CallBack,
                      AL_U32 DevId)
{
    AL_S32 Ret = AL_OK;
    AL_MMC_HwConfigStruct *HwConfig;
    AL_MMC_CallBackStruct EventCallBack;

    if (Handle == AL_NULL) {
        return AL_MMC_ERR_NULL_PTR;
    }

    AL_MMC_HAL_LOCK(Handle);

    /* 1. look up hardware config */
    HwConfig = AlMmc_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        Handle->Dev = &AL_MMC_DevInstance[DevId];
        // memset(Handle->Dev, 0, sizeof(AL_MMC_DevStruct));
    }

#ifdef USE_RTOS
    if (CallBack == AL_NULL) {
        EventCallBack.Func  = AlMmc_Hal_DefEventCallBack;
        EventCallBack.Ref   = Handle->Dev;
    } else {
        EventCallBack.Func  = CallBack->Func;
        EventCallBack.Ref   = CallBack->Ref;
    }

    AlMmc_Dev_RegisterEventCallBack(Handle->Dev, &EventCallBack);

    AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlMmc_Dev_IntrHandler, Handle->Dev);
    Al_Intr_SetGlobalInterrupt(AL_FUNC_ENABLE);
#endif

    /* 2. Init IP, do not use intr */
    Ret = AlMmc_Dev_Init(Handle->Dev, HwConfig, InitConfig);
    if (Ret != AL_OK) {
        AL_MMC_HAL_UNLOCK(Handle);
        return Ret;
    }

#ifndef USE_RTOS
    /* 3. register callback */
    if (CallBack == AL_NULL) {
        EventCallBack.Func  = AlMmc_Hal_DefEventCallBack;
        EventCallBack.Ref   = Handle->Dev;
    } else {
        EventCallBack.Func  = CallBack->Func;
        EventCallBack.Ref   = CallBack->Ref;
    }

    AlMmc_Dev_RegisterEventCallBack(Handle->Dev, &EventCallBack);

    /* 4. register intr */
    AlIntr_RegHandler(HwConfig->IntrId, AL_NULL, AlMmc_Dev_IntrHandler, Handle->Dev);
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);
#endif

    AL_MMC_HAL_UNLOCK(Handle);

    return Ret;
}

AL_S32 AlMmc_Hal_DeInit(AL_MMC_HalStruct *Handle)
{
    /*TODO:*/
}

AL_S32 AlMmc_Hal_Write(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt)
{
    AL_S32 Ret = AL_OK;

    if ((Handle == AL_NULL) || (Buf == AL_NULL)) {
        return AL_MMC_ERR_NULL_PTR;
    }

    AL_MMC_HAL_LOCK(Handle);

    Ret = AlMmc_Dev_Write(Handle->Dev, Buf, BlkOffset, BlkCnt);
    if (Ret != AL_OK) {
        AL_MMC_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_MMC_HAL_UNLOCK(Handle);

    return Ret;

}

AL_S32 AlMmc_Hal_WriteBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    if ((Handle == AL_NULL) || (Buf == AL_NULL)) {
        return AL_MMC_ERR_NULL_PTR;
    }

    AL_MMC_HAL_LOCK(Handle);

    Ret = AlMmc_Dev_Write(Handle->Dev, Buf, BlkOffset, BlkCnt);
    if (Ret != AL_OK) {
        AL_MMC_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlMmc_Hal_WaitWriteDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        AL_MMC_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_MMC_HAL_UNLOCK(Handle);

    return Ret;

}

AL_S32 AlMmc_Hal_Read(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt)
{
    AL_S32 Ret = AL_OK;

    if ((Handle == AL_NULL) || (Buf == AL_NULL)) {
        return AL_MMC_ERR_NULL_PTR;
    }

    AL_MMC_HAL_LOCK(Handle);

    Ret = AlMmc_Dev_Read(Handle->Dev, Buf, BlkOffset, BlkCnt);
    if (Ret != AL_OK) {
        AL_MMC_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_MMC_HAL_UNLOCK(Handle);

    return Ret;

}

AL_S32 AlMmc_Hal_ReadBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    if ((Handle == AL_NULL) || (Buf == AL_NULL)) {
        return AL_MMC_ERR_NULL_PTR;
    }

    AL_MMC_HAL_LOCK(Handle);

    Ret = AlMmc_Dev_Read(Handle->Dev, Buf, BlkOffset, BlkCnt);
    if (Ret != AL_OK) {
        AL_MMC_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlMmc_Hal_WaitReadDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        AL_MMC_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_MMC_HAL_UNLOCK(Handle);

    return Ret;

}

AL_S32 AlMmc_Hal_IoCtl(AL_MMC_HalStruct *Handle, AL_MMC_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_MMC_ERR_NULL_PTR;
    }

    AL_MMC_HAL_LOCK(Handle);

    Ret = AlMmc_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Ioctrl error\r\n");
    }

    AL_MMC_HAL_UNLOCK(Handle);

    return Ret;
}
