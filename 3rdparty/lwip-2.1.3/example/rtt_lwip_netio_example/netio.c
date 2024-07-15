/*
 * NETIO - Network Benchmark, Version 1.33
 * (C) 1997-2018 Kai Uwe Rommel
 *
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 * 
 * This is a network benchmark for Windows, Linux and Unix that
 * measures net througput of a network with TCP and UDP protocols.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */


/* netio.c
 *
 * Author:  Kai-Uwe Rommel <rommel@ars.de>
 * Created: Wed Sep 25 1996
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TCP/IP system specific details */
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <lwip/sys.h>

#include <rtthread.h>

#define DEFAULTPORT 8888 /* "IO" */
#define DEFAULTNBSRV "NETIOSRV"
#define DEFAULTNBCLT "NETIOCLT"
#define THREADSTACK 65536

#define SOCKLEN_T
#define psock_errno(x) printf(x)
#define soclose(x) lwip_close(x)

#ifdef SOCKLEN_T
typedef socklen_t socklen_type;
#else
typedef size_t socklen_type;
#endif

/* global data */

#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef EINTR
#define EINTR 0
#endif

int nSizes[] = {1024, 2048, 4096, 8192, 16384, 32768};
size_t nnSizes = sizeof(nSizes) / sizeof(int);
#define NMAXSIZE 65536

int tSizes[] = {1024, 2048, 4096, 8192, 16384, 32767};
size_t ntSizes = sizeof(tSizes) / sizeof(int);
#define TMAXSIZE 65536

#define INTERVAL 6

char data_buffer[NMAXSIZE];

/* you may need to adapt this to your platform/compiler */
typedef unsigned int uint32;

typedef struct
{
  uint32 cmd;
  uint32 data;
}
CONTROL;

#define CMD_QUIT  0
#define CMD_C2S   1
#define CMD_S2C   2
#define CMD_RES   3

#define CTLSIZE sizeof(CONTROL)

/* timer code */

int bTimeOver;

typedef struct timeval TIMER;

/* initialize data to transfer */
void GenerateRandomData(char *cBuffer, size_t nSize)
{
    if (cBuffer != NULL)
    {
        size_t i;

        cBuffer[0] = 0;
        srand(time(NULL));

        for (i = 1; i < nSize; i++)
        cBuffer[i] = (char) rand();
    }
}

char *InitBuffer(size_t nSize)
{
    //char *cBuffer = mem_malloc(nSize);
    char *cBuffer = data_buffer;
    GenerateRandomData(cBuffer, nSize);
    return cBuffer;
}

char *PacketSize(int nSize)
{
    static char szBuffer[64];

    if ((nSize % 1024) == 0 || (nSize % 1024) == 1023)
        sprintf(szBuffer, "%2dk", (nSize + 512) / 1024);
    else
        sprintf(szBuffer, "%d", nSize);

    return szBuffer;
}

/* print results */

typedef enum {nf_auto, nf_bytes, nf_kbytes, nf_mbytes, nf_gbytes} numberformat;
numberformat nFormat = nf_auto;

void print_result(long long nData, long nTime)
{
    numberformat nThisFormat = nFormat;
    int nResult = 0;

    if (nThisFormat == nf_auto)
    {
        if (nData < 10 * 1024 * INTERVAL)
        nThisFormat = nf_bytes;
        else if (nData < 10 * 1024 * 1024 * INTERVAL)
        nThisFormat = nf_kbytes;
        else if (nData < (long long) 1024 * 1024 * 1024 * INTERVAL)
        nThisFormat = nf_mbytes;
        else
        nThisFormat = nf_gbytes;
    }

    switch(nThisFormat)
    {
    case nf_auto:
        break;

    case nf_bytes:
        nResult = (int) nData / nTime;
        printf(" %d Byte/s", nResult);
        break;

    case nf_kbytes:
        nResult = (int) nData / nTime / 1024;
        printf(" %d KByte/s", nResult);
        break;

    case nf_mbytes:
        nResult = (int) nData / nTime / 1024 / 1024;
        printf(" %d MByte/s", nResult);
        break;

    case nf_gbytes:
        nResult = (int) nData / nTime / 1024 / 1024 / 1024;
        printf(" %d GByte/s", nResult);
        break;
    }
}

/* TCP/IP code */

int send_data(int socket, void *buffer, size_t size, int flags)
{
    int rc = send(socket, buffer, size, flags);

    if (rc < 0)
    {
        psock_errno("send()");
        return -1;
    }

    if (rc != size)
        return 1;

    return 0;
}

int recv_data(int socket, void *buffer, size_t size, int flags)
{
    size_t rc;
    size_t recv_total_conut = 0;
    char *recv_buffer= (char *)buffer;

    while (1)
    {
        rc = recv(socket, recv_buffer + recv_total_conut, size - recv_total_conut, flags);
        if (rc < 0)
        {
            psock_errno("recv()");
            return -1;
        }

        recv_total_conut += rc;
        if (recv_total_conut >= size)
            return 0;
    }

    return 0;
}

const int sobufsize = 131072;
int nPort = DEFAULTPORT;
int nAuxPort = DEFAULTPORT + 1;
struct in_addr addr_server;
struct in_addr addr_local;

int TCP_Server(void *arg)
{
    char *cBuffer;
    CONTROL ctl;
    long long nData;
    struct sockaddr_in sa_server, sa_client;
    int server, client;
    socklen_type length;
    struct timeval tv;
    fd_set fds;
    int rc;
    int nByte;
    int flag = 1;
    char success_flag = 1;

    u32_t start_time = 0;
    u32_t current_time = 0;

    if ((cBuffer = InitBuffer(TMAXSIZE)) == NULL)
    {
        perror("InitBuffer()");
        return 1;
    }
    printf("\r\n");

    if ((server = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        psock_errno("socket()");
        return 1;
    }

    setsockopt(server, SOL_SOCKET, SO_RCVBUF, (char *) &sobufsize, sizeof(sobufsize));
    setsockopt(server, SOL_SOCKET, SO_SNDBUF, (char *) &sobufsize, sizeof(sobufsize));
    setsockopt(server, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

    addr_local.s_addr = INADDR_ANY;

    sa_server.sin_family = AF_INET;
    sa_server.sin_port = htons(nPort);
    sa_server.sin_addr = addr_local;

    if (bind(server, (struct sockaddr *) &sa_server, sizeof(sa_server)) < 0)
    {
        psock_errno("bind()");
        soclose(server);
        return 1;
    }

    if (listen(server, 2) != 0)
    {
        psock_errno("listen()");
        soclose(server);
        return 1;
    }

    for (;;)
    {
        printf("TCP server listening.\r\n");

        FD_ZERO(&fds);
        FD_SET(server, &fds);
        tv.tv_sec  = 3600;
        tv.tv_usec = 0;

        if ((rc = select(FD_SETSIZE, &fds, 0, 0, &tv)) < 0)
        {
            psock_errno("select()\r\n");
            break;
        }

        if (rc == 0 || FD_ISSET(server, &fds) == 0)
            continue;

        length = sizeof(sa_client);
        if ((client = accept(server, (struct sockaddr *) &sa_client, &length)) == -1)
            continue;

        setsockopt(client, SOL_SOCKET, SO_RCVBUF, (char *) &sobufsize, sizeof(sobufsize));
        setsockopt(client, SOL_SOCKET, SO_SNDBUF, (char *) &sobufsize, sizeof(sobufsize));

        printf("TCP connection established ... \r\n");
        fflush(stdout);

        for (;;)
        {
            if (recv_data(client, (void *) &ctl, CTLSIZE, 0)) {
                    printf("recv_data\r\n");
                break;
            }

            ctl.cmd = ntohl(ctl.cmd);
            ctl.data = ntohl(ctl.data);

            if (ctl.cmd == CMD_C2S)
            {
                start_time = sys_now();

                printf("Receiving from client, packet size %s ... ", PacketSize(ctl.data));
                nData = 0;

                do
                {
                    for (nByte = 0; nByte < ctl.data; )
                    {
                        rc = recv(client, cBuffer + nByte, ctl.data - nByte, 0);

                        if (rc < 0 && errno != EINTR)
                        {
                            psock_errno("recv()");
                            break;
                        }

                        if (rc > 0)
                            nByte += rc;
                    }

                    nData += ctl.data;
                }
                while (cBuffer[0] == 0 && rc > 0);

                current_time = sys_now();
                print_result(nData, ((current_time-start_time) / 1000));
                printf("\r\n");

            }
            else if (ctl.cmd == CMD_S2C)
            {
                if (1)
                {
                    start_time = sys_now();

                    printf("Sending to client, packet size %s ... ", PacketSize(ctl.data));
                    cBuffer[0] = 0;
                    nData = 0;

                    while (1)
                    {
                        current_time = sys_now();
                        if ((current_time - start_time) > (INTERVAL*1000)) {
                            break;
                        }

                        for (nByte = 0; nByte < ctl.data; )
                        {
                            rc = send(client, cBuffer + nByte, ctl.data - nByte, 0);

                            if (rc < 0 && errno != EINTR)
                            {
                                psock_errno("send()\r\n");
                                success_flag = 0;
                                break;
                            }

                            if (rc > 0)
                                nByte += rc;
                        }
                        nData += ctl.data;
                        if (success_flag == 0)
                        {
                            break;
                        }
                    }

                    if (success_flag == 0)
                    {
                        break;
                    }

                    cBuffer[0] = 1;

                    if (send_data(client, cBuffer, ctl.data, 0))
                        break;

                    print_result(nData, ((current_time - start_time) / 1000));
                    printf("\r\n");
                }
            }
            else /* quit */
                break;
        }

        printf("Netio Done.\r\n");
        printf("\r\n");

        soclose(client);

        if (rc < 0)
            break;
    }

    soclose(server);

    return 0;
}

void TCP_Server_Netio_Thread()
{
    TCP_Server(NULL);
    printf("rtt_lwip_netio Done\r\n");

    /* suspend thread */
    struct rt_thread *thread = rt_thread_self();
    rt_thread_suspend(thread);
}
