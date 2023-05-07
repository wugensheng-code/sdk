
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

#ifdef SPI_DEBUG
AL_VOID AlSpi_Dev_DumpReg(AL_SPI_DevStruct *Spi)
{
    printf("CTRLR0:0x%x ",  READ_REG(Spi->BaseAddr + SPI_CTRLR0_MST_OFFSET));
    printf("CTRLR1:0x%x ",  READ_REG(Spi->BaseAddr + SPI_CTRLR1_MST_OFFSET));
    printf("SSIENR:0x%x ",  READ_REG(Spi->BaseAddr + SPI_SSIENR_MST_OFFSET));
    printf("MWCR:0x%x ",    READ_REG(Spi->BaseAddr + SPI_MWCR_MST_OFFSET));
    printf("SER:0x%x ",     READ_REG(Spi->BaseAddr + SPI_SER_MST_OFFSET));
    printf("BAUDR:0x%x ",   READ_REG(Spi->BaseAddr + SPI_BAUDR_MST_OFFSET));
    printf("TXFTLR:0x%x ",  READ_REG(Spi->BaseAddr + SPI_TXFTLR_MST_OFFSET));
    printf("RXFTLR:0x%x ",  READ_REG(Spi->BaseAddr + SPI_RXFTLR_MST_OFFSET));
    printf("TXFLR:0x%x ",   READ_REG(Spi->BaseAddr + SPI_TXFLR_MST_OFFSET));
    printf("RXFLR:0x%x ",   READ_REG(Spi->BaseAddr + SPI_RXFLR_MST_OFFSET));
    printf("SR:0x%x ",      READ_REG(Spi->BaseAddr + SPI_SR_MST_OFFSET));
    printf("IMR:0x%x ",     READ_REG(Spi->BaseAddr + SPI_IMR_MST_OFFSET));
    printf("ISR:0x%x ",     READ_REG(Spi->BaseAddr + SPI_ISR_MST_OFFSET));
    printf("RISR:0x%x ",    READ_REG(Spi->BaseAddr + SPI_RISR_MST_OFFSET));
    printf("TXOICR:0x%x ",  READ_REG(Spi->BaseAddr + SPI_TXOICR_MST_OFFSET));
    printf("RXOICR:0x%x ",  READ_REG(Spi->BaseAddr + SPI_RXOICR_MST_OFFSET));
    printf("RXUICR:0x%x ",  READ_REG(Spi->BaseAddr + SPI_RXUICR_MST_OFFSET));
    printf("MSTICR:0x%x ",  READ_REG(Spi->BaseAddr + SPI_MSTICR_MST_OFFSET));
    printf("ICR:0x%x ",     READ_REG(Spi->BaseAddr + SPI_ICR_MST_OFFSET));
    printf("DMACR:0x%x ",   READ_REG(Spi->BaseAddr + SPI_DMACR_MST_OFFSET));
    printf("DMATDLR:0x%x ", READ_REG(Spi->BaseAddr + SPI_DMATDLR_MST_OFFSET));
    printf("DMARDLR:0x%x ", READ_REG(Spi->BaseAddr + SPI_DMARDLR_MST_OFFSET));
    printf("IDR:0x%x ",     READ_REG(Spi->BaseAddr + SPI_IDR_MST_OFFSET));
    printf("SSI_VERSION_ID:0x%x ", READ_REG(Spi->BaseAddr + SPI_SSI_VERSION_ID_MST_OFFSET));
}
#endif

AL_S32 AlSpi_Dev_Init(AL_SPI_DevStruct *Spi, AL_SPI_ConfigsStruct *InitConfig)
{
    if (Spi == AL_NULL) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    Spi->Configs = (InitConfig == AL_NULL) ? SpiDefInitConfigs : (*InitConfig);
  
    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetSpiMode(Spi->BaseAddr, Spi->Configs.Mode);
    AlSpi_ll_SetProtFormat(Spi->BaseAddr, Spi->Configs.ProtFormat);
    AlSpi_ll_SetCpolAndCpha(Spi->BaseAddr, Spi->Configs.ClockEnum);
    AlSpi_ll_SetDataFrameSize(Spi->BaseAddr, Spi->Configs.DataFrameSize);
    AlSpi_ll_SetClkDiv(Spi->BaseAddr, Spi->Configs.ClkDiv);
    AlSpi_ll_SetSlvSelToggle(Spi->BaseAddr, Spi->Configs.SlvToggleEnum);
    AlSpi_ll_MaskIntr(Spi->BaseAddr, SPI_MASK_TXEIM);
    AlSpi_ll_MaskIntr(Spi->BaseAddr, SPI_MASK_RXFIM);
    AlSpi_ll_Enable(Spi->BaseAddr);

    Spi->State |= AL_SPI_STATE_READY;

    return AL_OK;
}

AL_S32 AlSpi_Dev_SendData(AL_SPI_DevStruct *Spi, AL_U8 *SendBuf, AL_U32 SendSize)
{
    AL_U16 SendLevel;

    if (SendBuf == AL_NULL || SendSize == 0) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_READY);
    }

    if (SPI_SR_TXFIFO_FULL == AlSpi_ll_IsTxFifoFull(Spi->BaseAddr)) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_SUPPORT);
    }

    AlSpi_Dev_SetTxBusy(Spi);

    Spi->SendBuffer.BufferPtr    = SendBuf;
    Spi->SendBuffer.RequestedCnt = SendSize;
    Spi->SendBuffer.HandledCnt   = 0;

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    AlSpi_ll_Enable(Spi->BaseAddr);

    SendLevel = (Spi->Fifolen < Spi->SendBuffer.RequestedCnt) ? Spi->Fifolen : Spi->SendBuffer.RequestedCnt;

	while (SendLevel--) {
		AlSpi_ll_DataTransmit(Spi->BaseAddr, Spi->SendBuffer.BufferPtr[Spi->SendBuffer.HandledCnt]);
		Spi->SendBuffer.HandledCnt++;
    }

    SendLevel = ((Spi->Fifolen / 2) < SendSize) ? (Spi->Fifolen / 2) : SendSize;
    AlSpi_ll_SetTxFifoThrLevel(Spi->BaseAddr, SendLevel);
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_MASK_TXEIM);

    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);

    return AL_OK;
}

AL_S32 AlSpi_Dev_RecvData(AL_SPI_DevStruct *Spi, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    if (Spi == AL_NULL || ReceiveBuf == AL_NULL || ReceiveSize == 0 ) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_READY);
    }

    if (SPI_SR_RXFIFO_NOTEMPTY== AlSpi_ll_IsRxFifoEmpty(Spi->BaseAddr)) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_SUPPORT);
    }

    AlSpi_Dev_SetRxBusy(Spi);

    Spi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Spi->RecvBuffer.RequestedCnt = ReceiveSize;
    Spi->RecvBuffer.HandledCnt   = 0;

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->BaseAddr, ReceiveSize - 1);
    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);
    AlSpi_ll_Enable(Spi->BaseAddr);

    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, 0);
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_MASK_RXFIM);
    
    return AL_OK;
}

AL_S32 AlSpi_Dev_SendRecvData(AL_SPI_DevStruct *Spi, AL_U8 *SendBuf, AL_U32 SendSize, AL_U8 *ReceiveBuf, AL_U16 ReceiveSize)
{
    AL_U32 ReceiveLevel, SendLevel;

    if (Spi == AL_NULL || SendBuf == AL_NULL || ReceiveBuf == AL_NULL || SendSize == 0 || ReceiveSize == 0) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    if ((Spi->State & AL_SPI_STATE_READY) == 0) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_READY);
    }

    AlSpi_Dev_SetRxBusy(Spi);
    AlSpi_Dev_SetTxBusy(Spi);
   
    Spi->SendBuffer.BufferPtr    = SendBuf;
    Spi->SendBuffer.RequestedCnt = SendSize;
    Spi->SendBuffer.HandledCnt   = 0;
    Spi->RecvBuffer.BufferPtr    = ReceiveBuf;
    Spi->RecvBuffer.RequestedCnt = ReceiveSize;
    Spi->RecvBuffer.HandledCnt   = 0;

    AlSpi_ll_Disable(Spi->BaseAddr);
    AlSpi_ll_SetTransfMode(Spi->BaseAddr, Spi->Configs.TransMode);
    AlSpi_ll_SetRecvNumOfDataFrames(Spi->BaseAddr, ReceiveSize ? ReceiveSize - 1 : 0);
    AlSpi_ll_Enable(Spi->BaseAddr);

    SendLevel = (Spi->Fifolen < Spi->SendBuffer.RequestedCnt) ? Spi->Fifolen : Spi->SendBuffer.RequestedCnt;

	while (SendLevel--) {
		AlSpi_ll_DataTransmit(Spi->BaseAddr, Spi->SendBuffer.BufferPtr[Spi->SendBuffer.HandledCnt]);
		Spi->SendBuffer.HandledCnt++;
    }
    
    SendLevel = ((Spi->Fifolen / 2) < SendSize) ? (Spi->Fifolen / 2) : SendSize;

    AlSpi_ll_SetRxFifoThrLevel(Spi->BaseAddr, 0);
    AlSpi_ll_SetTxFifoThrLevel(Spi->BaseAddr, SendLevel);
    
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_MASK_TXEIM);
    AlSpi_ll_EnableIntr(Spi->BaseAddr, SPI_MASK_RXFIM);

    /* Set cs to start transfer */
    AlSpi_ll_SetSlvSel(Spi->BaseAddr, Spi->Configs.SlvSelEnum);
    
    return AL_OK;
}

AL_S32 AlSpi_Dev_RegisterIntrCallBack(AL_SPI_DevStruct *Spi, SPI_EventCallBack Callback, AL_VOID *CallbackRef)
{
    if (Spi == AL_NULL || Callback == AL_NULL) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    if (Spi->EventCallBack != AL_NULL) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_SUPPORT);
    }

    Spi->EventCallBack        = Callback;
    Spi->EventCallBackRef     = CallbackRef;

    return AL_OK;
}

AL_S32 AlSpi_Dev_UnRegisterIntrCallBack(AL_SPI_DevStruct *Spi)
{
    if (Spi == AL_NULL) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    Spi->EventCallBack = (SPI_EventCallBack)AL_NULL;

    return AL_OK;
}

static AL_VOID AlSpi_Dev_RecvDataHandler(AL_SPI_DevStruct *Spi)
{ 
    AL_U32 Length, RxFifoLevel, Status;

    if (Spi == AL_NULL) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
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
#ifdef SPI_DEBUG
                printf("Error FIFO overflow on Rx\r\n");
#endif
                Spi->State |= AL_SPI_STATE_ERROR;
            }
        } else if ((Spi->Fifolen == RxFifoLevel) && (Spi->Configs.TransMode == SPI_EEPROM)) {
#ifdef SPI_DEBUG
            printf("error FIFO Full on Rx, cs high\r\n");
#endif
            Spi->State |= AL_SPI_STATE_ERROR;
        }

        RxFifoLevel = (RxFifoLevel < Length) ? RxFifoLevel : Length;
        for (; RxFifoLevel; --RxFifoLevel, --Length) {
            Spi->RecvBuffer.BufferPtr[Spi->RecvBuffer.HandledCnt] = (AL_U8)AlSpi_ll_DataReceive(Spi->BaseAddr);
            Spi->RecvBuffer.HandledCnt++;
        }
    }

    if (Spi->RecvBuffer.HandledCnt == Spi->RecvBuffer.RequestedCnt) {
        AlSpi_ll_MaskIntr(Spi->BaseAddr, SPI_MASK_RXFIM);
       
        if (Spi->EventCallBack) {
            AL_SPI_EventStruct SpiEvent = {
                .Event        = AL_SPI_RECEIVE_DONE,
                .EventData    = Spi->SendBuffer.HandledCnt
            };

            AlSpi_Dev_ClrRxBusy(Spi);
            Spi->EventCallBack(SpiEvent, Spi->EventCallBackRef);
        }
    } 

    return;
}

static AL_VOID AlSpi_Dev_SendDataHandler(AL_SPI_DevStruct *Spi)
{
	AL_U32 Length, TxFifoLevel, Room;

    if (Spi == AL_NULL) {
        return AL_DEF_ERR(AL_SPI, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM);
    }

    Length = Spi->SendBuffer.RequestedCnt - Spi->SendBuffer.HandledCnt;

    if (Length) {
	    TxFifoLevel = AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr);

        if (!TxFifoLevel) {
            /* return error fifo entries */
#ifdef SPI_DEBUG
            printf("Error Tx fifo enteries, cs high\r\n");
#endif
            Spi->State |= AL_SPI_STATE_ERROR;
        }

        Room = ((Spi->Fifolen - TxFifoLevel) < Length) ? (Spi->Fifolen - TxFifoLevel) : Length;
        for (; Room; --Room, --Length) {
            AlSpi_ll_DataTransmit(Spi->BaseAddr, Spi->SendBuffer.BufferPtr[Spi->SendBuffer.HandledCnt]);
            Spi->SendBuffer.HandledCnt++;
        }
    }

    if (Spi->SendBuffer.HandledCnt == Spi->SendBuffer.RequestedCnt) {
        AlSpi_ll_MaskIntr(Spi->BaseAddr, SPI_MASK_TXEIM);

        if (Spi->EventCallBack) {
            AL_SPI_EventStruct event = {
                .Event         = AL_SPI_SEND_DONE,
                .EventData    = Spi->SendBuffer.HandledCnt
            };
            AlSpi_Dev_ClrTxBusy(Spi);
            Spi->EventCallBack(event, Spi->EventCallBackRef);
        }
    } 
    return;
}

#define SPI_IN_TX_FE_INTR(Status)  (Status == TXEIS)
#define SPI_IN_TX_FO_INTR(Status)  (Status == TXOIS)
#define SPI_IN_RX_FU_INTR(Status)  (Status == RXUIS)
#define SPI_IN_RX_FO_INTR(Status)  (Status == RXOIS)
#define SPI_IN_RX_FF_INTR(Status)  (Status == RXFIS)

#define SPI_IN_STATUS_ERROR(Status)  0


AL_VOID AlSpi_Dev_IntrHandler(AL_VOID *instance)
{
	AL_SPI_DevStruct *Spi = (AL_SPI_DevStruct *) instance;
	AL_SPI_InterruptEnum IntrStatus = AlSpi_ll_GetIntrStatus(Spi->BaseAddr);

    if (SPI_IN_TX_FE_INTR(IntrStatus)) {
        AlSpi_Dev_SendDataHandler(Spi);
    }

	if (SPI_IN_RX_FF_INTR(IntrStatus)) {
		AlSpi_Dev_RecvDataHandler(Spi);
    }

    if (SPI_IN_TX_FO_INTR(IntrStatus)) {
#ifdef SPI_DEBUG
        // AL_LOG(AL_ERR_LEVEL_ERROR, "spi tx fifo over,STATUS_ERROR intr 0x%x\r\n", IntrStatus);
        printf("Error spi tx fifo over intr\r\n");
        printf("TXFLR:0x%x\r\n", AlSpi_ll_ReadTxFifoLevel(Spi->BaseAddr));
#endif
        Spi->State |= AL_SPI_STATE_ERROR;
    }

    if (SPI_IN_RX_FO_INTR(IntrStatus)) {
#ifdef SPI_DEBUG
        // AL_LOG(AL_ERR_LEVEL_ERROR, "spi rx fifo over, STATUS_ERROR intr 0x%x\r\n", IntrStatus);
        printf("Error spi rx fifo over intr\r\n");
        printf("RXFLR:0x%x\r\n", AlSpi_ll_ReadRxFifoLevel(Spi->BaseAddr));
#endif
        Spi->State |= AL_SPI_STATE_ERROR;
    }

    if (SPI_IN_RX_FU_INTR(IntrStatus)) {
#ifdef SPI_DEBUG
        // AL_LOG(AL_ERR_LEVEL_WARNING, "spi rx fifo underflow, STATUS_ERROR intr 0x%x", IntrStatus);
        printf("Error spi rx fifo underflow intr\r\n");
        printf("RXFLR:0x%x\r\n", AlSpi_ll_ReadRxFifoLevel(Spi->BaseAddr));
#endif
        Spi->State |= AL_SPI_STATE_ERROR;
    }

    /* A read clears the txo_intr, rxu_intr, rxo_intr interrupts. */
    AlSpi_ll_ClearAllIntr(Spi->BaseAddr);
    return;
}


