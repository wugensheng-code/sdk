/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_axi_qspi_ll.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2024/08/27
 * @brief   axi-qspi device driver
 */

#ifndef __AL_AXI_QSPI_LL_H__
#define __AL_AXI_QSPI_LL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "al_axi_quad_spi_hw.h"
#include "al_type.h"
#include "al_reg_io.h"


/***************** Macros (Inline Functions) Definitions *********************/
#if AXI_QSPI_XIP_MODE == 0

static inline AL_VOID AlAxiQspi_ll_SwRst(AL_U64 BaseAddr)
{
    AL_REG32_WRITE(BaseAddr + AXI_QSPI_SW_RST_OFFSET, AXI_QSPI_SW_RST_KEY_VAL);
}

static inline AL_VOID AlAxiQspi_ll_SetLoopback(AL_U64 BaseAddr, AL_U32 IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_LOOP_SHIFT, IsEnable);
}

static inline AL_VOID AlAxiQspi_ll_SetSpiEnable(AL_U64 BaseAddr, AL_U32 IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_SPE_SHIFT, IsEnable);
}

static inline AL_VOID AlAxiQspi_ll_SetMasterSlave(AL_U64 BaseAddr, AL_U32 IsMaster)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_MASTER_SHIFT, IsMaster);
}

static inline AL_VOID AlAxiQspi_ll_SetCPOL(AL_U64 BaseAddr, AL_U32 CPOL)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_CPOL_SHIFT, CPOL);
}

static inline AL_VOID AlAxiQspi_ll_SetCPHA(AL_U64 BaseAddr, AL_U32 CPHA)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_CPHA_SHIFT, CPHA);
}

static inline AL_VOID AlAxiQspi_ll_ResetTxFifo(AL_U64 BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_TX_FIFO_RST_SHIFT, 1);
}

static inline AL_VOID AlAxiQspi_ll_ResetRxFifo(AL_U64 BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_RX_FIFO_RST_SHIFT, 1);
}

static inline AL_VOID AlAxiQspi_ll_SetManualSlaveSelect(AL_U64 BaseAddr, AL_U32 ISManual)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_MANUAL_SLAVE_EN_SHIFT, ISManual);
}

static inline AL_VOID AlAxiQspi_ll_SetMasterTransInhibit(AL_U64 BaseAddr, AL_U32 IsInhibit)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_MASTER_TRANS_INHIBIT_SHIFT, IsInhibit);
}

static inline AL_VOID AlAxiQspi_ll_SetLsbFirst(AL_U64 BaseAddr, AL_U32 IsLsbFirst)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_CTL_OFFSET, AXI_QSPI_SPI_CTL_LSB_FIRST_SHIFT, IsLsbFirst);
}

static inline AL_U32 AlAxiQspi_ll_GetStatus(AL_U64 BaseAddr, AL_U32 Event)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_QSPI_SPI_STATUS_OFFSET, Event);
}

static inline AL_VOID AlAxiQspi_ll_WriteTxBuf(AL_U64 BaseAddr, AL_U32 Data)
{
    AL_REG32_WRITE(BaseAddr + AXI_QSPI_SPI_DTR_OFFSET, Data);
}

static inline AL_U32 AlAxiQspi_ll_ReadRxBuf(AL_U64 BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_QSPI_SPI_DRR_OFFSET);
}

static inline AL_VOID AlAxiQspi_ll_SetSlaveSelect(AL_U64 BaseAddr, AL_U32 SlvIdx, AL_U32 IsSelect)
{
    AL_REG32_WRITE(BaseAddr + AXI_QSPI_SPI_SLV_SEL_OFFSET, 
        IsSelect ? (0xFFFFFFFFU & ~(1 << SlvIdx)) : 0xFFFFFFFFU);
}

static inline AL_U32 AlAxiQspi_ll_GetTxFifoLevel(AL_U64 BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + AXI_QSPI_SPI_TX_FIFO_OCY_OFFSET, 
        AXI_QSPI_SPI_TX_FIFO_OCY_VAL_SHIFT, AXI_QSPI_SPI_TX_FIFO_OCY_VAL_SIZE);
}

static inline AL_U32 AlAxiQspi_ll_SetRxFifoLevel(AL_U64 BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + AXI_QSPI_SPI_RX_FIFO_OCY_OFFSET, 
        AXI_QSPI_SPI_RX_FIFO_OCY_VAL_SHIFT, AXI_QSPI_SPI_RX_FIFO_OCY_VAL_SIZE);
}

static inline AL_VOID ALAxiQspi_ll_SetGlobalIntr(AL_U64 BaseAddr, AL_U32 IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_DEV_GLB_INTR_EN_OFFSET, AXI_QSPI_SPI_DEV_GLB_INTR_EN_SHIFT, IsEnable);
}

static inline AL_U32 AlAxiQspi_ll_GetIntrStatus(AL_U64 BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_QSPI_SPI_INTR_STA_OFFSET);
}

static inline AL_VOID AlAxiQspi_ll_ClearIntrStatus(AL_U64 BaseAddr, AL_U32 Event)
{
    AL_REG32_WRITE(BaseAddr + AXI_QSPI_SPI_INTR_STA_OFFSET, 1 << Event);
}

static inline AL_VOID AlAxiQspi_ll_ClearAllIntrStatus(AL_U64 BaseAddr)
{
    AL_REG32_WRITE(BaseAddr + AXI_QSPI_SPI_INTR_STA_OFFSET, 
        AL_REG32_READ(BaseAddr + AXI_QSPI_SPI_INTR_STA_OFFSET));
}

static inline AL_VOID AlAxiQspi_ll_SetIntrStaReg(AL_U64 BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + AXI_QSPI_SPI_INTR_STA_OFFSET, Value);
}

static inline AL_VOID AlAxiQspi_ll_SetIntrEnable(AL_U64 BaseAddr, AL_U32 Event, AL_U32 IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + AXI_QSPI_SPI_INTR_EN_OFFSET, Event, IsEnable);
}

static inline AL_U32 AlAxiQspi_ll_GetIntrEnReg(AL_U64 BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AXI_QSPI_SPI_INTR_EN_OFFSET);
}

static inline AL_VOID AlAxiQspi_ll_SetIntrEnReg(AL_U64 BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + AXI_QSPI_SPI_INTR_EN_OFFSET, Value);
}

static inline AL_VOID AlAxiQspi_ll_DisableAllIntr(AL_U64 BaseAddr)
{
    AL_REG32_WRITE(BaseAddr + AXI_QSPI_SPI_INTR_EN_OFFSET, 0);
}

#else

static inline AL_VOID AlAxiQSpi_ll_XipSetCr(AL_U64 BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + AXI_QSPI_SPI_XIP_CR_OFFSET, Value);
}

static inline AL_U32 AlAxiQspi_ll_XipGetStatus(AL_U64 BaseAddr, AL_U32 Event)
{
    return AL_REG32_GET_BIT(BaseAddr + AXI_QSPI_SPI_XIP_SR_OFFSET, Event);
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* __AL_AXI_QSPI_LL_H__ */