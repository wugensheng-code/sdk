/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_QSPI_HAL_H_
#define __AL_QSPI_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_qspi_dev.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/
struct AL_QSPI_DevStruct;

typedef struct
{
    AL_QSPI_DevStruct            Dev;
    AL_Lock                      QspiLock;
    AL_MailBox                   TxEventQueue;
    AL_MailBox                   RxEventQueue;
} AL_QSPI_HalStruct;

/***************** Macros (Inline Functions) Definitions ********************/

/******************************** Variable Prototypes ******************************/

/******************************** Function Prototypes ******************************/
AL_S32 AlQspi_Hal_Init(AL_QSPI_HalStruct **Handle, AL_QSPI_ConfigsStruct *InitConfig, QSPI_EventCallBack Callback,  AL_U32 DevId);
AL_S32 AlQspi_Hal_SendDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_S32 AlQspi_Hal_RecvDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_S32 AlQspi_Hal_TranferDataBlock(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                   AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout);
AL_S32 AlQspi_Hal_DmaStartSendBlock(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U8* InstAndAddr, AL_U32 SendDataSize, AL_U32 Timeout);
AL_S32 AlQspi_Hal_DmaStartReceiveBlock(AL_QSPI_HalStruct *Handle, AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout);
AL_S32 AlQspi_Hal_DmaStartTranferBlock(AL_QSPI_HalStruct *Handle, AL_U8 *SendData, AL_U32 SendSize,
                                       AL_U8 *RecvData, AL_U16 RecvSize, AL_U32 Timeout);
AL_S32 AlQspi_Hal_IoCtl(AL_QSPI_HalStruct *Handle, AL_Qspi_IoCtlCmdEnum Cmd, AL_VOID *Data, AL_U32 Timeout);

#ifdef __cplusplus
}
#endif
#endif
