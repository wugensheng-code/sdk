#ifndef __AL_AXI_DMA_LL_H_
#define __AL_AXI_DMA_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_axi_dma_hw.h"
#include "al_utils_def.h"
#include "al_cache.h"

typedef enum
{
    AL_AXI_DMA_FUNC_DISABLE     = 0,
    AL_AXI_DMA_FUNC_ENABLE      = 1
} AlAxiDma_FunctionEnum;

typedef enum {
    AL_AXIDMA_DMA_TO_DEVICE    = 0,
    AL_AXIDMA_DEVICE_TO_DMA    = 1
} AlAxiDma_TransDirEnum;

typedef enum {
    AL_AXIDMA_DIRECT_MODE  = 0,
    AL_AXIDMA_SG_MODE      = 1
} AlAxiDma_ModeEnum;

typedef enum {
    AL_AXIDMA_BLOCK    = 0,
    AL_AXIDMA_POLLING  = 1
} AlAxiDma_TransferMethodEnum;

typedef enum {
    AL_AXIDMA_INTR_IOC_DONE              = (0x1 << 0),
    AL_AXIDMA_INTR_DLY_DONE              = (0x1 << 1),
    AL_AXIDMA_INTR_ERR_OCCUR             = (0x1 << 2)
} AlAxiDma_IntrEnum;

static inline AlAxiDma_IntrEnum AlAxiDma_ll_GetIntrStatus(AL_U64 ChanBase)
{
    return AL_REG32_GET_BITS(ChanBase + ALAXIDMA_SR_OFFSET, ALAXIDMA_SR_INTR_SHIFT,
                             ALAXIDMA_SR_INTR_SIZE);
}

static inline AL_U32 AlAxiDma_ll_GetStatus(AL_U64 ChanBase)
{
    return AL_REG32_READ(ChanBase + ALAXIDMA_SR_OFFSET);
}

static inline AL_VOID AlAxiDma_ll_ClrIoc(AL_U64 ChanBase)
{
    AL_REG32_SET_BIT(ChanBase + ALAXIDMA_SR_OFFSET, ALAXIDMA_SR_IOC_SHIFT, 1);
}

static inline AL_VOID AlAxiDma_ll_ClrErr(AL_U64 ChanBase)
{
    AL_REG32_SET_BIT(ChanBase + ALAXIDMA_SR_OFFSET, ALAXIDMA_SR_ERR_SHIFT, 1);
}

static inline AL_VOID AlAxiDma_ll_SetIocIntr(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + ALAXIDMA_CR_OFFSET, ALAXIDMA_CR_IOC_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_SetErrIntr(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + ALAXIDMA_CR_OFFSET, ALAXIDMA_CR_ERR_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_SetAddrLsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + ALAXIDMA_ADDR_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetAddrMsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + ALAXIDMA_ADDR_MSB_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetRunStop(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + ALAXIDMA_CR_OFFSET, ALAXIDMA_CR_RUNSTOP_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_Reset(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + ALAXIDMA_CR_OFFSET, ALAXIDMA_CR_RESET_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_SetLength(AL_U64 ChanBase, AL_U32 Length)
{
    AL_REG32_SET_BITS(ChanBase + ALAXIDMA_LENTH_OFFSET, ALAXIDMA_LENTH_START_SHIFT, ALAXIDMA_LENGTH_SIZE, Length);
}

static inline AL_BOOL AlAxiDma_ll_GetSrHalt(AL_U64 ChanBase)
{
    return AL_REG32_GET_BIT(ChanBase + ALAXIDMA_SR_OFFSET, ALAXIDMA_SR_HALT_SHIFT);
}

static inline AL_BOOL AlAxiDma_ll_GetSrIdle(AL_U64 ChanBase)
{
    return AL_REG32_GET_BIT(ChanBase + ALAXIDMA_SR_OFFSET, ALAXIDMA_SR_IDLE_SHIFT);
}

static inline AL_BOOL AlAxiDma_ll_GetCrReset(AL_U64 ChanBase)
{
    return AL_REG32_GET_BIT(ChanBase + ALAXIDMA_CR_OFFSET, ALAXIDMA_CR_RESET_SHIFT);
}

static inline AL_VOID AlAxiDma_ll_KeyHole(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + ALAXIDMA_CR_OFFSET, ALAXIDMA_CR_KEYHOLE_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_Cyclic(AL_U64 ChanBase, AlAxiDma_FunctionEnum State)
{
    AL_REG32_SET_BIT(ChanBase + ALAXIDMA_CR_OFFSET, ALAXIDMA_CR_CYCLIC_SHIFT, State);
}

static inline AL_VOID AlAxiDma_ll_SetThreshold(AL_U64 ChanBase, AL_U32 Threshold)
{
    AL_REG32_SET_BITS(ChanBase + ALAXIDMA_CR_OFFSET, ALAXIDMA_THRE_START_SHIFT, ALAXIDMA_THRE_SIZE, Threshold);
}

/**********************************************************************************/
static inline AL_VOID AlAxiDma_ll_SetCurDescLsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + ALAXIDMA_CURDESR_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetCurDescMsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + ALAXIDMA_CURDESR_MSB_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetTailDescLsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + ALAXIDMA_TAILDESR_OFFSET, Addr);
}

static inline AL_VOID AlAxiDma_ll_SetTailDescMsb(AL_U64 ChanBase, AL_U32 Addr)
{
    AL_REG32_WRITE(ChanBase + ALAXIDMA_TAILDESR_MSB_OFFSET, Addr);
}
/**********************************************************************************/
#ifdef __cplusplus
}
#endif

#endif
