/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>

#include "al_uart_ll.h"
#include "al_uart_dev.h"


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/
#undef  UART_DEBUG

/************************** Variable Definitions *****************************/
static AL_UART_InitStruct UartDefInitConfigs = {
    .BaudRate     = 115200,
    .Parity       = UART_NO_PARITY,
    .WordLength   = UART_CHAR_8BITS,
    .StopBits     = UART_STOP_1BIT
};

extern AL_UART_HwConfigStruct AlUart_HwConfig[AL_UART_NUM_INSTANCE];

/************************** Function Prototypes ******************************/


/********************************************************/

/**
 * This function look up hardware config structure
 * @param   DeviceId is hardware module id
 * @return  hardware config structure with AL_UART_HwConfigStruct
 * @note
*/
AL_UART_HwConfigStruct *AlUart_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AL_UART_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_UART_NUM_INSTANCE; Index++) {
        if (AlUart_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlUart_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

/**
 * This function get default hardware config structure
 * @param   Uart is pointer to AL_UART_DevStruct
 * @param   Init is pointer to default hardware config structure
 * @return
*           - AL_OK
 * @note
*/
AL_S32 AL_Uart_GetDefaultConf(AL_UART_DevStruct *Uart, AL_UART_InitStruct *Init)
{
    *Init = UartDefInitConfigs;
    return AL_OK;
}

/**
 * This function check whether the sending is busy
 * @param   Uart is pointer to AL_UART_DevStruct
 * @return  sending status
 * @note
*/
AL_BOOL AlUart_Dev_IsTxBusy(AL_UART_DevStruct *Uart)
{
    return (AL_BOOL)(Uart->State & AL_UART_STATE_TX_BUSY);
}

/**
 * This function check whether the receiving is busy
 * @param   Uart is pointer to AL_UART_DevStruct
 * @return  receiving status
 * @note
*/
AL_BOOL AlUart_Dev_IsRxBusy(AL_UART_DevStruct *Uart)
{
    return (AL_BOOL)(Uart->State & AL_UART_STATE_RX_BUSY);
}

/**
 * This function set sending status to busy
 * @param   Uart is pointer to AL_UART_DevStruct
 * @return
 * @note
*/
AL_VOID AlUart_Dev_SetTxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State |= AL_UART_STATE_TX_BUSY;
}

/**
 * This function set receiving status to busy
 * @param   Uart is pointer to AL_UART_DevStruct
 * @return
 * @note
*/
AL_VOID AlUart_Dev_SetRxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State |= AL_UART_STATE_RX_BUSY;
}

/**
 * This function clear sending busy status
 * @param   Uart is pointer to AL_UART_DevStruct
 * @return
 * @note
*/
AL_VOID AlUart_Dev_ClrTxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State &= (~AL_UART_STATE_TX_BUSY);
}

/**
 * This function clear receiving busy status
 * @param   Uart is pointer to AL_UART_DevStruct
 * @return
 * @note
*/
AL_VOID AlUart_Dev_ClrRxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State &= (~AL_UART_STATE_RX_BUSY);
}

/**
 * This function init Uart module
 * @param   Uart is pointer to AL_UART_DevStruct
 * @param   InitConfig is module config structure with AL_UART_InitStruct
 * @param   DevId is hardware module id
 * @return
 *          - AL_OK is init done
 * @note    don't set bandrate(LCR, DLL, DLH) during a transaction
*/
AL_S32 AlUart_Dev_Init(AL_UART_DevStruct *Uart, AL_UART_InitStruct *InitConfig, AL_U32 DevId)
{
    if (Uart == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    /* clear receive fifo before init */
    while(AlUart_ll_IsRxDataReady(Uart->BaseAddr)){
        AL_U8 tmp = (AL_U8)AlUart_ll_RecvByte(Uart->BaseAddr);
    }

    Uart->Configs  = (InitConfig == AL_NULL) ? UartDefInitConfigs : (*InitConfig);
    Uart->BaseAddr = AlUart_Dev_LookupConfig(DevId)->BaseAddress;

    AlUart_ll_SetBaudRate(Uart->BaseAddr, InitConfig->BaudRate);
    AlUart_ll_CfgCharacter(Uart->BaseAddr, InitConfig->WordLength, InitConfig->Parity, InitConfig->StopBits);

    AlUart_ll_SetFifo(Uart->BaseAddr, AL_FUNC_ENABLE);
    AlUart_ll_SetTxFifoThr(Uart->BaseAddr, UART_TxFIFO_EMPTY);
    AlUart_ll_SetRxFifoThr(Uart->BaseAddr, UART_RxFIFO_CHAR_1);
    AlUart_ll_SetThreIntr(Uart->BaseAddr, AL_FUNC_ENABLE);
    AlUart_ll_SetTxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);

    Uart->State |= AL_UART_STATE_READY;

    return AL_OK;
}

/**
 * This function send datas with interrupt
 * @param   Uart is pointer to AL_UART_DevStruct
 * @param   SendData is pointer to the datas to be send
 * @param   SendSize indicates how much data to send 
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlUart_Dev_SendData(AL_UART_DevStruct *Uart, AL_U8 *SendData, AL_U32 SendSize)
{

#ifdef UART_DEBUG
    AL_LOG(AL_ERR_LEVEL_DEBUG, "uart=%p SendDataAddr=%p SendSize=%d\n", Uart, SendData, SendSize);
#endif

    if (SendData == AL_NULL || SendSize == 0) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    if ((Uart->State & AL_UART_STATE_READY) == 0) {
        return AL_UART_ERR_NOT_READY;
    }

    if (AlUart_Dev_IsTxBusy(Uart)) {
        return AL_UART_ERR_BUSY;
    }

    /* Change Status */
    AlUart_Dev_SetTxBusy(Uart);

    Uart->SendBuffer.BufferPtr      = SendData;
    Uart->SendBuffer.RequestedCnt   = SendSize;
    Uart->SendBuffer.HandledCnt     = 0;

    AlUart_ll_SetTxIntr(Uart->BaseAddr, AL_FUNC_ENABLE);

    return AL_OK;
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
AL_S32 AlUart_Dev_RecvData(AL_UART_DevStruct *Uart, AL_U8 *ReceiveBuf, AL_U32 ReceiveSize)
{
    AL_U32 ReceivedCount = 0;

    if (Uart == AL_NULL || ReceiveBuf == AL_NULL || ReceiveSize == 0 ) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    if ((Uart->State & AL_UART_STATE_READY) != AL_UART_STATE_READY) {
        return AL_UART_ERR_NOT_READY;
    }

    if (AlUart_Dev_IsRxBusy(Uart)) {
        return AL_UART_ERR_BUSY;
    }

    /* Change Status */
    AlUart_Dev_SetRxBusy(Uart);

    /*
     * Loop until there is no more data in RX Fifo or the specified
     * number of bytes has been received
     */
    while ((ReceivedCount < ReceiveSize) && (AlUart_ll_IsRxDataReady(Uart->BaseAddr))) {
        ReceiveBuf[ReceivedCount] = (AL_U8)AlUart_ll_RecvByte(Uart->BaseAddr);
        ReceivedCount++;
    }

    /* Setup the buffer parameters */
    if (ReceivedCount < ReceiveSize) {
        Uart->RecvBuffer.RequestedCnt   = ReceiveSize;
        Uart->RecvBuffer.HandledCnt     = ReceivedCount;
        Uart->RecvBuffer.BufferPtr      = ReceiveBuf;

        /*
        * trigger interrupt to receive data in interrupt
        */
        AlUart_ll_SetRxIntr(Uart->BaseAddr, AL_FUNC_ENABLE);
    } else {
        if (Uart->EventCallBack) {
            AL_UART_EventStruct UartEvent = {
                .Event        = AL_UART_RECEIVE_DONE,
                .EventData    = ReceiveSize
            };

            Uart->EventCallBack(UartEvent, Uart->EventCallBackRef);
        }

        AlUart_Dev_ClrRxBusy(Uart);
    }
    return AL_OK;
}

/**
 * This function register send or receive down call back function
 * @param   Uart is pointer to AL_UART_DevStruct
 * @param   CallBack is call back struct with AL_Uart_EventCallBack
 * @param   CallbackRef
 * @return
 *          - AL_OK is register correct
 * @note
*/
AL_S32 AlUart_Dev_RegisterEventCallBack(AL_UART_DevStruct *Uart, AL_Uart_EventCallBack Callback, void *CallbackRef)
{
    if (Uart == AL_NULL || Callback == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    if (Uart->EventCallBack != AL_NULL) {

#ifdef UART_DEBUG
        AL_LOG(AL_ERR_LEVEL_WARNING, "uart=%p duplicate register callback: replace old:%p with New: %p\n", \
                Uart, Uart->EventCallBack, Callback);
#endif
    }

    Uart->EventCallBack        = Callback;
    Uart->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 * This function unregister interrupt call back function
 * @param   Uart is pointer to AL_UART_DevStruct
 * @return
 *          - AL_OK is unregister correct
 * @note
*/
AL_S32 AlUart_Dev_UnRegisterEventCallBack(AL_UART_DevStruct *Uart)
{
    if (Uart == AL_NULL) {
#ifdef UART_DEBUG
        AL_LOG(AL_ERR_LEVEL_ERROR, "uart=%p duplicate register callback: replace old:%p with New: %p\n", \
            Uart, Uart->EventCallBack, Callback);
#endif
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    Uart->EventCallBack = (AL_Uart_EventCallBack)AL_NULL;

    return AL_OK;
}

/**
 * This function is receive data interrupt handler
 * @param   Uart is pointer to AL_UART_DevStruct
 * @param   Timeout indicates the time limit between characters
 * @return
 * @note
*/
static AL_VOID AlUart_Dev_RecvDataHandler(AL_UART_DevStruct *Uart, AL_BOOL Timeout)
{
    while ((AlUart_ll_IsRxDataReady(Uart->BaseAddr) == AL_TRUE) &&
            (Uart->RecvBuffer.HandledCnt < Uart->RecvBuffer.RequestedCnt)) {
        Uart->RecvBuffer.BufferPtr[Uart->RecvBuffer.HandledCnt] = (AL_U8)AlUart_ll_RecvByte(Uart->BaseAddr);
        Uart->RecvBuffer.HandledCnt ++;
    }

    if (Uart->RecvBuffer.HandledCnt == Uart->RecvBuffer.RequestedCnt || Timeout) {
        AlUart_ll_SetRxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);
        if (Uart->EventCallBack) {
            AL_UART_EventStruct UartEvent = {
                .Event        = (AL_UART_RECEIVE_DONE | (Timeout ? AL_UART_RECEIVE_TIMEOUT : 0x0)),
                .EventData    = Uart->SendBuffer.HandledCnt
            };

            Uart->EventCallBack(UartEvent, Uart->EventCallBackRef);
        }

        /* Clear Status */
        AlUart_Dev_ClrRxBusy(Uart);
    }
}

/**
 * This function is send data interrupt handler
 * @param   Uart is pointer to AL_UART_DevStruct
 * @return
 * @note
*/
static AL_VOID AlUart_Dev_SendDataHandler(AL_UART_DevStruct *Uart)
{
    if (Uart->SendBuffer.HandledCnt == Uart->SendBuffer.RequestedCnt) {
        AlUart_ll_SetTxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);

        if (Uart->EventCallBack) {
            AL_UART_EventStruct event = {
                .Event         = AL_UART_SEND_DONE,
                .EventData     = Uart->SendBuffer.HandledCnt
            };

            (*Uart->EventCallBack)(event, Uart->EventCallBackRef);
        }

        /* Clear Status */
        AlUart_Dev_ClrTxBusy(Uart);
    } else {
        while (!(AlUart_ll_IsTransmitFifoFull(Uart->BaseAddr)) &&
                (Uart->SendBuffer.HandledCnt < Uart->SendBuffer.RequestedCnt)) {
            AlUart_ll_SendByte(Uart->BaseAddr, Uart->SendBuffer.BufferPtr[Uart->SendBuffer.HandledCnt]);
            Uart->SendBuffer.HandledCnt ++;
        }
    }
}

/**
 * This function send datas with polling
 * @param   Uart is pointer to AL_UART_DevStruct
 * @param   Data is pointer to the datas to be send
 * @param   Size indicates how much data to send 
 * @return
 *          - AL_OK
 * @note
*/
AL_S32 AlUart_Dev_SendDataPolling(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 Size)
{
    static int i;
    AL_U32 HandledCnt = 0;
    while (HandledCnt < Size) {
        if (!(AlUart_ll_IsTransmitFifoFull(Uart->BaseAddr))) {
            AlUart_ll_SendByte(Uart->BaseAddr, Data[HandledCnt]);
            HandledCnt ++;
        }
    }

    /* this bit is set whenever the Transmitter Shift Register and the FIFO are both empty. */
    while(!(AlUart_ll_IsTxDown(Uart->BaseAddr)));

    return AL_OK;
}

/**
 * This function is error interrupt handler
 * @param   Uart is pointer to AL_UART_DevStruct
 * @param   IntrStatus is interrupt register status
 * @return
 * @note
*/
static AL_VOID AlUart_Dev_ErrorHandler(AL_UART_DevStruct *Uart, AL_UART_InterruptEnum IntrStatus)
{
    AL_UART_EventStruct UartEvent;
    if(AlUart_ll_IsOeIntr(Uart->BaseAddr)) {
        UartEvent.Event = AL_UART_EVENT_OVER_RUN_ERR;
        UartEvent.EventData = AL_NULL;
    }
    if(AlUart_ll_IsPeIntr(Uart->BaseAddr)) {
        UartEvent.Event = AL_UART_EVENT_PARITY_ERR;
        UartEvent.EventData = AL_NULL;
    }
    if(AlUart_ll_IsFeIntr(Uart->BaseAddr)) {
        UartEvent.Event = AL_UART_EVENT_FRAMING_ERR;
        UartEvent.EventData = AL_NULL;
    }
    if(AlUart_ll_IsBiIntr(Uart->BaseAddr)) {
        UartEvent.Event = AL_UART_EVENT_BREAK_INTR;
        UartEvent.EventData = AL_NULL;
    }
}

#define UART_IS_CHARACTER_TIMEOUT(Status)        (Status == UART_CHARACTER_TIMEOUT)


/**
 * This function is interrupts call back
 * @param   Instance is pointer to interrupts call back reference
 * @return
 * @note
*/
AL_VOID AlUart_Dev_IntrHandler(AL_VOID *Instance)
{
    AL_UART_DevStruct *Uart = (AL_UART_DevStruct *)Instance;
    AL_UART_InterruptEnum IntrStatus = AlUart_ll_GetIntrStatus(Uart->BaseAddr);

    if (IntrStatus & (UART_RECEIVED_DATA_AVAILABLE | UART_CHARACTER_TIMEOUT)) {
        AlUart_Dev_RecvDataHandler(Uart, UART_IS_CHARACTER_TIMEOUT(IntrStatus));
    }

    if (IntrStatus & UART_THR_EMPTY) {
        AlUart_Dev_SendDataHandler(Uart);
    }
    
    if (IntrStatus & UART_RECEIVER_LINE_STATUS) {
        AlUart_Dev_ErrorHandler(Uart, IntrStatus);
    }
}
