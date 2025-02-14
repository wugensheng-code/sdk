#include "common.h"

static AL_MPU_HalStruct *ApuHandle;
#define APU_MPU_REGION_SIZE     32

AL_VOID AlMpu_ApuExample()
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
    assert_int_equal(RetValue, AL_OK);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_ConfigInit success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_ConfigInit failed\r\n");

    /* AlMpu_Hal_MpuDisable */
    RetValue = AlMpu_Hal_MpuDisable(ApuHandle);
    assert_int_equal(RetValue, AL_OK);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_MpuDisable success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_MpuDisable failed\r\n");

    /* AlMpu_Hal_MpuEnable */
    RetValue = AlMpu_Hal_MpuEnable(ApuHandle);
    assert_int_equal(RetValue, AL_OK);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_MpuEnable success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_MpuEnable failed\r\n");

    /* AlMpu_Hal_EnableRegion */
    RetValue = AlMpu_Hal_EnableRegion(ApuHandle, 30);
    assert_int_equal(RetValue, AL_OK);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_EnableRegion success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_EnableRegion failed\r\n");

    /* AlMpu_Hal_DisableRegion */
    RetValue = AlMpu_Hal_DisableRegion(ApuHandle, 30);
    assert_int_equal(RetValue, AL_OK);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_DisableRegion success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_DisableRegion failed\r\n");

    RetValue = AlMpu_Hal_ConfigRegionByRegionNum(ApuHandle, 31, &ApuRegionConfig[31]);
    assert_int_equal(RetValue, AL_OK);
    if (RetValue == AL_OK)
        printf("[TEST] APU AlMpu_Hal_ConfigRegionByRegionNum success\r\n");
    else
        printf("[TEST] APU AlMpu_Hal_ConfigRegionByRegionNum failed\r\n");

}

CMOCKA_TC_DEFINE(AlMpu_ApuExample, NULL, NULL, NULL);