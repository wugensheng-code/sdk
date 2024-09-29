/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/********************************* including Files *********************************/
#include "al_axi_dma_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AlAxiDma_HwConfigStruct AlAxiDma_HwConfig[AL_AXI_DMA_NUM_INSTANCE] =
{
    {
        // Device general configuration
        .DeviceId                  = 0,
        .BaseAddress               = AXI_DMA0_BASE_ADDR,
        .AddrWidth                 = AXI_DMA0_ADDR_WIDTH,
        .EnableMicroMode           = AXI_DMA0_ENABLE_MICRO,
        .DmaMode                   = AXI_DMA0_MODE,
        .BufferlenWidth            = AXI_DMA0_BUFFER_LENGTH_WIDTH,
        .EnableStatusControlStream = AXI_DMA0_ENABLE_CONTROL_STATUS_STREAM,

        // MM2S direction configuration
        .IntrNum_Mm2s              = AXI_DMA0_MM2S_IRQn,
        .HasMm2s                   = AXI_DMA0_INCLUDE_MM2S,
        .HasMm2sDRE                = AXI_DMA0_HAS_MM2S_DRE,
        .Mm2sMemoryMapDataWidth    = AXI_DMA0_MM2S_MEMORY_MAP_DATA_WIDTH,
        .Mm2sStreamDataWidth       = AXI_DMA0_MM2S_STREAM_DATA_WIDTH,
        .Mm2sBurstSize             = AXI_DMA0_MM2S_BURST_SIZE,

        // S2MM direction configuration
        .IntrNum_S2mm              = AXI_DMA0_S2MM_IRQn,
        .HasS2mm                   = AXI_DMA0_INCLUDE_S2MM,
        .HasS2mmDRE                = AXI_DMA0_HAS_S2MM_DRE,
        .S2mmMemoryMapDataWidth    = AXI_DMA0_S2MM_MEMORY_MAP_DATA_WIDTH,
        .S2mmStreamDataWidth       = AXI_DMA0_S2MM_STREAM_DATA_WIDTH,
        .S2mmBurstSize             = AXI_DMA0_S2MM_BURST_SIZE,
        .EnableRxLength            = AXI_DMA0_ENABLE_S2MM_RXLENGTH,
    },
};
