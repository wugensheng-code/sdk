/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_mpu_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_MPU_HwConfigStruct AlMpu_HwConfig[AL_MPU_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .BaseAddress    = MPU_DDRS0_BASE_ADDR,
    },

    {
        .DeviceId       = 1,
        .BaseAddress    = MPU_DDRS1_BASE_ADDR,
    },

    {
        .DeviceId       = 2,
        .BaseAddress    = MPU_HPM0_BASE_ADDR,
    },

    {
        .DeviceId       = 3,
        .BaseAddress    = MPU_HPM1_BASE_ADDR,
    },

    {
        .DeviceId       = 4,
        .BaseAddress    = MPU_NPU_BASE_ADDR,
    },

    {
        .DeviceId       = 5,
        .BaseAddress    = MPU_OCMS2_BASE_ADDR,
    },

    {
        .DeviceId       = 6,
        .BaseAddress    = MPU_APU_BASE_ADDR,
    }
};