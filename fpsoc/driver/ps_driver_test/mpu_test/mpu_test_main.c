#include "al_mpu_hal.h"

#include <stdio.h>
#include <string.h>

AL_MPU_HalStruct ApuHandle;
AL_MPU_HalStruct NpuHandle;

static AL_U32 AlMpu_RegisterConfigCheck(AL_REG MpuBaseAddr,  AL_U8 RegionNumber, AL_MPU_RegionConfigStruct *Config)
{
    AL_MPU_RegionEnEnum RegionEnable;
    AL_MPU_RegionSecureEnum Secure;
    AL_MPU_RegionPrivilegeEnum Privilege;
    AL_MPU_RegionReadWriteEnum ReadWrite;
    AL_MPU_RegionInterruptEnEnum InterruptEnable;
    AL_U32 StartAddr;
    AL_U32 EndAddr;
    AL_U32 Size;
    AL_U32 GroupId;
    AL_U32 RegisterRASA;

    AL_REG RegionBaseAddr = (MpuBaseAddr + 0x10) + (0x10 * RegionNumber);

    StartAddr = *((volatile AL_U32 *)(RegionBaseAddr + 0x4));
    EndAddr = *((volatile AL_U32 *)(RegionBaseAddr + 0x8));
    Size = (EndAddr - StartAddr + 1) * 4096;
    StartAddr = StartAddr << 12;

    GroupId = *((volatile AL_U32 *)(RegionBaseAddr + 0xC));

    RegisterRASA = *((volatile AL_U32 *)(RegionBaseAddr + 0x0));

    RegionEnable = RegisterRASA & 0x1;
    Secure = (RegisterRASA >> 6) & 0x1;
    Privilege = (RegisterRASA >> 7) & 0x1;
    ReadWrite = (RegisterRASA >> 8) & 0x3;
    InterruptEnable = (RegisterRASA >> 12) & 0x1;

    /* Compare attribute */
    if ((Secure != Config->Secure)                   ||
        (Privilege != Config->Privilege)             ||
        (ReadWrite != Config->ReadWrite)             ||
        (InterruptEnable != Config->InterruptEnable) ||
        (StartAddr != Config->StartAddr)             ||
        (Size != Config->Size)                       ||
        (GroupId != Config->GroupId)) {
            return 1;
        }

    return 0;
}

AL_S32 AlMpu_ApuTest()
{
    AL_U32 RetValue = 0;
    AL_U32 Index;

    AL_MPU_RegionConfigStruct RegionConfigTest;
    AL_U32 StartAddr = 0x0;

    AL_MPU_RegionConfigStruct ApuRegionConfig[32];

    memset(&RegionConfigTest, 0, sizeof(RegionConfigTest));
    memset(ApuRegionConfig, 0, sizeof(ApuRegionConfig));

    RegionConfigTest.ReadWrite       = MPU_REGION_NOREADWRITE;
    RegionConfigTest.Secure          = MPU_REGION_SECURE;
    RegionConfigTest.Privilege       = MPU_REGION_PRIVILEGE;
    RegionConfigTest.GroupId         = MPU_GROUP_ID_APU;
    RegionConfigTest.InterruptEnable = MPU_REGION_INTERRUPT_ENABLE;

    for (Index = 0; Index < 32; Index++) {
        memcpy(&ApuRegionConfig[Index], &RegionConfigTest, sizeof(RegionConfigTest));
        ApuRegionConfig[Index].StartAddr = StartAddr;
        ApuRegionConfig[Index].Size      = (AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE * 2);

        StartAddr = StartAddr + ApuRegionConfig[Index].Size;
    }

    /* 1. Test AlMpu_Hal_ConfigInit */
    RetValue = AlMpu_Hal_ConfigInit(6, &ApuHandle, AL_NULL, ApuRegionConfig, 32);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_ConfigInit success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_ConfigInit failed\r\n");

    /* 2. Test AlMpu_Hal_EnableRegion */
    RetValue = AlMpu_Hal_EnableRegion(&ApuHandle, 31);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_EnableRegion success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_EnableRegion failed\r\n");

    /* 3. Test AlMpu_Hal_DisableRegion */
    RetValue = AlMpu_Hal_DisableRegion(&ApuHandle, 31);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_DisableRegion success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_DisableRegion failed\r\n");

    /* 4. Test AlMpu_Hal_EnableRegion, RetValue == AL_OK Indicate test success, because step 3 disable the region */
    RetValue = AlMpu_Hal_EnableRegion(&ApuHandle, 31);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_EnableRegion success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_EnableRegion failed\r\n");

    /* 4. Check whether the register configuration of each region is correct */
    for (Index = 0; Index < 32; Index++) {
        RetValue = AlMpu_RegisterConfigCheck(ApuHandle.Dev->HwConfig.BaseAddress, Index, &ApuRegionConfig[Index]);
        if (RetValue != AL_OK)
            printf("[TEST] APU AlMpu_RegisterConfigCheck region%d failed\r\n", Index);
        else
            printf("[TEST] APU AlMpu_RegisterConfigCheck region%d success\r\n", Index);
    }

    return RetValue;
}

AL_S32 AlMpu_NpuTest()
{
    AL_U32 RetValue = 0;
    AL_U32 Index;
    AL_MPU_RegionConfigStruct RegionConfigTest;
    AL_U32 StartAddr = 0x0;

    AL_MPU_RegionConfigStruct NpuRegionConfig[8];

    memset(&RegionConfigTest, 0, sizeof(RegionConfigTest));
    memset(NpuRegionConfig, 0, sizeof(NpuRegionConfig));

    RegionConfigTest.ReadWrite       = MPU_REGION_NOREADWRITE;
    RegionConfigTest.Secure          = MPU_REGION_SECURE;
    RegionConfigTest.Privilege       = MPU_REGION_PRIVILEGE;
    RegionConfigTest.GroupId         = MPU_GROUP_ID_APU;
    RegionConfigTest.InterruptEnable = MPU_REGION_INTERRUPT_ENABLE;

    StartAddr = 0x63e00000;

    for (Index = 0; Index < 6; Index++) {
        memcpy(&NpuRegionConfig[Index], &RegionConfigTest, sizeof(RegionConfigTest));
        NpuRegionConfig[Index].StartAddr = StartAddr;
        NpuRegionConfig[Index].Size      = (AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE * 2);

        StartAddr = StartAddr +  NpuRegionConfig[Index].Size;
    }

    /* 1. Test AlMpu_Hal_ConfigInit */
    RetValue = AlMpu_Hal_ConfigInit(4, &NpuHandle, AL_NULL, NpuRegionConfig, 6);
    if (RetValue == AL_OK)
        printf("NPU AlMpu_Hal_ConfigInit success\r\n");
    else
        printf("NPU AlMpu_Hal_ConfigInit failed\r\n");

    memcpy(&NpuRegionConfig[6], &RegionConfigTest, sizeof(RegionConfigTest));
    NpuRegionConfig[6].StartAddr = StartAddr;
    NpuRegionConfig[6].Size      = (AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE * 2);

    /* 2. Test AlMpu_Hal_ConfigRegion */
    RetValue = AlMpu_Hal_ConfigRegion(&NpuHandle, &NpuRegionConfig[6]);
    if (RetValue == AL_OK)
        printf("[TEST] NPU AlMpu_Hal_ConfigRegion success\r\n");
    else
        printf("[TEST] NPU AlMpu_Hal_ConfigRegion failed\r\n");

    StartAddr = StartAddr +  NpuRegionConfig[6].Size;
    memcpy(&NpuRegionConfig[7], &RegionConfigTest, sizeof(RegionConfigTest));
    NpuRegionConfig[7].StartAddr = StartAddr;
    NpuRegionConfig[7].Size      = (AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE * 2);

    /* 3. Test AlMpu_Hal_ConfigRegionByRegionNum */
    RetValue = AlMpu_Hal_ConfigRegionByRegionNum(&NpuHandle, 7, &NpuRegionConfig[7]);
    if (RetValue == AL_OK)
        printf("[TEST] NPU AlMpu_Hal_ConfigRegionByRegionNum success\r\n");
    else
        printf("[TEST] NPU AlMpu_Hal_ConfigRegionByRegionNum failed\r\n");

    /* 4. Check whether the register configuration of each region is correct */
    for (Index = 0; Index < 8; Index++) {
        RetValue = AlMpu_RegisterConfigCheck(NpuHandle.Dev->HwConfig.BaseAddress, Index, &NpuRegionConfig[Index]);
        if (RetValue != AL_OK)
            printf("[TEST] APU AlMpu_RegisterConfigCheck region%d failed\r\n", Index);
        else
            printf("[TEST] APU AlMpu_RegisterConfigCheck region%d success\r\n", Index);
    }

    return RetValue;
}

int main()
{
    AlMpu_Hal_Init();

    printf("[TEST]AlMpu_ApuTest satrt\r\n");
    AlMpu_ApuTest();
    printf("[TEST]AlMpu_ApuTest done\r\n");
    printf("\r\n");

    printf("[TEST]AlMpu_NpuTest satrt\r\n");
    AlMpu_NpuTest();
    printf("[TEST]AlMpu_NpuTest done\r\n");
    printf("\r\n");

    while(1);

    return 0;
}