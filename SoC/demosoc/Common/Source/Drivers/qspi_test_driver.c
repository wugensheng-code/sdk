#include "qspi_test_driver.h"
#include "mtimer.h"



static QspiFlash_Msg FlashMsg;

static MtimerParams Timer1;
static MtimerParams Timer2;

static MtimerParams* QspiTimer1 = &Timer1;
static MtimerParams* QspiTimer2 = &Timer2;
extern QSPI_CORE_HANDLE    *g_pdev;



 u32 ReadCommand=0U;

static void Config_QspiTx(QSPI_CORE_HANDLE *pdev, u32 start_level)
{
    Qspi_TxfifoStartLevelSet(pdev,start_level-1); //transfer will start on serial line
}


static void Config_QspiRx(QSPI_CORE_HANDLE *pdev, u32 rx_numfame)
{
    Qspi_Ctrl1Ndf(pdev,rx_numfame-1);

}

static u32 Wait_TxCompl(QSPI_CORE_HANDLE *pdev)
{
    //Mtimer_Start(30);
    //while( (!Qspi_TxfifoEmpty(pdev)) && (Mtimer_TimerOut()==0) ); // wait TFE returns to 1,txfifoempty
	MTIMER_OUT_CONDITION(300000,QspiTimer2,!Qspi_TxfifoEmpty(pdev));

	if (Mtimer_IsTimerOut(QspiTimer2) == 1) {
		 return TIMER_OUT_ERR0;
	}

	return AL_SUCCESS;

}






uint32_t Wait_WIP(void)
{
	volatile uint32_t Status = 0, ReadData = 0x01;

	MTIMER_OUT_CONDITION(500000,QspiTimer1, (ReadData&0x01)  ) {

		Qspi_Disable(g_pdev);
		Config_QspiTx(g_pdev, 1);
		Config_QspiRx(g_pdev,1);
		Qspi_Width(g_pdev,QSPI_WIDTH_X1);
		Qspi_Mode(g_pdev,TMOD_EEPROM);
		Qspi_Enable(g_pdev);

		Qspi_DataTransmit(g_pdev, READ_STATUS_CMD);

		Status = Wait_TxCompl(g_pdev);
		if(Status != AL_SUCCESS)
		{
			return Status;
		}

		Mtimer_Start(QspiTimer2, 1000);

		while(!Qspi_RxNotempty(g_pdev))  // RXFIFO not empty
		{
			if (Mtimer_IsTimerOut(QspiTimer2) == 1)
			{
				return TIMER_OUT_ERR11;
			}
		}

		ReadData = Qspi_DataRead(g_pdev) & 0xFF;
	}

	if (Mtimer_IsTimerOut(QspiTimer1) == 1) {
		return TIMER_OUT_ERR12;
	}

	return AL_SUCCESS;

}

uint32_t SetQuad(uint8_t SetQuadCmd, uint8_t ReadQuadCmd, uint8_t QuadPos)
{
	uint32_t Status = 0,data = 0;

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 1);
	Config_QspiRx(g_pdev, 1);
	Qspi_Mode(g_pdev,TMOD_EEPROM);
	Qspi_Width(g_pdev,QSPI_WIDTH_X1);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, ReadQuadCmd);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		return Status;
	}

	MTIMER_OUT_CONDITION(30000,QspiTimer2, !(Qspi_RxNotempty(g_pdev)));

	if (Mtimer_IsTimerOut(QspiTimer2) == 1) {
		 return TIMER_OUT_ERR10;
	}

	data = Qspi_DataRead(g_pdev);
	data = data | (1 << QuadPos);

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 1);
	Qspi_Mode(g_pdev,TMOD_TX_ONLY);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, 0x06);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		return Status;
	}

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 2);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, SetQuadCmd);
	Qspi_DataTransmit(g_pdev, data);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		return Status;
	}

	Status = Wait_WIP();

	if(Status != AL_SUCCESS)
	{
		return Status;
	}


	return AL_SUCCESS;
}


void Micorn_Test(void)
{
	uint32_t i = 0;

	i = Wait_TxCompl(g_pdev);

	if(i != AL_SUCCESS)
	{
		printf("Wait_TxCompl Error\r\n");
	}
}






void Qspi_ReadStatus(void)
{
    uint8_t ReadData = 0, Status = AL_SUCCESS;

    Qspi_Disable(g_pdev);
    Config_QspiTx(g_pdev, 1);
    Config_QspiRx(g_pdev,1);
    Qspi_Enable(g_pdev);

    Qspi_DataTransmit(g_pdev, READ_STATUS_REG1);
    Status = Wait_TxCompl(g_pdev);

    	if(Status != AL_SUCCESS)
    	{
    		printf("Wait_TxCompl Error\r\n");
    	}

    if(AL_SUCCESS != Status)
    {
        return DATA_XFER_ERR1;
    }

    Mtimer_Start(QspiTimer2, 1000);

    while(!Qspi_RxNotempty(g_pdev))  // RXFIFO not empty
    {
        if (Mtimer_IsTimerOut(QspiTimer2) == 1)
        {
            return DATA_XFER_ERR1;
        }
    }

    ReadData = Qspi_DataRead(g_pdev) & 0xFF;
    printf("0x05 status_reg1:%x\r\n", ReadData);


    Qspi_Disable(g_pdev);
    Config_QspiTx(g_pdev, 1);
    Config_QspiRx(g_pdev,1);
    Qspi_Enable(g_pdev);

    Qspi_DataTransmit(g_pdev, READ_STATUS_REG2);
    Status = Wait_TxCompl(g_pdev);
    if(AL_SUCCESS != Status)
    {
        return DATA_XFER_ERR1;
    }

    Mtimer_Start(QspiTimer2, 1000);

    while(!Qspi_RxNotempty(g_pdev))  // RXFIFO not empty
    {
        if (Mtimer_IsTimerOut(QspiTimer2) == 1)
        {
            return DATA_XFER_ERR1;
        }
    }

    ReadData = Qspi_DataRead(g_pdev) & 0xFF;
    printf("0x35 status_reg2:%x\r\n", ReadData);


    Qspi_Disable(g_pdev);
    Config_QspiTx(g_pdev, 1);
    Config_QspiRx(g_pdev,1);
    Qspi_Enable(g_pdev);

    Qspi_DataTransmit(g_pdev, READ_STATUS_REG3);
    Status = Wait_TxCompl(g_pdev);
    if(AL_SUCCESS != Status)
    {
        return DATA_XFER_ERR1;
    }

    Mtimer_Start(QspiTimer2, 1000);

    while(!Qspi_RxNotempty(g_pdev))  // RXFIFO not empty
    {
        if (Mtimer_IsTimerOut(QspiTimer2) == 1)
        {
            return DATA_XFER_ERR1;
        }
    }

    ReadData = Qspi_DataRead(g_pdev) & 0xFF;
    printf("0x15 status_reg3:%x\r\n", ReadData);

    return AL_SUCCESS;
}


uint32_t flash_get_cr(void)
{
	uint32_t config_reg = 0;
	uint32_t i = 0,Status = 0;


	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 1);
	Config_QspiRx(g_pdev, 1);
	Qspi_Mode(g_pdev,TMOD_EEPROM);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, 0x35);

	Status = Wait_TxCompl(g_pdev);
	if(AL_SUCCESS != Status)
	{
		return DATA_XFER_ERR1;
	}

	config_reg = Qspi_DataRead(g_pdev) & 0xFF;

	return config_reg;
}



void Qspi_ReadStatusAndRemovePortection(void)
{
	uint32_t Status = 0,temp = 0,ReadData = 0;
	uint8_t tempcr = 0;

    Qspi_Disable(g_pdev);
    Config_QspiTx(g_pdev, 1);
    Config_QspiRx(g_pdev, 1);
    Qspi_Mode(g_pdev,TMOD_EEPROM);
    Qspi_Enable(g_pdev);


	Qspi_DataTransmit(g_pdev, READ_STATUS_REG1);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("Qspi_ReadStatusAndRemovePortection Error\r\n");
	}

	if(Qspi_RxNotempty(g_pdev))
	{
		temp = Qspi_DataRead(g_pdev);
		printf("Qspi_ReadStatusAndRemovePortection Status:0x%x \r\n",temp);
	}

	if(temp&(7 << 2))
	{
//		tempcr = flash_get_cr();
		EnableWrite();

		Qspi_Disable(g_pdev);
		Config_QspiTx(g_pdev, 2);
		Qspi_Mode(g_pdev,TMOD_TX_ONLY);
		Qspi_Enable(g_pdev);

		Qspi_DataTransmit(g_pdev, 0x01);
		Qspi_DataTransmit(g_pdev, 0);
//		Qspi_DataTransmit(g_pdev, tempcr);

		Status = Wait_TxCompl(g_pdev);
		if(AL_SUCCESS != Status)
		{
			return DATA_XFER_ERR1;
		}

		do{

			Qspi_Disable(g_pdev);
			Config_QspiTx(g_pdev, 1);
			Config_QspiRx(g_pdev,1);
			Qspi_Mode(g_pdev,TMOD_EEPROM);
			Qspi_Enable(g_pdev);

			Qspi_DataTransmit(g_pdev, READ_STATUS_REG1);
		   // Qspi_DataTransmit(g_pdev, 0x07);
			Status = Wait_TxCompl(g_pdev);

			Mtimer_Start(QspiTimer2, 1000);

			while(!Qspi_RxNotempty(g_pdev))  // RXFIFO not empty
			{
				if (Mtimer_IsTimerOut(QspiTimer2) == 1)
				{
					printf("WIP-write in progress Mtimer_IsTimerOut\r\n");
					return DATA_XFER_ERR1;
				}
			}

			ReadData = Qspi_DataRead(g_pdev) & 0xFF;

			printf("READ_STATUS_REG1:%x\r\n", ReadData);

			Mtimer_Delay(100000);

		}while(ReadData&0x01);
	}


}

void Qspi_ReadStatus1(void)
{
	uint32_t Status = 0;

    Qspi_Disable(g_pdev);
    Config_QspiTx(g_pdev, 1);
    Config_QspiRx(g_pdev, 1);
    Qspi_Mode(g_pdev,TMOD_EEPROM);
    Qspi_Enable(g_pdev);

    do{
    	Qspi_DataTransmit(g_pdev, READ_STATUS_REG1);

    	Status = Wait_TxCompl(g_pdev);

    	if(Status != AL_SUCCESS)
    	{
    		printf("Qspi_ReadStatus1 Error\r\n");
    	}

    	if(Qspi_RxNotempty(g_pdev))
    	{
    		Status = Qspi_DataRead(g_pdev);
    		printf("Qspi_ReadStatus1 Status:0x%x \r\n",Status);
    	}
    }while(!(Status&0x02));

}


uint8_t Qspi_ReadStatus2(void)
{
	uint32_t Status = 0;

    Qspi_Disable(g_pdev);
    Config_QspiTx(g_pdev, 1);
    Config_QspiRx(g_pdev, 1);
    Qspi_Mode(g_pdev,TMOD_EEPROM);
    Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, READ_STATUS_REG2);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("Qspi_ReadStatus3 Error\r\n");
	}

	while(!(Qspi_RxNotempty(g_pdev)))

	Status = Qspi_DataRead(g_pdev);
	printf("Qspi_ReadStatus3 Status:0x%x \r\n",Status);

	return Status;



}


void Qspi_WriteStatus2(uint8_t value)
{
	uint32_t Status = 0;

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 3);
	Qspi_Mode(g_pdev,TMOD_TX_ONLY);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, WRITE_ENABLE);
	Qspi_DataTransmit(g_pdev, WRITE_STATUS_REG2);
	Qspi_DataTransmit(g_pdev, value);
	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("Wait_TxCompl Error\r\n");
	}
}



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

uint8_t EnableWrite(void)
{
	uint32_t Status = 0;

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 1);
	Qspi_Mode(g_pdev,TMOD_TX_ONLY);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, WRITE_ENABLE);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("WRITE_ENABLE Error\r\n");
	}
}


uint8_t SectorErase(uint32_t addr)
{
//	printf("SectorErase Start\r\n");

	uint32_t Status = 0;

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 4);
	Qspi_Mode(g_pdev,TMOD_TX_ONLY);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, 0xDC);

	Qspi_DataTransmit(g_pdev, (addr >> 24)&0xff );
	Qspi_DataTransmit(g_pdev, (addr >> 16)&0xff );
	Qspi_DataTransmit(g_pdev, (addr >> 8)&0xff );
	Qspi_DataTransmit(g_pdev, (addr >> 0)&0xff );

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("SectorErase Error\r\n");
	}

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 1);
	Config_QspiRx(g_pdev, 1);
	Qspi_Mode(g_pdev,TMOD_EEPROM);
	Qspi_Enable(g_pdev);

	do{
		Qspi_DataTransmit(g_pdev, READ_STATUS_REG1);

		Status = Wait_TxCompl(g_pdev);

		if(Status != AL_SUCCESS)
		{
			printf("READ_STATUS_REG1 Error\r\n");
		}


		if(Qspi_RxNotempty(g_pdev))
		{
			Status = Qspi_DataRead(g_pdev);
			printf("READ_STATUS_REG1 Status:0x%x \r\n",Status);
		}
	}while(Status&0x01);



}



uint8_t WriteStatusReg1(void)
{
	uint32_t Status = 0;

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 3);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, WRITE_ENABLE);
	Qspi_DataTransmit(g_pdev, 0x01);
	Qspi_DataTransmit(g_pdev, (7<<2)+2);
	Status = Wait_TxCompl(g_pdev);

		if(Status != AL_SUCCESS)
		{
			printf("Wait_TxCompl Error\r\n");
		}
}




uint8_t WriteTest(void)
{
	uint32_t Status = 0;




	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 1);
	Qspi_Mode(g_pdev,TMOD_TX_ONLY);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, WRITE_ENABLE);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("WRITE_ENABLE Error\r\n");
	}




	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 10+5);
	Qspi_Mode(g_pdev,TMOD_TX_ONLY);
//	Qspi_Dfs(g_pdev,DFS_WORD);
	Qspi_Enable(g_pdev);



	Qspi_DataTransmit(g_pdev, 0x02);

	Qspi_DataTransmit(g_pdev,0);
	Qspi_DataTransmit(g_pdev,0);
	Qspi_DataTransmit(g_pdev,50);

	Qspi_DataTransmit(g_pdev,1);
	Qspi_DataTransmit(g_pdev,66);
	Qspi_DataTransmit(g_pdev,3);
	Qspi_DataTransmit(g_pdev,66);
	Qspi_DataTransmit(g_pdev,54);

	Qspi_DataTransmit(g_pdev,54);

	Qspi_DataTransmit(g_pdev,1);
		Qspi_DataTransmit(g_pdev,2);
		Qspi_DataTransmit(g_pdev,65);
		Qspi_DataTransmit(g_pdev,46);
		Qspi_DataTransmit(g_pdev,54);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("WriteTest Error\r\n");
	}

	Qspi_Disable(g_pdev);
	Qspi_Mode(g_pdev,TMOD_EEPROM);
//	Qspi_Dfs(g_pdev,DFS_BYTE);
	Qspi_Enable(g_pdev);

}



uint8_t Test_ReadId(void)
{
	uint32_t Status = 0,index = 0;

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 1);
	Config_QspiRx(g_pdev, 3);
	Qspi_Enable(g_pdev);

//	Qspi_DataTransmit(g_pdev, 0x90);
//
	Qspi_DataTransmit(g_pdev, 0x9f);

//	Qspi_DataTransmit(g_pdev, 0xab);
//
//	Qspi_DataTransmit(g_pdev, 0x9f);
//	Qspi_DataTransmit(g_pdev, 0x9f);
//	Qspi_DataTransmit(g_pdev, 0x9f);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("READ_ID_CMD \r\n");
	}

	printf("TEST Id: \r\n");

	 while ( (index < 3) && (Mtimer_TimerOut(QspiTimer2) == 0)) {
		//MTIMER_OUT_CONDITION(300,QspiTimer2,index < 3) {
	        if (Qspi_RxNotempty(g_pdev)) {
	            printf("%x ", Qspi_DataRead(g_pdev) & 0xff);
	            index++;
	        }
	    }

	 printf("\r\n");

}

uint8_t Qspi_ChipErase(void)
{
    uint8_t Status = AL_SUCCESS, ReadData = 0;
    Qspi_Disable(g_pdev);
    Config_QspiTx(g_pdev, 1);
    Qspi_Mode(g_pdev,TMOD_TX_ONLY);
    Qspi_Enable(g_pdev);

 //   Qspi_DataTransmit(g_pdev, WRITE_ENABLE);
    Qspi_DataTransmit(g_pdev, CHIP_ERASE);


    Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("Wait_TxCompl Error\r\n");
	}

    do{
        printf("ChipErase going\r\n");

        Qspi_Disable(g_pdev);
        Config_QspiTx(g_pdev, 1);
        Config_QspiRx(g_pdev,1);
        Qspi_Mode(g_pdev,TMOD_EEPROM);
        Qspi_Enable(g_pdev);

        Qspi_DataTransmit(g_pdev, READ_STATUS_REG1);
       // Qspi_DataTransmit(g_pdev, 0x07);
        Status = Wait_TxCompl(g_pdev);

        Mtimer_Start(QspiTimer2, 1000);

        while(!Qspi_RxNotempty(g_pdev))  // RXFIFO not empty
        {
            if (Mtimer_IsTimerOut(QspiTimer2) == 1)
            {
            	 printf("ChipErase Mtimer_IsTimerOut\r\n");
                return DATA_XFER_ERR1;
            }
        }

        ReadData = Qspi_DataRead(g_pdev) & 0xFF;

        printf("READ_STATUS_REG1:%x\r\n", ReadData);

        Mtimer_Delay(5000000);

        printf("5s delay\r\n");

    }while(ReadData&0x01);

    printf("ChipErase finish\r\n");

    return Status;
}

















u32 Test_QspiTransfer(QSPI_CORE_HANDLE *pdev, QspiFlash_Msg *Msg)
{

    volatile u32 Index = 0, i = 0;
	u32 Status=AL_SUCCESS;

    /*
     * Set the busy flag, which will be cleared when the transfer is
     * entirely done.
     */
//	Qspi_Disable(g_pdev);
//	Config_QspiTx(g_pdev, 1);
//	Qspi_Enable(g_pdev);
//
//	Qspi_DataTransmit(g_pdev, WRITE_ENABLE);
//
//	Status = Wait_TxCompl(g_pdev);
//
//	if(Status != AL_SUCCESS)
//		{
//			printf("WRITE_ENABLE Error\r\n");
//		}




    Index = 0;

    if (Msg->BusWidth == QSPI_WIDTH_X1) {


    	Qspi_Disable(g_pdev);
		Config_QspiTx(g_pdev, 1);
		Qspi_Mode(g_pdev,TMOD_TX_ONLY);
		Qspi_Enable(g_pdev);

		Qspi_DataTransmit(g_pdev, WRITE_ENABLE);

		Status = Wait_TxCompl(g_pdev);

		if(Status != AL_SUCCESS)
		{
			printf("WRITE_ENABLE Error\r\n");
		}

//		printf("Msg->rxByteCount:%d\r\n",Msg->rxByteCount);
//		printf("Msg->txByteCount:%d\r\n",Msg->txByteCount);
//
//		printf("Msg->TxBfrPtr: ");
//		for(i = 0;i < Msg->txByteCount;i++)
//		{
//			printf("%d ", Msg->TxBfrPtr[i]);
//		}
//		printf("\r\n");
//
//
//		printf("Msg->RxBfrPtr: ");
//		for(i = 0;i < Msg->rxByteCount;i++)
//		{
//			printf("%d ", Msg->RxBfrPtr[i]);
//		}
//		printf("\r\n\r\n\r\n\r\n");

		Qspi_Disable(g_pdev);
		Config_QspiTx(g_pdev, (Msg->txByteCount+Msg->rxByteCount));
		// Config_QspiRx(g_pdev,Length);
		Qspi_Enable(g_pdev);


		Index = 0;
        //Mtimer_Start(30);
		i = Msg->txByteCount;

        while ((i > 0) ) {
		//MTIMER_OUT_CONDITION(300,QspiTimer2,Msg->txByteCount > 0) {
            Qspi_DataTransmit(pdev,Msg->TxBfrPtr[Index]);

            Index++;
            i--;
        }
//		if (Mtimer_IsTimerOut(QspiTimer2) == 1)
//		{
//			return DATA_XFER_ERR0;
//		}
//		printf("%d ",Index);printf("\r\n");

   //     Status=Wait_TxCompl(pdev);



        Index = 0;

        //printf("Msg->rxByteCount:%d Msg->RxBfrPtr[0]:%d\r\n",Msg->rxByteCount, Msg->RxBfrPtr[0]);

//        Qspi_Disable(g_pdev);
//        Config_QspiTx(g_pdev, Msg->rxByteCount);
//        Qspi_Enable(g_pdev);

      //  printf("Msg->RxBfrPtr: ");
//		for(i = 0;i < Msg->rxByteCount;i++)
//		{
//			printf("%d ", Msg->RxBfrPtr[i]);
//		}
//		printf("\r\n");


       // MTIMER_OUT_CONDITION(100000,QspiTimer2,Msg->rxByteCount > 0) {
        i = Msg->rxByteCount;
        while ((i > 0) ) {
            Qspi_DataTransmit(pdev, Msg->RxBfrPtr[Index]);
            Index++;
            i--;
        }


//		if (Mtimer_IsTimerOut(QspiTimer2) == 1)
//		{
//			return DATA_XFER_ERR0;
//		}


        Status=Wait_TxCompl(pdev);
        if(Status != AL_SUCCESS)
		{
			printf("Wait_TxCompl Error\r\n");
		}

        Index = 0;

    }
    else if (Msg->BusWidth == QSPI_WIDTH_X2 || Msg->BusWidth == QSPI_WIDTH_X4) {
        printf(" error QSPI_WIDTH_X2 || QSPI_WIDTH_X4\r\n");
    }

    return Status;
}


uint32_t Qspi_Program(u32 SrcAddress, u8* DestAddress, u32 Length)
{

    s32 Status=0;
    u32 DiscardByteCnt;



    u8 WriteBuffer[10] __attribute__ ((aligned(32)));

    if((SrcAddress + Length) <= 0x1000000)
    {
    	//printf("3 addr mode\r\n");
        WriteBuffer[COMMAND_OFFSET]   = PAGE_PROGRAM;
        DiscardByteCnt = 4;
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)(SrcAddress & 0xFFU);
    }
    else
    {
    	//printf("4 addr mode\r\n");
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
        printf(" error SPI_FRF_X2_MODE || SPI_FRF_X4_MODE\r\n");
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
    volatile u32 RemainingBytes=0;
    volatile u32 TransferBytes=0;
    volatile u32 Status=AL_SUCCESS;

    Qspi_Disable(g_pdev);
	Qspi_Mode(g_pdev,TMOD_TX_ONLY);
	Qspi_Enable(g_pdev);


//	Qspi_Disable(g_pdev);
//	Config_QspiTx(g_pdev, 1);
//	Qspi_Enable(g_pdev);
//
//	Qspi_DataTransmit(g_pdev, WRITE_ENABLE);
//
//	Status = Wait_TxCompl(g_pdev);
//
//	if(Status != AL_SUCCESS)
//	{
//		printf("WRITE_ENABLE Error\r\n");
//	}

	RemainingBytes = length;

	while(RemainingBytes > 0U ) {

		if (RemainingBytes > 110)
			TransferBytes = 110;
		else
			TransferBytes = RemainingBytes;

		if((offset/(256)) < ((offset + TransferBytes)/(256)))
		{
			TransferBytes = 256-offset%256;
			Status = Qspi_Program(offset, dest, TransferBytes);
		}
		else
		{
			Status = Qspi_Program(offset, dest, TransferBytes);
		}

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

		Mtimer_Delay(3500);
	}

	Qspi_Disable(g_pdev);
	Qspi_Mode(g_pdev,TMOD_EEPROM);
	Qspi_Enable(g_pdev);

    return AL_SUCCESS;
}

void Nor_SendCmd(uint8_t cmd)
{
	uint32_t Status;

	Qspi_Disable(g_pdev);
	Config_QspiTx(g_pdev, 1);
	Qspi_Mode(g_pdev,TMOD_TX_ONLY);
	Qspi_Enable(g_pdev);

	Qspi_DataTransmit(g_pdev, cmd);

	Status = Wait_TxCompl(g_pdev);

	if(Status != AL_SUCCESS)
	{
		printf("WRITE_ENABLE Error\r\n");
		return Status;
	}

}






















uint8_t firstCmdSend = 0;

uint8_t QPI_Performance_Enhance = 0;

static u32 Qspi_PolledTransfer(QSPI_CORE_HANDLE *pdev, QspiFlash_Msg *Msg)
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
    	printf(" error Qspi_PolledTransfer QSPI_WIDTH_X1\r\n");
    }
    else if ((Msg->BusWidth == QSPI_WIDTH_X2) || (Msg->BusWidth == QSPI_WIDTH_X4)) {
         //Mtimer_Start(30);
         //while ( (Msg->txByteCount > 0) && (Mtimer_TimerOut() ==  0) ) {
		 MTIMER_OUT_CONDITION(5000000,QspiTimer2,Msg->txByteCount > 0) {
            if (Index == 0) {
            	if (QPI_Performance_Enhance == 1) {
            		printf("QPI_Performance_Enhance == 1\r\n");
					if(firstCmdSend == 0)
					{
						printf(" firstCmdSend = 0\r\n");
						firstCmdSend = 1;
						Tdata = Msg->TxBfrPtr[0];
						Qspi_DataTransmit(pdev,Tdata);
						Index++;
						Msg->txByteCount--;
					}
					else
					{
						printf(" firstCmdSend = 1\r\n");

						Qspi_Disable(g_pdev);
						Qspi_InstructionLength(pdev,0);
						Qspi_Enable(g_pdev);

						Index++;
						Msg->txByteCount--;
					}
            	}
            	else
            	{
            		Tdata = Msg->TxBfrPtr[0];
					Qspi_DataTransmit(pdev,Tdata);
					Index++;
					Msg->txByteCount--;
            	}
            }
            else if(Index == 1) {
            	if(pdev->cfg.addr_mode <= SPI_FLASH_ADDR32)
            	{
            		printf("pdev->cfg.addr_mode <= SPI_FLASH_ADDR32\r\n");

					if (pdev->cfg.addr_mode == SPI_FLASH_ADDR32) {
						Tdata = (Msg->TxBfrPtr[1] << 24) | (Msg->TxBfrPtr[2] << 16) | (Msg->TxBfrPtr[3] << 8) | (Msg->TxBfrPtr[4]);
						Index += 4;
						Msg->txByteCount -= 4;
					}
					else if (pdev->cfg.addr_mode == SPI_FLASH_ADDR24) {
						 Tdata = (Msg->TxBfrPtr[1] << 16) | (Msg->TxBfrPtr[2] << 8) | (Msg->TxBfrPtr[3]) ;
						 Index += 3;
						 Msg->txByteCount -= 3;
					}
					  Qspi_DataTransmit(pdev,Tdata);
            	}
            	else
            	{
            		printf(" SPI_FLASH_ADDR40\r\n");

            		if (pdev->cfg.addr_mode == 0x0a) {
						Tdata = (Msg->TxBfrPtr[1] << 24) | (Msg->TxBfrPtr[2] << 16) | (Msg->TxBfrPtr[3] << 8) | (Msg->TxBfrPtr[4]);
						Index += 4;
						Msg->txByteCount -= 4;
						Qspi_DataTransmit(pdev,Tdata);

						Tdata = (Msg->TxBfrPtr[5]);
						Index += 1;
						Msg->txByteCount -= 1;
						Qspi_DataTransmit(pdev,Tdata);
					}
            	}
            }
            else {
              // never run here error
               Index += 4;
               Msg->txByteCount -= 4;
               //printf("never run here error Msg->txByteCount:%d\r\n", Msg->txByteCount);
            }
         }

		 if (Mtimer_IsTimerOut(QspiTimer2) == 1) {
			return DATA_XFER_ERR2;
		 }

            Status=Wait_TxCompl(pdev);
            Index = 0;
            //Mtimer_Start(1000);
            //while ( (Msg->rxByteCount > 0) && (Mtimer_TimerOut()==0)  ) {
			MTIMER_OUT_CONDITION(500000,QspiTimer2,Msg->rxByteCount > 0) {
                if(Qspi_RxNotempty(pdev))  // RXFIFO not empty, is must 4byte aligned
                {
                    Rdata = Qspi_DataRead(pdev);
                    Msg->RxBfrPtr[Index] = Rdata & 0xFF;
                    Index++;
                    Msg->rxByteCount --;
                }
        }

		if (Mtimer_IsTimerOut(QspiTimer2) == 1) {
			return DATA_XFER_ERR3;
		 }

    }

    return Status;
}

















static u32 QspiFlashSize =  0;

static u8 WriteBuffer[10] __attribute__ ((aligned(32)));

static u32 Qspi_TestCopy(u32 SrcAddress, u8* DestAddress, u32 Length)
{
    u32 Status=AL_SUCCESS;
    u32 DiscardByteCnt = 0;


    WriteBuffer[COMMAND_OFFSET]   = (u8)ReadCommand;

    printf("ReadCommand:0x%x\r\n", ReadCommand);

    if (ReadCommand == 0xEB || ReadCommand == 0xBB) {
        WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
        WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
        WriteBuffer[ADDRESS_3_OFFSET] = (u8)(SrcAddress & 0xFFU);
        DiscardByteCnt = 4;
    }
    else  if (ReadCommand == 0xEC || ReadCommand == 0xBC) {
    	WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF000000U) >> 24);
		WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
		WriteBuffer[ADDRESS_3_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
		WriteBuffer[ADDRESS_4_OFFSET] = (u8)(SrcAddress & 0xFFU);
		DiscardByteCnt = 5;
    }
    else
    {
    	printf("Qspi_TestCopy error\r\n");
    	while(1);
    }

   if (QPI_Performance_Enhance == 1) {
	   printf("QPI_Performance_Enhance == 1\r\n");
	   if (ReadCommand == 0xEB || ReadCommand == 0xBB) {

			WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
			WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
			WriteBuffer[ADDRESS_3_OFFSET] = (u8)(SrcAddress & 0xFFU);
			if((g_pdev->cfg.flashID&0xff) != 0xC2)
			{
				WriteBuffer[ADDRESS_4_OFFSET] = 0xA0;
			}
			else
			{
				WriteBuffer[ADDRESS_4_OFFSET] = 0xA5;
			}

			DiscardByteCnt = 5;
	   	}
	   else  if (ReadCommand == 0xEC || ReadCommand == 0xBC) {

	      	WriteBuffer[ADDRESS_1_OFFSET] = (u8)((SrcAddress & 0xFF000000U) >> 24);
	  		WriteBuffer[ADDRESS_2_OFFSET] = (u8)((SrcAddress & 0xFF0000U) >> 16);
	  		WriteBuffer[ADDRESS_3_OFFSET] = (u8)((SrcAddress & 0xFF00U) >> 8);
	  		WriteBuffer[ADDRESS_4_OFFSET] = (u8)(SrcAddress & 0xFFU);
	  		if((g_pdev->cfg.flashID&0xff) != 0xC2)
			{
				WriteBuffer[5] = 0xA0;
			}
			else
			{
				WriteBuffer[5] = 0xA5;
			}

	  		DiscardByteCnt = 6;
	    }
	   	else
		{
			printf("QPI_Performance_Enhance Qspi_TestCopy error\r\n");
			while(1);
		}

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

        if(DiscardByteCnt == 6)
        {
        	Config_QspiTx(g_pdev, 3);
        	printf("Config_QspiTx(g_pdev, 3);\r\n");
        }

        Config_QspiRx(g_pdev,Length);
        Qspi_Enable(g_pdev);
    }
    /**
    * Send the read command to the Flash to read the specified number
    * of bytes from the Flash, send the read command and address and
    * receive the specified number of bytes of data in the data buffer
    */
    Status = Qspi_PolledTransfer(g_pdev, &FlashMsg);

    return Status;
}




/*!
    \brief       Csu_QspiRead：QSPI flash data read.
    \param[in]  : offset-address of flash space
                  dest-the user buffer point
                  length-read data length
    \param[out] : no
    \retval     : 0 or -2,-3
*/

u32 Csu_TestQspiRead(u32 offset, u8* dest, u32 length)
{
    u32 RemainingBytes=0;
    u32 TransferBytes=0;
	u32 Status=AL_SUCCESS;
	QspiFlashSize =  g_pdev->cfg.flashSize;

     /**
     * Check the read length with Qspi flash size
     */
    if ((offset + length) > QspiFlashSize)
    {
        return  FLASH_READ_ERR0;
    }

    RemainingBytes = length;

     MTIMER_OUT_CONDITION(10000000,QspiTimer1,RemainingBytes > 0U) {

        if (RemainingBytes > QSPI_FIFO_MAX_SIZE)
             TransferBytes = QSPI_FIFO_MAX_SIZE;
         else
             TransferBytes = RemainingBytes;

		  Status=Qspi_TestCopy(offset,dest,TransferBytes);

        if ( Status == AL_SUCCESS ) {

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

	if (Mtimer_IsTimerOut(QspiTimer1) == 1) {
		return FLASH_READ_ERR1;
	}
    return AL_SUCCESS;
}
