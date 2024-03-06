/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_xmon_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_XMON_HwConfigStruct AlXmon_HwConfig[AL_XMON_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .BaseAddress        = AL_XMON_DDR_S0_BASE_ADDR,
    },

    {
        .DeviceId           = 1,
        .BaseAddress        = AL_XMON_DDR_S1_BASE_ADDR,
    },

    {
        .DeviceId           = 2,
        .BaseAddress        = AL_XMON_DDR_S2_BASE_ADDR,
    },

    {
        .DeviceId           = 3,
        .BaseAddress        = AL_XMON_DDR_S3_BASE_ADDR,
    },

    {
        .DeviceId           = 4,
        .BaseAddress        = AL_XMON_SX2X_M0_BASE_ADDR,
    },

    {
        .DeviceId           = 5,
        .BaseAddress        = AL_XMON_SX2X_M1_BASE_ADDR,
    },

    {
        .DeviceId           = 6,
        .BaseAddress        = AL_XMON_OCM_S2_BASE_ADDR,
    },

    {
        .DeviceId           = 7,
        .BaseAddress        = AL_XMON_SH_M2_BASE_ADDR,
    },

    {
        .DeviceId           = 8,
        .BaseAddress        = AL_XMON_SMC_BASE_ADDR,
    },

    {
        .DeviceId           = 9,
        .BaseAddress        = AL_XMON_DMACX_BASE_ADDR,
    },

    {
        .DeviceId           = 10,
        .BaseAddress        = AL_XMON_GP_S0_BASE_ADDR,
    },

    {
        .DeviceId           = 11,
        .BaseAddress        = AL_XMON_GP_S1_BASE_ADDR,
    },

    {
        .DeviceId           = 12,
        .BaseAddress        = AL_XMON_HP_S0_BASE_ADDR,
    },

    {
        .DeviceId           = 13,
        .BaseAddress        = AL_XMON_HP_S1_BASE_ADDR,
    },

    {
        .DeviceId           = 14,
        .BaseAddress        = AL_XMON_APU_BASE_ADDR,
    },

    {
        .DeviceId           = 15,
        .BaseAddress        = AL_XMON_JPU_BASE_ADDR,
    },

    {
        .DeviceId           = 16,
        .BaseAddress        = AL_XMON_MAIN_S6_BASE_ADDR,
    },

    {
        .DeviceId           = 17,
        .BaseAddress        = AL_XMON_MAIN_M6_BASE_ADDR,
    },

    {
        .DeviceId           = 18,
        .BaseAddress        = AL_XMON_MAIN_S1_BASE_ADDR,
    },

    {
        .DeviceId           = 19,
        .BaseAddress        = AL_XMON_MAIN_M0_BASE_ADDR,
    },

    {
        .DeviceId           = 20,
        .BaseAddress        = AL_XMON_ACP_BASE_ADDR,
    },
};
