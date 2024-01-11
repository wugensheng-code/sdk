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

//#define AL_GBE_TX_DESC_CNT    16
//#define AL_GBE_RX_DESC_CNT    16

/*
   If it is defined as 1536, it will cause memory stampede,
   here it is defined as 1560, which can work normally.
   need check.
*/
#define ETH_RX_BUFFER_SIZE          (1600UL)

/* Maximum Ethernet payload size */
#define ETH_MAX_PAYLOAD             (1500UL)

#define GBE_MAC_ADDR0    (0x64)
#define GBE_MAC_ADDR1    (0x00)
#define GBE_MAC_ADDR2    (0x6a)
#define GBE_MAC_ADDR3    (0x19)
#define GBE_MAC_ADDR4    (0x74)
#define GBE_MAC_ADDR5    (0x7f)

#define IP_ADDR0        192
#define IP_ADDR1        168
#define IP_ADDR2        31
#define IP_ADDR3        222

#define GW_ADDR0        192
#define GW_ADDR1        168
#define GW_ADDR2        31
#define GW_ADDR3        1

#define NETMASK_ADDR0        255
#define NETMASK_ADDR1        255
#define NETMASK_ADDR2        255
#define NETMASK_ADDR3        0

void netif_config(void);

#if NO_SYS
void ethernetif_input(struct netif *netif);
#endif


#ifdef __cplusplus
}
#endif

#endif /* AL_GBE_ETHERNETIF_H */
