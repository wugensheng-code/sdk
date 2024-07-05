/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_ethernetif.h"

#include <rtthread.h>
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"
#include <rtconfig.h>

#include <string.h>

#define TCP_SERVER_NETIO_THREAD_STACK_SIZE  (4096)
#define TCP_SERVER_NETIO_THREAD_PRIO        6
sys_thread_t netio_handle = NULL;

extern void TCP_Server_Netio_Thread(void *arg);

int main()
{
    rt_kprintf("rtt_lwip_netio start>>>\r\n");
    struct rt_thread *thread;

    /* Initialize the LwIP stack */
    tcpip_init(NULL, NULL);

    netif_config();

    netio_handle = sys_thread_new("netio_test", TCP_Server_Netio_Thread, NULL, TCP_SERVER_NETIO_THREAD_STACK_SIZE, 6);

    /* suspend thread */
    thread = rt_thread_self();
    rt_thread_suspend(thread);

    return 0;
}
