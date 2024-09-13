/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_ethlite_hal.h"


#define ETH_MAC_ADDR_LEN    6
#define ETH_IP_ADDR_LEN     4

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


#define AL_ETHLITE_DEVICE_ID       0

AL_U8 LocalMacAddr[ETH_MAC_ADDR_LEN]= {0x64, 0x00, 0x6a, 0x19, 0x74, 0x7f};
AL_U8 LocalIpAddr[ETH_IP_ADDR_LEN]= {192, 168, 31, 222};

AL_U8 BroadcastAddr[ETH_MAC_ADDR_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};


/*
 * Buffers used for Transmission and Reception of Packets. These are declared as
 * global so that they are not a part of the stack.
 */
#define AL_MAX_FRAME_SIZE                 1600
#define AL_MAX_RX_BUFFER_COUNT            32

typedef struct
{
    AL_U8       RxBuffer[AL_MAX_FRAME_SIZE];
    AL_U16      RxFrameLen;
    AL_BOOL     RxValidFlag;
} AL_RxBufferStruct;

static AL_AXI_ETHLITE_HalStruct *AXIETHHandle;

static AL_U8 SendFrame[AL_MAX_FRAME_SIZE];
static AL_RxBufferStruct RecvFrame[AL_MAX_RX_BUFFER_COUNT];

static volatile AL_U8 TxDoneFlag = 0;
static volatile RxValidCount = 0;
static volatile RxValidProcessed = 0;

static AL_VOID AlAxiEthLite_TxDoneCallback(AL_VOID *CallbackRef)
{
    TxDoneFlag = 1;
}

static AL_VOID AlAxiEthLite_RxValidCallback(AL_VOID *CallbackRef)
{
    AL_S32 Ret;

    Ret  = AlAxiEthLite_Hal_RecvFrameIntr(AXIETHHandle, &RecvFrame[RxValidCount], &RecvFrame[RxValidCount].RxFrameLen);
    if (Ret == AL_OK) {
        RecvFrame[RxValidCount].RxValidFlag = AL_TRUE;
        RxValidCount = (RxValidCount + 1) % AL_MAX_RX_BUFFER_COUNT;
    }
}

AL_VOID AlAxiEthLite_SendFrame(AL_U8 *Frame, AL_U32 FrameSize)
{
    AlAxiEthLite_Hal_SendFrameIntr(AXIETHHandle, Frame, FrameSize);

    while (TxDoneFlag == 0);
    TxDoneFlag = 0;
}

static AL_U16 CalculatCheckSum(AL_U16 *RxFramePtr, AL_U16 Start, AL_U16 Length)
{
    AL_U32 Sum = 0;
    AL_U16 CheckSum = 0;
    AL_U16 Index;

    Index = Start;
    while (Index < (Start + Length)) {
        Sum = Sum + AL_NTOHS(*(RxFramePtr + Index));
        Index++;
    }

    CheckSum = Sum;
    Sum = Sum >> 16;
    CheckSum = Sum + CheckSum;
    return CheckSum;
}

AL_VOID  AlAxiEthLite_ProcessIcmpPacket(AL_AXI_ETHLITE_HalStruct *AXIETHHandle, AL_U8 *RecvFrame, AL_U32 FrameSize)
{
    AL_U16 *RxFramePtr;
    AL_U16 *TxFramePtr;
    AL_U16 *TempPtr;
    AL_U16 CheckSum;
    AL_U32 Index;

    TxFramePtr = (AL_U16 *)SendFrame;
    RxFramePtr = (AL_U16 *)RecvFrame;

    /* Check ETH protocol type*/
    if (AL_NTOHS(*(RxFramePtr + ETH_PROTO_TYPE_OFFSET)) == ETH_PROTO_TYPE_IP) {

        /* Check IP header checksum */
        CheckSum = CalculatCheckSum(RxFramePtr, IP_HDR_START_OFFSET, IP_HDR_LEN);

        if (CheckSum == CORRECT_CKSUM_VALUE) {

            AL_U16 IpPacketLen = AL_NTOHS(*(RxFramePtr + 8));
            AL_U16 ICMPpacketLen = IpPacketLen - (IP_HDR_LEN * 2);
            ICMPpacketLen = ICMPpacketLen / 2;

            /* Check ICMP checksum */
            CheckSum = CalculatCheckSum(RxFramePtr, ICMP_DATA_START_OFFSET, ICMPpacketLen);
            if (CheckSum == CORRECT_CKSUM_VALUE) {

                /* Fill the dest mac address into ETH header */
                Index = SRC_MAC_ADDR_OFFSET;
                while (Index < (SRC_MAC_ADDR_OFFSET + MAC_ADDR_LEN)) {
                    *TxFramePtr++ = *(RxFramePtr + Index);
                    Index++;
                }

                /* Fill the source mac address into ETH header */
                Index = 0;
                TempPtr = (AL_U16 *)LocalMacAddr;
                while (Index < MAC_ADDR_LEN) {
                    *TxFramePtr++ = *TempPtr++;
                    Index++;
                }

                /* Fill the ETH protocol type、 IP version into ETH and IP HEADER */
                Index = ETH_PROTO_TYPE_OFFSET;
                while (Index < (ETH_PROTO_TYPE_OFFSET + ETH_PROTO_TYPE_LEN + IP_VERSION_LEN + IP_PACKET_LEN)) {
                    *TxFramePtr++ = *(RxFramePtr + Index);
                    Index++;
                }

                /* Fill Identification into IP header */
                *TxFramePtr++ = IP_IDENT_FIELD_VALUE;

                /* Fill Flags、IP frag、TTL into IP header */
                Index = IP_FRAG_FIELD_OFFSET;
                while (Index < (IP_FRAG_FIELD_OFFSET + IP_TTL_ICM_LEN + IP_FRAG_FIELD_LEN)) {
                    *TxFramePtr++ = *(RxFramePtr + Index);
                    Index++;
                }

                /* Fill 0 into Checksum first */
                *TxFramePtr++ = 0x0000;

                /* Fill source IP address */
                Index = 0;
                TempPtr = (AL_U16 *)LocalIpAddr;
                while (Index < IP_ADDR_LEN) {
                    *TxFramePtr++ = *TempPtr++;
                    Index++;
                }

                /* Fill dest IP address */
                Index = ICMP_REQ_SRC_IP_OFFSET;
                while (Index < (ICMP_REQ_SRC_IP_OFFSET + IP_ADDR_LEN)) {
                    *TxFramePtr++ = *(RxFramePtr + Index);
                    Index++;
                }

                /* Calculate IP header checksum and fill checksum */
                CheckSum = CalculatCheckSum( (AL_U16 *)SendFrame, IP_HDR_START_OFFSET, IP_HDR_LEN);
                CheckSum = ~CheckSum;
                *(TxFramePtr - IP_CSUM_OFFSET_BACK) = AL_HTONS(CheckSum);

                AL_U16 *IcmpHead = TxFramePtr;
                Index = ICMP_ECHO_FIELD_OFFSET;
                while (Index < (ICMP_ECHO_FIELD_OFFSET +
                        ICMP_ECHO_FIELD_LEN)) {
                    *TxFramePtr++ = 0x0000;
                    Index++;
                }

                /* Fill data to buffer which was received from the packet */
                Index = ICMP_DATA_OFFSET;
                while (Index < (ICMP_DATA_OFFSET + ICMPpacketLen-2)) {
                    *TxFramePtr++ = (*(RxFramePtr + Index));
                    Index++;
                }

                /* Calculate ICMP checksum and fill checksum */
                CheckSum = CalculatCheckSum((AL_U16 *)SendFrame, ICMP_DATA_START_OFFSET, ICMPpacketLen);
                CheckSum = ~CheckSum;
                *(IcmpHead + 1) = AL_HTONS(CheckSum);

                /* Send Frame */
                AlAxiEthLite_SendFrame( (AL_U8 *)&SendFrame, IpPacketLen + 14);
            }
        }
    }
}

AL_VOID  AlAxiEthLite_ProcessArpPacket(AL_AXI_ETHLITE_HalStruct *AXIETHHandle, AL_U8 *RecvFrame, AL_U32 FrameSize)
{
    AL_U16 *RxFramePtr;
    AL_U16 *TxFramePtr;
    AL_U16 *TempPtr;
    AL_U32 Index;

    TxFramePtr = (AL_U16 *)SendFrame;
    RxFramePtr = (AL_U16 *)RecvFrame;

    if (AL_NTOHS(*(RxFramePtr + ETH_PROTO_TYPE_OFFSET)) == ETH_PROTO_TYPE_ARP) {

        TempPtr = (AL_U16 *)LocalIpAddr;

        /* Check whether the arp packet is for us */
        if (memcmp(RxFramePtr + ARP_REQ_DEST_IP_OFFSET_1, TempPtr, ETH_IP_ADDR_LEN) == 0) {

            /* Check ARP packet type, request or reply */
            if (AL_NTOHS(*(RxFramePtr + ARP_REQ_STATUS_OFFSET)) == ARP_REQUEST) {

                /* Fill the dest mac address into ETH header */
                Index = SRC_MAC_ADDR_OFFSET;
                while (Index < (SRC_MAC_ADDR_OFFSET + MAC_ADDR_LEN)) {
                    *TxFramePtr++ = *(RxFramePtr + Index);
                    Index++;
                }

                /* Fill the source mac address into ETH IP header */
                Index = 0;
                TempPtr = (AL_U16 *)LocalMacAddr;
                while (Index < MAC_ADDR_LEN) {
                    *TxFramePtr++ = *TempPtr++;
                    Index++;
                }

                /* Fill ARP type、Hardware type、Protocol type、Hardware size、Protocol size into ETH and IP header */
                Index = ETH_PROTO_TYPE_OFFSET;
                while (Index < (ETH_PROTO_TYPE_OFFSET + ETH_PROTO_TYPE_LEN + ARP_HW_TYPE_LEN +
                                ARP_HW_ADD_LEN + ARP_PROTO_ADD_LEN)) {
                    *TxFramePtr++ = *(RxFramePtr + Index);
                    Index++;
                }

                /* Fill Opcode(reply) into ARP header */
                *TxFramePtr++ = AL_HTONS(ARP_REPLY);

                /* Fill Send MAC into ARP header */
                TempPtr = (AL_U16 *)LocalMacAddr;
                Index = 0;
                while (Index < MAC_ADDR_LEN) {
                    *TxFramePtr++ = *TempPtr++;
                    Index++;
                }

                /* Fill Send IP Address into ARP header */
                TempPtr = (AL_U16 *)LocalIpAddr;
                Index = 0;
                while (Index < IP_ADDR_LEN) {
                    *TxFramePtr++ = *TempPtr++ ;
                    Index++;
                }

                /* Fill Target MAC into ARP header */
                Index = SRC_MAC_ADDR_OFFSET;
                while (Index < (SRC_MAC_ADDR_OFFSET + MAC_ADDR_LEN)) {
                    *TxFramePtr++ = *(RxFramePtr + Index);
                    Index++;
                }

                /* Fill Trget IP Address into ARP header */
                Index = ARP_REQ_SRC_IP_OFFSET;
                while (Index < (ARP_REQ_SRC_IP_OFFSET + IP_ADDR_LEN)) {
                    *TxFramePtr++ = *(RxFramePtr + Index);
                    Index++;
                }

                /* Fill 0 */
                Index = 0;
                while (Index < ARP_ZEROS_LEN) {
                    *TxFramePtr++ = 0x0000;
                    Index++;
                }

                /*
                 * Transmit the Reply Packet.
                 */
                AlAxiEthLite_SendFrame((AL_U8 *)&SendFrame, ARP_PACKET_SIZE);
            }
        }
    }

}

static void ProcessPingReplyRecvFrame(AL_RxBufferStruct *Buffer)
{
    AL_U16 *RxFramePtr;

    RxFramePtr = (AL_U16 *)Buffer->RxBuffer;
    AL_U16 FrameSize = Buffer->RxFrameLen;

    /* Check the packet type */
    if (memcmp(RxFramePtr, BroadcastAddr, ETH_MAC_ADDR_LEN) == 0) {
         AlAxiEthLite_ProcessArpPacket(AXIETHHandle, RxFramePtr, FrameSize);
    } else if (memcmp(RxFramePtr, LocalMacAddr, ETH_MAC_ADDR_LEN) == 0) {
         AlAxiEthLite_ProcessIcmpPacket(AXIETHHandle, RxFramePtr, FrameSize);
    } else {
        // Do Nothing
    }

}

static int AlAxiEthLite_PingReplyExample()
{
    AL_U16 FrameLen = 0;

    AL_AXI_ETHLITE_InitStruct InitConfig = {
        .MacAddr = {LocalMacAddr[0], LocalMacAddr[1], LocalMacAddr[2],
                    LocalMacAddr[3], LocalMacAddr[4], LocalMacAddr[5]},
        .Speed   = AL_AXI_ETHLITE_SPEED_AUTONEG,
    };

    AlAxiEthLite_Hal_Init(&AXIETHHandle, AL_ETHLITE_DEVICE_ID, &InitConfig, AL_NULL);

    AL_LOG(AL_LOG_LEVEL_INFO, "Static IP  : %d.%d.%d.%d\r\n", LocalIpAddr[0], LocalIpAddr[1],
                                                              LocalIpAddr[2], LocalIpAddr[3]);
    AL_LOG(AL_LOG_LEVEL_INFO, "Mac Address: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                                            LocalMacAddr[0], LocalMacAddr[1], LocalMacAddr[2],
                                            LocalMacAddr[3], LocalMacAddr[4], LocalMacAddr[5]);

   AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

    AlAxiEthLite_Hal_RegisterIntrHandlerCallBack(AXIETHHandle, AL_AXI_ETHLITE_INTR_RX_VALID,
                                                 AlAxiEthLite_RxValidCallback);
    AlAxiEthLite_Hal_RegisterIntrHandlerCallBack(AXIETHHandle, AL_AXI_ETHLITE_INTR_TX_DONE,
                                                 AlAxiEthLite_TxDoneCallback);

    AlAxiEthLite_Hal_EnableIntr(AXIETHHandle);

    while (1) {
        if (RecvFrame[RxValidProcessed].RxValidFlag == AL_TRUE) {
            ProcessPingReplyRecvFrame(&RecvFrame[RxValidProcessed]);
            RecvFrame[RxValidProcessed].RxValidFlag = AL_FALSE;
            RxValidProcessed = (RxValidProcessed + 1) % AL_MAX_RX_BUFFER_COUNT;
        }
    }
}

AL_S32 main()
{
    AL_U32 RegValue;

    RegValue = AL_REG32_READ(0xF8800080);
    RegValue &= ~0x2;
    AL_REG32_WRITE(0xF8800080, RegValue);

    AL_LOG(AL_LOG_LEVEL_INFO, "AlAxiEthLite ping reply intr example\r\n");

    AlAxiEthLite_PingReplyExample();

    return AL_OK;
}