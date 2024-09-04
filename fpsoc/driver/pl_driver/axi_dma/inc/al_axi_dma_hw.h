#ifndef __AL_AXI_DMA_HW_H_
#define __AL_AXI_DMA_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_core.h"

typedef struct {
    // Device general configuration
    AL_U32      DeviceId;
    AL_U64      BaseAddress;
    AL_U32      AddrWidth;
    AL_U32      BufferlenWidth;
    AL_U32      EnableMicroMode;
    AL_U32      DmaMode;

    // MM2S (Memory-Mapped to Stream)
    AL_U32      IntrNum_Mm2s;
    AL_U32      HasMm2s;
    AL_U32      HasMm2sDRE;
    AL_U32      Mm2sMemoryMapDataWidth;
    AL_U32      Mm2sStreamDataWidth;
    AL_U32      Mm2sBurstSize;
    AL_U32      Mm2sMicroMaxTransferLen;

    // S2MM (Stream to Memory-Mapped)
    AL_U32      IntrNum_S2mm;
    AL_U32      HasS2mm;
    AL_U32      HasS2mmDRE;
    AL_U32      S2mmMemoryMapDataWidth;
    AL_U32      S2mmStreamDataWidth;
    AL_U32      S2mmBurstSize;
    AL_U32      S2mmMicroMaxTransferLen;
} AlAxiDma_HwConfigStruct;


/* Definitions for PL AXI DMA */
#define AL_AXI_DMA_NUM_INSTANCE                       1
#define AL_AXI_DMA0_DEVICE_ID                         0
#define AL_AXI_DMA0_BASE_ADDR                0x80000000
#define AL_AXI_DMA0_S2MM_IRQn                       114
#define AL_AXI_DMA0_MM2S_IRQn                       115
#define AL_AXI_DMA0_INCLUDE_MM2S                      1
#define AL_AXI_DMA0_INCLUDE_S2MM                      1
#define AL_AXI_DMA0_ADDR_WIDTH                       32
#define AL_AXI_DMA0_ENABLE_MICRO                      0
#define AL_AXI_DMA0_MODE                              0  // DIRECT_MODE = 0; SG_MODE = 1; 
#define AL_AXI_DMA0_BUFFER_LENGTH_WIDTH              26
#define AL_AXI_DMA0_DESC_BUFFER_LENGTH_WIDTH         26

#define AL_AXI_DMA0_HAS_MM2S_DRE                      0
#define AL_AXI_DMA0_HAS_S2MM_DRE                      0

#define AL_AXI_DMA0_MM2S_MEMORY_MAP_DATA_WIDTH       64
#define AL_AXI_DMA0_S2MM_MEMORY_MAP_DATA_WIDTH       64

#define AL_AXI_DMA0_MM2S_STREAM_DATA_WIDTH           32
#define AL_AXI_DMA0_S2MM_STREAM_DATA_WIDTH           32

#define AL_AXI_DMA0_MM2S_BURST_SIZE                  32
#define AL_AXI_DMA0_S2MM_BURST_SIZE                  32

#define AXIDMA_MM2S_ALIGN_SIZE           (AL_AXI_DMA0_MM2S_MEMORY_MAP_DATA_WIDTH / 8)
#define AXIDMA_S2MM_ALIGN_SIZE           (AL_AXI_DMA0_S2MM_MEMORY_MAP_DATA_WIDTH / 8)
#define AXIDMA_DESC_ALIGN_SIZE           (0X40)

/* Attribute for aligning the buffer address for MM2S transfers */
#define AXIDMA_MM2S_BUFFER_ALIGN __attribute__((aligned(AXIDMA_MM2S_ALIGN_SIZE)))

/* Attribute for aligning the buffer address for S2MM transfers */
#define AXIDMA_S2MM_BUFFER_ALIGN __attribute__((aligned(AXIDMA_S2MM_ALIGN_SIZE)))

/* Attribute for placing descriptors in non-cacheable RAM section with alignment */
#define AXIDMA_DESC_NOCACHE_ALIGN_RAM_SECTION __attribute__((aligned(AXIDMA_DESC_ALIGN_SIZE))) __attribute__((section(".noncacheable")))

/***************************** Register Offsets（MM2S / S2MM）*****************************/
#define ALAXIDMA_MM2S_OFFSET            0x00000000
#define ALAXIDMA_S2MM_OFFSET            0x00000030
#define ALAXIDMA_CR_OFFSET                    0x00
#define ALAXIDMA_SR_OFFSET                    0x04
#define ALAXIDMA_CURDESR_OFFSET               0x08
#define ALAXIDMA_CURDESR_MSB_OFFSET           0x0C
#define ALAXIDMA_TAILDESR_OFFSET              0x10
#define ALAXIDMA_TAILDESR_MSB_OFFSET          0x14
#define ALAXIDMA_ADDR_OFFSET                  0x18
#define ALAXIDMA_ADDR_MSB_OFFSET              0x1C
#define ALAXIDMA_LENTH_OFFSET                 0x28

/***************************** AXI_DMA_CR_OFFSET Register *****************************/
#define ALAXIDMA_CR_RUNSTOP_SHIFT        0
#define ALAXIDMA_CR_RESET_SHIFT          2
#define ALAXIDMA_CR_KEYHOLE_SHIFT        3
#define ALAXIDMA_CR_CYCLIC_SHIFT         4
#define ALAXIDMA_CR_IOC_SHIFT           12
#define ALAXIDMA_CR_ERR_SHIFT           14

/***************************** AXI_DMA_SR_OFFSET Register *****************************/
#define ALAXIDMA_SR_HALT_SHIFT           0
#define ALAXIDMA_SR_IDLE_SHIFT           1
#define ALAXIDMA_SR_IOC_SHIFT           12
#define ALAXIDMA_SR_ERR_SHIFT           14

#define ALAXIDMA_SR_INTR_SHIFT          12
#define ALAXIDMA_SR_INTR_SIZE            3

/***************************** AXI_DMA_LENTH_OFFSET Register *****************************/
#define ALAXIDMA_LENTH_START_SHIFT       0
#define ALAXIDMA_LENGTH_SIZE            26

/***************************** AXI_DMA_Threshold_OFFSET Register *****************************/
#define ALAXIDMA_THRE_START_SHIFT       16
#define ALAXIDMA_THRE_SIZE             8

/***************************** Scatter Gather Descriptor *********************************/
#define ALAXIDMA_DESC_CONTROL_SOF_SHIFT      27
#define ALAXIDMA_DESC_CONTROL_EOF_SHIFT      26

#ifdef __cplusplus
}
#endif

#endif
