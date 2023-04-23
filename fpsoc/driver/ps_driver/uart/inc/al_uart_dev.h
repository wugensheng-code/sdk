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
#include "al_type.h"
#include "al_errno.h"
#include "al_uart_ll.h"



#define AL_UART_NULL_PTR             (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_NULL_PTR))

#define AL_UART_ERR_ILLEGAL_PARAM    (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))

#define AL_UART_ERR_NOT_READY        (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_READY))

#define AL_UART_ERR_NOT_SUPPORT      (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_SUPPORT))

#define AL_UART_ERR_TIMEOUT          (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT))

#define AL_UART_ERR_BUSY             (AL_DEF_ERR(AL_UART, AL_ERR_LEVEL_ERROR, AL_ERR_BUSY))


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

typedef struct {
    AL_U8                     *BufferPtr;
    AL_U32                    RequestedCnt;        /*total size requested*/
    AL_U32                    HandledCnt;          /*the size has handled*/
} AL_Uart_BufferStruct;


typedef enum {
    AL_UART_STATE_NOT_INIT     = (0x0),
    AL_UART_STATE_READY        = (0x01 << 0),
    AL_UART_STATE_TX_BUSY      = (0x01 << 1),
    AL_UART_STATE_RX_BUSY      = (0x01 << 2),
} AL_UART_StateEnum;

typedef enum
{
    AL_UART_SEND_DONE                = (0x01),
    AL_UART_RECEIVE_DONE             = (0x01 << 1),
    AL_UART_RECEIVE_TIMEOUT          = (0x01 << 2),
    AL_UART_EVENT_PARE_FRAME_BRKE    = (0x01 << 3),
} AL_UART_EventIdEnum;


typedef struct
{
    AL_UART_EventIdEnum     Event;
    AL_U32                  EventData;
} AL_UART_EventStruct;

typedef AL_VOID (*UART_EventCallBack)(AL_UART_EventStruct UartEvent, AL_VOID *CallbackRef);


typedef struct
{
    AL_REG32                      BaseAddr;
    AL_UART_InitStruct            Configs;
    AL_Uart_BufferStruct          SendBuffer;
    AL_Uart_BufferStruct          RecvBuffer;

    UART_EventCallBack            EventCallBack;
    AL_VOID                       *EventCallBackRef;
    AL_U32                        ErrorCode;
    AL_UART_StateEnum             State;
} AL_UART_DevStruct;

typedef enum
{
	MODEM_STATUS			= 0x0,
	NO_INTERRUPT_PENDING    = 0x1,
    THR_EMPTY				= 0x2,
	RECEIVED_DATA_AVAILABLE	= 0x4,
	RECEIVER_LINE_STATUS	= 0x6,
	BUSY_DETECT				= 0x7,
	CHARACTER_TIMEOUT		= 0xc
} InterruptID;



AL_S32 AlUart_Dev_Init(AL_UART_DevStruct *Uart, AL_UART_InitStruct *InitConfig, AL_U32 DevID);

AL_S32 AlUart_Dev_SendData(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 SendSize);

AL_S32 AlUart_Dev_RecvData(AL_UART_DevStruct *Uart, AL_U8 *Data, AL_U32 ReceiveSize);

AL_S32 AlUart_Dev_RegisterEventCallBack(AL_UART_DevStruct *Uart, UART_EventCallBack *Callback, void *CallbackRef);

AL_S32 AlUart_Dev_UnRegisterEventCallBack(AL_UART_DevStruct *Uart);

AL_UART_HwConfigStruct *AlUart_Dev_LookupConfig(AL_U32 DeviceId);

AL_VOID AlUart_Dev_IntrHandler(AL_UART_DevStruct *Uart);

#ifdef __cplusplus
}
#endif

#endif
