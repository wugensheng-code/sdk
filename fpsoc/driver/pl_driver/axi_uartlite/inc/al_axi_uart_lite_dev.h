/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_UART_LITE_DEV_H_
#define AL_AXI_UART_LITE_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_axi_uart_lite_ll.h"

#define AL_AXI_UART_LITE 99
#define AL_AXI_UART_LITE_EVENT_START_BIT    6

#define AL_AXI_UART_LITE_ERR_NULL_PTR                 (AL_DEF_ERR(AL_AXI_UART_LITE, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_AXI_UART_LITE_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_AXI_UART_LITE, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_AXI_UART_LITE_ERR_NOT_READY                (AL_DEF_ERR(AL_AXI_UART_LITE, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_AXI_UART_LITE_ERR_NOT_SUPPORT              (AL_DEF_ERR(AL_AXI_UART_LITE, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))
#define AL_AXI_UART_LITE_ERR_TIMEOUT                  (AL_DEF_ERR(AL_AXI_UART_LITE, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT))
#define AL_AXI_UART_LITE_ERR_BUSY                     (AL_DEF_ERR(AL_AXI_UART_LITE, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY))
#define AL_AXI_UART_LITE_ERR_NOMEM                    (AL_DEF_ERR(AL_AXI_UART_LITE, AL_LOG_LEVEL_ERROR, AL_ERR_NOMEM))
#define AL_AXI_UART_LITE_EVENTS_TO_ERRS(Events)       (AL_DEF_ERR(AL_AXI_UART_LITE, AL_LOG_LEVEL_ERROR, Events << AL_AXI_UART_LITE_EVENT_START_BIT))


/**
 * @brief  Module status enum
 */
typedef enum
{
    AL_AXI_UART_LITE_RX_FIFO_HAS_DATA       = 0,
    AL_AXI_UART_LITE_RX_FIFO_FULL           = 1,
    AL_AXI_UART_LITE_TX_FIFO_EMPTY          = 2,
    AL_AXI_UART_LITE_TX_FIFO_FULL           = 3,
    AL_AXI_UART_LITE_INT_EN                 = 4,
    AL_AXI_UART_LITE_OVERRUN_ERR            = 5,
    AL_AXI_UART_LITE_FRAME_ERR              = 6,
    AL_AXI_UART_LITE_PARITY_ERR             = 7
} Al_AxiUartLite_StatusEnum;

/**
 * @brief  Data buffer
 */
typedef struct
{
    AL_U8                     *BufferPtr;
    AL_U32                    RequestedCnt;        /* total size requested */
    AL_U32                    HandledCnt;          /* the size has handled */
} Al_AxiUartLite_BufferStruct;

/**
 * @brief  Module running state enum
 */
typedef enum
{
    AL_AXI_UART_LITE_STATE_NOT_INIT     = (0x0),
    AL_AXI_UART_LITE_STATE_READY        = (0x01 << 0),
    AL_AXI_UART_LITE_STATE_TX_BUSY      = (0x01 << 1),
    AL_AXI_UART_LITE_STATE_RX_BUSY      = (0x01 << 2)
} Al_AxiUartLite_StateEnum;

/**
 * @brief  Event enum
 */
typedef enum
{
    AL_AXI_UART_LITE_EVENT_SEND_DONE            = BIT(0),
    AL_AXI_UART_LITE_EVENT_RECEIVE_DONE         = BIT(1),
    AL_AXI_UART_LITE_EVENT_READY_TO_RECEIVE     = BIT(2),
    AL_AXI_UART_LITE_EVENT_READY_TO_SEND        = BIT(3),
    AL_AXI_UART_LITE_EVENT_OVER_RUN_ERR         = BIT(4),
    AL_AXI_UART_LITE_EVENT_PARITY_ERR           = BIT(5),
    AL_AXI_UART_LITE_EVENT_FRAMING_ERR          = BIT(6),
} Al_AxiUartLite_EventIdEnum;

/**
 * @brief  Events struct
 */
typedef struct
{
    Al_AxiUartLite_EventIdEnum        Events;
    AL_U32                        EventData;
} Al_AxiUartLite_EventStruct;

typedef AL_VOID (*Al_AxiUartLite_EventCallBack)(Al_AxiUartLite_EventStruct AxiUartLiteEvent, AL_VOID *CallbackRef);

typedef struct
{
    AlAxiUartLite_HwConfigStruct     HwConfig;
    Al_AxiUartLite_BufferStruct       SendBuffer;
    Al_AxiUartLite_BufferStruct       RecvBuffer;
    Al_AxiUartLite_EventCallBack      EventCallBack;
    AL_VOID                       *EventCallBackRef;
    Al_AxiUartLite_StateEnum          State;
} Al_AxiUartLite_DevStruct;

AlAxiUartLite_HwConfigStruct *AlAxiUartLite_Dev_LookupConfig(AL_U32 DevId);
AL_S32 AlAxiUartLite_Dev_Init(Al_AxiUartLite_DevStruct *Uart, AL_U32 DevId);
AL_S32 AlAxiUartLite_Dev_SendDataPolling(Al_AxiUartLite_DevStruct *Uart, AL_U8 *Data, AL_U32 Size);
AL_S32 AlAxiUartLite_Dev_RecvDataPolling(Al_AxiUartLite_DevStruct *Uart, AL_U8 *Data, AL_U32 Size);
AL_S32 AlAxiUartLite_Dev_SendData(Al_AxiUartLite_DevStruct *Uart, AL_U8 *Data, AL_U32 Size);
AL_S32 AlAxiUartLite_Dev_RecvData(Al_AxiUartLite_DevStruct *Uart, AL_U8 *Data, AL_U32 Size);
AL_VOID AlAxiUartLite_Dev_IntrHandler(void *Instance);
AL_S32 AlAxiUartLite_Dev_RegisterEventCallBack(Al_AxiUartLite_DevStruct *Uart, Al_AxiUartLite_EventCallBack Callback, AL_VOID *CallbackRef);
AL_S32 AlAxiUartLite_Dev_UnRegisterEventCallBack(Al_AxiUartLite_DevStruct *Uart);

#ifdef __cplusplus
}
#endif

#endif