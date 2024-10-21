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

        .MaxFrameStoreNum       = AXI_VDMA0_MaxFrameStoreNum,

        .HasS2Mm                = AXI_VDMA0_HasS2Mm,
        .S2Mm_IntrId            = AXI_VDMA0_S2Mm_IntrId,
        .S2MmMmDataWidth        = AXI_VDMA0_S2MmMmDataWidth,
        .S2MmBurstSize          = AXI_VDMA0_S2MmBurstSize,
        .S2MmStreamDataWidth    = AXI_VDMA0_S2MmStreamDataWidth,
        .S2MmLineBufferDepth    = AXI_VDMA0_S2MmLineBufferDepth,
        .S2MmFsyncOptions       = AXI_VDMA0_S2MmFsyncOptions,
        .S2MmGenlockMode        = AXI_VDMA0_S2MmGenlockMode,
        .S2MmHasDRE             = AXI_VDMA0_S2MmHasDRE,

        .HasMm2s                = AXI_VDMA0_HasMm2s,
        .Mm2s_IntrId            = AXI_VDMA0_Mm2s_IntrId,
        .Mm2sMmDataWidth        = AXI_VDMA0_Mm2sMmDataWidth,
        .Mm2sBurstSize          = AXI_VDMA0_Mm2sBurstSize,
        .Mm2sStreamDataWidth    = AXI_VDMA0_Mm2sStreamDataWidth,
        .Mm2sLineBufferDepth    = AXI_VDMA0_Mm2sLineBufferDepth,
        .Mm2sFsyncOptions       = AXI_VDMA0_Mm2sFsyncOptions,
        .Mm2sGenlockMode        = AXI_VDMA0_Mm2sGenlockMode,
        .Mm2sHasDRE             = AXI_VDMA0_Mm2sHasDRE,

        .EnableAsynsMode        = AXI_VDMA0_EnableAsynsMode,
        .HasVFlip               = AXI_VDMA0_HasVFlip,
    }
};