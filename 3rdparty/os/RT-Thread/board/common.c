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
#include "common.h"
#include "cpuport.h"
#include <unistd.h>
#include <stdio.h>
#include <al_log.h>
#include <rthw.h>
#include "board.h"
#include "al_chip.h"


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


void rt_hw_console_output(const char* str)
{
    rt_size_t size = 0;
    rt_int32_t ret = RT_EOK;
    char cr = '\r';

    while (*str != '\0')
    {
		// 换行
		if ( *str == '\n')	
		{
			ret = AlLog_Write(&cr, 1);
		}

        ret = AlLog_Write(str++, 1);
    }
    
}




void cli_thread_entry(void *parameter)
{
    AL_S32 Ret = AL_OK;

    Ret = AlCli_Init(AL_CLI_CONSOLE_UART);
    if (Ret) {
        AL_CLI_PRINTF("Cli Init failed");
        return;
    }

    AlCli_Main(AL_NULL);
}

int cli_system_init(void)
{
    rt_err_t result = RT_EOK;
    rt_thread_t tid;


#ifdef RT_USING_HEAP
    /* create or set shell structure */
    tid = rt_thread_create(CLI_THREAD_NAME,
                           cli_thread_entry, RT_NULL,
                           CLI_THREAD_STACK_SIZE, CLI_THREAD_PRIORITY, 10);
#else
    shell = &_shell;
    tid = &finsh_thread;
    result = rt_thread_init(&cli_thread,
                            CLI_THREAD_NAME,
                            cli_thread_entry, RT_NULL,
                            &cli_thread_stack[0], sizeof(cli_thread_stack),
                            CLI_THREAD_PRIORITY, 10);
#endif /* RT_USING_HEAP */

    if (tid != NULL && result == RT_EOK)
        rt_thread_startup(tid);
    return 0;
}
INIT_APP_EXPORT(cli_system_init);

/******************** end of file *******************/

