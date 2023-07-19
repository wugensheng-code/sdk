/***************************** Include Files *********************************/
#include "al_qspi_dev.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
/* Default init config */

/**
 *_____________________________________________________________________________________________________________________________
 *          |                  x1 instruction/address         |                  x2/x4 instruction/address                    |
 *   DFS    |          24-bit address        | 32-bit address |        24-bit address       |           32-bit address        |
 *__________|________________________________|________________|_____________________________|_________________________________|
 *          |         {24’b0,i[7:0]}         |{24’b0,i[7:0]}  |      {24’b0,i[7:0]}         |           {24’b0,i[7:0]}        |
 *          |         {24’b0,a[23:16]}       |{24’b0,a[31:24]}|      {8’b0,a[23:0]}         |           {a[31:0]}             |
 *  8-bit   |         {24’b0,a[15:8]}        |{24’b0,a[23:16]}|                             |                                 |
 *          |         {24’b0,a[7:0]}         |{24’b0,a[15:8]} |                             |                                 |
 *          |                                |{24’b0,a[7:0]}  |                             |                                 |
 *__________|________________________________|________________|_____________________________|_________________________________|
 *          |    {16’b0,a[23:16],i[7:0]}     |                |     {16’b0,i[7:0],8’b0}     |       {16’b0,i[7:0],8’b0}       |
 *  16-bit  |    {16’b0,a[7:0],a[15:8]}      |   No Support   |{a[23:16],8’b0,a[7:0],[15:8]}|{a[23:16],a[31:24],a[7:0],[15:8]}|
 * _________|________________________________|________________|_____________________________|_________________________________|
 *          |                                |                |       {i[7:0],24’b0}        |          {i[7:0],24’b0}         |
 *  32-bit  |{a[7:0],a[15:8],a[23:16],i[7:0]}|   No Support   |{a[7:0],[15:8],a[23:16],8’b0}|{a[7:0],[15:8],a[23:16],a[31:24]}|
 * _________|________________________________|________________|_____________________________|_________________________________|
 *
*/
/* When SpiFrameFormat is Dual Quad,TransMode can only use QSPI_TX_ONLY,QSPI_RX_ONLY mode */
static AL_QSPI_ConfigsStruct QSPIDefInitConfigs =
{
    .TransMode          = QSPI_TX_ONLY,
    .SpiFrameFormat     = SPI_STANDARD_FORMAT,
    .DataFrameSize      = QSPI_DFS_8BITS,
    .EnSpiCfg.AddrLength    = QSPI_ADDR_L32,
    .EnSpiCfg.InstLength    = QSPI_INST_L8,
    .EnSpiCfg.TransType     = QSPI_TT0,
    .EnSpiCfg.WaitCycles    = 8,
    .ClkDiv             = 70,
    .ClockStretch       = QSPI_EnableClockStretch,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SlvSelEnum         = QSPI_SER_SS0_EN,
    .IsUseDma           = AL_QSPI_USE_INTR
};

extern AL_QSPI_HwConfigStruct AlQSPI_HwCfg[AL_QSPI_NUM_INSTANCE];
/************************** Function Prototypes ******************************/

/********************************************************/
/**
 * This function look up hardware config structure
 * @param   DeviceId is hardware module id
 * @return  hardware config structure pointer with AL_QSPI_HwConfigStruct
 * @note    None
*/
AL_QSPI_HwConfigStruct *AlQspi_Dev_LookupConfig(AL_U32 DeviceId)
{
    AL_U32 Index;
    AL_QSPI_HwConfigStruct *CfgPtr = AL_NULL;

    for (Index = 0; Index < AL_QSPI_NUM_INSTANCE; Index++) {
        if (AlQSPI_HwCfg[Index].DeviceId == DeviceId) {
            CfgPtr = &AlQSPI_HwCfg[Index];
            break;
        }
    }

    return CfgPtr;
}

/**
 * This function look up hardware config structure
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
AL_BOOL AlQspi_Dev_IsTxBusy(AL_QSPI_DevStruct *Qspi)
{
    return (AL_BOOL)(Qspi->State & AL_QSPI_STATE_TX_BUSY);
}

/**
 * This function is set qspi AL_QSPI_DevStruct struct rx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
AL_BOOL AlQspi_Dev_IsRxBusy(AL_QSPI_DevStruct *Qspi)
{
    return (AL_BOOL)(Qspi->State & AL_QSPI_STATE_RX_BUSY);
}

/**
 * This function is set qspi AL_QSPI_DevStruct struct tx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQspi_Dev_SetTxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State |= AL_QSPI_STATE_TX_BUSY;
}

/**
 * This function is clear qspi AL_QSPI_DevStruct struct rx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQspi_Dev_SetRxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State |= AL_QSPI_STATE_RX_BUSY;
}

/**
 * This function is clear qspi AL_QSPI_DevStruct struct tx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQspi_Dev_ClrTxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State &= (~AL_QSPI_STATE_TX_BUSY);
}

/**
 * This function is clear qspi AL_QSPI_DevStruct struct rx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQspi_Dev_ClrRxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State &= (~AL_QSPI_STATE_RX_BUSY);
}

// #ifdef QSPI_DEBUG
/**
 * This function prints the values of all registers in the qspi
 * @param   QSPIBaseAddr is qspi base address
 * @return  AL_VOID
 * @note    None
*/
AL_VOID AlQspi_Dev_DumpReg(AL_REG QSPIBaseAddr)
{
    AL_LOG(AL_LOG_LEVEL_DEBUG, "CTRLR0:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_CTRLR0_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "CTRLR1:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_CTRLR1_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SSIENR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_SSIENR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "MWCR:0x%x\r\n",    AL_REG32_READ(QSPIBaseAddr + QSPI_MWCR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SER:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_SER_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "BAUDR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_BAUDR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXFTLR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_TXFTLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXFTLR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_RXFTLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXFLR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_TXFLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXFLR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_RXFLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SR:0x%x\r\n",      AL_REG32_READ(QSPIBaseAddr + QSPI_SR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "IMR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_IMR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "ISR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_ISR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RISR:0x%x\r\n",    AL_REG32_READ(QSPIBaseAddr + QSPI_RISR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXOICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_TXOICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXOICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_RXOICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXUICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_RXUICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "MSTICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_MSTICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "ICR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_ICR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMACR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_DMACR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMATDLR:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_DMATDLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMARDLR:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_DMARDLR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "IDR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_IDR_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SSIC VERSION ID:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_SSIC_VERSION_ID_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RX SAMPLE DELAY:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_RX_SAMPLE_DELAY_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SPI CTRLR0:0x%x\r\n",      AL_REG32_READ(QSPIBaseAddr + QSPI_SPI_CTRLR0_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "XIP MODE BITS:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_XIP_MODE_BITS_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "XIP INCR INST:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_XIP_INCR_INST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "XIP WRAP INST:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_XIP_WRAP_INST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "XIP CNT TIME OUT:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_XIP_CNT_TIME_OUT_OFFSET));

}
// #endif

/**
 * This function initializes qspi through the AL_QSPI_ConfigsStruct struct
 * @param   Qspi is structure pointer to qspi device
 * @param   InitConfig is qspi configuration parameter struct
 * @return  Whether the qspi is initialized successfully
 * @note    if InitConfig is null will use QSPIDefInitConfigs
*/
AL_S32 AlQspi_Dev_Init(AL_QSPI_DevStruct *Qspi, AL_QSPI_ConfigsStruct *InitConfig)
{
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    Qspi->Configs = (InitConfig == AL_NULL) ? QSPIDefInitConfigs : (*InitConfig);

    AlQspi_ll_Disable(Qspi->BaseAddr);
    AlQspi_ll_SetClockStretch(Qspi->BaseAddr, Qspi->Configs.ClockStretch);
    // AlQspi_ll_SetDataFrameSize(Qspi->BaseAddr, Qspi->Configs.DataFrameSize);
    AlQspi_ll_SetClkDiv(Qspi->BaseAddr, Qspi->Configs.ClkDiv);
    AlQspi_ll_SetSlvSelToggle(Qspi->BaseAddr, Qspi->Configs.SlvToggleEnum);
    // AlQspi_ll_SetRecvDataSamplDelay(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.);
    // AlQspi_ll_SetRecvDataSamplEdge(Qspi->BaseAddr, Qspi->Configs.SlvToggleEnum);
    AlQspi_ll_MaskIntr(Qspi->BaseAddr, QSPI_TXEIM | QSPI_RXFIM);
    AlQspi_ll_SetXipModeBit(Qspi->BaseAddr, QSPI_XipModeBitDisable);
    AlQspi_ll_SetXipDfsFix(Qspi->BaseAddr, QSPI_XipDfsChange);
    AlQspi_ll_SetXipMbl(Qspi->BaseAddr, QSPI_MBL_4);
    AlQspi_ll_SetXipInstPhase(Qspi->BaseAddr, QSPI_XipInstPhaseDisable);
    AlQspi_ll_SetXipPrefetch(Qspi->BaseAddr, QSPI_DisableXipPrefetch);
    AlQspi_ll_SetXipContTrans(Qspi->BaseAddr, QSPI_DisableXipContTrans);
    AlQspi_ll_Enable(Qspi->BaseAddr);

    Qspi->State |= AL_QSPI_STATE_READY;

 //   AlStack_Dump();

    return AL_OK;
}
//  Qspi->Configs.XipCfg
/**
 * This function is qspi send data
 * @param   Qspi is structure pointer to qspi device
 * @param   SendBuf is send data buffer pointer
 * @param   SendSize is receive data size
 * @return  Whether the qspi send data successfully
 * @note    None
*/
AL_S32 AlQspi_Dev_SendData(AL_QSPI_DevStruct *Qspi, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U32 SendLevel, SendValue = 0xFFFFFFFF, TempSendSize;

    if (Qspi == AL_NULL || SendBuf == AL_NULL || SendSize == 0) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_TXFIFO_FULL == AlQspi_ll_IsTxFifoFull(Qspi->BaseAddr)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQspi_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQspi_Dev_SetTxBusy(Qspi);

    TempSendSize = SendSize - (Qspi->Configs.EnSpiCfg.AddrLength + Qspi->Configs.EnSpiCfg.InstLength) / 2;
    /* Different fifo widths are set depending on the amount of data */
    if(QSPI_ADDR_L0 != Qspi->Configs.EnSpiCfg.AddrLength) {
        if ((0 == (TempSendSize % 4)) && (0 != TempSendSize)) {
            Qspi->Configs.DataFrameSize = QSPI_DFS_32BITS;
            Qspi->BitsPerWord = 32;
        } else if ((0 == (TempSendSize % 2)) && (0 != TempSendSize)) {
            Qspi->Configs.DataFrameSize = QSPI_DFS_16BITS;
            Qspi->BitsPerWord = 16;
        } else {
            Qspi->Configs.DataFrameSize = QSPI_DFS_8BITS;
            Qspi->BitsPerWord = 8;
        }
    } else {
        Qspi->Configs.DataFrameSize = QSPI_DFS_8BITS;
        Qspi->BitsPerWord = 8;
    }

    Qspi->SendBuffer.BufferPtr    = SendBuf;
    Qspi->SendBuffer.HandledCnt   = 0;
    Qspi->SendBuffer.RequestedCnt = SendSize;

    AlQspi_ll_Disable(Qspi->BaseAddr);
    AlQspi_ll_SetDataFrameSize(Qspi->BaseAddr, Qspi->Configs.DataFrameSize);
    AlQspi_ll_SetAddrLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->BaseAddr, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->BaseAddr, Qspi->Configs.TransMode);

    SendLevel = MIN((AL_U32)(Qspi->Fifolen / 2), TempSendSize / (Qspi->BitsPerWord >> 3));
    if(SPI_STANDARD_FORMAT != Qspi->Configs.SpiFrameFormat) {
        SendLevel += 2;
    }
    AlQspi_ll_SetTxStartFifoLevel(Qspi->BaseAddr, SendLevel ? SendLevel - 1 : 0);
    AlQspi_ll_Enable(Qspi->BaseAddr);

    if(Qspi->Configs.SpiFrameFormat == SPI_STANDARD_FORMAT) {
        switch (Qspi->Configs.DataFrameSize)
        {
        case QSPI_DFS_32BITS:
            if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U32 *)(Qspi->SendBuffer.BufferPtr));
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "Qspi not support this mode combination\r\n");
            }
            break;
        case QSPI_DFS_16BITS:
            if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U16 *)(Qspi->SendBuffer.BufferPtr));
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + 2));
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "Qspi not support this mode combination\r\n");
            }
            break;
        case QSPI_DFS_8BITS:
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr));
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 1));
            }
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L8) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 2));
            }
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L16) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 3));
            }
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L24) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 4));
            }
            break;
        default:
            AL_LOG(AL_LOG_LEVEL_INFO, "DFS is set error in Qspi\r\n");
            break;
        }
    } else {
        switch (Qspi->Configs.DataFrameSize)
        {
        case QSPI_DFS_32BITS:
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[0] << 24);
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 8 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                    Qspi->SendBuffer.BufferPtr[3] << 24);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] | Qspi->SendBuffer.BufferPtr[2] << 8 |
                    Qspi->SendBuffer.BufferPtr[3] << 16 | Qspi->SendBuffer.BufferPtr[4] << 24);
                }
            }
            break;
        case QSPI_DFS_16BITS:
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[0] << 8);
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] |
                    Qspi->SendBuffer.BufferPtr[3] << 8);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 24 |
                    Qspi->SendBuffer.BufferPtr[3] | Qspi->SendBuffer.BufferPtr[4] << 8);
                }
            }
            break;
        case QSPI_DFS_8BITS:
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[0]);
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 8 |
                    Qspi->SendBuffer.BufferPtr[3]);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                    Qspi->SendBuffer.BufferPtr[3] << 8 | Qspi->SendBuffer.BufferPtr[4]);
                }
            }
            break;
        default:
            AL_LOG(AL_LOG_LEVEL_INFO, "DFS is set error in Qspi\r\n");
            break;
        }
    }

    Qspi->SendBuffer.HandledCnt = (Qspi->Configs.EnSpiCfg.AddrLength + Qspi->Configs.EnSpiCfg.InstLength) / 2;
    SendLevel = MIN(Qspi->Fifolen - AlQspi_ll_ReadTxFifoLevel(Qspi->BaseAddr), (Qspi->SendBuffer.RequestedCnt -
    Qspi->SendBuffer.HandledCnt) / (Qspi->BitsPerWord >> 3));

    while (SendLevel--) {
        if (Qspi->BitsPerWord == 8) {
            SendValue = *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        } else if (Qspi->BitsPerWord == 16) {
            SendValue = *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        } else {
            SendValue = *(AL_U32 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        }

        AlQspi_ll_DataTransmit(Qspi->BaseAddr, SendValue);
        Qspi->SendBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
    }

    SendLevel = MIN((AL_U32)(Qspi->Fifolen / 2), SendSize / (Qspi->BitsPerWord >> 3));
    AlQspi_ll_SetTxFifoThrLevel(Qspi->BaseAddr, SendLevel);

    AlQspi_ll_SetSlvSel(Qspi->BaseAddr, Qspi->Configs.SlvSelEnum);
    AlQspi_ll_EnableIntr(Qspi->BaseAddr, QSPI_TXEIM);

    return AL_OK;
}

/**
 * This function is to start qspi receive data
 * @param   Qspi is structure pointer to qspi device
 * @param   ReceiveBuf is send data buffer pointer
 * @param   ReceiveSize is receive data size
 * @return  Whether the qspi receive data successfully
 * @note    None
*/
AL_S32 AlQspi_Dev_RecvData(AL_QSPI_DevStruct *Qspi, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 Temp;

    if (Qspi == AL_NULL || ReceiveBuf == AL_NULL || ReceiveSize == 0 ) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_RXFIFO_NOTEMPTY== AlQspi_ll_IsRxFifoEmpty(Qspi->BaseAddr)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQspi_Dev_IsRxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQspi_Dev_SetRxBusy(Qspi);

    Qspi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Qspi->RecvBuffer.RequestedCnt = ReceiveSize;
    Qspi->RecvBuffer.HandledCnt   = 0;

    if (0 == (ReceiveSize % 4)) {
        Qspi->Configs.DataFrameSize = QSPI_DFS_32BITS;
        Qspi->BitsPerWord = 32;
    } else if (0 == (ReceiveSize % 2)) {
        Qspi->Configs.DataFrameSize = QSPI_DFS_16BITS;
        Qspi->BitsPerWord = 16;
    } else {
        Qspi->Configs.DataFrameSize = QSPI_DFS_8BITS;
        Qspi->BitsPerWord = 8;
    }

    AlQspi_ll_Disable(Qspi->BaseAddr);
    AlQspi_ll_SetAddrLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->BaseAddr, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->BaseAddr, Qspi->Configs.TransMode);
    Temp = ReceiveSize / (Qspi->BitsPerWord >> 3);
    AlQspi_ll_SetRecvNumOfDataFrames(Qspi->BaseAddr, Temp ? Temp - 1 : 0);
    AlQspi_ll_SetSlvSel(Qspi->BaseAddr, Qspi->Configs.SlvSelEnum);
    AlQspi_ll_SetDataFrameSize(Qspi->BaseAddr, Qspi->Configs.DataFrameSize);

    AlQspi_ll_Enable(Qspi->BaseAddr);
    Temp = MIN(ReceiveSize / (Qspi->BitsPerWord >> 3), Qspi->Fifolen / 2);
    AlQspi_ll_SetRxFifoThrLevel(Qspi->BaseAddr, Temp ? Temp - 1 : 0);
    //  AlQspi_ll_SetRxFifoThrLevel(Qspi->BaseAddr, 0);
    AlQspi_ll_EnableIntr(Qspi->BaseAddr, QSPI_RXFIM);

    return AL_OK;
}

/**
 * This function is to qspi tranfer in full-duplex mode
 * @param   Qspi is structure pointer to qspi device
 * @param   SendBuf is send data buffer pointer
 * @param   SendSize is send data size
 * @param   ReceiveBuf is send data buffer pointer
 * @param   ReceiveSize is receive data size
 * @return  Whether the qspi transfer data successfully
 * @note    None
*/
AL_S32 AlQspi_Dev_TranferData(AL_QSPI_DevStruct *Qspi, AL_U8 *SendBuf, AL_U32 SendSize, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 ReceiveLevel, SendLevel, SendValue = 0xffffffff, Temp, TempSendSize;

    if (Qspi == AL_NULL || SendBuf == AL_NULL || ReceiveBuf == AL_NULL || SendSize == 0 || ReceiveSize == 0) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (AlQspi_Dev_IsRxBusy(Qspi) ||AlQspi_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQspi_Dev_SetRxBusy(Qspi);
    AlQspi_Dev_SetTxBusy(Qspi);

    TempSendSize = SendSize - (Qspi->Configs.EnSpiCfg.AddrLength + Qspi->Configs.EnSpiCfg.InstLength) / 2;

    if(QSPI_ADDR_L0 != Qspi->Configs.EnSpiCfg.AddrLength) {
        if ((0 == (TempSendSize % 4)) && (0 != TempSendSize)) {
            Qspi->Configs.DataFrameSize = QSPI_DFS_32BITS;
            Qspi->BitsPerWord = 32;
        } else if ((0 == (TempSendSize % 2)) && (0 != TempSendSize)) {
            Qspi->Configs.DataFrameSize = QSPI_DFS_16BITS;
            Qspi->BitsPerWord = 16;
        } else {
            Qspi->Configs.DataFrameSize = QSPI_DFS_8BITS;
            Qspi->BitsPerWord = 8;
        }
    } else {
        Qspi->Configs.DataFrameSize = QSPI_DFS_8BITS;
        Qspi->BitsPerWord = 8;
    }

    Qspi->SendBuffer.BufferPtr    = SendBuf;
    Qspi->SendBuffer.RequestedCnt = SendSize;
    Qspi->SendBuffer.HandledCnt   = 0;
    Qspi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Qspi->RecvBuffer.RequestedCnt = ReceiveSize;
    Qspi->RecvBuffer.HandledCnt   = 0;

    AlQspi_ll_Disable(Qspi->BaseAddr);
    AlQspi_ll_SetAddrLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->BaseAddr, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->BaseAddr, Qspi->Configs.TransMode);
    Temp = ReceiveSize / (Qspi->BitsPerWord >> 3);
    AlQspi_ll_SetRecvNumOfDataFrames(Qspi->BaseAddr, Temp ? Temp - 1 : 0);
    AlQspi_ll_SetDataFrameSize(Qspi->BaseAddr, Qspi->Configs.DataFrameSize);
    SendLevel = MIN((AL_U32)(Qspi->Fifolen / 2), TempSendSize / (Qspi->BitsPerWord >> 3));
    if(SPI_STANDARD_FORMAT != Qspi->Configs.SpiFrameFormat) {
        SendLevel += 2;
    }
    AlQspi_ll_SetTxStartFifoLevel(Qspi->BaseAddr, SendLevel ? SendLevel - 1 : 0);

    AlQspi_ll_Enable(Qspi->BaseAddr);

    if(Qspi->Configs.SpiFrameFormat == SPI_STANDARD_FORMAT) {
        switch (Qspi->Configs.DataFrameSize)
        {
        case QSPI_DFS_32BITS:
            if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U32 *)(Qspi->SendBuffer.BufferPtr));
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "Qspi not support this mode combination\r\n");
            }
            break;
        case QSPI_DFS_16BITS:
            if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U16 *)(Qspi->SendBuffer.BufferPtr));
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + 2));
            } else {
                AL_LOG(AL_LOG_LEVEL_INFO, "Qspi not support this mode combination\r\n");
            }
            break;
        case QSPI_DFS_8BITS:
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr));
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 1));
            }
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L8) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 2));
            }
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L16) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 3));
            }
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L24) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 4));
            }
            break;
        default:
            AL_LOG(AL_LOG_LEVEL_INFO, "DFS is set error in Qspi\r\n");
            break;
        }
    } else {
        switch (Qspi->Configs.DataFrameSize)
        {
        case QSPI_DFS_32BITS:
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[0] << 24);
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 8 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                    Qspi->SendBuffer.BufferPtr[3] << 24);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] | Qspi->SendBuffer.BufferPtr[2] << 8 |
                    Qspi->SendBuffer.BufferPtr[3] << 16 | Qspi->SendBuffer.BufferPtr[4] << 24);
                }
            }
            break;
        case QSPI_DFS_16BITS:
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[0] << 8);
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] |
                    Qspi->SendBuffer.BufferPtr[3] << 8);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 24 |
                    Qspi->SendBuffer.BufferPtr[3] | Qspi->SendBuffer.BufferPtr[4] << 8);
                }
            }
            break;
        case QSPI_DFS_8BITS:
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[0]);
            if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
                if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 8 |
                    Qspi->SendBuffer.BufferPtr[3]);
                } else {
                    AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                    Qspi->SendBuffer.BufferPtr[3] << 8 | Qspi->SendBuffer.BufferPtr[4]);
                }
            }
            break;
        default:
            AL_LOG(AL_LOG_LEVEL_INFO, "DFS is set error in Qspi\r\n");
            break;
        }
    }

    Qspi->SendBuffer.HandledCnt = (Qspi->Configs.EnSpiCfg.AddrLength + Qspi->Configs.EnSpiCfg.InstLength) / 2;

    SendLevel = MIN3(Qspi->Fifolen - AlQspi_ll_ReadTxFifoLevel(Qspi->BaseAddr), (Qspi->SendBuffer.RequestedCnt -
    Qspi->SendBuffer.HandledCnt) / (Qspi->BitsPerWord >> 3) , Qspi->Fifolen -
    (Qspi->RecvBuffer.RequestedCnt - Qspi->SendBuffer.RequestedCnt)/ (Qspi->BitsPerWord >> 3));

    while (SendLevel--) {

        /* qspi send data can only be to adjust the byte order by software */
        if (Qspi->BitsPerWord == 8) {
            SendValue = *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        } else if (Qspi->BitsPerWord == 16) {
            SendValue = *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            // SendValue = QSPI_ENDIAN_SWAP16(SendValue);
        } else {
            SendValue = *(AL_U32 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            // SendValue = QSPI_ENDIAN_SWAP32(SendValue);
        }

        AlQspi_ll_DataTransmit(Qspi->BaseAddr, SendValue);
        Qspi->SendBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
    }

    SendLevel = MIN((AL_U32)(Qspi->Fifolen / 2), SendSize / (Qspi->BitsPerWord >> 3));
    Temp =  MIN(ReceiveSize / (Qspi->BitsPerWord >> 3), Qspi->Fifolen / 2);
    AlQspi_ll_SetRxFifoThrLevel(Qspi->BaseAddr, Temp ? Temp - 1 : 0);
    AlQspi_ll_SetTxFifoThrLevel(Qspi->BaseAddr, SendLevel);

    /* Set cs to start transfer */
    AlQspi_ll_SetSlvSel(Qspi->BaseAddr, Qspi->Configs.SlvSelEnum);
    AlQspi_ll_EnableIntr(Qspi->BaseAddr, QSPI_RXFIM | QSPI_TXEIM);

    return AL_OK;
}

/**
 * This function
 * @param   Qspi is structure pointer to qspi device
 * @return
 * @note    None
*/
AL_S32 AlQspi_Dev_DmaSendData(AL_QSPI_DevStruct *Qspi, AL_U32 SendSize)
{
    AL_U32 SendLevel, TempSendSize;

    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_TXFIFO_FULL == AlQspi_ll_IsTxFifoFull(Qspi->BaseAddr)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQspi_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    TempSendSize = SendSize - (Qspi->Configs.EnSpiCfg.AddrLength + Qspi->Configs.EnSpiCfg.InstLength) / 2;

    AlQspi_ll_Disable(Qspi->BaseAddr);
    AlQspi_ll_SetDataFrameSize(Qspi->BaseAddr, QSPI_DFS_8BITS);
    AlQspi_ll_SetAddrLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->BaseAddr, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->BaseAddr, Qspi->Configs.TransMode);
    SendLevel = MIN((AL_U32)(Qspi->Fifolen / 2), TempSendSize);
    if(SPI_STANDARD_FORMAT != Qspi->Configs.SpiFrameFormat) {
        SendLevel += 2;
    }
    AlQspi_ll_SetTxStartFifoLevel(Qspi->BaseAddr, SendLevel ? SendLevel - 1 : 0);
    // AlQspi_ll_SetTxStartFifoLevel(Qspi->BaseAddr, 0);
    AlQspi_ll_SetTxFifoThrLevel(Qspi->BaseAddr, 0);
    AlQspi_ll_TxDmaEnable(Qspi->BaseAddr);
    AlQspi_ll_SetDmaTransLevel(Qspi->BaseAddr, Qspi->Fifolen / 2 + 2);
    AlQspi_ll_Enable(Qspi->BaseAddr);

    if(Qspi->Configs.SpiFrameFormat == SPI_STANDARD_FORMAT) {
        AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr));
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 1));
        }
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L8) {
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 2));
        }
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L16) {
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 3));
        }
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L24) {
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 4));
        }
    } else {
        AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[0]);
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
            if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 8 |
                Qspi->SendBuffer.BufferPtr[3]);
            } else {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                Qspi->SendBuffer.BufferPtr[3] << 8 | Qspi->SendBuffer.BufferPtr[4]);
            }
        }
    }

    return AL_OK;
}

/**
 * This function
 * @param   Qspi is structure pointer to qspi device
 * @return
 * @note    None
*/
AL_S32 AlQspi_Dev_DmaRecvData(AL_QSPI_DevStruct *Qspi, AL_U16 RecvSize)
{
    AL_U32 TempRecvSize;
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_RXFIFO_NOTEMPTY== AlQspi_ll_IsRxFifoEmpty(Qspi->BaseAddr)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQspi_Dev_IsRxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQspi_ll_Disable(Qspi->BaseAddr);
    AlQspi_ll_SetDataFrameSize(Qspi->BaseAddr, QSPI_DFS_8BITS);
    AlQspi_ll_SetRxFifoThrLevel(Qspi->BaseAddr, 0);
    AlQspi_ll_RxDmaEnable(Qspi->BaseAddr);
    AlQspi_ll_SetDmaRecevLevel(Qspi->BaseAddr, 0);
    AlQspi_ll_SetAddrLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->BaseAddr, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetTransfMode(Qspi->BaseAddr, Qspi->Configs.TransMode);
    TempRecvSize = RecvSize / (Qspi->BitsPerWord >> 3);
    AlQspi_ll_SetRecvNumOfDataFrames(Qspi->BaseAddr, TempRecvSize ? TempRecvSize - 1 : 0);
    AlQspi_ll_Enable(Qspi->BaseAddr);

    return AL_OK;
}

/**
 * This function
 * @param   Qspi is structure pointer to qspi device
 * @return
 * @note    None
*/
AL_S32 AlQspi_Dev_DmaTranferData(AL_QSPI_DevStruct *Qspi, AL_U32 SendSize, AL_U16 RecvSize)
{
    AL_U32 SendLevel, TempSendSize;
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (AlQspi_Dev_IsRxBusy(Qspi) || AlQspi_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    TempSendSize = SendSize - (Qspi->Configs.EnSpiCfg.AddrLength + Qspi->Configs.EnSpiCfg.InstLength) / 2;

    AlQspi_ll_Disable(Qspi->BaseAddr);

    AlQspi_ll_SetRxFifoThrLevel(Qspi->BaseAddr, 0);
    AlQspi_ll_SetTxFifoThrLevel(Qspi->BaseAddr, 0);
    AlQspi_ll_TxDmaEnable(Qspi->BaseAddr);
    AlQspi_ll_RxDmaEnable(Qspi->BaseAddr);
    AlQspi_ll_SetDmaRecevLevel(Qspi->BaseAddr, 0);

    AlQspi_ll_SetAddrLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.AddrLength);
    AlQspi_ll_SetInstLength(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.InstLength);
    AlQspi_ll_SetTransType(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.TransType);
    AlQspi_ll_SetWaitCycles(Qspi->BaseAddr, Qspi->Configs.EnSpiCfg.WaitCycles);
    AlQspi_ll_SetQspiFrameFormat(Qspi->BaseAddr, Qspi->Configs.SpiFrameFormat);
    AlQspi_ll_SetDataFrameSize(Qspi->BaseAddr, QSPI_DFS_8BITS);
    AlQspi_ll_SetTransfMode(Qspi->BaseAddr, Qspi->Configs.TransMode);
    AlQspi_ll_SetRecvNumOfDataFrames(Qspi->BaseAddr, RecvSize ? RecvSize - 1 : 0);

    SendLevel = MIN((AL_U32)(Qspi->Fifolen / 2), TempSendSize);
    if(SPI_STANDARD_FORMAT != Qspi->Configs.SpiFrameFormat) {
        SendLevel += 2;
    }
    AlQspi_ll_SetDmaTransLevel(Qspi->BaseAddr, Qspi->Fifolen / 2 + 2);
    // AlQspi_ll_SetTxStartFifoLevel(Qspi->BaseAddr, 0);
    AlQspi_ll_SetTxStartFifoLevel(Qspi->BaseAddr, SendLevel ? SendLevel - 1 : 0);
    AlQspi_ll_Enable(Qspi->BaseAddr);

    if(Qspi->Configs.SpiFrameFormat == SPI_STANDARD_FORMAT) {
        AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr));
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 1));
        }
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L8) {
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 2));
        }
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L16) {
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 3));
        }
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L24) {
            AlQspi_ll_DataTransmit(Qspi->BaseAddr, *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + 4));
        }
    } else {
        AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[0]);
        if(Qspi->Configs.EnSpiCfg.AddrLength > QSPI_ADDR_L0) {
            if(Qspi->Configs.EnSpiCfg.AddrLength < QSPI_ADDR_L32) {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 16 | Qspi->SendBuffer.BufferPtr[2] << 8 |
                Qspi->SendBuffer.BufferPtr[3]);
            } else {
                AlQspi_ll_DataTransmit(Qspi->BaseAddr, Qspi->SendBuffer.BufferPtr[1] << 24 | Qspi->SendBuffer.BufferPtr[2] << 16 |
                Qspi->SendBuffer.BufferPtr[3] << 8 | Qspi->SendBuffer.BufferPtr[4]);
            }
        }
    }

    return AL_OK;
}

/**
 * This function
 * @param   Qspi is structure pointer to qspi device
 * @param   Cmd is io ctrl enum to set qspi status
 * @param   Data is AL_VOID pointer converted to AL_QSPI_SlvSelEnum pointer
 * @return  Whether the qspi is io ctrl successfully
 * @note    None
*/
AL_S32 AlQspi_Dev_IoCtl(AL_QSPI_DevStruct *Qspi, AL_Qspi_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    switch (Cmd)
    {
    case AL_QSPI_IOCTL_SET_SLAVE_SELECT: {
        AL_QSPI_SlvSelEnum SlvSel = *(AL_QSPI_SlvSelEnum *)Data;
        Qspi->Configs.SlvSelEnum = SlvSel;
        break;
    }

    case AL_QSPI_IOCTL_GET_SLAVE_SELECT: {
        AL_QSPI_SlvSelEnum *SlvSel = (AL_QSPI_SlvSelEnum *)Data;
        *SlvSel = Qspi->Configs.SlvSelEnum;
        break;
    }

    default:
        return AL_QSPI_ERR_NOT_SUPPORT;
        break;
    }

    return Ret;
}

/**
 * This function is register qspi intrrupt callback function
 * @param   Qspi is structure pointer to qspi device
 * @param   Callback is a function pointer to qspi event callback function
 * @param   CallbackRef is parameter of callback function
 * @return  Whether the qspi register intrrupt callback function successfully
 * @note    None
*/
AL_S32 AlQspi_Dev_RegisterIntrCallBack(AL_QSPI_DevStruct *Qspi, QSPI_EventCallBack Callback, AL_VOID *CallbackRef)
{
    if (Qspi == AL_NULL || Callback == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if (Qspi->EventCallBack != AL_NULL) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    Qspi->EventCallBack        = Callback;
    Qspi->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 * This function is unregister qspi intrrupt callback function
 * @param   Qspi is structure pointer to qspi device
 * @return  Whether the qspi unregister intrrupt callback function successfully
 * @note    None
*/
AL_S32 AlQspi_Dev_UnRegisterIntrCallBack(AL_QSPI_DevStruct *Qspi)
{
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    Qspi->EventCallBack = (QSPI_EventCallBack)AL_NULL;

    return AL_OK;
}

/**
 * This function is qspi receive intrrupt handler
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQspi_Dev_RecvDataHandler(AL_QSPI_DevStruct *Qspi)
{
    AL_U32 Length, RxFifoLevel, Status, Temp;

    Length = Qspi->RecvBuffer.RequestedCnt - Qspi->RecvBuffer.HandledCnt;

    if(Length) {
        RxFifoLevel = AlQspi_ll_ReadRxFifoLevel(Qspi->BaseAddr);
        if(!RxFifoLevel) {
            Status = AlQspi_ll_ReadRawIntrStatus(Qspi->BaseAddr);
            if (Status & RXOIS) {
                /* FIFO overflow on Rx */
                AL_LOG(AL_LOG_LEVEL_INFO, "Error FIFO overflow on Rx\r\n");
                Qspi->State |= AL_QSPI_STATE_ERROR;
            }
        } else if ((Qspi->Fifolen == RxFifoLevel) && (Qspi->Configs.TransMode == QSPI_EEPROM)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Error FIFO Full on Rx, cs high\r\n");
            Qspi->State |= AL_QSPI_STATE_ERROR;
        }

        RxFifoLevel = MIN(RxFifoLevel, Length / (Qspi->BitsPerWord >> 3));
        for (; RxFifoLevel; --RxFifoLevel) {
            if (Qspi->BitsPerWord == 8) {
                *(AL_U8 *)(Qspi->RecvBuffer.BufferPtr + Qspi->RecvBuffer.HandledCnt) = (AL_U8)AlQspi_ll_DataReceive(Qspi->BaseAddr);
            } else if (Qspi->BitsPerWord == 16) {
                *(AL_U16 *)(Qspi->RecvBuffer.BufferPtr + Qspi->RecvBuffer.HandledCnt) = (AL_U16)AlQspi_ll_DataReceive(Qspi->BaseAddr);
            } else if (Qspi->BitsPerWord == 32) {
                *(AL_U32 *)(Qspi->RecvBuffer.BufferPtr + Qspi->RecvBuffer.HandledCnt) = AlQspi_ll_DataReceive(Qspi->BaseAddr);
            }

            Qspi->RecvBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
            Length = Qspi->RecvBuffer.RequestedCnt - Qspi->RecvBuffer.HandledCnt;
        }
    }

    Temp =  MIN(Length / (Qspi->BitsPerWord >> 3), (AL_U32)(Qspi->Fifolen / 2));
    AlQspi_ll_SetRxFifoThrLevel(Qspi->BaseAddr, Temp ? Temp - 1 : 0);

    if (Qspi->RecvBuffer.HandledCnt == Qspi->RecvBuffer.RequestedCnt) {
        AlQspi_ll_MaskIntr(Qspi->BaseAddr, QSPI_RXFIM);

        if (Qspi->EventCallBack) {
            AL_QSPI_EventStruct QSPIEvent = {
                .Event        = AL_QSPI_RECEIVE_DONE,
                .EventData    = Qspi->SendBuffer.HandledCnt
            };
            AlQspi_Dev_ClrRxBusy(Qspi);
            Qspi->EventCallBack(QSPIEvent, Qspi->EventCallBackRef);
        }
    }
}

/**
 * This function is qspi send intrrupt handler
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQspi_Dev_SendDataHandler(AL_QSPI_DevStruct *Qspi)
{
    AL_U32 TxFifoLevel, Length, Room, SendValue = 0xffffffff;

    Length = Qspi->SendBuffer.RequestedCnt - Qspi->SendBuffer.HandledCnt;

    if (Length) {
        TxFifoLevel = AlQspi_ll_ReadTxFifoLevel(Qspi->BaseAddr);

        if (!TxFifoLevel) {
            /* return error fifo entries */
            AL_LOG(AL_LOG_LEVEL_INFO, "Error Tx fifo enteries, cs high\r\n");
            Qspi->State |= AL_QSPI_STATE_ERROR;
        }

        Room = MIN((Qspi->Fifolen - TxFifoLevel), Length / (Qspi->BitsPerWord >> 3));
        AlQspi_ll_SetTxStartFifoLevel(Qspi->BaseAddr, Room ? Room -1 : 0);
        for (; Room; --Room) {
            if (Qspi->BitsPerWord == 8) {
                SendValue = *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            } else if (Qspi->BitsPerWord == 16) {
                SendValue = *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
                // SendValue = QSPI_ENDIAN_SWAP16(SendValue);
            } else {
                SendValue = *(AL_U32 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
                // SendValue = QSPI_ENDIAN_SWAP32(SendValue);
            }

            AlQspi_ll_DataTransmit(Qspi->BaseAddr, SendValue);
            Qspi->SendBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
            Length = Qspi->SendBuffer.RequestedCnt - Qspi->SendBuffer.HandledCnt;
        }
    }

    if (Qspi->SendBuffer.HandledCnt == Qspi->SendBuffer.RequestedCnt) {
        AlQspi_ll_MaskIntr(Qspi->BaseAddr, QSPI_TXEIM);
        // AlQspi_ll_SetTxFifoThrLevel(Qspi->BaseAddr, 0);
        // AlQspi_ll_SetClockStretch(Qspi->BaseAddr, QSPI_DisableClockStretch);
        if (Qspi->EventCallBack) {
            AL_QSPI_EventStruct event = {
                .Event         = AL_QSPI_SEND_DONE,
                .EventData    = Qspi->SendBuffer.HandledCnt
            };
            AlQspi_Dev_ClrTxBusy(Qspi);
            Qspi->EventCallBack(event, Qspi->EventCallBackRef);
        }
    }
}

#define QSPI_IN_TX_FE_INTR(Status)  (Status & QSPI_TXEIS)
#define QSPI_IN_TX_FO_INTR(Status)  (Status & QSPI_TXOIS)
#define QSPI_IN_RX_FU_INTR(Status)  (Status & QSPI_RXUIS)
#define QSPI_IN_RX_FO_INTR(Status)  (Status & QSPI_RXOIS)
#define QSPI_IN_RX_FF_INTR(Status)  (Status & QSPI_RXFIS)

#define QSPI_IN_XIP_RX_FO_INTR(Status)  (Status & QSPI_XRXOIS)
#define QSPI_IN_TX_FU_INTR(Status)      (Status & QSPI_TXUIS)
#define QSPI_IN_AXI_ERR_INTR(Status)    (Status & QSPI_AXIES)
#define QSPI_IN_TFER_ERR_INTR(Status)   (Status & QSPI_SPITES)
#define QSPI_IN_DONE_INTR(Status)       (Status & QSPI_DONES)



#define QSPI_IN_STATUS_ERROR(Status)  0

/**
 * This function is qspi intrrupt handler function
 * @param   instance is AL_VOID pointer converted to AL_QSPI_DevStruct pointer
 * @return  AL_VOID
 * @note    None
*/
AL_VOID AlQspi_Dev_IntrHandler(AL_VOID *instance)
{
    AL_QSPI_DevStruct *Qspi = (AL_QSPI_DevStruct *) instance;
    AL_QSPI_IntrStatusEnum IntrStatus = AlQspi_ll_GetIntrStatus(Qspi->BaseAddr);
    // AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);

    if (QSPI_IN_RX_FF_INTR(IntrStatus)) {
        AlQspi_Dev_RecvDataHandler(Qspi);
    }

    if (QSPI_IN_TX_FE_INTR(IntrStatus)) {
        AlQspi_Dev_SendDataHandler(Qspi);
    }

    if (QSPI_IN_TX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi tx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "TXFLR:0x%x\r\n", AlQspi_ll_ReadTxFifoLevel(Qspi->BaseAddr));
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQspi_ll_ClearAllIntr(Qspi->BaseAddr);
    }

    if (QSPI_IN_RX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi rx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "RXFLR:0x%x\r\n", AlQspi_ll_ReadRxFifoLevel(Qspi->BaseAddr));
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQspi_ll_ClearAllIntr(Qspi->BaseAddr);
    }

    if (QSPI_IN_RX_FU_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi rx fifo underflow intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "RXFLR:0x%x\r\n", AlQspi_ll_ReadRxFifoLevel(Qspi->BaseAddr));
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQspi_ll_ClearAllIntr(Qspi->BaseAddr);
    }

    if (QSPI_IN_XIP_RX_FO_INTR(IntrStatus)){
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi XIP rx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "RXFLR:0x%x\r\n", AlQspi_ll_ReadRxFifoLevel(Qspi->BaseAddr));
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQspi_ll_ClearAllIntr(Qspi->BaseAddr);
    }
}


