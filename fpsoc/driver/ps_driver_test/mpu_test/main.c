#include "al_mpu_hal.h"

int main()
{
    AL_MPU_RegionConfigStruct Config;

    AL_REG32 Instance = (AL_REG32)MPU_DDRS0_BASE_ADDR;

    Config.RegionNumber = MPU_COMMON_REGION_NUMBER1;
    Config.StartAddr = 0x0;
    Config.EndAddr   = 0x1;

    Config.RegionEnable = MPU_REGION_ENABLE;
    Config.Secure = MPU_REGION_SECURE;
    Config.Priviledge = MPU_REGION_PRIVILEDGE;
    Config.ReadWrite = MPU_REGION_NOREADWRITE;
    Config.InterruptEnable = MPU_REGION_INTERRUPT_DISABLE;

    Config.GroupId = MPU_GROUP_ID_APU;

    AlMpu_Hal_ConfigRegion(Instance, &Config);

    AlMpu_Hal_MpuEnable(Instance);

    AlMpu_Hal_DisableRegion(Instance, MPU_COMMON_REGION_NUMBER1);

    AlMpu_Hal_MpuDisable(Instance);
}