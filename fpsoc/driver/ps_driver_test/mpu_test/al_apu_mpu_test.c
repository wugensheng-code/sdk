/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_mpu_hal.h"

#include <stdio.h>
#include <string.h>

#define APU_MPU_DEVICE_ID       6

static AL_MPU_HalStruct *ApuHandle;

int main()
{
    AL_LOG(AL_LOG_LEVEL_INFO, "APU MPU config example\r\n");

    AlMpu_Hal_Init();
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AL_U32 RetValue = 0;
    AL_MPU_RegionConfigStruct RegionConfig;

    memset(&RegionConfig, 0, sizeof(RegionConfig));

    RegionConfig.StartAddr = 0x0;
    RegionConfig.Size      = AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE;
    RegionConfig.ReadWrite       = MPU_REGION_NOREADWRITE;
    RegionConfig.Secure          = MPU_REGION_SECURE;
    RegionConfig.Privilege       = MPU_REGION_PRIVILEGE;
    RegionConfig.InterruptEnable = MPU_REGION_INTERRUPT_DISABLE;

    /* AlMpu_Hal_ConfigInit */
    RetValue = AlMpu_Hal_ConfigInit(APU_MPU_DEVICE_ID, &ApuHandle, AL_NULL, &RegionConfig, 1);
    if (RetValue != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlMpu_Hal_ConfigInit failed\r\n");
        return RetValue;
    }

    AlMpu_ProtectTest(ApuHandle, &RegionConfig);

    return 0;
}