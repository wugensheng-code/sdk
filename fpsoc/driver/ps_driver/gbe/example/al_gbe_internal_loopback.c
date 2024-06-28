/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_hal.h"

#define AL_GBE_DEVICE_ID            GBE_DEVICE_ID

#define GBE_PHY_ADDR                (0)

#define ETH_RX_BUFFER_SIZE          (1600UL)

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

/* Gbe driver handle */
static AL_GBE_HalStruct *GbeHandle;

/* Tx config, Configure the Tx descriptor function according to this */
static AL_GBE_TxDescConfigStruct TxConfig;

AL_U8 SendFrame[] =
{
    0x64 ,0x00 ,0x6a ,0x7b ,0x95 ,0x49 ,0xf4 ,0xa4 ,0xd6 ,0xd2 ,0xef ,0x37 ,0x08 ,0x00 ,0x45 ,0x00,
    0x00 ,0x5d ,0x29 ,0xf1 ,0x00 ,0x00 ,0x7f ,0x11 ,0xdb ,0x0d ,0x0a ,0x08 ,0x19 ,0x80 ,0x0a ,0x08,
    0x09 ,0x02 ,0xcc ,0xfb ,0x0d ,0x3d ,0x00 ,0x49 ,0x7c ,0x46 ,0x3a ,0x04 ,0xc1 ,0x51 ,0x0a ,0x00,
    0x24 ,0xe0 ,0x01 ,0x39 ,0x17 ,0x03 ,0x03 ,0x00 ,0x32 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x3a,
    0x3f ,0xf1 ,0xb0 ,0x52 ,0x1d ,0xba ,0xaa ,0x5e ,0xdb ,0x43 ,0xa1 ,0x99 ,0x85 ,0x03 ,0xcf ,0x10,
    0x25 ,0x82 ,0xd8 ,0xb1 ,0x47 ,0x95 ,0xe5 ,0x7d ,0x43 ,0x12 ,0x98 ,0x43 ,0x79 ,0x0d ,0xb0 ,0x23,
    0x2b ,0xeb ,0xd8 ,0x2d ,0x34 ,0x8a ,0x91 ,0x2b ,0x9e ,0xb2 ,0xd9
};

AL_U8 RecvFrame[ETH_RX_BUFFER_SIZE];

AL_S32 AlGbe_PhyInit(AL_GBE_MacDmaConfigStruct *MacDmaConfig)
{
    AL_S32 Ret;
    AL_U8 Speed;
    AL_U8 Duplex;
    AL_U8 Linkchange = 0;

    /* Initialize the ETH PHY */
    Ret = AlGbe_Hal_PhyInit(GbeHandle, GBE_PHY_ADDR);
    if (Ret != 0)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Hal_PhyInit Init failed\r\n");
        return Ret;
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "AlGbe get phy link status...\r\n");
    Ret = AlGbe_Hal_GetPhyLinkStatus(GbeHandle, GBE_PHY_ADDR, &Speed, &Duplex);
    if (Ret != 0)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe phy link down\r\n");
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

    if (GbeHandle->Dev.PhyId != PHY_ID_DEFAULT) {
        Linkchange = 1;
    }

    if (Linkchange)
    {
        AlGbe_Hal_ConfigDuplexAndSpeed(GbeHandle);
    }

    return AL_OK;
}

AL_VOID AlGbe_Init()
{
    AL_S32 Ret;
    AL_GBE_InitStruct InitConfig;
    AL_GBE_MacDmaConfigStruct MacDmaConfig;

    /* Initialize Gbe driver first */
    InitConfig.MacAddr[0] = 0x64;
    InitConfig.MacAddr[1] = 0x00;
    InitConfig.MacAddr[2] = 0x6a;
    InitConfig.MacAddr[3] = 0x19;
    InitConfig.MacAddr[4] = 0x74;
    InitConfig.MacAddr[5] = 0x7f;
    InitConfig.MediaInterface = AL_GBE_RGMII_MODE;
    InitConfig.RxDescList = DMARxDescList;
    InitConfig.TxDescList = DMATxDescList;
    InitConfig.RxBuffLen = ETH_RX_BUFFER_SIZE;

    MacDmaConfig.DuplexMode = AL_GBE_FULL_DUPLEX_MODE;
    MacDmaConfig.Speed = AL_GBE_SPEED_1G;

    Ret = AlGbe_Hal_Init(&GbeHandle, GBE_DEVICE_ID, &InitConfig, &MacDmaConfig, AL_NULL);
    if (Ret != AL_OK)
    {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Hal_Init failed\r\n");
        return Ret;
    }

    AlGbe_ll_SetLoopbackModeEnable(GbeHandle->Dev.HwConfig.BaseAddress, AL_GBE_FUNC_ENABLE);
    AlGbe_ll_SetRecvAllEnable(GbeHandle->Dev.HwConfig.BaseAddress, AL_GBE_FUNC_ENABLE);

    /* Use static buffer to config rx descriptor buffer */
    for (int idx = 0; idx < AL_GBE_RX_DESC_CNT; idx ++)
    {
        AlGbe_Hal_ConfigRxDescBuffer(GbeHandle, idx, RxBuffTab[idx], NULL);
    }

    AlGbe_PhyInit(&MacDmaConfig);

    AlGbe_Hal_StartMacDma(GbeHandle);
}

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

AL_VOID AlGbe_InternalLoopbackExample()
{
    AL_U32 FrameLen = 0;
    AL_U32 TestLoop = 0;
    AL_U8  FailFlag = 0;


    AlGbe_Init();

    while (1) {

        AlGbe_SendFrame(SendFrame, sizeof(SendFrame));

        FrameLen = 0;
        memset(RecvFrame, 0, sizeof(RecvFrame));
        AlGbe_RecvFrame(RecvFrame, &FrameLen);
        if (FrameLen == sizeof(SendFrame)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Gbe Received packet:");

            for (AL_U16 Index = 0; Index < FrameLen; Index++) {
                 printf("0x%x ", RecvFrame[Index]);
                if ((Index+1) % 16 == 0) {
                    printf("\r\n");
                }
                if (RecvFrame[Index] != SendFrame[Index]) {
                    FailFlag = 1;
                }

            }
            printf("\r\n");
            printf("\r\n");
            TestLoop++;
        } else if (FrameLen == 0 ) {
            // Do Nothing
        } else {
            FailFlag = 1;
        }

        if (FailFlag) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Gbe internal loopback fail\r\n");
            return;
        }

        if (TestLoop == 6) {
            break;
        }
    }

    AL_LOG(AL_LOG_LEVEL_INFO, "Gbe internal loopback pass\r\n");
}

int main()
{
    AL_LOG(AL_LOG_LEVEL_INFO, "AL GBE internal loopback example\r\n");

    AlGbe_InternalLoopbackExample();

    return 0;
}
