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

#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/sys.h"
#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"

#define BUFSZ   1024
#define LWIP_UDP_PRIORITY (2)

int started = 0;
int is_running = 0;
int port = 5000;
int remote_port = 8080;
extern struct netif gnetif;

/**
* @brief  This function is for creating a udp server on Freertos
*/

void udpserv(void)
{
    int sock, bytes_read;
    char *recv_data;
    char *send_data;
    socklen_t addr_len;
    struct sockaddr_in server_addr, client_addr;
    struct timeval timeout;
    fd_set readset;
    /* Allocate space for recv_data */
    recv_data = malloc(BUFSZ);

    if (recv_data == NULL)
    {
        printf("No memory...\r\n");
        return;
    }

    /* Create a socket and set it to SOCK_DGRAM(UDP) */
    if ((sock = lwip_socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("Create socket error...\r\n");
        goto __exit;
    }

    /* Initialize server side address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
    /* Bind socket to server side address */
    if (bind(sock, (struct sockaddr *)&server_addr,
             sizeof(struct sockaddr)) == -1)
    {
        printf("Unable to bind\r\n");
        goto __exit;
    }
    addr_len = sizeof(struct sockaddr);
    printf("UDPServer Waiting for client on port %d...\r\n", port);

    started = 1;
    is_running = 1;

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    while (is_running)
    {
        FD_ZERO(&readset);
        FD_SET(sock, &readset);

        /* Wait for read or write */
        if (select(sock + 1, &readset, NULL, NULL, &timeout) == 0)
            continue;
        /* The maximum size received from sock is BUFSZ-1 bytes*/
        bytes_read = lwip_recvfrom(sock, recv_data, BUFSZ - 1, 0,
                              (struct sockaddr *)&client_addr, &addr_len);
        // bytes_send = lwip_sendto();
        if (bytes_read < 0)
        {
            printf("Received error, close the connect.\r\n");
            goto __exit;
        }
        else if (bytes_read == 0)
        {
            printf("Received warning, recv function return 0.\r\n");
            continue;
        }
        else
        {
            recv_data[bytes_read] = '\0'; /* Append '\0' at the end of message */
            /* Output received message */
            printf("Received data = %s\r\n", recv_data);
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
        free(recv_data);
        recv_data = NULL;
    }
    if (sock >= 0)
    {
        closesocket(sock);
        sock = -1;
    }
    started = 0;
    is_running = 0;


}

