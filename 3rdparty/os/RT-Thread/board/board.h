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

#include "nmsis_core.h"
#include "al_intr.h"
#include "al_hal.h"

#define BSP_UART0_RX_BUFSIZE   256
#define BSP_UART0_TX_BUFSIZE   256

void rt_hw_board_init(void);

#endif /* __BOARD__ */

/******************** end of file *******************/
