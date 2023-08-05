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
#include "al_uart_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************************** Constant Definitions *****************************/
#define BUF_SIZE 0X10
#define TIME_OUT 1000000

/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Variable Definitions *****************************/
static AL_UART_InitStruct UART_InitStruct = {
        .BaudRate           = 115200,
        .Parity             = UART_NO_PARITY,
        .WordLength         = UART_CHAR_8BITS,
        .StopBits           = UART_STOP_1BIT,
        .CharTimeoutEnable  = AL_TRUE
    };

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
