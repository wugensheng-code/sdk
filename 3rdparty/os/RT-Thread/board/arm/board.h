/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-5-30      Bernard      the first version
 */

#ifndef BOARD_H__
#define BOARD_H__

#include <virt.h>
#include "al_gicv3.h"
#include <al_core.h>


extern void *_heap_start;
extern void *_heap_end;

#define RT_HW_HEAP_BEGIN    &_heap_start
#define RT_HW_HEAP_END      &_heap_end

void rt_hw_board_init(void);

#endif
