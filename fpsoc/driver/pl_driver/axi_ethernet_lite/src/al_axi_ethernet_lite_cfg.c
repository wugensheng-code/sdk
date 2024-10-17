/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/********************************* including Files *********************************/
#include "al_hal.h"
#include "al_axi_ethernet_lite_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AlAxiEthLite_HwConfigStruct AlAxiEthLite_HwConfig[AL_AXI_ETH_LITE_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .BaseAddress    = AXI_ETH_LITE0_BASE_ADDR,
        .InputClk       = AXI_ETH_LITE_CLOCK,
        .IntrId         = AXI_ETH_LITE0_INTR_ID,
    }
};