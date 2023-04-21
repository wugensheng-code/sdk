#include "mpu_test.h"

#ifdef SWITCH_TO_EL0_FROM_EL3
static AL_U8 CpuInPrivilegeMode = 0;
#else
static AL_U8 CpuInPrivilegeMode = 1;
#endif /* SWITCH_TO_EL0_FROM_EL3 */

#ifdef SUPPORT_NONSECURE
static AL_U8 CpuInSecureMode = 0;
#else
static AL_U8 CpuInSecureMode = 1;
#endif /* SUPPORT_NONSECURE */

#ifndef APU_MPU_PRIVILEGE_TEST
void do_sync_handle(struct pt_regs *pt_regs, unsigned int esr)
{
#if (defined _AARCH_64 || defined _aarch64_)
    asm volatile("mrs x25, elr_el1; add x25, x25, #0x04; msr elr_el1, x25" ::: "x25");
#else
    // Todo
#endif /* defined _AARCH_64 || defined _aarch64_ */
}
#endif /* APU_MPU_PRIVILEGE_TEST */

static AL_U32 AlMpu_ProtectCheck(AL_REG32 Instance, AL_MPU_RegionConfigStruct *Config)
{
    AL_U32 RetValue = 0;
    /* get the actual address */
    AL_U32 Addr = Config->StartAddr * (0x1000);

    AL_U32 ReadValueBeforeMpuDisable = 0x99999999;
    AL_U32 ReadValueAfterMpuDisable = 0x88888888;

    AL_U32 ModifyValue = 0xa5a5a5a5;
    AL_U32 InitialValue = 0x12345678;

    do {
        /* First disable the region */
        AlMpu_Hal_DisableRegion(Instance, Config->RegionNumber);

        /* Second Initializes the value for this address */
        *((volatile AL_U32 *)(long)(Addr)) = InitialValue;
    } while (*((volatile AL_U32 *)(long)(Addr)) != InitialValue);

    AlMpu_Hal_EnableRegion(Instance, Config->RegionNumber);

    /* write and read data bofore MPU disable*/
    *((volatile AL_U32 *)(Addr)) = ModifyValue;
    ReadValueBeforeMpuDisable = *((volatile AL_U32 *)(Addr));

    /* read after MPU disable */
    AlMpu_Hal_DisableRegion(Instance, Config->RegionNumber);
    ReadValueAfterMpuDisable = *((volatile AL_U32 *)(Addr));

    AlMpu_Hal_EnableRegion(Instance, Config->RegionNumber);

    /* check result */
    if (((Config->Secure == 1) && (CpuInSecureMode == 0)) ||
        ((Config->Privilege == 1) && (CpuInPrivilegeMode == 0)) ||
#if (defined _AARCH_64 || defined __aarch64__)
        (Config->GroupId == MPU_GROUP_ID_APU) ||
#else
        (Config->GroupId == MPU_GROUP_ID_RPU) ||
#endif
        (Config->ReadWrite == MPU_REGION_NOREADWRITE)) {
        if ((ReadValueBeforeMpuDisable != ModifyValue) && (ReadValueAfterMpuDisable != ModifyValue)) {
            printf("Test Case Pass!\r\n");
        } else {
            RetValue = 1;
            printf("Test Case Fail!\r\n");
        }
    } else if (Config->ReadWrite == MPU_REGION_WRITEONLY) {
        if ((ReadValueBeforeMpuDisable != ModifyValue) && (ReadValueAfterMpuDisable == ModifyValue)) {
            printf("Test Case Pass!\r\n");
        } else {
            RetValue = 1;
            printf("Test Case Fail!\r\n");
        }
    } else if (Config->ReadWrite == MPU_REGION_READONLY) {
        if ((ReadValueBeforeMpuDisable == InitialValue) && (ReadValueAfterMpuDisable == InitialValue)) {
            printf("Test Case Pass!\r\n");
        } else {
            RetValue = 1;
            printf("Test Case Fail!\r\n");
        }
    } else {
        if ((ReadValueBeforeMpuDisable == ModifyValue) && (ReadValueAfterMpuDisable == ModifyValue)) {
            printf("Test Case Pass!\r\n");
        } else {
            RetValue = 1;
            printf("Test Case Fail!\r\n");
        }
    }

    return RetValue;
}

AL_S32 AlMpu_Test(AL_REG32 Instance, AL_MPU_RegionConfigStruct *Config)
{
    AL_U8 SecureMode;
    AL_U8 PrivilegeMode;
    AL_U8 ReadWrite;
    AL_U8 RegionNumber;
    AL_U8 MaxRegionNumber = MPU_COMMON_REGION_NUMBER8;
    AL_U8 GroupId;

    /* APU MPU has 32 regions and others have 8 regions */
    if (Instance == MPU_APU_BASE_ADDR) {
        MaxRegionNumber = MPU_APU_REGION_NUMBER32;
    }

    for (RegionNumber = MPU_COMMON_REGION_NUMBER1; RegionNumber <= MaxRegionNumber; RegionNumber++) {
        for (int PrivilegeMode = 0; PrivilegeMode <= MPU_REGION_UNPRIVILEGE; PrivilegeMode++) {
            for (int SecureMode = 0; SecureMode <= MPU_REGION_SECURE; SecureMode++) {
                for (GroupId = 0; GroupId <= MAX_GROUPID_NUMBER; GroupId++) {
                    for (ReadWrite = MPU_REGION_READWRITE; ReadWrite <= MPU_REGION_NOREADWRITE; ReadWrite++) {

                        Config->RegionNumber = RegionNumber;
                        Config->RegionEnable = MPU_REGION_ENABLE;
                        Config->Secure = SecureMode;
                        Config->Privilege = PrivilegeMode;
                        Config->ReadWrite = ReadWrite;
                        Config->GroupId = (1 << GroupId);

                        AlMpu_Hal_ConfigRegion(Instance, Config);
                        AlMpu_ProtectCheck(Instance, Config);

                        /* region test done, disable the current region */
                        AlMpu_Hal_DisableRegion(Instance, RegionNumber);
                    }
                }
            }
        }

        /* Config the address for next region */
        Config->StartAddr = Config->EndAddr + 0x1;
        Config->EndAddr   = Config->StartAddr + 0x1;
    }

    return 0;
}

static AL_U32 AlMpu_ProtectConfigCheck(AL_REG32 Instance, AL_MPU_RegionConfigStruct *Config)
{
    AL_MPU_RegionEnEnum RegionEnable;
    AL_MPU_RegionSecureEnum Secure;
    AL_MPU_RegionPrivilegeEnum Privilege;
    AL_MPU_RegionReadWriteEnum ReadWrite;
    AL_MPU_RegionInterruptEnEnum InterruptEnable;
    AL_U32 StartAddr;
    AL_U32 EndAddr;
    AL_U32 GroupId;
    AL_U32 RegisterRASA;

    AL_REG32 RegionBaseAddr = Instance + (0x10 * (Config->RegionNumber));

    StartAddr = *((volatile AL_U32 *)(RegionBaseAddr + 0x4));
    EndAddr = *((volatile AL_U32 *)(RegionBaseAddr + 0x8));
    GroupId = *((volatile AL_U32 *)(RegionBaseAddr + 0xC));

    RegisterRASA = *((volatile AL_U32 *)(RegionBaseAddr + 0x0));

    RegionEnable = RegisterRASA & 0x1;
    Secure = (RegisterRASA >> 6) & 0x1;
    Privilege = (RegisterRASA >> 7) & 0x1;
    ReadWrite = (RegisterRASA >> 8) & 0x3;
    InterruptEnable = (RegisterRASA >> 12) & 0x1;

    /* Compare attribute */
    if ((RegionEnable != Config->RegionEnable)       ||
        (Secure != Config->Secure)                   ||
        (Privilege != Config->Privilege)             ||
        (ReadWrite != Config->ReadWrite)             ||
        (InterruptEnable != Config->InterruptEnable) ||
        (StartAddr != Config->StartAddr)             ||
        (EndAddr != Config->EndAddr)                 ||
        (GroupId != Config->GroupId)) {
            printf("Test case fail\r\n");
        }

    printf("Test case pass\r\n");

    return 0;
}

AL_S32 AlMpu_ConfigTest(AL_REG32 Instance, AL_MPU_RegionConfigStruct *Config)
{
    AL_U8 SecureMode;
    AL_U8 PrivilegeMode;
    AL_U8 ReadWrite;
    AL_U8 RegionNumber;
    AL_U8 MaxRegionNumber = MPU_COMMON_REGION_NUMBER8;
    AL_U8 GroupId;

    /* APU MPU has 32 regions and others have 8 regions */
    if (Instance == MPU_APU_BASE_ADDR) {
        MaxRegionNumber = MPU_APU_REGION_NUMBER32;
    }

    for (RegionNumber = MPU_COMMON_REGION_NUMBER1; RegionNumber <= MaxRegionNumber; RegionNumber++) {
        for (int PrivilegeMode = 0; PrivilegeMode <= MPU_REGION_UNPRIVILEGE; PrivilegeMode++) {
            for (int SecureMode = 0; SecureMode <= MPU_REGION_SECURE; SecureMode++) {
                for (GroupId = 0; GroupId <= MAX_GROUPID_NUMBER; GroupId++) {
                    for (ReadWrite = MPU_REGION_READWRITE; ReadWrite <= MPU_REGION_NOREADWRITE; ReadWrite++) {

                        Config->RegionNumber = RegionNumber;
                        Config->RegionEnable = MPU_REGION_ENABLE;
                        Config->Secure = SecureMode;
                        Config->Privilege = PrivilegeMode;
                        Config->ReadWrite = ReadWrite;
                        Config->GroupId = (1 << GroupId);

                        AlMpu_Hal_ConfigRegion(Instance, Config);

                        /* For manual test case, only check the register configure */
                        AlMpu_ProtectConfigCheck(Instance, Config);

                        /* region test done, disable the current region */
                        AlMpu_Hal_DisableRegion(Instance, RegionNumber);
                    }
                }
            }
        }

        /* Config the address for next region */
        Config->StartAddr = Config->EndAddr + 0x1;
        Config->EndAddr   = Config->StartAddr + 0x1;
    }

    return 0;
}