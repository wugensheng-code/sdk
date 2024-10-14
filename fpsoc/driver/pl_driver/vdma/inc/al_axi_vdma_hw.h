/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_VDMA_HW_H_
#define AL_AXI_VDMA_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

// test, parameters should from TD HPF
#define AL_AXI_VDMA_NUM_INSTANCE    2
#define AXI_VDMA0_IRQn              114
#define AXI_VDMA0__BASE_ADDR        0x80002000U



/***************************** Include Files ********************************/
#include "al_core.h"

/***************************** Register Offset ********************************/
#define AL_AXI_VDMA_MM2S_VDMACR_OFFSET 0x00
#define AL_AXI_VDMA_MM2S_VDMASR_OFFSET 0x04
#define AL_AXI_VDMA_MM2S_REG_INDEX_OFFSET 0x14
#define AL_AXI_VDMA_PARK_PTR_REG_OFFSET 0x28
#define AL_AXI_VDMA_VDMA_VERSION_OFFSET 0x2C
#define AL_AXI_VDMA_S2MM_VDMACR_OFFSET 0x30
#define AL_AXI_VDMA_S2MM_VDMASR_OFFSET 0x34
#define AL_AXI_VDMA_S2MM_VDMA_IRQ_MASK_OFFSET 0x3C
#define AL_AXI_VDMA_S2MM_REG_INDEX_OFFSET 0x44
#define AL_AXI_VDMA_MM2S_VSIZE_OFFSET 0x50
#define AL_AXI_VDMA_MM2S_HSIZE_OFFSET 0x54
#define AL_AXI_VDMA_MM2S_FRMDLY_STRIDE_OFFSET 0x58
#define AL_AXI_VDMA_MM2S_START_ADDRESS_OFFSET 0x5C
#define AL_AXI_VDMA_S2MM_VSIZE_OFFSET 0xA0
#define AL_AXI_VDMA_S2MM_HSIZE_OFFSET 0xA4
#define AL_AXI_VDMA_S2MM_FRMDLY_STRIDE_OFFSET 0xA8
#define AL_AXI_VDMA_S2MM_START_ADDRESS_OFFSET 0xAC
#define AL_AXI_VDMA_ENABLE_VERTICAL_FLIP_OFFSET 0xEC


#define AL_AXI_VDMA_ALL_INTR_MASK         0x00007000 /**< All interrupts */


/***************************** MM2S_VDMACR Register Shift ********************************/
#define AL_AXI_VDMA_MM2S_VDMACR_RS_SHIFT 0
#define AL_AXI_VDMA_MM2S_VDMACR_Circular_Park_SHIFT 1
#define AL_AXI_VDMA_MM2S_VDMACR_Reset_SHIFT 2
#define AL_AXI_VDMA_MM2S_VDMACR_GenlockEn_SHIFT 3
#define AL_AXI_VDMA_MM2S_VDMACR_FrameCntEn_SHIFT 4
#define AL_AXI_VDMA_MM2S_VDMACR_GenlockSrc_SHIFT 7
#define AL_AXI_VDMA_MM2S_VDMACR_RdPntrNum_SHIFT 8
#define AL_AXI_VDMA_MM2S_VDMACR_FrmCnt_IrqEn_SHIFT 12
#define AL_AXI_VDMA_MM2S_VDMACR_DlyCnt_IrqEn_SHIFT 13
#define AL_AXI_VDMA_MM2S_VDMACR_Err_IrqEn_SHIFT 14
#define AL_AXI_VDMA_MM2S_VDMACR_Repeat_En_SHIFT 15
#define AL_AXI_VDMA_MM2S_VDMACR_IRQFrameCount_SHIFT 16
#define AL_AXI_VDMA_MM2S_VDMACR_IRQDelayCount_SHIFT 24

/***************************** MM2S_VDMASR Register Shift ********************************/
#define AL_AXI_VDMA_MM2S_VDMASR_Halted_SHIFT 0
#define AL_AXI_VDMA_MM2S_VDMASR_VDMAIntErr_SHIFT 4
#define AL_AXI_VDMA_MM2S_VDMASR_VDMASlvErr_SHIFT 5
#define AL_AXI_VDMA_MM2S_VDMASR_VDMADecErr_SHIFT 6
#define AL_AXI_VDMA_MM2S_VDMASR_SOFEarlyErr_SHIFT 7
#define AL_AXI_VDMA_MM2S_VDMASR_FrmCnt_Irq_SHIFT 12
#define AL_AXI_VDMA_MM2S_VDMASR_DlyCnt_Irq_SHIFT 13
#define AL_AXI_VDMA_MM2S_VDMASR_Err_Irq_SHIFT 14
#define AL_AXI_VDMA_MM2S_VDMASR_IRQFrameCntSts_SHIFT 16
#define AL_AXI_VDMA_MM2S_VDMASR_IRQDelayCntSts_SHIFT 24

/***************************** MM2S_REG_INDEX Register Shift ********************************/
#define AL_AXI_VDMA_MM2S_REG_INDEX_MM2S_Reg_Index_SHIFT 0

/***************************** PARK_PTR_REG Register Shift ********************************/
#define AL_AXI_VDMA_PARK_PTR_REG_RdFrmPtrRef_SHIFT 0
#define AL_AXI_VDMA_PARK_PTR_REG_WrFrmPtrRef_SHIFT 8
#define AL_AXI_VDMA_PARK_PTR_REG_RdFrmStore_SHIFT 16
#define AL_AXI_VDMA_PARK_PTR_REG_WrFrmStore_SHIFT 24

/***************************** VDMA_VERSION Register Shift ********************************/

/***************************** S2MM_VDMACR Register Shift ********************************/
#define AL_AXI_VDMA_S2MM_VDMACR_RS_SHIFT 0
#define AL_AXI_VDMA_S2MM_VDMACR_Circular_Park_SHIFT 1
#define AL_AXI_VDMA_S2MM_VDMACR_Reset_SHIFT 2
#define AL_AXI_VDMA_S2MM_VDMACR_GenlockEn_SHIFT 3
#define AL_AXI_VDMA_S2MM_VDMACR_FrameCntEn_SHIFT 4
#define AL_AXI_VDMA_S2MM_VDMACR_GenlockSrc_SHIFT 7
#define AL_AXI_VDMA_S2MM_VDMACR_WrPntrNum_SHIFT 8
#define AL_AXI_VDMA_S2MM_VDMACR_FrmCnt_IrqEn_SHIFT 12
#define AL_AXI_VDMA_S2MM_VDMACR_DlyCnt_IrqEn_SHIFT 13
#define AL_AXI_VDMA_S2MM_VDMACR_Err_IrqEn_SHIFT 14
#define AL_AXI_VDMA_S2MM_VDMACR_Repeat_En_SHIFT 15
#define AL_AXI_VDMA_S2MM_VDMACR_IRQFrameCount_SHIFT 16
#define AL_AXI_VDMA_S2MM_VDMACR_IRQDelayCount_SHIFT 24

/***************************** S2MM_VDMASR Register Shift ********************************/
#define AL_AXI_VDMA_S2MM_VDMASR_Halted_SHIFT 0
#define AL_AXI_VDMA_S2MM_VDMASR_VDMAIntErr_SHIFT 4
#define AL_AXI_VDMA_S2MM_VDMASR_VDMASlvErr_SHIFT 5
#define AL_AXI_VDMA_S2MM_VDMASR_VDMADecErr_SHIFT 6
#define AL_AXI_VDMA_S2MM_VDMASR_SOFEarlyErr_SHIFT 7
#define AL_AXI_VDMA_S2MM_VDMASR_EOLEarlyErr_SHIFT 8
#define AL_AXI_VDMA_S2MM_VDMASR_SOFLateErr_SHIFT 11
#define AL_AXI_VDMA_S2MM_VDMASR_FrmCnt_Irq_SHIFT 12
#define AL_AXI_VDMA_S2MM_VDMASR_DlyCnt_Irq_SHIFT 13
#define AL_AXI_VDMA_S2MM_VDMASR_Err_Irq_SHIFT 14
#define AL_AXI_VDMA_S2MM_VDMASR_EOLLateErr_SHIFT 15
#define AL_AXI_VDMA_S2MM_VDMASR_IRQFrameCntSts_SHIFT 16
#define AL_AXI_VDMA_S2MM_VDMASR_IRQDelayCntSts_SHIFT 24

/***************************** S2MM_VDMA_IRQ_MASK Register Shift ********************************/
#define AL_AXI_VDMA_S2MM_VDMA_IRQ_MASK_IRQMaskSOFEarlyErr_SHIFT 0
#define AL_AXI_VDMA_S2MM_VDMA_IRQ_MASK_IRQMaskEOLEarlyErr_SHIFT 1
#define AL_AXI_VDMA_S2MM_VDMA_IRQ_MASK_IRQMaskSOFLateErr_SHIFT 2
#define AL_AXI_VDMA_S2MM_VDMA_IRQ_MASK_IRQMaskEOLLateErr_SHIFT 3

/***************************** S2MM_REG_INDEX Register Shift ********************************/
#define AL_AXI_VDMA_S2MM_REG_INDEX_S2MM_Reg_Index_SHIFT 0

/***************************** MM2S_VSIZE Register Shift ********************************/
#define AL_AXI_VDMA_MM2S_VSIZE_Vertical_Size_SHIFT 0

/***************************** MM2S_HSIZE Register Shift ********************************/
#define AL_AXI_VDMA_MM2S_HSIZE_Horizontal_Size_SHIFT 0

/***************************** MM2S_FRMDLY_STRIDE Register Shift ********************************/
#define AL_AXI_VDMA_MM2S_FRMDLY_STRIDE_Stride_SHIFT 0
#define AL_AXI_VDMA_MM2S_FRMDLY_STRIDE_Frame_Delay_SHIFT 24

/***************************** MM2S_START_ADDRESS Register Shift ********************************/

/***************************** S2MM_VSIZE Register Shift ********************************/
#define AL_AXI_VDMA_S2MM_VSIZE_Vertical_Size_SHIFT 0

/***************************** S2MM_HSIZE Register Shift ********************************/
#define AL_AXI_VDMA_S2MM_HSIZE_Horizontal_Size_SHIFT 0

/***************************** S2MM_FRMDLY_STRIDE Register Shift ********************************/
#define AL_AXI_VDMA_S2MM_FRMDLY_STRIDE_Stride_SHIFT 0
#define AL_AXI_VDMA_S2MM_FRMDLY_STRIDE_Frame_Delay_SHIFT 24

/***************************** S2MM_START_ADDRESS Register Shift ********************************/


/***************************** ENABLE_VERTICAL_FLIP Register Shift ********************************/
#define AL_AXI_VDMA_ENABLE_VERTICAL_FLIP_Enable_Vertical_Flip_SHIFT 0



typedef struct
{
    AL_U8 DeviceId;
    AL_UINTPTR BaseAddress;

    AL_U32 MaxFrameStoreNum;

    AL_U32 HasS2Mm;
    AL_U32 S2Mm_IntrId;
    AL_U32 S2MmMmDataWidth;
    AL_U32 S2MmBurstSize;
    AL_U32 S2MmStreamDataWidth;
    AL_U32 S2MmLineBufferDepth;
    AL_U32 S2MmFsyncOptions;
    AL_U32 S2MmGenlockMode;
    AL_U32 S2MmHasDRE;

    AL_U32 HasMm2s;
    AL_U32 Mm2s_IntrId;
    AL_U32 Mm2sMmDataWidth;
    AL_U32 Mm2sBurstSize;
    AL_U32 Mm2sStreamDataWidth;
    AL_U32 Mm2sLineBufferDepth;
    AL_U32 Mm2sFsyncOptions;
    AL_U32 Mm2sGenlockMode;
    AL_U32 Mm2sHasDRE;

    AL_U32 EnableAsynsMode;
    AL_U32 HasVFlip;
} AlAxiVdma_HwConfigStruct;

#ifdef __cplusplus
}
#endif

#endif
