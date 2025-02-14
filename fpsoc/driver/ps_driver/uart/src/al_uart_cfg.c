/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/********************************* including Files *********************************/
#include "al_hal.h"
#include "al_uart_hw.h"

/********************************* Constant Definitions *****************************/


/********************************* Type definitions *********************************/


/********************************* Macros  & Inline Functions Definitions ***********/


/******************************** Function Prototypes ******************************/


/******************************** Variable Prototypes ******************************/

AL_UART_HwConfigStruct AlUart_HwConfig[AL_UART_NUM_INSTANCE] =
{
#ifdef UART0PS_ENABLE
    {
        .DeviceId           = 0,
        .BaseAddress        = UART0__BASE_ADDR,
        .InputClockHz       = UART_CLOCK,
        .IntrNum            = SOC_UART0_IRQn,
        .ModemPinsConnected = 1
    },
#endif

#ifdef UART1PS_ENABLE
    {
        .DeviceId           = 1,
        .BaseAddress        = UART1__BASE_ADDR,
        .InputClockHz       = UART_CLOCK,
        .IntrNum            = SOC_UART1_IRQn,
        .ModemPinsConnected = 1
    }
#endif
};