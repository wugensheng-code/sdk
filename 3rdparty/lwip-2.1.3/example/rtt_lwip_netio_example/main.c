#include "al_gbe_ethernetif.h"

#include <rtthread.h>
#include "lwip/tcpip.h"
#include "lwip/timeouts.h"
#include <rtconfig.h>

#include <string.h>

sys_thread_t netio_handle = NULL;

extern void TCP_Server(void *arg);
extern void TCP_Bench(void *arg);

int main()
{
    rt_kprintf("main start>>>\r\n");
    struct rt_thread *thread;

    /* Initialize the LwIP stack */
    tcpip_init(NULL, NULL);

    netif_config();

    netio_handle = sys_thread_new("netio_test", TCP_Server, NULL, 3096, 6);

//    netio_handle = sys_thread_new("netio_test", TCP_Bench, NULL, 3096, 6);

    /* suspend thread */
    thread = rt_thread_self();
    rt_thread_suspend(thread);

    return 0;
}
