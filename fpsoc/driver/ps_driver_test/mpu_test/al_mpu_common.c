/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_mpu_hal.h"

#include <stdio.h>
#include <string.h>


#ifdef SWITCH_TO_EL1_EL0_FROM_EL3
static AL_U8 CpuInPrivilegeMode = 0;
#else
static AL_U8 CpuInPrivilegeMode = 1;
#endif /* SWITCH_TO_EL0_FROM_EL3 */

#ifdef SUPPORT_NONSECURE
static AL_U8 CpuInSecureMode = 0;
#else
static AL_U8 CpuInSecureMode = 1;
#endif /* SUPPORT_NONSECURE */

void do_sync_handle(AL_UINTPTR *Regs)
{
#if defined __aarch64__
    asm volatile("mrs x25, elr_el1; add x25, x25, #0x04; msr elr_el1, x25" ::: "x25");
#else
    // Todo
#endif /* defined __aarch64__*/
}

static AL_U32 AlMpu_ProtectCheck(AL_MPU_HalStruct *Handle, AL_U8 RegionNumber, AL_MPU_RegionConfigStruct *Config)
{
    AL_U32 RetValue = 0;
    /* get the actual address */
    AL_U32 Addr = Config->StartAddr;

    AL_U32 ReadValueBeforeMpuDisable = 0x99999999;
    AL_U32 ReadValueAfterMpuDisable = 0x88888888;

    AL_U32 ModifyValue = 0xa5a5a5a5;
    AL_U32 InitialValue = 0x12345678;

    do {
        /* First disable the region */
        AlMpu_Hal_DisableRegion(Handle, RegionNumber);

        /* Second Initializes the value for this address */
        *((volatile AL_U32 *)(long)(Addr)) = InitialValue;

#ifdef ENABLE_MMU
        AlCache_FlushDcacheAll();
#endif
    } while (*((volatile AL_U32 *)(long)(Addr)) != InitialValue);

    AlMpu_Hal_EnableRegion(Handle, RegionNumber);

    /* write and read data bofore MPU disable*/
    *((volatile AL_U32 *)(Addr)) = ModifyValue;

#ifdef ENABLE_MMU
    AlCache_FlushDcacheAll();
#endif

    ReadValueBeforeMpuDisable = *((volatile AL_U32 *)(Addr));

    /* read after MPU disable */
    AlMpu_Hal_DisableRegion(Handle, RegionNumber);

    ReadValueAfterMpuDisable = *((volatile AL_U32 *)(Addr));

    AlMpu_Hal_EnableRegion(Handle, RegionNumber);

    /* check result */
    if (((Config->Secure == 1) && (CpuInSecureMode == 0)) ||
        ((Config->Privilege == 1) && (CpuInPrivilegeMode == 0)) ||
#if defined __aarch64__
        (Config->GroupId == MPU_GROUP_ID_APU) ||
#else
        (Config->GroupId == MPU_GROUP_ID_RPU) ||
#endif
        (Config->ReadWrite == MPU_REGION_NOREADWRITE)) {
        if ((ReadValueBeforeMpuDisable != ModifyValue) && (ReadValueAfterMpuDisable != ModifyValue)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_ProtectCheck:Test Case Pass!\r\n");
        } else {
            RetValue = 1;
            AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_ProtectCheck:Test Case Fail!\r\n");
        }
    } else if (Config->ReadWrite == MPU_REGION_WRITEONLY) {
        if ((ReadValueBeforeMpuDisable != ModifyValue) && (ReadValueAfterMpuDisable == ModifyValue)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_ProtectCheck:Test Case Pass!\r\n");
        } else {
            RetValue = 1;
            AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_ProtectCheck:Test Case Fail!\r\n");
        }
    } else if (Config->ReadWrite == MPU_REGION_READONLY) {
        if ((ReadValueBeforeMpuDisable == InitialValue) && (ReadValueAfterMpuDisable == InitialValue)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_ProtectCheck:Test Case Pass!\r\n");
        } else {
            RetValue = 1;
            AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_ProtectCheck:Test Case Fail!\r\n");
        }
    } else {
        if ((ReadValueBeforeMpuDisable == ModifyValue) && (ReadValueAfterMpuDisable == ModifyValue)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_ProtectCheck:Test Case Pass!\r\n");
        } else {
            RetValue = 1;
            AL_LOG(AL_LOG_LEVEL_INFO, "AlMpu_ProtectCheck:Test Case Fail!\r\n");
        }
    }

    return RetValue;
}

AL_S32 AlMpu_ProtectTest(AL_MPU_HalStruct *Handle, AL_MPU_RegionConfigStruct *RegionConfig)
{
    AL_U8 SecureMode;
    AL_U8 PrivilegeMode;
    AL_U8 ReadWrite;
    AL_U8 RegionNumber;
    AL_U8 MaxRegionNumber = AL_MPU_APU_MAX_REGION_NUMBER;
    AL_U8 GroupId;

    AL_MPU_RegionConfigStruct Config;

    memset(&Config, 0, sizeof(Config));

    Config.StartAddr       = RegionConfig->StartAddr;
    Config.Size            = (AL_MPU_DEFAULT_REGION_GRANULARITY_SIZE * 2);
    Config.InterruptEnable = RegionConfig->InterruptEnable;

    if (Handle->Dev->HwConfig.DeviceId != 6) {
        MaxRegionNumber = AL_MPU_COMMON_MAX_REGION_NUMBER;
    }

    /* 1. Before test, disable all region */
    for (RegionNumber = 0; RegionNumber <= MaxRegionNumber; RegionNumber++) {
        AlMpu_Hal_DisableRegion(Handle, RegionNumber);
    }

    for (RegionNumber = AL_MPU_COMMON_REGION_0; RegionNumber <= MaxRegionNumber; RegionNumber++) {
        for (int PrivilegeMode = 0; PrivilegeMode <= MPU_REGION_PRIVILEGE; PrivilegeMode++) {
            for (int SecureMode = 0; SecureMode <= MPU_REGION_SECURE; SecureMode++) {
                for (GroupId = 0; GroupId < 6; GroupId++) {
                    for (ReadWrite = MPU_REGION_READWRITE; ReadWrite <= MPU_REGION_NOREADWRITE; ReadWrite++) {

                        Config.Secure = SecureMode;
                        Config.Privilege = PrivilegeMode;
                        Config.ReadWrite = ReadWrite;
                        Config.GroupId = (1 << GroupId);

                        AlMpu_Hal_ConfigRegionByRegionNum(Handle, RegionNumber, &Config);
                        AlMpu_ProtectCheck(Handle, RegionNumber, &Config);

                        /* region test done, disable the current region */
                        AlMpu_Hal_DisableRegion(Handle, RegionNumber);
                    }
                }
            }
        }

        /* Config the address for next region */
        Config.StartAddr = Config.StartAddr + 4096;
    }

    return AL_OK;
}