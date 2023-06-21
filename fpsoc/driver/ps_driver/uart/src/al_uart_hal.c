/***************************** Include Files *********************************/
#include "al_uart_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_UART_DevStruct AL_UART_DevInstance[AL_UART_NUM_INSTANCE];

extern AL_BOOL AlUart_Dev_IsTxBusy(AL_UART_DevStruct *Uart);

extern AL_BOOL AlUart_Dev_IsRxBusy(AL_UART_DevStruct *Uart);

/********************************************************/
#ifdef USE_RTOS
static AL_S32 AlUart_Hal_WaitTxDoneOrTimeout(AL_UART_HalStruct *Handle, AL_U32 Timeout)
{
    Al_WaitEvent(Handle->TxEvent);
}

static AL_S32 AlUart_Hal_WaitRxDoneOrTimeout(AL_UART_HalStruct *Handle, AL_U32 Timeout)
{
    Al_WaitEvent(Handle->RxEvent);
}


#define AL_UART_HAL_LOCK(Handle)        do {} while (0)

#define AL_UART_HAL_UNLOCK(Handle)      do {} while (0)

#else

/**
 * This function waiting for uart send done or timeout.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart dev instance
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
static AL_S32 AlUart_Hal_WaitTxDoneOrTimeout(AL_UART_HalStruct *Handle, AL_U32 Timeout)
{
    while (AlUart_Dev_IsTxBusy(Handle->Dev));

    if (Timeout == 0) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Uart wait send done time out!\r\n");
        return AL_UART_ERR_TIMEOUT;
    }

    return AL_OK;
}

/**
 * This function waiting for uart receive done or timeout.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart dev instance
 * @param   Timeout is max wait time for receive done
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
static AL_S32 AlUart_Hal_WaitRxDoneOrTimeout(AL_UART_HalStruct *Handle, AL_U32 Timeout)
{
    while (AlUart_Dev_IsRxBusy(Handle->Dev));

    if (Timeout == 0) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Uart wait recv done time out!\r\n");
        return AL_UART_ERR_TIMEOUT;
    }

    return AL_OK;
}

#define AL_UART_HAL_LOCK(Handle)          do {} while (0)

#define AL_UART_HAL_UNLOCK(Handle)        do {} while (0)

#endif

/**
 * This function action when receive or send data down.
 * @param   UartEvent Pointer to AL_UART_EventStruct contains event datas
 * @param   CallbackRef Pointer to a AL_UART_HalStruct structure that contains uart dev instance
 * @return
 * @note
*/
static AL_VOID AlUart_Hal_EventHandler(AL_UART_EventStruct UartEvent, AL_VOID *CallbackRef)
{
    AL_UART_HalStruct *Handle = (AL_UART_HalStruct *)CallbackRef;
    switch (UartEvent.Event)
    {
    case AL_UART_SEND_DONE:
        break;

    case AL_UART_RECEIVE_DONE:
    case AL_UART_RECEIVE_TIMEOUT:
        break;

    case AL_UART_EVENT_PARE_FRAME_BRKE:
        break;

    case AL_UART_BUSY_DETECT:
        break;

    default:
        break;
    }
}

/**
 * This function initialize the UART mode according to the specified
 *          parameters in the AL_UART_InitStruct and initialize the associated handle.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart dev instance
 * @param   DevId is hardware module id
 * @param   InitConfig pointer to a AL_UART_InitStruct structure
 *          that contains the configuration information for the specified UART peripheral
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_Init(AL_UART_HalStruct *Handle, AL_U32 DevId, AL_UART_InitStruct *InitConfig)
{
    AL_S32 Ret;
    AL_UART_DevStruct *Dev;
    AL_UART_HwConfigStruct *HwConfig;

    if (Handle == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    AL_UART_HAL_LOCK(Handle);

    HwConfig = AlUart_Dev_LookupConfig(DevId);
    if (HwConfig != AL_NULL) {
        Dev = &AL_UART_DevInstance[DevId];
    } else {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    Ret = AlUart_Dev_Init(Dev, InitConfig, DevId);
    if (Ret != AL_OK) {
        AL_UART_HAL_UNLOCK(Handle);
        return Ret;
    }

    Ret = AlUart_Dev_RegisterEventCallBack(Dev, AlUart_Hal_EventHandler, (void *)Handle);
    if (Ret != AL_OK) {
        AL_UART_HAL_UNLOCK(Handle);
        return Ret;
    }

    // (AL_VOID)AlIntr_RegHandler(Dev->IrqNum, AL_NULL, AlUart_Dev_IntrHandler, Dev);
    (AL_VOID)AlIntr_RegHandler(Dev->IntrNum, AL_NULL, AlUart_Dev_IntrHandler, Dev);
    AlIntr_SetGlobalInterrupt(AL_FUNC_ENABLE);

    Handle->Dev  = Dev;
    AL_UART_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function send an amount of data in polling mode.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_SendDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL || Handle->Dev == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    AL_UART_HAL_LOCK(Handle);

    Ret = AlUart_Dev_SendDataPolling(Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        AL_UART_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_UART_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function send an amount of data in blocking mode.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @param   Timeout Timeout duration
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_SendDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    /*
     * check only Handle, more checks in AlUart_Dev_Init function;
    */
    if (Handle == AL_NULL || Handle->Dev == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    AL_UART_HAL_LOCK(Handle);

    Ret = AlUart_Dev_SendData(Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        AL_UART_HAL_UNLOCK(Handle);
        return Ret;
    }

    /*
     * wait until data send done
    */
    Ret = AlUart_Hal_WaitTxDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        AlUart_ll_SetTxIntr(Handle->Dev->BaseAddr, AL_FUNC_DISABLE);
        AL_UART_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_UART_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function receive an amount of data in blocking mode.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be received
 * @param   RecvSize Pointer to Amount how much data has been received
 * @param   Timeout Timeout duration
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_RecvDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 *RecvSize, AL_U32 Timeout)
{
    AL_S32 Ret = AL_OK;

    /*
     * check only Handle, more checks in AlUart_Dev_Init function;
    */
    if (Handle == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    AL_UART_HAL_LOCK(Handle);

    Ret = AlUart_Dev_RecvData(Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        AL_UART_HAL_UNLOCK(Handle);
        return Ret;
    }

    /*
     * wait until data receive done
    */
    Ret = AlUart_Hal_WaitRxDoneOrTimeout(Handle, Timeout);
    if (Ret != AL_OK) {
        AlUart_ll_SetRxIntr(Handle->Dev->BaseAddr, AL_FUNC_DISABLE);
    }

    *RecvSize = Handle->Dev->RecvBuffer.HandledCnt;

    AL_UART_HAL_UNLOCK(Handle);

    return AL_OK;
}

/**
 * This function send an amount of data in non-blocking mode.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_SendData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    /*
     * check only Handle, more checks in AlUart_Dev_Init function;
    */
    if (Handle == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    AL_UART_HAL_LOCK(Handle);

    Ret = AlUart_Dev_SendData(Handle->Dev, Data, Size);

    AL_UART_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function receive an amount of data in non-blocking mode.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be received
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_RecvData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    AL_UART_HAL_LOCK(Handle);

    Ret = AlUart_Dev_RecvData(Handle->Dev, Data, Size);
    if (Ret != AL_OK) {
        AL_UART_HAL_UNLOCK(Handle);
        return Ret;
    }

    AL_UART_HAL_UNLOCK(Handle);

    return AL_OK;
}

/**
 * This function excute operations to set or check uart status.
 * @param   Handle Pointer to a AL_UART_HalStruct structure that contains uart device instance
 * @param   Cmd is io ctl cmd to AL_Uart_IoCtlCmdEnum
 * @param   Data Pointer to cmd args
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Hal_IoCtl(AL_UART_HalStruct *Handle, AL_Uart_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_UART_ERR_NULL_PTR;
    }

    AL_UART_HAL_LOCK(Handle);

    Ret = AlUart_Dev_IoCtl(Handle->Dev, Cmd, Data);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Uart io ctl cmd error:%d\r\n", Ret);
    }

    AL_UART_HAL_LOCK(Handle);

    return Ret;
}
