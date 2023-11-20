/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_uart_dev.h"
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/

#define IS_SAME_INITCONFIGS(Dest, Src)                                                  \
(                                                                                  \
    (Dest).BaudRate == (Src).BaudRate && (Dest).WordLength == (Src).WordLength          \
    && (Dest).StopBits == (Src).StopBits && (Dest).Parity == (Src).Parity               \
    && (Dest).HwFlowCtl == (Src).HwFlowCtl && (Dest).CharTimeoutEnable == (Src).CharTimeoutEnable    \
)

/************************** Variable Definitions *****************************/
static AL_UART_InitStruct UartDefInitConfigs = {
    .BaudRate           = 115200,
    .Parity             = AL_UART_NO_PARITY,
    .WordLength         = AL_UART_CHAR_8BITS,
    .StopBits           = AL_UART_STOP_1BIT,
    .HwFlowCtl          = AL_FALSE,
    .CharTimeoutEnable  = AL_TRUE
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
AL_S32 AlUart_Dev_Init(AL_UART_DevStruct *Uart, AL_U32 DevId, AL_UART_InitStruct *InitConfig)
{
    AL_UART_HwConfigStruct *UartHwConfig = AL_NULL;

    AL_ASSERT((Uart != AL_NULL && DevId < AL_UART_NUM_INSTANCE), AL_UART_ERR_ILLEGAL_PARAM);

    if (Uart->State & AL_UART_STATE_READY) {
        if (InitConfig == AL_NULL || IS_SAME_INITCONFIGS(Uart->Configs, *InitConfig)) {
            return AL_OK;
        } else {
            /* soft reset uart bus */
            if(Uart->State != AL_UART_STATE_NOT_INIT) {
                if (Uart->BaseAddr == UART0__BASE_ADDR) {
                    AlUart_ll_ResetUart0Bus();
                } else {
                    AlUart_ll_ResetUart1Bus();
                }
            }
            Uart->Configs = *InitConfig;
        }
    } else {
        Uart->Configs = (InitConfig == AL_NULL) ? UartDefInitConfigs : (*InitConfig);
        if (DevId == 0) {
            #ifdef UART0_BAUDRATE
                Uart->Configs.BaudRate = UART0_BAUDRATE;
            #endif
        }
        if (DevId == 1) {
            #ifdef UART1_BAUDRATE
                Uart->Configs.BaudRate = UART1_BAUDRATE;
            #endif
        }
    }

    UartHwConfig = AlUart_Dev_LookupConfig(DevId);
    Uart->DevId        = DevId;
    Uart->BaseAddr     = UartHwConfig->BaseAddress;
    Uart->IntrNum      = UartHwConfig->IntrNum;
    Uart->InputClockHz = UartHwConfig->InputClockHz;

    /* Initialize UART */
    AlUart_ll_DisableAllIntr(Uart->BaseAddr);
    AlUart_ll_ResetTxFifo(Uart->BaseAddr);
    AlUart_ll_ResetRxFifo(Uart->BaseAddr);

    AlUart_ll_ResetDllDlhReg(Uart->BaseAddr);

    if (AlUart_ll_IsUartBusy(Uart->BaseAddr)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Al uart cannot set baudrate written while the UART is busy");
    } else {
        AlUart_ll_SetBaudRate(Uart->BaseAddr, Uart->Configs.BaudRate, Uart->InputClockHz);
    }

    if (AlUart_ll_IsUartBusy(Uart->BaseAddr)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Al uart cannot set line control written while the UART is busy");
    } else {
        AlUart_ll_SetDataWidth(Uart->BaseAddr, Uart->Configs.WordLength);
        AlUart_ll_SetParity(Uart->BaseAddr, Uart->Configs.Parity);
        AlUart_ll_SetStopBitsLength(Uart->BaseAddr, Uart->Configs.StopBits);
    }

    AlUart_ll_EnableFifo(Uart->BaseAddr, AL_TRUE);
    AlUart_ll_SetTxFifoThre(Uart->BaseAddr, AL_UART_TxFIFO_HALF_FULL);
    AlUart_ll_SetRxFifoThre(Uart->BaseAddr, AL_UART_RxFIFO_HALF_FULL);

    /* Enable programmable THRE mode*/
    AlUart_ll_EnableThreIntr(Uart->BaseAddr, AL_TRUE);

    /* Enable Rx Line Interrupt*/
    AlUart_ll_EnableLineIntr(Uart->BaseAddr, AL_TRUE);

    /* Disable Tx and Rx interrupt*/
    AlUart_ll_SetTxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);
    AlUart_ll_SetRxIntr(Uart->BaseAddr, AL_FUNC_DISABLE);

    /* Set AutoFlowControl */
    if (Uart->Configs.HwFlowCtl) {
        AlUart_ll_SetAutoFlowCtl(Uart->BaseAddr, AL_TRUE);
    }

    Uart->State |= AL_UART_STATE_READY;

    return AL_OK;
}

AL_VOID AlUart_Dev_SendByte(AL_UART_DevStruct *Uart, AL_S8 Char)
{
    while (AlUart_ll_IsTxFifoFull(Uart->BaseAddr));

    AlUart_ll_SendByte(Uart->BaseAddr, Char);
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
        if (!(AlUart_ll_IsTxFifoFull(Uart->BaseAddr))) {
            AlUart_ll_SendByte(Uart->BaseAddr, Data[HandledCnt]);
            HandledCnt ++;
        }
    }

    /* Waiting for transmitter Shift Register and the FIFO both empty. */
    while (!(AlUart_ll_IsTxDone(Uart->BaseAddr)));

    AlUart_Dev_ClrTxBusy(Uart);

    return AL_OK;
}

/**
 * This function recv an amount of data in polling mode.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   Data Pointer to data buffer
 * @param   Size Amount of data to be recv
 * @return
 *          - AL_OK for function success
 * @note
*/
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
    AL_ASSERT((Uart != AL_NULL && SendData != AL_NULL && SendSize != 0), AL_UART_ERR_ILLEGAL_PARAM);
    AL_ASSERT(((Uart->State & AL_UART_STATE_READY) == AL_UART_STATE_READY), AL_UART_ERR_NOT_READY);
    AL_ASSERT((!AlUart_Dev_IsTxBusy(Uart)), AL_UART_ERR_BUSY);

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

    AL_ASSERT((Uart != AL_NULL && ReceiveBuf != AL_NULL && ReceiveSize != 0), AL_UART_ERR_ILLEGAL_PARAM);
    AL_ASSERT(((Uart->State & AL_UART_STATE_READY) == AL_UART_STATE_READY), AL_UART_ERR_NOT_READY);
    AL_ASSERT((!AlUart_Dev_IsRxBusy(Uart)), AL_UART_ERR_BUSY);

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

        AlUart_Dev_SetIntr(Uart, UART_RX, AL_FUNC_ENABLE);
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
AL_S32 AlUart_Dev_RegisterEventCallBack(AL_UART_DevStruct *Uart, AL_Uart_EventCallBack Callback, AL_VOID *CallbackRef)
{
    AL_ASSERT((Uart != AL_NULL && Callback != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

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
    AL_ASSERT((Uart != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

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
        UartEvent.Events = ((Uart->RecvBuffer.HandledCnt == Uart->RecvBuffer.RequestedCnt) ?
                             AL_UART_EVENT_RECEIVE_DONE : AL_UART_EVENT_CHAR_TIMEOUT);
        UartEvent.EventData = Uart->RecvBuffer.HandledCnt;

        AlUart_Dev_StopReceive(Uart);

        if (Uart->EventCallBack) {
            Uart->EventCallBack(UartEvent, Uart->EventCallBackRef);
        }
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

        AlUart_Dev_StopSend(Uart);

        if (Uart->EventCallBack) {
            AL_UART_EventStruct UartEvent = {
                .Events        = AL_UART_EVENT_SEND_DONE,
                .EventData     = Uart->SendBuffer.HandledCnt
            };
            (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
        }

    } else {
        while (!(AlUart_ll_IsTxFifoFull(Uart->BaseAddr)) &&
            (Uart->SendBuffer.HandledCnt < Uart->SendBuffer.RequestedCnt)) {
            AlUart_ll_SendByte(Uart->BaseAddr, Uart->SendBuffer.BufferPtr[Uart->SendBuffer.HandledCnt]);
            Uart->SendBuffer.HandledCnt ++;
        }
    }
}

/**
 * This function is error interrupt handler.
 * @param   Uart Pointer to a AL_UART_DevStruct structure that contains uart device instance
 * @param   IntrStatus is interrupt register status
 * @return
 * @note
*/
static AL_VOID AlUart_Dev_RecvErrorHandler(AL_UART_DevStruct *Uart, AL_UART_IntrEnum IntrStatus)
{
    AL_UART_EventStruct UartEvent = {
        .Events     = 0,
        .EventData  = 0,
    };

    AL_U32 ErrorStatus = AlUart_ll_GetLineRegStatus(Uart->BaseAddr);

    if(ErrorStatus & (AL_UART_OVERRUN_ERR)) {
        while ((AlUart_ll_IsRxDataReady(Uart->BaseAddr) == AL_TRUE) &&
            (Uart->RecvBuffer.HandledCnt < Uart->RecvBuffer.RequestedCnt)) {
                Uart->RecvBuffer.BufferPtr[Uart->RecvBuffer.HandledCnt] = (AL_U8)AlUart_ll_RecvByte(Uart->BaseAddr);
                Uart->RecvBuffer.HandledCnt ++;
        }
        UartEvent.Events |= AL_UART_EVENT_OVER_RUN_ERR;
    }

    if (ErrorStatus & (AL_UART_PARITY_ERR)) {
        UartEvent.Events |= AL_UART_EVENT_PARITY_ERR;
    }
    if (ErrorStatus & (AL_UART_FRAMING_ERR)) {
        UartEvent.Events |= AL_UART_EVENT_FRAMING_ERR;
    }
    if (ErrorStatus & (AL_UART_BREAK_ERR)) {
        UartEvent.Events |= AL_UART_EVENT_BREAK_INTR;
    }

    AlUart_Dev_StopReceive(Uart);

    UartEvent.EventData = Uart->RecvBuffer.HandledCnt;
    if (Uart->EventCallBack) {
        (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
    }


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

    /* soft reset uart bus */
    if (Uart->BaseAddr == UART0__BASE_ADDR) {
        AlUart_ll_ResetUart0Bus();
    } else {
        AlUart_ll_ResetUart1Bus();
    }

    /* Reinitialize with the original configuration of the uart port in question */
    AlUart_Dev_Init(Uart, Uart->DevId, &(Uart->Configs));

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

#define AL_UART_INTR_MODEM_STATUS(Status)           (Status == AL_UART_MODEM_STATUS)
#define AL_UART_NO_INTER_PEDING(Status)             (Status == AL_UART_NO_INTERRUPT_PENDING)
#define AL_UART_INTR_THRE_EMPTY(Status)             (Status == AL_UART_THR_EMPTY)
#define AL_UART_INTR_RECV_AVAIL(Status)             (Status == AL_UART_RECEIVED_DATA_AVAILABLE)
#define AL_UART_INTR_RECV_LINE_STATUS(Status)       (Status == AL_UART_RECEIVER_LINE_STATUS)
#define AL_UART_INTR_BUSY_DETECT(Status)            (Status == AL_UART_BUSY_DETECT)
#define AL_UART_INTR_CHAR_RECEV_TIMEOUT(Status)     (Status == AL_UART_CHARACTER_TIMEOUT)

/**
 * This function is uart's all interrupt entries handler.
 * @param   Instance is pointer to interrupts call back reference
 * @return
 * @note
*/
AL_VOID AlUart_Dev_IntrHandler(AL_VOID *Instance)
{
    AL_UART_DevStruct *Uart = (AL_UART_DevStruct *)Instance;
    AL_UART_IntrEnum IntrStatus = AlUart_ll_GetIntrStatus(Uart->BaseAddr);

    if (AL_UART_INTR_RECV_AVAIL(IntrStatus) || AL_UART_INTR_CHAR_RECEV_TIMEOUT(IntrStatus)) {
        AlUart_Dev_RecvDataHandler(Uart, AL_UART_INTR_CHAR_RECEV_TIMEOUT(IntrStatus));
    } else if (AL_UART_INTR_THRE_EMPTY(IntrStatus)) {
        AlUart_Dev_SendDataHandler(Uart);
    } else if (AL_UART_INTR_RECV_LINE_STATUS(IntrStatus)) {
        AlUart_Dev_RecvErrorHandler(Uart, IntrStatus);
    } else if (AL_UART_INTR_BUSY_DETECT(IntrStatus)) {
        AlUart_Dev_BusBusyHandler(Uart);
    } else if (AL_UART_NO_INTER_PEDING(IntrStatus)) {
        AlUart_Dev_NoInterruptPeding(Uart);
    } else if (AL_UART_INTR_MODEM_STATUS(IntrStatus)) {
        AlUart_Dev_ModemStatusIntr(Uart);
    }
}


static AL_VOID AlUart_Dev_SetIntr(AL_UART_DevStruct *Uart, AL_Uart_TransferEnum Transfer,  AL_FUNCTION State)
{
    AL_BOOL IsRxBusy = AlUart_Dev_IsRxBusy(Uart);
    AL_BOOL IsTxBusy = AlUart_Dev_IsTxBusy(Uart);

    AlOsal_EnterDevCtritical(Uart->IntrNum, (IsRxBusy && IsTxBusy));

    (Transfer == UART_TX) ? AlUart_ll_SetTxIntr(Uart->BaseAddr, State) :
                            AlUart_ll_SetRxIntr(Uart->BaseAddr, State);

    AlOsal_ExitDevCtritical(Uart->IntrNum, IsRxBusy && IsTxBusy);
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
 * @param   Cmd is io ctl operation to AL_UART_IoCtlCmdEnum
 * @param   Data is pointer reference to Cmd
 * @return
 *          - AL_OK for function success
 *          - Other for function failure
 * @note
*/
AL_S32 AlUart_Dev_IoCtl(AL_UART_DevStruct *Uart, AL_UART_IoCtlCmdEnum Cmd, AL_UART_IoctlParamUnion *IoctlParam)
{
    AL_S32 Ret = AL_OK;

    AL_ASSERT((Uart != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    if(AlUart_ll_IsUartBusy(Uart->BaseAddr)){
        AL_LOG(AL_LOG_LEVEL_ERROR, "access during a transaction\r\n");
        return AL_UART_ERR_BUSY;
    }

    switch (Cmd)
    {
    case AL_UART_IOCTL_SET_BAUD_RATE: {
        AL_U32 BaudRate = IoctlParam->BaudRate;
        AlUart_ll_SetBaudRate(Uart->BaseAddr, BaudRate, Uart->InputClockHz);
        break;
    }
    case AL_UART_IOCTL_GET_BAUD_RATE: {
        AL_U32 BaudRate = AlUart_ll_GetBaudRate(Uart->BaseAddr, Uart->InputClockHz);
        IoctlParam->BaudRate = BaudRate;
        break;
    }
    case AL_UART_IOCTL_SET_DATA_WIDTH: {
        AL_U32 DataWidth = IoctlParam->DataWidth;
        AlUart_ll_SetDataWidth(Uart->BaseAddr, DataWidth);
        break;
    }
    case AL_UART_IOCTL_GET_DATA_WIDTH: {
        AL_U32 DataWidth = AlUart_ll_GetDataWidth(Uart->BaseAddr);
        IoctlParam->DataWidth = DataWidth;
        break;
    }
    case AL_UART_IOCTL_SET_STOP_BITS: {
        AL_U32 StopBits = IoctlParam->StopBits;
        AlUart_ll_SetStopBitsLength(Uart->BaseAddr, StopBits);
        break;
    }
    case AL_UART_IOCTL_GET_STOP_BITS: {
        AL_U32 StopBits = AlUart_ll_GetStopBitsLength(Uart->BaseAddr);
        IoctlParam->StopBits = StopBits;
        break;
    }
    case AL_UART_IOCTL_SET_PARITY: {
        AL_U32 Parity = IoctlParam->Parity;
        AlUart_ll_SetParity(Uart->BaseAddr, Parity);
        break;
    }
    case AL_UART_IOCTL_GET_PARITY: {
        AL_U32 Parity = AlUart_ll_GetParity(Uart->BaseAddr);
        IoctlParam->Parity = Parity;
        break;
    }
    case AL_UART_IOCTL_SET_AUTO_FLOW_CTL: {
        AL_BOOL AutoFlowState = IoctlParam->AutoFlowState;
        AlUart_ll_SetAutoFlowCtl(Uart->BaseAddr, AutoFlowState);
    break;
    }
    case AL_UART_IOCTL_SET_LOOPBACK: {
        AL_BOOL LoopBack = IoctlParam->LoopBack;
        AlUart_ll_Set_LoopBack(Uart->BaseAddr, LoopBack);
    break;
    }
    default:
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_UAER_ERR_IOCTL_CMD");
        break;
    }

    return Ret;
}
