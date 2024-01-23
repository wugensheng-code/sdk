/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_mpu_hal.h"

#include <stdio.h>
#include <string.h>

static AL_MPU_HalStruct *NpuHandle;

#define NPU_MPU_DEVICE_ID       4

int main()
{
    AL_LOG(AL_LOG_LEVEL_INFO, "NPU MPU config example\r\n");

    AlMpu_Hal_Init();
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_U32 RetValue = 0;
    AL_MPU_RegionConfigStruct RegionConfig;

    memset(&RegionConfig, 0, sizeof(RegionConfig));

    RegionConfig.StartAddr = 0x63e00000;
    RegionConfig.Size = AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE*2;
    RegionConfig.ReadWrite       = MPU_REGION_READONLY;
    RegionConfig.Secure          = MPU_REGION_NONSECURE;
    RegionConfig.Privilege       = MPU_REGION_UNPRIVILEGE;
    RegionConfig.GroupId         = MPU_GROUP_ID_APU | MPU_GROUP_ID_RPU;
    RegionConfig.InterruptEnable = MPU_REGION_INTERRUPT_DISABLE;

    /* AlMpu_Hal_ConfigInit */
    RetValue = AlMpu_Hal_ConfigInit(NPU_MPU_DEVICE_ID, &NpuHandle, AL_NULL, &RegionConfig, 1);
    if (RetValue != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlMpu_Hal_ConfigInit failed\r\n");
        return RetValue;
    }

    /* This line of code will trigger an exception, indicating that mpu protection is successful */
    *((volatile int *)(0x63e00000+0x8)) = 0x12345678;

    return 0;
}