/**
 * @file    al_SPI_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-03-29
 * @brief   SPI device driver
 */

#ifndef AL_SPI_DRV_H
#define AL_SPI_DRV_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************* Exported Includes ************************************/
#include "al_type.h"
#include "al_errno.h"
#include "al_spi_ll.h"


/******************************* Exported Typedef ************************************/
typedef struct {
    AL_U8                     *BufferPtr;
    AL_U32                    RequestedCnt;        /*total size requested*/
    AL_U32                    HandledCnt;          /*the size has handled*/
} AL_SPI_BufferStruct;

typedef enum {
    AL_SPI_STATE_NOT_INIT     = 0,
    AL_SPI_STATE_READY        = (1 << 0),
    AL_SPI_STATE_TX_BUSY      = (1 << 1),
    AL_SPI_STATE_RX_BUSY      = (1 << 2),
    AL_SPI_STATE_ERROR        = (1 << 3)
} AL_SPI_StateEnum;

typedef AL_S32 (*SPI_EventCallBack)(AL_SPI_EventStruct SpiEvent, AL_VOID *CallbackRef);

typedef struct
{
    AL_REG                        BaseAddr;
    AL_U8                         Fifolen;
    AL_SPI_ConfigsStruct          Configs;
    AL_SPI_BufferStruct           SendBuffer;
    AL_SPI_BufferStruct           RecvBuffer;
    SPI_EventCallBack             EventCallBack;
    AL_VOID                       *EventCallBackRef;
    AL_U32                        ErrorCode;
    AL_SPI_StateEnum              State;
} AL_SPI_DevStruct;



AL_S32 AlSpi_Dev_Init(AL_SPI_DevStruct *Spi, AL_SPI_ConfigsStruct *Init);
AL_S32 AlSpi_Dev_SendData(AL_SPI_DevStruct *Spi, AL_U8 *Data, AL_U32 SendSize);
AL_S32 AlSpi_Dev_RecvData(AL_SPI_DevStruct *Spi, AL_U8 *Data, AL_U16 ReceiveSize);
AL_S32 AlSpi_Dev_SendRecvData(AL_SPI_DevStruct *Spi, AL_U8 *SendBuf, AL_U32 SendSize, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize);
AL_S32 AlSpi_Dev_RegisterIntrCallBack(AL_SPI_DevStruct *Spi, SPI_EventCallBack Callback, AL_VOID *CallbackRef);
AL_S32 AlSpi_Dev_UnRegisterIntrCallBack(AL_SPI_DevStruct *Spi);
AL_VOID AlSpi_Dev_IntrHandler(AL_VOID *instance);
AL_SPI_HwConfigStruct *AlSpi_Dev_LookupConfig(AL_U32 DeviceId);

#ifdef SPI_DEBUG
AL_VOID AlSpi_Dev_DumpReg(AL_SPI_DevStruct *Spi);
#endif


#ifdef __cplusplus
}
#endif

#endif
