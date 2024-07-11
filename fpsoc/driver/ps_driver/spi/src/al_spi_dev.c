/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


/***************************** Include Files *********************************/
#include "al_spi_dev.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
/* Default init config */
static AL_SPI_ConfigsStruct SpiDefInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE
};

extern AL_SPI_HwConfigStruct AlSpi_HwCfg[AL_SPI_NUM_INSTANCE];
/************************** Function Prototypes ******************************/

/********************************************************/
/**
 *
 * This function searches for the hardware configuration structure associated with the specified
 * SPI device ID. It iterates through the available configurations and returns a pointer to the
 * matching configuration structure.
 *
 * @param DeviceId The SPI device ID to look up.
 *
 * @return A pointer to the hardware configuration structure if found, or NULL if not found.
 *
 */
AL_SPI_HwConfigStruct *AlSpi_Dev_LookupConfig(AL_U32 DeviceId)
{
    AL_U32 Index;
    AL_SPI_HwConfigStruct *CfgPtr = AL_NULL;

    for (Index = 0; Index < AL_SPI_NUM_INSTANCE; Index++) {
        if (AlSpi_HwCfg[Index].DeviceId == DeviceId) {
            CfgPtr = &AlSpi_HwCfg[Index];
            break;
        }
    }

    return CfgPtr;
}

/**
 *
 * This function checks the state of the SPI device and determines if the transmit bus is busy.
 *
 * @param Spi Pointer to the SPI device structure.
 *
 * @return AL_TRUE if the transmit bus is busy, AL_FALSE otherwise.
 *
 */
AL_BOOL AlSpi_Dev_IsTxBusy(AL_SPI_DevStruct *Spi)
{
    return (AL_BOOL)(Spi->State & AL_SPI_STATE_TX_BUSY);
}

/**
 *
 * This function checks the state of the SPI device to determine if the receive bus is busy.
 *
 * @param Spi Pointer to the AL_SPI_DevStruct representing the SPI device.
 *
 * @return AL_TRUE if the RX bus is busy, AL_FALSE otherwise.
 *
 */
AL_BOOL AlSpi_Dev_IsRxBusy(AL_SPI_DevStruct *Spi)
{
    return (AL_BOOL)(Spi->State & AL_SPI_STATE_RX_BUSY);
}

/**
 *
 * This function sets the transmission bus as busy for the specified SPI device.
 *
 * @param Spi Pointer to the AL_SPI_DevStruct representing the SPI device.
 *
 * @return None.
 *
 */
static AL_VOID AlSpi_Dev_SetTxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State |= AL_SPI_STATE_TX_BUSY;
}

/**
 *
 * This function sets the receive busy state of the specified SPI device.
 * When the RX busy state is set, it indicates that the SPI device is currently
 * receiving data.
 *
 * @param Spi Pointer to the AL_SPI_DevStruct representing the SPI device.
 *
 * @return None.
 *
 */
static AL_VOID AlSpi_Dev_SetRxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State |= AL_SPI_STATE_RX_BUSY;
}

/**
 *
 * This function clears the transmit busy flag of the SPI device, indicating that the transmit
 * operation is no longer in progress.
 *
 * @param Spi Pointer to the AL_SPI_DevStruct representing the SPI device.
 *
 * @return None.
 *
 */
static AL_VOID AlSpi_Dev_ClrTxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State &= (~AL_SPI_STATE_TX_BUSY);
}

/**
 *
 * This function clears the receive busy flag of the SPI device, indicating that the receive
 * operation is no longer in progress.
 *
 * @param Spi Pointer to the AL_SPI_DevStruct representing the SPI device.
 *
 * @return None.
 *
 */
static AL_VOID AlSpi_Dev_ClrRxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State &= (~AL_SPI_STATE_RX_BUSY);
}

#ifdef SPI_DEBUG
/**
 *
 * This function prints the values of various registers of the SPI device.
 *
 * @param SpiBaseAddr The base address of the SPI device.
 *
 * @return None.
 *
 */
AL_VOID AlSpi_Dev_DumpReg(AL_REG SpiBaseAddr)
{
    AL_LOG(AL_LOG_LEVEL_DEBUG, "CTRLR0:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_CTRLR0_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "CTRLR1:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_CTRLR1_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SSIENR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_SSIENR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "MWCR:0x%x\r\n",    AL_REG32_READ(SpiBaseAddr + SPI_MWCR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SER:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_SER_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "BAUDR:0x%x\r\n",   AL_REG32_READ(SpiBaseAddr + SPI_BAUDR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXFTLR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_TXFTLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXFTLR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_RXFTLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXFLR:0x%x\r\n",   AL_REG32_READ(SpiBaseAddr + SPI_TXFLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXFLR:0x%x\r\n",   AL_REG32_READ(SpiBaseAddr + SPI_RXFLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SR:0x%x\r\n",      AL_REG32_READ(SpiBaseAddr + SPI_SR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "IMR:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_IMR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "ISR:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_ISR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RISR:0x%x\r\n",    AL_REG32_READ(SpiBaseAddr + SPI_RISR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXOICR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_TXOICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXOICR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_RXOICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXUICR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_RXUICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "MSTICR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_MSTICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "ICR:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_ICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMACR:0x%x\r\n",   AL_REG32_READ(SpiBaseAddr + SPI_DMACR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMATDLR:0x%x\r\n", AL_REG32_READ(SpiBaseAddr + SPI_DMATDLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMARDLR:0x%x\r\n", AL_REG32_READ(SpiBaseAddr + SPI_DMARDLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "IDR:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_IDR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SSI_VERSION_ID:0x%x\r\n", AL_REG32_READ(SpiBaseAddr + SPI_SSI_VERSION_ID_MST_OFFSET));
}
#endif

/**
 *
 * This function initializes the SPI device with the provided hardware configuration and initialization configuration.
 *
 * @param Spi Pointer to the SPI device structure.
 * @param HwConfig Pointer to the hardware configuration structure.
 * @param InitConfig Pointer to the initialization configuration structure.
 *
 * @return AL_S32 Returns AL_SPI_ERR_ILLEGAL_PARAM if Spi is NULL, otherwise returns AL_OK.
 *
 */
AL_S32 AlSpi_Dev_Init(AL_SPI_DevStruct *Spi, AL_SPI_HwConfigStruct *HwConfig, AL_SPI_ConfigsStruct *InitConfig)
{
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }
    AL_U16 ClkDiv;

    Spi->HwConfig = *HwConfig;

    Spi->Configs = (InitConfig == AL_NULL) ? SpiDefInitConfigs : (*InitConfig);

    Spi->HwConfig.BaseAddress = AlSpi_ll_SetSpiMasterSlave(Spi->HwConfig.BaseAddress, Spi->Configs.Mode);

    AlSpi_ll_Disable(Spi->HwConfig.BaseAddress);
    AlSpi_ll_SetProtFormat(Spi->HwConfig.BaseAddress, Spi->Configs.ProtFormat);
    AlSpi_ll_SetCpolAndCpha(Spi->HwConfig.BaseAddress, Spi->Configs.ClockEnum);

    ClkDiv = (Spi->HwConfig.InputClockHz) / (Spi->HwConfig.IoClockHz);
    AlSpi_ll_SetClkDiv(Spi->HwConfig.BaseAddress, ClkDiv);

    AlSpi_ll_SetSlvSelToggle(Spi->HwConfig.BaseAddress, Spi->Configs.SlvToggleEnum);
    AlSpi_ll_MaskIntr(Spi->HwConfig.BaseAddress, SPI_TXEIM | SPI_RXFIM);
    AlSpi_ll_Enable(Spi->HwConfig.BaseAddress);

    Spi->State |= AL_SPI_STATE_READY;

    return AL_OK;
}

/**
 *
 * This function sends data over SPI using the provided SPI device structure.
 *
 * @param Spi       Pointer to the SPI device structure.
 * @param SendBuf   Pointer to the buffer containing the data to be sent.
 * @param SendSize  Size of the data to be sent, in bytes.
 *
 * @return AL_S32   Returns AL_OK if the data was sent successfully. Returns an error code if an error occurred.
 *
 */
AL_S32 AlSpi_Dev_SendData(AL_SPI_DevStruct *Spi, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U32 SendLevel, SendValue = 0xFFFFFFFF;

    if (Spi == AL_NULL || SendSize == 0) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (SPI_SR_TXFIFO_FULL == AlSpi_ll_IsTxFifoFull(Spi->HwConfig.BaseAddress)) {
        return AL_SPI_ERR_NOT_SUPPORT;
    }

    if (AlSpi_Dev_IsTxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_Dev_SetTxBusy(Spi);

    Spi->SendBuffer.BufferPtr    = SendBuf;
    Spi->SendBuffer.HandledCnt   = 0;
    Spi->SendBuffer.RequestedCnt = SendSize;

    /* Different fifo widths are set depending on the amount of data */
    if (0 == (SendSize % 4)) {
        Spi->Configs.Trans.DataFrameSize = SPI_FRAME_32BITS;
        Spi->BitsPerWord = 32;
    } else if (0 == (SendSize % 2)) {
        Spi->Configs.Trans.DataFrameSize = SPI_FRAME_16BITS;
        Spi->BitsPerWord = 16;
    } else {
        Spi->Configs.Trans.DataFrameSize = SPI_FRAME_8BITS;
        Spi->BitsPerWord = 8;
    }

    AlSpi_ll_Disable(Spi->HwConfig.BaseAddress);
    AlSpi_ll_SetTransfMode(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.TransMode);
    AlSpi_ll_SetDataFrameSize(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.DataFrameSize);

    AlSpi_ll_Enable(Spi->HwConfig.BaseAddress);

    SendLevel = MIN((AL_U32)(Spi->HwConfig.FifoLen - AlSpi_ll_ReadTxFifoLevel(Spi->HwConfig.BaseAddress)), Spi->SendBuffer.RequestedCnt / (Spi->BitsPerWord >> 3));

    while (SendLevel--) {
        if (Spi->BitsPerWord == 8) {
            SendValue = *(AL_U8 *)(Spi->SendBuffer.BufferPtr + Spi->SendBuffer.HandledCnt);
        } else if (Spi->BitsPerWord == 16) {
            SendValue = *(AL_U16 *)(Spi->SendBuffer.BufferPtr + Spi->SendBuffer.HandledCnt);
            SendValue = SPI_ENDIAN_SWAP16(SendValue);
        } else {
            SendValue = *(AL_U32 *)(Spi->SendBuffer.BufferPtr + Spi->SendBuffer.HandledCnt);
            SendValue = SPI_ENDIAN_SWAP32(SendValue);
        }

        AlSpi_ll_DataTransmit(Spi->HwConfig.BaseAddress, SendValue);
        Spi->SendBuffer.HandledCnt += Spi->BitsPerWord >> 3;
    }

    SendLevel = MIN((AL_U32)(Spi->HwConfig.FifoLen / 2), SendSize / (Spi->BitsPerWord >> 3));
    AlSpi_ll_SetTxFifoThrLevel(Spi->HwConfig.BaseAddress, SendLevel);

    AlSpi_ll_SetSlvSel(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.SlvSelEnum);
    AlSpi_ll_EnableIntr(Spi->HwConfig.BaseAddress, SPI_TXEIM);

    return AL_OK;
}

/**
 *
 * This function receives data over SPI using the provided SPI device structure.
 *
 * @param Spi       Pointer to the SPI device structure.
 * @param ReceiveBuf   Pointer to the buffer containing the data to be received.
 * @param ReceiveSize  Size of the data to be received, in bytes.
 *
 * @return AL_S32   Returns AL_OK if the data was received successfully. Returns an error code if an error occurred.
 *
 */
AL_S32 AlSpi_Dev_RecvData(AL_SPI_DevStruct *Spi, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 Temp;

    if (Spi == AL_NULL || ReceiveSize == 0 ) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (SPI_SR_RXFIFO_NOTEMPTY== AlSpi_ll_IsRxFifoEmpty(Spi->HwConfig.BaseAddress)) {
        return AL_SPI_ERR_NOT_SUPPORT;
    }

    if (AlSpi_Dev_IsRxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_Dev_SetRxBusy(Spi);

    Spi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Spi->RecvBuffer.RequestedCnt = ReceiveSize;
    Spi->RecvBuffer.HandledCnt   = 0;
    Spi->Configs.Trans.DataFrameSize = SPI_FRAME_8BITS;
    Spi->BitsPerWord = 8;

    AlSpi_ll_Disable(Spi->HwConfig.BaseAddress);
    AlSpi_ll_SetTransfMode(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.TransMode);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->HwConfig.BaseAddress, ReceiveSize ? ReceiveSize - 1 : 0);
    AlSpi_ll_SetSlvSel(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.SlvSelEnum);
    AlSpi_ll_SetDataFrameSize(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.DataFrameSize);

    AlSpi_ll_Enable(Spi->HwConfig.BaseAddress);
    Temp = MIN(ReceiveSize, (AL_U16)(Spi->HwConfig.FifoLen / 2));
    AlSpi_ll_SetRxFifoThrLevel(Spi->HwConfig.BaseAddress, Temp ? Temp - 1 : 0);
    //  AlSpi_ll_SetRxFifoThrLevel(Spi->HwConfig.BaseAddress, 0);
    AlSpi_ll_EnableIntr(Spi->HwConfig.BaseAddress, SPI_RXFIM);

    return AL_OK;
}

/**
 *
 * This function transfers data over SPI using the provided SPI device structure.
 *
 * @param Spi Pointer to the SPI device structure.
 * @param SendBuf Pointer to the buffer containing the data to be sent.
 * @param SendSize Size of the data to be sent, in bytes.
 * @param ReceiveBuf Pointer to the buffer to store the received data.
 * @param ReceiveSize Size of the buffer to store the received data, in bytes.
 *
 * @return Returns AL_OK if the transfer is successful. Returns an error code if there is an error.
 *         Possible error codes are:
 *         - AL_SPI_ERR_ILLEGAL_PARAM: Invalid parameters passed to the function.
 *         - AL_SPI_ERR_NOT_READY: SPI device is not ready for transfer.
 *         - AL_SPI_ERR_BUSY: SPI device is busy with another transfer.
 *
 */
AL_S32 AlSpi_Dev_TranferData(AL_SPI_DevStruct *Spi, AL_U8 *SendBuf, AL_U32 SendSize, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 SendLevel, SendValue = 0xffffffff, Temp;

    if (Spi == AL_NULL || SendSize == 0 || ReceiveSize == 0) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (AlSpi_Dev_IsRxBusy(Spi) || AlSpi_Dev_IsTxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_Dev_SetRxBusy(Spi);
    AlSpi_Dev_SetTxBusy(Spi);

    Spi->SendBuffer.BufferPtr    = SendBuf;
    Spi->SendBuffer.RequestedCnt = SendSize;
    Spi->SendBuffer.HandledCnt   = 0;
    Spi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Spi->RecvBuffer.RequestedCnt = ReceiveSize;
    Spi->RecvBuffer.HandledCnt   = 0;

    if ((0 == (SendSize % 4)) && (0 == (ReceiveSize % 4))) {
        Spi->Configs.Trans.DataFrameSize = SPI_FRAME_32BITS;
        Spi->BitsPerWord = 32;
    } else if ((0 == (SendSize % 2)) && (0 == (ReceiveSize % 2))) {
        Spi->Configs.Trans.DataFrameSize = SPI_FRAME_16BITS;
        Spi->BitsPerWord = 16;
    } else {
        Spi->Configs.Trans.DataFrameSize = SPI_FRAME_8BITS;
        Spi->BitsPerWord = 8;
    }

    AlSpi_ll_Disable(Spi->HwConfig.BaseAddress);
    AlSpi_ll_SetTransfMode(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.TransMode);
    Temp = ReceiveSize / (Spi->BitsPerWord >> 3);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->HwConfig.BaseAddress, Temp ? Temp - 1 : 0);
    AlSpi_ll_SetDataFrameSize(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.DataFrameSize);

    /* If the fifo width is set to more than 8, the received data will have byte order problems,
     * you can enable byte order reversal */
    if (Spi->BitsPerWord != 8) {
        AlSpi_ll_EnableEndianConver(Spi->HwConfig.BaseAddress);
    } else {
        AlSpi_ll_DisableEndianConver(Spi->HwConfig.BaseAddress);
    }

    AlSpi_ll_Enable(Spi->HwConfig.BaseAddress);


    SendLevel = MIN3(Spi->HwConfig.FifoLen - AlSpi_ll_ReadTxFifoLevel(Spi->HwConfig.BaseAddress), Spi->SendBuffer.RequestedCnt / (Spi->BitsPerWord >> 3),
                     Spi->HwConfig.FifoLen - (Spi->RecvBuffer.RequestedCnt - Spi->SendBuffer.RequestedCnt)/ (Spi->BitsPerWord >> 3));

    while (SendLevel--) {
        /* spi send data can only be to adjust the byte order by software */
        if (Spi->BitsPerWord == 8) {
            SendValue = *(AL_U8 *)(Spi->SendBuffer.BufferPtr + Spi->SendBuffer.HandledCnt);
        } else if (Spi->BitsPerWord == 16) {
            SendValue = *(AL_U16 *)(Spi->SendBuffer.BufferPtr + Spi->SendBuffer.HandledCnt);
            SendValue = SPI_ENDIAN_SWAP16(SendValue);
        } else {
            SendValue = *(AL_U32 *)(Spi->SendBuffer.BufferPtr + Spi->SendBuffer.HandledCnt);
            SendValue = SPI_ENDIAN_SWAP32(SendValue);
        }

        AlSpi_ll_DataTransmit(Spi->HwConfig.BaseAddress, SendValue);
        Spi->SendBuffer.HandledCnt += Spi->BitsPerWord >> 3;
    }

    SendLevel = MIN((AL_U32)(Spi->HwConfig.FifoLen / 2), SendSize / (Spi->BitsPerWord >> 3));
    Temp =  MIN((AL_U32)(ReceiveSize / (Spi->BitsPerWord >> 3)), (AL_U32)(Spi->HwConfig.FifoLen / 2));
    AlSpi_ll_SetRxFifoThrLevel(Spi->HwConfig.BaseAddress, Temp ? Temp - 1 : 0);
    AlSpi_ll_SetTxFifoThrLevel(Spi->HwConfig.BaseAddress, SendLevel);

    /* Set cs to start transfer */
    AlSpi_ll_SetSlvSel(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.SlvSelEnum);
    AlSpi_ll_EnableIntr(Spi->HwConfig.BaseAddress, SPI_RXFIM | SPI_TXEIM);

    return AL_OK;
}

/**
 *
 * This function sends data using DMA in the specified SPI device.
 *
 * @param Spi Pointer to the SPI device structure.

 * @return Returns AL_SPI_ERR_ILLEGAL_PARAM if Spi is NULL, AL_SPI_ERR_NOT_READY if the SPI device is not ready,
 *         AL_SPI_ERR_NOT_SUPPORT if the transmit FIFO is full, AL_SPI_ERR_BUSY if the transmit is already in progress,
 *         or AL_OK if the data is sent successfully.
 *
 */
AL_S32 AlSpi_Dev_DmaSendData(AL_SPI_DevStruct *Spi)
{
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (SPI_SR_TXFIFO_FULL == AlSpi_ll_IsTxFifoFull(Spi->HwConfig.BaseAddress)) {
        return AL_SPI_ERR_NOT_SUPPORT;
    }

    if (AlSpi_Dev_IsTxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_ll_Disable(Spi->HwConfig.BaseAddress);
    AlSpi_ll_SetTransfMode(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.TransMode);
    AlSpi_ll_SetDataFrameSize(Spi->HwConfig.BaseAddress, SPI_FRAME_8BITS);
    AlSpi_ll_SetTxFifoThrLevel(Spi->HwConfig.BaseAddress, Spi->HwConfig.FifoLen / 2);
    AlSpi_ll_TxDmaEnable(Spi->HwConfig.BaseAddress);
    AlSpi_ll_SetDmaTransLevel(Spi->HwConfig.BaseAddress, Spi->HwConfig.FifoLen / 2);
    AlSpi_ll_Enable(Spi->HwConfig.BaseAddress);

    AlSpi_ll_SetSlvSel(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.SlvSelEnum);

    return AL_OK;
}

/**
 *
 * This function receives data using DMA in the specified SPI device.
 *
 * @param Spi Pointer to the SPI device structure.
 * @param RecvSize Number of data frames to receive.

 * @return Returns AL_OK on success, or an error code on failure.
 *         - AL_SPI_ERR_ILLEGAL_PARAM if Spi is NULL.
 *         - AL_SPI_ERR_NOT_READY if the SPI device is not ready.
 *         - AL_SPI_ERR_NOT_SUPPORT if the receive FIFO is empty.
 *         - AL_SPI_ERR_BUSY if the receive operation is already in progress.
 *
 */
AL_S32 AlSpi_Dev_DmaRecvData(AL_SPI_DevStruct *Spi, AL_U16 RecvSize)
{
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (SPI_SR_RXFIFO_NOTEMPTY== AlSpi_ll_IsRxFifoEmpty(Spi->HwConfig.BaseAddress)) {
        return AL_SPI_ERR_NOT_SUPPORT;
    }

    if (AlSpi_Dev_IsRxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_ll_Disable(Spi->HwConfig.BaseAddress);
    AlSpi_ll_SetTransfMode(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.TransMode);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->HwConfig.BaseAddress, RecvSize - 1);

    AlSpi_ll_SetDataFrameSize(Spi->HwConfig.BaseAddress, SPI_FRAME_8BITS);
    AlSpi_ll_SetRxFifoThrLevel(Spi->HwConfig.BaseAddress, Spi->HwConfig.FifoLen / 2);

    AlSpi_ll_RxDmaEnable(Spi->HwConfig.BaseAddress);
    AlSpi_ll_SetDmaRecevLevel(Spi->HwConfig.BaseAddress, 0);
    AlSpi_ll_Enable(Spi->HwConfig.BaseAddress);

    AlSpi_ll_SetSlvSel(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.SlvSelEnum);

    return AL_OK;
}

/**
 *
 * This function performs a DMA transfer of data over SPI. It configures the SPI controller
 * with the specified transfer mode, receive size, and data frame size. It also enables the
 * transmit and receive DMA channels, sets the DMA transfer levels, and enables the SPI controller.
 * Finally, it sets the chip select (CS) signal to start the transfer.
 *
 * @param Spi Pointer to the SPI device structure.
 * @param RecvSize The size of the data to be received in number of frames.

 * @return Returns AL_OK if the transfer was successful. Returns an error code if an error occurred.
 *         Possible error codes are: AL_SPI_ERR_ILLEGAL_PARAM, AL_SPI_ERR_NOT_READY, AL_SPI_ERR_BUSY.

 */
AL_S32 AlSpi_Dev_DmaTranferData(AL_SPI_DevStruct *Spi, AL_U16 RecvSize)
{
    // Check for NULL pointer
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    // Check if SPI is ready
    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    // Check if SPI is busy
    if (AlSpi_Dev_IsRxBusy(Spi) || AlSpi_Dev_IsTxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    // Disable SPI controller
    AlSpi_ll_Disable(Spi->HwConfig.BaseAddress);

    // Set transfer mode
    AlSpi_ll_SetTransfMode(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.TransMode);

    // Set receive size
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->HwConfig.BaseAddress, RecvSize - 1);

    // Set data frame size
    AlSpi_ll_SetDataFrameSize(Spi->HwConfig.BaseAddress, SPI_FRAME_8BITS);

    // Set receive FIFO threshold level
    AlSpi_ll_SetRxFifoThrLevel(Spi->HwConfig.BaseAddress, Spi->HwConfig.FifoLen / 2);

    // Set transmit FIFO threshold level
    AlSpi_ll_SetTxFifoThrLevel(Spi->HwConfig.BaseAddress, Spi->HwConfig.FifoLen / 2);

    // Enable transmit DMA channel
    AlSpi_ll_TxDmaEnable(Spi->HwConfig.BaseAddress);

    // Enable receive DMA channel
    AlSpi_ll_RxDmaEnable(Spi->HwConfig.BaseAddress);

    // Set DMA receive level
    AlSpi_ll_SetDmaRecevLevel(Spi->HwConfig.BaseAddress, 0);

    // Set DMA transmit level
    AlSpi_ll_SetDmaTransLevel(Spi->HwConfig.BaseAddress, Spi->HwConfig.FifoLen / 2);

    // Enable SPI controller
    AlSpi_ll_Enable(Spi->HwConfig.BaseAddress);

    // Set chip select (CS) signal to start transfer
    AlSpi_ll_SetSlvSel(Spi->HwConfig.BaseAddress, Spi->Configs.Trans.SlvSelEnum);

    return AL_OK;
}

/**
 * This function performs various control operations on the SPI device.
 *
 * @param Spi Pointer to the SPI device structure.
 * @param Cmd The control command to be executed.
 * @param Data Pointer to the data associated with the control command.
 *
 * @return AL_S32 Returns AL_OK if the operation is successful, otherwise returns an error code.
 *
 */
AL_S32 AlSpi_Dev_IoCtl(AL_SPI_DevStruct *Spi, AL_Spi_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    switch (Cmd) {
    case AL_SPI_IOCTL_SET_SLAVE_SELECT: {
        AL_SPI_SlvSelEnum SlvSel = *(AL_SPI_SlvSelEnum *)Data;
        Spi->Configs.Trans.SlvSelEnum = SlvSel;
        break;
    }

    case AL_SPI_IOCTL_GET_SLAVE_SELECT: {
        AL_SPI_SlvSelEnum *SlvSel = (AL_SPI_SlvSelEnum *)Data;
        *SlvSel = Spi->Configs.Trans.SlvSelEnum;
        break;
    }

    case AL_SPI_IOCTL_SET_MODE: {
        AL_SPI_Mode *Mode = (AL_SPI_Mode *)Data;
        Spi->Configs.Mode = *Mode;
        Spi->HwConfig.BaseAddress = AlSpi_ll_SetSpiMasterSlave(Spi->HwConfig.BaseAddress, Spi->Configs.Mode);
    }

    case AL_SPI_IOCTL_GET_MODE: {
        AL_SPI_Mode *Mode = (AL_SPI_Mode *)Data;
        *Mode = Spi->Configs.Mode;
    }

    case AL_SPI_IOCTL_SET_PROT_FORMAT: {
        AL_SPI_ProtFormat *ProtFormat = (AL_SPI_ProtFormat *)Data;
        Spi->Configs.ProtFormat = *ProtFormat;
        AlSpi_ll_SetProtFormat(Spi->HwConfig.BaseAddress, Spi->Configs.ProtFormat);
    }

    case AL_SPI_IOCTL_GET_PROT_FORMAT: {
        AL_SPI_ProtFormat *ProtFormat = (AL_SPI_ProtFormat *)Data;
        *ProtFormat = Spi->Configs.ProtFormat;
    }

    case AL_SPI_IOCTL_SET_CPOLANDCPHA: {
        AL_SPI_ClockEnum *ClockEnum = (AL_SPI_ClockEnum *)Data;
        Spi->Configs.ClockEnum = *ClockEnum;
        AlSpi_ll_SetCpolAndCpha(Spi->HwConfig.BaseAddress, Spi->Configs.ClockEnum);
    }

    case AL_SPI_IOCTL_GET_CPOLANDCPHA: {
        AL_SPI_ClockEnum *ClockEnum = (AL_SPI_ClockEnum *)Data;
        *ClockEnum = Spi->Configs.ClockEnum;
    }

    case AL_SPI_IOCTL_SET_CLOCK_DIV: {
        AL_U16 *ClockDiv = (AL_U16 *)Data;
        AlSpi_ll_SetClkDiv(Spi->HwConfig.BaseAddress, *ClockDiv);
    }

    case AL_SPI_IOCTL_GET_CLOCK_DIV: {
        AL_U16 *ClockDiv = (AL_U16 *)Data;
        *ClockDiv = AlSpi_ll_GetClkDiv(Spi->HwConfig.BaseAddress);
    }

    case AL_SPI_IOCTL_SET_SLV_TOGGLE: {
        AL_SPI_SlvSelToggleEnum *SlvToggle = (AL_SPI_SlvSelToggleEnum *)Data;
        Spi->Configs.SlvToggleEnum = *SlvToggle;
        AlSpi_ll_SetSlvSelToggle(Spi->HwConfig.BaseAddress, Spi->Configs.SlvToggleEnum);
    }

    case AL_SPI_IOCTL_GET_SLV_TOGGLE: {
        AL_SPI_SlvSelToggleEnum *SlvToggle = (AL_SPI_SlvSelToggleEnum *)Data;
        *SlvToggle = Spi->Configs.SlvToggleEnum;
    }
    default:
        return AL_SPI_ERR_NOT_SUPPORT;
        break;
    }

    return Ret;
}

/**
 *
 * This function registers an interrupt callback function and a reference pointer
 * to be used when the interrupt event occurs for the SPI device.
 *
 * @param Spi           Pointer to the AL_SPI_DevStruct representing the SPI device.
 * @param Callback      Pointer to the interrupt callback function.
 * @param CallbackRef   Pointer to the reference data to be passed to the callback function.
 *
 * @return AL_S32       Returns AL_OK if the callback is successfully registered,
 *                      AL_SPI_ERR_ILLEGAL_PARAM if Spi or Callback is NULL,
 *                      AL_SPI_ERR_NOT_SUPPORT if an interrupt callback is already registered.
 *
 */
AL_S32 AlSpi_Dev_RegisterIntrCallBack(AL_SPI_DevStruct *Spi, SPI_EventCallBack Callback, AL_VOID *CallbackRef)
{
    if (Spi == AL_NULL || Callback == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if (Spi->EventCallBack != AL_NULL) {
        return AL_SPI_ERR_NOT_SUPPORT;
    }

    Spi->EventCallBack        = Callback;
    Spi->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

/**
 *
 * This function unregisters the interrupt callback function for the specified SPI device.
 *
 * @param Spi Pointer to the AL_SPI_DevStruct representing the SPI device.

 * @return AL_S32 Returns AL_OK if the operation is successful, otherwise returns AL_SPI_ERR_ILLEGAL_PARAM.

 */
AL_S32 AlSpi_Dev_UnRegisterIntrCallBack(AL_SPI_DevStruct *Spi)
{
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    Spi->EventCallBack = (SPI_EventCallBack)AL_NULL;

    return AL_OK;
}

/**
 *
 * This function is responsible for handling the reception of data in the SPI device.
 * It checks the length of the data to be received and performs the necessary operations
 * based on the received data. If there is an error, it sets the appropriate state and
 * triggers the event callback. Once the reception is complete, it masks the interrupt
 * and triggers the receive done event callback.
 *
 * @param Spi Pointer to the AL_SPI_DevStruct representing the SPI device.
 *
 * @return None.

 */
static AL_VOID AlSpi_Dev_RecvDataHandler(AL_SPI_DevStruct *Spi)
{
    AL_U32 Length, RxFifoLevel, Status, Temp;

    Length = Spi->RecvBuffer.RequestedCnt - Spi->RecvBuffer.HandledCnt;

    if (Length) {
        RxFifoLevel = AlSpi_ll_ReadRxFifoLevel(Spi->HwConfig.BaseAddress);
        if (!RxFifoLevel) {
            Status = AlSpi_ll_ReadRawIntrStatus(Spi->HwConfig.BaseAddress);
            if (Status & RXOIS) {
                /* FIFO overflow on Rx */
                AL_LOG(AL_LOG_LEVEL_INFO, "Error FIFO overflow on Rx\r\n");
                Spi->State |= AL_SPI_STATE_ERROR;
                AL_SPI_EventStruct SpiEvent = {
                    .Events       = AL_SPI_RX_FIFO_FULL,
                    .EventData    = Spi->SendBuffer.HandledCnt
                };
                (*Spi->EventCallBack)(SpiEvent, Spi->EventCallBackRef);
            }
        } else if ((Spi->HwConfig.FifoLen == RxFifoLevel) && (Spi->Configs.Trans.TransMode == SPI_EEPROM)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Error FIFO Full on Rx, cs high\r\n");
            Spi->State |= AL_SPI_STATE_ERROR;
            AL_SPI_EventStruct SpiEvent = {
                .Events       = AL_SPI_RX_FIFO_FULL,
                .EventData    = Spi->SendBuffer.HandledCnt
            };
            (*Spi->EventCallBack)(SpiEvent, Spi->EventCallBackRef);
        }

        RxFifoLevel = MIN(RxFifoLevel, Length / (Spi->BitsPerWord >> 3));
        for (; RxFifoLevel; --RxFifoLevel) {
            if (Spi->BitsPerWord == 8) {
                *(AL_U8 *)(Spi->RecvBuffer.BufferPtr + Spi->RecvBuffer.HandledCnt) = (AL_U8)AlSpi_ll_DataReceive(Spi->HwConfig.BaseAddress);
            } else if (Spi->BitsPerWord == 16) {
                *(AL_U16 *)(Spi->RecvBuffer.BufferPtr + Spi->RecvBuffer.HandledCnt) = (AL_U16)AlSpi_ll_DataReceive(Spi->HwConfig.BaseAddress);
            } else if (Spi->BitsPerWord == 32) {
                *(AL_U32 *)(Spi->RecvBuffer.BufferPtr + Spi->RecvBuffer.HandledCnt) = AlSpi_ll_DataReceive(Spi->HwConfig.BaseAddress);
            }
            Spi->RecvBuffer.HandledCnt += Spi->BitsPerWord >> 3;
            Length = Spi->RecvBuffer.RequestedCnt - Spi->RecvBuffer.HandledCnt;
        }
    }

    Temp =  MIN(Length / (Spi->BitsPerWord >> 3), (AL_U32)(Spi->HwConfig.FifoLen / 2));
    AlSpi_ll_SetRxFifoThrLevel(Spi->HwConfig.BaseAddress, Temp ? Temp - 1 : 0);

    if (Spi->RecvBuffer.HandledCnt == Spi->RecvBuffer.RequestedCnt) {
        AlSpi_ll_MaskIntr(Spi->HwConfig.BaseAddress, SPI_RXFIM);

        if (Spi->EventCallBack) {
            AL_SPI_EventStruct SpiEvent = {
                .Events       = AL_SPI_RECEIVE_DONE,
                .EventData    = Spi->SendBuffer.HandledCnt
            };
            AlSpi_Dev_ClrRxBusy(Spi);
            (*Spi->EventCallBack)(SpiEvent, Spi->EventCallBackRef);
        }
    }
}

/**
 *
 * This function is responsible for sending data from the send buffer of the AL_SPI_DevStruct.
 * It checks the length of the send buffer and the transmit FIFO level to determine how much data can be sent.
 * It then sends the data using the appropriate bit width and updates the send buffer's handled count.
 * If all data has been sent, it masks the transmit interrupt and calls the event callback function.
 *
 * @param Spi Pointer to the AL_SPI_DevStruct.
 *
 */
static AL_VOID AlSpi_Dev_SendDataHandler(AL_SPI_DevStruct *Spi)
{
    AL_U32 TxFifoLevel, Length, Room, SendValue = 0xffffffff;
    Length = Spi->SendBuffer.RequestedCnt - Spi->SendBuffer.HandledCnt;

    if (Length) {
        TxFifoLevel = AlSpi_ll_ReadTxFifoLevel(Spi->HwConfig.BaseAddress);

        if (!TxFifoLevel) {
            /* return error fifo entries */
            AL_LOG(AL_LOG_LEVEL_INFO, "Error Tx fifo enteries, cs high\r\n");
            Spi->State |= AL_SPI_STATE_ERROR;
            AL_SPI_EventStruct SpiEvent = {
                .Events       = AL_SPI_TX_FIFO_EMPTY,
                .EventData    = Spi->SendBuffer.HandledCnt
            };
            (*Spi->EventCallBack)(SpiEvent, Spi->EventCallBackRef);
        }

        Room = MIN((AL_U32)(Spi->HwConfig.FifoLen - TxFifoLevel), Length / (Spi->BitsPerWord >> 3));
        for (; Room; --Room) {
            if (Spi->BitsPerWord == 8) {
                SendValue = *(AL_U8 *)(Spi->SendBuffer.BufferPtr + Spi->SendBuffer.HandledCnt);
            } else if (Spi->BitsPerWord == 16) {
                SendValue = *(AL_U16 *)(Spi->SendBuffer.BufferPtr + Spi->SendBuffer.HandledCnt);
                SendValue = SPI_ENDIAN_SWAP16(SendValue);
            } else {
                SendValue = *(AL_U32 *)(Spi->SendBuffer.BufferPtr + Spi->SendBuffer.HandledCnt);
                SendValue = SPI_ENDIAN_SWAP32(SendValue);
            }

            AlSpi_ll_DataTransmit(Spi->HwConfig.BaseAddress, SendValue);
            Spi->SendBuffer.HandledCnt += Spi->BitsPerWord >> 3;
            Length = Spi->SendBuffer.RequestedCnt - Spi->SendBuffer.HandledCnt;
        }
    }

    if (Spi->SendBuffer.HandledCnt == Spi->SendBuffer.RequestedCnt) {
        AlSpi_ll_MaskIntr(Spi->HwConfig.BaseAddress, SPI_TXEIM);

        if (Spi->EventCallBack) {
            AL_SPI_EventStruct event = {
                .Events       = AL_SPI_SEND_DONE,
                .EventData    = Spi->SendBuffer.HandledCnt
            };
            AlSpi_Dev_ClrTxBusy(Spi);
            (*Spi->EventCallBack)(event, Spi->EventCallBackRef);
        }
    }
}

#define SPI_IN_TX_FE_INTR(Status)  (Status & TXEIS)
#define SPI_IN_TX_FO_INTR(Status)  (Status & TXOIS)
#define SPI_IN_RX_FU_INTR(Status)  (Status & RXUIS)
#define SPI_IN_RX_FO_INTR(Status)  (Status & RXOIS)
#define SPI_IN_RX_FF_INTR(Status)  (Status & RXFIS)

#define SPI_IN_STATUS_ERROR(Status)  0

/**
 *
 * This function is responsible for handling events that occur on the SPI device.
 * It checks if a callback function is registered and if so, it creates a SPI event
 * structure and calls the callback function with the event and a reference to the SPI device.
 *
 * @param Spi The SPI device structure.
 * @param EventId The ID of the event that occurred.
 *
 * @return None.
 *
 */
static AL_VOID AlSpi_Dev_EventHandler(AL_SPI_DevStruct *Spi, AL_SPI_EventIdEnum EventId)
{
    if (Spi->EventCallBack) {
        AL_SPI_EventStruct SpiEvent = {
            .Events         = EventId,
            .EventData      = 0,
        };
        (*Spi->EventCallBack)(SpiEvent, Spi->EventCallBackRef);
    }
}

/**
 *
 * This function is called when an interrupt is triggered for the SPI device.
 * It checks the interrupt status and performs the necessary actions based on the interrupt type.
 *
 * @param instance Pointer to the SPI device instance.
 *
 * @return None.
 *
 */
AL_VOID AlSpi_Dev_IntrHandler(AL_VOID *instance)
{
    AL_SPI_DevStruct *Spi = (AL_SPI_DevStruct *) instance;
    AL_SPI_IntrStatusEnum IntrStatus = AlSpi_ll_GetIntrStatus(Spi->HwConfig.BaseAddress);

    if (SPI_IN_RX_FF_INTR(IntrStatus)) {
        AlSpi_Dev_RecvDataHandler(Spi);
    }

    if (SPI_IN_TX_FE_INTR(IntrStatus)) {
        AlSpi_Dev_SendDataHandler(Spi);
    }

    if (SPI_IN_TX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error spi tx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "TXFLR:0x%x\r\n", AlSpi_ll_ReadTxFifoLevel(Spi->HwConfig.BaseAddress));
        AlSpi_Dev_EventHandler(Spi, AL_SPI_TX_FO);
        Spi->State |= AL_SPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlSpi_ll_ClearAllIntr(Spi->HwConfig.BaseAddress);
    }

    if (SPI_IN_RX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error spi rx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "RXFLR:0x%x\r\n", AlSpi_ll_ReadRxFifoLevel(Spi->HwConfig.BaseAddress));
        AlSpi_Dev_EventHandler(Spi, AL_SPI_RX_FO);
        Spi->State |= AL_SPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlSpi_ll_ClearAllIntr(Spi->HwConfig.BaseAddress);
    }

    if (SPI_IN_RX_FU_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error spi rx fifo underflow intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "RXFLR:0x%x\r\n", AlSpi_ll_ReadRxFifoLevel(Spi->HwConfig.BaseAddress));
        AlSpi_Dev_EventHandler(Spi, AL_SPI_RX_FU);
        Spi->State |= AL_SPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlSpi_ll_ClearAllIntr(Spi->HwConfig.BaseAddress);
    }
}


