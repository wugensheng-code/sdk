/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***************************** Include Files *********************************/
#include "al_vdma_dev.h"

/************************** Variable Prototypes ******************************/
AlAxiVdma_HwConfigStruct AlAxiVdma_HwCfg[AL_VDMA_NUM_INSTANCE] =
{
    {
        .DeviceId               = AXI_VDMA0_DEV_ID,
        .BaseAddress            = AXI_VDMA0_BASEADDR,
        .AddrWidth              = AXI_VDMA0_ADDRWIDTH,

        .MaxFrameStoreNum       = AXI_VDMA0_MAXFRAMESTOENUM,

        .HasS2Mm                = AXI_VDMA0_HASS2MM,
        .S2Mm_IntrId            = AXI_VDMA0_S2MM_INTRID,
        .S2MmMmDataWidth        = AXI_VDMA0_S2MMMMDATAWIDTH,
        .S2MmBurstSize          = AXI_VDMA0_S2MMBURSTSIZE,
        .S2MmStreamDataWidth    = AXI_VDMA0_S2MMSTREAMDATAWIDITH,
        .S2MmLineBufferDepth    = AXI_VDMA0_S2MMLINEBUFFERDEPTH,
        .S2MmFsyncOptions       = AXI_VDMA0_S2MMFSYNCOPTIONS,
        .S2MmGenlockMode        = AXI_VDMA0_S2MMGENLOCKMODE,
        .S2MmHasDRE             = AXI_VDMA0_S2MMHASDRE,

        .HasMm2s                = AXI_VDMA0_HASMM2S,
        .Mm2s_IntrId            = AXI_VDMA0_MM2S_INTRID,
        .Mm2sMmDataWidth        = AXI_VDMA0_MM2SMMDATAWIDTH,
        .Mm2sBurstSize          = AXI_VDMA0_MM2SBURSTSIZE,
        .Mm2sStreamDataWidth    = AXI_VDMA0_MM2SSTREAMDATAWIDTH,
        .Mm2sLineBufferDepth    = AXI_VDMA0_MM2SLINEBUFFERDEPTH,
        .Mm2sFsyncOptions       = AXI_VDMA0_MM2SFSYNCOPTIONS,
        .Mm2sGenlockMode        = AXI_VDMA0_MM2SGENLOCKMODE,
        .Mm2sHasDRE             = AXI_VDMA0_MM2SHASDRE,

        .EnableAsynsMode        = AXI_VDMA0_ENABLEASYNSMODE,
        .HasVFlip               = AXI_VDMA0_HASVFLIP,
    }
};