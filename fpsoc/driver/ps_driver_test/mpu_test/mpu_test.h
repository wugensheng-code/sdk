#ifndef MPU_TEST_H
#define MPU_TEST_H

#include "al_mpu_hal.h"

/* CSU、RPU、APU、NPU、JPU、DMA_AXI group id */
#define MAX_GROUPID_NUMBER      5

AL_S32 AlMpu_Test(AL_REG32 Instance, AL_MPU_RegionConfigStruct *Config);

AL_S32 AlMpu_ManualTest(AL_REG32 Instance, AL_MPU_RegionConfigStruct *Config);

#endif /* MPU_TEST_H */