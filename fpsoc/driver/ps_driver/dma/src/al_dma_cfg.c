/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_hwcfg.h"
#include "al_dma_hw.h"
#include "al_chip.h"


/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_DMA_HwConfigStruct AlDma_HwConfig[AL_DMA_NUM_INSTANCE] =
{
    {
        .DeviceId      = 0,
        .BaseAddress   = AL_DMA_S_BASE_ADDR,
        .ChanCount     = AL_DMA_CHAN_COUNT,
        .IntrNum       = {
                            SOC_DMAC_CH0_IRQ, SOC_DMAC_CH1_IRQ, SOC_DMAC_CH2_IRQ, SOC_DMAC_CH3_IRQ,
                            SOC_DMAC_CH4_IRQ, SOC_DMAC_CH5_IRQ, SOC_DMAC_CH6_IRQ, SOC_DMAC_CH7_IRQ
                         },
        .AbortIntr      = SOC_DMAC_ABORT_IRQn,
        .LockAddr       = AL_DMA_LOCK_ADDR,
    }
};
