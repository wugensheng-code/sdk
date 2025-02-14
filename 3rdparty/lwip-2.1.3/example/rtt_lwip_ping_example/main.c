/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_ethernetif.h"
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"

#include <rtthread.h>
#include <rtconfig.h>


int main()
{
    rt_kprintf("main start>>>\r\n");
    struct rt_thread *thread;

    /* Initialize the LwIP stack */
    tcpip_init(NULL, NULL);

    netif_config();

    /* suspend thread */
    thread = rt_thread_self();
    rt_thread_suspend(thread);

    return 0;
}
