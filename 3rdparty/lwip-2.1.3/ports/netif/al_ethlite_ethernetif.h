/*
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
 */

/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_GBE_ETHERNETIF_H_
#define __AL_GBE_ETHERNETIF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lwip/netif.h"

/* Maximum Ethernet payload size */
#define ETH_MAX_PAYLOAD             (1500UL)

#define MAC_ADDR0       (0x64)
#define MAC_ADDR1       (0x00)
#define MAC_ADDR2       (0x6a)
#define MAC_ADDR3       (0x19)
#define MAC_ADDR4       (0x74)
#define MAC_ADDR5       (0x7f)

#define IP_ADDR0        192
#define IP_ADDR1        168
#define IP_ADDR2        31
#define IP_ADDR3        222

#define GW_ADDR0        192
#define GW_ADDR1        168
#define GW_ADDR2        31
#define GW_ADDR3        1

#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0


#define QUEUE_SIZE      4096
#define NUM_QUEUES      2

typedef struct {
    void *data[QUEUE_SIZE];
    int head;
    int tail;
    int len;
} queue_t;

/*
  The following PHY registers are defined by the IEEE802.3 standard,
  and the same register and bit represent the same or similar functions.
 */
/* Basic mode control register */
#define PHY_BCR_REG                         0x0

/* Basic Mode Status Register */
#define PHY_BSR_REG                         0x1

/* PHY ID one register. */
#define PHY_ID1_REG                         0x02

/* PHY ID two register. */
#define PHY_ID2_REG                         0x03

/* PHY auto-negotiate advertise register */
#define PHY_AUTONEG_ADVERTISE_REG           0x04

/* PHY auto negotiation link partner ability register */
#define PHY_AUTONEG_LINKPARTNER_REG         0x05

/* PHY auto negotiation expansion register */
#define PHY_AUTONEG_EXPANSION_REG           0x06

#define PHY_SSR_REG                         0x10

#define PHY_BCTL_RESET_MASK                 0x8000
#define PHY_BCTL_AUTONEG_MASK               0x1000
#define PHY_BCTL_RESTART_AUTONEG_MASK       0x0200


#define PHY_ADVERTISE_10HALF_MASK           0x0020  /* Try for 10mbps half-duplex  */
#define PHY_ADVERTISE_10FULL_MASK           0x0040  /* Try for 10mbps full-duplex  */
#define PHY_ADVERTISE_100HALF_MASK          0x0080  /* Try for 100mbps half-duplex */
#define PHY_ADVERTISE_100FULL_MASK          0x0100  /* Try for 100mbps full-duplex */
#define PHY_ADVERTISE_100BASE4_MASK         0x0200  /* Try for 100mbps 4k packets  */


#define PHY_ADVERTISE_100_AND_10_MASK       (PHY_ADVERTISE_10FULL_MASK | PHY_ADVERTISE_100FULL_MASK | \
                                             PHY_ADVERTISE_10HALF_MASK | PHY_ADVERTISE_100HALF_MASK)
#define PHY_ADVERTISE_100_MASK              (PHY_ADVERTISE_100FULL_MASK | PHY_ADVERTISE_100HALF_MASK)
#define PHY_ADVERTISE_10_MASK               (PHY_ADVERTISE_10FULL_MASK | PHY_ADVERTISE_10HALF_MASK)

#define PHY_BSTATUS_AUTONEGCOMP_MASK        0x0020

#define PHY_SPEED_10M                       1
#define PHY_SPEED_100M                      0

/* Defines the PHY link duplex. */
#define PHY_HALF_DUPLEX                     0
#define PHY_FULL_DUPLEX                     1

void ethlite_netif_config(void);

#if NO_SYS
void ethlite_ethernetif_input(struct netif *netif);
#endif

queue_t* queue_create();
int queue_send(queue_t *q, void *p);
void *queue_recv(queue_t *q);
int queue_get_len(queue_t *q);

#ifdef __cplusplus
}
#endif

#endif /* AL_GBE_ETHERNETIF_H */
