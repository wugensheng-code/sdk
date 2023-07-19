#ifndef AL_UART_HAL_H
#define AL_UART_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_uart_dev.h"

typedef struct
{
    AL_UART_DevStruct            *Dev;
    AL_Mutex                     TxLock;
    AL_Mutex                     RxLock; 
    AL_Semaphore                 TxDoneSem;
    AL_Semaphore                 RxDoneSem;
    AL_S32                       Error;
} AL_UART_HalStruct;


AL_S32 AlUart_Hal_Init(AL_UART_HalStruct *Handle, AL_U32 DevId, AL_UART_InitStruct *InitConfig);

AL_S32 AlUart_Hal_SendData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_RecvData(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_RecvDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 *RecvSize, AL_U32 Timeout);

AL_S32 AlUart_Hal_SendDataBlock(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlUart_Hal_SendDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_RecvDataPolling(AL_UART_HalStruct *Handle, AL_U8 *Data, AL_U32 Size);

AL_S32 AlUart_Hal_IoCtl(AL_UART_HalStruct *Handle, AL_Uart_IoCtlCmdEnum Cmd, AL_VOID *Data);


#ifdef __cplusplus
}
#endif

#endif
