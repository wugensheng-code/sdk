#include "al_axi_dma_hw.h"

AlAxiDma_HwConfigStruct AlAxiDma_HwConfig[AL_AXI_DMA_NUM_INSTANCE] = 
{
    {
        // Device general configuration
        .DeviceId               = AL_AXI_DMA0_DEVICE_ID,
        .BaseAddress            = AL_AXI_DMA0_BASE_ADDR,
        .AddrWidth              = AL_AXI_DMA0_ADDR_WIDTH,
        .NormalMaxTransferLen   = AL_AXI_DMA0_NORMAL_MAX_LENGTH,

        // MM2S direction configuration
        .IntrNum_Mm2s            = AL_AXI_DMA0_MM2S_IRQn,
        .HasMm2s                = AL_AXI_DMA0_INCLUDE_MM2S,
        .HasMm2sDRE             = AL_AXI_DMA0_HAS_MM2S_DRE,
        .Mm2sMemoryMapDataWidth = AL_AXI_DMA0_MM2S_MEMORY_MAP_DATA_WIDTH,
        .Mm2sStreamDataWidth    = AL_AXI_DMA0_MM2S_STREAM_DATA_WIDTH,
        .Mm2sBurstSize          = AL_AXI_DMA0_MM2S_BURST_SIZE,

        // S2MM direction configuration
        .IntrNum_S2mm            = AL_AXI_DMA0_S2MM_IRQn,
        .HasS2mm                = AL_AXI_DMA0_INCLUDE_S2MM,
        .HasS2mmDRE             = AL_AXI_DMA0_HAS_S2MM_DRE,
        .S2mmMemoryMapDataWidth = AL_AXI_DMA0_S2MM_MEMORY_MAP_DATA_WIDTH,
        .S2mmStreamDataWidth    = AL_AXI_DMA0_S2MM_STREAM_DATA_WIDTH,
        .S2mmBurstSize          = AL_AXI_DMA0_S2MM_BURST_SIZE,
    },
};
