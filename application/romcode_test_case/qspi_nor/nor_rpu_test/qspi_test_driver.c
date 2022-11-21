#include "qspi_test_driver.h"



static void Qspi_PinmuxInit(void)
{
     *(uint32_t *)(0xf8803000u) =0x1;	  //QSPI  MIO0
     *(uint32_t *)(0xf8803004u) =0x1;
     *(uint32_t *)(0xf8803008u) =0x1;
     *(uint32_t *)(0xf880300cu) =0x1;
     *(uint32_t *)(0xf8803010u) =0x1;
     *(uint32_t *)(0xf8803014u) =0x1;
     *(uint32_t *)(0xf8803018u) =0x1;   
}





uint32_t Qspi_SendCmd(QspiParams *params)
{
    Qspi_PinmuxInit();

    return QSPI_SUCCESS;
}


uint32_t Qspi_Program(uint32_t offset, uint32_t dest, uint32_t TransferBytes)
{

        s32 Status=0;
    u32 DiscardByteCnt;
    
    WriteBuffer[COMMAND_OFFSET]   = (u8)ReadCommand;
    
    if (ReadCommand == BASE_READ_CMD_24BIT || ReadCommand == DUAL_READ_CMD_24BIT || ReadCommand == QUAD_READ_CMD_24BIT) {
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)(SrcAddress & 0xFFU);
        DiscardByteCnt = 4;
    }
    else if (ReadCommand == BASE_READ_CMD_32BIT ||ReadCommand == DUAL_READ_CMD_32BIT || ReadCommand == QUAD_READ_CMD_32BIT) {
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF000000U) >> 24);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
        WriteBuffer[ADDRESS_4_OFFSET] = (u8)(SrcAddress & 0xFFU);
        DiscardByteCnt = 5;
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
        Config_QspiRx(g_pdev,Length);
        Qspi_Enable(g_pdev);
    }
    else if (g_pdev->cfg.width == SPI_FRF_X2_MODE || g_pdev->cfg.width == SPI_FRF_X4_MODE) {
        Qspi_Disable(g_pdev);
        Config_QspiTx(g_pdev,DiscardByteCnt/4+1);
        Config_QspiRx(g_pdev,Length);
        Qspi_Enable(g_pdev);
    }
    /**
    * Send the read command to the Flash to read the specified number
    * of bytes from the Flash, send the read command and address and
    * receive the specified number of bytes of data in the data buffer
    */
    Status = Qspi_PolledTransfer(g_pdev, &FlashMsg);
   
    if (Status != 0) {
         Status= 2;
    }
    
END:
    return Status;
    
    return QSPI_SUCCESS;
}

uint32_t Test_QspiInit(void)
{
    uint32_t Status;

    QspiParams params;

    Qspi_PinmuxInit();

    Status = Csu_QspiInit(&params);
    if(QSPI_SUCCESS != Status)
    {
        printf("Csu_QspiInit error\r\n");
        return Status;
    }

    printf("flashID:%x\r\n", params.flashID);
    printf("flashSize:%x\r\n", params.flashSize);

    return QSPI_SUCCESS;
}

uint32_t Test_QspiProgram(u32 offset, u8* dest, u32 length)
{
    u32 RemainingBytes=0;
	u32 TransferBytes=0;
    

	if ((offset + length) > QspiFlashSize)
	{
		return  -1;
	}
    
	RemainingBytes = length;
	Mtimer1_Start(1000000); //1s
	while(RemainingBytes > 0U && (Mtimer1_TimerOut() == 0) ) {

		if (RemainingBytes > QSPI_FIFO_MAX_SIZE)
			 TransferBytes = QSPI_FIFO_MAX_SIZE;
		 else
			 TransferBytes = RemainingBytes;

		if(Qspi_Program(offset, dest, TransferBytes) == 0) {

			/**
			* Update the variables
			*/
			RemainingBytes -= TransferBytes;
			dest    += TransferBytes;
			offset  += TransferBytes;
		}
		else {
			return 1;
		}
	}
    
    return QSPI_SUCCESS;
}


