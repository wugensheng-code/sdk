/***************************** Include Files *********************************/
#include "al_uart_hal.h"
#include "al_uart_ll.h"
#include "al_uart_dev.h"
#include "al_errno.h"
#include "nuclei_sdk_soc.h"
#include "gic_v3.h"

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

static AL_S32 AlUart_Hal_WaitTxDoneOrTimeout(AL_UART_HalStruct *Handle, AL_U32 Timeout)
{
    while (AlUart_Dev_IsTxBusy(Handle->Dev));
}

static AL_S32 AlUart_Hal_WaitRxDoneOrTimeout(AL_UART_HalStruct *Handle, AL_U32 Timeout)
{
    while (AlUart_Dev_IsRxBusy(Handle->Dev));
}

#define AL_UART_HAL_LOCK(Handle)          do {} while (0)

#define AL_UART_HAL_UNLOCK(Handle)        do {} while (0)

#endif

static AL_VOID AlUart_Hal_EventHandler(AL_UART_HalStruct *Handle, AL_UART_EventStruct UartEvent, AL_U32 EventData)
{
    //Handle = (AL_UART_HalStruct *)CallbackRef;
    switch (UartEvent.Event)
    {
    case AL_UART_SEND_DONE:
        //Al_SendEvent(Handle->TxEvent);
        break;

    case AL_UART_RECEIVE_DONE:
    case AL_UART_RECEIVE_TIMEOUT:
        //Al_SendEvent(Handle->RxEvent);
        break;

    case AL_UART_EVENT_PARE_FRAME_BRKE:
        break;

    default:
        break;
    }
}


AL_S32 AlUart_Hal_Init(AL_UART_HalStruct *Handle, AL_UART_InitStruct *InitConfig, AL_U32 DevId)
{
    AL_S32 ret = AL_OK;

    if (Handle == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    AL_UART_HAL_LOCK(Handle);

    ret = AlUart_Dev_LookupConfig(DevId);
    if (ret == AL_OK) {
        Handle->Dev = &AL_UART_DevInstance[DevId];
    }

    ret = AlUart_Dev_Init(Handle->Dev,InitConfig,DevId);

    //register interrupt

    AlUart_ll_Set_IER_PTIME(Handle->Dev->BaseAddr,AL_FUNC_ENABLE);
	ECLIC_Register_IRQ(SOC_INT89_IRQn, ECLIC_NON_VECTOR_INTERRUPT, ECLIC_LEVEL_TRIGGER, 1, 1, AlUart_Dev_IntrHandler);
    __enable_irq();

    ret = AlUart_Dev_RegisterEventCallBack(Handle->Dev, AlUart_Hal_EventHandler, (void *)Handle);

    AL_UART_HAL_UNLOCK(Handle);

    return ret;
}


AL_S32 AlUart_Hal_SendDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /*
     * check only Handle, more checks in AlUart_Dev_Init function;
    */
    if (Handle == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    AL_UART_HAL_LOCK(Handle);

    ret = AlUart_Dev_SendData(Handle->Dev, Data, Size);
    if (ret != AL_OK) {
        return ret;
    }

    /*
     * wait until data send done
    */
    ret = AlUart_Hal_WaitTxDoneOrTimeout(Handle->Dev, Timeout);
    if (ret != AL_OK) {
        //ret = AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT);
        AlUart_ll_SetTxIntr(Handle->Dev->BaseAddr, AL_FUNC_DISABLE);
    }

    AL_UART_HAL_UNLOCK(Handle);

    return ret;
}


AL_S32 AlUart_Hal_RecvDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 *Size, AL_U32 Timeout)
{
    AL_S32 ret = AL_OK;

    /*
     * check only Handle, more checks in AlUart_Dev_Init function;
    */
    if (Handle == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    AL_UART_HAL_LOCK(Handle);
    ret = AlUart_Dev_RecvData(Handle->Dev, Data, Size);
    if (ret != AL_OK) {
        AL_UART_HAL_UNLOCK(Handle);
        return ret;
    }

    /*
     * wait until data receive done
    */
    ret = AlUart_Hal_WaitTxDoneOrTimeout(Handle->Dev, Timeout);
    if (ret != AL_OK) {
        //ret = AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT);
        AlUart_ll_SetRxIntr(Handle->Dev->BaseAddr, AL_FUNC_DISABLE);
    }

    AL_UART_HAL_UNLOCK(Handle);

    return AL_OK;
}

AL_S32 AlUart_Hal_Ioctl(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 *Size, AL_U32 Timeout)
{

}