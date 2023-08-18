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

#ifdef USE_RISCV_CORE
#include "al_rv_core.h"
#include "ext_eclic.h"
#elifdef USE_ARM_CORE

#endif

void rt_hw_board_init(void);

#endif /* __BOARD__ */

/******************** end of file *******************/
