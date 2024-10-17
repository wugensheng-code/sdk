/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_axi_ethernet_lite_dev.h"

static AL_AXI_ETHLITE_InitStruct AlAxiEthLiteInitConfigs =
{
    .MacAddr = {0x64, 0x00, 0x6a, 0x19, 0x74, 0x7f},
    .Speed   = AL_AXI_ETHLITE_SPEED_100M,
};

static AL_AXI_ETHLITE_DefaultInitStruct AxiEthDefInitConfigs =
{
    .TxIfgVal               = 0,
    .RxEnable               = AL_AXI_ETHLITE_FUNC_ENABLE,
    .RxFlowCtlEnable        = AL_AXI_ETHLITE_FUNC_DISABLE,
    .RxLenTypChkEnable      = AL_AXI_ETHLITE_FUNC_ENABLE,
    .RxCtlFrameLenChkEnable = AL_AXI_ETHLITE_FUNC_ENABLE,
    .RxJumboEnable          = AL_AXI_ETHLITE_FUNC_DISABLE,
    .RxFcsChkEnable         = AL_AXI_ETHLITE_FUNC_ENABLE,
    .RxVlanEnable           = AL_AXI_ETHLITE_FUNC_DISABLE,
    .RxHfDuplexEnable       = AL_AXI_ETHLITE_FUNC_DISABLE,
    .TxEnable               = AL_AXI_ETHLITE_FUNC_ENABLE,
    .TxFlowCtlEnable        = AL_AXI_ETHLITE_FUNC_DISABLE,
    .TxIfgAdjEnable         = AL_AXI_ETHLITE_FUNC_DISABLE,
    .TxJumboEnable          = AL_AXI_ETHLITE_FUNC_DISABLE,
    .TxFcsGenEnable         = AL_AXI_ETHLITE_FUNC_ENABLE,
    .TxVlanEnable           = AL_AXI_ETHLITE_FUNC_DISABLE,
    .TxHfDuplexEnable       = AL_AXI_ETHLITE_FUNC_DISABLE,
};

extern AlAxiEthLite_HwConfigStruct AlAxiEthLite_HwConfig[AL_AXI_ETH_LITE_NUM_INSTANCE];

AlAxiEthLite_HwConfigStruct *AlAxiEthLite_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AlAxiEthLite_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_AXI_ETH_LITE_NUM_INSTANCE; Index++) {
        if (AlAxiEthLite_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlAxiEthLite_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

static AL_VOID AlAxiEthLite_AlignWrite(const AL_VOID *SrcPtr, AL_U32 *DestPtr, AL_U32 Length)
{
    const AL_U16 *FromU16Ptr;
    AL_U32 AlignBuffer;
    AL_U32 *ToU32Ptr;
    AL_U16 *ToU16Ptr;

    ToU32Ptr = DestPtr;
    FromU16Ptr = SrcPtr;
    AlignBuffer = 0;

    for (; Length > 3; Length -= 4) {
        ToU16Ptr = (AL_U16 *)&AlignBuffer;
        *ToU16Ptr++ = *FromU16Ptr++;
        *ToU16Ptr++ = *FromU16Ptr++;

        /* This barrier resolves occasional issues seen around
         * cases where the data is not properly flushed out
         * from the processor store buffers to the destination
         * memory locations.
         */
        //wmb();
       //DMB();

        /* Output a word */
        *ToU32Ptr++ = AlignBuffer;
    }
    if (Length) {
        AL_U8 *FromU8Ptr, *ToU8Ptr;

        /* Set up to output the remaining data */
        AlignBuffer = 0;
        ToU8Ptr = (AL_U8 *)&AlignBuffer;
        FromU8Ptr = (AL_U8 *)FromU16Ptr;

        /* Output the remaining data */
        for (; Length > 0; Length--)
            *ToU8Ptr++ = *FromU8Ptr++;

        /* This barrier resolves occasional issues seen around
         * cases where the data is not properly flushed out
         * from the processor store buffers to the destination
         * memory locations.
         */
        //wmb();
        //DMB();
        *ToU32Ptr = AlignBuffer;
    }
}

AL_VOID AlAxiEthLite_AlignRead(AL_U32 *SrcPtr, AL_U8 *DestPtr, AL_U32 Length)
{
    AL_U16 *ToU16Ptr, *FromU16Ptr;
    AL_U32 *FromU32Ptr;
    AL_U32 AlignBuffer;

    FromU32Ptr = SrcPtr;
    ToU16Ptr = (AL_U16 *)DestPtr;

    for (; Length > 3; Length -= 4) {
        /* Copy each word into the temporary buffer */
        AlignBuffer = *FromU32Ptr++;
        FromU16Ptr = (AL_U16 *)&AlignBuffer;

        /* Read data from source */
        *ToU16Ptr++ = *FromU16Ptr++;
        *ToU16Ptr++ = *FromU16Ptr++;
    }

    if (Length) {
        AL_U8 *ToU8Ptr, *FromU8Ptr;

        /* Set up to read the remaining data */
        ToU8Ptr = (AL_U8 *)ToU16Ptr;
        AlignBuffer = *FromU32Ptr++;
        FromU8Ptr = (AL_U8 *)&AlignBuffer;

        /* Read the remaining data */
        for (; Length > 0; Length--)
            *ToU8Ptr = *FromU8Ptr;
    }
}

AL_S32 AlAxiEthLite_Dev_WritePhyRegister(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U32 PHYAddr,
                                         AL_U32 PHYReg, AL_U16 RegValue)
{
    AL_ASSERT((AxiEth != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;
    AL_U32 TmpReg;

    TmpReg = AlAxiEthLite_ll_ReadMdioOperation(EthBaseAddr);

    AL_REG32_SET_BITS(&TmpReg, AXI_ETHLITE_MDIO_OPERATION_PHY_ADDR_SHIFT,
                      AXI_ETHLITE_MDIO_OPERATION_PHY_ADDR_SIZE, PHYAddr);
    AL_REG32_SET_BITS(&TmpReg, AXI_ETHLITE_MDIO_OPERATION_PHY_REG_SHIFT,
                      AXI_ETHLITE_MDIO_OPERATION_PHY_REG_SIZE, PHYReg);
    AL_REG32_SET_BITS(&TmpReg, AXI_ETHLITE_MDIO_OPERATION_READ_WRITE_SHIFT, 
                      AXI_ETHLITE_MDIO_OPERATION_READ_WRITE_SIZE, AL_AXI_ETHLITE_MDIO_CMD_WRITE);
    AL_REG32_SET_BIT(&TmpReg, AXI_ETHLITE_MDIO_OPERATION_TRANS_ENABLE_SHIFT, AL_AXI_ETHLITE_FUNC_ENABLE);

    AlAxiEthLite_ll_MdioWriteData(EthBaseAddr, (AL_U16)RegValue);

    AlAxiEthLite_ll_WriteMdioOperation(EthBaseAddr, TmpReg);

    do {

    } while(AlAxiEthLite_ll_IsMdioBusy(EthBaseAddr));

    return AL_OK;
}

AL_S32 AlAxiEthLite_Dev_ReadPhyRegister(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U32 PHYAddr,
                                        AL_U32 PHYReg, AL_U16 *RegValue)
{
    AL_ASSERT((AxiEth != AL_NULL) && (RegValue != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;
    AL_U32 TmpReg;

    TmpReg = AlAxiEthLite_ll_ReadMdioOperation(EthBaseAddr);

    AL_REG32_SET_BITS(&TmpReg, AXI_ETHLITE_MDIO_OPERATION_PHY_ADDR_SHIFT,
                      AXI_ETHLITE_MDIO_OPERATION_PHY_ADDR_SIZE, PHYAddr);
    AL_REG32_SET_BITS(&TmpReg, AXI_ETHLITE_MDIO_OPERATION_PHY_REG_SHIFT,
                      AXI_ETHLITE_MDIO_OPERATION_PHY_REG_SIZE, PHYReg);
    AL_REG32_SET_BITS(&TmpReg, AXI_ETHLITE_MDIO_OPERATION_READ_WRITE_SHIFT,
                      AXI_ETHLITE_MDIO_OPERATION_READ_WRITE_SIZE, AL_AXI_ETHLITE_MDIO_CMD_READ);
    AL_REG32_SET_BIT(&TmpReg, AXI_ETHLITE_MDIO_OPERATION_TRANS_ENABLE_SHIFT, AL_AXI_ETHLITE_FUNC_ENABLE);

   AlAxiEthLite_ll_WriteMdioOperation(EthBaseAddr, TmpReg);

    do {

    } while(AlAxiEthLite_ll_IsMdioBusy(EthBaseAddr));

    *RegValue = AlAxiEthLite_ll_MdioReadData(EthBaseAddr);

    return AL_OK;
}

static AL_S32 AlAxiEthLite_Dev_CheckConfigParam(AL_AXI_ETHLITE_InitStruct *InitConfig)
{
    AL_ASSERT (((InitConfig->Speed == AL_AXI_ETHLITE_SPEED_10M) ||
                (InitConfig->Speed == AL_AXI_ETHLITE_SPEED_100M) ||
                (InitConfig->Speed == AL_AXI_ETHLITE_SPEED_AUTONEG)), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    return AL_OK;
}

static AL_VOID AlAxiEthLite_Dev_SetEthConfig(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_AXI_ETHLITE_InitStruct *InitConfig)
{
    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    AlAxiEthLite_ll_EnableTxHalfDuplex(EthBaseAddr, AxiEthDefInitConfigs.TxHfDuplexEnable);
    AlAxiEthLite_ll_EnableTxVlan(EthBaseAddr, AxiEthDefInitConfigs.TxVlanEnable);
    AlAxiEthLite_ll_EnableTxFcs(EthBaseAddr, AxiEthDefInitConfigs.TxFcsGenEnable);
    AlAxiEthLite_ll_EnableTxJumboFrame(EthBaseAddr, AxiEthDefInitConfigs.TxJumboEnable);
    AlAxiEthLite_ll_EnableTxInterFrameGapAdjust(EthBaseAddr, AxiEthDefInitConfigs.TxIfgAdjEnable);
    AlAxiEthLite_ll_EnableTxFlowControl(EthBaseAddr, AxiEthDefInitConfigs.TxFlowCtlEnable);
    AlAxiEthLite_ll_EnableTx(EthBaseAddr, AxiEthDefInitConfigs.TxEnable);
    AlAxiEthLite_ll_EnableRxHalfDuplex(EthBaseAddr, AxiEthDefInitConfigs.RxHfDuplexEnable);
    AlAxiEthLite_ll_EnableRxVlan(EthBaseAddr, AxiEthDefInitConfigs.RxVlanEnable);
    AlAxiEthLite_ll_EnableRxFcs(EthBaseAddr, AxiEthDefInitConfigs.RxFcsChkEnable);
    AlAxiEthLite_ll_EnableRxJumboFrame(EthBaseAddr, AxiEthDefInitConfigs.RxJumboEnable);
    AlAxiEthLite_ll_EnableRxControlFrameLenCheck(EthBaseAddr, AxiEthDefInitConfigs.RxCtlFrameLenChkEnable);
    AlAxiEthLite_ll_EnableRxLenTypeCheck(EthBaseAddr, AxiEthDefInitConfigs.RxLenTypChkEnable);
    AlAxiEthLite_ll_EnableRxFlowControl(EthBaseAddr, AxiEthDefInitConfigs.RxFlowCtlEnable);
    AlAxiEthLite_ll_EnableRx(EthBaseAddr, AxiEthDefInitConfigs.RxEnable);
    AlAxiEthLite_ll_SetTxInterFrameGapValue(EthBaseAddr, AxiEthDefInitConfigs.TxIfgVal);
    AlAxiEthLite_ll_SetSpeed(EthBaseAddr, AL_AXI_ETHLITE_SPEED_100M);
}

static AL_S32 AlAxiEthLite_SoftReset(AL_AXI_ETHLITE_DevStruct *AxiEth)
{
    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    AlAxiEthLite_ll_ResetMac(EthBaseAddr, 0xFFFFFFFF);

    AlSys_MDelay(100);

    AlAxiEthLite_ll_ResetMac(EthBaseAddr, 0x0);

    AlSys_MDelay(100);

    return AL_OK;
}

static AL_VOID AlAxiEthLitelite_FlushReceive(AL_AXI_ETHLITE_DevStruct *AxiEth)
{
    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    AlAxiEthLite_ll_ClrRxPingBufferFrameFlag(EthBaseAddr);
    AlAxiEthLite_ll_ClrRxPongBufferFrameFlag(EthBaseAddr);
}

AL_S32 AlAxiEthLite_Dev_Init(AL_AXI_ETHLITE_DevStruct *AxiEth, AlAxiEthLite_HwConfigStruct *HwConfig,
                             AL_AXI_ETHLITE_InitStruct *InitConfig)
{
    AL_ASSERT((AxiEth != AL_NULL) && (HwConfig != AL_NULL) && (InitConfig != AL_NULL),
              AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_S32 Ret;
    AL_U32 IpInfo;
    AL_REG EthBaseAddr;
    AL_U16 TxBuffSize, RxBuffSize;
    AL_U8  MdioClockDiv;

    /* check the InitConfig */
    if (InitConfig != AL_NULL) {
        Ret = AlAxiEthLite_Dev_CheckConfigParam(InitConfig);
        if (Ret != AL_OK) {
            return Ret;
        }

        AxiEth->InitConfig = *InitConfig;
    } else {
        AxiEth->InitConfig = AlAxiEthLiteInitConfigs;
    }

    AxiEth->HwConfig = *HwConfig;

    EthBaseAddr = HwConfig->BaseAddress;

    IpInfo = AlAxiEthLite_ll_GetIpInfo(EthBaseAddr);
    if ((IpInfo == 0) || (IpInfo == 0xFFFFFFFF)) {
        return AL_ERR_NOT_SUPPORT;
    }

    TxBuffSize = AlAxiEthLite_ll_GetTxBuffSize(EthBaseAddr);
    RxBuffSize = AlAxiEthLite_ll_GetRxBuffSize(EthBaseAddr);

    AlAxiEthLite_Dev_SetEthConfig(AxiEth, InitConfig);

    if (AlAxiEthLite_ll_IsMdioExist(EthBaseAddr) == AL_TRUE) {
        MdioClockDiv = AxiEth->HwConfig.InputClk / 2500000;
        AlAxiEthLite_ll_SetMdioClockDiv(EthBaseAddr, MdioClockDiv);
        AlAxiEthLite_ll_EnableMdio(EthBaseAddr, AL_AXI_ETHLITE_FUNC_ENABLE);

    } else {
        return AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM;
    }

    AxiEth->NextRxBufIsPong = AL_FALSE;
    AxiEth->NextTxBufIsPong = AL_FALSE;
    AxiEth->MaxTxPktSize = (AxiEthDefInitConfigs.TxJumboEnable) ? (TxBuffSize+1) : AL_AXI_ETHLITE_MAX_FRAME_SIZE;
    AxiEth->MaxRxPktSize = (AxiEthDefInitConfigs.RxJumboEnable) ? (RxBuffSize+1) : AL_AXI_ETHLITE_MAX_FRAME_SIZE;

    AlAxiEthLite_SoftReset(AxiEth);
    AlAxiEthLitelite_FlushReceive(AxiEth);

    return AL_OK;
}

AL_S32 AlAxiEthLite_Dev_ConfigDuplexAndSpeed(AL_AXI_ETHLITE_DevStruct *AxiEth,
                                             AL_AXI_ETHLITE_SpeedEnum Speed, AL_U8 Duplex)
{
    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    AlAxiEthLite_ll_SetSpeed(EthBaseAddr, Speed);
    return AL_OK;
}

AL_BOOL AlAxiEthLite_IsRxBufferEmpty(AL_AXI_ETHLITE_DevStruct *AxiEth)
{
    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    if ((AlAxiEthLite_ll_IsRxPingBufferRecvFrame(EthBaseAddr) == AL_TRUE) ||
        (AlAxiEthLite_ll_IsRxPongBufferRecvFrame(EthBaseAddr) == AL_TRUE)) {
        return AL_FALSE;
    }

    return AL_TRUE;
}

AL_U16 AlAxiEthLite_Dev_RecvFrame(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U8 *RecvBuffer, AL_U16 *RecvLen)
{
    AL_U16 FrameLen;
    AL_BOOL IsBadFrame;
    volatile AL_U32 *RxBufferAddr;

    AL_ASSERT((AxiEth != AL_NULL) && (RecvBuffer != AL_NULL) && (RecvLen != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    /* Wait for a frame to arrive - this is a blocking call */
    while (AlAxiEthLite_IsRxBufferEmpty(AxiEth) == AL_TRUE);

    /*
     * Get the Length of the frame that arrived, only 32-bit reads are
     * allowed LengthType is in the upper half of the 32-bit word.
     */
    if (AlAxiEthLite_ll_IsRxPingBufferRecvFrame(EthBaseAddr) == AL_TRUE) {
        FrameLen = AlAxiEthLite_ll_GetRxPingBufferRecvFrameLen(EthBaseAddr);
        IsBadFrame = AlAxiEthLite_ll_IsRxPingBufferRecvBadFrame(EthBaseAddr);
        RxBufferAddr = (AL_U8 *)EthBaseAddr + AXI_ETHLITE_RX_PING_BUFFER_ADDR_OFFSET;

        if (IsBadFrame == AL_FALSE) {
            //memcpy(RecvBuffer, RxBufferAddr, FrameLen);
            AlAxiEthLite_AlignRead(RxBufferAddr, RecvBuffer, FrameLen);
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlAxiEthLite_RecvFrame bad frame\r\n");
        }

        AlAxiEthLite_ll_ClrRxPingBufferFrameFlag(EthBaseAddr);

        do {

        } while(AlAxiEthLite_ll_IsRxPingBufferRecvFrame(EthBaseAddr));

    } else {
        FrameLen = AlAxiEthLite_ll_GetRxPongBufferRecvFrameLen(EthBaseAddr);
        IsBadFrame = AlAxiEthLite_ll_IsRxPongBufferRecvBadFrame(EthBaseAddr);
        RxBufferAddr = (AL_U8 *)EthBaseAddr + AXI_ETHLITE_RX_PONG_BUFFER_ADDR_OFFSET;

        if (IsBadFrame == AL_FALSE) {
            //memcpy(RecvBuffer, RxBufferAddr, FrameLen);
            AlAxiEthLite_AlignRead(RxBufferAddr, RecvBuffer, FrameLen);
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlAxiEthLite_RecvFrame bad frame\r\n");
        }

        AlAxiEthLite_ll_ClrRxPongBufferFrameFlag(EthBaseAddr);

        do {

        } while(AlAxiEthLite_ll_IsRxPongBufferRecvFrame(EthBaseAddr));
    }

    *RecvLen = FrameLen;

    return AL_OK;
}

AL_S32 AlAxiEthLite_Dev_SendFrame(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U8 *SendBuffer, AL_U16 SendLen)
{
    AL_ASSERT((AxiEth != AL_NULL) && (SendBuffer != AL_NULL) && (SendLen > 0), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    if (SendLen > AxiEth->MaxTxPktSize) {

        AL_LOG(AL_LOG_LEVEL_WARNING, "AlAxiEthLite_SendFrame The fame Length exceeded the Tx Buffer Length.\r\n");
        SendLen = AxiEth->MaxTxPktSize;
    }

    if (AxiEth->NextTxBufIsPong) {

        //memcpy((AL_U8*)EthBaseAddr + AXI_ETHLITE_TX_PONG_BUFFER_ADDR_OFFSET, SendBuffer, SendLen);
        AlAxiEthLite_AlignWrite(SendBuffer, EthBaseAddr + AXI_ETHLITE_TX_PONG_BUFFER_ADDR_OFFSET, SendLen);

        AlAxiEthLite_ll_SetTxPongBufferFrameLenAndEnableTx(EthBaseAddr, SendLen);

        while(AlAxiEthLite_ll_IsTxPongBufferBusy(EthBaseAddr) == AL_TRUE);
        AxiEth->NextTxBufIsPong = AL_FALSE;

    } else {

        //memcpy((AL_U8 *)EthBaseAddr + AXI_ETHLITE_TX_PING_BUFFER_ADDR_OFFSET, SendBuffer, SendLen);
        AlAxiEthLite_AlignWrite(SendBuffer, EthBaseAddr + AXI_ETHLITE_TX_PING_BUFFER_ADDR_OFFSET, SendLen);

        AlAxiEthLite_ll_SetTxPingBufferFrameLenAndEnableTx(EthBaseAddr, SendLen);

        while(AlAxiEthLite_ll_IsTxPingBufferBusy(EthBaseAddr) == AL_TRUE);
        AxiEth->NextTxBufIsPong = AL_TRUE;
    }

    return AL_OK;
}

AL_U32 AlAxiEthLite_Dev_RecvFrameIntr(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U8 *RecvBuffer, AL_U16 *RecvLen)
{
    AL_U16 FrameLen;
    volatile AL_U32 *RxBufferAddr;

    AL_U32 PtrStatus;

    AL_ASSERT((AxiEth != AL_NULL) && (RecvBuffer != AL_NULL) && (RecvLen != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    if (AlAxiEthLite_ll_IsRxPingBufferRecvFrame(EthBaseAddr) == AL_TRUE) {
        FrameLen = AlAxiEthLite_ll_GetRxPingBufferRecvFrameLen(EthBaseAddr);
        RxBufferAddr = (AL_U8 *)EthBaseAddr + AXI_ETHLITE_RX_PING_BUFFER_ADDR_OFFSET;

        //memcpy(RecvBuffer, RxBufferAddr, FrameLen);

        AlAxiEthLite_AlignRead(RxBufferAddr, RecvBuffer, FrameLen);
        AlAxiEthLite_ll_ClrRxPingBufferFrameFlag(EthBaseAddr);

    } else if (AlAxiEthLite_ll_IsRxPongBufferRecvFrame(EthBaseAddr) == AL_TRUE) {
        FrameLen = AlAxiEthLite_ll_GetRxPongBufferRecvFrameLen(EthBaseAddr);
        RxBufferAddr = (AL_U8 *)EthBaseAddr + AXI_ETHLITE_RX_PONG_BUFFER_ADDR_OFFSET;

        //memcpy(RecvBuffer, RxBufferAddr, FrameLen);
        AlAxiEthLite_AlignRead(RxBufferAddr, RecvBuffer, FrameLen);
        AlAxiEthLite_ll_ClrRxPongBufferFrameFlag(EthBaseAddr);

    } else {
        AL_LOG(AL_LOG_LEVEL_WARNING, "AlAxiEthLite_Dev_RecvFrameIntr no data valid.\r\n");
        return AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM;
    }

    *RecvLen = FrameLen;

    return AL_OK;
}

AL_BOOL AlAxiEthLite_Dev_IsTxBufferAvailable(AL_AXI_ETHLITE_DevStruct *AxiEth)
{
    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    if ((AlAxiEthLite_ll_IsTxPingBufferBusy(EthBaseAddr) == AL_FALSE) ||
        (AlAxiEthLite_ll_IsTxPongBufferBusy(EthBaseAddr) == AL_FALSE)) {
        return AL_TRUE;
    }

    return AL_FALSE;
}

AL_S32 AlAxiEthLite_Dev_SendFrameIntr(AL_AXI_ETHLITE_DevStruct *AxiEth, AL_U8 *SendBuffer, AL_U32 SendLen)
{
    AL_ASSERT((AxiEth != AL_NULL) && (SendBuffer != AL_NULL) && (SendLen > 0), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_U32 Status;
    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    if (SendLen > AxiEth->MaxTxPktSize) {

        AL_LOG(AL_LOG_LEVEL_WARNING, "AlAxiEthLite_SendFrame The fame Length exceeded the Tx Buffer Length.\r\n");
        SendLen = AxiEth->MaxTxPktSize;
    }

    if (AlAxiEthLite_ll_IsTxPingBufferBusy(EthBaseAddr) == AL_FALSE) {
        AlAxiEthLite_AlignWrite(SendBuffer, EthBaseAddr + AXI_ETHLITE_TX_PING_BUFFER_ADDR_OFFSET, SendLen);

        AlAxiEthLite_ll_SetTxPingBufferFrameLenAndEnableTx(EthBaseAddr, SendLen);

    } else if (AlAxiEthLite_ll_IsTxPongBufferBusy(EthBaseAddr) == AL_FALSE) {
        AlAxiEthLite_AlignWrite(SendBuffer, EthBaseAddr + AXI_ETHLITE_TX_PONG_BUFFER_ADDR_OFFSET, SendLen);

        AlAxiEthLite_ll_SetTxPongBufferFrameLenAndEnableTx(EthBaseAddr, SendLen);

    } else {
        AL_LOG(AL_LOG_LEVEL_WARNING, "AlAxiEthLite_Dev_SendFrameIntr tx busy");
    }

    return AL_OK;
}

AL_S32 AlAxiEthLite_Dev_EnableIntr(AL_AXI_ETHLITE_DevStruct *AxiEth)
{
    AL_ASSERT((AxiEth != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    AlAxiEthLite_ll_ClrRxPingBufferFrameFlag(EthBaseAddr);
    AlAxiEthLite_ll_ClrRxPongBufferFrameFlag(EthBaseAddr);
    AlAxiEthLite_ll_ClrRXFrameLossCount(EthBaseAddr);

    AlAxiEthLite_ll_ClrIntr(EthBaseAddr, 0xffffffff);

    AlAxiEthLite_ll_EnableRxValidIntr(EthBaseAddr, AL_AXI_ETHLITE_FUNC_ENABLE);
    AlAxiEthLite_ll_EnableTxDoneIntr(EthBaseAddr, AL_AXI_ETHLITE_FUNC_ENABLE);
    AlAxiEthLite_ll_EnableRxRecvBadFrameIntr(EthBaseAddr, AL_AXI_ETHLITE_FUNC_ENABLE);

    return AL_OK;
}

AL_S32 AlAxiEthLite_Dev_DisableleIntr(AL_AXI_ETHLITE_DevStruct *AxiEth)
{
    AL_ASSERT((AxiEth != AL_NULL), AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    AlAxiEthLite_ll_EnableRxValidIntr(EthBaseAddr, AL_AXI_ETHLITE_FUNC_DISABLE);
    AlAxiEthLite_ll_EnableTxDoneIntr(EthBaseAddr, AL_AXI_ETHLITE_FUNC_DISABLE);
    AlAxiEthLite_ll_EnableRxRecvBadFrameIntr(EthBaseAddr, AL_AXI_ETHLITE_FUNC_DISABLE);

    AlAxiEthLite_ll_ClrIntr(EthBaseAddr, 0xffffffff);

    return AL_OK;
}

AL_S32 AlAxiEthLite_Dev_RegisterEventCallBack(AL_AXI_ETHLITE_DevStruct *AxiEth,
                                              AL_AXI_ETHLITE_EventCallBack Callback, AL_VOID *CallbackRef)
{
    AL_ASSERT((AxiEth != AL_NULL) && (Callback != AL_NULL) && (CallbackRef != AL_NULL),
              AL_AXI_ETHLITE_ERR_ILLEGAL_PARAM);

    AxiEth->EventCallBack        = Callback;
    AxiEth->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

static AL_VOID AlAxiEthLite_Dev_RxValidHandler(AL_AXI_ETHLITE_DevStruct *AxiEth)
{
    if (AxiEth->EventCallBack) {
        AL_AXI_ETHLITE_EventStruct Event = {
            .Event        = AL_AXI_ETHLITE_EVENT_RX_VALID,
        };
        (*AxiEth->EventCallBack)(&Event, AxiEth->EventCallBackRef);
    }

    //AlAxiEthLite_ll_ClrRxPingBufferFrameFlag(AxiEth->HwConfig.BaseAddress);
    //AlAxiEthLite_ll_ClrRxPongBufferFrameFlag(AxiEth->HwConfig.BaseAddress);
}

static AL_VOID AlAxiEthLite_Dev_TxDoneHandler(AL_AXI_ETHLITE_DevStruct *AxiEth)
{
    if (AxiEth->EventCallBack) {
        AL_AXI_ETHLITE_EventStruct Event = {
            .Event        = AL_AXI_ETHLITE_EVENT_TX_DONE,
        };
        (*AxiEth->EventCallBack)(&Event, AxiEth->EventCallBackRef);
    }
}

static AL_VOID AlAxiEthLite_Dev_RxBadFrameHandler(AL_AXI_ETHLITE_DevStruct *AxiEth)
{
    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    AlAxiEthLite_ll_ClrRxPingBufferFrameFlag(EthBaseAddr);
    AlAxiEthLite_ll_ClrRxPongBufferFrameFlag(EthBaseAddr);
}

#define ETHLITE_IN_RX_VALID_INTR(Event)             (Event & AL_AXI_ETHLITE_INTR_RX_VALID)
#define ETHLITE_IN_TX_IDLE_INTR(Event)              (Event & AL_AXI_ETHLITE_INTR_TX_IDLE)
#define ETHLITE_IN_TX_DONE_INTR(Event)              (Event & AL_AXI_ETHLITE_INTR_TX_DONE)
#define ETHLITE_IN_RX_FRAME_LOSS_INTR(Event)        (Event & AL_AXI_ETHLITE_INTR_RX_FRAME_LOSS)
#define ETHLITE_IN_RX_BAD_FRAME_INTR(Event)         (Event & AL_AXI_ETHLITE_INTR_RX_BAD_FRAME)

AL_VOID AlAxiEthLite_Dev_IntrHandler(AL_VOID *Instance)
{
    AL_U32 IntrEvent;
    AL_AXI_ETHLITE_DevStruct *AxiEth = (AL_AXI_ETHLITE_DevStruct *)Instance;
    AL_REG EthBaseAddr = AxiEth->HwConfig.BaseAddress;

    IntrEvent = AlAxiEthLite_ll_GetIntrPending(EthBaseAddr);

    if (ETHLITE_IN_RX_VALID_INTR(IntrEvent)) {
        AlAxiEthLite_Dev_RxValidHandler(AxiEth);
    }

    if (ETHLITE_IN_TX_DONE_INTR(IntrEvent)) {
        AlAxiEthLite_Dev_TxDoneHandler(AxiEth);
    }

    if (ETHLITE_IN_TX_IDLE_INTR(IntrEvent)) {
    }

    if (ETHLITE_IN_RX_FRAME_LOSS_INTR(IntrEvent)) {
        AlAxiEthLite_ll_ClrRXFrameLossCount(EthBaseAddr);
    }

    if (ETHLITE_IN_RX_BAD_FRAME_INTR(IntrEvent)) {
        AlAxiEthLite_Dev_RxBadFrameHandler(AxiEth);
    }

    AlAxiEthLite_ll_ClrIntr(EthBaseAddr, IntrEvent);
}