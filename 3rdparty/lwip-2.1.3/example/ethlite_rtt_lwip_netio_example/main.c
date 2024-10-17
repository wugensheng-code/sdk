/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_ethernet_lite_hal.h"
#include "al_ethlite_ethernetif.h"
#include "lwip/tcpip.h"

#include <rtthread.h>
#include <rtconfig.h>

#define TCP_SERVER_NETIO_THREAD_STACK_SIZE  (4096)
#define TCP_SERVER_NETIO_THREAD_PRIO        6
sys_thread_t netio_handle = NULL;

extern void TCP_Server_Netio_Thread(void *arg);

int main()
{
    AL_U32 RegValue;

    RegValue = AL_REG32_READ(0xF8800080);
    RegValue &= ~0x2;
    AL_REG32_WRITE(0xF8800080, RegValue);

    rt_kprintf("Ethlite RTT lwip netio example\r\n");
    struct rt_thread *thread;

    /* Initialize the LwIP stack */
    tcpip_init(NULL, NULL);

    ethlite_netif_config();

    netio_handle = sys_thread_new("netio_test", TCP_Server_Netio_Thread, NULL, TCP_SERVER_NETIO_THREAD_STACK_SIZE, 6);

    /* suspend thread */
    thread = rt_thread_self();
    rt_thread_suspend(thread);

    return 0;
}