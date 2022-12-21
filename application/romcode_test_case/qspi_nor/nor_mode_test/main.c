#include "qspi_test_driver.h"

QspiParams norParams;



uint8_t __attribute__ ((aligned(32))) test_buf[88888] = {0};
#define WRITE_SIZE 10000
#define READ_SIZE sizeof(test_buf)
#define TEST_SIZE 10000

extern QSPI_CORE_HANDLE    *g_pdev;
extern u32 ReadCommand;
extern uint8_t firstCmdSend;


#define TEMP_DDR_3 ((volatile uint32_t *)(0X6103E000UL+0X8UL))	//int_count_num


//#define DEEP_POWER_TEST 							1
//#define QPI_DEEP_POWER_TEST 						1

//#define QPI_DUMMY 6

#ifndef QPI_DUMMY
#define QPI_DUMMY 6
#endif


void DataTest(void)
{
	volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0,value = 0;

	printf("DataTest Start\r\n");

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("DataTest Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

	for(j =200;j < TEST_SIZE;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);

		status = Csu_QspiRead(j, test_buf, i);
		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
		}


		for(l = 0; l < i; l++)
		{

			temp1 = (j + l)/4;
			temp2 = (j + l)%4;

			if( test_buf[l] != ((temp1>>(temp2*8)) &0xff) )
			{
				printf("i:%d, j:%d \r\n",i, j);

				printf("test_buf[%d]:%x\r\n",l, test_buf[l]);
				printf("%x\r\n", ((temp1>>(temp2*8)) &0xff));

				printf("error Unequal data\r\n");
			}
		}

		j += i;

		i++;
		if(i > READ_SIZE)
		{
			i = 1;
		}

	}

	printf("DataTest End\r\n");
}


void QUAD_Read_Test(void)
{
	volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0,value = 0;

	printf("QUAD_Read_Test Start\r\n");

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("QUAD_Read_Test Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	if((norParams.flashID&0xff) != 0x01)
	{

		if( ((norParams.flashID&0xff) != 0x9d) && ((norParams.flashID&0xff) != 0xc2) )
		{
			status = SetQuad(0x31, 0x35, 1);
		}
		else
		{
			status = SetQuad(0x01, 0x05, 6);
		}

		if(status != AL_SUCCESS)
		{
			printf("SetQuad error:%d\r\n",status);
			return status;
		}

	}

	if(norParams.flashSize <= 0x1000000)
	{
		status = Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_24);
	}
	else
	{
		status = Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_32);
	}

	if(status != AL_SUCCESS)
	{
		printf("QUAD_Read_Test Csu_QspiSetMode error:%d\r\n",status);
		while(1);
	}


	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

	for(j =200;j < TEST_SIZE;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);

		status = Csu_QspiRead(j, test_buf, i);
		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
		}


		for(l = 0; l < i; l++)
		{

			temp1 = (j + l)/4;
			temp2 = (j + l)%4;

			if( test_buf[l] != ((temp1>>(temp2*8)) &0xff) )
			{
				printf("i:%d, j:%d \r\n",i, j);

				printf("test_buf[%d]:%x\r\n",l, test_buf[l]);
				printf("%x\r\n", ((temp1>>(temp2*8)) &0xff));

				printf("error Unequal data\r\n");
			}
		}

		j += i;

		i++;
		if(i > READ_SIZE)
		{
			i = 1;
		}

	}

	printf("QUAD_Read_Test End\r\n");

}


void QPI_Read_Test(void)
{
	volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0,value = 0;

	printf("QPI_Read_Test Start\r\n");

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("QPI_Read_Test Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

	if((norParams.flashID&0xff) != 0x01)
	{
		if( ((norParams.flashID&0xff) != 0x9d) && ((norParams.flashID&0xff) != 0xc2) )
		{
			status = SetQuad(0x31, 0x35, 1);
		}
		else
		{
			status = SetQuad(0x01, 0x05, 6);
		}

		if(status != AL_SUCCESS)
		{
			printf("SetQuad error:%d\r\n",status);
			return status;
		}
	}

	if( ((norParams.flashID&0xff) != 0x9d) && ((norParams.flashID&0xff) != 0xc2) )
		Nor_SendCmd(ENABLE_QPI_CMD);
	else
		Nor_SendCmd(0x35);

	status = Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_24);

	if(status != AL_SUCCESS)
	{
		printf("QPI_Read_Test Csu_QspiSetMode error:%d\r\n",status);
		while(1);
	}

	Qspi_Disable(g_pdev);
	Qspi_TransType(g_pdev, SPI_TRANSFER_TYPE2);

	Qspi_WaitCycles(g_pdev, QPI_DUMMY);

	Qspi_Enable(g_pdev);

	ReadCommand = 0xEB;

//	for(j =500;j < TEST_SIZE;)
//	{
		j = 0;
		i =200;

		printf("Csu_QspiRead Start addr:%d\r\n", j);

		status = Csu_TestQspiRead(j, test_buf, i);
		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_TestQspiRead error:%d\r\n", status);
		}


		for(l = 0; l < i; l++)
		{

			temp1 = (j + l)/4;
			temp2 = (j + l)%4;

			if( test_buf[l] != ((temp1>>(temp2*8)) &0xff) )
			{
				printf("i:%d, j:%d \r\n",i, j);

				printf("test_buf[%d]:%x\r\n",l, test_buf[l]);
				printf("%x\r\n", ((temp1>>(temp2*8)) &0xff));

				printf("error Unequal data\r\n");
			}
		}

//		j += i;
//
//		i++;
//		if(i > READ_SIZE)
//		{
//			i = 1;
//		}
//
//	}
	printf("QPI_Read_Test End\r\n");
}



extern uint8_t QPI_Performance_Enhance;

void QPI_QUAD_3ADDR_Performance_Enhance_Mode_Test(void)
{
	volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0,value = 0;

	printf("QPI_QUAD_3ADDR_Performance_Enhance_Mode_Test Start\r\n");

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("QUAD_Performance_Enhance_Mode_Test Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);


	if((norParams.flashID&0xff) != 0x01)
	{
		if( ((norParams.flashID&0xff) != 0x9d) && ((norParams.flashID&0xff) != 0xc2) )
		{
			status = SetQuad(0x31, 0x35, 1);
		}
		else
		{
			status = SetQuad(0x01, 0x05, 6);
		}

		if(status != AL_SUCCESS)
		{
			printf("SetQuad error:%d\r\n",status);
			return status;
		}
	}

	if( ((norParams.flashID&0xff) != 0x9d) && ((norParams.flashID&0xff) != 0xc2) )
		Nor_SendCmd(ENABLE_QPI_CMD);
	else
		Nor_SendCmd(0x35);

	status = Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_32);

	if(status != AL_SUCCESS)
	{
		printf("QPI_Read_Test Csu_QspiSetMode error:%d\r\n",status);
		while(1);
	}

	Qspi_Disable(g_pdev);
	Qspi_TransType(g_pdev, SPI_TRANSFER_TYPE2);
	Qspi_WaitCycles(g_pdev,QPI_DUMMY-2);
	Qspi_AddrMode(g_pdev, 0x08); /* 32 addr*/
	Qspi_Enable(g_pdev);

	ReadCommand = 0xEB;
	QPI_Performance_Enhance = 1;
	firstCmdSend = 0;


//	for(j =0;j < 200;)
//	{

		j = 0;
		i =200;

		printf("Csu_QspiRead Start addr:%d\r\n", j);

		status = Csu_TestQspiRead(j, test_buf, i);
		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_TestQspiRead error:%d\r\n", status);
		}


		for(l = 0; l < i; l++)
		{

			temp1 = (j + l)/4;
			temp2 = (j + l)%4;

			if( test_buf[l] != ((temp1>>(temp2*8)) &0xff) )
			{
				printf("i:%d, j:%d \r\n",i, j);

				printf("test_buf[%d]:%x\r\n",l, test_buf[l]);
				printf("%x\r\n", ((temp1>>(temp2*8)) &0xff));

				printf("error Unequal data\r\n");
			}
		}

//		j += i;
//
//		i++;
//		if(i > READ_SIZE)
//		{
//			i = 1;
//		}
//
//	}

	QPI_Performance_Enhance = 0;
	printf("QPI_QUAD_3ADDR_Performance_Enhance_Mode_Test End\r\n");
}


void DUAL_3ADDR_Performance_Enhance_Mode_Test(void)
{
	volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0,value = 0;

	printf("DUAL_3ADDR_Performance_Enhance_Mode_Test Start\r\n");

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("DUAL_3ADDR_Performance_Enhance_Mode_Test Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);


	status = Csu_QspiSetMode(QSPI_WIDTH_X2, QSPI_ADDR_32);

	if(status != AL_SUCCESS)
	{
		printf("DUAL_3ADDR_Performance_Enhance_Mode_Test Csu_QspiSetMode error:%d\r\n",status);
		while(1);
	}

	Qspi_Disable(g_pdev);
	Qspi_WaitCycles(g_pdev,0);
	Qspi_AddrMode(g_pdev, 0x08); /* 32 addr*/
	Qspi_TransType(g_pdev, SPI_TRANSFER_TYPE1);
	Qspi_Enable(g_pdev);

	ReadCommand = 0xBB;
	QPI_Performance_Enhance = 1;

//	for(j =200;j < TEST_SIZE;)
//	{
	j = 0;
	i =200;

	printf("Csu_TestQspiRead Start addr:%d\r\n", j);

	status = Csu_TestQspiRead(j, test_buf, i);
	if(AL_SUCCESS != status)
	{
		j += i;
		printf("Csu_TestQspiRead error:%d\r\n", status);
	}


	for(l = 0; l < i; l++)
	{

		temp1 = (j + l)/4;
		temp2 = (j + l)%4;

		if( test_buf[l] != ((temp1>>(temp2*8)) &0xff) )
		{
			printf("i:%d, j:%d \r\n",i, j);

			printf("test_buf[%d]:%x\r\n",l, test_buf[l]);
			printf("%x\r\n", ((temp1>>(temp2*8)) &0xff));

			printf("error Unequal data\r\n");
		}
	}

	printf("DUAL_3ADDR_Performance_Enhance_Mode_Test End\r\n");

	QPI_Performance_Enhance = 0;
	firstCmdSend = 0;
//	}


}

















void QUAD_4ADDR_Performance_Enhance_Mode_Test(void)
{
	volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0,value = 0;

	printf("QUAD_4ADDR_Performance_Enhance_Mode_Test Start\r\n");

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("QUAD_Performance_Enhance_Mode_Test Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);


	if((norParams.flashID&0xff) != 0x01)
	{
		if( ((norParams.flashID&0xff) != 0x9d) && ((norParams.flashID&0xff) != 0xc2) )
		{
			status = SetQuad(0x31, 0x35, 1);
		}
		else
		{
			status = SetQuad(0x01, 0x05, 6);
		}

		if(status != AL_SUCCESS)
		{
			printf("SetQuad error:%d\r\n",status);
			return status;
		}
	}

	status = Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_24);

	if(status != AL_SUCCESS)
	{
		printf("QPI_Read_Test Csu_QspiSetMode error:%d\r\n",status);
		while(1);
	}

	Qspi_Disable(g_pdev);
	Qspi_TransType(g_pdev, SPI_TRANSFER_TYPE1);
	Qspi_WaitCycles(g_pdev,8);
	Qspi_AddrMode(g_pdev, 0xa); /* 40 addr */
	Qspi_Enable(g_pdev);

	ReadCommand = 0xEC;
	QPI_Performance_Enhance = 1;


//	for(j =0;j < 200;)
//	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);

		status = Csu_TestQspiRead(j, test_buf, i);
		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_TestQspiRead error:%d\r\n", status);
		}


		for(l = 0; l < i; l++)
		{

			temp1 = (j + l)/4;
			temp2 = (j + l)%4;

			if( test_buf[l] != ((temp1>>(temp2*8)) &0xff) )
			{
				printf("i:%d, j:%d \r\n",i, j);

				printf("test_buf[%d]:%x\r\n",l, test_buf[l]);
				printf("%x\r\n", ((temp1>>(temp2*8)) &0xff));

				printf("error Unequal data\r\n");
			}
		}

//		j += i;
//
//		i++;
//		if(i > READ_SIZE)
//		{
//			i = 1;
//		}
//
//	}

	printf("QUAD_4ADDR_Performance_Enhance_Mode_Test End\r\n");

	QPI_Performance_Enhance = 0;
	firstCmdSend = 0;
}


void DUAL_4ADDR_Performance_Enhance_Mode_Test(void)
{
	volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0,value = 0;

	printf("DUAL_4ADDR_Performance_Enhance_Mode_Test Start\r\n");

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("DUAL_Performance_Enhance_Mode_Test Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

	status = Csu_QspiSetMode(QSPI_WIDTH_X2, QSPI_ADDR_32);

	if(status != AL_SUCCESS)
	{
		printf("DUAL_Performance_Enhance_Mode_Test Csu_QspiSetMode error:%d\r\n",status);
		while(1);
	}

	Qspi_Disable(g_pdev);
	Qspi_WaitCycles(g_pdev,0);
	Qspi_AddrMode(g_pdev, 0xa); /* 40 addr */
	Qspi_TransType(g_pdev, SPI_TRANSFER_TYPE1);
	Qspi_Enable(g_pdev);

	ReadCommand = 0xBC;
	QPI_Performance_Enhance = 1;

//	for(j =200;j < TEST_SIZE;)
//	{
		j = 0;
		i =200;
		printf("Csu_QspiRead Start addr:%d\r\n", j);

		status = Csu_TestQspiRead(j, test_buf, i);
		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_TestQspiRead error:%d\r\n", status);
		}


		for(l = 0; l < i; l++)
		{

			temp1 = (j + l)/4;
			temp2 = (j + l)%4;

			if( test_buf[l] != ((temp1>>(temp2*8)) &0xff) )
			{
				printf("i:%d, j:%d \r\n",i, j);

				printf("test_buf[%d]:%x\r\n",l, test_buf[l]);
				printf("%x\r\n", ((temp1>>(temp2*8)) &0xff));

				printf("error Unequal data\r\n");
			}
		}

//		j += i;
//
//		i++;
//		if(i > READ_SIZE)
//		{
//			i = 1;
//		}
//	}
		printf("DUAL_4ADDR_Performance_Enhance_Mode_Test End\r\n");
		QPI_Performance_Enhance = 0;
		firstCmdSend = 0;
}



















void QPI_Deep_Power_Read_Test(void)
{
	volatile uint32_t status = 0;

	printf("QPI_Deep_Power_Read_Test Start\r\n");

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("QPI_Deep_Power_Read_Test Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

	if((norParams.flashID&0xff) != 0x01)
	{
		if( ((norParams.flashID&0xff) != 0x9d) && ((norParams.flashID&0xff) != 0xc2) )
		{
			status = SetQuad(0x31, 0x35, 1);
		}
		else
		{
			status = SetQuad(0x01, 0x05, 6);
		}

		if(status != AL_SUCCESS)
		{
			printf("SetQuad error:%d\r\n",status);
			return status;
		}
	}

	if( ((norParams.flashID&0xff) != 0x9d) && ((norParams.flashID&0xff) != 0xc2) )
		Nor_SendCmd(ENABLE_QPI_CMD);
	else
		Nor_SendCmd(0x35);

	status = Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_24);

	if(status != AL_SUCCESS)
	{
		printf("QPI_Read_Test Csu_QspiSetMode error:%d\r\n",status);
		while(1);
	}

	Nor_SendCmd(DEEP_POWER_DOWN_CMD);


}




void program_nor(void)
{
	volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0, value = 0;

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiInit2 error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

	printf("Qspi_ChipErase Start\r\n");

	EnableWrite();

	Qspi_ChipErase();

	status = Csu_QspiRead(0, test_buf, 100);
	if(AL_SUCCESS != status)
	{
		printf("!!!!!!!Csu_QspiRead error:%d\r\n", status);
	}

	printf("Qspi_ChipErase:");
	for(i = 0; i < 100; i++)
	{
		printf("%d ",test_buf[i]);
	}
	printf("\r\n");

	while(value*4 < norParams.flashSize)
	{
		printf("Test_QspiProgram Start addr:0x%x\r\n",value*4);

		for(k = value; value < (k+WRITE_SIZE); value++)
		{
			*((uint32_t *) (&test_buf[(value-k)*4])) = value;
		}



		if((k*4+WRITE_SIZE*4) < norParams.flashSize)
			status = Test_QspiProgram(k*4, test_buf, WRITE_SIZE*4);
		else
		{
			status = Test_QspiProgram(k*4, test_buf, norParams.flashSize-k*4);
		}

		if(AL_SUCCESS != status)
		{
			printf("Test_QspiProgram error:%d\r\n", status);
			while(1);
		}

		printf("Test_QspiProgram End\r\n");
	}
}



void main(void)
{
	volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0, value = 0;


   // Qspi_PinmuxInit();

	printf(" Nor mode Test Start \r\n");

#ifdef PROGRAM_AND_ERASE_PAGE

	if(*TEMP_DDR_3 != 123456)
	{
		program_nor();
		*TEMP_DDR_3 = 123456;
	}

#endif



/*
 *    [19:16] Quad Enable Sequence (for the device support SFDP 1.0 only)
 *      0 - Not needed
 *      1 - QE bit is at bit 6 in Status Register 1
 *      2 - QE bit is at bit1 in Status Register 2
 *      3 - QE bit is at bit7 in Status Register 2
 *      4 - QE bit is at bit1 in Status Register 2 and should be programmed by 0x31
 */



#ifdef QUAD

	QUAD_Read_Test();
	DataTest();

#endif




#ifdef QPI

	QPI_Read_Test();
	DataTest();

#endif



#ifdef QPI_QUAD_3ADDR_Performance_Enhance_Mode

	QPI_QUAD_3ADDR_Performance_Enhance_Mode_Test();
	DataTest();

#endif


#ifdef DUAL_3ADDR_Performance_Enhance_Mode

	DUAL_3ADDR_Performance_Enhance_Mode_Test();
	DataTest();


#endif

#ifdef QUAD_4ADDR_Performance_Enhance_Mode

	QUAD_4ADDR_Performance_Enhance_Mode_Test();
	DataTest();

#endif


#ifdef DUAL_4ADDR_Performance_Enhance_Mode

	DUAL_4ADDR_Performance_Enhance_Mode_Test();
	DataTest();


#endif

#ifdef DEEP_POWER_TEST

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

	Nor_SendCmd(DEEP_POWER_DOWN_CMD);
	DataTest();

#endif



#ifdef QPI_DEEP_POWER_TEST

	status = Csu_QspiInit(&norParams);
	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiInit error:%d\r\n",status);
		while(1);
	}

	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

	QPI_Deep_Power_Read_Test();
	DataTest();

#endif


//#define READ_PAGE 1
#ifdef READ_PAGE


	for(j =0;j < TEST_SIZE;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);

		status = Csu_QspiRead(j, test_buf, i);
		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
		}


		for(l = 0; l < i; l++)
		{

			temp1 = (j + l)/4;
			temp2 = (j + l)%4;

			if( test_buf[l] != ((temp1>>(temp2*8)) &0xff) )
			{
				printf("i:%d, j:%d \r\n",i, j);

				printf("test_buf[%d]:%x\r\n",l, test_buf[l]);
				printf("%x\r\n", ((temp1>>(temp2*8)) &0xff));

				printf("error Unequal data\r\n");
			}
		}

		j += i;

		i++;
		if(i > READ_SIZE)
		{
			i = 1;
		}

		printf("Csu_QspiRead End\r\n");
	}

#endif




    return 0;

}
