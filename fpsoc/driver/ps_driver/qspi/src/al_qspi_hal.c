/***************************** Include Files *********************************/
#include "al_qspi_hal.h"
#include "al_intr.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_QSPI_DevStruct AL_QSPI_DevInstance[AL_QSPI_NUM_INSTANCE];

/********************************************************/
#ifdef USE_RTOS
/**
 * This function
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitTxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO: wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

/**
 * This function
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send or receive timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitRxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    /*
     * TODO:wait for event timeout
    */
    (void) Handle;
    (void) Timeout;
}

/**
 * This function
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param
 * @param
 * @return
 * @note    None
*/
static AlQspi_Hal_IntrHandler(AL_QSPI_HalStruct *Handle, AL_U32 Event, AL_U32 EventData)
{
    /*
     * TODO:send event
    */
    (void) Handle;
    (void) Event;
    (void) EventData;
}

#define AL_QSPI_HAL_LOCK(Handle)        do {} while (0)

#define AL_QSPI_HAL_UNLOCK(Handle)      do {} while (0)

#else

/**
 * This function is wait send done or timeout
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitTxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlQspi_Dev_IsTxBusy(AL_QSPI_DevStruct *Qspi);

#ifdef QSPI_DEBUG
    AlQspi_Dev_DumpReg(Handle->Dev->BaseAddr);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_Dev_IsTxBusy:%d\r\n", AlQspi_Dev_IsTxBusy(Handle->Dev));
#endif

    while(AlQspi_Dev_IsTxBusy(Handle->Dev));
    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);

    return AL_OK;
}

/**
 * This function is wait receive done or timeout
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to receive timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitRxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlQspi_Dev_IsRxBusy(AL_QSPI_DevStruct *Qspi);

#ifdef QSPI_DEBUG
    AlQspi_Dev_DumpReg(Handle->Dev->BaseAddr);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_Dev_IsRxBusy:%d\r\n", AlQspi_Dev_IsRxBusy(Handle->Dev));
#endif

    while(AlQspi_Dev_IsRxBusy(Handle->Dev));
    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);

    return AL_OK;
}

/**
 * This function is wait receive done and send done or timeout
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Timeout is max wait time
 * @return  Return whether to send and receive timeout
 * @note    None
*/
static AL_S32 AlQspi_Hal_WaitTxRxDoneOrTimeout(AL_QSPI_HalStruct *Handle, AL_U32 Timeout)
{
    extern AL_BOOL AlQspi_Dev_IsRxBusy(AL_QSPI_DevStruct *Qspi);
    extern AL_BOOL AlQspi_Dev_IsTxBusy(AL_QSPI_DevStruct *Qspi);

#ifdef QSPI_DEBUG
    AlQspi_Dev_DumpReg(Handle->Dev->BaseAddr);
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_Dev_IsRxBusy:%d\r\n", AlQspi_Dev_IsRxBusy(Handle->Dev));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_Dev_IsTxBusy:%d\r\n", AlQspi_Dev_IsTxBusy(Handle->Dev));
#endif

    while(AlQspi_Dev_IsRxBusy(Handle->Dev) || AlQspi_Dev_IsTxBusy(Handle->Dev));
    while(QSPI_SR_BUSY == AlQspi_ll_IsBusy(Handle->Dev->BaseAddr));

    AlQspi_ll_SetSlvSel(Handle->Dev->BaseAddr, 0);

    return AL_OK;
}



#define AL_QSPI_HAL_LOCK(Handle)          do {} while (0)

#define AL_QSPI_HAL_UNLOCK(Handle)        do {} while (0)

#endif

/**
 * This is default event callback function
 * @param   QspiEvent is a AL_QSPI_EventStruct struct
 * @param   CallbackRef is parameter of callback function
 * @return  AL_OK
 * @note    None
*/
static AL_S32 AlQspi_DefEventCallBack(AL_QSPI_EventStruct QspiEvent, void *CallbackRef)
{
    switch (QspiEvent.Event) {
    case AL_QSPI_SEND_DONE:
#ifdef QSPI_DEBUG
       AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack AL_QSPI_SEND_DONE\r\n");
#endif
        break;
    case AL_QSPI_RECEIVE_DONE:
#ifdef QSPI_DEBUG
       AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack AL_QSPI_RECEIVE_DONE\r\n");
#endif
        break;
    case AL_QSPI_SEND_TIMEOUT:
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack AL_QSPI_SEND_TIMEOUT\r\n");
#endif
        break;
    case AL_QSPI_RECEIVE_TIMEOUT:
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack AL_QSPI_RECEIVE_TIMEOUT\r\n");
#endif
        break;
    default:
#ifdef QSPI_DEBUG
        AL_LOG(AL_LOG_LEVEL_DEBUG, "AlQspi_DefEventCallBack default\r\n");
#endif
        break;
    }

    return AL_OK;
}

/**
 * This function initialize the qspi according to the specified
 *          parameters in the AL_QSPI_ConfigsStruct and initialize the associated handle.
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   InitConfig pointer to a AL_QSPI_ConfigsStruct structure
 *          that contains the configuration information for the specified qspi peripheral
 * @param   Callback is a function pointer to qspi event callback function
 * @param   CallbackRef is parameter of callback function
 * @param   DevId is hardware module id
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_Init(AL_QSPI_HalStruct *Handle, AL_QSPI_ConfigsStruct *InitConfig, QSPI_EventCallBack Callback,
                      AL_VOID *CallbackRef, AL_U32 DevId)
{
    AL_S32 ret = AL_OK;
    AL_QSPI_HwConfigStruct *CfgPtr = AL_NULL;

    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    CfgPtr = AlQspi_Dev_LookupConfig(DevId);
    Handle->Dev = &AL_QSPI_DevInstance[CfgPtr->DeviceId];
    Handle->Dev->BaseAddr = CfgPtr->BaseAddress;
    Handle->Dev->Fifolen  = CfgPtr->FifoLen;

    if(AL_NULL == Callback) {
        ret = AlQspi_Dev_RegisterIntrCallBack(Handle->Dev, AlQspi_DefEventCallBack, AL_NULL);
    } else {
        ret = AlQspi_Dev_RegisterIntrCallBack(Handle->Dev, Callback, CallbackRef);
    }

    if (ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return ret;
    }

    ret = AlQspi_Dev_Init(Handle->Dev, InitConfig);
    if (ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return ret;
    }

    AlIntr_RegHandler(CfgPtr->InterrupId + 13, AL_NULL, AlQspi_Dev_IntrHandler, Handle->Dev);
    AL_QSPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function is qspi blocking send data
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Data is pointer to send data
 * @param   Size is send data size
 * @param   Timeout is max wait time for send done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_SendDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    ret = AlQspi_Dev_SendData(Handle->Dev, Data, Size);
    if (ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return ret;
    }

    /* wait until data send done */
    ret = AlQspi_Hal_WaitTxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_QSPI_ERR_TIMEOUT;
        AlQspi_ll_MaskIntr(Handle->Dev->BaseAddr, QSPI_TXEIM);
    }

    AL_QSPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function is qspi blocking receive data
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   Data is pointer to receive data
 * @param   Size is receive data size
 * @param   Timeout is max wait time for receive done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_RecvDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    ret = AlQspi_Dev_RecvData(Handle->Dev, Data, Size);
    if (ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return ret;
    }

    /* wait until data receive done */
    ret = AlQspi_Hal_WaitRxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_QSPI_ERR_TIMEOUT;
        AlQspi_ll_MaskIntr(Handle->Dev->BaseAddr, QSPI_RXFIM);
    }

    AL_QSPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @param   SendData is pointer to send data
 * @param   SendSize is send data size
 * @param   RecvData is pointer to receive data
 * @param   RecvSize is receive data size
 * @param   Timeout is max wait time for send done
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_TranferDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                   AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /* check only Handle, more checks in AlQspi_Dev_Init function */
    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    ret = AlQspi_Dev_TranferData(Handle->Dev, SendData, SendSize, RecvData, RecvSize);
    if (ret != AL_OK) {
        AL_QSPI_HAL_UNLOCK(Handle);
        return ret;
    }

    /* wait until data done */
    ret = AlQspi_Hal_WaitTxRxDoneOrTimeout(Handle, Timeout);
    if (ret != AL_OK) {
        ret = AL_QSPI_ERR_TIMEOUT;
        AlQspi_ll_MaskIntr(Handle->Dev->BaseAddr, QSPI_TXEIM | QSPI_RXFIM);
    }

    AL_QSPI_HAL_UNLOCK(Handle);

    return ret;
}

/**
 * This function is excute operations to set or check qspi configuration status.
 * @param   Handle is pointer to AL_QSPI_HalStruct
 * @return  The state of function execution
 * @note    None
*/
AL_S32 AlQspi_Hal_IoCtl(AL_QSPI_HalStruct *Handle, AL_Qspi_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    AL_QSPI_HAL_LOCK(Handle);

    Ret = AlQspi_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Qspi io ctl cmd error:%d\r\n", Ret);
    }

    AL_QSPI_HAL_LOCK(Handle);

    return Ret;
}


