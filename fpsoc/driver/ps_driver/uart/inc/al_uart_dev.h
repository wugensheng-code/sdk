/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_uart_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-03-29
 * @brief   uart device driver
 */

#ifndef __AL_UART_DEV_H_
#define __AL_UART_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************* Exported Includes ************************************/
#include "al_uart_ll.h"

#define AL_UART_EVENT_START_BIT    6

#define AL_UART_ERR_NULL_PTR                 (AL_DEF_ERR(AL_UART, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_UART_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_UART, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_UART_ERR_NOT_READY                (AL_DEF_ERR(AL_UART, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_UART_ERR_NOT_SUPPORT              (AL_DEF_ERR(AL_UART, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))
#define AL_UART_ERR_TIMEOUT                  (AL_DEF_ERR(AL_UART, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT))
#define AL_UART_ERR_BUSY                     (AL_DEF_ERR(AL_UART, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY))
#define AL_UART_ERR_NOMEM                    (AL_DEF_ERR(AL_UART, AL_LOG_LEVEL_ERROR, AL_ERR_NOMEM))
#define AL_UART_EVENTS_TO_ERRS(Events)       (AL_DEF_ERR(AL_UART, AL_LOG_LEVEL_ERROR, Events << AL_UART_EVENT_START_BIT))


/******************************* Exported Typedef ************************************/
/**
 * @brief  Configs Struct
 */

typedef struct
{
    AL_U32                  BaudRate;
    AL_U32                  WordLength;
    AL_U32                  StopBits;
    AL_UART_ParityEnum      Parity;
    AL_BOOL                 HwFlowCtl;
    AL_BOOL                 CharTimeoutEnable;
} AL_UART_InitStruct;

typedef struct
{
    AL_U8                     *BufferPtr;
    AL_U32                    RequestedCnt;        /*total size requested*/
    AL_U32                    HandledCnt;          /*the size has handled*/
} AL_Uart_BufferStruct;

typedef enum
{
    AL_UART_STATE_NOT_INIT     = (0x0),
    AL_UART_STATE_READY        = (0x01 << 0),
    AL_UART_STATE_TX_BUSY      = (0x01 << 1),
    AL_UART_STATE_RX_BUSY      = (0x01 << 2)
} AL_UART_StateEnum;

typedef enum
{
    AL_UART_OVERRUN_ERR    = BIT(1),
    AL_UART_PARITY_ERR     = BIT(2),
    AL_UART_FRAMING_ERR    = BIT(3),
    AL_UART_BREAK_ERR      = BIT(4)
} AL_UART_RecvErrorTypeEnum;

typedef enum
{
    AL_UART_EVENT_SEND_DONE            = BIT(0),
    AL_UART_EVENT_RECEIVE_DONE         = BIT(1),
    AL_UART_EVENT_READY_TO_RECEIVE     = BIT(2),
    AL_UART_EVENT_READY_TO_SEND        = BIT(3),
    AL_UART_EVENT_BUSY_DETECT_RX       = BIT(4),
    AL_UART_EVENT_BUSY_DETECT_TX       = BIT(5),
    AL_UART_EVENT_CHAR_TIMEOUT         = BIT(6),
    AL_UART_EVENT_PARE_FRAME_BRKE      = BIT(7),
    AL_UART_EVENT_OVER_RUN_ERR         = BIT(8),
    AL_UART_EVENT_PARITY_ERR           = BIT(9),
    AL_UART_EVENT_FRAMING_ERR          = BIT(10),
    AL_UART_EVENT_BREAK_INTR           = BIT(11),
    AL_UART_EVENT_NO_INTR_PEDING       = BIT(12),
    AL_UART_EVENT_MODEM_STATUS_INTR    = BIT(13),
} AL_UART_EventIdEnum;

typedef enum
{
    AL_UART_IOCTL_SET_BAUD_RATE,
    AL_UART_IOCTL_GET_BAUD_RATE,
    AL_UART_IOCTL_SET_DATA_WIDTH,
    AL_UART_IOCTL_GET_DATA_WIDTH,
    AL_UART_IOCTL_SET_STOP_BITS,
    AL_UART_IOCTL_GET_STOP_BITS,
    AL_UART_IOCTL_SET_PARITY,
    AL_UART_IOCTL_GET_PARITY,
    AL_UART_IOCTL_SET_AUTO_FLOW_CTL,
    AL_UART_IOCTL_SET_LOOPBACK
} AL_UART_IoCtlCmdEnum;

typedef union {
    AL_U32     BaudRate;
    AL_U32     DataWidth;
    AL_U32     StopBits;
    AL_U32     Parity;
    AL_BOOL    AutoFlowState;
    AL_BOOL    LoopBack;
}AL_UART_IoctlParamUnion;

typedef struct
{
    AL_UART_EventIdEnum     Events;
    AL_U32                  EventData;
} AL_UART_EventStruct;

typedef AL_VOID (*AL_Uart_EventCallBack)(AL_UART_EventStruct UartEvent, AL_VOID *CallbackRef);

typedef struct
{
    AL_REG                        BaseAddr;
    AL_UART_InitStruct            Configs;
    AL_Uart_BufferStruct          SendBuffer;
    AL_Uart_BufferStruct          RecvBuffer;
    AL_Uart_EventCallBack         EventCallBack;
    AL_VOID                       *EventCallBackRef;
    AL_UART_StateEnum             State;
    AL_U32                        IntrNum;
    AL_U32                        DevId;
    AL_U32                        InputClockHz;
} AL_UART_DevStruct;

AL_UART_HwConfigStruct *AlUart_Dev_LookupConfig(AL_U32 DevId);
AL_S32 AlUart_Dev_Init(AL_UART_DevStruct *Uart, AL_U32 DevId, AL_UART_InitStruct *InitConfig);
AL_S32 AlUart_Dev_SendData(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 SendSize);
AL_S32 AlUart_Dev_RecvData(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 ReceiveSize);
AL_VOID AlUart_Dev_SendByte(AL_UART_DevStruct *Uart, AL_S8 Char);
AL_S32 AlUart_Dev_SendDataPolling(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 Size);
AL_S32 AlUart_Dev_RecvDataPolling(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 Size);
AL_VOID AlUart_Dev_StopSend(AL_UART_DevStruct *Uart);
AL_VOID AlUart_Dev_StopReceive(AL_UART_DevStruct *Uart);
AL_VOID AlUart_Dev_IntrHandler(void *Instance);
AL_S32 AlUart_Dev_RegisterEventCallBack(AL_UART_DevStruct *Uart, AL_Uart_EventCallBack Callback, AL_VOID *CallbackRef);
AL_S32 AlUart_Dev_UnRegisterEventCallBack(AL_UART_DevStruct *Uart);
AL_S32 AlUart_Dev_IoCtl(AL_UART_DevStruct *Uart, AL_UART_IoCtlCmdEnum Cmd, AL_UART_IoctlParamUnion *IoctlParam);

#ifdef __cplusplus
}
#endif

#endif
