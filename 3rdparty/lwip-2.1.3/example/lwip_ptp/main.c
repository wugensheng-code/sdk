/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ptpd.h"

#include "al_gbe_ethernetif.h"
#include "al_gbe_hal.h"

#include "lwip/init.h"
#include "lwip/timeouts.h"

extern AL_GBE_HalStruct *GbeHandle;
extern struct netif gnetif;

ptp_role default_role = PTP_ROLE_SLAVE;

int main(void)
{
    printf("lwip ptp start>>>\r\n");

    /* Initialize the LwIP stack */
    lwip_init();

    netif_config();

    /* Initialize ptpd */
    ptpd_init(default_role);

    while (1)
    {
        /* Read a received packet from the Ethernet buffers and send it
        to the lwIP for handling */
        ethernetif_input(&gnetif);

        ptpd_periodic_handle();

        /* Handle timeouts */
        sys_check_timeouts();

    }

    return 0;
}