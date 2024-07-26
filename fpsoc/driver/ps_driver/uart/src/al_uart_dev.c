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
 * This function looks up the hardware configuration for a given UART device ID.
 *
 * @param DevId The device ID for which to lookup the configuration.
 * @return A pointer to the hardware configuration structure if found, NULL otherwise.
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
 * This function checks if the UART device's transmit operation is busy.
 *
 * @param Uart Pointer to the UART device structure.
 * @return True if the transmit operation is busy, false otherwise.
 */
AL_BOOL AlUart_Dev_IsTxBusy(AL_UART_DevStruct *Uart)
{
    return (AL_BOOL)(Uart->State & AL_UART_STATE_TX_BUSY);
}

/**
 * This function checks if the UART device's receive operation is busy.
 *
 * @param Uart Pointer to the UART device structure.
 * @return True if the receive operation is busy, false otherwise.
 */
AL_BOOL AlUart_Dev_IsRxBusy(AL_UART_DevStruct *Uart)
{
    return (AL_BOOL)(Uart->State & AL_UART_STATE_RX_BUSY);
}

/**
 * This function sets the UART device's transmit operation as busy.
 *
 * @param Uart Pointer to the UART device structure.
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
 * This function sets the UART device's receive operation as busy.
 *
 * @param Uart Pointer to the UART device structure.
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
 * This function clears the UART device's transmit operation from being busy.
 *
 * @param Uart Pointer to the UART device structure.
 */
AL_VOID AlUart_Dev_ClrTxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State &= (~AL_UART_STATE_TX_BUSY);
}

/**
 * This function clears the UART device's receive operation from being busy.
 *
 * @param Uart Pointer to the UART device structure.
 */
AL_VOID AlUart_Dev_ClrRxBusy(AL_UART_DevStruct *Uart)
{
    Uart->State &= (~AL_UART_STATE_RX_BUSY);
}

/**
 * This function sets the baud rate for the UART device.
 *
 * @param Uart Pointer to the UART device structure.
 * @param BaudRate The baud rate to set.
 * @param InputClockHz The input clock frequency in Hz.
 */
AL_VOID AlUart_Dev_SetBaudRate(AL_UART_DevStruct *Uart, AL_U32 BaudRate, AL_U32 InputClockHz)
{
    AL_FLOAT DllFloat = 0.0f;
    AL_FLOAT DlhFloat = 0.0f;
    AL_U8 DllInt = 0;
    AL_U8 DlhInt = 0;

    DllFloat = ((AL_FLOAT)(InputClockHz >> 4) / (AL_FLOAT)BaudRate);
    DllInt = (AL_U8)DllFloat;

    if (DllFloat - DllInt > 0.5) {
        DllInt++;
    }

    DlhInt = (AL_U8)(((InputClockHz >> 4) / BaudRate) >> 8);

    AlUart_ll_SetBaudRate(Uart->BaseAddr, DllInt, DlhInt);
}

/**
 * This function initializes the UART device with specified configurations.
 *
 * @param Uart Pointer to the UART device structure to initialize.
 * @param DevId The device ID of the UART.
 * @param InitConfig Pointer to the initialization configuration structure.
 * @return AL_OK if successful, error code otherwise.
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

    AlUart_ll_DisableAllIntr(Uart->BaseAddr);
    AlUart_ll_ResetTxFifo(Uart->BaseAddr);
    AlUart_ll_ResetRxFifo(Uart->BaseAddr);

    /* Waiting for uart to be idle */
    while (AlUart_ll_IsUartBusy(Uart->BaseAddr));

    if (AlUart_ll_IsUartBusy(Uart->BaseAddr)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "Al uart cannot set baudrate written while the UART is busy");
    } else {
        AlUart_Dev_SetBaudRate(Uart, Uart->Configs.BaudRate, Uart->InputClockHz);
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

    if (Uart->Configs.HwFlowCtl) {
        AlUart_ll_SetAutoFlowCtl(Uart->BaseAddr, AL_TRUE);
    }

    Uart->State |= AL_UART_STATE_READY;

    return AL_OK;
}

/**
 * This function checks if the UART device's transmit FIFO is full.
 *
 * @param Uart Pointer to the UART device structure.
 * @return True if the transmit FIFO is full, false otherwise.
 */
AL_BOOL AlUart_Dev_IsTxFifoFull(AL_UART_DevStruct *Uart)
{
    /* If the FIFOs is enabled and 'THRE' mode is turned on,
     * this register indicates that TX FIFO is full,
     * otherwise it indicates that THR or TX FIFO is empty.
     */

    while (!(AlUart_ll_GetThreState(Uart->BaseAddr)) && (AlUart_ll_IsFifosEnable(Uart->BaseAddr)));
    return (AlUart_ll_GetLsrThreState(Uart->BaseAddr));
}

/**
 * This function sends a single byte over the UART device.
 *
 * @param Uart Pointer to the UART device structure.
 * @param Char The character to send.
 */
AL_VOID AlUart_Dev_SendByte(AL_UART_DevStruct *Uart, AL_S8 Char)
{
    AlUart_ll_EnableThreIntr(Uart->BaseAddr, AL_FALSE);
    while (!AlUart_ll_GetLsrThreState(Uart->BaseAddr));
    AlUart_ll_SendByte(Uart->BaseAddr, Char);
}

/**
 * This function sends data over the UART device using polling method.
 *
 * @param Uart Pointer to the UART device structure.
 * @param Data Pointer to the data buffer to send.
 * @param Size The size of the data to send.
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlUart_Dev_SendDataPolling(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 Size)
{
    AL_U32 HandledCnt = 0;

    if (AlUart_Dev_IsTxBusy(Uart)){
        return AL_UART_ERR_BUSY;
    }

    AlUart_Dev_SetTxBusy(Uart);

    AlUart_ll_EnableThreIntr(Uart->BaseAddr, AL_FALSE);

    while (HandledCnt < Size) {
        if (AlUart_ll_GetLsrThreState(Uart->BaseAddr)) {
            AlUart_ll_SendByte(Uart->BaseAddr, Data[HandledCnt]);
            HandledCnt ++;
        }
    }

    /* Waiting for transmitter Shift Register and the FIFO both empty. */
    //while (!(AlUart_ll_IsEmptyTsrAndTxFifo(Uart->BaseAddr)));

    AlUart_Dev_ClrTxBusy(Uart);

    return AL_OK;
}

/**
 * This function receives data over the UART device using polling method.
 *
 * @param Uart Pointer to the UART device structure.
 * @param Data Pointer to the buffer to store received data.
 * @param Size The size of the buffer/data to receive.
 * @return AL_OK if successful, error code otherwise.
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
 * This function sends data over the UART device.
 *
 * @param Uart Pointer to the UART device structure.
 * @param SendData Pointer to the data buffer to send.
 * @param SendSize The size of the data to send.
 * @return AL_OK if successful, error code otherwise.
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

    /* Enable programmable THRE mode*/
    AlUart_ll_EnableThreIntr(Uart->BaseAddr, AL_TRUE);

    AlUart_Dev_SetIntr(Uart, UART_TX, AL_FUNC_ENABLE);

    return AL_OK;
}

/**
 *
 * This function receives data from the UART device specified by the Uart parameter and stores it in the buffer
 * pointed to by ReceiveBuf. The function will receive up to ReceiveSize bytes. The actual number of bytes received
 * is determined by the availability of data in the UART's RX FIFO and the ReceiveSize parameter. If the number of
 * bytes received is less than ReceiveSize, the function will enable UART RX interrupt to handle the remaining data.
 *
 * @param Uart Pointer to the UART device structure.
 * @param ReceiveBuf Pointer to the buffer where received data will be stored.
 * @param ReceiveSize Number of bytes to receive.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlUart_Dev_RecvData(AL_UART_DevStruct *Uart, AL_U8 *ReceiveBuf, AL_U32 ReceiveSize)
{
    AL_U32 ReceivedCount = 0;

    AL_ASSERT((Uart != AL_NULL && ReceiveBuf != AL_NULL && ReceiveSize != 0), AL_UART_ERR_ILLEGAL_PARAM);
    AL_ASSERT(((Uart->State & AL_UART_STATE_READY) == AL_UART_STATE_READY), AL_UART_ERR_NOT_READY);
    AL_ASSERT((!AlUart_Dev_IsRxBusy(Uart)), AL_UART_ERR_BUSY);

    AlUart_Dev_SetRxBusy(Uart);

    /* Enable Rx Line Interrupt */
    AlUart_ll_EnableRecvLineIntr(Uart->BaseAddr, AL_TRUE);

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
 *
 * This function registers a callback function that will be called by the UART driver when specified events occur.
 * The callback function is specified by the Callback parameter, and a reference to user data can be passed with
 * the CallbackRef parameter. This allows the user to register a custom function to handle UART events such as
 * transmission completion, reception completion, and errors.
 *
 * @param Uart Pointer to the UART device structure.
 * @param Callback Pointer to the callback function.
 * @param CallbackRef Pointer to user data that will be passed to the callback function.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlUart_Dev_RegisterEventCallBack(AL_UART_DevStruct *Uart, AL_Uart_EventCallBack Callback, AL_VOID *CallbackRef)
{
    AL_ASSERT((Uart != AL_NULL && Callback != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Uart->EventCallBack        = Callback;
    Uart->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 *
 * This function unregisters the callback function that was previously registered for handling UART events. After
 * calling this function, the UART driver will no longer call the event callback function when events occur.
 *
 * @param Uart Pointer to the UART device structure.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlUart_Dev_UnRegisterEventCallBack(AL_UART_DevStruct *Uart)
{
    AL_ASSERT((Uart != AL_NULL), AL_UART_ERR_ILLEGAL_PARAM);

    Uart->EventCallBack = (AL_Uart_EventCallBack)AL_NULL;

    return AL_OK;
}

/**
 *
 * This function is called internally by the UART driver to handle received data. It is typically called from an
 * interrupt service routine when data is available in the UART's RX FIFO. The function reads data from the RX FIFO
 * into the receive buffer until the buffer is full or there is no more data available. If a character timeout
 * interrupt occurs and character timeout is enabled, the function will also handle this condition.
 *
 * @param Uart Pointer to the UART device structure.
 * @param CharTimeoutIntr Indicates whether a character timeout interrupt occurred.
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
 *
 * This function is called internally by the UART driver to handle data transmission. It is typically called from an
 * interrupt service routine when the UART's TX FIFO is ready to accept more data. The function sends data from the
 * transmit buffer to the TX FIFO until the buffer is empty or the TX FIFO is full.
 *
 * @param Uart Pointer to the UART device structure.
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
        while (!(AlUart_Dev_IsTxFifoFull(Uart)) &&
            (Uart->SendBuffer.HandledCnt < Uart->SendBuffer.RequestedCnt)) {
            AlUart_ll_SendByte(Uart->BaseAddr, Uart->SendBuffer.BufferPtr[Uart->SendBuffer.HandledCnt]);
            Uart->SendBuffer.HandledCnt ++;
        }
    }
}

/**
 *
 * This function is called internally by the UART driver to handle receive errors such as overrun, parity, framing,
 * and break conditions. It is typically called from an interrupt service routine when a receive line status
 * interrupt occurs. The function reads any remaining data from the RX FIFO and updates the event structure with
 * the appropriate error flags.
 *
 * @param Uart Pointer to the UART device structure.
 * @param IntrStatus The interrupt status indicating the type of receive error.
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
 *
 * This function is called internally by the UART driver to handle a bus busy condition. It performs a soft reset
 * of the UART bus and reinitializes the UART with its original configuration. If the RX or TX bus is busy, the
 * function updates the event structure with the appropriate busy detect event.
 *
 * @param Uart Pointer to the UART device structure.
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

/**
 *
 * This function is called internally by the UART driver when there are no pending interrupts for the UART device.
 * It updates the event structure with the no interrupt pending event and calls the event callback function if
 * registered.
 *
 * @param Uart Pointer to the UART device structure.
 */
static AL_VOID AlUart_Dev_NoInterruptPeding(AL_UART_DevStruct *Uart)
{
    AL_UART_EventStruct UartEvent = {
        .Events = AL_UART_EVENT_NO_INTR_PEDING
    };
    if (Uart->EventCallBack) {
        (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
    }
}

/**
 *
 * This function is called internally by the UART driver to handle modem status interrupts. It updates the event
 * structure with the modem status interrupt event and calls the event callback function if registered.
 *
 * @param Uart Pointer to the UART device structure.
 */
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
 *
 * This function is the main interrupt handler for the UART device. It checks the interrupt status and calls the
 * appropriate handler function for the specific interrupt condition. This includes handling receive data available,
 * transmit holding register empty, receive line status, bus busy detect, no interrupt pending, and modem status
 * interrupts.
 *
 * @param Instance Pointer to the UART device structure.
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

/**
 *
 * This function enables or disables UART interrupts for receive or transmit operations. It is called internally by
 * the UART driver to control the interrupt enable state based on the current operation, such as starting or stopping
 * receive or transmit operations.
 *
 * @param Uart Pointer to the UART device structure.
 * @param Transfer Specifies the type of operation (UART_RX or UART_TX) for which to set or clear interrupts.
 * @param State Specifies whether to enable or disable interrupts for the specified operation.
 */
static AL_VOID AlUart_Dev_SetIntr(AL_UART_DevStruct *Uart, AL_Uart_TransferEnum Transfer,  AL_FUNCTION State)
{
    AL_BOOL IsRxBusy = AlUart_Dev_IsRxBusy(Uart);
    AL_BOOL IsTxBusy = AlUart_Dev_IsTxBusy(Uart);

    AlOsal_EnterDevCtritical(Uart->IntrNum, (IsRxBusy && IsTxBusy));

    (Transfer == UART_TX) ? AlUart_ll_SetTxIntr(Uart->BaseAddr, State) :
                            AlUart_ll_SetRxIntr(Uart->BaseAddr, State);

    AlOsal_ExitDevCtritical(Uart->IntrNum, IsRxBusy && IsTxBusy);
}

/**
 *
 * This function stops the transmission of data for the specified UART device. It disables the transmit interrupt
 * and clears the transmit busy state. It is called internally by the UART driver when all data in the transmit
 * buffer has been sent or when the user wants to abort the current transmission.
 *
 * @param Uart Pointer to the UART device structure.
 */
AL_VOID AlUart_Dev_StopSend(AL_UART_DevStruct *Uart)
{
    AlUart_Dev_SetIntr(Uart, UART_TX, AL_FUNC_DISABLE);
    AlUart_Dev_ClrTxBusy(Uart);
}

/**
 *
 * This function stops the reception of data for the specified UART device. It disables the receive interrupt and
 * clears the receive busy state. It is called internally by the UART driver when the receive buffer is full or when
 * the user wants to abort the current reception.
 *
 * @param Uart Pointer to the UART device structure.
 */
AL_VOID AlUart_Dev_StopReceive(AL_UART_DevStruct *Uart)
{
    AlUart_Dev_SetIntr(Uart, UART_RX, AL_FUNC_DISABLE);
    AlUart_Dev_ClrRxBusy(Uart);
}

/**
 *
 * This function is responsible for performing various control operations on the UART device, specified by the Cmd
 * parameter. The operations include setting and getting the baud rate, data width, stop bits, parity, auto flow
 * control state, loopback state, and DMA mode. The IoctlParam parameter is a union that provides the necessary
 * data for the operation or receives data from the operation. The function ensures that the UART device is not
 * busy before performing any operation and returns an error if the device is busy. Each control operation updates
 * the UART device's configuration or returns the current configuration based on the command.
 *
 * @param Uart Pointer to the UART device structure.
 * @param Cmd The control command to execute.
 * @param IoctlParam Pointer to the union that provides input parameters for the command or receives output from the command.
 * @return Returns AL_OK on success, AL_UART_ERR_BUSY if the UART device is busy, or AL_UAER_ERR_IOCTL_CMD if the command is invalid.
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
    case AL_UART_IOCTL_SET_DMA_MODE: {
        AL_BOOL DmaMode = IoctlParam->DmaMode;
        AlUart_ll_SetDmaMode(Uart->BaseAddr, DmaMode);
    break;
    }
    default:
        AL_LOG(AL_LOG_LEVEL_ERROR, "AL_UAER_ERR_IOCTL_CMD");
        break;
    }

    return Ret;
}
