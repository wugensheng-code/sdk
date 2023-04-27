#ifndef AL_UART_HAL_H
#define AL_UART_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_uart_dev.h"

typedef struct
{
    AL_UART_DevStruct            *Dev;

#ifdef USE_RTOS
    AL_Lock                      Lock;
    AL_OsEvent                   RxEvent;
    AL_OsEvent                   TxEvent;
#endif

} AL_UART_HalStruct;

AL_S32 AlUart_Hal_Init(AL_UART_HalStruct *Handle, AL_UART_InitStruct *InitConfig,
                       AL_Uart_EventCallBack CallBack , AL_U32 DevId);
AL_S32 AlUart_Hal_SendData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);
AL_S32 AlUart_Hal_RecvData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);
AL_S32 AlUart_Hal_RecvDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 *RecvSize, AL_U32 Timeout);
AL_S32 AlUart_Hal_SendDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

#ifdef __cplusplus
}
#endif

#endif
