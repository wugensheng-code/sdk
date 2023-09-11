/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_ethernetif.h"

#include "lwip/init.h"
#include "lwip/timeouts.h"

extern struct netif gnetif;

extern void netio_init(void);

extern AlCache_DisableMmu();

int main()
{
    AlCache_DisableMmu();

    printf("lwip netio start>>>\r\n");

    /* Initialize the LwIP stack */
    lwip_init();

    netif_config();

    netio_init();

    while (1)
    {
        /* Read a received packet from the Ethernet buffers and send it
        to the lwIP for handling */
        ethernetif_input(&gnetif);

        /* Handle timeouts */
        sys_check_timeouts();

    }

    return 0;
}
