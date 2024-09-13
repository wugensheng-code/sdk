/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_ethlite_hal.h"
#include "al_ethlite_ethernetif.h"
#include "lwip/tcpip.h"

#include <rtthread.h>
#include <rtconfig.h>

int main()
{
    AL_U32 RegValue;

    RegValue = AL_REG32_READ(0xF8800080);
    RegValue &= ~0x2;
    AL_REG32_WRITE(0xF8800080, RegValue);

    rt_kprintf("Ethlite RTT lwip ping example\r\n");
    struct rt_thread *thread;

    /* Initialize the LwIP stack */
    tcpip_init(NULL, NULL);

    ethlite_netif_config();

    /* suspend thread */
    thread = rt_thread_self();
    rt_thread_suspend(thread);

    return 0;
}