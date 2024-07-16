/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <lwip/sys.h>

#define BUFSZ   2048

char recv_data[BUFSZ];

int port = 5000;
int remote_port = 8080;

/**
* @brief  This function is for creating a udp server on Freertos
*/

void udpserv(void)
{
    int sock, bytes_read;
    socklen_t addr_len;
    struct sockaddr_in server_addr, client_addr;
    struct timeval timeout;
    fd_set readset;

udp_server:

    /* Create a socket and set it to SOCK_DGRAM(UDP) */
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
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

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    while (1)
    {
        FD_ZERO(&readset);
        FD_SET(sock, &readset);

        /* Wait for read or write */
        if (select(sock + 1, &readset, NULL, NULL, &timeout) == 0)
            continue;
        /* The maximum size received from sock is BUFSZ-1 bytes*/
        bytes_read = recvfrom(sock, recv_data, BUFSZ - 1, 0,
                              (struct sockaddr *)&client_addr, &addr_len);
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
            /* Send received message */
            printf("Server received %d bytes packet, sendto client...\r\n", bytes_read);
            sendto(sock, recv_data, bytes_read, 0,
                        (struct sockaddr *)&client_addr, addr_len);
        }
    }

__exit:

    if (sock >= 0)
    {
        closesocket(sock);
        sock = -1;
    }

    goto udp_server;

    /* It won't run here */
    while(1);
}