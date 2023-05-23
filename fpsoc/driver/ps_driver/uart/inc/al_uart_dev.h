/**
 * @file    al_uart_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-03-29
 * @brief   uart device driver
 */

#ifndef AL_UART_DEV_H
#define AL_UART_DEV_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************* Exported Includes ************************************/
#include "al_errno.h"
#include "al_uart_ll.h"

#define AL_UART_ERR_NULL_PTR             (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_NULL_PTR))
#define AL_UART_ERR_ILLEGAL_PARAM        (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_UART_ERR_NOT_READY            (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_READY))
#define AL_UART_ERR_NOT_SUPPORT          (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))
#define AL_UART_ERR_TIMEOUT              (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT))
#define AL_UART_ERR_BUSY                 (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_BUSY))

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
    AL_U32                  Mode;
    AL_U32                  HwFlowCtl;
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
    AL_UART_BUSY_DETECT              = (0x00),
    AL_UART_SEND_DONE                = (0x01),
    AL_UART_RECEIVE_DONE             = (0x01 << 1),
    AL_UART_RECEIVE_TIMEOUT          = (0x01 << 2),
    AL_UART_EVENT_PARE_FRAME_BRKE    = (0x01 << 3),
    AL_UART_EVENT_OVER_RUN_ERR       = (0x01 << 4),
    AL_UART_EVENT_PARITY_ERR         = (0x01 << 5),
    AL_UART_EVENT_FRAMING_ERR        = (0x01 << 6),
    AL_UART_EVENT_BREAK_INTR         = (0x01 << 7)
} AL_UART_EventIdEnum;

/**
 * @brief  Io ctl cmd enum
 */
typedef enum
{
    AL_UART_IOCTL_SET_BAUD_RATE,
    AL_UART_IOCTL_GET_BAUD_RATE,
    AL_UART_IOCTL_SET_DATA_WIDTH,
    AL_UART_IOCTL_GET_DATA_WIDTH,
    AL_UART_IOCTL_SET_STOP_BITS,
    AL_UART_IOCTL_GET_STOP_BITS,
    AL_UART_IOCTL_SET_PARITY,
    AL_UART_IOCTL_GET_PARITY
} AL_Uart_IoCtlCmdEnum;


typedef struct
{
    AL_UART_EventIdEnum     Event;
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
    AL_U32                        ErrorCode;
    AL_UART_StateEnum             State;
} AL_UART_DevStruct;


AL_S32 AlUart_Dev_Init(AL_UART_DevStruct *Uart, AL_UART_InitStruct *InitConfig, AL_U32 DevId);
AL_S32 AlUart_Dev_SendData(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 SendSize);
AL_S32 AlUart_Dev_RecvData(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 ReceiveSize);
AL_S32 AlUart_Dev_RegisterEventCallBack(AL_UART_DevStruct *Uart, AL_Uart_EventCallBack Callback, AL_VOID *CallbackRef);
AL_S32 AlUart_Dev_UnRegisterEventCallBack(AL_UART_DevStruct *Uart);
AL_UART_HwConfigStruct *AlUart_Dev_LookupConfig(AL_U32 DevId);
AL_VOID AlUart_Dev_IntrHandler(void *Instance);
AL_S32 AlUart_Dev_IoCtl(AL_UART_DevStruct *Uart, AL_Uart_IoCtlCmdEnum Cmd, AL_VOID *Data);

AL_S32 AlUart_Dev_SendDataPolling(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 Size);

#ifdef __cplusplus
}
#endif

#endif
