/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
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
 * Author: Adam Dunkels <adam@sics.se>
 ******************************************************************************
 */

/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__


#ifdef USE_RTOS
#define NO_SYS                  0
#else
#define NO_SYS                  1
#endif

#if NO_SYS

#define LWIP_PTP                1

/**
 * SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
 * critical regions during buffer allocation, deallocation and memory
 * allocation and deallocation.
 */
#define SYS_LIGHTWEIGHT_PROT    0


/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN            0


/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
/**
 * LWIP_SOCKET==1: Enable Socket API (require to use sockets.c), OS need
 */
#define LWIP_SOCKET             0

#else

/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
/**
 * LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
 */
#define LWIP_NETCONN                    1


/*
   ---------------------------------
   ---------- OS options ----------
   ---------------------------------
*/

/* TCPIP thread options */
#define TCPIP_THREAD_NAME              "TCP/IP"
#define TCPIP_MBOX_SIZE                 #{{tcpip_mbox_size}}
#define DEFAULT_UDP_RECVMBOX_SIZE       #{{default_udp_recvmbox_size}}
#define DEFAULT_TCP_RECVMBOX_SIZE       #{{default_tcp_recvmbox_size}}
#define DEFAULT_ACCEPTMBOX_SIZE         6

#if defined(RTOS_RTTHREAD)

#define TCPIP_THREAD_PRIO               7
#define TCPIP_THREAD_STACKSIZE          3096
#define DEFAULT_THREAD_STACKSIZE        2048

#define ETH_INPUT_THREAD_STACKSIZE      2048
#define ETH_INPUT_THREAD_PRIO           8

#elif defined(RTOS_FREERTOS)

#define TCPIP_THREAD_PRIO               8

/* The length of this StackType_t is 8 */
#define TCPIP_THREAD_STACKSIZE          (3096/8)
#define DEFAULT_THREAD_STACKSIZE        (2048/8)

#define ETH_INPUT_THREAD_PRIO           7

/* The length of this StackType_t is 8 */
#define ETH_INPUT_THREAD_STACKSIZE      (2048/8)

#endif

/* Ethernetif input thread options */
#define ETH_INPUT_THREAD_NAME           "ETH_INPUT"

#endif /* NO_SYS */

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
   lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
   byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT               64

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE                    #{{mem_size}}

/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
   connections. */
#define MEMP_NUM_TCP_PCB            #{{memp_num_tcp_pcb}}

/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
   segments. */
#define MEMP_NUM_TCP_SEG            #{{memp_num_tcp_seg}}

#define MEMP_NUM_PBUF               #{{memp_num_pbuf}}

#define MEMP_NUM_TCP_PCB_LISTEN     #{{memp_num_tcp_pcb_listen}}

#define MEMP_NUM_UDP_PCB            #{{memp_num_udp_pcb}}

#define MEMP_NUM_API_MSG            #{{memp_num_api_msg}}

#define MEMP_NUM_NETBUF             #{{memp_num_netbuf}}

#define MEMP_NUM_NETCONN            #{{memp_num_netconn}}

#define MEMP_NUM_TCPIP_MSG_API      #{{memp_num_tcpip_msg_api}}

#define MEMP_NUM_SYS_TIMEOUT        #{{memp_num_sys_timeout}}

#define MEMP_NUM_TCPIP_MSG_INPKT    64


/* ---------- Pbuf options ---------- */

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool */
#define PBUF_POOL_BUFSIZE           #{{pbuf_pool_bufsize}}
#define PBUF_POOL_SIZE              #{{pbuf_pool_size}}
#define PBUF_LINK_HLEN              #{{pbuf_link_hlen}}


/* LWIP_SUPPORT_CUSTOM_PBUF == 1: to pass directly MAC Rx buffers to the stack 
   no copy is needed */
#define LWIP_SUPPORT_CUSTOM_PBUF    1

/*
   ------------------------------------------------
   ---------- Network Interfaces options ----------
   ------------------------------------------------
*/
#define LWIP_NETIF_LINK_CALLBACK    1


/* ---------- TCP options ---------- */
#define LWIP_TCP                    #{{lwip_tcp}}
#define TCP_TTL                     #{{tcp_ttl}}

#define TCP_SND_QUEUELEN            (48)
#define TCP_QUEUE_OOSEQ             #{{tcp_queue_ooseq}}
#define TCP_SYNMAXRTX               #{{tcp_synmaxrtx}}
#define TCP_MAXRTX                  #{{tcp_maxrtx}}

/* TCP Maximum segment size. */
#define TCP_MSS                     #{{tcp_mss}} /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF                 #{{tcp_snd_buf}}

/* TCP receive window. */
#define TCP_WND                     #{{tcp_wnd}}


/* ---------- UDP options ---------- */
#define LWIP_UDP                    #{{lwip_udp}}
#define UDP_TTL                     #{{udp_ttl}}


/* ---------- ICMP options ---------- */
#define LWIP_ICMP                   1
#define ICMP_TTL                    #{{icmp_ttl}}


/* ---------- IGMP options ---------- */
#if LWIP_PTP
#define LWIP_IGMP                   1
#else
#define LWIP_IGMP                   #{{lwip_igmp}}
#endif


/*
   --------------------------------
   ---------- IP options ----------
   --------------------------------
*/
#define IP_DEFAULT_TTL              #{{ip_default_ttl}}
#define IP_FORWARD                  #{{ip_forward}}
#define IP_FRAG                     #{{ip_frag}}
#define IP_OPTIONS_ALLOWED          #{{ip_options_allowed}}
#define IP_REASS_MAX_PBUFS          #{{ip_reass_max_pbufs}}
#define IP_REASSEMBLY               #{{ip_reassembly}}


/*
   ---------------------------------
   ---------- ARP options ----------
   ---------------------------------
*/
#define ARP_QUEUEING                #{{arp_queueing}}
#define ARP_TABLE_SIZE              #{{arp_table_size}}


/* ---------- Statistics options ---------- */
#define LWIP_STATS                  #{{lwip_stats}}


/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/

//#define CHECKSUM_BY_HARDWARE

#ifdef CHECKSUM_BY_HARDWARE
  /* CHECKSUM_GEN_IP==0: Generate checksums by hardware for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 0
  /* CHECKSUM_GEN_UDP==0: Generate checksums by hardware for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                0
  /* CHECKSUM_GEN_TCP==0: Generate checksums by hardware for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                0
  /* CHECKSUM_CHECK_IP==0: Check checksums by hardware for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               0
  /* CHECKSUM_CHECK_UDP==0: Check checksums by hardware for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              0
  /* CHECKSUM_CHECK_TCP==0: Check checksums by hardware for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              0
  /* CHECKSUM_GEN_ICMP==1: Check checksums by hardware for outgoing ICMP packets.*/
  /* Hardware TCP/UDP checksum insertion not supported when packet is an IPv4 fragment*/
  #define CHECKSUM_GEN_ICMP               1
  /* CHECKSUM_CHECK_ICMP==0: Check checksums by hardware for incoming ICMP packets.*/
  #define CHECKSUM_CHECK_ICMP             0
#else
  /* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 1
  /* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                1
  /* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                1
  /* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               1
  /* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              1
  /* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              1
  /* CHECKSUM_GEN_ICMP==1: Check checksums by hardware for outgoing ICMP packets.*/
  #define CHECKSUM_GEN_ICMP               1
  /* CHECKSUM_CHECK_ICMP==1: Check checksums by hardware for incoming ICMP packets.*/
  #define CHECKSUM_CHECK_ICMP             1
#endif


/*
   ---------------------------------------
   ---------- Debugging options ----------
   ---------------------------------------
*/
#define ICMP_DEBUG                      #{{icmp_debug}}
#define IGMP_DEBUG                      #{{igmp_debug}}
#define IP_DEBUG                        #{{ip_debug}}
#define LWIP_DBG_TYPES_ON               #{{lwip_dbg_types_on}}
#define NETIF_DEBUG                     #{{netif_debug}}
#define PBUF_DEBUG                      #{{pbuf_debug}}
#define SYS_DEBUG                       #{{sys_debug}}
#define TCP_DEBUG                       #{{tcp_debug}}
#define UDP_DEBUG                       #{{udp_debug}}

#endif /* __LWIPOPTS_H__ */