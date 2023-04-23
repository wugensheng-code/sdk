#include "mpu_test.h"

#include <string.h>

AL_S32 AL_MPU_Ddrs0MpuTest(void)
{
    AL_MPU_RegionConfigStruct Config;

    AL_REG32 Instance = (AL_REG32)MPU__MPU_DDRS0__BASE_ADDR;

    /* The actual start address is 0x0000, end address is 0x1FFF */
    memset(&Config, 0, sizeof(Config));
    Config.StartAddr = 0x0;
    Config.EndAddr = 0x1;

    AlMpu_Hal_MpuEnable(Instance);

    printf("DDRS0 MPU test start>>>\r\n");

    AlMpu_Test(Instance, &Config);

    printf("DDRS0 MPU test done<<<\r\n");

    AlMpu_Hal_MpuDisable(Instance);
}

AL_S32 AL_MPU_ApuMpuTest(void)
{
    AL_MPU_RegionConfigStruct Config;

    AL_REG32 Instance = (AL_REG32)MPU__MPU_APU__BASE_ADDR;

    /*
      Apu are configured to access ddrs0, so the protected address is the ddrs0 address.
      The actual start address is 0x0000, end address is 0x1FFF
    */
    memset(&Config, 0, sizeof(Config));
    Config.StartAddr = 0x0;
    Config.EndAddr = 0x1;

    AlMpu_Hal_MpuEnable(Instance);

    printf("APU MPU test start>>>\r\n");

    AlMpu_Test(Instance, &Config);

    printf("APU MPU test done<<<\r\n");

    AlMpu_Hal_MpuDisable(Instance);
}

AL_S32 AL_MPU_Ocms2MpuTest(void)
{
    AL_MPU_RegionConfigStruct Config;

    AL_REG32 Instance = (AL_REG32)MPU__MPU_OCMS2__BASE_ADDR;

    /*
      Configure the protected address of the ocm, using the last 128K.
      Since the code is running on the ocm, the protected address should not
      be in the area where the code is running.
    */
    memset(&Config, 0, sizeof(Config));
    Config.StartAddr = 0x61020;
    Config.EndAddr = 0x61021;

    AlMpu_Hal_MpuEnable(Instance);

    printf("OCMS2 MPU test start>>>\r\n");

    AlMpu_Test(Instance, &Config);

    printf("OCMS2 MPU test done<<<\r\n");

    AlMpu_Hal_MpuDisable(Instance);
}

AL_S32 AL_MPU_NpuMpuTest(void)
{
    AL_MPU_RegionConfigStruct Config;

    AL_REG32 Instance = (AL_REG32)MPU__MPU_NPU__BASE_ADDR;

    memset(&Config, 0, sizeof(Config));
    Config.StartAddr = 0x63e00;
    Config.EndAddr = 0x63e01;

    AlMpu_Hal_MpuEnable(Instance);

    printf("NPU MPU test start>>>\r\n");

    AlMpu_Test(Instance, &Config);

    printf("NPU MPU test done<<<\r\n");

    AlMpu_Hal_MpuDisable(Instance);
}

AL_S32 AL_MPU_Hp0Hp1MpuTest(void)
{
    AL_MPU_RegionConfigStruct Config;

    /* HP0 test */
    AL_REG32 Instance = (AL_REG32)MPU__MPU_HPM0__BASE_ADDR;

    memset(&Config, 0, sizeof(Config));
    Config.StartAddr = 0x100;
    Config.EndAddr = 0x101;

    AlMpu_Hal_MpuEnable(Instance);

    printf("HP0 MPU test start>>>\r\n");

    AlMpu_ConfigTest(Instance, &Config);

    printf("HP0 MPU test done<<<\r\n");

    AlMpu_Hal_MpuDisable(Instance);

    /* HP1 test */
    Instance = (AL_REG32)MPU__MPU_HPM1__BASE_ADDR;

    memset(&Config, 0, sizeof(Config));
    Config.StartAddr = 0x100;
    Config.EndAddr = 0x101;

    AlMpu_Hal_MpuEnable(Instance);

    printf("HP1 MPU test start>>>\r\n");

    AlMpu_ConfigTest(Instance, &Config);

    printf("HP1 MPU test done<<<\r\n");

    AlMpu_Hal_MpuDisable(Instance);
}

AL_S32 AL_MPU_Ddrs1MpuTest(void)
{
    AL_MPU_RegionConfigStruct Config;
    AL_REG32 Instance = (AL_REG32)MPU__MPU_DDRS1__BASE_ADDR;

    memset(&Config, 0, sizeof(Config));
    Config.StartAddr = 0x100;
    Config.EndAddr = 0x101;

    AlMpu_Hal_MpuEnable(Instance);

    printf("DDRS1 MPU test start>>>\r\n");

    AlMpu_ConfigTest(Instance, &Config);

    printf("DDRS1 MPU test done<<<\r\n");

    AlMpu_Hal_MpuDisable(Instance);
}

AL_S32 AL_MPU_ApuPrivilegeTest()
{
    AL_U8 PrivilegeTestLoop = 0;
    AL_MPU_RegionConfigStruct Config;

    AL_REG32 Instance = (AL_REG32)MPU__MPU_DDRS0__BASE_ADDR;

    memset(&Config, 0, sizeof(Config));
    Config.RegionNumber = MPU_COMMON_REGION_NUMBER1;
    Config.StartAddr = 0x0;
    Config.EndAddr   = 0x1;

    Config.RegionEnable = MPU_REGION_ENABLE;
    Config.Privilege = MPU_REGION_PRIVILEGE;

    AlMpu_Hal_ConfigRegion(Instance, &Config);

    AlMpu_Hal_MpuEnable(Instance);

    while (1) {
        printf("PrivilegeTestLoop = %d\r\n", PrivilegeTestLoop);
        printf("[APU PRIVILEGE]:[PASS]\r\n");
        PrivilegeTestLoop++;

        /*
         * readme: write this register will casue abort
         * and EL1 cpu does not initilize,
         * so if run to abort handler, this case pass;
         */

        *(unsigned int *)(Config.StartAddr * 0x1000 + 4) = 0x87654321;
    }

    AlMpu_Hal_DisableRegion(Instance, MPU_COMMON_REGION_NUMBER1);

    AlMpu_Hal_MpuDisable(Instance);

}

/*
  APU privilege test case, which runn at EL0 mode.
  need define APU_MPU_PRIVILEGE_TEST and SWITCH_TO_EL0_FROM_EL3
  in Makefile before test.
 */
#if defined (APU_MPU_PRIVILEGE_TEST) && defined(SWITCH_TO_EL0_FROM_EL3)
int main()
{
    AL_MPU_ApuPrivilegeTest();

    return 0;
}
#else

/*
  All of the test case were tested in EL1 mode.
 */
int main()
{
    /*
      All of the test case were tested in EL1 mode
      except for apu privilege test case, which runn at EL0.
     */
    AL_MPU_Ddrs0MpuTest();

    AL_MPU_ApuMpuTest();

    AL_MPU_Ocms2MpuTest();

    AL_MPU_NpuMpuTest();

    /*
       Because HP and DDRS1 MPU requires manual testing,
       Check only the register data after the driver API configuration
       to ensure that the driver API is working properly.
     */
    AL_MPU_Hp0Hp1MpuTest();
    AL_MPU_Ddrs1MpuTest();

    printf("[MPU]:[PASS]\r\n");

    return 0;
}
#endif