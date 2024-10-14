/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_UART_LITE_LL_H_
#define AL_AXI_UART_LITE_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_axi_uart_lite_hw.h"

static inline AL_U8 AlAxiUartLite_RecvData(AL_U64 BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + AL_AXI_UART_LITE_RX_FIFO_OFFSET, 0, 8);
}

static inline AL_U32 AlAxiUartLite_SendData(AL_U64 BaseAddr, AL_U8 Data)
{
    AL_REG32_SET_BITS(BaseAddr + AL_AXI_UART_LITE_TX_FIFO_OFFSET, 0, 8, Data);
}

static inline AL_U32 AlAxiUartLite_GetStatus(AL_U64 BaseAddr)
{
    return AL_REG32_READ(BaseAddr + AL_AXI_UART_LITE_SR_OFFSET);
}

static inline AL_VOID AlAxiUartLite_ResetTxFifo(AL_U64 BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_UART_LITE_CR_OFFSET, AL_AXI_UART_LITE_CR_TXFIFO_RST_SHIFT, 1);
}

static inline AL_VOID AlAxiUartLite_ResetRxFifo(AL_U64 BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_UART_LITE_CR_OFFSET, AL_AXI_UART_LITE_CR_RXFIFO_RST_SHIFT, 1);
}

static inline AL_VOID AlAxiUartLite_EnableIntr(AL_U64 BaseAddr, AL_BOOL IntrStatus)
{
    AL_REG32_SET_BIT(BaseAddr + AL_AXI_UART_LITE_CR_OFFSET, AL_AXI_UART_LITE_CR_ENABLE_INT_SHIFT, IntrStatus);
}


#ifdef __cplusplus
}
#endif

#endif /* AL_AXI_UART_LITE_LL_H_ */
