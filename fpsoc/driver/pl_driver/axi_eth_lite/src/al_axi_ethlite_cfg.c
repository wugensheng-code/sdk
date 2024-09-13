/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/********************************* including Files *********************************/
#include "al_hal.h"
#include "al_axi_ethlite_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_AXI_ETHLITE_HwConfigStruct AlAxiEthLite_HwConfig[AL_AXI_ETHLITE_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .BaseAddress    = AXI_ETHLITE_ETH0_BASE_ADDR,
        .ClockHz        = AXI_ETHLITE_ETH0_CLOCK,
        .IntrId         = AXI_ETHLITE_ETH0_INTR_ID,
    }
};