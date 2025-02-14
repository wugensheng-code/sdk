/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_uart_test_config.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-05-17
 * @brief   uart test config file
 */

#ifndef AL_UART_TEST_CONFIG_H
#define AL_UART_TEST_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "al_uart_hal.h"
#include "al_gpio_hal.h"

/************************** Constant Definitions *****************************/
#define BUF_SIZE    0x10
#define AL_UART_TIME_OUT_MS 10000
#define AL_UART_DEVID 1

AL_U8 SendBuffer[BUF_SIZE];	/* Buffer for Transmitting Data */
AL_U8 RecvBuffer[BUF_SIZE];	/* Buffer for Receiving Data */


/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/
static AL_UART_InitStruct UART_InitStruct = {
        .BaudRate           = 115200,
        .Parity             = AL_UART_NO_PARITY,
        .WordLength         = AL_UART_CHAR_8BITS,
        .StopBits           = AL_UART_STOP_1BIT,
        .HwFlowCtl          = AL_FALSE,
        .CharTimeoutEnable  = AL_TRUE
    };

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
