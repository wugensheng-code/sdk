#include <common.h>

#include "al_gbe_ethernetif.h"

#include "lwip/tcpip.h"

#include <stdio.h>
#include <stdlib.h>

#define ETH_INIT_THREAD_STACK_SIZE          (3072 / (sizeof( StackType_t )))

#define TCP_SERVER_NETIO_THREAD_STACK_SIZE  (3072 / (sizeof( StackType_t )))
#define TCP_SERVER_NETIO_THREAD_PRIO        (TCPIP_THREAD_PRIO + 1)

extern void idle_task(void);

extern void TCP_Server_Netio_Thread();

sys_thread_t netio_handle;

static void AlGbe_Netio_Tc(void **state) {

    TickType_t Start = xTaskGetTickCount();

    /* Initialize the LwIP stack */
    tcpip_init(NULL, NULL);

    netif_config();

    netio_handle = sys_thread_new("TCP_netio", TCP_Server_Netio_Thread, NULL,
                                  TCP_SERVER_NETIO_THREAD_STACK_SIZE, TCP_SERVER_NETIO_THREAD_PRIO);

    while ((xTaskGetTickCount() - Start) < (pdMS_TO_TICKS(300000)))
    {

    }
    
    return;
}

CMOCKA_TC_DEFINE(AlGbe_Netio_Tc, NULL, NULL, NULL);
