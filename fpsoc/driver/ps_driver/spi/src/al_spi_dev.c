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
    .TransMode          = SPI_TX_ONLY,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .DataFrameSize      = SPI_FRAME_8BITS,
    .ClkDiv             = 70,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
    .SlvSelEnum         = SPI_SER_SS0_EN,
    .IsUseDma           = AL_SPI_USE_INTR
};

extern AL_SPI_HwConfigStruct AlSpi_HwCfg[AL_SPI_NUM_INSTANCE];
/************************** Function Prototypes ******************************/

/********************************************************/
/**
 * This function look up hardware config structure
 * @param   DeviceId is hardware module id
 * @return  hardware config structure pointer with AL_SPI_HwConfigStruct
 * @note    None
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
 * This function look up hardware config structure
 * @param   Spi is structure pointer to spi device
 * @return  AL_VOID
 * @note    None
*/
AL_BOOL AlSpi_Dev_IsTxBusy(AL_SPI_DevStruct *Spi)
{
    return (AL_BOOL)(Spi->State & AL_SPI_STATE_TX_BUSY);
}

/**
 * This function is set spi AL_SPI_DevStruct struct rx busy state
 * @param   Spi is structure pointer to spi device
 * @return  AL_VOID
 * @note    None
*/
AL_BOOL AlSpi_Dev_IsRxBusy(AL_SPI_DevStruct *Spi)
{
    return (AL_BOOL)(Spi->State & AL_SPI_STATE_RX_BUSY);
}

/**
 * This function is set spi AL_SPI_DevStruct struct tx busy state
 * @param   Spi is structure pointer to spi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlSpi_Dev_SetTxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State |= AL_SPI_STATE_TX_BUSY;
}

/**
 * This function is clear spi AL_SPI_DevStruct struct rx busy state
 * @param   Spi is structure pointer to spi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlSpi_Dev_SetRxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State |= AL_SPI_STATE_RX_BUSY;
}

/**
 * This function is clear spi AL_SPI_DevStruct struct tx busy state
 * @param   Spi is structure pointer to spi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlSpi_Dev_ClrTxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State &= (~AL_SPI_STATE_TX_BUSY);
}

/**
 * This function is clear spi AL_SPI_DevStruct struct rx busy state
 * @param   Spi is structure pointer to spi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlSpi_Dev_ClrRxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State &= (~AL_SPI_STATE_RX_BUSY);
}

#ifdef SPI_DEBUG
/**
 * This function prints the values of all registers in the spi
 * @param   SpiBaseAddr is spi base address
 * @return  AL_VOID
 * @note    None
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
 * This function initializes spi through the AL_SPI_ConfigsStruct struct
 * @param   Spi is structure pointer to spi device
 * @param   InitConfig is spi configuration parameter struct
 * @return  Whether the spi is initialized successfully
 * @note    if InitConfig is null will use SpiDefInitConfigs
*/
AL_S32 AlSpi_Dev_Init(AL_SPI_DevStruct *Spi, AL_SPI_ConfigsStruct *InitConfig)
{
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    Spi->Configs = (InitConfig == AL_NULL) ? SpiDefInitConfigs : (*InitConfig);

    Spi->BaseAddr = AlSpi_ll_SetSpiMasterSlave(Spi->BaseAddr, Spi->Configs.Mode);

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetProtFormat(Spi->BaseAddr, Spi->Configs.ProtFormat);
    AlSpi_ll_SetCpolAndCpha(Spi->BaseAddr, Spi->Configs.ClockEnum);
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, Spi->Configs.DataFrameSize);
    AlSpi_ll_SetClkDiv(Spi->BaseAddr, Spi->Configs.ClkDiv);
    AlSpi_ll_SetSlvSelToggle(Spi->BaseAddr, Spi->Configs.SlvToggleEnum);
    AlSpi_ll_MaskIntr(Spi->BaseAddr, SPI_TXEIM | SPI_RXFIM);
    AlSpi_ll_Enable(Spi->BaseAddr);

    Spi->State |= AL_SPI_STATE_READY;

    return AL_OK;
}

/**
 * This function is spi send data
 * @param   Spi is structure pointer to spi device
 * @param   SendBuf is send data buffer pointer
 * @param   SendSize is receive data size
 * @return  Whether the spi send data successfully
 * @note    None
*/
AL_S32 AlSpi_Dev_SendData(AL_SPI_DevStruct *Spi, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U32 SendLevel, SendValue = 0xFFFFFFFF;

    if (Spi == AL_NULL || SendBuf == AL_NULL || SendSize == 0) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (SPI_SR_TXFIFO_FULL == AlSpi_ll_IsTxFifoFull(Spi->BaseAddr)) {
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
        Spi->Configs.DataFrameSize = SPI_FRAME_32BITS;
        Spi->BitsPerWord = 32;
    } else if (0 == (SendSize % 2)) {
        Spi->Configs.DataFrameSize = SPI_FRAME_16BITS;
        Spi->BitsPerWord = 16;
    } else {
        Spi->Configs.DataFrameSize = SPI_FRAME_8BITS;
        Spi->BitsPerWord = 8;
    }

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, Spi->Configs.DataFrameSize);

    AlSpi_ll_Enable(Spi->BaseAddr);

    SendLevel = MIN((AL_U32)(Spi->Fifolen - AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr)), Spi->SendBuffer.RequestedCnt / (Spi->BitsPerWord >> 3));

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

        AlSpi_ll_DataTransmit(Spi->BaseAddr, SendValue);
        Spi->SendBuffer.HandledCnt += Spi->BitsPerWord >> 3;
    }

    SendLevel = MIN((AL_U32)(Spi->Fifolen / 2), SendSize / (Spi->BitsPerWord >> 3));
    AlSpi_ll_SetTxFifoThrLevel(Spi->BaseAddr, SendLevel);

    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_TXEIM);

    return AL_OK;
}

/**
 * This function is to start spi receive data
 * @param   Spi is structure pointer to spi device
 * @param   ReceiveBuf is send data buffer pointer
 * @param   ReceiveSize is receive data size
 * @return  Whether the spi receive data successfully
 * @note    None
*/
AL_S32 AlSpi_Dev_RecvData(AL_SPI_DevStruct *Spi, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 Temp;

    if (Spi == AL_NULL || ReceiveBuf == AL_NULL || ReceiveSize == 0 ) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (SPI_SR_RXFIFO_NOTEMPTY== AlSpi_ll_IsRxFifoEmpty(Spi->BaseAddr)) {
        return AL_SPI_ERR_NOT_SUPPORT;
    }

    if (AlSpi_Dev_IsRxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_Dev_SetRxBusy(Spi);

    Spi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Spi->RecvBuffer.RequestedCnt = ReceiveSize;
    Spi->RecvBuffer.HandledCnt   = 0;

    if (0 == (ReceiveSize % 4)) {
        Spi->Configs.DataFrameSize = SPI_FRAME_32BITS;
        Spi->BitsPerWord = 32;
    } else if (0 == (ReceiveSize % 2)) {
        Spi->Configs.DataFrameSize = SPI_FRAME_16BITS;
        Spi->BitsPerWord = 16;
    } else {
        Spi->Configs.DataFrameSize = SPI_FRAME_8BITS;
        Spi->BitsPerWord = 8;
    }

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    Temp = ReceiveSize / (Spi->BitsPerWord >> 3);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->BaseAddr, Temp ? Temp - 1 : 0);
    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, Spi->Configs.DataFrameSize);

    /* If the fifo width is set to more than 8, the received data will have byte order problems,
     * you can enable byte order reversal */
    if (Spi->BitsPerWord != 8) {
        AlSpi_ll_EnableEndianConver(Spi->BaseAddr);
    } else {
        AlSpi_ll_DisableEndianConver(Spi->BaseAddr);
    }

    AlSpi_ll_Enable(Spi->BaseAddr);
    Temp = MIN(ReceiveSize / (Spi->BitsPerWord >> 3), (AL_U16)(Spi->Fifolen / 2));
    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, Temp ? Temp - 1 : 0);
    //  AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, 0);
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_RXFIM);

    return AL_OK;
}

/**
 * This function is to spi tranfer in full-duplex mode
 * @param   Spi is structure pointer to spi device
 * @param   SendBuf is send data buffer pointer
 * @param   SendSize is send data size
 * @param   ReceiveBuf is send data buffer pointer
 * @param   ReceiveSize is receive data size
 * @return  Whether the spi transfer data successfully
 * @note    None
*/
AL_S32 AlSpi_Dev_TranferData(AL_SPI_DevStruct *Spi, AL_U8 *SendBuf, AL_U32 SendSize, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 ReceiveLevel, SendLevel, SendValue = 0xffffffff, Temp;

    if (Spi == AL_NULL || SendBuf == AL_NULL || ReceiveBuf == AL_NULL || SendSize == 0 || ReceiveSize == 0) {
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
        Spi->Configs.DataFrameSize = SPI_FRAME_32BITS;
        Spi->BitsPerWord = 32;
    } else if ((0 == (SendSize % 2)) && (0 == (ReceiveSize % 2))) {
        Spi->Configs.DataFrameSize = SPI_FRAME_16BITS;
        Spi->BitsPerWord = 16;
    } else {
        Spi->Configs.DataFrameSize = SPI_FRAME_8BITS;
        Spi->BitsPerWord = 8;
    }

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);

    Temp = ReceiveSize / (Spi->BitsPerWord >> 3);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->BaseAddr, Temp ? Temp - 1 : 0);
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, Spi->Configs.DataFrameSize);

    /* If the fifo width is set to more than 8, the received data will have byte order problems,
     * you can enable byte order reversal */
    if (Spi->BitsPerWord != 8) {
        AlSpi_ll_EnableEndianConver(Spi->BaseAddr);
    } else {
        AlSpi_ll_DisableEndianConver(Spi->BaseAddr);
    }

    AlSpi_ll_Enable(Spi->BaseAddr);


    SendLevel = MIN3(Spi->Fifolen - AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr), Spi->SendBuffer.RequestedCnt / (Spi->BitsPerWord >> 3),
                     Spi->Fifolen - (Spi->RecvBuffer.RequestedCnt - Spi->SendBuffer.RequestedCnt)/ (Spi->BitsPerWord >> 3));

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

        AlSpi_ll_DataTransmit(Spi->BaseAddr, SendValue);
        Spi->SendBuffer.HandledCnt += Spi->BitsPerWord >> 3;
    }

    SendLevel = MIN((AL_U32)(Spi->Fifolen / 2), SendSize / (Spi->BitsPerWord >> 3));
    Temp =  MIN(ReceiveSize / (Spi->BitsPerWord >> 3), (AL_U16)(Spi->Fifolen / 2));
    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, Temp ? Temp - 1 : 0);
    AlSpi_ll_SetTxFifoThrLevel(Spi->BaseAddr, SendLevel);

    /* Set cs to start transfer */
    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_RXFIM | SPI_TXEIM);


    return AL_OK;
}

/**
 * This function
 * @param   Spi is structure pointer to spi device
 * @return
 * @note    None
*/
AL_S32 AlSpi_Dev_DmaSendData(AL_SPI_DevStruct *Spi)
{
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (SPI_SR_TXFIFO_FULL == AlSpi_ll_IsTxFifoFull(Spi->BaseAddr)) {
        return AL_SPI_ERR_NOT_SUPPORT;
    }

    if (AlSpi_Dev_IsTxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, SPI_FRAME_8BITS);
    AlSpi_ll_SetTxFifoThrLevel(Spi->BaseAddr, 0);
    AlSpi_ll_TxDmaEnable(Spi->BaseAddr);
    AlSpi_ll_SetDmaTransLevel(Spi->BaseAddr, 0);
    AlSpi_ll_Enable(Spi->BaseAddr);

    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);

    return AL_OK;
}

/**
 * This function
 * @param   Spi is structure pointer to spi device
 * @return
 * @note    None
*/
AL_S32 AlSpi_Dev_DmaRecvData(AL_SPI_DevStruct *Spi, AL_U16 RecvSize)
{
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (SPI_SR_RXFIFO_NOTEMPTY== AlSpi_ll_IsRxFifoEmpty(Spi->BaseAddr)) {
        return AL_SPI_ERR_NOT_SUPPORT;
    }

    if (AlSpi_Dev_IsRxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->BaseAddr, RecvSize - 1);

    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, SPI_FRAME_8BITS);
    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, 0);

    AlSpi_ll_RxDmaEnable(Spi->BaseAddr);
    AlSpi_ll_SetDmaRecevLevel(Spi->BaseAddr, 0);
    AlSpi_ll_Enable(Spi->BaseAddr);

    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);

    return AL_OK;
}

/**
 * This function
 * @param   Spi is structure pointer to spi device
 * @return
 * @note    None
*/
AL_S32 AlSpi_Dev_DmaTranferData(AL_SPI_DevStruct *Spi, AL_U16 RecvSize)
{

    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (AlSpi_Dev_IsRxBusy(Spi) || AlSpi_Dev_IsTxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->BaseAddr, RecvSize - 1);
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, SPI_FRAME_8BITS);

    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, 0);
    AlSpi_ll_SetTxFifoThrLevel(Spi->BaseAddr, 0);

    AlSpi_ll_TxDmaEnable(Spi->BaseAddr);
    AlSpi_ll_RxDmaEnable(Spi->BaseAddr);

    // AlSpi_ll_SetDmaRecevLevel(Spi->BaseAddr, Spi->Fifolen / 2 - 1);
    // AlSpi_ll_SetDmaTransLevel(Spi->BaseAddr, Spi->Fifolen / 2);

    AlSpi_ll_SetDmaRecevLevel(Spi->BaseAddr, 0);
    AlSpi_ll_SetDmaTransLevel(Spi->BaseAddr, 0);

    AlSpi_ll_Enable(Spi->BaseAddr);
    /* Set cs to start transfer */
    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);

    return AL_OK;
}

/**
 * This function
 * @param   Spi is structure pointer to spi device
 * @param   Cmd is io ctrl enum to set spi status
 * @param   Data is AL_VOID pointer converted to AL_SPI_SlvSelEnum pointer
 * @return  Whether the spi is io ctrl successfully
 * @note    None
*/
AL_S32 AlSpi_Dev_IoCtl(AL_SPI_DevStruct *Spi, AL_Spi_IoCtlCmdEnum Cmd, AL_VOID *Data)
{
    AL_S32 Ret = AL_OK;

    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    switch (Cmd)
    {
    case AL_SPI_IOCTL_SET_SLAVE_SELECT: {
        AL_SPI_SlvSelEnum SlvSel = *(AL_SPI_SlvSelEnum *)Data;
        Spi->Configs.SlvSelEnum = SlvSel;
        break;
    }

    case AL_SPI_IOCTL_GET_SLAVE_SELECT: {
        AL_SPI_SlvSelEnum *SlvSel = (AL_SPI_SlvSelEnum *)Data;
        *SlvSel = Spi->Configs.SlvSelEnum;
        break;
    }

    default:
        return AL_SPI_ERR_NOT_SUPPORT;
        break;
    }

    return Ret;
}

/**
 * This function is register spi intrrupt callback function
 * @param   Spi is structure pointer to spi device
 * @param   Callback is a function pointer to spi event callback function
 * @param   CallbackRef is parameter of callback function
 * @return  Whether the spi register intrrupt callback function successfully
 * @note    None
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
 * This function is unregister spi intrrupt callback function
 * @param   Spi is structure pointer to spi device
 * @return  Whether the spi unregister intrrupt callback function successfully
 * @note    None
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
 * This function is spi receive intrrupt handler
 * @param   Spi is structure pointer to spi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlSpi_Dev_RecvDataHandler(AL_SPI_DevStruct *Spi)
{
    AL_U32 Length, RxFifoLevel, Status, Temp;



    Length = Spi->RecvBuffer.RequestedCnt - Spi->RecvBuffer.HandledCnt;

    if(Length) {
        RxFifoLevel = AlSpi_ll_ReadRxFifoLevel(Spi->BaseAddr);
        if(!RxFifoLevel) {
            Status = AlSpi_ll_ReadRawIntrStatus(Spi->BaseAddr);
            if (Status & RXOIS) {
                /* FIFO overflow on Rx */
                AL_LOG(AL_LOG_LEVEL_INFO, "Error FIFO overflow on Rx\r\n");
                Spi->State |= AL_SPI_STATE_ERROR;
            }
        } else if ((Spi->Fifolen == RxFifoLevel) && (Spi->Configs.TransMode == SPI_EEPROM)) {
            AL_LOG(AL_LOG_LEVEL_INFO, "Error FIFO Full on Rx, cs high\r\n");
            Spi->State |= AL_SPI_STATE_ERROR;
        }

        RxFifoLevel = MIN(RxFifoLevel, Length / (Spi->BitsPerWord >> 3));
        for (; RxFifoLevel; --RxFifoLevel) {
            if (Spi->BitsPerWord == 8) {
                *(AL_U8 *)(Spi->RecvBuffer.BufferPtr + Spi->RecvBuffer.HandledCnt) = (AL_U8)AlSpi_ll_DataReceive(Spi->BaseAddr);
            } else if (Spi->BitsPerWord == 16) {
                *(AL_U16 *)(Spi->RecvBuffer.BufferPtr + Spi->RecvBuffer.HandledCnt) = (AL_U16)AlSpi_ll_DataReceive(Spi->BaseAddr);
            } else if (Spi->BitsPerWord == 32) {
                *(AL_U32 *)(Spi->RecvBuffer.BufferPtr + Spi->RecvBuffer.HandledCnt) = AlSpi_ll_DataReceive(Spi->BaseAddr);
            }
            Spi->RecvBuffer.HandledCnt += Spi->BitsPerWord >> 3;
            Length = Spi->RecvBuffer.RequestedCnt - Spi->RecvBuffer.HandledCnt;

        }
    }


    Temp =  MIN(Length / (Spi->BitsPerWord >> 3), (AL_U32)(Spi->Fifolen / 2));
    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, Temp ? Temp - 1 : 0);


    if (Spi->RecvBuffer.HandledCnt == Spi->RecvBuffer.RequestedCnt) {
        AlSpi_ll_MaskIntr(Spi->BaseAddr, SPI_RXFIM);

        if (Spi->EventCallBack) {
            AL_SPI_EventStruct SpiEvent = {
                .Event        = AL_SPI_RECEIVE_DONE,
                .EventData    = Spi->SendBuffer.HandledCnt
            };
            AlSpi_Dev_ClrRxBusy(Spi);
            Spi->EventCallBack(SpiEvent, Spi->EventCallBackRef);
        }
    }
}

/**
 * This function is spi send intrrupt handler
 * @param   Spi is structure pointer to spi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlSpi_Dev_SendDataHandler(AL_SPI_DevStruct *Spi)
{
    AL_U32 TxFifoLevel, Length, Room, SendValue = 0xffffffff;


    Length = Spi->SendBuffer.RequestedCnt - Spi->SendBuffer.HandledCnt;

    if (Length) {
        TxFifoLevel = AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr);

        if (!TxFifoLevel) {
            /* return error fifo entries */
            AL_LOG(AL_LOG_LEVEL_INFO, "Error Tx fifo enteries, cs high\r\n");
            Spi->State |= AL_SPI_STATE_ERROR;
        }

        Room = MIN((AL_U32)(Spi->Fifolen - TxFifoLevel), Length / (Spi->BitsPerWord >> 3));
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

            AlSpi_ll_DataTransmit(Spi->BaseAddr, SendValue);
            Spi->SendBuffer.HandledCnt += Spi->BitsPerWord >> 3;
            Length = Spi->SendBuffer.RequestedCnt - Spi->SendBuffer.HandledCnt;
        }
    }


    if (Spi->SendBuffer.HandledCnt == Spi->SendBuffer.RequestedCnt) {
        AlSpi_ll_MaskIntr(Spi->BaseAddr, SPI_TXEIM);

        if (Spi->EventCallBack) {
            AL_SPI_EventStruct event = {
                .Event         = AL_SPI_SEND_DONE,
                .EventData    = Spi->SendBuffer.HandledCnt
            };
            AlSpi_Dev_ClrTxBusy(Spi);
            Spi->EventCallBack(event, Spi->EventCallBackRef);
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
 * This function is spi intrrupt handler function
 * @param   instance is AL_VOID pointer converted to AL_SPI_DevStruct pointer
 * @return  AL_VOID
 * @note    None
*/
AL_VOID AlSpi_Dev_IntrHandler(AL_VOID *instance)
{
    AL_SPI_DevStruct *Spi = (AL_SPI_DevStruct *) instance;
    AL_SPI_IntrStatusEnum IntrStatus = AlSpi_ll_GetIntrStatus(Spi->BaseAddr);

    if (SPI_IN_RX_FF_INTR(IntrStatus)) {
        AlSpi_Dev_RecvDataHandler(Spi);
    }

    if (SPI_IN_TX_FE_INTR(IntrStatus)) {
        AlSpi_Dev_SendDataHandler(Spi);
    }

    if (SPI_IN_TX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error spi tx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "TXFLR:0x%x\r\n", AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr));
        Spi->State |= AL_SPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlSpi_ll_ClearAllIntr(Spi->BaseAddr);
    }

    if (SPI_IN_RX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error spi rx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "RXFLR:0x%x\r\n", AlSpi_ll_ReadRxFifoLevel(Spi->BaseAddr));
        Spi->State |= AL_SPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlSpi_ll_ClearAllIntr(Spi->BaseAddr);
    }

    if (SPI_IN_RX_FU_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error spi rx fifo underflow intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "RXFLR:0x%x\r\n", AlSpi_ll_ReadRxFifoLevel(Spi->BaseAddr));
        Spi->State |= AL_SPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlSpi_ll_ClearAllIntr(Spi->BaseAddr);
    }

}


