/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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
#include "al_spi_ll.h"


/******************************* Exported Typedef ************************************/
#define MIN3(x, y, z) MIN((typeof(x))MIN(x, y), z)

#define SPI_ENDIAN_SWAP16(A) ((((AL_U32)(A) & 0x0000ff00) >> 8) | \
                              (((AL_U32)(A) & 0x000000ff) << 8))

#define SPI_ENDIAN_SWAP32(A) ((((AL_U32)(A) & 0xff000000) >> 24) | \
                              (((AL_U32)(A) & 0x00ff0000) >> 8 ) | \
                              (((AL_U32)(A) & 0x0000ff00) << 8 ) | \
                              (((AL_U32)(A) & 0x000000ff) << 24))

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

#define AL_SPI_ERR_ILLEGAL_PARAM    AL_DEF_ERR(AL_SPI, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM)
#define AL_SPI_ERR_BUSY             AL_DEF_ERR(AL_SPI, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY)
#define AL_SPI_ERR_TIMEOUT          AL_DEF_ERR(AL_SPI, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT)
#define AL_SPI_ERR_NOT_SUPPORT      AL_DEF_ERR(AL_SPI, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT)
#define AL_SPI_ERR_NOT_READY        AL_DEF_ERR(AL_SPI, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_READY)

typedef AL_S32 (*SPI_EventCallBack)(AL_SPI_EventStruct SpiEvent, AL_VOID *CallbackRef);

typedef struct
{
    AL_REG                        BaseAddr;
    AL_U8                         Fifolen;
    AL_S32                        BitsPerWord;
    AL_SPI_ConfigsStruct          Configs;
    AL_SPI_BufferStruct           SendBuffer;
    AL_SPI_BufferStruct           RecvBuffer;
    SPI_EventCallBack             EventCallBack;
    AL_VOID                       *EventCallBackRef;
    AL_U32                        ErrorCode;
    AL_SPI_StateEnum              State;
} AL_SPI_DevStruct;

/**
 * @brief  Spi Io ctl cmd enum
 */
typedef enum
{
    AL_SPI_IOCTL_SET_SLAVE_SELECT,
    AL_SPI_IOCTL_GET_SLAVE_SELECT
} AL_Spi_IoCtlCmdEnum;

AL_S32 AlSpi_Dev_Init(AL_SPI_DevStruct *Spi, AL_SPI_ConfigsStruct *Init);
AL_S32 AlSpi_Dev_SendData(AL_SPI_DevStruct *Spi, AL_U8 *Data, AL_U32 SendSize);
AL_S32 AlSpi_Dev_RecvData(AL_SPI_DevStruct *Spi, AL_U8 *Data, AL_U16 ReceiveSize);
AL_S32 AlSpi_Dev_TranferData(AL_SPI_DevStruct *Spi, AL_U8 *SendBuf, AL_U32 SendSize, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize);
AL_S32 AlSpi_Dev_IoCtl(AL_SPI_DevStruct *Spi, AL_Spi_IoCtlCmdEnum Cmd, AL_VOID *Data);
AL_S32 AlSpi_Dev_RegisterIntrCallBack(AL_SPI_DevStruct *Spi, SPI_EventCallBack Callback, AL_VOID *CallbackRef);
AL_S32 AlSpi_Dev_UnRegisterIntrCallBack(AL_SPI_DevStruct *Spi);
AL_VOID AlSpi_Dev_IntrHandler(AL_VOID *instance);
AL_SPI_HwConfigStruct *AlSpi_Dev_LookupConfig(AL_U32 DeviceId);
AL_S32 AlSpi_Dev_DmaSendData(AL_SPI_DevStruct *Spi);
AL_S32 AlSpi_Dev_DmaRecvData(AL_SPI_DevStruct *Spi, AL_U16 RecvSize);
AL_S32 AlSpi_Dev_DmaTranferData(AL_SPI_DevStruct *Spi, AL_U16 RecvSize);

#ifdef SPI_DEBUG
AL_VOID AlSpi_Dev_DumpReg(AL_REG SpiBaseAddr);
#endif


#ifdef __cplusplus
}
#endif

#endif
