/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_MMC_HAL_H_
#define __AL_MMC_HAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_mmc_dev.h"

typedef struct
{
    AL_MMC_DevStruct    *Dev;
} AL_MMC_HalStruct;

AL_S32 AlMmc_Hal_Init(AL_MMC_HalStruct *Handle, AL_U32 DevId, AL_MMC_InitStruct *InitConfig,
                      AL_MMC_CallBackStruct *CallBack);

AL_S32 AlMmc_Hal_WriteBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout);

AL_S32 AlMmc_Hal_ReadBlocked(AL_MMC_HalStruct *Handle, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt, AL_U32 Timeout);

#ifdef __cplusplus
}
#endif

#endif
