/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_CAN_HAL_H
#define AL_CAN_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_can_dev.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/
#define AL_CAN_API_MODE_NUM     2

typedef enum {
    CAN_BLOCK    = 0,
    CAN_NONBLOCK = 1,
} AL_CAN_ModeEnum;

typedef struct
{
    AL_CAN_DevStruct    *Dev;
    AL_Lock             TxLock;
    AL_Lock             RxLock;
    AL_CAN_ModeEnum     ReqTxMode;
    AL_CAN_ModeEnum     CurTxMode;
    AL_MailBox          TxEventQueue[AL_CAN_API_MODE_NUM];
    AL_MailBox          RxEventQueue;
} AL_CAN_HalStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_S32 AlCan_Hal_Init(AL_CAN_HalStruct *Handle, AL_U32 DevId, AL_CAN_InitStruct *InitConfig,
                      AL_CAN_CallBackStruct *CallBack);

AL_S32 AlCan_Hal_SendFrame(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame);

AL_S32 AlCan_Hal_RecvFrame(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame);

AL_S32 AlCan_Hal_SendFrameBlock(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame, AL_U32 Timeout);

AL_S32 AlCan_Hal_RecvFrameBlock(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame, AL_U32 Timeout);

AL_S32 AlCan_Hal_RecvFrameDma(AL_CAN_HalStruct *Handle, AL_CAN_FrameStruct *Frame);

AL_S32 AlCan_Hal_IoCtl(AL_CAN_HalStruct *Handle, AL_CAN_IoCtlCmdEnum Cmd, AL_VOID *Data);

#ifdef __cplusplus
}
#endif

#endif
