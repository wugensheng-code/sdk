/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_mpu_hal.h"

#include <stdio.h>
#include <string.h>

static AL_MPU_HalStruct *ApuHandle;

#define APU_MPU_REGION_SIZE     32

/**
 *
 * This function initializes the MPU configuration for the APU by setting up various regions with specific attributes
 * such as read/write permissions, security settings, privilege levels, and interrupt enablement. It then attempts
 * to initialize, enable, and disable the MPU using HAL (Hardware Abstraction Layer) functions. Additionally,
 * it configures individual MPU regions and enables/disables them to test the MPU's functionality. The function
 * logs the outcome of each operation to indicate success or failure.
 *
 * @return AL_OK if all operations are successful, an error code otherwise.
 */
AL_S32 AlMpu_ApuExample()
{
    AL_U32 RetValue = 0;
    AL_U32 Index;

    AL_MPU_RegionConfigStruct RegionConfigTest;
    AL_U32 StartAddr = 0x0;

    AL_MPU_RegionConfigStruct ApuRegionConfig[APU_MPU_REGION_SIZE];

    memset(&RegionConfigTest, 0, sizeof(RegionConfigTest));
    memset(ApuRegionConfig, 0, sizeof(ApuRegionConfig));

    RegionConfigTest.ReadWrite       = MPU_REGION_NOREADWRITE;
    RegionConfigTest.Secure          = MPU_REGION_SECURE;
    RegionConfigTest.Privilege       = MPU_REGION_PRIVILEGE;
    RegionConfigTest.GroupId         = MPU_GROUP_ID_APU;
    RegionConfigTest.InterruptEnable = MPU_REGION_INTERRUPT_ENABLE;

    for (Index = 0; Index < APU_MPU_REGION_SIZE; Index++) {
        memcpy(&ApuRegionConfig[Index], &RegionConfigTest, sizeof(RegionConfigTest));
        ApuRegionConfig[Index].StartAddr = StartAddr;
        ApuRegionConfig[Index].Size      = (AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE * 2);

        StartAddr = StartAddr + ApuRegionConfig[Index].Size;
    }

    /* AlMpu_Hal_ConfigInit */
    RetValue = AlMpu_Hal_ConfigInit(6, &ApuHandle, AL_NULL, ApuRegionConfig, 31);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_ConfigInit success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_ConfigInit failed\r\n");

    /* AlMpu_Hal_MpuDisable */
    RetValue = AlMpu_Hal_MpuDisable(ApuHandle);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_MpuDisable success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_MpuDisable failed\r\n");

    /* AlMpu_Hal_MpuEnable */
    RetValue = AlMpu_Hal_MpuEnable(ApuHandle);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_MpuEnable success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_MpuEnable failed\r\n");

    /* AlMpu_Hal_EnableRegion */
    RetValue = AlMpu_Hal_EnableRegion(ApuHandle, 30);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_EnableRegion success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_EnableRegion failed\r\n");

    /* AlMpu_Hal_DisableRegion */
    RetValue = AlMpu_Hal_DisableRegion(ApuHandle, 30);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_DisableRegion success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_DisableRegion failed\r\n");

    RetValue = AlMpu_Hal_ConfigRegionByRegionNum(ApuHandle, 31, &ApuRegionConfig[31]);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_ConfigRegionByRegionNum success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_ConfigRegionByRegionNum failed\r\n");

    return AL_OK;
}

/**
 *
 * This function calls AlMpu_ApuExample to configure and test the MPU for the APU. It then returns the status of the operation.
 *
 * @return 0 indicating successful execution.
 */
int main()
{
    AlMpu_ApuExample();

    return 0;
}