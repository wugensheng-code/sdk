/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_uart_lite_dev.h"

extern AlAxiUartLite_HwConfigStruct AlAxiUartLite_HwConfig[AL_AXI_UART_LITE_NUM_INSTANCE];

AlAxiUartLite_HwConfigStruct *AlAxiUartLite_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AlAxiUartLite_HwConfigStruct *ConfigPtr = AL_NULL;
    for (Index = 0; Index < AL_AXI_UART_LITE_NUM_INSTANCE; Index++)
    {
        if (AlAxiUartLite_HwConfig[Index].DeviceId == DevId)
        {
            ConfigPtr = &AlAxiUartLite_HwConfig[Index];
            break;
        }
    }
    return ConfigPtr;
}

/**
 * This function initializes the UART device with specified configurations.
 *
 * @param Uart Pointer to the UART device structure to initialize.
 * @param DevId The device ID of the UART.
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlAxiUartLite_Dev_Init(Al_AxiUartLite_DevStruct *Uart, AL_U32 DevId)
{
    AlAxiUartLite_HwConfigStruct *UartHwConfig = AL_NULL;

    UartHwConfig = AlAxiUartLite_Dev_LookupConfig(DevId);
    Uart->HwConfig.BaseAddress = UartHwConfig->BaseAddress;
    Uart->HwConfig.IntrId = UartHwConfig->IntrId;
    Uart->HwConfig.BandRate = UartHwConfig->BandRate;
    Uart->HwConfig.DataWidth = UartHwConfig->DataWidth;
    Uart->HwConfig.ParityEn = UartHwConfig->ParityEn;
    Uart->HwConfig.ParityOdd = UartHwConfig->ParityOdd;
    Uart->HwConfig.ParityEven = UartHwConfig->ParityEven;

    AlAxiUartLite_EnableIntr(Uart->HwConfig.BaseAddress, AL_FALSE);
    AlAxiUartLite_ResetRxFifo(Uart->HwConfig.BaseAddress);
    AlAxiUartLite_ResetTxFifo(Uart->HwConfig.BaseAddress);

    Uart->State |= AL_AXI_UART_LITE_STATE_READY;

    return AL_OK;
}

/**
 * This function sends data over the UART device using polling method.
 *
 * @param Uart Pointer to the UART device structure.
 * @param Data Pointer to the data buffer to send.
 * @param Size The size of the data to send.
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlAxiUartLite_Dev_SendDataPolling(Al_AxiUartLite_DevStruct *Uart, AL_U8 *Data, AL_U32 Size)
{
    AL_U32 HandledCnt = 0;

    while (HandledCnt < Size) {
        if (!(AlAxiUartLite_GetStatus(Uart->HwConfig.BaseAddress) & BIT(AL_AXI_UART_LITE_TX_FIFO_FULL))) {
            AlAxiUartLite_SendData(Uart->HwConfig.BaseAddress, Data[HandledCnt]);
            HandledCnt ++;
        }
    }

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
AL_S32 AlAxiUartLite_Dev_RecvDataPolling(Al_AxiUartLite_DevStruct *Uart, AL_U8 *Data, AL_U32 Size)
{
    AL_U32 HandledCnt = 0;

    while (HandledCnt < Size) {
        if (AlAxiUartLite_GetStatus(Uart->HwConfig.BaseAddress) & BIT(AL_AXI_UART_LITE_RX_FIFO_HAS_DATA)) {
            Data[HandledCnt] = AlAxiUartLite_RecvData(Uart->HwConfig.BaseAddress);
            HandledCnt ++;
        }
    }

    return AL_OK;
}

/**
 * This function sends data over the UART device.
 *
 * @param Uart Pointer to the UART device structure.
 * @param Data Pointer to the buffer to store send data.
 * @param Size The size of the buffer/data to send.
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlAxiUartLite_Dev_SendData(Al_AxiUartLite_DevStruct *Uart, AL_U8 *Data, AL_U32 Size)
{
    Uart->SendBuffer.BufferPtr      = Data;
    Uart->SendBuffer.RequestedCnt   = Size;
    Uart->SendBuffer.HandledCnt     = 0;
    AL_U32 HandledCnt = 0;

    while (HandledCnt < 1) {
        if (!(AlAxiUartLite_GetStatus(Uart->HwConfig.BaseAddress) & BIT(AL_AXI_UART_LITE_TX_FIFO_FULL))) {
            AlAxiUartLite_SendData(Uart->HwConfig.BaseAddress, Data[HandledCnt]);
            HandledCnt ++;
            Uart->SendBuffer.HandledCnt ++;
        }
    }

    AlAxiUartLite_EnableIntr(Uart->HwConfig.BaseAddress, AL_TRUE);

    return AL_OK;
}

/**
 * This function recv data over the UART device.
 *
 * @param Uart Pointer to the UART device structure.
 * @param Data Pointer to the buffer to store received data.
 * @param Size The size of the buffer/data to receive.
 * @return AL_OK if successful, error code otherwise.
 */
AL_S32 AlAxiUartLite_Dev_RecvData(Al_AxiUartLite_DevStruct *Uart, AL_U8 *Data, AL_U32 Size)
{
    Uart->RecvBuffer.BufferPtr      = Data;
    Uart->RecvBuffer.RequestedCnt   = Size;
    Uart->RecvBuffer.HandledCnt     = 0;

    AlAxiUartLite_EnableIntr(Uart->HwConfig.BaseAddress, AL_TRUE);

    return AL_OK;
}

/**
 * @param Uart Pointer to the UART device structure.
 */
static AL_VOID AlAxiUartLite_Dev_RecvDataHandler(Al_AxiUartLite_DevStruct *Uart)
{
    if ((AlAxiUartLite_GetStatus(Uart->HwConfig.BaseAddress) & BIT(AL_AXI_UART_LITE_RX_FIFO_HAS_DATA)) &&
           (Uart->RecvBuffer.HandledCnt < Uart->RecvBuffer.RequestedCnt)) {
        Uart->RecvBuffer.BufferPtr[Uart->RecvBuffer.HandledCnt] = AlAxiUartLite_RecvData(Uart->HwConfig.BaseAddress);
        Uart->RecvBuffer.HandledCnt ++;
    }

    if (Uart->RecvBuffer.HandledCnt == Uart->RecvBuffer.RequestedCnt) {
        if (Uart->EventCallBack) {
            Al_AxiUartLite_EventStruct UartEvent = {
                .Events        = AL_AXI_UART_LITE_EVENT_RECEIVE_DONE,
                .EventData     = Uart->RecvBuffer.HandledCnt
            };
            (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
        }
    }
}

/**
 * @param Uart Pointer to the UART device structure.
 */
static AL_VOID AlAxiUartLite_Dev_SendDataHandler(Al_AxiUartLite_DevStruct *Uart)
{

    if ((!(AlAxiUartLite_GetStatus(Uart->HwConfig.BaseAddress) & BIT(AL_AXI_UART_LITE_TX_FIFO_FULL))) &&
        (Uart->SendBuffer.HandledCnt < Uart->SendBuffer.RequestedCnt)) {
        AlAxiUartLite_SendData(Uart->HwConfig.BaseAddress, Uart->SendBuffer.BufferPtr[Uart->SendBuffer.HandledCnt]);
        Uart->SendBuffer.HandledCnt ++;
    }

    if (Uart->SendBuffer.HandledCnt == Uart->SendBuffer.RequestedCnt) {
        if (Uart->EventCallBack) {
            Al_AxiUartLite_EventStruct UartEvent = {
                .Events        = AL_AXI_UART_LITE_EVENT_SEND_DONE,
                .EventData     = Uart->SendBuffer.HandledCnt
            };
            (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
        }
    }
}


/**
 *
 * @param Uart Pointer to the UART device structure.
 * @param Status The interrupt status indicating the type of receive error.
 */
static AL_VOID AlAxiUartLite_Dev_RecvErrorHandler(Al_AxiUartLite_DevStruct *Uart, AL_U32 UartStatus)
{
    Al_AxiUartLite_EventStruct UartEvent = {
        .Events     = 0,
        .EventData  = 0,
    };

    UartEvent.EventData = Uart->RecvBuffer.HandledCnt;
    if (Uart->EventCallBack) {
        (*Uart->EventCallBack)(UartEvent, Uart->EventCallBackRef);
    }
}

/**
 *
 * This function is the main interrupt handler for the UART device. It checks the interrupt status and calls the
 * appropriate handler function for the specific interrupt condition. This includes handling receive data available,
 * transmit holding register empty, receive line status, bus busy detect, no interrupt pending, and modem status
 * interrupts.
 *
 * @param Instance Pointer to the UART device structure.
 */
AL_VOID AlAxiUartLite_Dev_IntrHandler(AL_VOID *Instance)
{
    Al_AxiUartLite_DevStruct *Uart = (Al_AxiUartLite_DevStruct *)Instance;
    AL_U32 UartStatus = AlAxiUartLite_GetStatus(Uart->HwConfig.BaseAddress);

    if (UartStatus & BIT(AL_AXI_UART_LITE_INT_EN)) {
        if ((UartStatus & BIT(AL_AXI_UART_LITE_RX_FIFO_HAS_DATA)) || (UartStatus & BIT(AL_AXI_UART_LITE_RX_FIFO_FULL))) {
            AlAxiUartLite_Dev_RecvDataHandler(Uart);
        }
        if ((UartStatus & BIT(AL_AXI_UART_LITE_TX_FIFO_EMPTY))) {
            AlAxiUartLite_Dev_SendDataHandler(Uart);
        }
        if ((UartStatus & BIT(AL_AXI_UART_LITE_OVERRUN_ERR)) || (UartStatus & BIT(AL_AXI_UART_LITE_FRAME_ERR)) || (UartStatus & BIT(AL_AXI_UART_LITE_PARITY_ERR))) {
            AlAxiUartLite_Dev_RecvErrorHandler(Uart, UartStatus);
        }
    }
}

AL_S32 AlAxiUartLite_Dev_RegisterEventCallBack(Al_AxiUartLite_DevStruct *Uart, Al_AxiUartLite_EventCallBack Callback, AL_VOID *CallbackRef)
{
    AL_ASSERT((Uart != AL_NULL && Callback != AL_NULL), AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM);

    Uart->EventCallBack        = Callback;
    Uart->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

AL_S32 AlAxiUartLite_Dev_UnRegisterEventCallBack(Al_AxiUartLite_DevStruct *Uart)
{
    AL_ASSERT((Uart != AL_NULL), AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM);

    Uart->EventCallBack = (Al_AxiUartLite_EventCallBack)AL_NULL;

    return AL_OK;
}