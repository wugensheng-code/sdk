/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_SPI_HAL_H_
#define __AL_SPI_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "al_spi_dev.h"

struct AL_SPI_DevStruct;

typedef struct
{
    AL_SPI_DevStruct             Dev;
    AL_Lock                      SpiLock;
    AL_MailBox                   TxEventQueue;
    AL_MailBox                   RxEventQueue;
} AL_SPI_HalStruct;

AL_S32 AlSpi_Hal_Init(AL_SPI_HalStruct **Handle, AL_SPI_ConfigsStruct *InitConfig, SPI_EventCallBack Callback, AL_U32 DevId);
AL_S32 AlSpi_Hal_SendDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_S32 AlSpi_Hal_RecvDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_S32 AlSpi_Hal_TranferDataBlock(AL_SPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                  AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout);
AL_S32 AlSpi_Hal_IoCtl(AL_SPI_HalStruct *Handle, AL_Spi_IoCtlCmdEnum Cmd, AL_VOID *Data);
AL_S32 AlSpi_Hal_DmaStartBlockSend(AL_SPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize, AL_U32 Timeout);
AL_S32 AlSpi_Hal_DmaStartBlockReceive(AL_SPI_HalStruct *Handle, AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout);
AL_S32 AlSpi_Hal_DmaStartBlockTranfer(AL_SPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                       AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout);

#ifdef __cplusplus
}
#endif
#endif
