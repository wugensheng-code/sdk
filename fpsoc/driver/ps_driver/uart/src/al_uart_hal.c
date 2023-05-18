/***************************** Include Files *********************************/
#include "al_uart_hal.h"
#include "al_uart_ll.h"
#include "al_uart_dev.h"
#include "al_errno.h"
#include "nuclei_sdk_soc.h"
#include "al_intr.h"

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
 * This function wait for uart send done or timeout
 * @param   Handle is pointer to AL_UART_HalStruct
 * @param   Timeout is max wait time for send done
 * @return
 *          - AL_OK
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
 * This function wait for uart receive done or timeout
 * @param   Handle is pointer to AL_UART_HalStruct
 * @param   Timeout is max wait time for receive done
 * @return
 *          - AL_OK
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
 * This function action when receive or send data down
 * @param   UartEvent is pointer to AL_UART_EventStruct
 * @param   CallbackRef
 * @return
 *          - AL_OK
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

    default:
        break;
    }
}

/**
 * This function init Uart module
 * @param   Handle is pointer to AL_UART_HalStruct
 * @param   DevId is hardware module id
 * @param   InitConfig is module config structure with AL_UART_InitStruct
 * @return
 *          - AL_OK
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

    (AL_VOID)AlIntr_RegHandler(SOC_INT89_IRQn, AL_NULL, AlUart_Dev_IntrHandler, Dev);
    __enable_irq();

    Handle->Dev  = Dev;
    AL_UART_HAL_UNLOCK(Handle);

    return Ret;
}

/**
 * This function send datas with polling
 * @param   Handle is pointer to AL_UART_HalStruct
 * @param   Data is pointer to the datas to be send
 * @param   Size indicates how much data to send 
 * @return
 *          - AL_OK
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
 * This function send datas with timeout
 * @param   Handle is pointer to AL_UART_HalStruct
 * @param   Data is pointer to the datas buffer to be send
 * @param   Size indicates how much data to send 
 * @param   Timeout indicates the time limit for sending data
 * @return
 *          - AL_OK
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
 * This function receive datas with timeout
 * @param   Handle is pointer to AL_UART_HalStruct
 * @param   Data is pointer to the receive data buffer
 * @param   Size indicates how much data to receive 
 * @param   Timeout indicates the time limit for receiving data
 * @return
 *          - AL_OK
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
 * This function send datas 
 * @param   Handle is pointer to AL_UART_HalStruct
 * @param   Data is pointer to the datas buffer to be send
 * @param   Size indicates how much data to send 
 * @return
 *          - AL_OK
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
 * This function receive datas
 * @param   Handle is pointer to AL_UART_HalStruct
 * @param   Data is pointer to the receive data buffer
 * @param   Size indicates how much data to receive 
 * @return
 *          - AL_OK
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

AL_S32 AlUart_Hal_Ioctl(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 *Size, AL_U32 Timeout)
{

}
