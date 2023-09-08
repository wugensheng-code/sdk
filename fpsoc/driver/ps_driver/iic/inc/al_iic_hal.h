/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_MPU_HAL_H_
#define __AL_MPU_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_iic_dev.h"

typedef enum {
    AL_IIC_MODE_BLOCK    = 0,
    AL_IIC_MODE_NONBLOCK = 1,
} AL_IIC_TxRxModeEnum;

typedef struct
{
    AL_IIC_DevStruct             Dev;

    AL_Lock                      Lock;
    AL_IIC_TxRxModeEnum          RequestTxOrRxMode;
    AL_IIC_TxRxModeEnum          CurTxOrRxMode;
    AL_MailBox                   TxRxEventQueue[2];
} AL_IIC_HalStruct;

AL_S32 AlIic_Hal_Init(AL_IIC_HalStruct **Handle, AL_U32 DevId,
                      AL_IIC_InitStruct *InitConfig, AL_IIC_EventCallBack Callback);

AL_S32 AlIic_Hal_MasterSendDataBlock(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlIic_Hal_MasterSendDataPolling(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size);

AL_S32 AlIic_Hal_MasterRecvDataBlock(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlIic_Hal_MasterRecvDataPolling(AL_IIC_HalStruct *Handle, AL_U16 SlaveAddr, AL_U8 *Data, AL_U32 Size);

AL_S32 AlIic_Hal_SlaveSendDataBlock(AL_IIC_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlIic_Hal_SlaveRecvDataBlock(AL_IIC_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);

AL_S32 AlIic_Hal_MasterSetCmdOption(AL_IIC_HalStruct *Handle, AL_IIC_CmdOptionEnum CmdOption);

AL_IIC_CmdOptionEnum AlIic_Hal_MasterGetCmdOption(AL_IIC_HalStruct *Handle);

AL_S32 AlIic_Hal_IoCtl(AL_IIC_HalStruct *Handle, AL_IIC_IoCtlCmdEnum Cmd, AL_VOID *Data);

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_HAL_H */
