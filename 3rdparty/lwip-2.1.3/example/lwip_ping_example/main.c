#include "al_gbe_ethernetif.h"

#include "lwip/init.h"
#include "lwip/timeouts.h"

extern struct netif gnetif;

int main()
{
    netif_config();

    /* Initialize the LwIP stack */
    lwip_init();

    /* Infinite loop */
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
