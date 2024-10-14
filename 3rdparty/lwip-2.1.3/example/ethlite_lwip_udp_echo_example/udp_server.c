/*
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 */

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/sys.h"
#include <string.h>
#include <stdio.h>


#ifdef UDP_SEND_TEST
#define BUFFER_SIZE 1500
char test_buffer[BUFFER_SIZE];
#endif

#define UDP_LOCAL_PORT  8888
#define UDP_CLIENT_PORT 8080

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void udp_echoserver_init(void)
{
   struct udp_pcb *upcb;
   err_t err;

#ifdef UDP_SEND_TEST
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        test_buffer[i] = 0x1;
    }
#endif

   /* Create a new UDP control block  */
   upcb = udp_new();
   
   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDP_LOCAL_PORT);
      
      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, udp_echoserver_receive_callback, NULL);
      }
      else
      {
        udp_remove(upcb);
      }
   }
}


/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    struct pbuf *p_tx;

#ifdef UDP_SEND_TEST
    err_t ret;
    struct pbuf p_tmp;
    uint32_t send_start_time = 0;
    uint32_t send_end_time = 0;
    volatile int udp_send_cnt = 0;

    int send_total_time_ms = (30*1000);

    /* allocate pbuf from RAM */
    p_tx = pbuf_alloc(PBUF_TRANSPORT, 1472, PBUF_RAM);
    memcpy(&p_tmp, p_tx, sizeof(struct pbuf));

    if (p_tx != NULL)
    {
        pbuf_take(p_tx, test_buffer, 1472);
        /* Connect to the remote client */
        udp_connect(upcb, addr, 8080);

        printf("UDP send start>>>\r\n");
        send_start_time = sys_now();

        while (1) {
            memcpy(p_tx->payload, (void *)&udp_send_cnt, sizeof(udp_send_cnt));
            ret = udp_send(upcb, p_tx);
            if (ret != ERR_OK)
            {
                printf("fail%d\r\n", ret);
            }
            memcpy(p_tx, &p_tmp, sizeof(struct pbuf));

            udp_send_cnt++;

            send_end_time = sys_now();
            if ((send_end_time - send_start_time) > send_total_time_ms)
            {
                break;
            }

        }

        printf("Send total time :%ds\r\n", (send_total_time_ms /1000));
        printf("Send total packet:%d\r\n", udp_send_cnt);
        printf("UDP send done<<<\r\n");

        /* free the UDP connection, so we can accept new clients */
        udp_disconnect(upcb);

        /* Free the p_tx buffer */
        pbuf_free(p_tx);

        /* Free the p buffer */
        pbuf_free(p);
    }

#else

  /* allocate pbuf from RAM*/
  p_tx = pbuf_alloc(PBUF_TRANSPORT,p->len, PBUF_RAM);

  if(p_tx != NULL)
  {
    pbuf_take(p_tx, (char*)p->payload, p->len);
    /* Connect to the remote client */
    udp_connect(upcb, addr, UDP_CLIENT_PORT);
    
    /* Tell the client that we have accepted it */
    udp_send(upcb, p_tx);

    /* free the UDP connection, so we can accept new clients */
    udp_disconnect(upcb);
    
    /* Free the p_tx buffer */
    pbuf_free(p_tx);

    /* Free the p buffer */
    pbuf_free(p);
  }

#endif
}


