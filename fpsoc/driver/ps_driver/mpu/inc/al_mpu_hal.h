#ifndef AL_MPU_HAL_H
#define AL_MPU_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_mpu_dev.h"

typedef struct
{
    AL_MPU_DevStruct    *Dev;

#ifdef USE_RTOS
    AL_Lock             Lock;
#endif
} AL_MPU_HalStruct;

AL_S32 AlMpu_Hal_Init(AL_U8 MpuDevId, AL_MPU_HalStruct *Handle, AL_Mpu_EventCallBack CallBack,
                      AL_MPU_RegionConfigStruct *InitRegionConfig, AL_U8 ConfigNumber);

AL_S32 AlMpu_Hal_MpuEnable(AL_MPU_HalStruct *Handle);

AL_S32 AlMpu_Hal_MpuDisable(AL_MPU_HalStruct *Handle);

AL_S32 AlMpu_Hal_DisableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber);

AL_S32 AlMpu_Hal_EnableRegion(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber,
                              AL_MPU_RegionConfigStruct *InitRegionConfig);

AL_VOID AlMpu_Hal_MpuRegisterIntr();

#ifdef __cplusplus
}
#endif

#endif /* AL_MPU_HAL_H */
