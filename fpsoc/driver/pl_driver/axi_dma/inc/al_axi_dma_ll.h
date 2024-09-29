/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_AXI_DMA_LL_H_
#define __AL_AXI_DMA_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_axi_dma_hw.h"
#include "al_utils_def.h"
#include "al_cache.h"
/**************************** Type Definitions ******************************/
typedef enum
{
    AL_AXI_DMA_FUNC_DISABLE    = 0,
    AL_AXI_DMA_FUNC_ENABLE     = 1
} AlAxiDma_FunctionEnum;

typedef enum {
    AL_AXIDMA_DMA_TO_DEVICE    = 0,
    AL_AXIDMA_DEVICE_TO_DMA    = 1
} AlAxiDma_TransDirEnum;

typedef enum {
    AL_AXIDMA_BLOCK            = 0,
    AL_AXIDMA_POLLING          = 1
} AlAxiDma_TransferMethodEnum;

typedef enum {
    AL_AXIDMA_INTR_IOC_DONE              = (1UL << 0),
    AL_AXIDMA_INTR_DLY_DONE              = (1UL << 1),
    AL_AXIDMA_INTR_ERR_OCCUR             = (1UL << 2)
} AlAxiDma_IntrEnum;

/***************** Macros (Inline Functions) Definitions *********************/
static inline AlAxiDma_IntrEnum AlAxiDma_ll_GetIntrStatus(AL_U64 ChanBase)
{
    return AL_REG32_GET_BITS(ChanBase + AL_AXI_DMA_SR_OFFSET, AL_AXI_DMA_SR_INTR_SHIFT,
                             AL_AXI_DMA_SR_INTR_SIZE);
}

static inline AL_U32 AlAxiDma_ll_GetStatus(AL_U64 ChanBase)
{
    return AL_REG32_READ(ChanBase + AL_AXI_DMA_SR_OFFSET);
}

static inline AL_VOID AlAxiDma_ll_ClrIoc(AL_U64 ChanBase)
{
    AL_REG32_SET_BIT(ChanBase + AL_AXI_DMA_SR_OFFSET, AL_AXI_DMA_SR_IOC_SHIFT, 1);
}

static inline AL_VOID AlAxiDma_ll_ClrErr(AL_U64 ChanBase)
{
    AL_REG32_SET_BIT(ChanBase + AL_AXI_DMA_SR_OFFSET, AL_AXI_DMA_SR_ERR_SHIFT, 1);
}

static inline AL_VOID AlAxiDma_ll_SetIocIntr(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + AL_AXI_DMA_CR_OFFSET, AL_AXI_DMA_CR_IOC_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_SetErrIntr(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + AL_AXI_DMA_CR_OFFSET, AL_AXI_DMA_CR_ERR_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_SetAddrLsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + AL_AXI_DMA_ADDR_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetAddrMsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + AL_AXI_DMA_ADDR_MSB_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetRunStop(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + AL_AXI_DMA_CR_OFFSET, AL_AXI_DMA_CR_RUNSTOP_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_Reset(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + AL_AXI_DMA_CR_OFFSET, AL_AXI_DMA_CR_RESET_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_SetLength(AL_U64 ChanBase, AL_U32 Length)
{
    AL_REG32_SET_BITS(ChanBase + AL_AXI_DMA_LENTH_OFFSET, AL_AXI_DMA_LENTH_START_SHIFT, AL_AXI_DMA_LENGTH_SIZE, Length);
}

static inline AL_BOOL AlAxiDma_ll_GetSrHalt(AL_U64 ChanBase)
{
    return AL_REG32_GET_BIT(ChanBase + AL_AXI_DMA_SR_OFFSET, AL_AXI_DMA_SR_HALT_SHIFT);
}

static inline AL_BOOL AlAxiDma_ll_GetSrIdle(AL_U64 ChanBase)
{
    return AL_REG32_GET_BIT(ChanBase + AL_AXI_DMA_SR_OFFSET, AL_AXI_DMA_SR_IDLE_SHIFT);
}

static inline AL_BOOL AlAxiDma_ll_GetCrReset(AL_U64 ChanBase)
{
    return AL_REG32_GET_BIT(ChanBase + AL_AXI_DMA_CR_OFFSET, AL_AXI_DMA_CR_RESET_SHIFT);
}

static inline AL_VOID AlAxiDma_ll_KeyHole(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + AL_AXI_DMA_CR_OFFSET, AL_AXI_DMA_CR_KEYHOLE_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_Cyclic(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + AL_AXI_DMA_CR_OFFSET, AL_AXI_DMA_CR_CYCLIC_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_SetThreshold(AL_U64 ChanBase, AL_U32 Threshold)
{
    AL_REG32_SET_BITS(ChanBase + AL_AXI_DMA_CR_OFFSET, AL_AXI_DMA_THRE_START_SHIFT, AL_AXI_DMA_THRE_SIZE, Threshold);
}

/**********************************************************************************/
static inline AL_VOID AlAxiDma_ll_SetCurDescLsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + AL_AXI_DMA_CURDESR_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetCurDescMsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + AL_AXI_DMA_CURDESR_MSB_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetTailDescLsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + AL_AXI_DMA_TAILDESR_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetTailDescMsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + AL_AXI_DMA_TAILDESR_MSB_OFFSET, Addr);
}
/**********************************************************************************/
#ifdef __cplusplus
}
#endif

#endif
