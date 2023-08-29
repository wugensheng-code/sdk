/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_SMC_HAL_H
#define AL_SMC_HAL_H

#ifdef __cplusplus
extern "C" {
#endif
#include "al_smc_dev.h"

struct AL_SMC_DevStruct;

typedef struct
{
    AL_SMC_DevStruct            *Dev;
    AL_NAND_InfoStruct          *NandInfo;

// #ifdef USE_RTOS
//     AL_Lock                      Lock;
// #endif

} AL_SMC_HalStruct;

AL_U32 AlSmc_Hal_Init(AL_SMC_HalStruct *Handle, AL_SMC_ConfigsStruct *InitConfig, AL_U32 DevId);
AL_U32 AlSmc_Hal_ReadPage(AL_SMC_HalStruct *Handle, AL_U32 Addr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_U32 AlSmc_Hal_WritePage(AL_SMC_HalStruct *Handle, AL_U32 Addr, AL_U8 *Data, AL_U32 Size, AL_U32 Timeout);
AL_U32 AlSmc_Hal_ReadSpare(AL_SMC_HalStruct *Handle, AL_U32 Page, AL_U8 *Data, AL_U32 Timeout);
AL_U32 AlSmc_Hal_WriteSpare(AL_SMC_HalStruct *Handle, AL_U32 Page, AL_U8 *Data, AL_U32 Timeout);
AL_U32 AlSmc_Hal_EraseBlock(AL_SMC_HalStruct *Handle, AL_U32 Addr, AL_U32 Timeout);




#ifdef __cplusplus
}
#endif
#endif
