/***************************** Include Files *********************************/
#include "al_qspi_dev.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
/* Default init config */
static AL_QSPI_ConfigsStruct QSPIDefInitConfigs =
{
    .Mode               = QSPI_MASTER_MODE,
    .TransMode          = QSPI_TX_ONLY,
    .ProtFormat         = MOTOROLA_QSPI,
    .ClockEnum          = QSPI_CLK_MODE0,
    .DataFrameSize      = QSPI_FRAME_8BITS,
    .ClkDiv             = 70,
    .SlvToggleEnum      = QSPI_SLV_TOGGLE_DISABLE,
    .SlvSelEnum         = QSPI_SER_SS0_EN,
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
AL_QSPI_HwConfigStruct *AlQSPI_Dev_LookupConfig(AL_U32 DeviceId)
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
AL_BOOL AlQSPI_Dev_IsTxBusy(AL_QSPI_DevStruct *Qspi)
{
    return (AL_BOOL)(Qspi->State & AL_QSPI_STATE_TX_BUSY);
}

/**
 * This function is set qspi AL_QSPI_DevStruct struct rx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
AL_BOOL AlQSPI_Dev_IsRxBusy(AL_QSPI_DevStruct *Qspi)
{
    return (AL_BOOL)(Qspi->State & AL_QSPI_STATE_RX_BUSY);
}

/**
 * This function is set qspi AL_QSPI_DevStruct struct tx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQSPI_Dev_SetTxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State |= AL_QSPI_STATE_TX_BUSY;
}

/**
 * This function is clear qspi AL_QSPI_DevStruct struct rx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQSPI_Dev_SetRxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State |= AL_QSPI_STATE_RX_BUSY;
}

/**
 * This function is clear qspi AL_QSPI_DevStruct struct tx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQSPI_Dev_ClrTxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State &= (~AL_QSPI_STATE_TX_BUSY);
}

/**
 * This function is clear qspi AL_QSPI_DevStruct struct rx busy state
 * @param   Qspi is structure pointer to qspi device
 * @return  AL_VOID
 * @note    None
*/
static AL_VOID AlQSPI_Dev_ClrRxBusy(AL_QSPI_DevStruct *Qspi)
{
    Qspi->State &= (~AL_QSPI_STATE_RX_BUSY);
}

#ifdef QSPI_DEBUG
/**
 * This function prints the values of all registers in the qspi
 * @param   QSPIBaseAddr is qspi base address
 * @return  AL_VOID
 * @note    None
*/
AL_VOID AlQSPI_Dev_DumpReg(AL_REG QSPIBaseAddr)
{
    AL_LOG(AL_LOG_LEVEL_DEBUG, "CTRLR0:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_CTRLR0_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "CTRLR1:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_CTRLR1_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SSIENR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_SSIENR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "MWCR:0x%x\r\n",    AL_REG32_READ(QSPIBaseAddr + QSPI_MWCR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SER:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_SER_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "BAUDR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_BAUDR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXFTLR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_TXFTLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXFTLR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_RXFTLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXFLR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_TXFLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXFLR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_RXFLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SR:0x%x\r\n",      AL_REG32_READ(QSPIBaseAddr + QSPI_SR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "IMR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_IMR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "ISR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_ISR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RISR:0x%x\r\n",    AL_REG32_READ(QSPIBaseAddr + QSPI_RISR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "TXOICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_TXOICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXOICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_RXOICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "RXUICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_RXUICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "MSTICR:0x%x\r\n",  AL_REG32_READ(QSPIBaseAddr + QSPI_MSTICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "ICR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_ICR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMACR:0x%x\r\n",   AL_REG32_READ(QSPIBaseAddr + QSPI_DMACR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMATDLR:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_DMATDLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "DMARDLR:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_DMARDLR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "IDR:0x%x\r\n",     AL_REG32_READ(QSPIBaseAddr + QSPI_IDR_MST_OFFSET));
    AL_LOG(AL_LOG_LEVEL_DEBUG, "SSI_VERSION_ID:0x%x\r\n", AL_REG32_READ(QSPIBaseAddr + QSPI_SSI_VERSION_ID_MST_OFFSET));
}
#endif

/**
 * This function initializes qspi through the AL_QSPI_ConfigsStruct struct
 * @param   Qspi is structure pointer to qspi device
 * @param   InitConfig is qspi configuration parameter struct
 * @return  Whether the qspi is initialized successfully
 * @note    if InitConfig is null will use QSPIDefInitConfigs
*/
AL_S32 AlQSPI_Dev_Init(AL_QSPI_DevStruct *Qspi, AL_QSPI_ConfigsStruct *InitConfig)
{
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    Qspi->Configs = (InitConfig == AL_NULL) ? QSPIDefInitConfigs : (*InitConfig);

    AlQSPI_ll_Disable(Qspi->BaseAddr);
    AlQSPI_ll_SetQSPIMode(Qspi->BaseAddr, Qspi->Configs.Mode);
    AlQSPI_ll_SetProtFormat(Qspi->BaseAddr, Qspi->Configs.ProtFormat);
    AlQSPI_ll_SetCpolAndCpha(Qspi->BaseAddr, Qspi->Configs.ClockEnum);
    AlQSPI_ll_SetDataFrameSize(Qspi->BaseAddr, Qspi->Configs.DataFrameSize);
    AlQSPI_ll_SetClkDiv(Qspi->BaseAddr, Qspi->Configs.ClkDiv);
    AlQSPI_ll_SetSlvSelToggle(Qspi->BaseAddr, Qspi->Configs.SlvToggleEnum);
    AlQSPI_ll_MaskIntr(Qspi->BaseAddr, QSPI_TXEIM | QSPI_RXFIM);
    AlQSPI_ll_Enable(Qspi->BaseAddr);

    Qspi->State |= AL_QSPI_STATE_READY;

    return AL_OK;
}

/**
 * This function is qspi send data
 * @param   Qspi is structure pointer to qspi device
 * @param   SendBuf is send data buffer pointer
 * @param   SendSize is receive data size
 * @return  Whether the qspi send data successfully
 * @note    None
*/
AL_S32 AlQSPI_Dev_SendData(AL_QSPI_DevStruct *Qspi, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U32 SendLevel, SendValue = 0xFFFFFFFF;

    if (Qspi == AL_NULL || SendBuf == AL_NULL || SendSize == 0) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_TXFIFO_FULL == AlQSPI_ll_IsTxFifoFull(Qspi->BaseAddr)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQSPI_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQSPI_Dev_SetTxBusy(Qspi);

    Qspi->SendBuffer.BufferPtr    = SendBuf;
    Qspi->SendBuffer.HandledCnt   = 0;
    Qspi->SendBuffer.RequestedCnt = SendSize;

    /* Different fifo widths are set depending on the amount of data */
    if (0 == (SendSize % 4)) {
        Qspi->Configs.DataFrameSize = QSPI_FRAME_32BITS;
        Qspi->BitsPerWord = 32;
    } else if (0 == (SendSize % 2)) {
        Qspi->Configs.DataFrameSize = QSPI_FRAME_16BITS;
        Qspi->BitsPerWord = 16;
    } else {
        Qspi->Configs.DataFrameSize = QSPI_FRAME_8BITS;
        Qspi->BitsPerWord = 8;
    }

    AlQSPI_ll_Disable(Qspi->BaseAddr);
    AlQSPI_ll_SetTransfMode(Qspi->BaseAddr, Qspi->Configs.TransMode);
    AlQSPI_ll_SetDataFrameSize(Qspi->BaseAddr, Qspi->Configs.DataFrameSize);
    AlQSPI_ll_Enable(Qspi->BaseAddr);

    SendLevel = MIN(Qspi->Fifolen - AlQSPI_ll_ReadTxFifoLevel(Qspi->BaseAddr), Qspi->SendBuffer.RequestedCnt / (Qspi->BitsPerWord >> 3));

    while (SendLevel--) {
        if (Qspi->BitsPerWord == 8) {
            SendValue = *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        } else if (Qspi->BitsPerWord == 16) {
            SendValue = *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            SendValue = QSPI_ENDIAN_SWAP16(SendValue);
        } else {
            SendValue = *(AL_U32 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            SendValue = QSPI_ENDIAN_SWAP32(SendValue);
        }

        AlQSPI_ll_DataTransmit(Qspi->BaseAddr, SendValue);
        Qspi->SendBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
    }

    SendLevel = MIN((Qspi->Fifolen / 2), SendSize / (Qspi->BitsPerWord >> 3));
    AlQSPI_ll_SetTxFifoThrLevel(Qspi->BaseAddr, SendLevel);

    AlQSPI_ll_SetSlvSel(Qspi->BaseAddr, Qspi->Configs.SlvSelEnum);
    AlQSPI_ll_EnableIntr(Qspi->BaseAddr, QSPI_TXEIM);

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
AL_S32 AlQSPI_Dev_RecvData(AL_QSPI_DevStruct *Qspi, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 Temp;

    if (Qspi == AL_NULL || ReceiveBuf == AL_NULL || ReceiveSize == 0 ) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_RXFIFO_NOTEMPTY== AlQSPI_ll_IsRxFifoEmpty(Qspi->BaseAddr)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQSPI_Dev_IsRxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQSPI_Dev_SetRxBusy(Qspi);

    Qspi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Qspi->RecvBuffer.RequestedCnt = ReceiveSize;
    Qspi->RecvBuffer.HandledCnt   = 0;

    if (0 == (ReceiveSize % 4)) {
        Qspi->Configs.DataFrameSize = QSPI_FRAME_32BITS;
        Qspi->BitsPerWord = 32;
    } else if (0 == (ReceiveSize % 2)) {
        Qspi->Configs.DataFrameSize = QSPI_FRAME_16BITS;
        Qspi->BitsPerWord = 16;
    } else {
        Qspi->Configs.DataFrameSize = QSPI_FRAME_8BITS;
        Qspi->BitsPerWord = 8;
    }

    AlQSPI_ll_Disable(Qspi->BaseAddr);
    AlQSPI_ll_SetTransfMode(Qspi->BaseAddr, Qspi->Configs.TransMode);
    Temp = ReceiveSize / (Qspi->BitsPerWord >> 3);
    AlQSPI_ll_SetRecvNumOfDataFrames(Qspi->BaseAddr, Temp ? Temp - 1 : 0);
    AlQSPI_ll_SetSlvSel(Qspi->BaseAddr, Qspi->Configs.SlvSelEnum);
    AlQSPI_ll_SetDataFrameSize(Qspi->BaseAddr, Qspi->Configs.DataFrameSize);

    /* If the fifo width is set to more than 8, the received data will have byte order problems,
     * you can enable byte order reversal */
    if (Qspi->BitsPerWord != 8) {
        AlQSPI_ll_EnableEndianConver(Qspi->BaseAddr);
    } else {
        AlQSPI_ll_DisableEndianConver(Qspi->BaseAddr);
    }

    AlQSPI_ll_Enable(Qspi->BaseAddr);
    Temp = MIN(ReceiveSize / (Qspi->BitsPerWord >> 3), Qspi->Fifolen / 2);
    AlQSPI_ll_SetRxFifoThrLevel(Qspi->BaseAddr, Temp ? Temp - 1 : 0);
    //  AlQSPI_ll_SetRxFifoThrLevel(Qspi->BaseAddr, 0);
    AlQSPI_ll_EnableIntr(Qspi->BaseAddr, QSPI_RXFIM);

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
AL_S32 AlQSPI_Dev_TranferData(AL_QSPI_DevStruct *Qspi, AL_U8 *SendBuf, AL_U32 SendSize, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 ReceiveLevel, SendLevel, SendValue = 0xffffffff, Temp;

    if (Qspi == AL_NULL || SendBuf == AL_NULL || ReceiveBuf == AL_NULL || SendSize == 0 || ReceiveSize == 0) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (AlQSPI_Dev_IsRxBusy(Qspi) || AlQSPI_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQSPI_Dev_SetRxBusy(Qspi);
    AlQSPI_Dev_SetTxBusy(Qspi);

    Qspi->SendBuffer.BufferPtr    = SendBuf;
    Qspi->SendBuffer.RequestedCnt = SendSize;
    Qspi->SendBuffer.HandledCnt   = 0;
    Qspi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Qspi->RecvBuffer.RequestedCnt = ReceiveSize;
    Qspi->RecvBuffer.HandledCnt   = 0;

    if ((0 == (SendSize % 4)) && (0 == (ReceiveSize % 4))) {
        Qspi->Configs.DataFrameSize = QSPI_FRAME_32BITS;
        Qspi->BitsPerWord = 32;
    } else if ((0 == (SendSize % 2)) && (0 == (ReceiveSize % 2))) {
        Qspi->Configs.DataFrameSize = QSPI_FRAME_16BITS;
        Qspi->BitsPerWord = 16;
    } else {
        Qspi->Configs.DataFrameSize = QSPI_FRAME_8BITS;
        Qspi->BitsPerWord = 8;
    }

    AlQSPI_ll_Disable(Qspi->BaseAddr);
    AlQSPI_ll_SetTransfMode(Qspi->BaseAddr, Qspi->Configs.TransMode);
    Temp = ReceiveSize / (Qspi->BitsPerWord >> 3);
    AlQSPI_ll_SetRecvNumOfDataFrames(Qspi->BaseAddr, Temp ? Temp - 1 : 0);
    AlQSPI_ll_SetDataFrameSize(Qspi->BaseAddr, Qspi->Configs.DataFrameSize);

    /* If the fifo width is set to more than 8, the received data will have byte order problems,
     * you can enable byte order reversal */
    if (Qspi->BitsPerWord != 8) {
        AlQSPI_ll_EnableEndianConver(Qspi->BaseAddr);
    } else {
        AlQSPI_ll_DisableEndianConver(Qspi->BaseAddr);
    }

    AlQSPI_ll_Enable(Qspi->BaseAddr);

    SendLevel = MIN3(Qspi->Fifolen - AlQSPI_ll_ReadTxFifoLevel(Qspi->BaseAddr), Qspi->SendBuffer.RequestedCnt / (Qspi->BitsPerWord >> 3),
                     Qspi->Fifolen - (Qspi->RecvBuffer.RequestedCnt - Qspi->SendBuffer.RequestedCnt)/ (Qspi->BitsPerWord >> 3));

    while (SendLevel--) {

        /* qspi send data can only be to adjust the byte order by software */
        if (Qspi->BitsPerWord == 8) {
            SendValue = *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
        } else if (Qspi->BitsPerWord == 16) {
            SendValue = *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            SendValue = QSPI_ENDIAN_SWAP16(SendValue);
        } else {
            SendValue = *(AL_U32 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            SendValue = QSPI_ENDIAN_SWAP32(SendValue);
        }

        AlQSPI_ll_DataTransmit(Qspi->BaseAddr, SendValue);
        Qspi->SendBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
    }

    SendLevel = MIN((Qspi->Fifolen / 2), SendSize / (Qspi->BitsPerWord >> 3));
    Temp =  MIN(ReceiveSize / (Qspi->BitsPerWord >> 3), Qspi->Fifolen / 2);
    AlQSPI_ll_SetRxFifoThrLevel(Qspi->BaseAddr, Temp ? Temp - 1 : 0);
    AlQSPI_ll_SetTxFifoThrLevel(Qspi->BaseAddr, SendLevel);

    /* Set cs to start transfer */
    AlQSPI_ll_SetSlvSel(Qspi->BaseAddr, Qspi->Configs.SlvSelEnum);
    AlQSPI_ll_EnableIntr(Qspi->BaseAddr, QSPI_RXFIM | QSPI_TXEIM);

    return AL_OK;
}

/**
 * This function
 * @param   Qspi is structure pointer to qspi device
 * @return
 * @note    None
*/
AL_S32 AlQSPI_Dev_DmaSendData(AL_QSPI_DevStruct *Qspi)
{
    AL_U32 DefBurst = Qspi->Fifolen / 2;

    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_TXFIFO_FULL == AlQSPI_ll_IsTxFifoFull(Qspi->BaseAddr)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQSPI_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQSPI_Dev_SetTxBusy(Qspi);

    AlQSPI_ll_TxDmaEnable(Qspi->BaseAddr);

    AlQSPI_ll_SetDmaTransLevel(Qspi->BaseAddr, MIN(DefBurst, 256));

    return AL_OK;
}

/**
 * This function
 * @param   Qspi is structure pointer to qspi device
 * @return
 * @note    None
*/
AL_S32 AlQSPI_Dev_DmaRecvData(AL_QSPI_DevStruct *Qspi)
{
    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (QSPI_SR_RXFIFO_NOTEMPTY== AlQSPI_ll_IsRxFifoEmpty(Qspi->BaseAddr)) {
        return AL_QSPI_ERR_NOT_SUPPORT;
    }

    if (AlQSPI_Dev_IsRxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AL_U32 DefBurst = Qspi->Fifolen / 2;

    AlQSPI_ll_RxDmaEnable(Qspi->BaseAddr);

    AlQSPI_ll_SetDmaRecevLevel(Qspi->BaseAddr, MIN(DefBurst, 256) - 1);

    return AL_OK;
}

/**
 * This function
 * @param   Qspi is structure pointer to qspi device
 * @return
 * @note    None
*/
AL_S32 AlQSPI_Dev_DmaTranferData(AL_QSPI_DevStruct *Qspi)
{
    AL_U32 DefBurst = Qspi->Fifolen / 2;

    if (Qspi == AL_NULL) {
        return AL_QSPI_ERR_ILLEGAL_PARAM;
    }

    if ((Qspi->State & AL_QSPI_STATE_READY) == 0) {
        return AL_QSPI_ERR_NOT_READY;
    }

    if (AlQSPI_Dev_IsRxBusy(Qspi) || AlQSPI_Dev_IsTxBusy(Qspi)) {
        return AL_QSPI_ERR_BUSY;
    }

    AlQSPI_ll_TxDmaEnable(Qspi->BaseAddr);
    AlQSPI_ll_RxDmaEnable(Qspi->BaseAddr);

    AlQSPI_ll_SetDmaRecevLevel(Qspi->BaseAddr, MIN(DefBurst, 256) - 1);
    AlQSPI_ll_SetDmaTransLevel(Qspi->BaseAddr, MIN(DefBurst, 256));

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
AL_S32 AlQSPI_Dev_IoCtl(AL_QSPI_DevStruct *Qspi, AL_Qspi_IoCtlCmdEnum Cmd, AL_VOID *Data)
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
AL_S32 AlQSPI_Dev_RegisterIntrCallBack(AL_QSPI_DevStruct *Qspi, QSPI_EventCallBack Callback, AL_VOID *CallbackRef)
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
AL_S32 AlQSPI_Dev_UnRegisterIntrCallBack(AL_QSPI_DevStruct *Qspi)
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
static AL_VOID AlQSPI_Dev_RecvDataHandler(AL_QSPI_DevStruct *Qspi)
{
    AL_U32 Length, RxFifoLevel, Status, Temp;

    Length = Qspi->RecvBuffer.RequestedCnt - Qspi->RecvBuffer.HandledCnt;

    if(Length) {
        RxFifoLevel = AlQSPI_ll_ReadRxFifoLevel(Qspi->BaseAddr);
        if(!RxFifoLevel) {
            Status = AlQSPI_ll_ReadRawIntrStatus(Qspi->BaseAddr);
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
                *(AL_U8 *)(Qspi->RecvBuffer.BufferPtr + Qspi->RecvBuffer.HandledCnt) = (AL_U8)AlQSPI_ll_DataReceive(Qspi->BaseAddr);
            } else if (Qspi->BitsPerWord == 16) {
                *(AL_U16 *)(Qspi->RecvBuffer.BufferPtr + Qspi->RecvBuffer.HandledCnt) = (AL_U16)AlQSPI_ll_DataReceive(Qspi->BaseAddr);
            } else if (Qspi->BitsPerWord == 32) {
                *(AL_U32 *)(Qspi->RecvBuffer.BufferPtr + Qspi->RecvBuffer.HandledCnt) = AlQSPI_ll_DataReceive(Qspi->BaseAddr);
            }

            Qspi->RecvBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
            Length = Qspi->RecvBuffer.RequestedCnt - Qspi->RecvBuffer.HandledCnt;
        }
    }

    Temp =  MIN(Length / (Qspi->BitsPerWord >> 3), Qspi->Fifolen / 2);
    AlQSPI_ll_SetRxFifoThrLevel(Qspi->BaseAddr, Temp ? Temp - 1 : 0);

    if (Qspi->RecvBuffer.HandledCnt == Qspi->RecvBuffer.RequestedCnt) {
        AlQSPI_ll_MaskIntr(Qspi->BaseAddr, QSPI_RXFIM);

        if (Qspi->EventCallBack) {
            AL_QSPI_EventStruct QSPIEvent = {
                .Event        = AL_QSPI_RECEIVE_DONE,
                .EventData    = Qspi->SendBuffer.HandledCnt
            };
            AlQSPI_Dev_ClrRxBusy(Qspi);
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
static AL_VOID AlQSPI_Dev_SendDataHandler(AL_QSPI_DevStruct *Qspi)
{
    AL_U32 TxFifoLevel, Length, Room, SendValue = 0xffffffff;

    Length = Qspi->SendBuffer.RequestedCnt - Qspi->SendBuffer.HandledCnt;

    if (Length) {
        TxFifoLevel = AlQSPI_ll_ReadTxFifoLevel(Qspi->BaseAddr);

        if (!TxFifoLevel) {
            /* return error fifo entries */
            AL_LOG(AL_LOG_LEVEL_INFO, "Error Tx fifo enteries, cs high\r\n");
            Qspi->State |= AL_QSPI_STATE_ERROR;
        }

        Room = MIN((Qspi->Fifolen - TxFifoLevel), Length / (Qspi->BitsPerWord >> 3));
        for (; Room; --Room) {
            if (Qspi->BitsPerWord == 8) {
                SendValue = *(AL_U8 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
            } else if (Qspi->BitsPerWord == 16) {
                SendValue = *(AL_U16 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
                SendValue = QSPI_ENDIAN_SWAP16(SendValue);
            } else {
                SendValue = *(AL_U32 *)(Qspi->SendBuffer.BufferPtr + Qspi->SendBuffer.HandledCnt);
                SendValue = QSPI_ENDIAN_SWAP32(SendValue);
            }

            AlQSPI_ll_DataTransmit(Qspi->BaseAddr, SendValue);
            Qspi->SendBuffer.HandledCnt += Qspi->BitsPerWord >> 3;
            Length = Qspi->SendBuffer.RequestedCnt - Qspi->SendBuffer.HandledCnt;
        }
    }

    if (Qspi->SendBuffer.HandledCnt == Qspi->SendBuffer.RequestedCnt) {
        AlQSPI_ll_MaskIntr(Qspi->BaseAddr, QSPI_TXEIM);

        if (Qspi->EventCallBack) {
            AL_QSPI_EventStruct event = {
                .Event         = AL_QSPI_SEND_DONE,
                .EventData    = Qspi->SendBuffer.HandledCnt
            };
            AlQSPI_Dev_ClrTxBusy(Qspi);
            Qspi->EventCallBack(event, Qspi->EventCallBackRef);
        }
    }
}

#define QSPI_IN_TX_FE_INTR(Status)  (Status & TXEIS)
#define QSPI_IN_TX_FO_INTR(Status)  (Status & TXOIS)
#define QSPI_IN_RX_FU_INTR(Status)  (Status & RXUIS)
#define QSPI_IN_RX_FO_INTR(Status)  (Status & RXOIS)
#define QSPI_IN_RX_FF_INTR(Status)  (Status & RXFIS)

#define QSPI_IN_STATUS_ERROR(Status)  0

/**
 * This function is qspi intrrupt handler function
 * @param   instance is AL_VOID pointer converted to AL_QSPI_DevStruct pointer
 * @return  AL_VOID
 * @note    None
*/
AL_VOID AlQSPI_Dev_IntrHandler(AL_VOID *instance)
{
    AL_QSPI_DevStruct *Qspi = (AL_QSPI_DevStruct *) instance;
    AL_QSPI_IntrStatusEnum IntrStatus = AlQSPI_ll_GetIntrStatus(Qspi->BaseAddr);

    if (QSPI_IN_RX_FF_INTR(IntrStatus)) {
        AlQSPI_Dev_RecvDataHandler(Qspi);
    }

    if (QSPI_IN_TX_FE_INTR(IntrStatus)) {
        AlQSPI_Dev_SendDataHandler(Qspi);
    }

    if (QSPI_IN_TX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi tx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "TXFLR:0x%x\r\n", AlQSPI_ll_ReadTxFifoLevel(Qspi->BaseAddr));
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQSPI_ll_ClearAllIntr(Qspi->BaseAddr);
    }

    if (QSPI_IN_RX_FO_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi rx fifo over intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "RXFLR:0x%x\r\n", AlQSPI_ll_ReadRxFifoLevel(Qspi->BaseAddr));
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQSPI_ll_ClearAllIntr(Qspi->BaseAddr);
    }

    if (QSPI_IN_RX_FU_INTR(IntrStatus)) {
        AL_LOG(AL_LOG_LEVEL_ERROR, "IntrStatus:0x%x\r\n", IntrStatus);
        AL_LOG(AL_LOG_LEVEL_ERROR, "Error qspi rx fifo underflow intr\r\n");
        AL_LOG(AL_LOG_LEVEL_ERROR, "RXFLR:0x%x\r\n", AlQSPI_ll_ReadRxFifoLevel(Qspi->BaseAddr));
        Qspi->State |= AL_QSPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlQSPI_ll_ClearAllIntr(Qspi->BaseAddr);
    }
}


