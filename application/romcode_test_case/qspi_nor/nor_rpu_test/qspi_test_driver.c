#include "qspi_test_driver.h"
#include "mtimer.h"


static QspiFlash_Msg FlashMsg;

extern MtimerParams* QspiTimer1;  
extern MtimerParams* QspiTimer2;
extern QSPI_CORE_HANDLE    *g_pdev;

void Qspi_PinmuxInit(void)
{
     *(volatile uint32_t *)(0xf8803000u) =0x1;	  //QSPI  MIO0
     *(volatile uint32_t *)(0xf8803004u) =0x1;
     *(volatile uint32_t *)(0xf8803008u) =0x1;
     *(volatile uint32_t *)(0xf880300cu) =0x1;
     *(volatile uint32_t *)(0xf8803010u) =0x1;
     *(volatile uint32_t *)(0xf8803014u) =0x1;
     *(volatile uint32_t *)(0xf8803018u) =0x1;   
}

uint8_t Qspi_ChipErase(void)
{
    uint8_t Status = AL_SUCCESS, ReadData = 0;
    Qspi_Disable(g_pdev);
    Config_QspiTx(g_pdev, 2);
    Qspi_Enable(g_pdev);

    Qspi_DataTransmit(g_pdev, WRITE_ENABLE);
    Qspi_DataTransmit(g_pdev, CHIP_ERASE);

    Status = Wait_TxCompl(g_pdev);

    
    do{
        printf("ChipErase going\r\n");

        Qspi_Disable(g_pdev);
        Config_QspiTx(g_pdev, 1);
        Config_QspiRx(g_pdev,1);
        Qspi_Enable(g_pdev);

        Qspi_DataTransmit(g_pdev, READ_STATUS_REG1);
        Status = Wait_TxCompl(g_pdev);

        Mtimer_Start(QspiTimer2, 1000);

        while(!Qspi_RxNotempty(g_pdev))  // RXFIFO not empty
        {
            if (Mtimer_IsTimerOut(QspiTimer2) == 1)
            {
                return DATA_XFER_ERR1;
            }
        }

        ReadData = Qspi_DataRead(g_pdev) & 0xFF;
		
        Mtimer_Delay(1000000);

    }while(ReadData&0x01);
    
    return Status;
}




u32 Test_QspiTransfer(QSPI_CORE_HANDLE *pdev, QspiFlash_Msg *Msg)
{

    u32 Index;
    u32 Tdata=0;
    u32 Rdata=0;
	u32 Status=AL_SUCCESS;

    /*
     * Set the busy flag, which will be cleared when the transfer is
     * entirely done.
     */
     
    Index = 0;
    if (Msg->BusWidth == QSPI_WIDTH_X1) {
        //Mtimer_Start(30);
        //while ((Msg->txByteCount > 0) && (Mtimer_TimerOut() ==  0) ) {
		MTIMER_OUT_CONDITION(30,QspiTimer2,Msg->txByteCount > 0) {
            Qspi_DataTransmit(pdev,Msg->TxBfrPtr[Index]);
            Index++;
            Msg->txByteCount--;
        }
		if (Mtimer_IsTimerOut(QspiTimer2) == 1)
		{
			return DATA_XFER_ERR0;
		}    

        Status=Wait_TxCompl(pdev);
        Index = 0;

        Qspi_Disable(g_pdev);
        Config_QspiTx(g_pdev, Msg->rxByteCount);
        Qspi_Enable(g_pdev);

        MTIMER_OUT_CONDITION(30000,QspiTimer2,Msg->rxByteCount > 0) {
            Qspi_DataTransmit(pdev,Msg->TxBfrPtr[Index]);
            Index++;
            Msg->rxByteCount--;
        }
		if (Mtimer_IsTimerOut(QspiTimer2) == 1)
		{
			return DATA_XFER_ERR0;
		}    

        Status=Wait_TxCompl(pdev);
        Index = 0;
		
    }
    else if (Msg->BusWidth == QSPI_WIDTH_X2 || Msg->BusWidth == QSPI_WIDTH_X4) {
        printf("QSPI_WIDTH_X2 || QSPI_WIDTH_X4\r\n");
    }

    return Status;
}


uint32_t Qspi_Program(u32 SrcAddress, u8* DestAddress, u32 Length)
{

    s32 Status=0;
    u32 DiscardByteCnt;
    
    u8 WriteBuffer[10] __attribute__ ((aligned(32)));


    if((SrcAddress + Length) < 0x1000000)
    {
        WriteBuffer[COMMAND_OFFSET]   = PAGE_PROGRAM;
        DiscardByteCnt = 4; 
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)(SrcAddress & 0xFFU);
    }
    else
    {
        WriteBuffer[COMMAND_OFFSET]   = PAGE_PROGRAM_4ADDR;
        DiscardByteCnt = 5; 
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF000000U) >> 24);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
        WriteBuffer[ADDRESS_4_OFFSET] = (u8)(SrcAddress & 0xFFU);
    }
    
    
    FlashMsg.TxBfrPtr = WriteBuffer;
    FlashMsg.RxBfrPtr = (u8 *)DestAddress;
    FlashMsg.txByteCount = DiscardByteCnt;
    FlashMsg.rxByteCount = Length;
    FlashMsg.BusWidth = g_pdev->cfg.width;
    FlashMsg.Flags = XQSPIPSU_MSG_FLAG_TX | XQSPIPSU_MSG_FLAG_RX;

    if (g_pdev->cfg.width == SPI_FRF_X1_MODE) {
        Qspi_Disable(g_pdev);
        Config_QspiTx(g_pdev,DiscardByteCnt);
        // Config_QspiRx(g_pdev,Length);
        Qspi_Enable(g_pdev);
    }
    else if (g_pdev->cfg.width == SPI_FRF_X2_MODE || g_pdev->cfg.width == SPI_FRF_X4_MODE) {
        printf("SPI_FRF_X2_MODE || SPI_FRF_X4_MODE\r\n");
    }
    /**
    * Send the read command to the Flash to read the specified number
    * of bytes from the Flash, send the read command and address and
    * receive the specified number of bytes of data in the data buffer
    */
    Status = Test_QspiTransfer(g_pdev, &FlashMsg);
   
    return Status;
}

// uint32_t Test_QspiInit(void)
// {
//     uint32_t Status;

//     QspiParams params;

//     Qspi_PinmuxInit();

//     Status = Csu_QspiInit(&params);
//     if(AL_SUCCESS != Status)
//     {
//         printf("Csu_QspiInit error\r\n");
//         return Status;
//     }

//     printf("flashID:%x\r\n", params.flashID);
//     printf("flashSize:%x\r\n", params.flashSize);

//     return AL_SUCCESS;
// }

uint32_t Test_QspiProgram(u32 offset, u8* dest, u32 length)
{
    u32 RemainingBytes=0;
	u32 TransferBytes=0;
    u32 Status=AL_SUCCESS;

	RemainingBytes = length;

	while(RemainingBytes > 0U ) {

		if (RemainingBytes > QSPI_FIFO_MAX_SIZE)
			TransferBytes = QSPI_FIFO_MAX_SIZE;
		else
			TransferBytes = RemainingBytes;

        Status = Qspi_Program(offset, dest, TransferBytes);

		if(AL_SUCCESS == Status) {

			/**
			* Update the variables
			*/
			RemainingBytes -= TransferBytes;
			dest    += TransferBytes;
			offset  += TransferBytes;
		}
		else {
			return Status;
		}
	}
    
    return AL_SUCCESS;
}


