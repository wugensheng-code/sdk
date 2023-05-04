#include "al_mpu_hal.h"

#include <stdio.h>
#include <string.h>

int main()
{
    AL_U32 RetValue = 0;
    AL_U32 Index;
    AL_MPU_HalStruct ApuHandle;
    AL_MPU_HalStruct NpuHandle;
    AL_MPU_RegionConfigStruct RegionConfigTest;
    AL_U32 StartAddr = 0x0;

    AL_MPU_RegionConfigStruct ApuRegionConfig[32];
    AL_MPU_RegionConfigStruct NpuRegionConfig[8];

    memset(&RegionConfigTest, 0, sizeof(RegionConfigTest));
    memset(ApuRegionConfig, 0, sizeof(ApuRegionConfig));
    memset(NpuRegionConfig, 0, sizeof(NpuRegionConfig));

    RegionConfigTest.ReadWrite = MPU_REGION_NOREADWRITE;
    RegionConfigTest.Secure = MPU_REGION_SECURE;
    RegionConfigTest.Privilege = MPU_REGION_PRIVILEGE;
    RegionConfigTest.GroupId = MPU_GROUP_ID_APU;
    RegionConfigTest.InterruptEnable = MPU_REGION_INTERRUPT_ENABLE;

    for (Index = 0; Index < 32; Index++) {
        memcpy(&ApuRegionConfig[Index], &RegionConfigTest, sizeof(RegionConfigTest));
        ApuRegionConfig[Index].StartAddr = StartAddr;
        ApuRegionConfig[Index].Size = MPU_REGION_8K;

        StartAddr = StartAddr + MPU_REGION_8K;
    }
    RetValue = AlMpu_Hal_Init(6, &ApuHandle, AL_NULL, ApuRegionConfig, 32);
    if (RetValue == AL_OK)
        printf("APU AlMpu_Hal_Init success\r\n");
    else
        printf("APU AlMpu_Hal_Init failed\r\n");

    StartAddr = 0x63e00000;

    for (Index = 0; Index < 8; Index++) {
        memcpy(&NpuRegionConfig[Index], &RegionConfigTest, sizeof(RegionConfigTest));
        NpuRegionConfig[Index].StartAddr = StartAddr;
        NpuRegionConfig[Index].Size = MPU_REGION_8K;

        StartAddr = StartAddr + MPU_REGION_8K;
    }
    RetValue = AlMpu_Hal_Init(4, &NpuHandle, AL_NULL, NpuRegionConfig, 8);
    if (RetValue == AL_OK)
        printf("NPU AlMpu_Hal_Init success\r\n");
    else
        printf("NPU AlMpu_Hal_Init failed\r\n");

    while(1);

    return 0;
}