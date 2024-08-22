/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_example.h"

#define AL_GBE_DEVICE_ID            GBE_DEVICE_ID

#define GBE_PHY_ADDR                (0)

#ifdef ENABLE_MMU
/* Tx and Rx descriptors define, AL_GBE_RX_DESC_CNT and AL_GBE_TX_DESC_CNT at least four */
static AL_GBE_DMADescStruct DMARxDescList[AL_GBE_RX_DESC_CNT] __attribute__((section(".noncacheable")));
static AL_GBE_DMADescStruct DMATxDescList[AL_GBE_TX_DESC_CNT] __attribute__((section(".noncacheable")));
#else
/* Tx and Rx descriptors define, AL_GBE_RX_DESC_CNT and AL_GBE_TX_DESC_CNT at least four */
static AL_GBE_DMADescStruct DMARxDescList[AL_GBE_RX_DESC_CNT] CACHE_LINE_ALIGN;
static AL_GBE_DMADescStruct DMATxDescList[AL_GBE_TX_DESC_CNT] CACHE_LINE_ALIGN;
#endif

/* Rx descriptors buffer, use static buffer, just user Rx descriptors buffer1 */
static AL_U8 RxBuffTab[AL_GBE_RX_DESC_CNT][ETH_RX_BUFFER_SIZE] CACHE_LINE_ALIGN;
static AL_U8 TxBuffTab[AL_GBE_TX_DESC_CNT][ETH_RX_BUFFER_SIZE] CACHE_LINE_ALIGN;

/* Gbe driver handle */
static AL_GBE_HalStruct *GbeHandle;

AL_U8 LocalMacAddr[ETH_MAC_ADDR_LEN]= {0x64, 0x00, 0x6a, 0x19, 0x74, 0x7f};
AL_U8 LocalIpAddr[4]= {192, 168, 31, 222};

AL_U8 BroadcastAddr[ETH_MAC_ADDR_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

/* Tx config, Configure the Tx descriptor function according to this */
static AL_GBE_TxDescConfigStruct TxConfig;

#define FRAME_SIZE      ETH_RX_BUFFER_SIZE

AL_U8 SendFrame[FRAME_SIZE];
AL_U8 RecvFrame[FRAME_SIZE];

/**
 * This function initializes the PHY layer by setting up the PHY address, checking the link status,
 * and configuring the speed and duplex mode based on the PHY's capabilities. It also checks for
 * any link changes and applies the new configuration if necessary.
 *
 * @param MacDmaConfig Pointer to the MAC DMA configuration structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlPhy_Init(AL_GBE_MacDmaConfigStruct *MacDmaConfig)
{
    AL_S32 Ret;
    AL_U8 LinkStatus;
    AL_U8 Speed;
    AL_U8 Duplex;
    AL_U8 Linkchange = 0;

    Ret = AlGbe_DetectPhy(GbeHandle);
    if (Ret != 0)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_DetectPhy failed\r\n");
        return Ret;
    }

    /* Initialize the ETH PHY */
    Ret = AlGbe_PhyInit(&GbeHandle->Dev.PhyDev);
    if (Ret != 0)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_PhyInit failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AlGbe get phy link status...\r\n");
    Ret = AlGbe_GetPhyLinkStatus(&GbeHandle->Dev.PhyDev, &LinkStatus);
    if ((Ret != 0) && (LinkStatus != AL_TRUE))
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe phy link down\r\n");
        return Ret;
    }

    Ret = AlGbe_GetPhyLinkSpeedDuplex(&GbeHandle->Dev.PhyDev, &Speed, &Duplex);
    if (Ret != 0)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_GetPhyLinkSpeedDuplex failed\r\n");
        return Ret;
    }
    AL_LOG(AL_LOG_LEVEL_INFO, "AlGbe phy link up success\r\n");

    if (PHY_SPEED_10M == Speed)
    {
        if (MacDmaConfig->Speed != AL_GBE_SPEED_10M)
        {
            GbeHandle->Dev.MacDmaConfig.Speed = AL_GBE_SPEED_10M;
            Linkchange = 1;
        }
    }
    else if (PHY_SPEED_100M == Speed)
    {
        if (MacDmaConfig->Speed != AL_GBE_SPEED_100M)
        {
            GbeHandle->Dev.MacDmaConfig.Speed = AL_GBE_SPEED_100M;
            Linkchange = 1;
        }
    }
    else
    {
        if (MacDmaConfig->Speed != AL_GBE_SPEED_1G)
        {
            GbeHandle->Dev.MacDmaConfig.Speed = AL_GBE_SPEED_1G;
            Linkchange = 1;
        }
    }

    if (PHY_FULL_DUPLEX == Duplex)
    {
        if (MacDmaConfig->DuplexMode != AL_GBE_FULL_DUPLEX_MODE)
        {
            GbeHandle->Dev.MacDmaConfig.DuplexMode = AL_GBE_FULL_DUPLEX_MODE;
            Linkchange = 1;
        }
    }
    else
    {
        if (MacDmaConfig->DuplexMode != AL_GBE_HALF_DUPLEX_MODE)
        {
            GbeHandle->Dev.MacDmaConfig.DuplexMode = AL_GBE_HALF_DUPLEX_MODE;
            Linkchange = 1;
        }
    }

    if (GbeHandle->Dev.PhyDev.PhyId != PHY_ID_DEFAULT)
    {
        Linkchange = 1;
    }

    if (Linkchange)
    {
        AlGbe_Hal_ConfigDuplexAndSpeed(GbeHandle);
    }

    return AL_OK;
}

/**
 * This function sets up the GBE driver with the specified MAC address, media interface, and
 * descriptor lists for RX and TX. It also initializes the MAC DMA configuration to default values
 * and starts the MAC DMA.
 *
 * @return None.
 */
AL_VOID AlGbe_Init()
{
    AL_S32 Ret;
    AL_GBE_InitStruct InitConfig;
    AL_GBE_MacDmaConfigStruct MacDmaConfig;

    /* Initialize Gbe driver first */
    InitConfig.MacAddr[0] = LocalMacAddr[0];
    InitConfig.MacAddr[1] = LocalMacAddr[1];
    InitConfig.MacAddr[2] = LocalMacAddr[2];
    InitConfig.MacAddr[3] = LocalMacAddr[3];
    InitConfig.MacAddr[4] = LocalMacAddr[4];
    InitConfig.MacAddr[5] = LocalMacAddr[5];
    InitConfig.MediaInterface = AL_GBE_RGMII_MODE;
    InitConfig.RxDescList = DMARxDescList;
    InitConfig.TxDescList = DMATxDescList;
    InitConfig.RxBuffLen = ETH_RX_BUFFER_SIZE;

    MacDmaConfig.DuplexMode = AL_GBE_FULL_DUPLEX_MODE;
    MacDmaConfig.Speed = AL_GBE_SPEED_AUTONEG;

    Ret = AlGbe_Hal_Init(&GbeHandle, GBE_DEVICE_ID, &InitConfig, &MacDmaConfig, AL_NULL);
    if (Ret != AL_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Hal_Init failed\r\n");
        return Ret;
    }

    /* Use static buffer to config tx/rx descriptor buffer */
    Ret = AlGbe_Hal_ConfigRxDescBuffer(GbeHandle, RxBuffTab, AL_GBE_RX_DESC_CNT, ETH_RX_BUFFER_SIZE);
    if (Ret != AL_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Hal_ConfigRxDescBuffer failed\r\n");
        return Ret;
    }
    Ret = AlGbe_Hal_ConfigTxDescBuffer(GbeHandle, TxBuffTab, AL_GBE_TX_DESC_CNT, ETH_RX_BUFFER_SIZE);
    if (Ret != AL_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Hal_ConfigTxDescBuffer failed\r\n");
        return Ret;
    }

    Ret = AlPhy_Init(&MacDmaConfig);
    if (Ret != AL_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlPhy_Init Init failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Static IP  : %d.%d.%d.%d\r\n", LocalIpAddr[0], LocalIpAddr[1],
                                                              LocalIpAddr[2], LocalIpAddr[3]);
    AL_LOG(AL_LOG_LEVEL_INFO, "Mac Address: %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                                            LocalMacAddr[0], LocalMacAddr[1], LocalMacAddr[2],
                                            LocalMacAddr[3], LocalMacAddr[4], LocalMacAddr[5]);

    AlGbe_Hal_StartMacDma(GbeHandle);
}

/**
 * This function prepares a TX buffer with the provided frame and frame size, then initiates
 * transmission of the frame through the GBE interface.
 *
 * @param Frame Pointer to the frame data to be sent.
 * @param FrameSize Size of the frame in bytes.
 * @return None.
 */
AL_VOID AlGbe_SendFrame(AL_U8 *Frame, AL_U32 FrameSize)
{
    AL_GBE_BufferStruct Txbuffer = {0};

    Txbuffer.Buffer = Frame;
    Txbuffer.Len = FrameSize;

    TxConfig.Length = Txbuffer.Len;
    TxConfig.TxBuffer = &Txbuffer;
    TxConfig.pData = Frame;

    AlGbe_Hal_Transmit(GbeHandle, &TxConfig);

}

/**
 * This function iterates over the specified section of the frame, calculating the checksum
 * by summing the 16-bit words and folding the result.
 *
 * @param RxFramePtr Pointer to the start of the frame.
 * @param Start Starting index within the frame from which to begin checksum calculation.
 * @param Length Number of 16-bit words to include in the checksum calculation.
 * @return Calculated checksum value.
 */
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

/**
 * This function checks the received frame for an ICMP echo request, constructs an ICMP echo reply
 * if the request is valid, and sends the reply. It performs various checks and calculations,
 * including verifying checksums and calculating new checksums for the reply.
 *
 * @param RecvFrame Pointer to the received frame data.
 * @param FrameSize Size of the received frame in bytes.
 * @return None.
 */
AL_VOID AlGbe_ProcessIcmpPacket(AL_U8 *RecvFrame, AL_U32 FrameSize)
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
                AlGbe_SendFrame(SendFrame, IpPacketLen + 14);

            }
        }
    }
}

/**
 *
 * This function checks if the received frame is an ARP request for the local IP address. If so,
 * it constructs an ARP reply packet by filling in the source and destination MAC addresses, ARP
 * operation code, sender and target IP addresses, and then transmits the reply packet.
 *
 * @param RecvFrame Pointer to the received frame.
 * @param FrameSize Size of the received frame.
 * @return None.
 */
AL_VOID AlGbe_ProcessArpPacket(AL_U8 *RecvFrame, AL_U32 FrameSize)
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
                AlGbe_SendFrame(SendFrame, ARP_PACKET_SIZE);
            }
        }
    }

}

/**
 *
 * This function determines the type of the received frame. If the frame is a broadcast frame,
 * it is processed as an ARP packet. If the frame's destination MAC address matches the local MAC
 * address, it is processed as an ICMP packet. Otherwise, the frame is ignored.
 *
 * @param RecvFrame Pointer to the received frame.
 * @param FrameSize Size of the received frame.
 * @return None.
 */
AL_VOID AlGbe_ProcessRecvFrame(AL_U8 *RecvFrame, AL_U32 FrameSize)
{
    AL_U16 *RxFramePtr;

    RxFramePtr = (AL_U16 *)RecvFrame;

    /* Check the packet type */
    if (memcmp(RxFramePtr, BroadcastAddr, ETH_MAC_ADDR_LEN) == 0) {
        AlGbe_ProcessArpPacket(RecvFrame, FrameSize);
    } else if (memcmp(RxFramePtr, LocalMacAddr, ETH_MAC_ADDR_LEN) == 0) {
        AlGbe_ProcessIcmpPacket(RecvFrame, FrameSize);
    } else {
        // Do Nothing
    }

}

/**
 *
 * This function checks if a frame has been received. If so, it retrieves the frame from the
 * hardware, invalidates the cache for the received data (if MMU is enabled), and copies the
 * frame into the provided buffer. It then prepares the RX descriptor for the next data reception.
 *
 * @param Frame Pointer to the buffer where the received frame will be copied.
 * @param FrameSize Pointer to a variable where the size of the received frame will be stored.
 * @return None.
 */
AL_VOID AlGbe_RecvFrame(AL_U8 *Frame, AL_U32 *FrameSize)
{
    AL_GBE_BufferStruct RxBuff = {0};
    AL_U32 framelength = 0;

    if(AlGbe_Hal_GetRxDataBuffer(GbeHandle, &RxBuff) == AL_OK)
    {
        AlGbe_Hal_GetRxDataLength(GbeHandle, &framelength);

        /* Build Rx descriptor to be ready for next data reception */
        AlGbe_Hal_BuildRxDescriptors(GbeHandle);

#ifdef ENABLE_MMU
    AL_UINTPTR BufferAlign = (AL_UINTPTR)GBE_CACHE_ALIGN_MEMORY(RxBuff.Buffer);
    AL_UINTPTR BufferLenAlign = GBE_CACHE_ALIGN_SIZE(framelength) + CACHE_LINE_SIZE;

    AlCache_InvalidateDcacheRange((AL_UINTPTR)BufferAlign, (AL_UINTPTR)(BufferAlign + BufferLenAlign));
#endif

        memcpy(Frame, RxBuff.Buffer, framelength);
        *FrameSize = framelength;
    }
}

/**
 *
 * This function initializes the GBE hardware and continuously checks for received frames. If a
 * frame is received, it processes the frame accordingly. After processing, it resets the frame
 * length and clears the receive and send frame buffers.
 *
 * @return None.
 */
AL_VOID AlGbe_PingReplyPollingExample()
{
    AL_U32 FrameLen = 0;

    AlGbe_Init();

    while (1) {

        AlGbe_RecvFrame(RecvFrame, &FrameLen);
        if (FrameLen != 0) {
            AlGbe_ProcessRecvFrame(RecvFrame, FrameLen);
            FrameLen = 0;
            memset(RecvFrame, 0, FRAME_SIZE);
            memset(SendFrame, 0, FRAME_SIZE);
        }
    }
}

/**
 *
 * This function is the entry point for the program. It logs a start message and then calls the
 * function to demonstrate handling of ping replies using polling.
 *
 * @return Returns 0 on successful execution.
 */
int main()
{
    AL_LOG(AL_LOG_LEVEL_INFO, "AL GBE polling ping reply example\r\n");

    AlGbe_PingReplyPollingExample();

    return 0;
}
