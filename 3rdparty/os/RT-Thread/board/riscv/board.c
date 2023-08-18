/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-22     hqfang       first version
 *
 */

#include <rtthread.h>
#include "board.h"
#include "cpuport.h"
#include <unistd.h>
#include <stdio.h>
#include <al_log.h>
#include <rthw.h>
#include "board.h"
#include "al_chip.h"

#ifdef USE_ARM_CORE
#include <mmu.h>
#include <gic.h>
#include <gicv3.h>
#include <psci.h>
#include <gtimer.h>
#include <cpuport.h>
#include <interrupt.h>
#endif

#ifndef RT_USING_HEAP
    static struct rt_thread cli_thread;
    ALIGN(RT_ALIGN_SIZE)
    static char cli_thread_stack[CLI_THREAD_STACK_SIZE];
#endif

extern AL_CLI_CmdInfoStruct *CliCmdInfo;

/** _end symbol defined in linker script of Nuclei SDK */
extern void *_heap_start;

/** _heap_end symbol defined in linker script of Nuclei SDK */
extern void *_heap_end;
#define HEAP_BEGIN  &_heap_start
#define HEAP_END    &_heap_end


/*
 * - Implemented and defined in Nuclei SDK system_<Device>.c file
 * - Required macro NUCLEI_BANNER set to 0
 */

extern void _init(void);
/**
 * @brief Setup hardware board for rt-thread
 *
 */
void rt_hw_board_init(void)
{

    /* OS Tick Configuration */
    rt_hw_ticksetup();

#ifdef RT_USING_HEAP
    rt_system_heap_init((void *) HEAP_BEGIN, (void *) HEAP_END);
#endif

    // _init(); // __libc_init_array is not used in RT-Thread


    /* Set the shell console output device */
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    /* Board underlying hardware initialization */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}
