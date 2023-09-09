/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_CIPHER_HAL_H_
#define __AL_CIPHER_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_cipher_dev.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/
typedef struct
{
    AL_CIPHER_DevStruct Dev;
    AL_Lock             StartLock;
    AL_MailBox          StartEventQueue;
} AL_CIPHER_HalStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_S32 AlCipher_Hal_Init(AL_CIPHER_HalStruct **Handle, AL_U32 DevId, AL_CIPHER_EventCallBack CallBack);

AL_S32 AlCipher_Hal_Start(AL_CIPHER_HalStruct *Handle, AL_CIPHER_CmdEnum Cmd, AL_CIPHER_ConfigUnion *Config);

AL_S32 AlCipher_Hal_StartBlock(AL_CIPHER_HalStruct *Handle, AL_CIPHER_CmdEnum Cmd, AL_CIPHER_ConfigUnion *Config,
                                   AL_U32 Timeout);

#ifdef __cplusplus
}
#endif

#endif
