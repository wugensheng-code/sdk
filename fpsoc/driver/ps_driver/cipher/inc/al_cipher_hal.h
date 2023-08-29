/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_CIPHER_HAL_H
#define AL_CIPHER_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_cipher_dev.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/
#define AL_CIPHER_API_MODE_NUM     2

typedef enum {
    CIPHER_BLOCK    = 0,
    CIPHER_NONBLOCK = 1,
} AL_CIPHER_ModeEnum;

typedef struct
{
    AL_CIPHER_DevStruct *Dev;
    AL_Lock             StartLock;
    AL_CIPHER_ModeEnum  ReqStartMode;
    AL_CIPHER_ModeEnum  CurStartMode;
    AL_MailBox          StartEventQueue[AL_CIPHER_API_MODE_NUM];
} AL_CIPHER_HalStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_S32 AlCipher_Hal_Init(AL_CIPHER_HalStruct *Handle, AL_CIPHER_CallBackStruct *CallBack, AL_U32 DevId);

AL_S32 AlCipher_Hal_Start(AL_CIPHER_HalStruct *Handle, AL_CIPHER_CmdEnum Cmd, AL_CIPHER_ConfigUnion *Config);

AL_S32 AlCipher_Hal_StartBlock(AL_CIPHER_HalStruct *Handle, AL_CIPHER_CmdEnum Cmd, AL_CIPHER_ConfigUnion *Config,
                                   AL_U32 Timeout);

#ifdef __cplusplus
}
#endif

#endif
