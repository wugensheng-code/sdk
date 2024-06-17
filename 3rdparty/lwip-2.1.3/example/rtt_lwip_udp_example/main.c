/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_ethernetif.h"
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/sockets.h"

#include <rtthread.h>
#include <rtconfig.h>

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/sys.h"
#include <string.h>
#include <stdio.h>

#define TCP_SERVER_UDP_THREAD_STACK_SIZE  (8192)

sys_thread_t udp_handle = NULL;
extern void udpserv();
int main(void){

    rt_kprintf("rtt lwip udp test start>>>\r\n");
    /* Initialize the LwIP stack */
    struct rt_thread *thread;
    tcpip_init(NULL, NULL);

    netif_config();

    udp_handle = sys_thread_new("udp test",udpserv, NULL, TCP_SERVER_UDP_THREAD_STACK_SIZE, 6);

    /* suspend thread */
    thread = rt_thread_self();
    rt_thread_suspend(thread);
    
    return 0;
}






