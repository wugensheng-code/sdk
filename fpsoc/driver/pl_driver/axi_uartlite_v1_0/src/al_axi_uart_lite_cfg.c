/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_uart_lite_hw.h"

AlAxiUartLite_HwConfigStruct AlAxiUartLite_HwConfig[AL_AXI_UART_LITE_NUM_INSTANCE] =
{
    {
        .DeviceId       = (AL_U32)AXI_UART_LITE0_DEV_ID,
        .BaseAddress    = (AL_U64)AXI_UART_LITE0_BASE_ADDR,
        .IntrId         = (AL_U32)AXI_UART_LITE0_IRQn,
        .BandRate       = (AL_U32)AXI_UART_LITE0_BAUD_RATE,
        .DataWidth      = (AL_U8)AXI_UART_LITE0_DATA_WIDTH,
        .ParityEn       = (AL_BOOL)AXI_UART_LITE0_PARITY_EN,
        .ParityOdd      = (AL_BOOL)AXI_UART_LITE0_PARITY_ODD,
        .ParityEven     = (AL_BOOL)AXI_UART_LITE0_PARITY_EVEN,
    },
};
