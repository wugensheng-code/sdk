/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_smc_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_SMC_HwConfigStruct AlSmc_HwCfg[AL_SMC_NUM_INSTANCE] =
{
    {
        .DeviceId           = 0,
        .NandBaseAddr       = NAND_BASE_ADDR,
        .SmcBaseAddr        = SMC_BASE_ADDR
    }
};