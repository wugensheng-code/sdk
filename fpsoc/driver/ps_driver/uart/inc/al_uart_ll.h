/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_UART_LL_H_
#define __AL_UART_LL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "al_uart_hw.h"

typedef enum
{
    AL_UART_CHAR_5BITS                = 0,
    AL_UART_CHAR_6BITS                = 1,
    AL_UART_CHAR_7BITS                = 2,
    AL_UART_CHAR_8BITS                = 3
} AL_UART_DataWidthEnum;

typedef enum
{
    AL_UART_STOP_1BIT                 = 0,
    AL_UART_STOP_1_5BIT_OR_2BIT       = 1
} AL_UART_StopBitsEnum;

typedef enum
{
    AL_UART_NO_PARITY                 = 0x0,
    AL_UART_ODD_PARITY                = 0x1,
    AL_UART_EVEN_PARITY               = 0x3,
    AL_UART_STICK_PARITY_1            = 0x5,
    AL_UART_STICK_PARITY_0            = 0x7
} AL_UART_ParityEnum;

typedef enum
{
    AL_UART_MODEM_STATUS              = 0x0,
    AL_UART_NO_INTERRUPT_PENDING      = 0x1,
    AL_UART_THR_EMPTY                 = 0x2,
    AL_UART_RECEIVED_DATA_AVAILABLE   = 0x4,
    AL_UART_RECEIVER_LINE_STATUS      = 0x6,
    AL_UART_BUSY_DETECT               = 0x7,
    AL_UART_CHARACTER_TIMEOUT         = 0xc
} AL_UART_IntrEnum;

typedef enum
{
    AL_UART_TxFIFO_EMPTY               = 0x0,
    AL_UART_TxFIFO_CHAR_2              = 0x1,
    AL_UART_TxFIFO_QUARTER_FULL        = 0x2,
    AL_UART_TxFIFO_HALF_FULL           = 0x3
} AL_UART_TxFifoThrEnum;

typedef enum
{
    AL_UART_RxFIFO_CHAR_1               = 0x0,
    AL_UART_RxFIFO_QUARTER_FULL         = 0x1,
    AL_UART_RxFIFO_HALF_FULL            = 0x2,
    AL_UART_RxFIFO_FULL_2               = 0x3
} AL_UART_RxFifoThrEnum;

static inline AL_VOID AlUart_ll_ResetUart0Bus()
{
    AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART0__SRST__N__SHIFT, AL_FUNC_DISABLE);
    AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART0__SRST__N__SHIFT, AL_FUNC_ENABLE);
}

static inline AL_VOID AlUart_ll_ResetUart1Bus()
{
    AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART1__SRST__N__SHIFT, AL_FUNC_DISABLE);
    AL_REG32_SET_BIT(CRP__BASE_ADDR + CRP__SRST_CTRL3__OFFSET, CRP__SRST_CTRL3__UART1__SRST__N__SHIFT, AL_FUNC_ENABLE);
}

static inline AL_VOID AlUart_ll_DisableAllIntr(AL_REG BaseAddr)
{
    AL_U32 IntrStatus = AL_REG32_READ(BaseAddr + UART__IER_DLH__OFFSET) & (~0x0f);
    AL_REG32_WRITE(BaseAddr + UART__IER_DLH__OFFSET, IntrStatus);
}

static inline AL_VOID AlUart_ll_ResetDllDlhReg(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_ENABLE);
    AL_REG32_WRITE(BaseAddr + UART__IER_DLH__OFFSET, 0);
    AL_REG32_WRITE(BaseAddr + UART__RBR__THR__DLL__OFFSET, 0);
    AL_REG32_SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_DISABLE);
}

static inline AL_VOID AlUart_ll_Set_LoopBack(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + UART__MCR__ADDR_OFFSET, UART__MCR__LOOPBACK__SHIFT, State);
}

static inline AL_VOID AlUart_ll_SetDataWidth(AL_REG BaseAddr, AL_UART_DataWidthEnum DataWidth)
{
    AL_REG32_SET_BITS(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLS__SHIFT, UART__LCR__DLS__SIZE, DataWidth);
}

static inline AL_VOID AlUart_ll_SetStopBitsLength(AL_REG BaseAddr, AL_UART_StopBitsEnum StopBits)
{
    AL_REG32_SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__STOP__SHIFT, StopBits);
}

static inline AL_VOID AlUart_ll_SetParity(AL_REG BaseAddr, AL_UART_ParityEnum Parity)
{
    AL_REG32_SET_BITS(BaseAddr + UART__LCR__OFFSET, UART__LCR__PARITY__SHIFT, UART__LCR__PARITY__SIZE, Parity);
}

static inline AL_U32 AlUart_ll_GetParity(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + UART__LCR__OFFSET, UART__LCR__PARITY__SHIFT, UART__LCR__PARITY__SIZE);
}

static inline AL_U32 AlUart_ll_GetDataWidth(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLS__SHIFT, UART__LCR__DLS__SIZE);
}

static inline AL_BOOL AlUart_ll_GetStopBitsLength(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__STOP__SHIFT);
}

static inline AL_VOID AlUart_ll_EnableFifo(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__FIFOE__IID__SHIFT, State);
}

static inline AL_VOID AlUart_ll_ResetRxFifo(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__RFIFOR__IID__SHIFT, AL_TRUE);
}

static inline AL_VOID AlUart_ll_ResetTxFifo(AL_REG BaseAddr)
{
    AL_REG32_SET_BIT(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__XFIFOR__IID__SHIFT, AL_TRUE);
}

static inline AL_VOID AlUart_ll_SetTxFifoThre(AL_REG BaseAddr,AL_UART_TxFifoThrEnum FifoThre)
{
    AL_REG32_SET_BITS(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__TET__RSVD_IIR__SHIFT,
             UART__FCR__IIR__TET__RSVD_IIR__SIZE, FifoThre);
}

static inline AL_VOID AlUart_ll_SetRxFifoThre(AL_REG BaseAddr,AL_UART_RxFifoThrEnum FifoThre)
{
    AL_REG32_SET_BITS(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__RT__FIFOSE__SHIFT,
             UART__FCR__IIR__RT__FIFOSE__SSIZE, FifoThre);
}

static inline AL_VOID AlUart_ll_SendByte(AL_REG BaseAddr, AL_U8 Data)
{
    AL_REG32_WRITE(BaseAddr + UART__RBR__THR__DLL__OFFSET, Data);
}

static inline AL_U8 AlUart_ll_RecvByte(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + UART__RBR__THR__DLL__OFFSET);
}

static inline AL_BOOL AlUart_ll_IsTxDone(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__TEMT__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsUartBusy(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__USR__OFFSET, UART__USR__BUSY__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsRxDataReady(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__DR__SHIFT);
}

static inline AL_BOOL AlUart_ll_GetThreState(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__IER_DLH__OFFSET, UART__IER_DLH__PTIME__DLH__SHIFT);
}

/* If the 'THRE' mode is turned on, this function indicates
 * that fifo is full, otherwise it indicates that fifo is empty.
 */
static inline AL_BOOL AlUart_ll_IsTxFifoFull(AL_REG BaseAddr)
{
    while (!AlUart_ll_GetThreState(BaseAddr));
    return AL_REG32_GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__THRE__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsThrEmpty(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__THRE__SHIFT);
}

static inline AL_VOID AlUart_ll_EnableThreIntr(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + UART__IER_DLH__OFFSET, UART__IER_DLH__PTIME__DLH__SHIFT, State);
}

static inline AL_VOID AlUart_ll_EnableLineIntr(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + UART__IER_DLH__OFFSET, UART__IER_DLH__ELSI__DLH__SHIFT, State);
}

static inline AL_VOID AlUart_ll_SetTxIntr(AL_REG BaseAddr, AL_FUNCTION State)
{
    AL_REG32_SET_BIT(BaseAddr + UART__IER_DLH__OFFSET, UART__IER_DLH__ETBEI__DLH__SHIFT, State);
}

static inline AL_VOID AlUart_ll_SetRxIntr(AL_REG BaseAddr, AL_FUNCTION State)
{
    AL_REG32_SET_BIT(BaseAddr + UART__IER_DLH__OFFSET, UART__IER_DLH__ERBFI__DLH__SHIFT, State);
}

static inline AL_UART_IntrEnum AlUart_ll_GetIntrStatus(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__FIFOE__IID__SHIFT,
                             UART__FCR__IIR__FIFOE__IID__SIZE);
}

static inline AL_U32 AlUart_ll_GetLineRegStatus(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + UART__LSR__OFFSET);
}

static inline AL_BOOL AlUart_ll_IsOeIntr(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__OE__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsPeIntr(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__PE__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsFeIntr(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__FE__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsBiIntr(AL_REG BaseAddr)
{
    return AL_REG32_GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__BI__SHIFT);
}

static inline AL_VOID AlUart_ll_SetBaudRate(AL_REG BaseAddr, AL_U32 BaudRate, AL_U32 InputClockHz)
{
    AL_REG32_SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_ENABLE);
    AL_REG32_WRITE(BaseAddr + UART__IER_DLH__OFFSET, (((InputClockHz >> 4) + (BaudRate >> 1)) / BaudRate) >> 8);
    AL_REG32_WRITE(BaseAddr + UART__RBR__THR__DLL__OFFSET, (((InputClockHz >> 4) + (BaudRate >> 1)) / BaudRate));
    AL_REG32_SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_DISABLE);
}

static inline AL_U32 AlUart_ll_GetBaudRate(AL_REG BaseAddr, AL_U32 InputClockHz)
{
    AL_U16 Divisor;
    AL_U32 BaudRate;
    AL_REG32_SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_ENABLE);
    Divisor = (AL_REG32_READ(BaseAddr + UART__IER_DLH__OFFSET) << 8) | (AL_REG32_READ(BaseAddr + UART__RBR__THR__DLL__OFFSET));
    BaudRate = (AL_U32)(InputClockHz / (16 * Divisor));
    AL_REG32_SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_DISABLE);
    return BaudRate;
}

static inline AL_VOID AlUart_ll_SetAutoFlowCtl(AL_REG BaseAddr, AL_BOOL State)
{
    AL_REG32_SET_BIT(BaseAddr + UART__MCR__ADDR_OFFSET, UART__MCR__AFCE__SHIFT, State);
    AL_REG32_SET_BIT(BaseAddr + UART__MCR__ADDR_OFFSET, UART__MCR__RTS__SHIFT, State);
}

#ifdef __cplusplus
}
#endif

#endif
