/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_qspi_hw.h"

/************************** Variable Definitions *****************************/
AlAxiQspi_HwConfigStruct AlAxiQspi_HwCfg[AL_AXI_QSPI_NUM_INSTANCE] =
{
    {
        .DeviceId       = 0,
        .IntrId         = 114,
        .BaseAddress    = 0x80000000ULL,
        .InputClk       = 25000000,

        .IsAsyncClk     = 0,
        .AxiType        = 0,
        .AxiIdWidth     = 1,
        .XipPerfMode    = 0,
        .IsXipMode      = 0,
        .SlaveNums      = 1,
        .SCK_RATIO      = 2,
        .SpiMode        = 0,
        .FlashType      = 3,
        .TransBits      = 8,
        .FifoDepth      = 256,
        .FlashAddrLen   = 24,
    },
};

