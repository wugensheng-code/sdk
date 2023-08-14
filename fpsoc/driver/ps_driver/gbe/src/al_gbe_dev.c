#include "al_gbe_dev.h"

#include <string.h>

extern AL_GBE_HwConfigStruct AlGbe_HwConfig[AL_GBE_NUM_INSTANCE];

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

AL_BOOL AlGbe_Dev_IsTxBusy(AL_GBE_DevStruct *Gbe)
{
    return (AL_BOOL)(Gbe->State & AL_GBE_STATE_TX_BUSY);
}

AL_BOOL AlGbe_Dev_IsRxBusy(AL_GBE_DevStruct *Gbe)
{
    return (AL_BOOL)(Gbe->State & AL_GBE_STATE_RX_BUSY);
}

AL_VOID AlGbe_Dev_SetTxBusy(AL_GBE_DevStruct *Gbe)
{
    Gbe->State |= AL_GBE_STATE_TX_BUSY;
}

AL_VOID AlGbe_Dev_SetRxBusy(AL_GBE_DevStruct *Gbe)
{
    Gbe->State |= AL_GBE_STATE_RX_BUSY;
}

AL_VOID AlGbe_Dev_ClrTxBusy(AL_GBE_DevStruct *Gbe)
{
    Gbe->State &= (~AL_GBE_STATE_TX_BUSY);
}

AL_VOID AlGbe_Dev_ClrRxBusy(AL_GBE_DevStruct *Gbe)
{
    Gbe->State &= (~AL_GBE_STATE_RX_BUSY);
}

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
    MacDefaultCfg.DuplexMode = AL_GBE_FULL_DUPLEX_MODE;
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
    MacDefaultCfg.Speed = AL_GBE_SPEED_100M;
    MacDefaultCfg.Support2KPacket = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.TxQueueStoreForward = AL_GBE_FUNC_ENABLE;
    MacDefaultCfg.TransmitFlowControl = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.UnicastPausePacketDetect = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.UnicastSlowProtocolPacketDetect = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.WatchdogDisable = AL_GBE_FUNC_DISABLE;
    MacDefaultCfg.WatchdogTimeout =  AL_GBE_WatchdogTimeout2K;
    MacDefaultCfg.ZeroQuantaPause = AL_GBE_FUNC_ENABLE;

    /* Config Mac configuretion register */
    AlGbe_ll_SetPreLEn(GbeBaseAddr, MacDefaultCfg.PreambleLength);
    AlGbe_ll_SetCarrierSenseBeforeTransmitEnable(GbeBaseAddr, MacDefaultCfg.CarrierSenseBeforeTransmit);
    AlGbe_ll_SetLoopbackModeEnable(GbeBaseAddr, MacDefaultCfg.LoopbackMode);

    if (Gbe->MacDmaConfig.GbePhyAutoNegotiation != AL_GBE_FUNC_ENABLE) {
        AlGbe_ll_SetDuplexMode(GbeBaseAddr, MacDefaultCfg.DuplexMode);
        AlGbe_ll_SetSpeed(GbeBaseAddr, MacDefaultCfg.Speed);
    }

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

AL_U32 AlGbe_Dev_ConfigRxDescBuffer(AL_GBE_DevStruct *Gbe, AL_U32 DescIndex, AL_U8 *Buffer1, AL_U8 *Buffer2)
{
    if ((Gbe == AL_NULL) || (Buffer1 == AL_NULL) || (DescIndex >= (AL_U32)AL_GBE_RX_DESC_CNT)) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_GBE_DMADescStruct *DmaRxDesc = (AL_GBE_DMADescStruct *)Gbe->RxDescList.RxDesc[DescIndex];

    /* write buffer address to RDES0 */
    DmaRxDesc->DESC0 = (__IO AL_U32)Buffer1;
    /* store buffer address */
    DmaRxDesc->BackupAddr0 = (AL_U32)Buffer1;
    /* set buffer address valid bit to RDES3 */
    AlGbe_ll_SetRdesc3Buff1Valid(&(DmaRxDesc->DESC3), AL_GBE_FUNC_ENABLE);

    if (Buffer2 != AL_NULL) {
        /* write buffer 2 address to RDES1 */
        DmaRxDesc->DESC2 = (AL_U32)Buffer2;
        /* store buffer 2 address */
        DmaRxDesc->BackupAddr1 = (AL_U32)Buffer2;
        /* set buffer 2 address valid bit to RDES3 */
        AlGbe_ll_SetRdesc3Buff2Valid(&(DmaRxDesc->DESC3), AL_GBE_FUNC_ENABLE);
    }
    /* set OWN bit to RDES3 */
    AlGbe_ll_SetRdesc3OwnByDma(&(DmaRxDesc->DESC3), AL_GBE_FUNC_ENABLE);

    return AL_OK;
}

static void AlGbe_Dev_DMARxDescListInit(AL_GBE_DevStruct *Gbe)
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
        Gbe->RxDescList.RxDesc[Index]= (AL_U32)DmaRxDesc;
    }

    Gbe->RxDescList.CurRxDesc = 0;
    Gbe->RxDescList.FirstAppDesc = 0;
    Gbe->RxDescList.AppDescNbr = 0;
    Gbe->RxDescList.ItMode = 0;
    Gbe->RxDescList.AppContextDesc = 0;

    /* Set Receive Descriptor Ring Length */
    AlGbe_ll_SetDmaChannelRxDescRingLen(GbeBaseAddr, AL_GBE_RX_DESC_CNT - 1);

    /* Set Receive Descriptor List Address */
    AlGbe_ll_SetDmaChannelRxDescListAddr(GbeBaseAddr, (AL_U32)DmaRxDescList);

    /* Set Receive Descriptor Tail pointer Address */
    AlGbe_ll_SetDmaRxDescTailPointer(GbeBaseAddr, ((AL_U32)(DmaRxDescList + (AL_U32)(AL_GBE_RX_DESC_CNT - 1U))));
}

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

        Gbe->TxDescList.TxDesc[Index] = (AL_U32)DmaTxDesc;
    }

    Gbe->TxDescList.CurTxDesc = 0;

    /* Set Transmit Descriptor Ring Length */
    AlGbe_ll_SetDmaChannelTxDescRingLen(GbeBaseAddr, AL_GBE_TX_DESC_CNT - 1);

    /* Set Transmit Descriptor List Address */
    AlGbe_ll_SetDmaChannelTxDescListAddr(GbeBaseAddr, (AL_U32)DmaTxDescList);

    /* Set Transmit Descriptor Tail pointer */
    AlGbe_ll_SetDmaTxDescTailPointer(GbeBaseAddr, ((AL_U32)DmaTxDescList));
}

AL_S32 AlGbe_Dev_ConfigDuplexAndSpeed(AL_GBE_DevStruct *Gbe)
{
    if (Gbe == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    AlGbe_ll_SetDuplexMode(GbeBaseAddr, Gbe->MacDmaConfig.DuplexMode);
    AlGbe_ll_SetSpeed(GbeBaseAddr, Gbe->MacDmaConfig.Speed);

    return AL_OK;
}

AL_S32 AlGbe_Dev_CheckConfig(AL_GBE_InitStruct *InitConfig, AL_GBE_MacDmaConfigStruct *MacDmaConfig)
{
    if (InitConfig->TxDescList == AL_NULL || InitConfig->RxDescList == AL_NULL) {
        return AL_GBE_ERR_CONFIG_ERROR;
    }

    /*
      The buffer size must be a multiple of 4, 8, or 16
      depending on the data bus widths (32-bit, 64-bit, or 128-bit
      respectively). data bus widths is 32-bit.
     */
    if (InitConfig->RxBuffLen % 4) {
        return AL_GBE_ERR_CONFIG_ERROR;
    }

    return AL_OK;
}

AL_S32 AlGbe_Dev_Init(AL_GBE_DevStruct *Gbe, AL_GBE_HwConfigStruct *HwConfig,
                      AL_GBE_InitStruct *InitConfig, AL_GBE_MacDmaConfigStruct *MacDmaConfig)
{
    AL_S32 Ret;
    AL_REG GbeBaseAddr;
    AL_U32 MacAddrLow;
    AL_U16 MacAddrHigh;

    if (Gbe == AL_NULL || HwConfig == AL_NULL || InitConfig == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

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
        AlGbe_ll_SetGbeCtlRegister((GbeBaseAddr == GBE0__BASE_ADDR ? CFG_CTRL_GBE0_ADDR : CFG_CTRL_GBE1_ADDR), 0x321);
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

    Gbe->State |= AL_GBE_STATE_READY;

    return AL_OK;
}

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

AL_S32 AlGbe_Dev_ReadPhyRegister(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddr, AL_U32 PHYReg, AL_U16 *RegValue)
{
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_REG TmpReg;

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

AL_S32 AlGbe_Dev_PhyInit(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddress)
{
    AL_S32 Ret = 0;
    AL_U16 RegValue;

    if (Gbe == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    /* Reset the Phy */
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_ADDR, PHY_BCR_RESET);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%d\r\n", Ret);
        return Ret;
    }

    /* Delay at least 10 ms */
    AlSys_MDelay(5000);

    /* Check reset status complete */
    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BCR_ADDR, &RegValue);
    if ((Ret != AL_OK) || (RegValue & PHY_BCR_RESET)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%d\r\n", Ret);
        return AL_GBE_ERR_PHY_RESET_FAILED;
    }

    /* Read PHY_BSR_ADDR twice to check link status */
    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BSR_ADDR, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%d\r\n", Ret);
        return Ret;
    }
    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BSR_ADDR, &RegValue);
    if (Ret != AL_OK) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%d\r\n", Ret);
        return Ret;
    }
    if ((RegValue & PHY_BSR_LINK_STATUS) == 0) {
         /* Return Link Down status */
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister link error 563\r\n");
        return AL_GBE_ERR_PHY_LINK_DOWN;
    }

    /* AutoNegotiation is disabled */
    if (Gbe->MacDmaConfig.GbePhyAutoNegotiation == AL_GBE_FUNC_DISABLE) {
        AL_U16 TmpReg = 0;
        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BCR_ADDR, &TmpReg);
        if (Ret != 0) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister error:%d\r\n", Ret);
            return Ret;
        }

        if (Gbe->MacDmaConfig.DuplexMode == AL_GBE_FULL_DUPLEX_MODE) {
            AL_REG32_SET_BIT(&TmpReg, PHY_BCR_DUPLEX_SHIFT, AL_GBE_FUNC_ENABLE);
        } else {
            AL_REG32_SET_BIT(&TmpReg, PHY_BCR_DUPLEX_SHIFT, AL_GBE_FUNC_DISABLE);
        }

        if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_1G) {
            AL_REG32_SET_BIT(&TmpReg, PHY_BCR_SPEED1_SHIFT, AL_GBE_FUNC_ENABLE);
            AL_REG32_SET_BIT(&TmpReg, PHY_BCR_SPEED0_SHIFT, AL_GBE_FUNC_DISABLE);
        } else if (Gbe->MacDmaConfig.Speed == AL_GBE_SPEED_100M) {
            AL_REG32_SET_BIT(&TmpReg, PHY_BCR_SPEED1_SHIFT, AL_GBE_FUNC_DISABLE);
            AL_REG32_SET_BIT(&TmpReg, PHY_BCR_SPEED0_SHIFT, AL_GBE_FUNC_ENABLE);
        } else {
            AL_REG32_SET_BIT(&TmpReg, PHY_BCR_SPEED1_SHIFT, AL_GBE_FUNC_DISABLE);
            AL_REG32_SET_BIT(&TmpReg, PHY_BCR_SPEED0_SHIFT, AL_GBE_FUNC_DISABLE);
        }

        AL_REG32_SET_BIT(&TmpReg, PHY_BCR_AUTO_NEGO_SHIFT, AL_GBE_FUNC_DISABLE);

        Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_ADDR, TmpReg);
        if (Ret != 0) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_WritePhyRegister error:%d\r\n", Ret);
            return Ret;
        }

        AlSys_MDelay(4000);

        Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_SSR_ADDR, &TmpReg);
        if ((Ret != AL_OK) || (TmpReg & PHY_SSR_LINK_STATUS) == 0) {
            AL_LOG(AL_LOG_LEVEL_ERROR, "AlGbe_Dev_ReadPhyRegister link error 604\r\n");
            return Ret;
        }

        return AL_OK;
    }

    /* If Auto-negotiation is enabled, configure the Auto-negotiation function */
    Ret = AlGbe_Dev_WritePhyRegister(Gbe, PHYAddress, PHY_BCR_ADDR, PHY_BCR_AUTONEGOTIATION);
    if (Ret != AL_OK) {
        /* Return ERROR in case of write timeout */
        return Ret;
    }

    AlSys_MDelay(3000);

    /* Wait until the auto-negotiation will be completed */
    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_BSR_ADDR, &RegValue);
    if (Ret != AL_OK) {
        return Ret;
    }
    if ((RegValue & PHY_BSR_AUTO_NEGOTIATION_COMPLETE) == 0) {
        /* PHY_BSR_AUTO_NEGOTIATION_COMPLETE failed */
        return AL_GBE_ERR_PHY_RESET_FAILED;
    }

    /* Read phy status */
    Ret = AlGbe_Dev_ReadPhyRegister(Gbe, PHYAddress, PHY_SSR_ADDR, &RegValue);
    /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
    if (RegValue & PHY_SSR_FULL_DUPLEX) {
         /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
        Gbe->MacDmaConfig.DuplexMode = AL_GBE_FULL_DUPLEX_MODE;
    } else {
        /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
        Gbe->MacDmaConfig.DuplexMode = AL_GBE_HALF_DUPLEX_MODE;
    }
    /* Configure the MAC with the speed fixed by the auto-negotiation process */
    if (RegValue & PHY_SSR_SPEED_1G) {
        /* Set Ethernet speed to 10M following the auto-negotiation */
        Gbe->MacDmaConfig.Speed = AL_GBE_SPEED_1G;
    } else if (RegValue & AL_GBE_SPEED_100M) {
        /* Set Ethernet speed to 100M following the auto-negotiation */
        Gbe->MacDmaConfig.Speed = AL_GBE_SPEED_100M;
    } else {
        Gbe->MacDmaConfig.Speed = AL_GBE_SPEED_10M;
    }

    return AL_OK;
}

AL_S32 AlGbe_Dev_StartMacDmaIntr(AL_GBE_DevStruct *Gbe)
{
    if (Gbe == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    if ((Gbe->State & AL_GBE_STATE_READY) == 0) {
        return AL_GBE_ERR_NOT_READY;
    }

    AL_GBE_DMADescStruct *DmaRxDesc;

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    /* save IT mode to ETH Handle */
    Gbe->RxDescList.ItMode = 1U;

    /* Set IOC bit to all Rx descriptors */
    for (AL_S32 DescIndex = 0; DescIndex < AL_GBE_RX_DESC_CNT; DescIndex++) {
        DmaRxDesc = (AL_GBE_DMADescStruct *)Gbe->RxDescList.RxDesc[DescIndex];
        AlGbe_ll_SetRdesc3CompleteIntr(&(DmaRxDesc->DESC3), AL_GBE_FUNC_ENABLE);
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

AL_S32 AlGbe_Dev_StartMacDma(AL_GBE_DevStruct *Gbe)
{
    if (Gbe == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    if ((Gbe->State & AL_GBE_STATE_READY) == 0) {
        return AL_GBE_ERR_NOT_READY;
    }

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

AL_S32 AlGbe_Dev_IsRxDataAvailable(AL_GBE_DevStruct *Gbe)
{
    AL_GBE_RxDescListStruct *DmaRxDescList = &Gbe->RxDescList;
    AL_U32 DescIndex = DmaRxDescList->CurRxDesc;
    AL_GBE_DMADescStruct *DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];
    AL_U32 Descscancnt = 0;
    AL_U32 Appdesccnt = 0, FirstAppDescidx = 0;
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    if (DmaRxDescList->AppDescNbr != 0U) {
        /* data already received by not yet processed*/
        return 0;
    }

    /* Check if descriptor is not owned by DMA */
    while ((!AlGbe_ll_IsWbRxDescOwnByDma(&DmaRxDesc->DESC3)) && (Descscancnt < (AL_U32)AL_GBE_RX_DESC_CNT)) {
        Descscancnt++;

        /* Check if last descriptor */
        if (AlGbe_ll_IsWbRxDescLastDesc(&DmaRxDesc->DESC3)) {
            /* Increment the number of descriptors to be passed to the application */
            Appdesccnt += 1U;

            if (Appdesccnt == 1U) {
                FirstAppDescidx = DescIndex;
            }

            /* Increment current rx descriptor index */
            INCR_RX_DESC_INDEX(DescIndex, 1U);

            /* Check for Context descriptor */
            /* Get current descriptor address */
            DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];

            if (!AlGbe_ll_IsWbRxDescOwnByDma(&DmaRxDesc->DESC3)) {
                if (AlGbe_ll_GetWbRecvContextDesc(&DmaRxDesc->DESC3)) {
                    /* Increment the number of descriptors to be passed to the application */
                    DmaRxDescList->AppContextDesc = 1;
                    /* Increment current rx descriptor index */
                    INCR_RX_DESC_INDEX(DescIndex, 1U);
                }
            }
            /* Fill information to Rx descriptors list */
            DmaRxDescList->CurRxDesc = DescIndex;
            DmaRxDescList->FirstAppDesc = FirstAppDescidx;
            DmaRxDescList->AppDescNbr = Appdesccnt;

            /* Return function status */
            return 1;

        } else if (AlGbe_ll_IsWbRxDescFirstDesc(&DmaRxDesc->DESC3)) {
            /* Check if first descriptor */
            FirstAppDescidx = DescIndex;
            /* Increment the number of descriptors to be passed to the application */
            Appdesccnt = 1U;

            /* Increment current rx descriptor index */
            INCR_RX_DESC_INDEX(DescIndex, 1U);
            /* Get current descriptor address */
            DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];

        } else {
            /* It should be an intermediate descriptor */
            /* Increment the number of descriptors to be passed to the application */
            Appdesccnt += 1U;

            /* Increment current rx descriptor index */
            INCR_RX_DESC_INDEX(DescIndex, 1U);
            /* Get current descriptor address */
            DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];
        }
    }

    /* Build Descriptors if an incomplete Packet is received */
    if (Appdesccnt > 0U) {
        DmaRxDescList->CurRxDesc = DescIndex;
        DmaRxDescList->FirstAppDesc = FirstAppDescidx;
        DescIndex = FirstAppDescidx;
        DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];

        for (Descscancnt = 0; Descscancnt < Appdesccnt; Descscancnt++) {
            DmaRxDesc->DESC0 = DmaRxDesc->BackupAddr0;
            AlGbe_ll_SetRdesc3Buff1Valid(&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);

            if (DmaRxDesc->BackupAddr1 != 0) {
                DmaRxDesc->DESC2 = DmaRxDesc->BackupAddr1;
                AlGbe_ll_SetRdesc3Buff2Valid(&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);
            }

            AlGbe_ll_SetRdesc3OwnByDma(&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);

            if (DmaRxDescList->ItMode != 0) {
                AlGbe_ll_SetRdesc3CompleteIntr(&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);
            }
            if (Descscancnt < (Appdesccnt - 1U)) {
                /* Increment rx descriptor index */
                INCR_RX_DESC_INDEX(DescIndex, 1U);
                /* Get descriptor address */
                DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];
            }
        }

        /* Set the Tail pointer address to the last rx descriptor hold by the app */
        AlGbe_ll_SetDmaRxDescTailPointer(GbeBaseAddr, DmaRxDesc);
    }

    /* Fill information to Rx descriptors list: No received Packet */
    DmaRxDescList->AppDescNbr = 0U;

    return AL_OK;
}

AL_S32 AlGbe_Dev_GetRxDataBuffer(AL_GBE_DevStruct *Gbe, AL_GBE_BufferStruct *RxBuffer)
{
    AL_GBE_RxDescListStruct *DmaRxDescList = &Gbe->RxDescList;
    AL_U32 DescIndex = DmaRxDescList->FirstAppDesc;
    AL_U32 Index, AccumulatedLen = 0, LastDescLen;
    __IO const AL_GBE_DMADescStruct *DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];
    AL_GBE_BufferStruct *RxBuff = RxBuffer;

    if (RxBuff == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    if (DmaRxDescList->AppDescNbr == 0U) {
        if (AlGbe_Dev_IsRxDataAvailable(Gbe) == 0U) {
        /* No data to be transferred to the application */
            return AL_GBE_ERR_FATLA_BUS_ERROR;
        } else {
            DescIndex = DmaRxDescList->FirstAppDesc;
            DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];
        }
    }

    /* Get intermediate descriptors buffers: in case of the Packet is split into multi descriptors */
    for (Index = 0; Index < (DmaRxDescList->AppDescNbr - 1U); Index++) {
        /* Get Address and length of the first buffer address */
        RxBuff->Buffer = (AL_U8 *) DmaRxDesc->BackupAddr0;
        RxBuff->Len =  Gbe->InitConfig.RxBuffLen;

        /* Check if the second buffer address of this descriptor is valid */
        if (DmaRxDesc->BackupAddr1 != 0U) {
            /* Point to next buffer */
            RxBuff = RxBuff->next;
            /* Get Address and length of the second buffer address */
            RxBuff->Buffer = (AL_U8 *) DmaRxDesc->BackupAddr1;
            RxBuff->Len =  Gbe->InitConfig.RxBuffLen;
        } else {
            /* Nothing to do here */
        }

        /* get total length until this descriptor */
        AccumulatedLen = AlGbe_ll_GetWbRdesc3Packetlen(&DmaRxDesc->DESC3);

        /* Increment to next descriptor */
        INCR_RX_DESC_INDEX(DescIndex, 1U);
        DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];

        /* Point to next buffer */
        RxBuff = RxBuff->next;
    }

    /* last descriptor data length */
    LastDescLen = AlGbe_ll_GetWbRdesc3Packetlen(&DmaRxDesc->DESC3) - AccumulatedLen;

    /* Get Address of the first Buffer address */
    RxBuff->Buffer = (AL_U8 *) DmaRxDesc->BackupAddr0;

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
        RxBuff->Buffer = (AL_U8 *) DmaRxDesc->BackupAddr1;
        RxBuff->Len =  LastDescLen - (Gbe->InitConfig.RxBuffLen);
    } else {
        /* Buffer 2 not valid*/
        return AL_GBE_ERR_FATLA_BUS_ERROR;
    }

    return AL_OK;
}

/**
  * @brief  This function gets the length of last received Packet.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  Length: parameter to hold Rx packet length
  * @retval HAL Status
  */
AL_S32 AlGbe_Dev_GetRxDataLength(AL_GBE_DevStruct *Gbe, AL_U32 *Length)
{
    AL_GBE_RxDescListStruct *DmaRxDescList = &Gbe->RxDescList;
    AL_U32 DescIndex = DmaRxDescList->FirstAppDesc;
    __IO const AL_GBE_DMADescStruct *DmaRxDesc;

    if (DmaRxDescList->AppDescNbr == 0U) {
        if (AlGbe_Dev_IsRxDataAvailable(Gbe) == 0U) {
            /* No data to be transferred to the application */
            return AL_GBE_ERR_FATLA_BUS_ERROR;
        }
    }

    /* Get index of last descriptor */
    INCR_RX_DESC_INDEX(DescIndex, (DmaRxDescList->AppDescNbr - 1U));
    /* Point to last descriptor */
    DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];

    *Length = AlGbe_ll_GetWbRdesc3Packetlen(&DmaRxDesc->DESC3);

    return AL_OK;
}

AL_S32 AlGbe_Dev_BuildRxDescriptors(AL_GBE_DevStruct *Gbe)
{
    AL_GBE_RxDescListStruct *DmaRxDescList = &Gbe->RxDescList;
    AL_U32 DescIndex = DmaRxDescList->FirstAppDesc;
    __IO AL_GBE_DMADescStruct *DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];
    AL_U32 TotalAppdescNbr = DmaRxDescList->AppDescNbr;
    AL_U32 Descscan;

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
        AlGbe_ll_SetRdesc3Buff1Valid(&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);

        if (DmaRxDesc->BackupAddr1 != 0U) {
            DmaRxDesc->DESC2 = DmaRxDesc->BackupAddr1;
            AlGbe_ll_SetRdesc3Buff2Valid(&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);
        }

        AlGbe_ll_SetRdesc3OwnByDma(&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);

        if (DmaRxDescList->ItMode != 0U) {
            AlGbe_ll_SetRdesc3CompleteIntr(&DmaRxDesc->DESC3, AL_GBE_FUNC_ENABLE);
        }

        if (Descscan < (TotalAppdescNbr - 1U)) {
            /* Increment rx descriptor index */
            INCR_RX_DESC_INDEX(DescIndex, 1U);
            /* Get descriptor address */
            DmaRxDesc = (AL_GBE_DMADescStruct *)DmaRxDescList->RxDesc[DescIndex];
        }
    }

    /* Set the Tail pointer address to the last rx descriptor hold by the app */
    AlGbe_ll_SetDmaRxDescTailPointer(GbeBaseAddr, (AL_U32)DmaRxDesc);

    /* reset the Application desc number */
    DmaRxDescList->AppDescNbr = 0;

    /*  reset the application context descriptor */
    Gbe->RxDescList.AppContextDesc = 0;

    return AL_OK;
}

static AL_S32 AlGbe_Dev_PrepareTxDescriptors(AL_GBE_DevStruct *Gbe, AL_GBE_TxDescConfigStruct *TxConfig, AL_GBE_FunctionEnum IntrEnable)
{
    AL_GBE_TxDescListStruct *DmaTxDescList = &Gbe->TxDescList;
    AL_U32 DescIndex = DmaTxDescList->CurTxDesc;
    AL_U32 FirstDescIdx = DmaTxDescList->CurTxDesc;
    AL_U32 Index;
    AL_U32 DescNbr = 0;
    AL_GBE_DMADescStruct *DmaTxDesc = (AL_GBE_DMADescStruct *)DmaTxDescList->TxDesc[DescIndex];

    AL_GBE_BufferStruct  *TxBuffer = TxConfig->TxBuffer;
    AL_U32               BdCount = 0;

    /* Current Tx Descriptor Owned by DMA: cannot be used by the application  */
    if ((AlGbe_ll_IsTxDescOwnByDma(&DmaTxDesc->DESC3) == AL_TRUE) ||
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

    /* Set header or buffer 1 address */
    DmaTxDesc->DESC0 = (AL_U32)TxBuffer->Buffer;
    /* Set header or buffer 1 Length */
    AlGbe_ll_SetTdesc2Buffer1Len(&DmaTxDesc->DESC2, TxBuffer->Len);

    if (TxBuffer->next != AL_NULL) {
        TxBuffer = TxBuffer->next;
        /* Set buffer 2 address */
        DmaTxDesc->DESC1 = (AL_U32)TxBuffer->Buffer;
        /* Set buffer 2 Length */
        AlGbe_ll_SetTdesc2Buffer2Len(&DmaTxDesc->DESC2, TxBuffer->Len);
    } else {
        DmaTxDesc->DESC1 = 0x0;
        /* Set buffer 2 Length */
        AlGbe_ll_SetTdesc2Buffer2Len(&DmaTxDesc->DESC2, 0x0U);
    }

    AlGbe_ll_SetTdesc3FrameLen(&DmaTxDesc->DESC3, TxConfig->Length);

    /* Set Checksum Insertion Control */
    //AlGbe_ll_SetChksumInsertCtrl(&DmaTxDesc->DESC3, TxConfig->ChecksumCtrl);

    /* Mark it as First Descriptor */
    AlGbe_ll_SetTdesc3FirstDesc(&DmaTxDesc->DESC3, AL_GBE_FUNC_ENABLE);
    /* Mark it as NORMAL descriptor */
    AlGbe_ll_SetTdesc3ContextType(&DmaTxDesc->DESC3, AL_GBE_DESC_NORMAL_DESC);


    /* Ensure rest of descriptor is written to RAM before the OWN bit */
    //__DMB();

    /* set OWN bit of FIRST descriptor */
    AlGbe_ll_SetTdesc3OwnByDma(&DmaTxDesc->DESC3, AL_GBE_FUNC_ENABLE);

  /* only if the packet is split into more than one descriptors > 1 */
    while (TxBuffer->next != AL_NULL) {
        /* Clear the LD bit of previous descriptor */
        AlGbe_ll_SetTdesc3LastDesc(&DmaTxDesc->DESC3, AL_GBE_FUNC_DISABLE);
        /* Increment current tx descriptor index */
        INCR_TX_DESC_INDEX(DescIndex, 1U);
        /* Get current descriptor address */
        DmaTxDesc = (AL_GBE_DMADescStruct *)DmaTxDescList->TxDesc[DescIndex];


      /* Current Tx Descriptor Owned by DMA: cannot be used by the application  */
        if ((AlGbe_ll_IsTxDescOwnByDma(&DmaTxDesc->DESC3) == AL_TRUE) ||
          (DmaTxDescList->PacketAddress[DescIndex] != AL_NULL)) {
          DescIndex = FirstDescIdx;
          DmaTxDesc = (AL_GBE_DMADescStruct *)DmaTxDescList->TxDesc[DescIndex];

            /* clear previous desc own bit */
            for (AL_S32 Idx = 0; Idx < DescNbr; Idx ++) {
                /* Ensure rest of descriptor is written to RAM before the OWN bit */
                //__DMB();

                AlGbe_ll_SetTdesc3OwnByDma(&DmaTxDesc->DESC3, AL_GBE_FUNC_DISABLE);

                /* Increment current tx descriptor index */
                INCR_TX_DESC_INDEX(DescIndex, 1U);
                /* Get current descriptor address */
                DmaTxDesc = (AL_GBE_DMADescStruct *)DmaTxDescList->TxDesc[DescIndex];
            }

            return AL_GBE_ERR_DESC_STATE;
        }

        /* Clear the FD bit of new Descriptor */
        AlGbe_ll_SetTdesc3FirstDesc(&DmaTxDesc->DESC3, AL_GBE_FUNC_DISABLE);

        DescNbr += 1U;

        /* Get the next Tx buffer in the list */
        TxBuffer = TxBuffer->next;

        /* Set header or buffer 1 address */
        DmaTxDesc->DESC0 = (AL_U32)TxBuffer->Buffer;
        /* Set header or buffer 1 Length */
        AlGbe_ll_SetTdesc2Buffer1Len(&DmaTxDesc->DESC2, TxBuffer->Len);

        if (TxBuffer->next != AL_NULL) {
            /* Get the next Tx buffer in the list */
            TxBuffer = TxBuffer->next;
            /* Set buffer 2 address */
            DmaTxDesc->DESC1 = (AL_U32)TxBuffer->Buffer;
            /* Set buffer 2 Length */
            AlGbe_ll_SetTdesc2Buffer2Len(&DmaTxDesc->DESC2, TxBuffer->Len);
        } else {
            DmaTxDesc->DESC1 = 0x0;
            /* Set buffer 2 Length */
            AlGbe_ll_SetTdesc2Buffer2Len(&DmaTxDesc->DESC2, 0x0U);
        }

        /* Set the packet length */
        AlGbe_ll_SetTdesc3FrameLen(&DmaTxDesc->DESC3, TxConfig->Length);


        /* Set Checksum Insertion Control */
        //AlGbe_ll_SetChksumInsertCtrl(&DmaTxDesc->DESC3, TxConfig->ChecksumCtrl);

        BdCount += 1U;

        /* Ensure rest of descriptor is written to RAM before the OWN bit */
    //    __DMB();
        /* Set Own bit */
        AlGbe_ll_SetTdesc3OwnByDma(&DmaTxDesc->DESC3, AL_GBE_FUNC_ENABLE);
        /* Mark it as NORMAL descriptor */
        AlGbe_ll_SetTdesc3ContextType(&DmaTxDesc->DESC3, AL_GBE_DESC_NORMAL_DESC);
    }

    if (IntrEnable == AL_GBE_FUNC_ENABLE) {
        /* Set Interrupt on completion bit */
        AlGbe_ll_SetTdesc2CompleteIntr(&DmaTxDesc->DESC2, AL_GBE_FUNC_ENABLE);
    } else {
        /* Clear Interrupt on completion bit */
        AlGbe_ll_SetTdesc2CompleteIntr(&DmaTxDesc->DESC2, AL_GBE_FUNC_DISABLE);
    }

    /* Mark it as LAST descriptor */
    AlGbe_ll_SetTdesc3LastDesc(&DmaTxDesc->DESC3, AL_GBE_FUNC_ENABLE);
    /* Save the current packet address to expose it to the application */
    DmaTxDescList->PacketAddress[DescIndex] = DmaTxDescList->CurrentPacketAddress;

    DmaTxDescList->CurTxDesc = DescIndex;

    /* disable the interrupt */
#ifdef CHIP == dr1m90
    __disable_irq();
#else

#endif

    DmaTxDescList->BuffersInUse += BdCount + 1U;

    /* Enable interrupts back */
#ifdef CHIP == dr1m90
    __enable_irq();
#else

#endif

    return AL_OK;
}


// extern void HAL_ETH_TxFreeCallback(AL_U32 * buff);

AL_S32 AlGbe_Dev_ReleaseTxPacket(AL_GBE_DevStruct *Gbe)
{
    AL_GBE_TxDescListStruct *DmaTxDescList = &Gbe->TxDescList;
    AL_U32 NumOfBuff =  DmaTxDescList->BuffersInUse;
    AL_U32 Index =       DmaTxDescList->ReleaseIndex;
    AL_U8 PktTxStatus = 1U;
    AL_U8 PktInUse;

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
            if (!AlGbe_ll_IsTxDescOwnByDma(&(Gbe->InitConfig.TxDescList[Index].DESC3))) {

                /* Release the packet.  */
                // HAL_ETH_TxFreeCallback(DmaTxDescList->PacketAddress[Index]);

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

AL_S32 AlGbe_Dev_Transmit(AL_GBE_DevStruct *Gbe, AL_GBE_TxDescConfigStruct *TxConfig)
{
    AL_S32 Ret;

    if (Gbe == AL_NULL || TxConfig == 0) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    if ((Gbe->State & AL_GBE_STATE_READY) == 0) {
        return AL_GBE_ERR_NOT_READY;
    }

    if (AlGbe_Dev_IsTxBusy(Gbe)) {
        return AL_GBE_ERR_BUSY;
    }

    /* Change Status */
//    AlGbe_Dev_SetTxBusy(Gbe);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    Gbe->TxDescList.CurrentPacketAddress = (AL_U32 *)TxConfig->pData;

    Ret = AlGbe_Dev_PrepareTxDescriptors(Gbe, TxConfig, AL_GBE_FUNC_ENABLE);
    if (Ret != AL_OK) {
        return Ret;
    }

    INCR_TX_DESC_INDEX(Gbe->TxDescList.CurTxDesc, 1U);

    /* Start transmission */
    /* issue a poll command to Tx DMA by writing address of next immediate free descriptor */
    AlGbe_ll_SetDmaTxDescTailPointer(GbeBaseAddr, (AL_U32)(Gbe->TxDescList.TxDesc[Gbe->TxDescList.CurTxDesc]));

    return AL_OK;
}

AL_S32 AlGbe_Dev_TransmitPolling(AL_GBE_DevStruct *Gbe, AL_GBE_TxDescConfigStruct *TxConfig)
{
    AL_S32 Ret;
    const AL_GBE_DMADescStruct *DmaTxDesc;

    if (Gbe == AL_NULL || TxConfig == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    if ((Gbe->State & AL_GBE_STATE_READY) == 0) {
        return AL_GBE_ERR_NOT_READY;
    }

    if (AlGbe_Dev_IsTxBusy(Gbe)) {
        return AL_GBE_ERR_BUSY;
    }

    /* Change Status */
    AlGbe_Dev_SetTxBusy(Gbe);

    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);

    DmaTxDesc = (AL_GBE_DMADescStruct *)(&Gbe->TxDescList)->TxDesc[Gbe->TxDescList.CurTxDesc];

    Ret = AlGbe_Dev_PrepareTxDescriptors(Gbe, TxConfig, AL_GBE_FUNC_DISABLE);
    if (Ret != AL_OK) {
        return Ret;
    }

    INCR_TX_DESC_INDEX(Gbe->TxDescList.CurTxDesc, 1U);

    /* Start transmission */
    /* issue a poll command to Tx DMA by writing address of next immediate free descriptor */
    AlGbe_ll_SetDmaTxDescTailPointer(GbeBaseAddr, (AL_U32)(Gbe->TxDescList.TxDesc[Gbe->TxDescList.CurTxDesc]));

    /* Wait for data to be transmitted or timeout occurred */
    while (AlGbe_ll_IsWbTxDescOwnByDma(&DmaTxDesc->DESC3)) {
        if (AlGbe_ll_IsDmaChannelFatalBusError(GbeBaseAddr)) {
            return AL_GBE_ERR_FATLA_BUS_ERROR;
        }
    }

    AlGbe_Dev_ClrTxBusy(Gbe);

    return AL_OK;
}

static AL_VOID AlGbe_Dev_ErrorHandler(AL_GBE_DevStruct *Gbe)
{
    /*
       1. if support OS, need tell the rx task, use SemaphoreRelease here
       2. if not support OS, How to do?
     */

}

static AL_VOID AlGbe_Dev_TxHandler(AL_GBE_DevStruct *Gbe)
{
    /*
       1. if support OS, need tell the rx task, use SemaphoreRelease here
       2. if not support OS, How to do?
     */

}

static AL_VOID AlGbe_Dev_RxHandler(AL_GBE_DevStruct *Gbe)
{
    /*
       1. if support OS, need tell the rx task, use SemaphoreRelease here
       2. if not support OS, How to do?
     */

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

// extern void HAL_ETH_TxDoneCallback();
// extern void HAL_ETH_RxDoneCallback();

AL_VOID AlGbe_Dev_IntrHandler(AL_VOID *Instance)
{
    AL_GBE_DevStruct *Gbe = (AL_GBE_DevStruct *)Instance;
    AL_REG GbeBaseAddr = (AL_REG)(Gbe->HwConfig.BaseAddress);
    AL_GBE_IntrStatusEnum IntrStatus = AlGbe_ll_GetDmaChannelStatus(GbeBaseAddr);

    if (GBE_IN_RX_COMPLETE_INTR(IntrStatus)) {
        AlGbe_Dev_RxHandler(Gbe);

        AlGbe_ll_ClrRxCompletrIntr(GbeBaseAddr);
        AlGbe_ll_ClrNormalSummaryIntr(GbeBaseAddr);

        // HAL_ETH_RxDoneCallback();

    }

    if (GBE_IN_TX_COMPLETE_INTR(IntrStatus)) {
        AlGbe_Dev_TxHandler(Gbe);

        AlGbe_ll_ClrTxCompletrIntr(GbeBaseAddr);
        AlGbe_ll_ClrNormalSummaryIntr(GbeBaseAddr);

        // HAL_ETH_TxDoneCallback();
    }

    if (GBE_IN_ABNORMAL_SUMMARY_INTR(IntrStatus)) {
        AlGbe_Dev_ErrorHandler(Gbe);
        AlGbe_ll_ClrAbnormalSummaryIntr(GbeBaseAddr);

    }

#if 0
    if (GBE_IN_NORMAL_SUMMARY_INTR(IntrStatus)) {
        AlGbe_ll_ClrNormalSummaryIntr(GbeBaseAddr);

    }

    if (GBE_IN_TX_BUFFER_UNAVAILABLE_INTR(IntrStatus)) {
        AlGbe_ll_ClrTxBufferUnvailableIntr(GbeBaseAddr);

    }

    if (GBE_IN_EARLY_TX_INTR(IntrStatus)) {

        AlGbe_ll_ClrEarlyTxIntr(GbeBaseAddr);
        AlGbe_ll_ClrAbnormalSummaryIntr(GbeBaseAddr);

    }
#endif
}

AL_S32 AlGbe_Dev_RegisterEventCallBack(AL_GBE_DevStruct *Gbe, AL_GBE_EventCallBack Callback, void *CallbackRef)
{
    if (Gbe == AL_NULL || Callback == AL_NULL) {
        return AL_GBE_ERR_ILLEGAL_PARAM;
    }

    Gbe->EventCallBack        = Callback;
    Gbe->EventCallBackRef     = CallbackRef;

    return AL_OK;
}
