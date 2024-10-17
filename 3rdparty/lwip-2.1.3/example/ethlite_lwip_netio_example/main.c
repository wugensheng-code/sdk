/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_ethernet_lite_hal.h"
#include "al_ethlite_ethernetif.h"

#include "lwip/init.h"
#include "lwip/timeouts.h"

extern struct netif ethlite_netif;

extern void netio_init(void);

int main()
{
    AL_U32 RegValue;

    RegValue = AL_REG32_READ(0xF8800080);
    RegValue &= ~0x2;
    AL_REG32_WRITE(0xF8800080, RegValue);

    printf("EthLite Lwip netio example\r\n");

    /* Initialize the LwIP stack */
    lwip_init();

    ethlite_netif_config();

    netio_init();

    while (1)
    {
        /* Read a received packet from the Ethernet buffers and send it
        to the lwIP for handling */
        ethlite_ethernetif_input(&ethlite_netif);

        /* Handle timeouts */
        sys_check_timeouts();

    }

    return 0;
}
