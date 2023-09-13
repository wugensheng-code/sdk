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

#include "al_mpu_dev.h"

typedef struct
{
    AL_MPU_DevStruct    *Dev;
    AL_Lock             Lock;
} AL_MPU_HalStruct;

AL_VOID AlMpu_Hal_Init();

AL_S32 AlMpu_Hal_ConfigInit(AL_U8 MpuDevId, AL_MPU_HalStruct **Handle, AL_Mpu_EventCallBack EventCallBack,
                            AL_MPU_RegionConfigStruct *InitRegionConfig, AL_U8 ConfigNumber);

AL_S32 AlMpu_Hal_MpuEnable(AL_MPU_HalStruct *Handle);

AL_S32 AlMpu_Hal_MpuDisable(AL_MPU_HalStruct *Handle);

AL_S32 AlMpu_Hal_EnableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber);

AL_S32 AlMpu_Hal_DisableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber);

AL_S32 AlMpu_Hal_ConfigRegion(AL_MPU_HalStruct *Handle, AL_MPU_RegionConfigStruct *InitRegionConfig);

AL_S32 AlMpu_Hal_ConfigRegionByRegionNum(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber,
                                         AL_MPU_RegionConfigStruct *InitRegionConfig);

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_HAL_H */
