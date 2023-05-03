#ifndef AL_UART_LL_H
#define AL_UART_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_uart_hw.h"
#include "al_reg_io.h"

typedef enum
{
    UART_CHAR_5BITS                = 0,
    UART_CHAR_6BITS                = 1,
    UART_CHAR_7BITS                = 2,
    UART_CHAR_8BITS                = 3
} AL_UART_DataWidthEnum;

typedef enum
{
    UART_STOP_1BIT                 = 0,
    UART_STOP_1_5BIT_OR_2BIT       = 1
} AL_UART_StopBitsEnum;

typedef enum
{
    UART_NO_PARITY                 = 0x0,
    UART_ODD_PARITY                = 0x1,
    UART_EVEN_PARITY               = 0x3,
    UART_STICK_PARITY_1            = 0x5,
    UART_STICK_PARITY_0            = 0x7
} AL_UART_ParityEnum;

typedef enum
{
    UART_MODEM_STATUS              = 0x0,
    UART_NO_INTERRUPT_PENDING      = 0x1,
    UART_THR_EMPTY                 = 0x2,
    UART_RECEIVED_DATA_AVAILABLE   = 0x4,
    UART_RECEIVER_LINE_STATUS      = 0x6,
    UART_BUSY_DETECT               = 0x7,
    UART_CHARACTER_TIMEOUT         = 0xc
} AL_UART_InterruptEnum;

typedef enum
{
    UART_TxFIFO_EMPTY               = 0x0,
    UART_TxFIFO_CHAR_2              = 0x1,
    UART_TxFIFO_QUARTER_FULL        = 0x2,
    UART_TxFIFO_HALF_FULL           = 0x3
} AL_UART_TxFifoThrEnum;

typedef enum
{
    UART_RxFIFO_CHAR_1               = 0x0,
    UART_RxFIFO_QUARTER_FULL         = 0x1,
    UART_RxFIFO_HALF_FULL            = 0x2,
    UART_RxFIFO_FULL_2               = 0x3
} AL_UART_RxFifoThrEnum;

static inline AL_VOID AlUart_ll_SetParity(AL_REG32 BaseAddr, AL_UART_ParityEnum Parity)
{
    SET_BITS(BaseAddr + UART__LCR__OFFSET, UART__LCR__PARITY__SHIFT, UART__LCR__PARITY__SIZE, Parity);
}

static inline AL_VOID AlUart_ll_SetDataWidth(AL_REG32 BaseAddr, AL_UART_DataWidthEnum DataWidth)
{
    SET_BITS(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLS__SHIFT, UART__LCR__DLS__SIZE, DataWidth);
}

static inline AL_VOID AlUart_ll_SetStopBitsLength(AL_REG32 BaseAddr, AL_UART_StopBitsEnum StopBits)
{
    SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__STOP__SHIFT, StopBits);
}

static inline AL_VOID AlUart_ll_SetFifo(AL_REG32 BaseAddr, AL_FUNCTION State)
{
    SET_BIT(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__FIFOE__IID__SHIFT, State);
}

static inline AL_VOID AlUart_ll_SetTxFifoThr(AL_REG32 BaseAddr,AL_UART_TxFifoThrEnum FifoThr)
{
    SET_BITS(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__TET__RSVD_IIR__SHIFT,
             UART__FCR__IIR__TET__RSVD_IIR__SIZE, FifoThr);
}

static inline AL_VOID AlUart_ll_SetRxFifoThr(AL_REG32 BaseAddr,AL_UART_RxFifoThrEnum FifoThr)
{
    SET_BITS(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__RT__FIFOSE__SHIFT,
             UART__FCR__IIR__RT__FIFOSE__SSIZE, FifoThr);
}

static inline AL_VOID AlUart_ll_SendByte(AL_REG32 BaseAddr, AL_U8 Data)
{
    WRITE_REG(BaseAddr + UART__RBR__THR__DLL__OFFSET, Data);
}

static inline AL_U8 AlUart_ll_RecvByte(AL_REG32 BaseAddr)
{
    return READ_REG(BaseAddr + UART__RBR__THR__DLL__OFFSET);
}

static inline AL_BOOL AlUart_ll_IsThrEmpty(AL_REG32 BaseAddr)
{
    return GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__THRE__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsRxDataReady(AL_REG32 BaseAddr)
{
    return GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__DR__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsTransmitFifoFull(AL_REG32 BaseAddr)
{
    return GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__THRE__SHIFT);
}

static inline AL_VOID AlUart_ll_SetThreIntr(AL_REG32 BaseAddr, AL_FUNCTION State)
{
    SET_BIT(BaseAddr + UART__IER_DLH__OFFSET, UART__IER_DLH__PTIME__DLH__SHIFT, State);
}

static inline AL_VOID AlUart_ll_SetTxIntr(AL_REG32 BaseAddr, AL_FUNCTION State)
{
    SET_BIT(BaseAddr + UART__IER_DLH__OFFSET, UART__IER_DLH__ETBEI__DLH__SHIFT, State);
}

static inline AL_VOID AlUart_ll_SetRxIntr(AL_REG32 BaseAddr, AL_FUNCTION State)
{
    SET_BIT(BaseAddr + UART__IER_DLH__OFFSET, UART__IER_DLH__ERBFI__DLH__SHIFT, State);
}

static inline AL_UART_InterruptEnum AlUart_ll_GetIntrStatus(AL_REG32 BaseAddr)
{
    return GET_BITS(BaseAddr + UART__FCR__IIR__OFFSET, UART__FCR__IIR__FIFOE__IID__SHIFT,
                    UART__FCR__IIR__FIFOE__IID__SIZE);
}

static inline AL_BOOL AlUart_ll_IsOeIntr(AL_REG32 BaseAddr)
{
    return GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__OE__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsPeIntr(AL_REG32 BaseAddr)
{
    return GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__PE__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsFeIntr(AL_REG32 BaseAddr)
{
    return GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__FE__SHIFT);
}

static inline AL_BOOL AlUart_ll_IsBiIntr(AL_REG32 BaseAddr)
{
    return GET_BIT(BaseAddr + UART__LSR__OFFSET, UART__LSR__BI__SHIFT);
}

static inline AL_VOID AlUart_ll_SetBaudRate(AL_REG32 BaseAddr, AL_U32 BaudRate)
{
    SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_ENABLE);
    WRITE_REG(BaseAddr + UART__IER_DLH__OFFSET, ((AL_U16)(UART_CLOCK / (BaudRate*16))) >> 8);
    WRITE_REG(BaseAddr + UART__RBR__THR__DLL__OFFSET, ((AL_U8)(UART_CLOCK / (BaudRate*16))));
    SET_BIT(BaseAddr + UART__MCR__ADDR_OFFSET, UART__MCR__DTR__SHIFT, AL_FUNC_ENABLE);
    SET_BIT(BaseAddr + UART__MCR__ADDR_OFFSET, UART__MCR__RTS__SHIFT, AL_FUNC_ENABLE);
    SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLAB__SHIFT, AL_FUNC_DISABLE);
}

static inline AL_VOID AlUart_ll_CfgCharacter(AL_REG32 BaseAddr, AL_UART_DataWidthEnum DataWidth, AL_UART_ParityEnum Parity, AL_UART_StopBitsEnum StopBits)
{
    SET_BITS(BaseAddr + UART__LCR__OFFSET, UART__LCR__DLS__SHIFT, UART__LCR__DLS__SIZE, DataWidth);
    SET_BITS(BaseAddr + UART__LCR__OFFSET, UART__LCR__PARITY__SHIFT, UART__LCR__PARITY__SIZE, Parity);
    SET_BIT(BaseAddr + UART__LCR__OFFSET, UART__LCR__STOP__SHIFT, StopBits);
}

#ifdef __cplusplus
}
#endif

#endif
