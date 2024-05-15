/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_hal.h"
#include "al_mmc_hw.h"

AL_MMC_HwConfigStruct AlMmc_HwConfig[AL_MMC_NUM_INSTANCE] = {
#ifdef MMC0PS_ENABLE
    {
        .DeviceId       = 0,
        .BaseAddress    = MMC0__BASE_ADDR,
        .IntrId         = SOC_MMC0_IRQn,
        .BusWidth       = MMC0_BUS_WIDTH,
        .InputClk       = MMC_CLOCK,
        .IoClk          = MMC0_IO_FREQ,
        .CardDetectSig  = MMC_MMC0_CARD_DETECT_SIG,
        .WritePortSig   = MMC_MMC0_WR_PROT_SIG,
        .CardDetect     = MMC_MMC0_CARD_DETECT_STATE,
        .WritePort      = MMC_MMC0_WR_PROT_STATE,
        .ClkPhase       = MMC_MMC0_CLK_PHASE
    },
#endif
#ifdef MMC1PS_ENABLE
    {
        .DeviceId       = 1,
        .BaseAddress    = MMC1__BASE_ADDR,
        .IntrId         = SOC_MMC1_IRQn,
        .BusWidth       = MMC1_BUS_WIDTH,
        .InputClk       = MMC_CLOCK,
        .IoClk          = MMC1_IO_FREQ,
        .CardDetectSig  = MMC_MMC1_CARD_DETECT_SIG,
        .WritePortSig   = MMC_MMC1_WR_PROT_SIG,
        .CardDetect     = MMC_MMC1_CARD_DETECT_STATE,
        .WritePort      = MMC_MMC1_WR_PROT_STATE,
        .ClkPhase       = MMC_MMC1_CLK_PHASE
    }
#endif
};