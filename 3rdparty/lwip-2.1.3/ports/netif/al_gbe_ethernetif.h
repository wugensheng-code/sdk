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
#define ETH_RX_BUFFER_SIZE          (1560UL)

/* Maximum Ethernet payload size */
#define ETH_MAX_PAYLOAD             (1500UL)

#define GBE_MAC_ADDR0    (0x64)
#define GBE_MAC_ADDR1    (0x00)
#define GBE_MAC_ADDR2    (0x6a)
#define GBE_MAC_ADDR3    (0x19)
#define GBE_MAC_ADDR4    (0x74)
#define GBE_MAC_ADDR5    (0x7f)

#define IP_ADDR0        10
#define IP_ADDR1        8
#define IP_ADDR2        9
#define IP_ADDR3        88

#define GW_ADDR0        10
#define GW_ADDR1        8
#define GW_ADDR2        9
#define GW_ADDR3        1

#define NETMASK_ADDR0        255
#define NETMASK_ADDR1        255
#define NETMASK_ADDR2        255
#define NETMASK_ADDR3        0

#define ETH_TX_PACKETS_FEATURES_CSUM          0x00000001U
#define ETH_TX_PACKETS_FEATURES_SAIC          0x00000002U
#define ETH_TX_PACKETS_FEATURES_VLANTAG       0x00000004U
#define ETH_TX_PACKETS_FEATURES_INNERVLANTAG  0x00000008U
#define ETH_TX_PACKETS_FEATURES_TSO           0x00000010U
#define ETH_TX_PACKETS_FEATURES_CRCPAD        0x00000020U


#define ETH_DMATXNDESCRF_CIC_IPHDR_PAYLOAD_INSERT_PHDR_CALC 0x3

#define ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC  ETH_DMATXNDESCRF_CIC_IPHDR_PAYLOAD_INSERT_PHDR_CALC


#define ETH_DMATXNDESCRF_CPC_CRC_INSERT                      0x04000000U


#define ETH_CRC_PAD_INSERT  ETH_DMATXNDESCRF_CPC_CRC_INSERT


void netif_config(void);


#if NO_SYS
void ethernetif_input(struct netif *netif);
#else
static void ethernetif_input( void const * argument );
#endif


#ifdef __cplusplus
}
#endif

#endif /* AL_GBE_ETHERNETIF_H */
