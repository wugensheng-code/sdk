/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author          Notes
 * 2020-04-02     Huaqi Fang      first version
 *
 */

#ifndef __BOARD__
#define __BOARD__

#include <rtthread.h>
#include "al_hal.h"
#include "al_cli.h"


#define BSP_UART0_RX_BUFSIZE   256
#define BSP_UART0_TX_BUFSIZE   256

#ifndef CLI_THREAD_NAME
    #define CLI_THREAD_NAME   "tcli"
#endif
#ifndef CLI_THREAD_PRIORITY
    #define CLI_THREAD_PRIORITY 15
#endif
#ifndef CLI_THREAD_STACK_SIZE
    #define CLI_THREAD_STACK_SIZE 2048
#endif

void rt_hw_board_init(void);

#endif /* __BOARD__ */

/******************** end of file *******************/
