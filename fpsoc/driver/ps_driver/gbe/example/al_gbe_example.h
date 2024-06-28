/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_GBE_EXAMPLE_H_
#define __AL_GBE_EXAMPLE_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "al_gbe_hal.h"

#define ETH_MAC_ADDR_LEN    6
#define ETH_IP_ADDR_LEN     4

#define ETH_RX_BUFFER_SIZE          (1600UL)


#define AL_HTONS(DATA) ((AL_U16)((((DATA) & (AL_U16)0x00ffU) << 8) | (((DATA) & (AL_U16)0xff00U) >> 8)))
#define AL_NTOHS(DATA) AL_HTONS(DATA)


#define ETH_PROTO_TYPE_IP           0x0800  /* IP Protocol */
#define ETH_PROTO_TYPE_ARP          0x0806  /* ARP Protocol */


#define BROADCAST_PACKET            1 /* Broadcast packet */
#define ARP_REQUEST                 0x0001 /* ARP Request bits in Rx packet */
#define ARP_REPLY                   0x0002 /* ARP status bits indicating reply */
#define ARP_PACKET_SIZE             0x3C /* ARP packet len 60 Bytes */
#define BROADCAST_ADDR              0xFFFF /* Broadcast Address */
#define CORRECT_CKSUM_VALUE         0xFFFF /* Correct checksum value */
#define IP_IDENT_FIELD_VALUE        0x91A6 /* Identification field (random num) */


#define ETH_PROTO_TYPE_LEN          1 /* Ethernet protocol Type length  */
#define SRC_MAC_ADDR_OFFSET         3 /* Source MAC address location */
#define MAC_ADDR_LEN                3 /* MAC address length */
#define ETH_PROTO_TYPE_OFFSET       6 /* Ethernet Proto type location */


#define ARP_HW_TYPE_LEN             1 /* Hardware Type length  */
#define ARP_PROTO_TYPE_LEN          1 /* Protocol Type length  */
#define ARP_HW_ADD_LEN              1 /* Hardware address length */
#define ARP_PROTO_ADD_LEN           1 /* Protocol address length */
#define ARP_ZEROS_LEN               9 /* Length to be filled with zeros */
#define ARP_REQ_STATUS_OFFSET       10 /* ARP request location */
#define ARP_REQ_SRC_IP_OFFSET       14 /* Src IP address location of ARP request */
#define ARP_REQ_DEST_IP_OFFSET_1    19 /* Destination IP's 1st half word location */
#define ARP_REQ_DEST_IP_OFFSET_2    20 /* Destination IP's 2nd half word location */


#define IP_VERSION_LEN              1 /* IP Version length  */
#define IP_PACKET_LEN               1 /* IP Packet length field  */
#define IP_FRAG_FIELD_LEN           1 /* Fragment field len in ICMP packet */
#define IP_TTL_ICM_LEN              1 /* Time to live and ICM fields length */
#define IP_ADDR_LEN                 2 /* Size of IP address in half-words */
#define IP_CSUM_OFFSET_BACK         5 /* IP checksum location from end of frame */
#define IP_HDR_START_OFFSET         7 /* IP header start location */
#define IP_HDR_LEN                  10 /* IP Header length */
#define IP_FRAG_FIELD_OFFSET        10 /* Fragment field location */


#define ICMP_HEAD_LEN               4 /* ICMP Type length */
#define ICMP_TYPE_LEN               1 /* ICMP Type length */
#define ICMP_ECHO_FIELD_LEN         2 /* Echo field length in half-words */
#define ICMP_REQ_SRC_IP_OFFSET      13 /* Src IP address location of ICMP request */
#define ICMP_ECHO_FIELD_OFFSET      17 /* Echo field location */
#define ICMP_DATA_START_OFFSET      17 /* Data field start location */
#define ICMP_DATA_LEN               18 /* ICMP data length */
#define ICMP_DATA_OFFSET            19 /* ICMP data location including identifier number and sequence number */
#define ICMP_DATA_FIELD_LEN         20 /* Data field length */

#ifdef __cplusplus
}
#endif

#endif