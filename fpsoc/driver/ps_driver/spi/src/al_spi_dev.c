
/***************************** Include Files *********************************/
#include "al_spi_dev.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/
static AL_SPI_ConfigsStruct SpiDefInitConfigs =
{
    .Mode               = SPI_MASTER_MODE,
    .TransMode          = SPI_TX_ONLY,
    .ProtFormat         = MOTOROLA_SPI,
    .ClockEnum          = SPI_CLK_MODE0,
    .DataFrameSize      = SPI_FRAME_08BITS,
    .ClkDiv             = 70,
    .SlvToggleEnum      = SPI_SLV_TOGGLE_DISABLE,
    .SlvSelEnum         = SPI_SER_SS0_EN,
};

extern AL_SPI_HwConfigStruct AlSpi_HwCfg[AL_SPI_NUM_INSTANCE];
/************************** Function Prototypes ******************************/

/********************************************************/
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

AL_BOOL AlSpi_Dev_IsTxBusy(AL_SPI_DevStruct *Spi)
{
    return (AL_BOOL)(Spi->State & AL_SPI_STATE_TX_BUSY);
}

AL_BOOL AlSpi_Dev_IsRxBusy(AL_SPI_DevStruct *Spi)
{
    return (AL_BOOL)(Spi->State & AL_SPI_STATE_RX_BUSY);
}

static AL_VOID AlSpi_Dev_SetTxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State |= AL_SPI_STATE_TX_BUSY;
}

static AL_VOID AlSpi_Dev_SetRxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State |= AL_SPI_STATE_RX_BUSY;
}

static AL_VOID AlSpi_Dev_ClrTxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State &= (~AL_SPI_STATE_TX_BUSY);
}

static AL_VOID AlSpi_Dev_ClrRxBusy(AL_SPI_DevStruct *Spi)
{
    Spi->State &= (~AL_SPI_STATE_RX_BUSY);
}

// #ifdef SPI_DEBUG || SPI_TX_DEBUG
AL_VOID AlSpi_Dev_DumpReg(AL_REG SpiBaseAddr)
{
    printf("CTRLR0:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_CTRLR0_MST_OFFSET));
    printf("CTRLR1:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_CTRLR1_MST_OFFSET));
    printf("SSIENR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_SSIENR_MST_OFFSET));
    printf("MWCR:0x%x\r\n",    AL_REG32_READ(SpiBaseAddr + SPI_MWCR_MST_OFFSET));
    printf("SER:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_SER_MST_OFFSET));
    printf("BAUDR:0x%x\r\n",   AL_REG32_READ(SpiBaseAddr + SPI_BAUDR_MST_OFFSET));
    printf("TXFTLR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_TXFTLR_MST_OFFSET));
    printf("RXFTLR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_RXFTLR_MST_OFFSET));
    printf("TXFLR:0x%x\r\n",   AL_REG32_READ(SpiBaseAddr + SPI_TXFLR_MST_OFFSET));
    printf("RXFLR:0x%x\r\n",   AL_REG32_READ(SpiBaseAddr + SPI_RXFLR_MST_OFFSET));
    printf("SR:0x%x\r\n",      AL_REG32_READ(SpiBaseAddr + SPI_SR_MST_OFFSET));
    printf("IMR:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_IMR_MST_OFFSET));
    printf("ISR:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_ISR_MST_OFFSET));
    printf("RISR:0x%x\r\n",    AL_REG32_READ(SpiBaseAddr + SPI_RISR_MST_OFFSET));
    printf("TXOICR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_TXOICR_MST_OFFSET));
    printf("RXOICR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_RXOICR_MST_OFFSET));
    printf("RXUICR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_RXUICR_MST_OFFSET));
    printf("MSTICR:0x%x\r\n",  AL_REG32_READ(SpiBaseAddr + SPI_MSTICR_MST_OFFSET));
    printf("ICR:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_ICR_MST_OFFSET));
    printf("DMACR:0x%x\r\n",   AL_REG32_READ(SpiBaseAddr + SPI_DMACR_MST_OFFSET));
    printf("DMATDLR:0x%x\r\n", AL_REG32_READ(SpiBaseAddr + SPI_DMATDLR_MST_OFFSET));
    printf("DMARDLR:0x%x\r\n", AL_REG32_READ(SpiBaseAddr + SPI_DMARDLR_MST_OFFSET));
    printf("IDR:0x%x\r\n",     AL_REG32_READ(SpiBaseAddr + SPI_IDR_MST_OFFSET));
    printf("SSI_VERSION_ID:0x%x\r\n", AL_REG32_READ(SpiBaseAddr + SPI_SSI_VERSION_ID_MST_OFFSET));
}
// #endif

AL_S32 AlSpi_Dev_Init(AL_SPI_DevStruct *Spi, AL_SPI_ConfigsStruct *InitConfig)
{
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    Spi->Configs = (InitConfig == AL_NULL) ? SpiDefInitConfigs : (*InitConfig);
  
    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetSpiMode(Spi->BaseAddr, Spi->Configs.Mode);
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

    if (0 == (SendSize % 4)) {
        Spi->Configs.DataFrameSize = SPI_FRAME_32BITS;
        Spi->BitsPerWord = 32;
    } else if (0 == (SendSize % 2)) {
        Spi->Configs.DataFrameSize = SPI_FRAME_16BITS;
        Spi->BitsPerWord = 16;
    } else {
        Spi->Configs.DataFrameSize = SPI_FRAME_08BITS;
        Spi->BitsPerWord = 8;
    }

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, Spi->Configs.DataFrameSize);

    if (Spi->BitsPerWord != 8) {
        AlSpi_ll_SetEndianConver(Spi->BaseAddr);
    } else {
        AlSpi_ll_DisableEndianConver(Spi->BaseAddr);
    }

    AlSpi_ll_Enable(Spi->BaseAddr);

    SendLevel = MIN(Spi->Fifolen - AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr), Spi->SendBuffer.RequestedCnt / (Spi->BitsPerWord >> 3));

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

    SendLevel = MIN((Spi->Fifolen / 2), SendSize / (Spi->BitsPerWord >> 3));
    AlSpi_ll_SetTxFifoThrLevel(Spi->BaseAddr, SendLevel);
    
    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_TXEIM);
    // AlSpi_Dev_DumpReg(Spi->BaseAddr);
    return AL_OK;
}

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
        Spi->Configs.DataFrameSize = SPI_FRAME_08BITS;
        Spi->BitsPerWord = 8;
    }

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    Temp = ReceiveSize / (Spi->BitsPerWord >> 3);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->BaseAddr, Temp ? Temp - 1 : 0);
    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, Spi->Configs.DataFrameSize);

    if (Spi->BitsPerWord != 8) {
        AlSpi_ll_SetEndianConver(Spi->BaseAddr);
    } else {
        AlSpi_ll_DisableEndianConver(Spi->BaseAddr);
    }

    AlSpi_ll_Enable(Spi->BaseAddr);
    Temp = MIN(ReceiveSize / (Spi->BitsPerWord >> 3), Spi->Fifolen / 2);
    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, Temp ? Temp - 1 : 0);
    //  AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, 0);
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_RXFIM);
    
    return AL_OK;
}

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
        Spi->Configs.DataFrameSize = SPI_FRAME_08BITS;
        Spi->BitsPerWord = 8;
    }

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
   
    Temp = ReceiveSize / (Spi->BitsPerWord >> 3);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->BaseAddr, Temp ? Temp - 1 : 0);             
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, Spi->Configs.DataFrameSize);
    

   
AlSpi_ll_Enable(Spi->BaseAddr);


// printf("RecvNumOfDataFrames:%d\r\n", (ReceiveSize / (Spi->BitsPerWord >> 3)) ? ((ReceiveSize / (Spi->BitsPerWord >> 3)) - 1) : 0);
    SendLevel = MIN3(Spi->Fifolen - AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr), Spi->SendBuffer.RequestedCnt / (Spi->BitsPerWord >> 3),
                     Spi->Fifolen - (Spi->RecvBuffer.RequestedCnt - Spi->SendBuffer.RequestedCnt)/ (Spi->BitsPerWord >> 3));
    
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

      SendLevel = MIN((Spi->Fifolen / 2), SendSize / (Spi->BitsPerWord >> 3));
 // AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, 0);
    Temp =  MIN(ReceiveSize / (Spi->BitsPerWord >> 3), Spi->Fifolen / 2);
    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, Temp ? Temp - 1 : 0);
    AlSpi_ll_SetTxFifoThrLevel(Spi->BaseAddr, SendLevel);

    /* Set cs to start transfer */
    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_RXFIM | SPI_TXEIM);
    
    // printf("IMR III:0x%x\r\n",     AL_REG32_READ(Spi->BaseAddr + SPI_IMR_MST_OFFSET));
    return AL_OK;
}




AL_S32 AlSpi_Dev_DmaSendData(AL_SPI_DevStruct *Spi)
{
    AL_U32 DefBurst = Spi->Fifolen / 2;

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

    AlSpi_Dev_SetTxBusy(Spi);

    AlSpi_ll_TxDmaEnable(Spi->BaseAddr);

    AlSpi_ll_SetDmaTransLevel(Spi->BaseAddr, MIN(DefBurst, 256));

    return AL_OK;
}

AL_S32 AlSpi_Dev_DmaRecvData(AL_SPI_DevStruct *Spi)
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

    AL_U32 DefBurst = Spi->Fifolen / 2;

    AlSpi_ll_RxDmaEnable(Spi->BaseAddr);

    AlSpi_ll_SetDmaRecevLevel(Spi->BaseAddr, MIN(DefBurst, 256) - 1);
    
    return AL_OK;
}

AL_S32 AlSpi_Dev_DmaTranferData(AL_SPI_DevStruct *Spi)
{
    AL_U32 DefBurst = Spi->Fifolen / 2;

    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_SPI_ERR_NOT_READY;
    }

    if (AlSpi_Dev_IsRxBusy(Spi) || AlSpi_Dev_IsTxBusy(Spi)) {
        return AL_SPI_ERR_BUSY;
    }

    AlSpi_ll_TxDmaEnable(Spi->BaseAddr);
    AlSpi_ll_RxDmaEnable(Spi->BaseAddr);

    AlSpi_ll_SetDmaRecevLevel(Spi->BaseAddr, MIN(DefBurst, 256) - 1);
    AlSpi_ll_SetDmaTransLevel(Spi->BaseAddr, MIN(DefBurst, 256));
    
    return AL_OK;
}








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

AL_S32 AlSpi_Dev_UnRegisterIntrCallBack(AL_SPI_DevStruct *Spi)
{
    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    Spi->EventCallBack = (SPI_EventCallBack)AL_NULL;

    return AL_OK;
}

static AL_VOID AlSpi_Dev_RecvDataHandler(AL_SPI_DevStruct *Spi)
{ 
    AL_U32 Length, RxFifoLevel, Status, Temp;

    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }

    Length = Spi->RecvBuffer.RequestedCnt - Spi->RecvBuffer.HandledCnt;

    if(Length)
    {
        RxFifoLevel = AlSpi_ll_ReadRxFifoLevel(Spi->BaseAddr);
        if(!RxFifoLevel)
        {
            Status = AlSpi_ll_ReadRawIntrStatus(Spi->BaseAddr);
            if (Status & RXOIS) {
                /* FIFO overflow on Rx */
                printf("Error FIFO overflow on Rx\r\n");
                Spi->State |= AL_SPI_STATE_ERROR;
            }
        } else if ((Spi->Fifolen == RxFifoLevel) && (Spi->Configs.TransMode == SPI_EEPROM)) {
            printf("Error FIFO Full on Rx, cs high\r\n");
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
            // printf("Spi->RecvBuffer.HandledCnt += Spi->BitsPerWord >> 3\r\n");
        }
    }

    //AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, MIN(Length / (Spi->BitsPerWord >> 3), Spi->Fifolen / 2) - 1);
    Temp =  MIN(Length / (Spi->BitsPerWord >> 3), Spi->Fifolen / 2);
    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, Temp ? Temp - 1 : 0);

    // printf("IMR2:0x%x\r\n",     AL_REG32_READ(Spi->BaseAddr + SPI_IMR_MST_OFFSET));
    // printf("Length:%d\r\n", Length);
    // printf("Spi->BitsPerWord:%d\r\n", Spi->BitsPerWord);
    // printf("HandledCnt:%d\r\n", Spi->RecvBuffer.HandledCnt);
    // printf("RequestedCnt:%d\r\n", Spi->RecvBuffer.RequestedCnt);
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

static AL_VOID AlSpi_Dev_SendDataHandler(AL_SPI_DevStruct *Spi)
{
	AL_U32 TxFifoLevel, Length, Room, SendValue = 0xffffffff;

    if (Spi == AL_NULL) {
        return AL_SPI_ERR_ILLEGAL_PARAM;
    }
    Length = Spi->SendBuffer.RequestedCnt - Spi->SendBuffer.HandledCnt;

    if (Length) {
	    TxFifoLevel = AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr);

        if (!TxFifoLevel) {
            /* return error fifo entries */
            printf("Error Tx fifo enteries, cs high\r\n");
            Spi->State |= AL_SPI_STATE_ERROR;
        }
        
        Room = MIN((Spi->Fifolen - TxFifoLevel), Length / (Spi->BitsPerWord >> 3));
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

// printf("IMR2:0x%x\r\n",     AL_REG32_READ(Spi->BaseAddr + SPI_IMR_MST_OFFSET));
//     printf("Length:%d\r\n", Length);
//     printf("Spi->BitsPerWord:%d\r\n", Spi->BitsPerWord);
//     printf("HandledCnt:%d\r\n", Spi->RecvBuffer.HandledCnt);
//     printf("RequestedCnt:%d\r\n", Spi->RecvBuffer.RequestedCnt);
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


AL_VOID AlSpi_Dev_IntrHandler(AL_VOID *instance)
{
	AL_SPI_DevStruct *Spi = (AL_SPI_DevStruct *) instance;
	AL_SPI_IntrStatusEnum IntrStatus = AlSpi_ll_GetIntrStatus(Spi->BaseAddr);
// #ifdef SPI_DEBUG
    // printf("IntrStatus:0x%x\r\n", IntrStatus);
// #endif
    if (SPI_IN_RX_FF_INTR(IntrStatus)) {
		AlSpi_Dev_RecvDataHandler(Spi);
    }

    if (SPI_IN_TX_FE_INTR(IntrStatus)) {
        AlSpi_Dev_SendDataHandler(Spi);
    }

    if (SPI_IN_TX_FO_INTR(IntrStatus)) {
// #ifdef SPI_DEBUG
        // AL_LOG(AL_ERR_LEVEL_ERROR, "spi tx fifo over,STATUS_ERROR intr 0x%x\r\n", IntrStatus);
        printf("IntrStatus:0x%x\r\n", IntrStatus);
        printf("Error spi tx fifo over intr\r\n");
        printf("TXFLR:0x%x\r\n", AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr));
// #endif
        Spi->State |= AL_SPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlSpi_ll_ClearAllIntr(Spi->BaseAddr);
    }

    if (SPI_IN_RX_FO_INTR(IntrStatus)) {
// #ifdef SPI_DEBUG
        // AL_LOG(AL_ERR_LEVEL_ERROR, "spi rx fifo over, STATUS_ERROR intr 0x%x\r\n", IntrStatus);
        printf("IntrStatus:0x%x\r\n", IntrStatus);
        printf("Error spi rx fifo over intr\r\n");
        printf("RXFLR:0x%x\r\n", AlSpi_ll_ReadRxFifoLevel(Spi->BaseAddr));
// #endif
        Spi->State |= AL_SPI_STATE_ERROR;
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlSpi_ll_ClearAllIntr(Spi->BaseAddr);
    }

    if (SPI_IN_RX_FU_INTR(IntrStatus)) {
// #ifdef SPI_DEBUG
        // AL_LOG(AL_ERR_LEVEL_WARNING, "spi rx fifo underflow, STATUS_ERROR intr 0x%x", IntrStatus);
        printf("IntrStatus:0x%x\r\n", IntrStatus);
        printf("Error spi rx fifo underflow intr\r\n");
        printf("RXFLR:0x%x\r\n", AlSpi_ll_ReadRxFifoLevel(Spi->BaseAddr));
// #endif
        Spi->State |= AL_SPI_STATE_ERROR; 
        /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
        AlSpi_ll_ClearAllIntr(Spi->BaseAddr);
    }
    
}


