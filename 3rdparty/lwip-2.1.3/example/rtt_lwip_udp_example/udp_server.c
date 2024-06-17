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

#define BUFSZ   1024

int started = 0;
int is_running = 0;
int port = 5000;
extern struct netif gnetif;

/**
* @brief  This function is for creating a udp server on RT-Thread
*/

void udpserv(void)
{
    int sock, bytes_read;
    char *recv_data;
    socklen_t addr_len;
    struct sockaddr_in server_addr, client_addr;
    struct timeval timeout;
    fd_set readset;
    /* Allocate space for recv_data */
    recv_data = rt_malloc(BUFSZ);
    if (recv_data == RT_NULL)
    {
        rt_kprintf("No memory...\r\n");
        return;
    }

    /* Create a socket and set it to SOCK_DGRAM(UDP) */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        rt_kprintf("Create socket error...\r\n");
        goto __exit;
    }

    /* Initialize server side address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
    /* Bind socket to server side address */
    if (bind(sock, (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1)
    {
        rt_kprintf("Unable to bind");
        goto __exit;
    }
    addr_len = sizeof(struct sockaddr);
    rt_kprintf("UDPServer Waiting for client on port %d...", port);

    started = 1;
    is_running = 1;

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    while (is_running)
    {
        FD_ZERO(&readset);
        FD_SET(sock, &readset);

        /* Wait for read or write */
        if (select(sock + 1, &readset, RT_NULL, RT_NULL, &timeout) == 0)
            continue;
        /* The maximum size received from sock is BUFSZ-1 bytes*/
        bytes_read = recvfrom(sock, recv_data, BUFSZ - 1, 0,
                              (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_read < 0)
        {
            rt_kprintf("Received error, close the connect.");
            goto __exit;
        }
        else if (bytes_read == 0)
        {
            rt_kprintf("Received warning, recv function return 0.");
            continue;
        }
        else
        {
            recv_data[bytes_read] = '\0'; /* Append '\0' at the end of message */
            /* Output received message */
            rt_kprintf("Received data = %s\r\n", recv_data);
            /* Send received message */
            lwip_sendto(sock, recv_data, strlen(recv_data), 0,
                        (struct sockaddr *)&client_addr, addr_len);
            /* If the message received is 'exit', quit. */
            if (strcmp(recv_data, "exit") == 0)
            {
                goto __exit;
            }
        }
    }



__exit:
    if (recv_data)
    {
        rt_free(recv_data);
        recv_data = RT_NULL;
    }
    if (sock >= 0)
    {
        closesocket(sock);
        sock = -1;
    }
    started = 0;
    is_running = 0;

}
