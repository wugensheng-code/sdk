#ifndef AL_UART_HAL_H
#define AL_UART_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_uart_dev.h"

typedef enum {
    UART_TX_BLOCK    = 0,
    UART_TX_NONBLOCK = 1,
} AL_UART_TxModeEnum;


typedef enum {
    UART_RECV_BLOCK    = 0,
    UART_RECV_NONBLOCK = 1,
} AL_UART_RxModeEnum;


typedef struct
{
    AL_UART_DevStruct            *Dev;
    AL_Lock                      TxLock;
    AL_Lock                      RxLock;
    AL_UART_TxModeEnum           RequestTxMode;
    AL_UART_RxModeEnum           RequestRxMode;
    AL_UART_TxModeEnum           CurTxMode;
    AL_UART_RxModeEnum           CurRxMode;
    AL_MailBox                   TxEventQueue[2];
    AL_MailBox                   RxEventQueue[2];
} AL_UART_HalStruct;


AL_S32 AlUart_Hal_Init(AL_UART_HalStruct *Handle, AL_U32 DevId, AL_UART_InitStruct *InitConfig, AL_Uart_EventCallBack Callback);

AL_S32 AlUart_Hal_SendData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_RecvData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_RecvDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 *RecvSize, AL_U32 Timeout);

AL_S32 AlUart_Hal_SendDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlUart_Hal_SendDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_RecvDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_IoCtl(AL_UART_HalStruct *Handle, AL_UART_IoCtlCmdEnum Cmd, AL_UART_IoctlParamUnion *IoctlParam);


#ifdef __cplusplus
}
#endif

#endif
