/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef AL_AXI_UART_LITE_HW_H_
#define AL_AXI_UART_LITE_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

// test, parameter from TD hpf file
#define AL_AXI_UART_LITE_NUM_INSTANCE 2
#define AXI_UART_LITE0_DEV_ID 0
#define AXI_UART_LITE0_BASE_ADDR 0x80020000
#define AXI_UART_LITE0_IRQn 116
#define AXI_UART_LITE0_BAUD_RATE 115200
#define AXI_UART_LITE0_DATA_WIDTH 8
#define AXI_UART_LITE0_PARITY_EN  0
#define AXI_UART_LITE0_PARITY_ODD  0
#define AXI_UART_LITE0_PARITY_EVEN  0

/***************************** Include Files ********************************/
#include "al_core.h"

typedef struct {
    AL_U32      DeviceId;
    AL_U64      BaseAddress;
    AL_U32      InputClockHz;
    AL_U32      IntrId;
    AL_U32      BandRate;
    AL_U8       DataWidth;
    AL_BOOL     ParityEn;
    AL_BOOL     ParityOdd;
    AL_BOOL     ParityEven;
} AlAxiUartLite_HwConfigStruct;

/***************************** Register Offset ********************************/
#define AL_AXI_UART_LITE_RX_FIFO_OFFSET       0x00
#define AL_AXI_UART_LITE_TX_FIFO_OFFSET       0x04
#define AL_AXI_UART_LITE_CR_OFFSET            0x0c
#define AL_AXI_UART_LITE_SR_OFFSET            0x08

/***************************** AL_AXI_UART_LITE_CR_OFFSET Register    *****************************/
#define AL_AXI_UART_LITE_CR_TXFIFO_RST_SHIFT            0
#define AL_AXI_UART_LITE_CR_RXFIFO_RST_SHIFT            1
#define AL_AXI_UART_LITE_CR_ENABLE_INT_SHIFT            4

/***************************** AL_AXI_UART_LITE_SR_OFFSET Register   *****************************/
#define AL_AXI_UART_LITE_SR_RXFIFO_HAS_DATA_SHIFT   0
#define AL_AXI_UART_LITE_SR_RXFIFO_FULL_SHIFT       1
#define AL_AXI_UART_LITE_SR_TXFIFO_EMPTY_SHIFT      2
#define AL_AXI_UART_LITE_SR_TXFIFO_FULL_SHIFT       3
#define AL_AXI_UART_LITE_SR_INT_EN_SHIFT            4
#define AL_AXI_UART_LITE_SR_OVERRUN_ERR_SHIFT       5
#define AL_AXI_UART_LITE_SR_FRAME_ERR_SHIFT         6
#define AL_AXI_UART_LITE_SR_PARITY_ERR_SHIFT        7



#ifdef __cplusplus
}
#endif

#endif


