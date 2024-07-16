/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_gbe_dev.h"

#include <string.h>

#undef ENABLE_MMC_COUNT

extern AL_GBE_HwConfigStruct AlGbe_HwConfig[AL_GBE_NUM_INSTANCE];

/**
 * This function iterates through a predefined list of hardware configurations,
 * searching for a configuration that matches the given device ID. If a match is found,
 * a pointer to the configuration structure is returned. Otherwise, NULL is returned.
 *
 * @param DevId The device ID for which to find the hardware configuration.
 * @return A pointer to the matching hardware configuration structure, or NULL if no match is found.
 */
AL_GBE_HwConfigStruct *AlGbe_Dev_LookupConfig(AL_U32 DevId)
{
    AL_U32 Index;
    AL_GBE_HwConfigStruct *ConfigPtr = AL_NULL;

    for (Index = 0; Index < AL_GBE_NUM_INSTANCE; Index++) {
        if (AlGbe_HwConfig[Index].DeviceId == DevId) {
            ConfigPtr = &AlGbe_HwConfig[Index];
            break;
        }
    }

    return ConfigPtr;
}

/**
 * This function examines the state of the device to determine if the transmit path is currently
 * in use. It returns a boolean value indicating the status.
 *
 * @param Gbe A pointer to the device structure.
 * @return AL_TRUE if the TX path is busy, otherwise AL_FALSE.
 */
AL_BOOL AlGbe_Dev_IsTxBusy(AL_GBE_DevStruct *Gbe)
{
    return (AL_BOOL)(Gbe->State & AL_GBE_STATE_TX_BUSY);
}

/**
 * This function examines the state of the device to determine if the receive path is currently
 * in use. It returns a boolean value indicating the status.
 *
 * @param Gbe A pointer to the device structure.
 * @return AL_TRUE if the RX path is busy, otherwise AL_FALSE.
 */
AL_BOOL AlGbe_Dev_IsRxBusy(AL_GBE_DevStruct *Gbe)
{
    return (AL_BOOL)(Gbe->State & AL_GBE_STATE_RX_BUSY);
}

/**
 * This function modifies the state of the device to indicate that the transmit path is currently
 * in use.
 *
 * @param Gbe A pointer to the device structure.
 * @return None.
 */
AL_VOID AlGbe_Dev_SetTxBusy(AL_GBE_DevStruct *Gbe)
{
    Gbe->State |= AL_GBE_STATE_TX_BUSY;
}

/**
 * This function modifies the state of the device to indicate that the receive path is currently
 * in use.
 *
 * @param Gbe A pointer to the device structure.
 * @return None.
 */
AL_VOID AlGbe_Dev_SetRxBusy(AL_GBE_DevStruct *Gbe)
{
    Gbe->State |= AL_GBE_STATE_RX_BUSY;
}

/**
 * This function modifies the state of the device to indicate that the transmit path is no longer
 * in use.
 *
 * @param Gbe A pointer to the device structure.
 * @return None.
 */
AL_VOID AlGbe_Dev_ClrTxBusy(AL_GBE_DevStruct *Gbe)
{
    Gbe->State &= (~AL_GBE_STATE_TX_BUSY);
}

/**
 * This function modifies the state of the device to indicate that the receive path is no longer
 * in use.
 *
 * @param Gbe A pointer to the device structure.
 * @return None.
 */
AL_VOID AlGbe_Dev_ClrRxBusy(AL_GBE_DevStruct *Gbe)
{
    Gbe->State &= (~AL_GBE_STATE_RX_BUSY);
}

/**
 * This function calculates the appropriate CSR clock divider based on the input clock frequency
 * and configures the device's CSR clock range accordingly.
 *
 * @param Gbe A pointer to the device structure.
 * @return None.
 */
AL_VOID AlGbe_Dev_SetCsrClockRange(AL_GBE_DevStruct *Gbe)
{
    AL_U32 Clock = Gbe->HwConfig.InputClockHz;
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_GBE_CsrClockDivEnum ClockDiv;

    /* Set CR bits depending on clock value */
    if ((Clock >= 20000000U) && (Clock < 35000000U)) {
        /* CSR Clock Range between 20-35 MHz */
        ClockDiv = AL_GBE_CSR_CLOCK_DIV16;

    } else if ((Clock >= 35000000U) && (Clock < 60000000U)) {
        /* CSR Clock Range between 35-60 MHz */
        ClockDiv = AL_GBE_CSR_CLOCK_DIV26;

    } else if ((Clock >= 60000000U) && (Clock < 100000000U)) {
        /* CSR Clock Range between 60-100 MHz */
        ClockDiv = AL_GBE_CSR_CLOCK_DIV42;

    } else if ((Clock >= 100000000U) && (Clock < 150000000U)) {
        /* CSR Clock Range between 100-150 MHz */
        ClockDiv = AL_GBE_CSR_CLOCK_DIV62;

    } else if ((Clock >= 150000000) && (Clock <= 200000000)){
        /* CSR Clock Range between 150-200 MHz */
        ClockDiv = AL_GBE_CSR_CLOCK_DIV102;

    } else if ((Clock >= 250000000) && (Clock <= 300000000)) {
        /* CSR Clock Range between 150-200 MHz */
        ClockDiv = AL_GBE_CSR_CLOCK_DIV124;

    } else if ((Clock >= 300000000) && (Clock <= 500000000)) {
        /* CSR Clock Range between 150-200 MHz */
        ClockDiv = AL_GBE_CSR_CLOCK_DIV204;
    } else {
        ClockDiv = AL_GBE_CSR_CLOCK_DIV324;
    }

    AlGbe_ll_SetCsrClockRange(GbeBaseAddr, ClockDiv);
}

/**
 * This function initializes the MAC configuration structure with default values and then
 * modifies specific settings based on the device's current configuration. It applies these
 * settings to the device's MAC configuration registers.
 *
 * @param Gbe A pointer to the device structure.
 * @return None.
 */
AL_VOID AlGbe_Dev_SetMacConfig(AL_GBE_DevStruct *Gbe)
{
    AL_GBE_MacConfigStruct MacDefaultCfg;

    memset(&MacDefaultCfg, 0, sizeof(MacDefaultCfg));
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    MacDefaultCfg.AutoPadCrcStrip = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.CarrierSenseBeforeTransmit = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.ChecksumOffload = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.CrcCheckingRxPackets = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.CrcStripTypePacket = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.DisableDropTCPIPChecksumErrorPacket = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.DuplexMode = Gbe->MacDmaConfig.DuplexMode;
    MacDefaultCfg.ExtendedInterPacketGap = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.ExtendedInterPacketGapVal = 0x0;
    MacDefaultCfg.ForwardRxErrorPacket = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.ForwardRxUndersizedGoodPacket = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.GiantPacketSizeLimit = 1560;
    MacDefaultCfg.GiantPacketSizeLimitControl = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.InterPacketGapVal = AL_GBE_INTER_PACKET_GAP_96BIT;
    MacDefaultCfg.JabberDisable = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.JumboPacket = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.LoopbackMode = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.PauseLowThreshold = AL_GBE_PAUSE_LOW_MINUS_4SOLT_TIMES;
    MacDefaultCfg.PauseTime = 0x0;
    MacDefaultCfg.PreambleLength = AL_GBE_PRE_LEN_7BYTES;
    MacDefaultCfg.ProgrammableWatchdog = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.ReceiveFlowControl = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.RxQueueStoreForward = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.SlowProtocolDetect = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.Speed = Gbe->MacDmaConfig.Speed;
    MacDefaultCfg.Support2KPacket = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.TxQueueStoreForward = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.TransmitFlowControl = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.UnicastPausePacketDetect = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.UnicastSlowProtocolPacketDetect = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.WatchdogDisable = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.WatchdogTimeout =  AL_GBE_WatchdogTimeout2K;
    MacDefaultCfg.ZeroQuantaPause = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.PassAllMulticastPacket = AL_GBE_FUNC_ENABLE;

    /* Config Mac configuretion register */
    AlGbe_ll_SetPreLEn(GbeBaseAddr, MacDefaultCfg.PreambleLength);
    AlGbe_ll_SetCarrierSenseBeforeTransmitEnable(GbeBaseAddr, MacDefaultCfg.CarrierSenseBeforeTransmit);
    AlGbe_ll_SetLoopbackModeEnable(GbeBaseAddr, MacDefaultCfg.LoopbackMode);

    AlGbe_ll_SetDuplexMode(GbeBaseAddr, MacDefaultCfg.DuplexMode);
    AlGbe_ll_SetSpeed(GbeBaseAddr, MacDefaultCfg.Speed);

    AlGbe_ll_SetJumboPacketEnable(GbeBaseAddr, MacDefaultCfg.JumboPacket);
    AlGbe_ll_SetJabberDisable(GbeBaseAddr, MacDefaultCfg.JabberDisable);
    AlGbe_ll_SetWatchdogDisable(GbeBaseAddr, MacDefaultCfg.WatchdogDisable);
    AlGbe_ll_SetAutoPadCrcStripEnable(GbeBaseAddr, MacDefaultCfg.AutoPadCrcStrip);
    AlGbe_ll_SetCrcStripForTypePacketEnable(GbeBaseAddr, MacDefaultCfg.CrcStripTypePacket);
    AlGbe_ll_SetSupport2KPacketEnable(GbeBaseAddr, MacDefaultCfg.Support2KPacket);
    AlGbe_ll_SetGiantPacketSizeLimitCtlEnable(GbeBaseAddr, MacDefaultCfg.GiantPacketSizeLimitControl);
    AlGbe_ll_SetInterPacketGap(GbeBaseAddr, MacDefaultCfg.InterPacketGapVal);
    AlGbe_ll_SetChecksumOffloadEnable(GbeBaseAddr, MacDefaultCfg.ChecksumOffload);

    /* Config Mac extend configuration register */

    /* This field is applicable when the GPSLCE bit is enabled */
    AlGbe_ll_SetGiantPacketSizeLimit(GbeBaseAddr, MacDefaultCfg.GiantPacketSizeLimit);
    AlGbe_ll_SetDisableCrcCheckRecvPackets(GbeBaseAddr, MacDefaultCfg.CrcCheckingRxPackets);
    AlGbe_ll_SetSlowProtDectEnable(GbeBaseAddr, MacDefaultCfg.SlowProtocolDetect);
    AlGbe_ll_SetUnicastSlowProtDectEnable(GbeBaseAddr, MacDefaultCfg.UnicastSlowProtocolPacketDetect);

    AlGbe_ll_SetExtendInterPacketGapEnable(GbeBaseAddr, MacDefaultCfg.ExtendedInterPacketGap);
    /* This field is applicable when the EIPGEN bit is enabled */
    AlGbe_ll_SetExtendInterPacketGap(GbeBaseAddr, MacDefaultCfg.ExtendedInterPacketGapVal);

    /* Config Mac packet filter register */
    AlGbe_ll_SetPromiscuousModeEnable(GbeBaseAddr, MacDefaultCfg.PromiscuousMode);

    /*
      When this bit is set, the MAC performs the destination address
      filtering of unicast packets according to the hash table.
      When this bit is reset, the MAC performs a perfect destination
      address filtering for unicast packets
     */
    AlGbe_ll_SetHashUnicastEnable(GbeBaseAddr, MacDefaultCfg.HashUnicast);
    AlGbe_ll_SetHashMulticastEnable(GbeBaseAddr, MacDefaultCfg.HashMulticast);
    AlGbe_ll_SetDestAddrInverseFilterEnable(GbeBaseAddr, MacDefaultCfg.DesteAddrInverseFilter);
    AlGbe_ll_SetPassAllMulticastEnable(GbeBaseAddr, MacDefaultCfg.PassAllMulticastPacket);
    AlGbe_ll_SetBroadcasPacketsDisable(GbeBaseAddr, MacDefaultCfg.DisableBroadcastPackets);
    AlGbe_ll_SetPassCtlPacketsMode(GbeBaseAddr, MacDefaultCfg.PassControlPackets);
    AlGbe_ll_SetSrcAddrInverseFilterEnable(GbeBaseAddr, MacDefaultCfg.SourceAddrInverseFilter);
    AlGbe_ll_SetSrcAddrFilterEnable(GbeBaseAddr, MacDefaultCfg.SourceAddrFilter);
    AlGbe_ll_SetHashOrPerfectEnable(GbeBaseAddr, MacDefaultCfg.HashOrPerfectFilter);
    AlGbe_ll_SetRecvAllEnable(GbeBaseAddr, MacDefaultCfg.ReceiveAll);

    /* Config Mac hash table register */
    // Todo:

    /* Config Mac watchdog timeout register */
    AlGbe_ll_SetWatchdogTimeout(GbeBaseAddr, MacDefaultCfg.WatchdogTimeout);
    AlGbe_ll_SetProgrammableWatchdogEnable(GbeBaseAddr, MacDefaultCfg.ProgrammableWatchdog);


    /* Config Mac Tx Rx flow control register */
    AlGbe_ll_SetTxFlowCtlEnable(GbeBaseAddr, MacDefaultCfg.TransmitFlowControl);
    AlGbe_ll_SetPauseLowThreshlod(GbeBaseAddr, MacDefaultCfg.PauseLowThreshold);
    AlGbe_ll_SetZeroQuantaPauseDisable(GbeBaseAddr, MacDefaultCfg.ZeroQuantaPause);
    AlGbe_ll_SetPauseTime(GbeBaseAddr, MacDefaultCfg.PauseTime);

    AlGbe_ll_SetRxFlowCtlEnable(GbeBaseAddr, MacDefaultCfg.ReceiveFlowControl);
    AlGbe_ll_SetUnicastPausePacketDetEnable(GbeBaseAddr, MacDefaultCfg.UnicastPausePacketDetect);

    /* Config Mac Vlan register */
    // Todo:

    /* Config MTL TX and RX Queue0 register */
    AlGbe_ll_SetMtlTxQueueStoreForwardEnable(GbeBaseAddr, MacDefaultCfg.TxQueueStoreForward);
    AlGbe_ll_SetMtlRxQueueStoreForwardEnable(GbeBaseAddr, MacDefaultCfg.RxQueueStoreForward);
    AlGbe_ll_SetMtlRxQueueForwardUndersizeGoodPacketsEnable(GbeBaseAddr, MacDefaultCfg.ForwardRxUndersizedGoodPacket);
    AlGbe_ll_SetMtlRxQueueForwardErrorPacketsEnable(GbeBaseAddr, MacDefaultCfg.ForwardRxErrorPacket);
    AlGbe_ll_SetMtlRxQueueDisableDropTcpIpChecksumErrorpackets(GbeBaseAddr, MacDefaultCfg.DisableDropTCPIPChecksumErrorPacket);
}

/**
 * This function sets up the DMA configuration with default values and applies these settings
 * through lower-level hardware interface functions. It configures various aspects of the DMA
 * operation such as arbitration scheme, priority, burst lengths, and more.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return None.
 */

AL_VOID AlGbe_Dev_SetDmaConfig(AL_GBE_DevStruct *Gbe)
{
    AL_GBE_DmaConfigStruct DmaDefaultCfg;
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    memset(&DmaDefaultCfg, 0, sizeof(DmaDefaultCfg));

    DmaDefaultCfg.AddressAlignedBeats = AL_GBE_FUNC_ENABLE;
    DmaDefaultCfg.FixedBurstLen = AL_GBE_FUNC_ENABLE;
    DmaDefaultCfg.DMAArbitration = AL_GBE_DMA_TX_RX_WEIGHT_ROUND_ROBIN;
    DmaDefaultCfg.DMATxPriority = AL_GBE_FUNC_DISABLE;
    DmaDefaultCfg.PriorityRatio = AL_GBE_DMA_PRIORITY_RATIO_1_1;
    DmaDefaultCfg.FlushRxPacket = AL_GBE_FUNC_DISABLE;
    DmaDefaultCfg.PBLx8Mode = AL_GBE_FUNC_DISABLE;
    DmaDefaultCfg.RebuildINCRxBurst = AL_GBE_FUNC_DISABLE;
    DmaDefaultCfg.RxDMABurstLength = AL_GBE_DMA_TX_BURST_LENGTH_32BEAT;
    DmaDefaultCfg.SecondPacketOperate = AL_GBE_FUNC_DISABLE;
    DmaDefaultCfg.TxDMABurstLength = AL_GBE_DMA_TX_BURST_LENGTH_32BEAT;
    DmaDefaultCfg.TCPSegmentation = AL_GBE_FUNC_DISABLE;
    DmaDefaultCfg.MaximumSegmentSize = AL_GBE_SEGMENT_DEFAULT_SIZE;

    /* Config Dma mode register */
    AlGbe_ll_SetDmaTxRxArbitrationScheme(GbeBaseAddr, DmaDefaultCfg.DMAArbitration);
    AlGbe_ll_SetDmaTxPriorityEnable(GbeBaseAddr, DmaDefaultCfg.DMATxPriority);
    AlGbe_ll_SetDmaTxRxPriorityRatio(GbeBaseAddr, DmaDefaultCfg.PriorityRatio);

    /* Config Dma system bus register */
    AlGbe_ll_SetAddrAlignedBeatsEnable(GbeBaseAddr, DmaDefaultCfg.AddressAlignedBeats);
    AlGbe_ll_SetDmaFixedBurstLengthEnable(GbeBaseAddr, DmaDefaultCfg.FixedBurstLen);
    AlGbe_ll_SetRebuildIncrxBurstEnable(GbeBaseAddr, DmaDefaultCfg.RebuildINCRxBurst);

    /* Config Dma channel control register */
    AlGbe_ll_SetDmaMaximumSegmentSize(GbeBaseAddr, DmaDefaultCfg.MaximumSegmentSize);
    AlGbe_ll_SetDma8xPblEnable(GbeBaseAddr, DmaDefaultCfg.PBLx8Mode);

    /* Config Dma channel TX control register */
    AlGbe_ll_SetDmaOperateOnSecondPacketEnable(GbeBaseAddr, DmaDefaultCfg.SecondPacketOperate);
    AlGbe_ll_SetDmaTxBurstLen(GbeBaseAddr, DmaDefaultCfg.TxDMABurstLength);
    AlGbe_ll_SetDmaTcpSegmentEnable(GbeBaseAddr, DmaDefaultCfg.TCPSegmentation);

    /* Config Dma channel RX control register */
    AlGbe_ll_SetDmaRxBurstLen(GbeBaseAddr, DmaDefaultCfg.RxDMABurstLength);
    AlGbe_ll_SetDmaRxPacketFlushEnable(GbeBaseAddr, DmaDefaultCfg.FlushRxPacket);
    AlGbe_ll_SetDmaRxBufferSize(GbeBaseAddr, ((Gbe->InitConfig.RxBuffLen) / 4));
}

/**
 * This function initializes the RX descriptor list with buffer addresses and sets up control bits
 * for each descriptor to prepare them for receiving packets. It also ensures that the buffer
 * addresses and counts are valid and asserts if any illegal parameters are detected.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param BuffersAddr Pointer to the start of the buffer addresses array.
 * @param BufferCnt Count of buffers to be configured.
 * @param BufferSize Size of each buffer.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_ConfigRxDescBuffer(AL_GBE_DevStruct *Gbe, AL_U8 *BuffersAddr, AL_U32 BufferCnt, AL_U32 BufferSize)
{
    AL_ASSERT((Gbe != AL_NULL) && (BuffersAddr != AL_NULL) && (BufferCnt == (AL_U32)AL_GBE_RX_DESC_CNT) &&
              (BufferSize == AL_GBE_TX_RX_BUFF_SIZE), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_U32 DescIndex = 0;
    AL_GBE_DMADescStruct *DmaRxDesc;

    for (DescIndex = 0; DescIndex < AL_GBE_RX_DESC_CNT; DescIndex++) {
        DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)(Gbe->RxDescList.RxDesc[DescIndex]));

        /* write buffer address to RDES0 */
        DmaRxDesc->DESC0 = (__IO AL_U32)((AL_UINTPTR)(BuffersAddr + (BufferSize * DescIndex)));
        /* store buffer address */
        DmaRxDesc->BackupAddr0 = (AL_U32)((AL_UINTPTR)(BuffersAddr + (BufferSize * DescIndex)));
        /* set buffer address valid bit to RDES3 */
        AlGbe_ll_SetRdesc3Buff1Valid((AL_REG)&(DmaRxDesc->DESC3), AL_GBE_FUNC_ENABLE);

        /* set OWN bit to RDES3 */
        AlGbe_ll_SetRdesc3OwnByDma((AL_REG)&(DmaRxDesc->DESC3), AL_GBE_FUNC_ENABLE);
    }

    return AL_OK;
}

/**
 * This function initializes the TX descriptor list with buffer addresses. It prepares the
 * descriptors for transmission by setting up buffer addresses in the descriptors. It also
 * performs parameter validation to ensure that the provided buffer addresses and counts are
 * within expected ranges.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param BuffersAddr Pointer to the start of the buffer addresses array.
 * @param BufferCnt Count of buffers to be configured.
 * @param BufferSize Size of each buffer.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_ConfigTxDescBuffer(AL_GBE_DevStruct *Gbe, AL_U8 *BuffersAddr, AL_U32 BufferCnt, AL_U32 BufferSize)
{
    AL_ASSERT((Gbe != AL_NULL) && (BuffersAddr != AL_NULL) && (BufferCnt == (AL_U32)AL_GBE_TX_DESC_CNT) &&
              (BufferSize == AL_GBE_TX_RX_BUFF_SIZE), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_U32 DescIndex = 0;
    AL_GBE_DMADescStruct *DmaTxDesc;

    for (DescIndex = 0; DescIndex < AL_GBE_TX_DESC_CNT; DescIndex++) {
        DmaTxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)(Gbe->TxDescList.TxDesc[DescIndex]));

        /* Store buffer1 address, buffer2 address not used */
        Gbe->TxDescList.BufferAddress[DescIndex] = BuffersAddr + (BufferSize * DescIndex);

        /* write buffer address to TDES0 */
        DmaTxDesc->DESC0 = (__IO AL_U32)((AL_UINTPTR)(BuffersAddr + (BufferSize * DescIndex)));
        /* store buffer address */
        DmaTxDesc->BackupAddr0 = (AL_U32)((AL_UINTPTR)(BuffersAddr + (BufferSize * DescIndex)));
    }

    return AL_OK;
}

/**
 * This function sets up the initial state of the RX descriptor list. It clears all descriptor
 * entries and configures the hardware with the base address of the descriptor list, among other
 * initial settings. It prepares the RX descriptor list to receive packets.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return None.
 */
static AL_VOID AlGbe_Dev_DMARxDescListInit(AL_GBE_DevStruct *Gbe)
{
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_GBE_DMADescStruct *DmaRxDescList = Gbe->InitConfig.RxDescList;
    AL_GBE_DMADescStruct *DmaRxDesc;
    AL_U32 Index;

    for (Index = 0; Index < AL_GBE_RX_DESC_CNT; Index++) {
        DmaRxDesc = DmaRxDescList + Index;

        DmaRxDesc->DESC0 = 0x0;
        DmaRxDesc->DESC1 = 0x0;
        DmaRxDesc->DESC2 = 0x0;
        DmaRxDesc->DESC3 = 0x0;
        DmaRxDesc->BackupAddr0 = 0x0;
        DmaRxDesc->BackupAddr1 = 0x0;

        /* Set Rx descritors addresses */
        Gbe->RxDescList.RxDesc[Index]= (AL_U32)((AL_UINTPTR)DmaRxDesc);
    }

    Gbe->RxDescList.CurRxDesc = 0;
    Gbe->RxDescList.FirstAppDesc = 0;
    Gbe->RxDescList.AppDescNbr = 0;
    Gbe->RxDescList.ItMode = 0;
    Gbe->RxDescList.AppContextDesc = 0;

    /* Set Receive Descriptor Ring Length */
    AlGbe_ll_SetDmaChannelRxDescRingLen(GbeBaseAddr, AL_GBE_RX_DESC_CNT - 1);

    /* Set Receive Descriptor List Address */
    AlGbe_ll_SetDmaChannelRxDescListAddr(GbeBaseAddr, (AL_U32)((AL_UINTPTR)DmaRxDescList));

    /* Set Receive Descriptor Tail pointer Address */
    AlGbe_ll_SetDmaRxDescTailPointer(GbeBaseAddr, (AL_U32)((AL_UINTPTR)(DmaRxDescList + (AL_U32)(AL_GBE_RX_DESC_CNT - 1U))));
}

/**
 * This function prepares the TX descriptor list for use. It clears all descriptor entries and
 * sets up the hardware with the necessary configuration for the TX descriptor list, including
 * the base address and ring length. It prepares the TX descriptor list for packet transmission.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return None.
 */
AL_VOID AlGbe_Dev_DMATxDescListInit(AL_GBE_DevStruct *Gbe)
{
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_GBE_DMADescStruct *DmaTxDescList = Gbe->InitConfig.TxDescList;
    AL_GBE_DMADescStruct *DmaTxDesc;
    AL_U32 Index;

    /* Fill each DMATxDesc descriptor with the right values */
    for (Index = 0; Index < (AL_U32)AL_GBE_TX_DESC_CNT; Index++) {
        DmaTxDesc = DmaTxDescList + Index;

        DmaTxDesc->DESC0 = 0x0;
        DmaTxDesc->DESC1 = 0x0;
        DmaTxDesc->DESC2 = 0x0;
        DmaTxDesc->DESC3 = 0x0;
        DmaTxDesc->BackupAddr0 = 0x0;
        DmaTxDesc->BackupAddr1 = 0x0;

        Gbe->TxDescList.TxDesc[Index] = (AL_U32)((AL_UINTPTR)DmaTxDesc);
    }

    Gbe->TxDescList.CurTxDesc = 0;

    /* Set Transmit Descriptor Ring Length */
    AlGbe_ll_SetDmaChannelTxDescRingLen(GbeBaseAddr, AL_GBE_TX_DESC_CNT - 1);

    /* Set Transmit Descriptor List Address */
    AlGbe_ll_SetDmaChannelTxDescListAddr(GbeBaseAddr, (AL_U32)((AL_UINTPTR)DmaTxDescList));

    /* Set Transmit Descriptor Tail pointer */
    AlGbe_ll_SetDmaTxDescTailPointer(GbeBaseAddr,  (AL_U32)((AL_UINTPTR)DmaTxDescList));
}

/**
 *
 * This function sets the duplex mode and speed for the GBE device based on the configuration
 * provided in the Gbe structure. It also configures the GBE control register based on the PHY ID
 * and speed settings. If the media interface is RGMII, it sets the GBE control register with the
 * appropriate phase settings for the PHY.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return AL_OK on success, error code otherwise.
 * @param Gbe Pointer to the GBE device structure.
 * @return None.
 */
AL_S32 AlGbe_Dev_ConfigDuplexAndSpeed(AL_GBE_DevStruct *Gbe)
{
    AL_ASSERT((Gbe != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_U32 GbeTopSetting;
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    AlGbe_ll_SetDuplexMode(GbeBaseAddr, Gbe->MacDmaConfig.DuplexMode);
    AlGbe_ll_SetSpeed(GbeBaseAddr, Gbe->MacDmaConfig.Speed);

    if (Gbe->InitConfig.MediaInterface == AL_GBE_RGMII_MODE) {
        if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_100M) {
            if (Gbe->PhyId == PHY_ID_RTL8211F) {
                GbeTopSetting = PHY_RTL8211F_PHASE_100M;
            } else if (Gbe->PhyId == PHY_ID_YT8531) {
                GbeTopSetting = PHY_YT8531_PHASE_100M;
            }
        } else {
            if (Gbe->PhyId == PHY_ID_RTL8211F) {
                GbeTopSetting = PHY_RTL8211F_PHASE_1000M;
            } else if (Gbe->PhyId == PHY_ID_YT8531) {
                GbeTopSetting = PHY_YT8531_PHASE_1000M;
            }
        }
        AlGbe_ll_SetGbeCtlRegister((GbeBaseAddr == GBE0__BASE_ADDR ? CFG_CTRL_GBE0_ADDR : CFG_CTRL_GBE1_ADDR), GbeTopSetting);
    } else {
        //Todo:
    }

    return AL_OK;
}

/**
 *
 * This function validates the configuration of the GBE device. It ensures that the transmit and
 * receive descriptor lists are not NULL and that the receive buffer length is a multiple of 4,
 * which is required for a 32-bit data bus width.
 *
 * @param InitConfig Pointer to the GBE initialization configuration structure.
 * @param MacDmaConfig Pointer to the GBE MAC DMA configuration structure.
 * @return AL_OK if the configuration is valid, error code otherwise.
 */
AL_S32 AlGbe_Dev_CheckConfig(AL_GBE_InitStruct *InitConfig, AL_GBE_MacDmaConfigStruct *MacDmaConfig)
{
    AL_ASSERT((InitConfig->TxDescList != AL_NULL) && (InitConfig->RxDescList != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    /*
      The buffer size must be a multiple of 4, 8, or 16
      depending on the data bus widths (32-bit, 64-bit, or 128-bit
      respectively). data bus widths is 32-bit.
     */
    AL_ASSERT(!(InitConfig->RxBuffLen % 4), AL_GBE_ERR_CONFIG_ERROR);

    return AL_OK;
}

/**
 *
 * This function initializes the GBE device with the provided hardware, initialization, and MAC DMA
 * configurations. It performs a series of steps including checking the configuration, setting the
 * control register, performing a software reset, configuring the clock range, MAC, DMA, and the
 * MAC address. It also initializes the DMA descriptor lists in ring mode.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param HwConfig Pointer to the GBE hardware configuration structure.
 * @param InitConfig Pointer to the GBE initialization configuration structure.
 * @param MacDmaConfig Pointer to the GBE MAC DMA configuration structure.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_Init(AL_GBE_DevStruct *Gbe, AL_GBE_HwConfigStruct *HwConfig,
                      AL_GBE_InitStruct *InitConfig, AL_GBE_MacDmaConfigStruct *MacDmaConfig)
{
    AL_S32 Ret;
    AL_REG GbeBaseAddr;
    AL_U32 MacAddrLow;
    AL_U16 MacAddrHigh;
    AL_U32 GbeTopSetting;

    AL_ASSERT((Gbe != AL_NULL) && (HwConfig != AL_NULL) && (InitConfig != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    Ret = AlGbe_Dev_CheckConfig(InitConfig, MacDmaConfig);
    if (Ret != AL_OK) {
        return Ret;
    }

    Gbe->HwConfig = *HwConfig;
    Gbe->InitConfig = *InitConfig;
    Gbe->MacDmaConfig = *MacDmaConfig;
    GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    /* Config gbe control register */
    if (Gbe->InitConfig.MediaInterface == AL_GBE_RGMII_MODE) {
        if (MacDmaConfig->Speed == AL_GBE_SPEED_100M) {
            GbeTopSetting = PHY_DEFAULT_PHASE_100M;
        } else {
            GbeTopSetting = PHY_DEFAULT_PHASE_1000M;
        }
        AlGbe_ll_SetGbeCtlRegister((GbeBaseAddr == GBE0__BASE_ADDR ? CFG_CTRL_GBE0_ADDR : CFG_CTRL_GBE1_ADDR), GbeTopSetting);
    } else {
        //Todo:
    }

    /* Provide a software reset and wait for the completion of the reset process */
    AlGbe_ll_ResetMac(GbeBaseAddr);
    while(AlGbe_ll_GetMacResetStatus(GbeBaseAddr) != AL_GBE_FUNC_DISABLE);

    /* Config Clock range */
    AlGbe_Dev_SetCsrClockRange(Gbe);

    /* Config MAC and DMA */
    AlGbe_Dev_SetMacConfig(Gbe);
    AlGbe_Dev_SetDmaConfig(Gbe);

    /* Config DMA descriptors, in ring mode */
    AlGbe_Dev_DMATxDescListInit(Gbe);
    AlGbe_Dev_DMARxDescListInit(Gbe);
    AlGbe_ll_SetDescSkipLen(GbeBaseAddr, AL_GBE_DESC_SKIP_LEN_64BIT);

    /* Config MAC Address */
    MacAddrLow = (((AL_U32)(Gbe->InitConfig.MacAddr[3]) << 24) | ((AL_U32)(Gbe->InitConfig.MacAddr[2]) << 16) |
                 ((AL_U32)(Gbe->InitConfig.MacAddr[1])  << 8 ) | (AL_U32)Gbe->InitConfig.MacAddr[0]);
    MacAddrHigh = (((AL_U16)(Gbe->InitConfig.MacAddr[5]) << 8) | (AL_U16)Gbe->InitConfig.MacAddr[4]);

    AlGbe_ll_SetMacAddrLow(GbeBaseAddr, MacAddrLow);
    AlGbe_ll_SetMacAddrHigh(GbeBaseAddr, MacAddrHigh);

#ifdef ENABLE_MMC_COUNT
    AlGbe_ll_SetMmcCntFreez(GbeBaseAddr, AL_FUNC_DISABLE);
#else
    AlGbe_ll_SetMmcCntFreez(GbeBaseAddr, AL_FUNC_ENABLE);
#endif

    Gbe->State |= AL_GBE_STATE_READY;

    return AL_OK;
}

/**
 *
 * This function writes a specified value to a PHY register. It checks if the GMII interface is
 * busy before proceeding with the write operation. It prepares the MDIO Address Register value,
 * writes the value to the MII data register, and then writes the result into the MII Address
 * register. It waits for the GMII interface to become available before completing.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param PHYAddr The PHY device address.
 * @param PHYReg The PHY register address.
 * @param RegValue The value to write to the PHY register.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_WritePhyRegister(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddr, AL_U32 PHYReg, AL_U16 RegValue)
{
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_U32 TmpReg;

    /* Check for the Busy flag */
    if (AlGbe_ll_IsGmiiBusy(GbeBaseAddr)) {
        return AL_GBE_ERR_PHY_BUSY;
    }

    /* Get the  MACMDIOAR value */
    TmpReg = AlGbe_ll_ReadMdioAddr(GbeBaseAddr);

    /*
       Prepare the MDIO Address Register value
     - Set the PHY device address
     - Set the PHY register address
     - Set the write mode
     - Set the MII Busy bit
    */
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__PA__SHIFT, GBE__MAC_MDIO_ADDRESS__PA__SIZE, PHYAddr);
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__RDA__SHIFT, GBE__MAC_MDIO_ADDRESS__RDA__SIZE, PHYReg);
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__GOC_0__SHIFT, GBE__MAC_MDIO_ADDRESS__GOC__SIZE, AL_GBE_GMII_CMD_WRITE);
    AL_REG32_SET_BIT(&TmpReg, GBE__MAC_MDIO_ADDRESS__GB__SHIFT, AL_GBE_FUNC_ENABLE);

    /* Give the value to the MII data register */
    AlGbe_ll_WriteMdioGmiiData(GbeBaseAddr, (AL_U16)RegValue);

    /* Write the result value into the MII Address register */
    AlGbe_ll_WriteMdioAddr(GbeBaseAddr, TmpReg);

    /* Wait for the Busy flag */
    while (AlGbe_ll_IsGmiiBusy(GbeBaseAddr)) {
        //ToDo: TimeOut
    }

  return AL_OK;
}

/**
 *
 * This function reads a value from a specified PHY register. It checks if the GMII interface is
 * busy before proceeding with the read operation. It prepares the MDIO Address Register value,
 * writes the result into the MII Address register, and waits for the GMII interface to become
 * available. It then reads the value from the MII data register.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param PHYAddr The PHY device address.
 * @param PHYReg The PHY register address.
 * @param RegValue Pointer to store the read value from the PHY register.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_ReadPhyRegister(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddr, AL_U32 PHYReg, AL_U16 *RegValue)
{
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_U32 TmpReg;

    /* Check for the Busy flag */
    if (AlGbe_ll_IsGmiiBusy(GbeBaseAddr)) {
        return AL_GBE_ERR_PHY_BUSY;
    }

    /* Get the  MACMDIOAR value */
    TmpReg = AlGbe_ll_ReadMdioAddr(GbeBaseAddr);

    /*
       Prepare the MDIO Address Register value
     - Set the PHY device address
     - Set the PHY register address
     - Set the read mode
     - Set the MII Busy bit
    */
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__PA__SHIFT, GBE__MAC_MDIO_ADDRESS__PA__SIZE, PHYAddr);
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__RDA__SHIFT, GBE__MAC_MDIO_ADDRESS__RDA__SIZE, PHYReg);
    AL_REG32_SET_BITS(&TmpReg, GBE__MAC_MDIO_ADDRESS__GOC_0__SHIFT, GBE__MAC_MDIO_ADDRESS__GOC__SIZE, AL_GBE_GMII_CMD_READ);
    AL_REG32_SET_BIT(&TmpReg, GBE__MAC_MDIO_ADDRESS__GB__SHIFT, AL_GBE_FUNC_ENABLE);

    /* Write the result value into the MII Address register */
    AlGbe_ll_WriteMdioAddr(GbeBaseAddr, TmpReg);

    /* Wait for the Busy flag */
    while (AlGbe_ll_IsGmiiBusy(GbeBaseAddr)) {
        //ToDo: TimeOut
    }

    /* Get MACMIIDR value */
    *RegValue = (AL_U16)AlGbe_ll_ReadMdioGmiiData(GbeBaseAddr);

    return AL_OK;
}

/**
 * This function performs several operations to initialize the PHY device, including reading the PHY ID,
 * resetting the PHY, configuring RGMII delays, setting up auto-negotiation, and enabling or disabling
 * 1000BaseT full duplex mode based on the MAC DMA configuration. It handles different PHY models by checking
 * the PHY ID and applies specific initialization sequences for each. The function also ensures that the PHY
 * is reset properly and waits for auto-negotiation to complete if applicable.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param PHYAddress The address of the PHY device to initialize.
 * @return AL_OK on success, or an error code on failure.
 *
 * @retval AL_GBE_ERR_ILLEGAL_PARAM Indicates an illegal parameter error.
 * @retval AL_GBE_ERR_PHY_RESET_FAILED Indicates that resetting the PHY device failed.
 */
AL_S32 AlGbe_Dev_PhyInit(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddress)
{
    AL_S32 Ret = 0;
    AL_U16 RegValue;
    AL_U16 PhyId1, PhyId2;

    AL_ASSERT((Gbe != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_ID1_REG, &PhyId1);
    if (Ret != AL_OK ) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return AL_GBE_ERR_PHY_RESET_FAILED;
    }

    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_ID2_REG, &PhyId2);
    if (Ret != AL_OK ) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
        return AL_GBE_ERR_PHY_RESET_FAILED;
    }

    Gbe->PhyId = ((PhyId1 & 0xffff) << 16) | (PhyId2 & 0xffff);
    AL_LOG(AL_LOG_LEVEL_INFO, "AlGbe get phy id : 0x%x \r\n", Gbe->PhyId);

    if (Gbe->PhyId == PHY_ID_RTL8211F) {
        /* Reset the Phy */
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_REG, PHY_BCTL_RESET_MASK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        /* Delay at least 10 ms */
        AlSys_MDelay(500);

        /* Check reset status complete */
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BCR_REG, &RegValue);
        if ((Ret != AL_OK) || (RegValue & PHY_BCTL_RESET_MASK)) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return AL_GBE_ERR_PHY_RESET_FAILED;
        }

        /* The RGMII specifies output TXC/RXC and TXD/RXD without any clock skew. Need to add skew on clock line
        to make sure the other side sample right data. This can also be done in PCB traces. */
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_PAGE_SELECT_REG, PHY_PAGE_RGMII_TXRX_DELAY_ADDR);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        /* Set Tx Delay */
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_RGMII_TX_DELAY_REG, &RegValue);
        if (Ret == AL_OK) {
            RegValue = 0x9; //Todo: There needs to be a reasonable explanation
            Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_RGMII_TX_DELAY_REG, RegValue);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
                return Ret;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        /* Set Rx Delay */
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_RGMII_RX_DELAY_REG, &RegValue);
        if (Ret == AL_OK) {
            RegValue |= PHY_RGMII_RX_DELAY_MASK;
            Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_RGMII_RX_DELAY_REG, RegValue);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
                return Ret;
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        /* Restore to default page 0 */
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_PAGE_SELECT_REG, 0x0);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        /* Set the auto-negotiation. */
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_AUTONEG_ADVERTISE_REG,
                                        PHY_100BASETX_FULLDUPLEX_MASK | PHY_100BASETX_HALFDUPLEX_MASK |
                                        PHY_10BASETX_FULLDUPLEX_MASK | PHY_10BASETX_HALFDUPLEX_MASK |
                                        PHY_IEEE802_3_SELECTOR_MASK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        AL_U16 Phy1000BaseTFullDuplex = PHY_1000BASET_FULLDUPLEX_MASK;
        if (Gbe->MacDmaConfig.Speed != AL_GBE_SPEED_1G) {
            Phy1000BaseTFullDuplex = 0;
        }

        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_1000BASET_CONTROL_REG, Phy1000BaseTFullDuplex);
        if (Ret == AL_OK) {
            Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BCR_REG, &RegValue);
            if (Ret == AL_OK) {
                Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_REG,
                                             (RegValue | PHY_BCTL_AUTONEG_MASK | PHY_BCTL_RESTART_AUTONEG_MASK));
            } else {
                AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            }
        } else {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }
    } else if (Gbe->PhyId == PHY_ID_YT8531) {
        /* Reset the Phy */
        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_REG, PHY_BCTL_RESET_MASK);
        if (Ret != AL_OK) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%x\r\n", Ret);
            return Ret;
        }

        /* Delay at least 10 ms */
        AlSys_MDelay(500);

        /* Check reset status complete */
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BCR_REG, &RegValue);
        if ((Ret != AL_OK) || (RegValue & PHY_BCTL_RESET_MASK)) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
            return AL_GBE_ERR_PHY_RESET_FAILED;
        }

        AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_AUTONEGO_ADVERTISE_REG, &RegValue);
        RegValue |= IEEE_ASYMMETRIC_PAUSE_MASK;
        RegValue |= IEEE_PAUSE_MASK;
        RegValue |= ADVERTISE_100;
        RegValue |= ADVERTISE_10;
        AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_AUTONEGO_ADVERTISE_REG, RegValue);

        AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_1000_ADVERTISE_REG_OFFSET,
                        &RegValue);
        RegValue |= ADVERTISE_1000;
        AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_1000_ADVERTISE_REG_OFFSET,
                        RegValue);

        AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, &RegValue);
        RegValue |= IEEE_CTRL_AUTONEGOTIATE_ENABLE;
        RegValue |= IEEE_STAT_AUTONEGOTIATE_RESTART;
        AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, RegValue);

        AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, &RegValue);
        RegValue |= IEEE_CTRL_RESET_MASK;
        AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, RegValue);

        while (1) {
            AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_CONTROL_REG_OFFSET, &RegValue);
            if (RegValue & IEEE_CTRL_RESET_MASK)
                continue;
            else
             break;
        }

        AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_STATUS_REG_OFFSET, &RegValue);

        while ( !(RegValue & IEEE_STAT_AUTONEGOTIATE_COMPLETE) ) {
            AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, IEEE_STATUS_REG_OFFSET, &RegValue);
        }
    }

    return AL_OK;
}

/**
 * This function queries the PHY link status, including speed and duplex mode, for a specified PHY address.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param PHYAddress The address of the PHY to query.
 * @param Speed Pointer to a variable where the link speed will be stored.
 * @param Duplex Pointer to a variable where the duplex mode will be stored.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlGbe_Dev_GetPhyLinkStatus(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddress, AL_U8 *Speed, AL_U8 *Duplex)
{
    AL_S32 Ret;
    AL_U16 RegValue;

    AL_ASSERT((Gbe != AL_NULL) && (Speed != AL_NULL) && (Duplex != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    if (Gbe->PhyId == PHY_ID_RTL8211F) {
        do {
            Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_SSR_REG, &RegValue);
            if (Ret != AL_OK) {
                AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%x\r\n", Ret);
                return Ret;
            }

            //Todo: Timeout
        } while(((PHY_SSTATUS_LINKSTATUS_MASK & RegValue) == 0));

        switch ((RegValue & PHY_SSTATUS_LINKSPEED_MASK) >> PHY_SSTATUS_LINKSPEED_SHIFT)
        {
        case (AL_U32)PHY_SPEED_10M:
            *Speed = PHY_SPEED_10M;
            break;
        case (AL_U32)PHY_SPEED_100M:
           *Speed = PHY_SPEED_100M;
            break;
        case (AL_U32)PHY_SPEED_1000M:
            *Speed = PHY_SPEED_1000M;
            break;
        default:
            *Speed = PHY_SPEED_10M;
            break;
        }

        if ((RegValue & PHY_SSTATUS_LINKDUPLEX_MASK) != 0U) {
            *Duplex = PHY_FULL_DUPLEX;
        } else {
            *Duplex = PHY_HALF_DUPLEX;
        }
    } else if (Gbe->PhyId == PHY_ID_YT8531) {
        AL_U16 TempSpeed;

        AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress,0X11,&RegValue);
        TempSpeed = RegValue >> 14;

        switch (TempSpeed)
        {
        case (AL_U32)PHY_SPEED_10M:
            *Speed = PHY_SPEED_10M;
            break;
        case (AL_U32)PHY_SPEED_100M:
            *Speed = PHY_SPEED_100M;
            break;
        case (AL_U32)PHY_SPEED_1000M:
            *Speed = PHY_SPEED_1000M;
            break;
        default:
            *Speed = PHY_SPEED_10M;
            break;
        }

        if ((RegValue & 0x2000) != 0U) {
            *Duplex = PHY_FULL_DUPLEX;
        } else {
            *Duplex = PHY_HALF_DUPLEX;
        }
    }

    return AL_OK;
}

/**
 * This function initializes and starts the MAC DMA and interrupt handling.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlGbe_Dev_StartMacDmaIntr(AL_GBE_DevStruct *Gbe)
{
    AL_ASSERT((Gbe != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);
    AL_ASSERT(((Gbe->State & AL_GBE_STATE_READY) == AL_GBE_STATE_READY), AL_GBE_ERR_NOT_READY);

    AL_GBE_DMADescStruct *DmaRxDesc;

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    /* save IT mode to ETH Handle */
    Gbe->RxDescList.ItMode = 1U;

    /* Set IOC bit to all Rx descriptors */
    for (AL_S32 DescIndex = 0; DescIndex < AL_GBE_RX_DESC_CNT; DescIndex++) {
        DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)(Gbe->RxDescList.RxDesc[DescIndex]));
        AlGbe_ll_SetRdesc3CompleteIntr((AL_REG)&(DmaRxDesc->DESC3), AL_GBE_FUNC_ENABLE);
    }

    /* Enable the MAC transmission */
    AlGbe_ll_SetTransEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Enable the MAC reception */
    AlGbe_ll_SetRecvEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Set the Flush Transmit FIFO bit */
    AlGbe_ll_SetMtlTxQueueFlushEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Enable the DMA transmission */
    AlGbe_ll_SetDmaStartTransEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Enable the DMA reception */
    AlGbe_ll_SetDmaStartRecvEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /*
      Enable ETH DMA interrupts:
    - Tx complete interrupt
    - Rx complete interrupt
    - Fatal bus interrupt
    */
    AlGbe_ll_SetDmaTransIntrEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
    AlGbe_ll_SetDmaTransBufferUnavailableIntrEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
    AlGbe_ll_SetDmaRecvIntrEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
    AlGbe_ll_SetDmaRecvBufferUnavailableIntrEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
    AlGbe_ll_SetDmaFatalBusErrorIntrEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
    AlGbe_ll_SetDmaAbnormalSummaryIntrEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
    AlGbe_ll_SetDmaNormalSummaryIntrEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function starts the MAC DMA for transmission and reception.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlGbe_Dev_StartMacDma(AL_GBE_DevStruct *Gbe)
{
    AL_ASSERT((Gbe != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);
    AL_ASSERT(((Gbe->State & AL_GBE_STATE_READY) == AL_GBE_STATE_READY), AL_GBE_ERR_NOT_READY);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    /* Enable the MAC transmission */
    AlGbe_ll_SetTransEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Enable the MAC reception */
    AlGbe_ll_SetRecvEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Set the Flush Transmit FIFO bit */
    AlGbe_ll_SetMtlTxQueueFlushEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Enable the DMA transmission */
    AlGbe_ll_SetDmaStartTransEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Enable the DMA reception */
    AlGbe_ll_SetDmaStartRecvEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function retrieves the receive timestamp for a specified descriptor index.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param DescIndex The index of the descriptor to retrieve the timestamp for.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlGbe_Dev_GetRxTimeStamp(AL_GBE_DevStruct *Gbe, AL_U32 DescIndex)
{
    AL_GBE_RxDescListStruct *DmaRxDescList = &Gbe->RxDescList;
    AL_GBE_DMADescStruct *DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);

    if (!AlGbe_ll_GetWbRecvContextDesc((AL_REG)&DmaRxDesc->DESC3)) {
        return AL_GBE_ERR_DESC_STATE;
    }

    /* Get timestamp high */
    Gbe->RxDescList.RxTimeStamp.TimeStampHigh = DmaRxDesc->DESC1;
    /* Get timestamp low */
    Gbe->RxDescList.RxTimeStamp.TimeStampLow  = DmaRxDesc->DESC0;

    DmaRxDesc->DESC0 = DmaRxDesc->BackupAddr0;

    AlGbe_ll_ClearWbRecvContextDesc((AL_REG)&DmaRxDesc->DESC3);

    return AL_OK;
}

/**
 * This function checks the timestamp status of a descriptor.
 *
 * @param Desc Pointer to the descriptor to check.
 * @return Returns AL_OK if a valid timestamp is ready to be read,
 *         AL_GBE_DESC_TX_TIMESTAMP_STATUS_ERROR if the timestamp is corrupted,
 *         or AL_GBE_DESC_RX_TIMESTAMP_STATUS_NOT_READY if the timestamp is not ready.
 */
static AL_S32 AlGbe_Dev_CheckTimestamp(AL_VOID *Desc)
{
    AL_S32 Ret = AL_GBE_DESC_RX_TIMESTAMP_STATUS_NOT_READY;

    AL_GBE_DMADescStruct *CurrentDesc = (AL_GBE_DMADescStruct *)Desc;
    AL_U32 Rdes0 = CurrentDesc->DESC0;
    AL_U32 Rdes1 = CurrentDesc->DESC1;

    if (!AlGbe_ll_IsWbRxDescOwnByDma((AL_REG)&(CurrentDesc->DESC3)) &&
        AlGbe_ll_GetWbRecvContextDesc((AL_REG)&(CurrentDesc->DESC3))) {

        if ((Rdes0 == 0xffffffff) && (Rdes1 == 0xffffffff))
            /* Corrupted value */
            Ret = AL_GBE_DESC_TX_TIMESTAMP_STATUS_ERROR;
        else
            /* A valid Timestamp is ready to be read */
            Ret = AL_OK;
    }

    /* Timestamp not ready */
    return Ret;
}

/**
 * This function gets the receive timestamp status for a descriptor, checking if a valid timestamp is available.
 *
 * @param Desc Pointer to the current descriptor.
 * @param NextDesc Pointer to the next descriptor in the chain.
 * @return Returns AL_OK if a valid timestamp is available,
 *         AL_GBE_DESC_RX_TIMESTAMP_STATUS_NOT_READY if the timestamp is not ready,
 *         or AL_GBE_DESC_TX_TIMESTAMP_STATUS_ERROR if an error occurred.
 */
static AL_S32 AlGbe_Dev_GetRxTimestampStatus(AL_VOID *Desc, AL_VOID *NextDesc)
{
     AL_GBE_DMADescStruct *CurrentDesc = (AL_GBE_DMADescStruct *)Desc;
     AL_S32 Ret = AL_GBE_DESC_RX_TIMESTAMP_STATUS_NOT_READY;

     /* Get the status from normal w/b descriptor */
    if (AlGbe_ll_IsWbRxDesc3StatusValid((AL_REG)&(CurrentDesc->DESC3))) {
        if (AlGbe_ll_IsWbRxDesc1TimeStampAvaliable((AL_REG)&(CurrentDesc->DESC1))) {
            AL_S32 Index = 0;

            /* Check if timestamp is OK from context descriptor */
            do {
                Ret = AlGbe_Dev_CheckTimestamp(NextDesc);
                if (Ret == AL_GBE_DESC_RX_TIMESTAMP_STATUS_ERROR)
                    return Ret;
                Index++;

            } while ((Ret == AL_GBE_DESC_RX_TIMESTAMP_STATUS_NOT_READY) && (Index < 10));

            if (Index == 10)
                Ret = AL_GBE_DESC_RX_TIMESTAMP_STATUS_NOT_READY;
        }
    }

    return Ret;
}

/**
 *
 * This function iterates through the RX descriptor list to determine if there are any packets
 * that have been received but not yet processed by the application. It updates the descriptor
 * list with the index and count of descriptors that contain unprocessed received data.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return Returns 1 if unprocessed received data is available, 0 if all received data has been processed, or AL_OK if no data is received.
 * @param None
 */
AL_S32 AlGbe_Dev_IsRxDataAvailable(AL_GBE_DevStruct *Gbe)
{
    AL_GBE_RxDescListStruct *DmaRxDescList = &Gbe->RxDescList;
    AL_U32 DescIndex = DmaRxDescList->CurRxDesc;
    AL_GBE_DMADescStruct *DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);
    AL_U32 Descscancnt = 0;
    AL_U32 Appdesccnt = 0, FirstAppDescidx = 0;
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    AL_GBE_DMADescStruct *CurrentDmaRxDesc;

    if (DmaRxDescList->AppDescNbr != 0U) {
        /* data already received by not yet processed*/
        return 0;
    }

    /* Check if descriptor is not owned by DMA */
    while ((!AlGbe_ll_IsWbRxDescOwnByDma((AL_REG)&DmaRxDesc->DESC3)) && (Descscancnt < (AL_U32)AL_GBE_RX_DESC_CNT)) {
        Descscancnt++;

        /* Check if last descriptor */
        if (AlGbe_ll_IsWbRxDescLastDesc((AL_REG)&DmaRxDesc->DESC3)) {
            /* Increment the number of descriptors to be passed to the application */
            Appdesccnt += 1U;

            if (Appdesccnt == 1U) {
                FirstAppDescidx = DescIndex;
            }

            /* Increment current rx descriptor index */
            INCR_RX_DESC_INDEX(DescIndex, 1U);

            CurrentDmaRxDesc = DmaRxDesc;

            /* Check for Context descriptor */
            /* Get current descriptor address */
            DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);

            if (AlGbe_Dev_GetRxTimestampStatus(CurrentDmaRxDesc, DmaRxDesc) == AL_OK) {
                AlGbe_Dev_GetRxTimeStamp(Gbe, DescIndex);

                /* Increment the number of descriptors to be passed to the application */
                DmaRxDescList->AppContextDesc = 1;
                /* Increment current rx descriptor index */
                INCR_RX_DESC_INDEX(DescIndex, 1U);
            }

            /* Fill information to Rx descriptors list */
            DmaRxDescList->CurRxDesc = DescIndex;
            DmaRxDescList->FirstAppDesc = FirstAppDescidx;
            DmaRxDescList->AppDescNbr = Appdesccnt;

            /* Return function status */
            return 1;

        } else if (AlGbe_ll_IsWbRxDescFirstDesc((AL_REG)&DmaRxDesc->DESC3)) {
            /* Check if first descriptor */
            FirstAppDescidx = DescIndex;
            /* Increment the number of descriptors to be passed to the application */
            Appdesccnt = 1U;

            /* Increment current rx descriptor index */
            INCR_RX_DESC_INDEX(DescIndex, 1U);
            /* Get current descriptor address */
            DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);

        } else {
            /* It should be an intermediate descriptor */
            /* Increment the number of descriptors to be passed to the application */
            Appdesccnt += 1U;

            if (!AlGbe_ll_GetWbRecvContextDesc((AL_REG)&DmaRxDesc->DESC3)) {
                /* Increment current rx descriptor index */
                INCR_RX_DESC_INDEX(DescIndex, 1U);
                /* Get current descriptor address */
                DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);
            }
        }
    }

    /* Build Descriptors if an incomplete Packet is received */
    if (Appdesccnt > 0U) {
        DmaRxDescList->CurRxDesc = DescIndex;
        DmaRxDescList->FirstAppDesc = FirstAppDescidx;
        DescIndex = FirstAppDescidx;
        DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);

        for (Descscancnt = 0; Descscancnt < Appdesccnt; Descscancnt++) {
            DmaRxDesc->DESC0 = DmaRxDesc->BackupAddr0;
            AlGbe_ll_SetRdesc3Buff1Valid((AL_REG)&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);

            if (DmaRxDesc->BackupAddr1 != 0) {
                DmaRxDesc->DESC2 = DmaRxDesc->BackupAddr1;
                AlGbe_ll_SetRdesc3Buff2Valid((AL_REG)&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);
            }

            AlGbe_ll_SetRdesc3OwnByDma((AL_REG)&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);

            if (DmaRxDescList->ItMode != 0) {
                AlGbe_ll_SetRdesc3CompleteIntr((AL_REG)&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);
            }
            if (Descscancnt < (Appdesccnt - 1U)) {
                /* Increment rx descriptor index */
                INCR_RX_DESC_INDEX(DescIndex, 1U);
                /* Get descriptor address */
                DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);
            }
        }

        /* Set the Tail pointer address to the last rx descriptor hold by the app */
        AlGbe_ll_SetDmaRxDescTailPointer(GbeBaseAddr, (AL_U32)((AL_UINTPTR)DmaRxDesc));
    }

    /* Fill information to Rx descriptors list: No received Packet */
    DmaRxDescList->AppDescNbr = 0U;

    return AL_OK;
}

/**
 *
 * This function is called after determining that received data is available. It compiles the
 * list of buffers that make up the received packet. This may involve multiple descriptors if
 * the packet is split across them. The function updates the provided RxBuffer structure with
 * the addresses and lengths of the buffers that contain the packet data.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param RxBuffer Pointer to the structure where the buffer information will be stored.
 * @return Returns AL_OK on success, or AL_GBE_ERR_FATLA_BUS_ERROR if no valid data is found.
 * @param None
 */
AL_S32 AlGbe_Dev_GetRxDataBuffer(AL_GBE_DevStruct *Gbe, AL_GBE_BufferStruct *RxBuffer)
{
    AL_GBE_RxDescListStruct *DmaRxDescList = &Gbe->RxDescList;
    AL_U32 DescIndex = DmaRxDescList->FirstAppDesc;
    AL_U32 Index, AccumulatedLen = 0, LastDescLen;
    __IO const AL_GBE_DMADescStruct *DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);
    AL_GBE_BufferStruct *RxBuff = RxBuffer;

    AL_ASSERT((Gbe != AL_NULL) && (RxBuff != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    if (DmaRxDescList->AppDescNbr == 0U) {
        if (AlGbe_Dev_IsRxDataAvailable(Gbe) == 0U) {
        /* No data to be transferred to the application */
            return AL_GBE_ERR_FATLA_BUS_ERROR;
        } else {
            DescIndex = DmaRxDescList->FirstAppDesc;
            DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);
        }
    }

    /* Get intermediate descriptors buffers: in case of the Packet is split into multi descriptors */
    for (Index = 0; Index < (DmaRxDescList->AppDescNbr - 1U); Index++) {
        /* Get Address and length of the first buffer address */
        RxBuff->Buffer = (AL_U8 *)((AL_UINTPTR)DmaRxDesc->BackupAddr0);
        RxBuff->Len =  Gbe->InitConfig.RxBuffLen;

        /* Check if the second buffer address of this descriptor is valid */
        if (DmaRxDesc->BackupAddr1 != 0U) {
            /* Point to next buffer */
            RxBuff = RxBuff->next;
            /* Get Address and length of the second buffer address */
            RxBuff->Buffer = (AL_U8 *)((AL_UINTPTR)DmaRxDesc->BackupAddr1);
            RxBuff->Len =  Gbe->InitConfig.RxBuffLen;
        } else {
            /* Nothing to do here */
        }

        /* get total length until this descriptor */
        AccumulatedLen = AlGbe_ll_GetWbRdesc3Packetlen((AL_REG)&DmaRxDesc->DESC3);

        /* Increment to next descriptor */
        INCR_RX_DESC_INDEX(DescIndex, 1U);
        DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);

        /* Point to next buffer */
        RxBuff = RxBuff->next;
    }

    /* last descriptor data length */
    LastDescLen = AlGbe_ll_GetWbRdesc3Packetlen((AL_REG)&DmaRxDesc->DESC3) - AccumulatedLen;

    /* Get Address of the first Buffer address */
    RxBuff->Buffer = (AL_U8 *)((AL_UINTPTR)DmaRxDesc->BackupAddr0);

    /* data is in only one Buffer */
    if (LastDescLen <= Gbe->InitConfig.RxBuffLen) {
        RxBuff->Len = LastDescLen;
    } else if (DmaRxDesc->BackupAddr1 != 0U) {
        /* data is in two Buffers */
        /* Get the Length of the first Buffer address */
        RxBuff->Len = Gbe->InitConfig.RxBuffLen;
        /* Point to next Buffer */
        RxBuff = RxBuff->next;
        /* Get the Address the Length of the second Buffer address */
        RxBuff->Buffer = (AL_U8 *)((AL_UINTPTR)DmaRxDesc->BackupAddr1);
        RxBuff->Len =  LastDescLen - (Gbe->InitConfig.RxBuffLen);
    } else {
        /* Buffer 2 not valid*/
        return AL_GBE_ERR_FATLA_BUS_ERROR;
    }

    return AL_OK;
}

/**
 *
 * This function calculates the length of the data that has been received and is ready to be processed.
 * It checks if there is any data available to be read. If there is no data available, it returns an error.
 * Otherwise, it calculates the length of the data based on the last descriptor's information.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param Length Pointer to store the length of the received data.
 * @return Returns AL_OK on success, AL_GBE_ERR_ILLEGAL_PARAM if any parameter is NULL, or AL_GBE_ERR_FATLA_BUS_ERROR if no data is available.
 */
AL_S32 AlGbe_Dev_GetRxDataLength(AL_GBE_DevStruct *Gbe, AL_U32 *Length)
{
    AL_GBE_RxDescListStruct *DmaRxDescList = &Gbe->RxDescList;
    AL_U32 DescIndex = DmaRxDescList->FirstAppDesc;
    __IO const AL_GBE_DMADescStruct *DmaRxDesc;

    AL_ASSERT((Gbe != AL_NULL) && (Length != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    if (DmaRxDescList->AppDescNbr == 0U) {
        if (AlGbe_Dev_IsRxDataAvailable(Gbe) == 0U) {
            /* No data to be transferred to the application */
            return AL_GBE_ERR_FATLA_BUS_ERROR;
        }
    }

    /* Get index of last descriptor */
    INCR_RX_DESC_INDEX(DescIndex, (DmaRxDescList->AppDescNbr - 1U));
    /* Point to last descriptor */
    DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);

    *Length = AlGbe_ll_GetWbRdesc3Packetlen((AL_REG)&DmaRxDesc->DESC3);

    return AL_OK;
}

/**
 *
 * This function prepares the receive descriptors for incoming data. It initializes each descriptor with
 * the backup address and sets the valid and ownership bits accordingly. If interrupt mode is enabled,
 * it also configures the complete interrupt for each descriptor. Finally, it resets the application descriptor
 * count and context descriptor to indicate that the descriptors are ready for use.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return Returns AL_OK on success, AL_GBE_ERR_ILLEGAL_PARAM if the GBE pointer is NULL, or AL_GBE_ERR_FATLA_BUS_ERROR if no descriptors are to be built.
 */
AL_S32 AlGbe_Dev_BuildRxDescriptors(AL_GBE_DevStruct *Gbe)
{
    AL_GBE_RxDescListStruct *DmaRxDescList = &Gbe->RxDescList;
    AL_U32 DescIndex = DmaRxDescList->FirstAppDesc;
    __IO AL_GBE_DMADescStruct *DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);
    AL_U32 TotalAppdescNbr = DmaRxDescList->AppDescNbr;
    AL_U32 Descscan;

    AL_ASSERT((Gbe != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    if (DmaRxDescList->AppDescNbr == 0U) {
        /* No Rx descriptors to build */
        return AL_GBE_ERR_FATLA_BUS_ERROR;
    }

    if (DmaRxDescList->AppContextDesc != 0U) {
        /* A context descriptor is available */
        TotalAppdescNbr += 1U;
    }

    for (Descscan =0; Descscan < TotalAppdescNbr; Descscan++) {
        DmaRxDesc->DESC0 = DmaRxDesc->BackupAddr0;
        AlGbe_ll_SetRdesc3Buff1Valid((AL_REG)&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);

        if (DmaRxDesc->BackupAddr1 != 0U) {
            DmaRxDesc->DESC2 = DmaRxDesc->BackupAddr1;
            AlGbe_ll_SetRdesc3Buff2Valid((AL_REG)&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);
        }

        AlGbe_ll_SetRdesc3OwnByDma((AL_REG)&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);

        if (DmaRxDescList->ItMode != 0U) {
            AlGbe_ll_SetRdesc3CompleteIntr((AL_REG)&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);
        }

        if (Descscan < (TotalAppdescNbr - 1U)) {
            /* Increment rx descriptor index */
            INCR_RX_DESC_INDEX(DescIndex, 1U);
            /* Get descriptor address */
            DmaRxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaRxDescList->RxDesc[DescIndex]);
        }
    }

    /* Set the Tail pointer address to the last rx descriptor hold by the app */
    AlGbe_ll_SetDmaRxDescTailPointer(GbeBaseAddr, (AL_U32)((AL_UINTPTR)DmaRxDesc));

    /* reset the Application desc number */
    DmaRxDescList->AppDescNbr = 0;

    /*  reset the application context descriptor */
    Gbe->RxDescList.AppContextDesc = 0;

    return AL_OK;
}

/**
 *
 * This function sets up the transmit descriptors based on the provided configuration. It handles the buffer
 * alignment for cache coherency, sets the buffer lengths, and configures the descriptor for timestamping if
 * required. It also marks the first descriptor and sets the OWN bit to indicate that the descriptor is ready
 * to be processed by the DMA. If the packet spans multiple descriptors, it properly links them and clears the
 * last descriptor bit of the previous descriptor in the chain.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param TxConfig Pointer to the transmit descriptor configuration structure.
 * @param IntrEnable Specifies whether to enable interrupt upon completion.
 * @return Returns AL_GBE_ERR_DESC_STATE if the current descriptor is owned by DMA or already has a packet address assigned, otherwise AL_OK.
 */
static AL_S32 AlGbe_Dev_PrepareTxDescriptors(AL_GBE_DevStruct *Gbe, AL_GBE_TxDescConfigStruct *TxConfig, AL_GBE_FunctionEnum IntrEnable)
{
    AL_GBE_TxDescListStruct *DmaTxDescList = &Gbe->TxDescList;
    AL_U32 DescIndex = DmaTxDescList->CurTxDesc;
    AL_U32 FirstDescIdx = DmaTxDescList->CurTxDesc;
    AL_U32 DescNbr = 0;
    AL_GBE_DMADescStruct *DmaTxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaTxDescList->TxDesc[DescIndex]);

#ifdef ENABLE_MMU
    AL_UINTPTR BufferAlign;
    AL_U32 BufferLenAlign;
#endif

    AL_GBE_BufferStruct  *TxBuffer = TxConfig->TxBuffer;
    AL_U32               BdCount = 0;

    /* Current Tx Descriptor Owned by DMA: cannot be used by the application  */
    if ((AlGbe_ll_IsTxDescOwnByDma((AL_REG)&DmaTxDesc->DESC3) == AL_TRUE) ||
        (DmaTxDescList->PacketAddress[DescIndex] != AL_NULL)) {
        return AL_GBE_ERR_DESC_STATE;
    }

    /*
      Todo: Optional configuration
      1. VLAN tag
      2.tcp segmentation
     */

    /* Set descriptors here */
    DescNbr += 1U;

    /* Reload Buffer1 address and copy data to buffer1 */
    DmaTxDesc->DESC0 = Gbe->TxDescList.BufferAddress[DescIndex];
    memcpy((AL_U8 *)(DmaTxDesc->DESC0), (AL_U8 *)(TxBuffer->Buffer), TxBuffer->Len);

#ifdef ENABLE_MMU
    BufferAlign = (AL_UINTPTR)GBE_CACHE_ALIGN_MEMORY(DmaTxDesc->DESC0);
    BufferLenAlign = GBE_CACHE_ALIGN_SIZE((TxBuffer->Len)) + CACHE_LINE_SIZE;
    AlCache_FlushDcacheRange((AL_UINTPTR)BufferAlign, (AL_UINTPTR)(BufferAlign + BufferLenAlign));
#endif

    /* Set header or buffer 1 Length */
    AlGbe_ll_SetTdesc2Buffer1Len((AL_REG)&DmaTxDesc->DESC2, TxBuffer->Len);

    /* Buffer2 not use */
    DmaTxDesc->DESC1 = 0x0;
    /* Set buffer 2 Length */
    AlGbe_ll_SetTdesc2Buffer2Len((AL_REG)&DmaTxDesc->DESC2, 0x0U);

    AlGbe_ll_SetTdesc3FrameLen((AL_REG)&DmaTxDesc->DESC3, TxConfig->Length);

    /* Set Checksum Insertion Control */
    //AlGbe_ll_SetChksumInsertCtrl(&DmaTxDesc->DESC3, TxConfig->ChecksumCtrl);

    /* Mark it as First Descriptor */
    AlGbe_ll_SetTdesc3FirstDesc((AL_REG)&DmaTxDesc->DESC3, AL_GBE_FUNC_ENABLE);
    /* Mark it as NORMAL descriptor */
    AlGbe_ll_SetTdesc3ContextType((AL_REG)&DmaTxDesc->DESC3, AL_GBE_DESC_NORMAL_DESC);

    if (TxConfig->Attributes & AL_GBE_TX_PACKETS_FEATURES_TTSE) {
        AlGbe_ll_SetTdesc2TxTimeStampEnable((AL_REG)&DmaTxDesc->DESC2, AL_GBE_FUNC_ENABLE);
    }

    /* Ensure rest of descriptor is written to RAM before the OWN bit */
    DMB();
    /* set OWN bit of FIRST descriptor */
    AlGbe_ll_SetTdesc3OwnByDma((AL_REG)&DmaTxDesc->DESC3, AL_GBE_FUNC_ENABLE);

  /* only if the packet is split into more than one descriptors > 1 */
    while (TxBuffer->next != AL_NULL) {
        /* Clear the LD bit of previous descriptor */
        AlGbe_ll_SetTdesc3LastDesc((AL_REG)&DmaTxDesc->DESC3, AL_GBE_FUNC_DISABLE);
        /* Increment current tx descriptor index */
        INCR_TX_DESC_INDEX(DescIndex, 1U);
        /* Get current descriptor address */
        DmaTxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaTxDescList->TxDesc[DescIndex]);


      /* Current Tx Descriptor Owned by DMA: cannot be used by the application  */
        if ((AlGbe_ll_IsTxDescOwnByDma((AL_REG)&DmaTxDesc->DESC3) == AL_TRUE) ||
          (DmaTxDescList->PacketAddress[DescIndex] != AL_NULL)) {
          DescIndex = FirstDescIdx;
          DmaTxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaTxDescList->TxDesc[DescIndex]);

            /* clear previous desc own bit */
            for (AL_S32 Idx = 0; Idx < DescNbr; Idx ++) {
                /* Ensure rest of descriptor is written to RAM before the OWN bit */
                DMB();
                AlGbe_ll_SetTdesc3OwnByDma((AL_REG)&DmaTxDesc->DESC3, AL_GBE_FUNC_DISABLE);

                /* Increment current tx descriptor index */
                INCR_TX_DESC_INDEX(DescIndex, 1U);
                /* Get current descriptor address */
                DmaTxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)DmaTxDescList->TxDesc[DescIndex]);
            }

            return AL_GBE_ERR_DESC_STATE;
        }

        /* Clear the FD bit of new Descriptor */
        AlGbe_ll_SetTdesc3FirstDesc((AL_REG)&DmaTxDesc->DESC3, AL_GBE_FUNC_DISABLE);

        DescNbr += 1U;

        /* Get the next Tx buffer in the list */
        TxBuffer = TxBuffer->next;

        /* Reload Buffer1 address and copy data to buffer1 */
        DmaTxDesc->DESC0 = Gbe->TxDescList.BufferAddress[DescIndex];
        memcpy((AL_U8 *)(DmaTxDesc->DESC0), (AL_U8 *)(TxBuffer->Buffer), TxBuffer->Len);

#ifdef ENABLE_MMU
        BufferAlign = (AL_UINTPTR)GBE_CACHE_ALIGN_MEMORY(DmaTxDesc->DESC0);
        BufferLenAlign = GBE_CACHE_ALIGN_SIZE((TxBuffer->Len)) + CACHE_LINE_SIZE;
        AlCache_FlushDcacheRange((AL_UINTPTR)BufferAlign, (AL_UINTPTR)(BufferAlign + BufferLenAlign));
#endif

        /* Set header or buffer 1 Length */
        AlGbe_ll_SetTdesc2Buffer1Len((AL_REG)&DmaTxDesc->DESC2, TxBuffer->Len);

        /* Buffer2 not use */
        DmaTxDesc->DESC1 = 0x0;
        /* Set buffer 2 Length */
        AlGbe_ll_SetTdesc2Buffer2Len((AL_REG)&DmaTxDesc->DESC2, 0x0U);

        /* Set the packet length */
        AlGbe_ll_SetTdesc3FrameLen((AL_REG)&DmaTxDesc->DESC3, TxConfig->Length);


        /* Set Checksum Insertion Control */
        //AlGbe_ll_SetChksumInsertCtrl(&DmaTxDesc->DESC3, TxConfig->ChecksumCtrl);

        BdCount += 1U;

        /* Ensure rest of descriptor is written to RAM before the OWN bit */
        DMB();
        /* Set Own bit */
        AlGbe_ll_SetTdesc3OwnByDma((AL_REG)&DmaTxDesc->DESC3, AL_GBE_FUNC_ENABLE);
        /* Mark it as NORMAL descriptor */
        AlGbe_ll_SetTdesc3ContextType((AL_REG)&DmaTxDesc->DESC3, AL_GBE_DESC_NORMAL_DESC);
    }

    if (IntrEnable == AL_GBE_FUNC_ENABLE) {
        /* Set Interrupt on completion bit */
        AlGbe_ll_SetTdesc2CompleteIntr((AL_REG)&DmaTxDesc->DESC2, AL_GBE_FUNC_ENABLE);
    } else {
        /* Clear Interrupt on completion bit */
        AlGbe_ll_SetTdesc2CompleteIntr((AL_REG)&DmaTxDesc->DESC2, AL_GBE_FUNC_DISABLE);
    }

    /* Mark it as LAST descriptor */
    AlGbe_ll_SetTdesc3LastDesc((AL_REG)&DmaTxDesc->DESC3, AL_GBE_FUNC_ENABLE);
    /* Save the current packet address to expose it to the application */
    DmaTxDescList->PacketAddress[DescIndex] = DmaTxDescList->CurrentPacketAddress;

    DmaTxDescList->CurTxDesc = DescIndex;

    DmaTxDescList->BuffersInUse += BdCount + 1U;

    return AL_OK;
}

/**
 * This function iterates through the buffers in use and releases packets that have been transmitted.
 * It updates the release index and the number of buffers in use accordingly.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return Returns AL_OK on success.
 */
AL_S32 AlGbe_Dev_ReleaseTxPacket(AL_GBE_DevStruct *Gbe)
{
    AL_GBE_TxDescListStruct *DmaTxDescList = &Gbe->TxDescList;
    AL_U32 NumOfBuff =  DmaTxDescList->BuffersInUse;
    AL_U32 Index =       DmaTxDescList->ReleaseIndex;
    AL_U8 PktTxStatus = 1U;
    AL_U8 PktInUse;

    AL_ASSERT((Gbe != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    /* Loop through Buffers in use.  */
    while ((NumOfBuff != 0U) && (PktTxStatus != 0U)) {
        PktInUse = 1U;
        NumOfBuff--;
        /* If no packet, just examine the next packet.  */
        if (DmaTxDescList->PacketAddress[Index] == AL_NULL) {
            /* No packet in use, skip to next.  */
            Index = (Index + 1U) & (AL_GBE_TX_DESC_CNT - 1U);
            PktInUse = 0U;
        }

        if (PktInUse != 0U) {
            /* Determine if the packet has been transmitted.  */
            if (!AlGbe_ll_IsTxDescOwnByDma((AL_REG)(&(Gbe->InitConfig.TxDescList[Index].DESC3)))) {

                /* Release the packet.  */
                Gbe->TxFreeCallback(DmaTxDescList->PacketAddress[Index]);

                /* Clear the entry in the in-use array.  */
                DmaTxDescList->PacketAddress[Index] = AL_NULL;

                /* Update the transmit relesae index and number of Buffers in use.  */
                Index = (Index + 1U) & (AL_GBE_TX_DESC_CNT - 1U);
                DmaTxDescList->BuffersInUse = NumOfBuff;
                DmaTxDescList->ReleaseIndex = Index;
            } else {
                /* break the loop */
                PktTxStatus = 0U;
            }
        }
    }

    return AL_OK;
}

/**
 * This function prepares the transmit descriptors, sets the device as busy, and starts the transmission.
 * It ensures that the device and the provided configuration are valid and that the device is ready and not busy.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param TxConfig Pointer to the transmit descriptor configuration structure.
 * @return Returns AL_OK on success, or an error code on failure.
 */
AL_S32 AlGbe_Dev_Transmit(AL_GBE_DevStruct *Gbe, AL_GBE_TxDescConfigStruct *TxConfig)
{
    AL_S32 Ret;

    AL_ASSERT((Gbe != AL_NULL) && (TxConfig != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);
    AL_ASSERT(((Gbe->State & AL_GBE_STATE_READY) == AL_GBE_STATE_READY), AL_GBE_ERR_NOT_READY);
    AL_ASSERT((!AlGbe_Dev_IsTxBusy(Gbe)), AL_GBE_ERR_BUSY);

    /* Change Status */
    AlGbe_Dev_SetTxBusy(Gbe);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    Gbe->TxDescList.CurrentPacketAddress = (AL_U32 *)TxConfig->pData;

    Ret = AlGbe_Dev_PrepareTxDescriptors(Gbe, TxConfig, AL_GBE_FUNC_ENABLE);
    if (Ret != AL_OK) {
        return Ret;
    }

    DMB();

    INCR_TX_DESC_INDEX(Gbe->TxDescList.CurTxDesc, 1U);

    /* Start transmission */
    /* issue a poll command to Tx DMA by writing address of next immediate free descriptor */
    AlGbe_ll_SetDmaTxDescTailPointer(GbeBaseAddr, (AL_U32)(Gbe->TxDescList.TxDesc[Gbe->TxDescList.CurTxDesc]));

    return AL_OK;
}

/**
 * This function checks if the timestamp is available for the given transmit descriptor and retrieves it if available.
 * It clears the timestamp status after retrieval.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param DmaTxDesc Pointer to the DMA transmit descriptor.
 * @return Returns AL_OK on success, or AL_GBE_DESC_TX_TIMESTAMP_STATUS_ERROR if the timestamp is not available.
 */
AL_S32 AlGbe_Dev_GetTxTimeStamp(AL_GBE_DevStruct *Gbe, const AL_GBE_DMADescStruct *DmaTxDesc)
{
    if (!AlGbe_ll_GetWbTxDesc3TxTimeStatus((AL_REG)&DmaTxDesc->DESC3)) {
        return AL_GBE_DESC_TX_TIMESTAMP_STATUS_ERROR;
    }

    Gbe->TxTimeStamp.TimeStampLow = DmaTxDesc->DESC0;
    Gbe->TxTimeStamp.TimeStampHigh = DmaTxDesc->DESC1;

    AlGbe_ll_ClearWbTxDesc3TxTimeStatus((AL_REG)&DmaTxDesc->DESC3);

    return AL_OK;
}

/**
 * This function is similar to AlGbe_Dev_Transmit but waits for the packet to be sent if timestamping is enabled.
 * It checks for fatal bus errors during transmission.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param TxConfig Pointer to the transmit descriptor configuration structure.
 * @return Returns AL_OK on success, AL_GBE_ERR_FATLA_BUS_ERROR on fatal bus error, or another error code on failure.
 */
AL_S32 AlGbe_Dev_TransmitPolling(AL_GBE_DevStruct *Gbe, AL_GBE_TxDescConfigStruct *TxConfig)
{
    AL_S32 Ret;
    const AL_GBE_DMADescStruct *DmaTxDesc;

    AL_ASSERT((Gbe != AL_NULL) && (TxConfig != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);
    AL_ASSERT(((Gbe->State & AL_GBE_STATE_READY) == AL_GBE_STATE_READY), AL_GBE_ERR_NOT_READY);
    AL_ASSERT((!AlGbe_Dev_IsTxBusy(Gbe)), AL_GBE_ERR_BUSY);

    /* Change Status */
    AlGbe_Dev_SetTxBusy(Gbe);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    DmaTxDesc = (AL_GBE_DMADescStruct *)((AL_UINTPTR)((&Gbe->TxDescList)->TxDesc[Gbe->TxDescList.CurTxDesc]));

    Ret = AlGbe_Dev_PrepareTxDescriptors(Gbe, TxConfig, AL_GBE_FUNC_DISABLE);
    if (Ret != AL_OK) {
        return Ret;
    }

    DMB();

    INCR_TX_DESC_INDEX(Gbe->TxDescList.CurTxDesc, 1U);

    /* Start transmission */
    /* issue a poll command to Tx DMA by writing address of next immediate free descriptor */
    AlGbe_ll_SetDmaTxDescTailPointer(GbeBaseAddr, (AL_U32)(Gbe->TxDescList.TxDesc[Gbe->TxDescList.CurTxDesc]));

    if (TxConfig->Attributes & AL_GBE_TX_PACKETS_FEATURES_TTSE) {

        /* If need to get the timestamp of tx, need to wait for the packet to finish sending */
        while (AlGbe_ll_IsWbTxDescOwnByDma((AL_REG)&DmaTxDesc->DESC3)) {
            if (AlGbe_ll_IsDmaChannelFatalBusError(GbeBaseAddr)) {
                return AL_GBE_ERR_FATLA_BUS_ERROR;
            }
        }

        Ret = AlGbe_Dev_GetTxTimeStamp(Gbe, DmaTxDesc);
        if (Ret != AL_OK) {
            AlGbe_Dev_ClrTxBusy(Gbe);
            return Ret;
        }
    }

    AlGbe_Dev_ClrTxBusy(Gbe);

    return AL_OK;
}

/**
 * This function is called when a transmit operation is completed.
 * It notifies the registered event callback with the TX done event and clears the busy status of the device.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return None.
 */
static AL_VOID AlGbe_Dev_TxDoneHandler(AL_GBE_DevStruct *Gbe)
{
    if (Gbe->EventCallBack) {
        AL_GBE_EventStruct GbeEvent = {
            .Event        = AL_GBE_EVENT_TX_DONE,
        };
        (*Gbe->EventCallBack)(&GbeEvent, Gbe->EventCallBackRef);
    }
    AlGbe_Dev_ClrTxBusy(Gbe);
}

/**
 * This function is called when a receive operation is completed.
 * It notifies the registered event callback with the RX done event.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return None.
 */
static AL_VOID AlGbe_Dev_RxDoneHandler(AL_GBE_DevStruct *Gbe)
{
    if (Gbe->EventCallBack) {
        AL_GBE_EventStruct GbeEvent = {
            .Event        = AL_GBE_EVENT_RX_DONE,
        };
        (*Gbe->EventCallBack)(&GbeEvent, Gbe->EventCallBackRef);
    }

}

#define GBE_IN_TX_COMPLETE_INTR(Status)             (Status & AL_GBE_INTR_TX_COMPLETE)
#define GBE_IN_TX_PROCESS_STOP_INTR(Status)         (Status & AL_GBE_INTR_TX_PROCESS_STOP)
#define GBE_IN_TX_BUFFER_UNAVAILABLE_INTR(Status)   (Status & AL_GBE_INTR_TX_BUFFER_UNAVAILABLE)
#define GBE_IN_RX_COMPLETE_INTR(Status)             (Status & AL_GBE_INTR_RX_COMPLETE)
#define GBE_IN_RX_BUFFER_UNAVAILABLE_INTR(Status)   (Status & AL_GBE_INTR_RX_BUFFER_UNAVAILABLE)
#define GBE_IN_RX_PROCESS_STOP_INTR(Status)         (Status & AL_GBE_INTR_RX_PROCESS_STOP)
#define GBE_IN_RX_WATCHDOG_TIMEOUT_INTR(Status)     (Status & AL_GBE_INTR_RX_WATCHDOG_TIMEOUT)
#define GBE_IN_EARLY_TX_INTR(Status)                (Status & AL_GBE_INTR_EARLY_TX)
#define GBE_IN_EARLY_RX_INTR(Status)                (Status & AL_GBE_INTR_EARLY_RX)
#define GBE_IN_FATAL_BUS_ERROR_INTR(Status)         (Status & AL_GBE_INTR_FATAL_BUS_ERROR)
#define GBE_IN_CONTEXT_DESC_ERROR_INTR(Status)      (Status & AL_GBE_INTR_CONTEXT_DESC_ERROR)
#define GBE_IN_ABNORMAL_SUMMARY_INTR(Status)        (Status & AL_GBE_INTR_ABNORMAL_SUMMARY)
#define GBE_IN_NORMAL_SUMMARY_INTR(Status)          (Status & AL_GBE_INTR_NORMAL_SUMMARY)

/**
 * This function handles device errors by checking the interrupt status and executing the appropriate actions based on the type of error.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return None.
 */
static AL_VOID AlGbe_Dev_ErrorHandler(AL_GBE_DevStruct *Gbe)
{
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_GBE_IntrStatusEnum IntrStatus = AlGbe_ll_GetDmaChannelStatus(GbeBaseAddr);
    AL_GBE_EventIdEnum Event = AL_GBE_EVENT_CTX_DESC_ERROR;

    if (GBE_IN_FATAL_BUS_ERROR_INTR(IntrStatus)) {
        Event = AL_GBE_EVENT_FATAL_BUS_ERROR;

        /* Fatal error, disable all interrupt */
        AlGbe_ll_SetDmaAbnormalSummaryIntrEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
        AlGbe_ll_SetDmaNormalSummaryIntrEnable(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    } else if (GBE_IN_RX_BUFFER_UNAVAILABLE_INTR(IntrStatus)) {
        Event = AL_GBE_EVENT_RX_BUFFER_UNAVAILABLE;

    } else if (GBE_IN_EARLY_TX_INTR(IntrStatus)) {
        Event = AL_GBE_EVENT_EARLY_TX;

    } else if (GBE_IN_CONTEXT_DESC_ERROR_INTR(IntrStatus)) {
        Event = AL_GBE_EVENT_CTX_DESC_ERROR;

    } else if (GBE_IN_RX_PROCESS_STOP_INTR(IntrStatus)) {
        Event = AL_GBE_EVENT_RX_STOP;

    } else if (GBE_IN_RX_WATCHDOG_TIMEOUT_INTR(IntrStatus)) {
        Event = AL_GBE_EVENT_RX_WATCHDOG_TIMEOUT;

    }

    if (Gbe->EventCallBack) {
        AL_GBE_EventStruct GbeEvent = {
            .Event        = Event,
        };
        (*Gbe->EventCallBack)(&GbeEvent, Gbe->EventCallBackRef);
    }
}

/**
 * This function dumps the MMC registers for debugging purposes.
 *
 * @param Instance Pointer to the GBE device instance.
 * @return None.
 */
static AL_VOID AlGbe_Dev_DumpMmcRegister(AL_VOID *Instance)
{
    AL_U32 addr;
    AL_GBE_DevStruct *Gbe = (AL_GBE_DevStruct *)Instance;
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    AL_LOG(AL_LOG_LEVEL_DEBUG, "dump mmc registers:: \r\n");
    for (addr = GbeBaseAddr + GBE__MMC_CONTROL__OFFSET; addr <= GbeBaseAddr + GBE__RX_CONTROL_PACKETS_GOOD__OFFSET; addr += 4) {
        AL_LOG(AL_LOG_LEVEL_DEBUG, "0x%8x : 0x%8x \r\n", addr, AL_REG32_READ(addr));
    }
}

/**
 * This function checks the type of interrupt and calls the appropriate handler function.
 *
 * @param Instance Pointer to the GBE device instance.
 * @return None.
 */
AL_VOID AlGbe_Dev_IntrHandler(AL_VOID *Instance)
{
    AL_GBE_DevStruct *Gbe = (AL_GBE_DevStruct *)Instance;
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_GBE_IntrStatusEnum IntrStatus = AlGbe_ll_GetDmaChannelStatus(GbeBaseAddr);

    if (GBE_IN_RX_COMPLETE_INTR(IntrStatus)) {
        AlGbe_Dev_RxDoneHandler(Gbe);
    }

    if (GBE_IN_TX_COMPLETE_INTR(IntrStatus)) {
        AlGbe_Dev_TxDoneHandler(Gbe);
    }

    if (GBE_IN_ABNORMAL_SUMMARY_INTR(IntrStatus)) {
        AlGbe_Dev_ErrorHandler(Gbe);
    }

    AlGbe_ll_ClrDmaIntr(GbeBaseAddr, IntrStatus);

#ifdef ENABLE_MMC
    /*
     * This is a workaround when enable mmc,
     * mmc interrupt are all masked, but the interrupt still happen
    */
    if (IntrStatus == 0) {
        AlGbe_Dev_DumpMmcRegister(Instance);
    }
#endif
}

/**
 * This function registers a callback function for GBE device events.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param Callback Function pointer to the callback function.
 * @param CallbackRef Pointer to user data that will be passed to the callback function.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_RegisterEventCallBack(AL_GBE_DevStruct *Gbe, AL_GBE_EventCallBack Callback, AL_VOID *CallbackRef)
{
    AL_ASSERT((Gbe != AL_NULL) && (Callback != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    Gbe->EventCallBack        = Callback;
    Gbe->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 * This function sets the PTP (Precision Time Protocol) timestamp.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param Timestamp Pointer to the structure containing the timestamp to be set.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_SetPtpTimestamp(AL_GBE_DevStruct *Gbe, AL_GBE_PtpTimeStruct *Timestamp)
{
    AL_ASSERT((Gbe != AL_NULL) && (Timestamp != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    AlGbe_ll_SetSystemTimeSecondsUpdate(GbeBaseAddr, Timestamp->Sec);
    AlGbe_ll_SetSystemTimeNanosecondsUpdate(GbeBaseAddr, Timestamp->Nsec);

    AlGbe_ll_EnableInitializeTimestamp(GbeBaseAddr);
    while(AlGbe_ll_IsInitializeTimestampEnabled(GbeBaseAddr) == AL_GBE_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function retrieves the current PTP (Precision Time Protocol) timestamp.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param Timestamp Pointer to the structure where the current timestamp will be stored.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_GetPtpTimestamp(AL_GBE_DevStruct *Gbe, AL_GBE_PtpTimeStruct *Timestamp)
{
    AL_ASSERT((Gbe != AL_NULL) && (Timestamp != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    Timestamp->Sec = AlGbe_ll_GetSystemTimeSeconds(GbeBaseAddr);
    Timestamp->Nsec = AlGbe_ll_GetSystemTimeNanoseconds(GbeBaseAddr);

    return AL_OK;
}

/**
 * This function updates the PTP time offset.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param TimeOffset Pointer to the structure containing the time offset to be applied.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_UpdatePtpTimeOffset(AL_GBE_DevStruct *Gbe, AL_GBE_PtpTimeStruct *TimeOffset)
{
    AL_ASSERT((Gbe != AL_NULL) && (TimeOffset != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    AlGbe_ll_SetSystemTimeSecondsUpdate(GbeBaseAddr, TimeOffset->Sec);

    AlGbe_ll_SetSystemTimeNanosecondsUpdate(GbeBaseAddr, (TimeOffset->Nsec) |
                                            (TimeOffset->Sign << GBE__MAC_SYSTEM_TIME_NANOSECONDS_UPDATE__ADDSUB__SHIFT));

    AlGbe_ll_EnableUpdateTimestamp(GbeBaseAddr);
    while(AlGbe_ll_IsUpdateTimestampEnabled(GbeBaseAddr) == AL_GBE_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function adjusts the PTP time frequency.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param Adj Adjustment value for the PTP time frequency.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_AdjustPtpTimeFreq(AL_GBE_DevStruct *Gbe, AL_U32 Adj)
{
    AL_ASSERT((Gbe != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    AL_U32 Addend = Adj;

    AlGbe_ll_SetTimestampAddend(GbeBaseAddr, Addend);

    AlGbe_ll_EnableUpdateAddendRegister(GbeBaseAddr);
    while(AlGbe_ll_IsUpdateAddendRegisterEnabled(GbeBaseAddr) == AL_GBE_FUNC_ENABLE);

    return AL_OK;
}

/**
 * This function enables timestamping for the GBE device.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @return None.
 */
AL_VOID AlGbe_Dev_EnableTimestamp(AL_GBE_DevStruct *Gbe)
{
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    AlGbe_ll_EnableTimeStamp(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Set PTP frame type */
    AlGbe_ll_EnablePtpV2(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
    AlGbe_ll_EnablePtpPacketsOverEthernet(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
    AlGbe_ll_EnablePtpPacketsOverIpv4Udp(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    /* Set PTP packets type for Taking Snapshots */
    AlGbe_ll_EnableTimestampSnapshotForEventMessage(GbeBaseAddr, AL_GBE_FUNC_DISABLE);
    AlGbe_ll_EnableTMessageSnapshotForRelevantMaster(GbeBaseAddr, AL_GBE_FUNC_DISABLE);
    AlGbe_ll_PtpSelectPacketsForTakingSnapshot(GbeBaseAddr, 1);
}

/**
 * This function initializes the PTP (Precision Time Protocol) settings for the GBE device, including setting the initial timestamp and adjusting the time frequency if necessary.
 *
 * @param Gbe Pointer to the GBE device structure.
 * @param PtpConfig Pointer to the structure containing the PTP configuration settings.
 * @return AL_OK on success, error code otherwise.
 */
AL_S32 AlGbe_Dev_PtpInit(AL_GBE_DevStruct *Gbe, AL_GBE_PtpConfigStruct *PtpConfig)
{
    AL_S32 Data;
    AL_S64 Temp;
    AL_U32 Addend;
    AL_U32 PtpClock;

    AL_ASSERT((Gbe != AL_NULL) && (PtpConfig != AL_NULL), AL_GBE_ERR_ILLEGAL_PARAM);

    Gbe->PtpConfig = *PtpConfig;

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    AlGbe_Dev_EnableTimestamp(Gbe);

    AlGbe_ll_EnableTimestampDigitalorBinaryRollover(GbeBaseAddr, AL_GBE_FUNC_ENABLE);

    PtpClock = Gbe->HwConfig.PtpInputClockHz;

    if (PtpConfig->UpdateMethod == AL_GBE_PTP_FINE_UPDATE) {
        AlGbe_ll_EnableFineTimestampUpdate(GbeBaseAddr, AL_GBE_FUNC_ENABLE);
        Data = (AL_GBE_ONE_SEC_IN_NANOSEC / PtpClock) * 2;
    } else {
        Data = (AL_GBE_ONE_SEC_IN_NANOSEC / PtpClock);
    }

    /* 0.465ns accuracy */
    //Data = (Data * 1000) / 465;

    if (Data > AL_GBE_PTP_MAX_SUB_SECOND_INCREMENT)
        Data = AL_GBE_PTP_MAX_SUB_SECOND_INCREMENT;

    AlGbe_ll_SetSubSecondIncrementValue(GbeBaseAddr, Data);

    Temp = AL_GBE_ONE_SEC_IN_NANOSEC / Data;

    /* calculate default added value:
     * formula is :
     * Addend = (2^32)/freq_div_ratio;
     * where, freq_div_ratio = 1e9ns/sec_inc
     */
    Temp = Temp << 32;
    Addend = Temp / PtpClock;

    Gbe->PtpConfig.DefaultAddend = Addend;

    if (PtpConfig->UpdateMethod == AL_GBE_PTP_FINE_UPDATE) {
        AlGbe_Dev_AdjustPtpTimeFreq(Gbe, Addend);
    }

    AL_GBE_PtpTimeStruct Timestamp = {
        .Sec = 1703593218,
        .Nsec = 0,
        .Sign = 0,
    };

    AlGbe_Dev_SetPtpTimestamp(Gbe, &Timestamp);

    return AL_OK;
}
