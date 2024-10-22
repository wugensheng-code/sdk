/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_VDMA_ll_H_
#define AL_VDMA_ll_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "al_vdma_hw.h"

// test, parameter from TD hpf file
#define AL_VDMA_NUM_INSTANCE 2
#define AXI_VDMA0_DEV_ID 0
#define AXI_VDMA0_BASEADDR 0x80002000
#define AXI_VDMA0_ADDRWIDTH 32

#define AXI_VDMA0_MAXFRAMESTOENUM 5

#define AXI_VDMA0_HASS2MM 1
#define AXI_VDMA0_S2MM_INTRID 114
#define AXI_VDMA0_S2MMMMDATAWIDTH 128
#define AXI_VDMA0_S2MMBURSTSIZE 8
#define AXI_VDMA0_S2MMSTREAMDATAWIDITH 32
#define AXI_VDMA0_S2MMLINEBUFFERDEPTH 512
#define AXI_VDMA0_S2MMFSYNCOPTIONS 0
#define AXI_VDMA0_S2MMGENLOCKMODE 3
#define AXI_VDMA0_S2MMHASDRE 0

#define AXI_VDMA0_HASMM2S 1
#define AXI_VDMA0_MM2S_INTRID 115
#define AXI_VDMA0_MM2SMMDATAWIDTH 128
#define AXI_VDMA0_MM2SBURSTSIZE 8
#define AXI_VDMA0_MM2SSTREAMDATAWIDTH 32
#define AXI_VDMA0_MM2SLINEBUFFERDEPTH 512
#define AXI_VDMA0_MM2SFSYNCOPTIONS 0
#define AXI_VDMA0_MM2SGENLOCKMODE 2
#define AXI_VDMA0_MM2SHASDRE 0

#define AXI_VDMA0_ENABLEASYNSMODE 0
#define AXI_VDMA0_HASVFLIP 1

/***************************** AL_AXI_VDMA_DATA ********************************/


static inline AL_VOID AlAxiVdma_ll_SetMm2sIrqDelayCount(AL_REG BaseAddr, AL_U8 DelayCount)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 24, 8, DelayCount);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sIrqFrameCount(AL_REG BaseAddr, AL_U8 DelayCount)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 16, 8, DelayCount);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sRepeatEn(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 15, State);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sErrIrqEn(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 14, State);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sDlyCntIrqEn(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 13, State);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sFrmCntIrqEn(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 12, State);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sRdPntrNum(AL_REG BaseAddr, AL_U32 PntrNum)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 8, 4, PntrNum);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sGenlockSrc(AL_REG BaseAddr, AL_U32 GenlockSrc)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 7, GenlockSrc);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sFrameCntEn(AL_REG BaseAddr, AL_BOOL FrameCntEn)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 4, FrameCntEn);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sGenlockEn(AL_REG BaseAddr, AL_BOOL GenlockEn)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 3, GenlockEn);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sReset(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 2, State);
}

static inline AL_BOOL AlAxiVdma_ll_GetMm2sResetStatus(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 2);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sCircularPark(AL_REG BaseAddr, AL_U32 CircularPark)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 1, CircularPark);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sStart(AL_REG BaseAddr, AL_U32 State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 0, State);
}

static inline AL_BOOL AlAxiVdma_ll_IsRunMm2s(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET, 0);
}

static inline AL_U32 AlAxiVdma_ll_GetMm2sControlReg(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AL_AXI_VDMA_MM2S_VDMACR_OFFSET);
}

static inline AL_U32 AlAxiVdma_ll_GetMm2sStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AL_AXI_VDMA_MM2S_VDMASR_OFFSET);
}

static inline AL_BOOL AlAxiVdma_ll_IsHaltMm2s(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_VDMASR_OFFSET, 0);
}

static inline AL_VOID AlAxiVdma_ll_ClearMm2sIntr(AL_REG BaseAddr, AL_U32 Data)
{
    AL_REG32_WRITE(BaseAddr + AL_AXI_VDMA_MM2S_VDMASR_OFFSET, Data);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sRegIndex(AL_REG BaseAddr, AL_U32 RegIndex)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_MM2S_REG_INDEX_OFFSET, 0, RegIndex);
}

static inline AL_VOID AlAxiVdma_ll_SetRdFrmPtrRef(AL_REG BaseAddr, AL_U32 RdFrmPtrRef)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_PARK_PTR_REG_OFFSET, 0, 5, RdFrmPtrRef);
}

static inline AL_VOID AlAxiVdma_ll_SetWrFrmPtrRef(AL_REG BaseAddr, AL_U32 WrFrmPtrRef)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_PARK_PTR_REG_OFFSET, 8, 5, WrFrmPtrRef);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmIrqDelayCount(AL_REG BaseAddr, AL_U8 DelayCount)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 24, 8, DelayCount);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmIrqFrameCount(AL_REG BaseAddr, AL_U8 DelayCount)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 16, 8, DelayCount);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmRepeatEn(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 15, State);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmErrIrqEn(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 14, State);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmDlyCntIrqEn(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 13, State);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmFrmCntIrqEn(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 12, State);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmRdPntrNum(AL_REG BaseAddr, AL_U32 PntrNum)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 8, 4, PntrNum);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmGenlockSrc(AL_REG BaseAddr, AL_U32 GenlockSrc)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 7, GenlockSrc);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmFrameCntEn(AL_REG BaseAddr, AL_BOOL FrameCntEn)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 4, FrameCntEn);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmGenlockEn(AL_REG BaseAddr, AL_BOOL GenlockEn)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 3, GenlockEn);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmReset(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 2, State);
}

static inline AL_BOOL AlAxiVdma_ll_GetS2MmResetStatus(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 2);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmCircularPark(AL_REG BaseAddr, AL_U32 CircularPark)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 1, CircularPark);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmStart(AL_REG BaseAddr, AL_U32 State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 0, State);
}

static inline AL_BOOL AlAxiVdma_ll_IsRunS2Mm(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, 0);
}

static inline AL_U32 AlAxiVdma_ll_GetS2MmControlReg(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET);
}

static inline AL_U32 AlAxiVdma_ll_GetS2MmStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AL_AXI_VDMA_S2MM_VDMASR_OFFSET);
}

static inline AL_BOOL AlAxiVdma_ll_IsHaltS2Mm(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_VDMASR_OFFSET, 0);
}

static inline AL_VOID AlAxiVdma_ll_ClearS2MmIntr(AL_REG BaseAddr, AL_U32 Data)
{
    AL_REG32_WRITE(BaseAddr + AL_AXI_VDMA_S2MM_VDMASR_OFFSET, Data);
}

static inline AL_VOID AlAxiVdma_ll_MaskIrqS2Mm(AL_REG BaseAddr, AL_U32 State)
{
    AL_REG32_WRITE(BaseAddr + AL_AXI_VDMA_S2MM_VDMACR_OFFSET, State);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmRegIndex(AL_REG BaseAddr, AL_U32 RegIndex)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_S2MM_REG_INDEX_OFFSET, 0, RegIndex);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sVsize(AL_REG BaseAddr, AL_U32 Vsize)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_MM2S_VSIZE_OFFSET, 0, 13, Vsize);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sHsize(AL_REG BaseAddr, AL_U32 Hsize)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_MM2S_HSIZE_OFFSET, 0, 16, Hsize);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sFrameDelay(AL_REG BaseAddr, AL_U32 FrameDelay)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_MM2S_FRMDLY_STRIDE_OFFSET, 24, 5, FrameDelay);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sStride(AL_REG BaseAddr, AL_U32 Stride)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_MM2S_FRMDLY_STRIDE_OFFSET, 0, 16, Stride);
}

static inline AL_VOID AlAxiVdma_ll_SetMm2sStartAddr(AL_REG BaseAddr, AL_U32 Addr)
{
    AL_REG32_WRITE(BaseAddr + AL_AXI_VDMA_MM2S_START_ADDRESS_OFFSET, Addr);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmVsize(AL_REG BaseAddr, AL_U32 Vsize)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_S2MM_VSIZE_OFFSET, 0, 13, Vsize);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmHsize(AL_REG BaseAddr, AL_U32 Hsize)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_S2MM_HSIZE_OFFSET, 0, 16, Hsize);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmFrameDelay(AL_REG BaseAddr, AL_U32 FrameDelay)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_S2MM_FRMDLY_STRIDE_OFFSET, 24, 5, FrameDelay);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmStride(AL_REG BaseAddr, AL_U32 Stride)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_VDMA_S2MM_FRMDLY_STRIDE_OFFSET, 0, 16, Stride);
}

static inline AL_VOID AlAxiVdma_ll_SetS2MmStartAddr(AL_REG BaseAddr, AL_U32 Addr)
{
    AL_REG32_WRITE(BaseAddr + AL_AXI_VDMA_S2MM_START_ADDRESS_OFFSET, Addr);
}

static inline AL_VOID AlAxiVdma_ll_EnableVerticalFlip(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_VDMA_ENABLE_VERTICAL_FLIP_OFFSET, 0, State);
}



#ifdef __cplusplus
}
#endif

#endif


