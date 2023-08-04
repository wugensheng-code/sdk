/***************************** Include Files *********************************/
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

typedef enum
{
    UART_TX                = 0,
    UART_RX                = 1,
} AL_Uart_TransferEnum;

extern AL_UART_HwConfigStruct AlUart_HwConfig[AL_UART_NUM_INSTANCE];

/************************** Function Prototypes ******************************/
static AL_VOID AlUart_Dev_SetIntr(AL_UART_DevStruct *Uart, AL_Uart_TransferEnum Transfer,  AL_FUNCTION State);

/********************************************************/

/**
 * This function look up hardware config structure.
 * @param   DeviceId is hardware module id
 * @return
 *          - AL_UART_HwConfigStruct for hardware config
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
 * This function get default hardware config structure.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   Init pointer to a AL_UART_InitStruct structure
 *          that contains the configuration information for the specified UART peripheral
 * @return
 *          - AL_OK for function success
 * @note
*/
AL_S32 AL_Uart_GetDefaultConf(AL_UART_DevStruct *Uart, AL_UART_InitStruct *Init)
{
    *Init = UartDefInitConfigs;
    return AL_OK;
}

/**
 * This function check whether the uart tx is busy.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @return
 *          - AL_BOOL for uart tx status
 * @note
*/
AL_BOOL AlUart_Dev_IsTxBusy(AL_UART_DevStruct *Uart)
{
    return (AL_BOOL)(Uart->State & AL_UART_STATE_TX_BUSY);
}

/**
 * This function check whether the uart rx is busy.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @return
 *          - AL_BOOL for uart rx status
 * @note
*/
AL_BOOL AlUart_Dev_IsRxBusy(AL_UART_DevStruct *Uart)
{
    return (AL_BOOL)(Uart->State & AL_UART_STATE_RX_BUSY);
}

/**
 * This function set uart tx status to busy.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @return
 * @note
*/
AL_VOID AlUart_Dev_SetTxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State |= AL_UART_STATE_TX_BUSY;
    if (Uart->EventCallBack) {
        AL_UART_EventStruct UartEvent = {
            .Events        = AL_UART_EVENT_READY_TO_SEND
        };
        Uart->EventCallBack(UartEvent, Uart->EventCallBackRef);
    }
}

/**
 * This function set uart tx status to busy.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @return
 * @note
*/
AL_VOID AlUart_Dev_SetRxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State |= AL_UART_STATE_RX_BUSY;
        if (Uart->EventCallBack) {
        AL_UART_EventStruct UartEvent = {
            .Events        = AL_UART_EVENT_READY_TO_RECEIVE
        };
        Uart->EventCallBack(UartEvent, Uart->EventCallBackRef);
    }
}

/**
 * This function clear uart tx busy status.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @return
 * @note
*/
AL_VOID AlUart_Dev_ClrTxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State &= (~AL_UART_STATE_TX_BUSY);
}

/**
 * This function clear uart rx busy status.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @return
 * @note
*/
AL_VOID AlUart_Dev_ClrRxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State &= (~AL_UART_STATE_RX_BUSY);
}

/**
 * This function initialize UART registers according to the specified parameters in AL_UART_InitStruct.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   DevId is hardware module id
 * @param   InitConfig pointer to a AL_UART_InitStruct structure
 *          that contains the configuration information for the specified UART peripheral
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note    access baudrate(LCR, DLL, DLH) related register during a transaction will cause busy detect interrupt
*/
AL_S32 AlUart_Dev_Init(AL_UART_DevStruct *Uart, AL_UART_InitStruct *InitConfig, AL_U32 DevId)
{
    if (Uart == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    if (DevId >= AL_UART_NUM_INSTANCE) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    /* soft reset uart bus */
    if(Uart->State != AL_UART_STATE_NOT_INIT) {
        if (Uart->BaseAddr == UART0__BASE_ADDR) {
            AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART0__SRST__N__SHIFT, AL_FUNC_DISABLE);
            AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART0__SRST__N__SHIFT, AL_FUNC_ENABLE);
        } else {
            AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART1__SRST__N__SHIFT, AL_FUNC_DISABLE);
            AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART1__SRST__N__SHIFT, AL_FUNC_ENABLE);
        }
    }

    /* Save the relevant configuration to the uart device structure */
    Uart->DevId        = DevId;
    Uart->Configs      = (InitConfig == AL_NULL) ? UartDefInitConfigs : (*InitConfig);
    Uart->BaseAddr     = AlUart_Dev_LookupConfig(DevId)->BaseAddress;
    Uart->IntrNum      = AlUart_Dev_LookupConfig(DevId)->IntrNum;
    Uart->InputClockHz = AlUart_Dev_LookupConfig(DevId)->InputClockHz;

    /* step 0. Set AutoFlowControl */
    if (Uart->Configs.AutoFlowCtlEnable) {
        AlUart_ll_SetAutoFlowCtl(Uart->BaseAddr , AL_FUNC_ENABLE);
    }

    /* in the initialization stage ensure that no characters during this time period are received/transmitted */
    /* step 1. Set DLL and DLH to 0 */
    AL_REG32_SET_BIT(Uart->BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_ENABLE);
    AL_REG32_WRITE(Uart->BaseAddr + UART__IER_DLH__OFFSET, 0);
    AL_REG32_WRITE(Uart->BaseAddr + UART__RBR__THR__DLL__OFFSET, 0);
    AL_REG32_SET_BIT(Uart->BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_DISABLE);

    /* step 2 & step 3 */
    AlUart_ll_CfgCharacter(Uart->BaseAddr, InitConfig->WordLength, InitConfig->Parity, InitConfig->StopBits);

    /* step 4. set the FCR register to enable FIFOs */
    AlUart_ll_SetFifo(Uart->BaseAddr, AL_FUNC_ENABLE);
    AlUart_ll_SetTxFifoThr(Uart->BaseAddr, UART_TxFIFO_EMPTY);
    AlUart_ll_SetRxFifoThr(Uart->BaseAddr, UART_RxFIFO_FULL_2);

    /* step 5. set IER register to enable interrupts */
    AlUart_ll_SetLineIntr(Uart->BaseAddr, AL_FUNC_ENABLE);
    AlUart_ll_SetThreIntr(Uart->BaseAddr, AL_FUNC_ENABLE);
    AlUart_ll_SetTxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);
    AlUart_ll_SetRxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);

    /* step 6. All things completed, write to the divisor registers DLL and DLH to set the bit rate*/
    AlUart_ll_SetBaudRate(Uart->BaseAddr, InitConfig->BaudRate, Uart->InputClockHz);

    Uart->State |= AL_UART_STATE_READY;

    return AL_OK;
}

/**
 * This function send an amount of data in interrupt mode.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   SendData Pointer to data buffer
 * @param   SendSize Amount of data to be sent
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Dev_SendData(AL_UART_DevStruct *Uart, AL_U8 *SendData, AL_U32 SendSize)
{
#ifdef UART_DEBUG
    AL_LOG(AL_LOG_LEVEL_DEBUG, "uart=%p SendDataAddr=%p SendSize=%d\n", Uart, SendData, SendSize);
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

    AlUart_Dev_SetTxBusy(Uart);

    Uart->SendBuffer.BufferPtr      = SendData;
    Uart->SendBuffer.RequestedCnt   = SendSize;
    Uart->SendBuffer.HandledCnt     = 0;

    AlUart_Dev_SetIntr(Uart, UART_TX, AL_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function receive an amount of data in interrupt mode.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be received
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
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
        ReceivedCount ++;
    }

    /* Setup the buffer parameters */
    if (ReceivedCount < ReceiveSize) {
        Uart->RecvBuffer.RequestedCnt   = ReceiveSize;
        Uart->RecvBuffer.HandledCnt     = ReceivedCount;
        Uart->RecvBuffer.BufferPtr      = ReceiveBuf;

        AlUart_Dev_SetIntr(Uart, UART_TX, AL_FUNC_ENABLE);
    } else {
        if (Uart->EventCallBack) {
            AL_UART_EventStruct UartEvent = {
                .Events        = AL_UART_EVENT_RECEIVE_DONE,
                .EventData    = Uart->RecvBuffer.HandledCnt
            };
            Uart->EventCallBack(UartEvent, Uart->EventCallBackRef);
        }
        AlUart_Dev_ClrRxBusy(Uart);
    }
    return AL_OK;
}

/**
 * This function register a User UART Callback To be used when send or receive done.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   CallBack pointer to the Callback function
 * @param   CallbackRef pointer to the Callback function params
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Dev_RegisterEventCallBack(AL_UART_DevStruct *Uart, AL_Uart_EventCallBack Callback, void *CallbackRef)
{
    if (Uart == AL_NULL || Callback == AL_NULL) {
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    if (Uart->EventCallBack != AL_NULL) {

#ifdef UART_DEBUG
        AL_LOG(AL_LOG_LEVEL_WARNING, "uart=%p duplicate register callback: replace old:%p with New: %p\n", \
                Uart, Uart->EventCallBack, Callback);
#endif
    }

    Uart->EventCallBack        = Callback;
    Uart->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 * This function unregister a User UART Callback To be used when send or receive done.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Dev_UnRegisterEventCallBack(AL_UART_DevStruct *Uart)
{
    if (Uart == AL_NULL) {
#ifdef UART_DEBUG
        AL_LOG(AL_LOG_LEVEL_ERROR, "uart=%p duplicate register callback: replace old:%p with New: %p\n", \
            Uart, Uart->EventCallBack, Callback);
#endif
        return AL_UART_ERR_ILLEGAL_PARAM;
    }

    Uart->EventCallBack = (AL_Uart_EventCallBack)AL_NULL;

    return AL_OK;
}

/**
 * This function receive data interrupt handler.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   Timeout Timeout duration
 * @return
 * @note
*/
static AL_VOID AlUart_Dev_RecvDataHandler(AL_UART_DevStruct *Uart, AL_BOOL CharTimeoutIntr)
{
    AL_UART_EventStruct UartEvent = {
        .EventData = 0,
        .Events    = 0,
    };

    while ((AlUart_ll_IsRxDataReady(Uart->BaseAddr) == AL_TRUE) &&
           (Uart->RecvBuffer.HandledCnt < Uart->RecvBuffer.RequestedCnt)) {
        Uart->RecvBuffer.BufferPtr[Uart->RecvBuffer.HandledCnt] = (AL_U8)AlUart_ll_RecvByte(Uart->BaseAddr);
        Uart->RecvBuffer.HandledCnt ++;
    }

    if ((Uart->RecvBuffer.HandledCnt == Uart->RecvBuffer.RequestedCnt) || (CharTimeoutIntr && Uart->Configs.CharTimeoutEnable)) {
        UartEvent.Events = ((Uart->RecvBuffer.HandledCnt == Uart->RecvBuffer.RequestedCnt)) ?
                           AL_UART_EVENT_RECEIVE_DONE : AL_UART_EVENT_CHAR_TIMEOUT;
        UartEvent.EventData = Uart->RecvBuffer.HandledCnt;

        //stop
        AlUart_ll_SetRxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);
        if (Uart->EventCallBack) {
            Uart->EventCallBack(UartEvent, Uart->EventCallBackRef);
        }
        AlUart_Dev_ClrRxBusy(Uart);
    }
}


/**
 * This function send data interrupt handler.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @return
 * @note
*/
static AL_VOID AlUart_Dev_SendDataHandler(AL_UART_DevStruct *Uart)
{
    if (Uart->SendBuffer.HandledCnt == Uart->SendBuffer.RequestedCnt) {
        AlUart_ll_SetTxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);

        if (Uart->EventCallBack) {
            AL_UART_EventStruct UartEvent = {
                .Events        = AL_UART_EVENT_SEND_DONE,
                .EventData     = Uart->SendBuffer.HandledCnt
            };
            (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
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
 * This function send an amount of data in polling mode.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be sent
 * @return
 *          - AL_OK for function success
 * @note
*/
AL_S32 AlUart_Dev_SendDataPolling(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 Size)
{
    AL_U32 HandledCnt = 0;

    if (AlUart_Dev_IsTxBusy(Uart)){
        return AL_UART_ERR_BUSY;
    }

    AlUart_Dev_SetTxBusy(Uart);

    while (HandledCnt < Size) {
        if (!(AlUart_ll_IsTransmitFifoFull(Uart->BaseAddr))) {
            AlUart_ll_SendByte(Uart->BaseAddr, Data[HandledCnt]);
            HandledCnt ++;
        }
    }

    /* Waiting for transmitter Shift Register and the FIFO both empty. */
    while (!(AlUart_ll_IsTxDone(Uart->BaseAddr)));

    AlUart_Dev_ClrTxBusy(Uart);

    return AL_OK;
}

AL_S32 AlUart_Dev_RecvDataPolling(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 Size)
{
    AL_U32 HandledCnt = 0;

    if (AlUart_Dev_IsRxBusy(Uart)){
        return AL_UART_ERR_BUSY;
    }

    AlUart_Dev_SetRxBusy(Uart);

    while (HandledCnt < Size) {
        if ((AlUart_ll_IsRxDataReady(Uart->BaseAddr))) {
            Data[HandledCnt] = AlUart_ll_RecvByte(Uart->BaseAddr);
            HandledCnt ++;
        }
    }
    AlUart_Dev_ClrRxBusy(Uart);

    return AL_OK;
}

/**
 * This function is error interrupt handler.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   IntrStatus is interrupt register status
 * @return
 * @note
*/


#define AL_UART_OVERRUN_ERR    1
#define AL_UART_PARITY_ERR     2
#define AL_UART_FRAMING_ERR    3
#define AL_UART_BREAK_ERR      4
static AL_VOID AlUart_Dev_ErrorHandler(AL_UART_DevStruct *Uart, AL_UART_InterruptEnum IntrStatus)
{
    AL_UART_EventStruct UartEvent = {
        .Events     = 0,
    };

    AL_U32 ErrorStatus = AlUart_ll_GetLineRegStatus(Uart->BaseAddr);

    if(ErrorStatus & (1 << AL_UART_OVERRUN_ERR)) {
        while ((AlUart_ll_IsRxDataReady(Uart->BaseAddr) == AL_TRUE) &&
            (Uart->RecvBuffer.HandledCnt < Uart->RecvBuffer.RequestedCnt)) {
                Uart->RecvBuffer.BufferPtr[Uart->RecvBuffer.HandledCnt] = (AL_U8)AlUart_ll_RecvByte(Uart->BaseAddr);
                Uart->RecvBuffer.HandledCnt ++;
        }
        UartEvent.Events   |= AL_UART_EVENT_OVER_RUN_ERR;
    }

    if (ErrorStatus & (1 << AL_UART_PARITY_ERR)) {
        UartEvent.Events |= AL_UART_EVENT_PARITY_ERR;
    }
    if (ErrorStatus & (1 << AL_UART_FRAMING_ERR)) {
        UartEvent.Events |= AL_UART_EVENT_FRAMING_ERR;
    }

    if (ErrorStatus & (1 << AL_UART_BREAK_ERR)) {
        UartEvent.Events |= AL_UART_EVENT_BREAK_INTR;
    }

    UartEvent.EventData = Uart->RecvBuffer.HandledCnt;

    /* trigger EventCallBack */
    if (Uart->EventCallBack) {
        (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
    }

    //stop
    AlUart_ll_SetRxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);
    AlUart_Dev_ClrRxBusy(Uart);
}

/**
 * This function is bus busy interrupt handler.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @return
 * @note    Busy detect; Busy functionality helps to safe guard against errors if the LCR, DLL, and/or DLH registers
 *          are changed during a transaction even though they should only be set during initialization
*/
static AL_VOID AlUart_Dev_BusBusyHandler(AL_UART_DevStruct *Uart)
{
    AL_UART_EventStruct UartEvent = {
        .Events     = 0,
        .EventData  = 0,
    };

#ifdef UART_DEBUG
    AL_LOG(AL_LOG_LEVEL_DEBUG,"uart bus busy, bus reset then reinitialize\r\n");
#endif
    /* soft reset uart bus */
    if (Uart->BaseAddr == UART0__BASE_ADDR) {
        AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART0__SRST__N__SHIFT, AL_FUNC_DISABLE);
        AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART0__SRST__N__SHIFT, AL_FUNC_ENABLE);
    } else {
        AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART1__SRST__N__SHIFT, AL_FUNC_DISABLE);
        AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART1__SRST__N__SHIFT, AL_FUNC_ENABLE);
    }

    /* Reinitialize with the original configuration of the uart port in question
    * Fixme, not change uart status here
    */
    AlUart_Dev_Init(Uart, &(Uart->Configs), Uart->DevId);

    if (AlUart_Dev_IsRxBusy(Uart)) {
        UartEvent.Events    = AL_UART_EVENT_BUSY_DETECT_RX;
        UartEvent.EventData = Uart->RecvBuffer.HandledCnt;

    }

    if (AlUart_Dev_IsTxBusy(Uart)) {
        UartEvent.Events    = AL_UART_EVENT_BUSY_DETECT_TX;
        UartEvent.EventData = Uart->SendBuffer.HandledCnt;

    }

    if (UartEvent.Events != 0 && Uart->EventCallBack) {
        (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
    }
}

static AL_VOID AlUart_Dev_NoInterruptPeding(AL_UART_DevStruct *Uart)
{
    AL_UART_EventStruct UartEvent = {
        .Events = AL_UART_EVENT_NO_INTR_PEDING
    };
    if (Uart->EventCallBack) {
        (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
    }
}

static AL_VOID AlUart_Dev_ModemStatusIntr(AL_UART_DevStruct *Uart)
{
    AL_UART_EventStruct UartEvent = {
        .Events = AL_UART_EVENT_MODEM_STATUS_INTR,
    };
    if (Uart->EventCallBack) {
        (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
    }
}

#define UART_IN_MODEM_STATUS_INTR(Status)           (Status == UART_MODEM_STATUS)
#define UART_IN_NO_INTERRUPT_PEDING(Status)         (Status == UART_NO_INTERRUPT_PENDING)
#define UART_IN_THRE_EMPTY_INTR(Status)             (Status == UART_THR_EMPTY)
#define UART_IN_RECV_AVAIL_INTR(Status)             (Status == UART_RECEIVED_DATA_AVAILABLE)
#define UART_IN_RECV_LINE_STATUS_INTR(Status)       (Status == UART_RECEIVER_LINE_STATUS)
#define UART_IN_BUSY_DETECT_INTR(Status)            (Status == UART_BUSY_DETECT)
#define UART_IN_CHAR_RECEV_TIMEOUT(Status)          (Status == UART_CHARACTER_TIMEOUT)

/**
 * This function is uart's all interrupt entries handler.
 * @param   Instance is pointer to interrupts call back reference
 * @return
 * @note
*/
AL_VOID AlUart_Dev_IntrHandler(AL_VOID *Instance)
{
    AL_UART_DevStruct *Uart = (AL_UART_DevStruct *)Instance;
    AL_UART_InterruptEnum IntrStatus = AlUart_ll_GetIntrStatus(Uart->BaseAddr);

    if (UART_IN_RECV_AVAIL_INTR(IntrStatus) || UART_IN_CHAR_RECEV_TIMEOUT(IntrStatus)) {
        AlUart_Dev_RecvDataHandler(Uart, UART_IN_CHAR_RECEV_TIMEOUT(IntrStatus));
    } else if (UART_IN_THRE_EMPTY_INTR(IntrStatus)) {
        AlUart_Dev_SendDataHandler(Uart);
    } else if (UART_IN_RECV_LINE_STATUS_INTR(IntrStatus)) {
        AlUart_Dev_ErrorHandler(Uart, IntrStatus);
    } else if (UART_IN_BUSY_DETECT_INTR(IntrStatus)) {
        AlUart_Dev_BusBusyHandler(Uart);
    } else if (UART_IN_NO_INTERRUPT_PEDING(IntrStatus)) {
        AlUart_Dev_NoInterruptPeding(Uart);
    } else if (UART_IN_MODEM_STATUS_INTR(IntrStatus)) {
        AlUart_Dev_ModemStatusIntr(Uart);
    }
}


static AL_VOID AlUart_Dev_SetIntr(AL_UART_DevStruct *Uart, AL_Uart_TransferEnum Transfer,  AL_FUNCTION State)
{
    AL_BOOL IsRxBusy = AlUart_Dev_IsRxBusy(Uart);
    AL_BOOL IsTxBusy = AlUart_Dev_IsTxBusy(Uart);

    Al_OSAL_EnterDevCtritical(Uart->IntrNum, (IsRxBusy && IsTxBusy));

    (Transfer == UART_TX) ? AlUart_ll_SetTxIntr(Uart->BaseAddr, State) :
                            AlUart_ll_SetRxIntr(Uart->BaseAddr, State);

    Al_OSAL_ExitDevCtritical(Uart->IntrNum, IsRxBusy && IsTxBusy);
}

AL_VOID AlUart_Dev_StopSend(AL_UART_DevStruct *Uart)
{
    AlUart_Dev_SetIntr(Uart, UART_TX, AL_FUNC_DISABLE);
    AlUart_Dev_ClrTxBusy(Uart);
}

AL_VOID AlUart_Dev_StopReceive(AL_UART_DevStruct *Uart)
{
    AlUart_Dev_SetIntr(Uart, UART_RX, AL_FUNC_DISABLE);
    AlUart_Dev_ClrRxBusy(Uart);
}

/**
 * This function excute operations to set or get uart status
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   Cmd is io ctl operation to AL_Uart_IoCtlCmdEnum
 * @param   Data is pointer reference to Cmd
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Dev_IoCtl(AL_UART_DevStruct *Uart, AL_Uart_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Uart == AL_NULL) {
        return AL_UART_ERR_NULL_PTR;
    }

    if(AlUart_ll_IsUartBusy(Uart->BaseAddr)){
        AL_LOG(AL_LOG_LEVEL_ERROR, "access during a transaction\r\n");
        return AL_UART_ERR_BUSY;
    }

    switch (Cmd)
    {
    case AL_UART_IOCTL_SET_BAUD_RATE: {
        AL_U32 BaudRate = *(AL_U32 *)Data;
        AlUart_ll_SetBaudRate(Uart->BaseAddr, BaudRate, Uart->InputClockHz);
        break;
    }
    case AL_UART_IOCTL_GET_BAUD_RATE: {
        AL_U32 *BaudRate = (AL_U32 *)Data;
        *BaudRate = AlUart_ll_GetBaudRate(Uart->BaseAddr, Uart->InputClockHz);
        break;
    }
    case AL_UART_IOCTL_SET_DATA_WIDTH: {
        AL_U32 DataWidth = *(AL_U32 *)Data;
        AlUart_ll_SetDataWidth(Uart->BaseAddr, DataWidth);
        break;
    }
    case AL_UART_IOCTL_GET_DATA_WIDTH: {
        AL_U32 *DataWidth = (AL_U32 *)Data;
        *DataWidth = AlUart_ll_GetDataWidth(Uart->BaseAddr);
        break;
    }
    case AL_UART_IOCTL_SET_STOP_BITS: {
        AL_U32 StopBits = *(AL_U32 *)Data;
        AlUart_ll_SetStopBitsLength(Uart->BaseAddr, StopBits);
        break;
    }
    case AL_UART_IOCTL_GET_STOP_BITS: {
        AL_U32 *StopBits = (AL_U32 *)Data;
        *StopBits = AlUart_ll_GetStopBitsLength(Uart->BaseAddr);
        break;
    }
    case AL_UART_IOCTL_SET_PARITY: {
        AL_U32 Parity = *(AL_U32 *)Data;
        AlUart_ll_SetParity(Uart->BaseAddr, Parity);
        break;
    }
    case AL_UART_IOCTL_GET_PARITY: {
        AL_U32 *Parity = (AL_U32 *)Data;
        *Parity = AlUart_ll_GetParity(Uart->BaseAddr);
        break;
    }
    case AL_UART_IOCTL_SET_AUTO_FLOW_CTL: {
        AL_BOOL state = *(AL_U32 *)Data;
        AlUart_ll_SetAutoFlowCtl(Uart->BaseAddr, state);
    break;
    }
    case AL_UART_IOCTL_SET_LOOPBACK: {
        AL_BOOL LoopBack = *(AL_U32 *)Data;
        AlUart_ll_Set_LoopBack(Uart->BaseAddr, LoopBack);
    break;
    }
    default:
        return AL_UART_ERR_NOT_SUPPORT;
        break;
    }

    return Ret;
}
