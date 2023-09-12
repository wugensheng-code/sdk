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

#ifdef WIN32
#define _INTEGRAL_MAX_BITS 64
#endif

#define UNIX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
#if defined(UNIX) || defined(DJGPP)
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#else
#include <process.h>
#include "getopt.h"
#endif

#include <rtthread.h>

#define DEFAULTPORT 8888 /* "IO" */
#define DEFAULTNBSRV "NETIOSRV"
#define DEFAULTNBCLT "NETIOCLT"
#define THREADSTACK 65536

/* TCP/IP system specific details */

#ifdef UNIX

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
//#include <netinet/in.h>
//#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>


#define SOCKLEN_T
#define psock_errno(x) perror(x)
#define soclose(x) close(x)

int sock_init(void)
{
  return 0;
}

#include <pthread.h>
pthread_t thread;
#define newthread(entry) (pthread_create(&thread, 0, entry, 0) != 0)
#define THREAD void*
#define THREADRESULT ((void*)0)

#endif /* UNIX */

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

char *data_buffer[NMAXSIZE];

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


#if defined(UNIX) || defined(DJGPP)

typedef struct timeval TIMER;

void on_alarm(int signum)
{
//  alarm(0);
//  bTimeOver = 1;
}

int StartAlarm(long nSeconds)
{
//  bTimeOver = 0;
//  signal(SIGALRM, on_alarm);
//  alarm(nSeconds);
  return 0;
}

int StartTimer(TIMER *nStart)
{
  struct timezone tz = {0, 0};

  gettimeofday(nStart, &tz);

  return 0;
}

long StopTimer(TIMER *nStart, int nAccuracy)
{
  struct timezone tz = {0, 0};
  TIMER nStop;

  gettimeofday(&nStop, &tz);

  return (nStop.tv_sec - nStart->tv_sec) * nAccuracy
       + (nStop.tv_usec - nStart->tv_usec) * nAccuracy / 1000000;
}

#endif /* UNIX || DJGPP */

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
  case nf_bytes:
    nResult = (int) nData * 1024 / nTime;
    printf(" %d Byte/s", nResult);
    break;

  case nf_kbytes:
    nResult = (int) nData / nTime;
    printf(" %d KByte/s", nResult);
    break;

  case nf_mbytes:
    nResult = (int) nData / nTime / 1024;
    printf(" %d MByte/s", nResult);
    break;

  case nf_gbytes:
    nResult = (int) nData / nTime / 1024 / 1024;
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
  size_t rc = recv(socket, buffer, size, flags);

  if (rc < 0)
  {
    psock_errno("recv()");
    return -1;
  }

  if (rc != size)
    return 1;

  return 0;
}

const int sobufsize = 131072;
int nPort = DEFAULTPORT;
int nAuxPort = DEFAULTPORT + 1;
struct in_addr addr_server;
struct in_addr addr_local;

int udpsocket, udpd;
unsigned long nUDPCount;
long long nUDPData;

THREAD TCP_Server(void *arg)
{
  char *cBuffer;
  CONTROL ctl;
  TIMER nTimer;
  long nTime;
  long long nData;
  struct sockaddr_in sa_server, sa_client;
  int server, client;
  socklen_type length;
  struct timeval tv;
  fd_set fds;
  int rc;
  int nByte;
  int flag = 1;

  rt_tick_t tick1 = 0;
  rt_tick_t tick2 = 0;

  if ((cBuffer = InitBuffer(TMAXSIZE)) == NULL)
  {
    perror("malloc()");
    return THREADRESULT;
  }

server_loop:

  rt_thread_mdelay(500);

  if ((server = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    psock_errno("socket()");
//    free(cBuffer);
    return THREADRESULT;
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
//    free(cBuffer);
    return THREADRESULT;
  }

  if (listen(server, 2) != 0)
  {
    psock_errno("listen()");
    soclose(server);
//    free(cBuffer);
    return THREADRESULT;
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
      if (recv_data(client, (void *) &ctl, CTLSIZE, 0))
        break;

      ctl.cmd = ntohl(ctl.cmd);
      ctl.data = ntohl(ctl.data);

      if (ctl.cmd == CMD_C2S)
      {
        tick1 = rt_tick_get();

        printf("\nReceiving from client, packet size %s ... ", PacketSize(ctl.data));
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

//        if ((nTime = StopTimer(&nTimer, 1024)) != -1)
//          print_result(nData, nTime);

        tick2 = rt_tick_get();
        print_result(nData, ((tick2-tick1) / 1000));
        printf("\r\n");
      }
      else if (ctl.cmd == CMD_S2C)
      {
        if (1)
        {
          tick1 = rt_tick_get();

        printf("\nSending to client, packet size %s ... ", PacketSize(ctl.data));
        cBuffer[0] = 0;
        nData = 0;

        while (1)
        {
            tick2 = rt_tick_get();
            if ((tick2 -tick1) > INTERVAL*1000)
                break;
            //GenerateRandomData(cBuffer, ctl.data);

            for (nByte = 0; nByte < ctl.data; )
            {
            rc = send(client, cBuffer + nByte, ctl.data - nByte, 0);

            if (rc < 0 && errno != EINTR)
            {
            psock_errno("send()\r\n");
            break;
            }

            if (rc > 0)
            nByte += rc;
            }

            nData += ctl.data;
        }

        cBuffer[0] = 1;

        if (send_data(client, cBuffer, ctl.data, 0))
            break;

//        if ((nTime = StopTimer(&nTimer, 1024)) != -1)
//            print_result(nData, nTime);

          print_result(nData, ((tick2-tick1) / 1000));
          printf("\r\n");
        }
      }
      else /* quit */
	    break;
    }

    printf("\nDone.\r\n");

    soclose(client);

    if (rc < 0)
      break;
  }

  soclose(server);

  goto server_loop;

//    free(cBuffer);

  while(1);

  return THREADRESULT;
}

void TCP_Bench(void *arg)
{
  char *cBuffer;
  CONTROL ctl;
  TIMER nTimer;
  long nTime;
  long long nData;
  int i;
  struct sockaddr_in sa_server, sa_client;
  int server;
  int rc;
  int nByte;
  int flag = 1;

  rt_tick_t tick1 = 0;
  rt_tick_t tick2 = 0;

  if ((cBuffer = InitBuffer(TMAXSIZE)) == NULL)
  {
    perror("malloc()");
    return;
  }

client_loop:

  rt_thread_mdelay(500);

  if ((server = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    psock_errno("socket()");
//    free(cBuffer);
    return;
  }

  setsockopt(server, SOL_SOCKET, SO_RCVBUF, (char *) &sobufsize, sizeof(sobufsize));
  setsockopt(server, SOL_SOCKET, SO_SNDBUF, (char *) &sobufsize, sizeof(sobufsize));
  setsockopt(server, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

  addr_local.s_addr = INADDR_ANY;

  sa_client.sin_family = AF_INET;
  sa_client.sin_port = htons(0);
  sa_client.sin_addr = addr_local;

  if (bind(server, (struct sockaddr *) &sa_client, sizeof(sa_client)) < 0)
  {
    psock_errno("bind()");
    soclose(server);
//    free(cBuffer);
    return;
  }

  addr_server.s_addr = inet_addr("10.8.9.2");

  sa_server.sin_family = AF_INET;
  sa_server.sin_port = htons(nPort);
  sa_server.sin_addr = addr_server;

  if (connect(server, (struct sockaddr *) &sa_server, sizeof(sa_server)) < 0)
  {
    psock_errno("connect()");
    soclose(server);
//    free(cBuffer);
    return;
  }

  printf("\r\nTCP connection established.\r\n");

  for (i = 0; i < ntSizes; i++)
  {
    printf("Packet size %s bytes: ", PacketSize(tSizes[i]));

    /* tell the server we will send it data now */

    ctl.cmd = htonl(CMD_C2S);
    ctl.data = htonl(tSizes[i]);

    if (send_data(server, (void *) &ctl, CTLSIZE, 0))
      break;

    /* 1 - Tx test */

    if (1)
    {
//      StartTimer(&nTimer);
      tick1 = rt_tick_get();
      tick2 = rt_tick_get();
      nData = 0;
      cBuffer[0] = 0;

      while ((tick2 - tick1) < (INTERVAL * 1000))
      {

        tick2 = rt_tick_get();

        for (nByte = 0; nByte < tSizes[i]; )
        {
        rc = send(server, cBuffer + nByte, tSizes[i] - nByte, 0);

        if (rc < 0 && errno != EINTR)
        {
            psock_errno("send()");
            break;
        }

        if (rc > 0)
            nByte += rc;
        }

        nData += tSizes[i];
      }

//      if ((nTime = StopTimer(&nTimer, 1024)) == -1)
//        printf(" (failed)\r\n");
//      else
      print_result(nData, ((tick2 - tick1)/1000));

      printf(" Tx, ");

      cBuffer[0] = 1;

      if (send_data(server, cBuffer, tSizes[i], 0))
        break;
    }

    /* tell the server we expect him to send us data now */

    ctl.cmd = htonl(CMD_S2C);
    ctl.data = htonl(tSizes[i]);

    if (send_data(server, (void *) &ctl, CTLSIZE, 0))
      break;

    /* 2 - Rx test */

    tick1 = rt_tick_get();
    nData = 0;

    do
    {
      for (nByte = 0; nByte < tSizes[i]; )
      {
	rc = recv(server, cBuffer + nByte, tSizes[i] - nByte, 0);

	if (rc < 0 && errno != EINTR)
	{
	  psock_errno("recv()");
	  break;
	}
	  
	if (rc > 0)
	  nByte += rc;
      }

      nData += tSizes[i];
    }
    while (cBuffer[0] == 0 && rc > 0);

//    if ((nTime = StopTimer(&nTimer, 1024)) == -1)
//      printf(" (failed)");
//    else

      tick2 = rt_tick_get();

      print_result(nData, (tick2 - tick1)/1000);

    printf(" Rx.\r\n");
  }

  ctl.cmd = htonl(CMD_QUIT);
  ctl.data = 0;

  send_data(server, (void *) &ctl, CTLSIZE, 0);

  printf("Done.\r\n");

  soclose(server);

  goto client_loop;

//  free(cBuffer);

    while(1);
}

THREAD UDP_Receiver(void *arg)
{
  char *cBuffer;
  struct sockaddr_in sa_server, sa_client;
  int rc;
  socklen_type nBytes;

  if ((cBuffer = InitBuffer(TMAXSIZE)) == NULL)
  {
    perror("malloc()");
    return THREADRESULT;
  }

  if ((udpsocket = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    psock_errno("socket(DGRAM)");
    free(cBuffer);
    return THREADRESULT;
  }
  
  setsockopt(udpsocket, SOL_SOCKET, SO_RCVBUF, (char *) &sobufsize, sizeof(sobufsize));
  setsockopt(udpsocket, SOL_SOCKET, SO_SNDBUF, (char *) &sobufsize, sizeof(sobufsize));

  sa_server.sin_family = AF_INET;
  sa_server.sin_port = htons(nAuxPort);
  sa_server.sin_addr = addr_local;

  if (bind(udpsocket, (struct sockaddr *) &sa_server, sizeof(sa_server)) < 0)
  {
    psock_errno("bind(DGRAM)");
    soclose(udpsocket);
    free(cBuffer);
    return THREADRESULT;
  }

  udpd = 1;

  for (;;)
  {
    nBytes = sizeof(sa_client);
    rc = recvfrom(udpsocket, cBuffer, TMAXSIZE, 0, (struct sockaddr *) &sa_client, &nBytes);

    if (rc < 0 && errno != EINTR)
      psock_errno("recvfrom()");

    if (rc > 0)
    {
      nUDPCount++;
      nUDPData += rc;
    }
  }

  soclose(udpsocket);
  free(cBuffer);

  return THREADRESULT;
}

THREAD UDP_Server(void *arg)
{
  char *cBuffer;
  CONTROL ctl;
  TIMER nTimer;
  long nTime;
  long long nData;
  struct sockaddr_in sa_server, sa_client;
  int server, client;
  struct timeval tv;
  fd_set fds;
  int rc, nByte;
  socklen_type nLength;

  if ((cBuffer = InitBuffer(TMAXSIZE)) == NULL)
  {
    perror("malloc()");
    return THREADRESULT;
  }

  if ((server = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    psock_errno("socket(STREAM)");
    free(cBuffer);
    return THREADRESULT;
  }

  setsockopt(server, SOL_SOCKET, SO_RCVBUF, (char *) &sobufsize, sizeof(sobufsize));
  setsockopt(server, SOL_SOCKET, SO_SNDBUF, (char *) &sobufsize, sizeof(sobufsize));

  sa_server.sin_family = AF_INET;
  sa_server.sin_port = htons(nAuxPort);
  sa_server.sin_addr = addr_local;

  if (bind(server, (struct sockaddr *) &sa_server, sizeof(sa_server)) < 0)
  {
    psock_errno("bind(STREAM)");
    soclose(server);
    free(cBuffer);
    return THREADRESULT;
  }

  if (listen(server, 2) != 0)
  {
    psock_errno("listen()");
    soclose(server);
    free(cBuffer);
    return THREADRESULT;
  }

  for (;;)
  {
    printf("UDP server listening.\n");

    FD_ZERO(&fds);
    FD_SET(server, &fds);
    tv.tv_sec  = 3600;
    tv.tv_usec = 0;

    if ((rc = select(FD_SETSIZE, &fds, 0, 0, &tv)) < 0)
    {
      psock_errno("select()");
      break;
    }

    if (rc == 0 || FD_ISSET(server, &fds) == 0)
      continue;

    nLength = sizeof(sa_client);
    if ((client = accept(server, (struct sockaddr *) &sa_client, &nLength)) == -1)
      continue;

    setsockopt(client, SOL_SOCKET, SO_RCVBUF, (char *) &sobufsize, sizeof(sobufsize));
    setsockopt(client, SOL_SOCKET, SO_SNDBUF, (char *) &sobufsize, sizeof(sobufsize));

    printf("UDP connection established ... ");
    fflush(stdout);

    sa_client.sin_port = htons(nAuxPort);

    for (;;)
    {
      if (recv_data(client, (void *) &ctl, CTLSIZE, 0))
	break;

      ctl.cmd = ntohl(ctl.cmd);
      ctl.data = ntohl(ctl.data);

      if (ctl.cmd == CMD_C2S)
      {
	StartTimer(&nTimer);
	nUDPCount = 0;
	nUDPData = 0;

	printf("\nReceiving from client, packet size %s ... ", PacketSize(ctl.data));

	ctl.cmd = htonl(ctl.cmd);
	ctl.data = htonl(ctl.data);

	if (send_data(client, (void *) &ctl, CTLSIZE, 0))
	  break;
      }
      else if (ctl.cmd == CMD_RES)
      {
	ctl.cmd = htonl(ctl.cmd);
	ctl.data = htonl(nUDPCount);

	if (send_data(client, (void *) &ctl, CTLSIZE, 0))
	  break;

	if ((nTime = StopTimer(&nTimer, 1024)) != -1)
	  print_result(nUDPData, nTime);
      }
      else if (ctl.cmd == CMD_S2C)
      {
	if (StartAlarm(INTERVAL) == 0)
	{
	  StartTimer(&nTimer);
	  nData = 0;

	  printf("\nSending to client, packet size %s ... ", PacketSize(ctl.data));
	  cBuffer[0] = 0;
	  nLength = ctl.data;

	  ctl.cmd = htonl(CMD_RES);
	  ctl.data = 0;

	  while (!bTimeOver)
	  {
	    //GenerateRandomData(cBuffer, nLength);

	    for (nByte = 0; nByte < nLength; )
	    {
	      do
	      {
	        rc = sendto(udpsocket, cBuffer + nByte, nLength - nByte, 0, 
			    (struct sockaddr *) &sa_client, sizeof(sa_client));
	      }
#ifdef ENOBUFS
	      while (rc < 0 && errno == ENOBUFS);
#else
	      while (0);
#endif

	      if (rc < 0 && errno != EINTR)
	      {
		psock_errno("sendto()");
		break;
	      }
	      
	      if (rc > 0)
		nByte += rc;
	    }

	    ctl.data++;
	    nData += nLength;
	  }

	  ctl.data = htonl(ctl.data);

	  if (send_data(client, (void *) &ctl, CTLSIZE, 0))
	    break;

	  if ((nTime = StopTimer(&nTimer, 1024)) != -1)
	    print_result(nData, nTime);
	}
      }
      else /* quit */
	break;
    }

    printf("\nDone.\n");

    soclose(client);

    if (rc < 0)
      break;
  }

  soclose(server);
  free(cBuffer);

  return THREADRESULT;
}

void UDP_Bench(void *arg)
{
  char *cBuffer;
  CONTROL ctl;
  TIMER nTimer;
  long nTime, nCount;
  long nResult;
  long long nData;
  int i;
  struct sockaddr_in sa_server, sa_client;
  int server;
  int rc, nByte;

  if ((cBuffer = InitBuffer(TMAXSIZE)) == NULL)
  {
    perror("malloc()");
    return;
  }

  if ((server = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    psock_errno("socket()");
    free(cBuffer);
    return;
  }

  setsockopt(server, SOL_SOCKET, SO_RCVBUF, (char *) &sobufsize, sizeof(sobufsize));
  setsockopt(server, SOL_SOCKET, SO_SNDBUF, (char *) &sobufsize, sizeof(sobufsize));

  sa_client.sin_family = AF_INET;
  sa_client.sin_port = htons(0);
  sa_client.sin_addr = addr_local;

  if (bind(server, (struct sockaddr *) &sa_client, sizeof(sa_client)) < 0)
  {
    psock_errno("bind(STREAM)");
    soclose(server);
    free(cBuffer);
    return;
  }

  sa_server.sin_family = AF_INET;
  sa_server.sin_port = htons(nAuxPort);
  sa_server.sin_addr = addr_server;

  if (connect(server, (struct sockaddr *) &sa_server, sizeof(sa_server)) < 0)
  {
    psock_errno("connect()");
    soclose(server);
    free(cBuffer);
    return;
  }

  printf("\nUDP connection established.\n");

  for (i = 0; i < ntSizes; i++)
  {
    printf("Packet size %s bytes: ", PacketSize(tSizes[i]));
    fflush(stdout);

    /* tell the server we will send it data now */

    ctl.cmd = htonl(CMD_C2S);
    ctl.data = htonl(tSizes[i]);

    if (send_data(server, (void *) &ctl, CTLSIZE, 0))
      break;

    if (recv_data(server, (void *) &ctl, CTLSIZE, 0))
      break;

    /* 1 - Tx test */

    if (StartAlarm(INTERVAL) == 0)
    {
      StartTimer(&nTimer);
      nData = 0;
      nCount = 0;
      cBuffer[0] = 0;

      while (!bTimeOver)
      {
	//GenerateRandomData(cBuffer, tSizes[i]);

	for (nByte = 0; nByte < tSizes[i]; )
	{
	  rc = sendto(udpsocket, cBuffer + nByte, tSizes[i] - nByte, 0, 
		      (struct sockaddr *) &sa_server, sizeof(sa_server));

	  if (rc < 0)
	  {
	    if (errno != EINTR)
	    {
	      psock_errno("sendto()");
	      break;
	    }
	  }
	  else
	    nByte += rc;
	}

	nData += tSizes[i];
	nCount++;
      }
      
      if ((nTime = StopTimer(&nTimer, 1024)) == -1)
	printf(" (failed)");

      ctl.cmd = htonl(CMD_RES);

      if (send_data(server, (void *) &ctl, CTLSIZE, 0))
	break;

      if (recv_data(server, (void *) &ctl, CTLSIZE, 0))
	break;

      ctl.data = ntohl(ctl.data);
      nData = (long long) tSizes[i] * ctl.data;

      print_result(nData, nTime);
      nResult = (nCount - ctl.data) * 100 / nCount;
      printf(" (%ld%%) Tx, ", nResult);
      fflush(stdout);
    }

    /* tell the server we expect him to send us data now */

    ctl.cmd = htonl(CMD_S2C);
    ctl.data = htonl(tSizes[i]);

    if (send_data(server, (void *) &ctl, CTLSIZE, 0))
      break;

    /* 2 - Rx test */

    StartTimer(&nTimer);
    nUDPCount = 0;
    nUDPData = 0;

    if (recv_data(server, (void *) &ctl, CTLSIZE, 0))
      break;

    if ((nTime = StopTimer(&nTimer, 1024)) == -1)
      printf(" (failed)");

    ctl.data = ntohl(ctl.data);

    print_result(nUDPData, nTime);
    nResult = (ctl.data - nUDPCount) * 100 / ctl.data;
    printf(" (%ld%%) Rx.\n", nResult);
  }

  ctl.cmd = htonl(CMD_QUIT);
  ctl.data = 0;

  send_data(server, (void *) &ctl, CTLSIZE, 0);

  printf("Done.\n");

  soclose(server);
  free(cBuffer);
}

/* main / user interface */

int bSRV, bTCP, bUDP;

void handler(int sig)
{
  _exit(0);
}

void usage(void)
{
  printf(
	 "\nUsage: netio [options] [<server>]\n"
	 "\n  -s              run server side of benchmark (otherwise run client)"
	 "\n  -b <size>[k]    use this block size (otherwise run with 1,2,4,8,16 and 32k)"
	 "\n  -B -K -M -G     force number formatting to Bytes, K, M or G Bytes\n"

	 "\n  -t              use TCP protocol for benchmark"
	 "\n  -u              use UDP protocol for benchmark"
	 "\n  -h <addr/name>  bind TCP and UDP sockets to this local host address/name"
	 "\n                  defaults to all (server) or unspecified (client)"
	 "\n  -p <port>       bind TCP and UDP servers to this port (default is %d)\n"

         "\n  <server>        If the client side of the benchmark is running,"
	 "\n                  a server name or address is required.\n"

	 "\nThe server side can run either TCP (-t) or UDP (-u) protocol or both"
	 "\n(default, if neither -t or -u is specified). The client runs one of"
	 "\nthese protocols only (must specify -t or -u).\n"
	 "\n", nPort);
  exit(1);
}

#if 0
int main(int argc, char **argv)
{
  char szVersion[32], *szName = 0, *szLocal = 0, *szEnd;
  int option;
  struct hostent *host;
  long nSize;

  strcpy(szVersion, rcsrev + sizeof("$Revision: ") - 1);
  *strchr(szVersion, ' ') = 0;

  printf("\nNETIO - Network Throughput Benchmark, Version %s"
	 "\n(C) 1997-2018 Kai Uwe Rommel\n", szVersion);

  if (argc == 1)
    usage();

  /* check arguments */

  while ((option = getopt(argc, argv, "?stup:h:b:dBKMG")) !=  -1)
    switch (option)
    {
    case 's':
      bSRV = 1;
      break;
    case 't':
      bTCP = 1;
      break;
    case 'u':
      bUDP = 1;
      break;
    case 'p':
      nPort = atoi(optarg);
      nAuxPort = nPort + 1;
      break;
    case 'h':
      szLocal = optarg;
      break;
    case 'b':
      nSize = strtol(optarg, &szEnd, 10);
      if (*szEnd == 'k')
	nSize *= 1024;
      nSizes[0] = min(max(nSize, 1), NMAXSIZE);
      tSizes[0] = min(max(nSize, 1), TMAXSIZE);
      nnSizes = ntSizes = 1;
      break;
#ifdef WATT32
    case 'd':
      dbug_init();
      break;
#endif
    case 'B':
      nFormat = nf_bytes;
      break;
    case 'K':
      nFormat = nf_kbytes;
      break;
    case 'M':
      nFormat = nf_mbytes;
      break;
    case 'G':
      nFormat = nf_gbytes;
      break;
    default:
      usage();
      break;
    }

  if (bSRV == 1 && bTCP == 0 && bUDP == 0)
    bTCP = bUDP = 1;

  /* initialize TCP/IP */

  if (bTCP || bUDP)
  {
    if (sock_init())
      return psock_errno("sock_init()"), 1;

    if (szLocal == 0)
      addr_local.s_addr = INADDR_ANY; 
    else
    {
      if (isdigit(*szLocal))
	addr_local.s_addr = inet_addr(szLocal);
      else
      {
	if ((host = gethostbyname(szLocal)) == NULL)
	  return psock_errno("gethostbyname()"), 1;

	addr_local = * (struct in_addr *) (host->h_addr);
      }
    }

    if (!bSRV)
    {
      if (optind == argc)
	usage();

      if (isdigit(*argv[optind]))
	addr_server.s_addr = inet_addr(argv[optind]);
      else
      {
	if ((host = gethostbyname(argv[optind])) == NULL)
	  return psock_errno("gethostbyname()"), 1;

	addr_server = * (struct in_addr *) (host->h_addr);
      }
    }
  }

  /* do work */

  signal(SIGINT, handler);

  if (bSRV)
  {
    printf("\n");

    if (bTCP)
    {
      if (newthread(TCP_Server))
	return printf("Cannot create additional thread.\n"), 2;
    }
    if (bUDP)
    {
      if (newthread(UDP_Receiver))
	return printf("Cannot create additional thread.\n"), 2;
      if (newthread(UDP_Server))
	return printf("Cannot create additional thread.\n"), 2;
    }

    for (;;) sleep(86400);
  }
  else
  {
    if (bTCP + bUDP > 1) /* exactly one only */
      usage();

    if (bTCP)
      TCP_Bench(0);
    else if (bUDP)
    {
      if (newthread(UDP_Receiver))
	return printf("Cannot create additional thread.\n"), 2;
      while (udpd == 0)	sleep(1);
      UDP_Bench(0);
    }
  }

  /* terminate */

  printf("\n");

  return 0;
}

#endif

/* end of netio.c */
