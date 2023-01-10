#include "qspi_test_driver.h"

QspiParams norParams;


uint8_t ReadBuf[100] = {0};
uint8_t WriteBuf[100] = {0};


#define TEMP_DDR_3 ((volatile uint32_t *)(0X6103E000UL+0X8UL))	//int_count_num

uint8_t __attribute__ ((aligned(32))) test_buf[88888] = {0};
#define READ_SIZE sizeof(test_buf)
#define WRITE_SIZE 10000

void main(void)
{
    volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0, value = 0;


   // Qspi_PinmuxInit();


    if(*TEMP_DDR_3 == 123456)
	{
    	printf(" wdt_nor_init \r\n");
		wdt_nor_init();
	}

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

    nor_printf("\r\n\r\n\r\n");
  	nor_printf("Csu_QspiInit:  ");
  	print_log();
  	nor_printf("\r\n\r\n\r\n");


    //*TEMP_DDR_3 = 123456;

#define PROGRAM_AND_ERASE_PAGE 1
#ifdef PROGRAM_AND_ERASE_PAGE

	if(*TEMP_DDR_3 != 123456)
	{
//		*TEMP_DDR_3 = 123456;

		printf("Qspi_ChipErase Start\r\n");

		EnableWrite();

		Qspi_ChipErase();

		status = Csu_QspiRead(0, test_buf, 100);
		if(AL_SUCCESS != status)
		{
			printf("!!!!!!!Csu_QspiRead error:%d\r\n", status);
		}

		nor_printf("\r\n\r\n\r\n");
		nor_printf("Csu_QspiRead:  ");
		print_log();
		nor_printf("\r\n\r\n\r\n");


		printf("Qspi_ChipErase:");
		for(i = 0; i < 100; i++)
		{
			printf("%d ",test_buf[i]);
		}
		printf("\r\n");

    //EnableWrite();
    //Qspi_ReadStatus1();

		while(value*4 < norParams.flashSize)
		{
			printf("Test_QspiProgram Start addr:0x%x\r\n",value*4);

			for(k = value; value < (k+WRITE_SIZE); value++)
			{
				*((uint32_t *) (&test_buf[(value-k)*4])) = value;
			}

	//    	printf("offset:%d length:%d \r\n",k*4, WRITE_SIZE*4);

			if((k*4+WRITE_SIZE*4) < norParams.flashSize)
				status = Test_QspiProgram(k*4, test_buf, WRITE_SIZE*4);
			else
			{
				status = Test_QspiProgram(k*4, test_buf, norParams.flashSize-k*4);
			//	printf("k*4:%d norParams.flashSize-k*4:%d\r\n",(k*4),(norParams.flashSize-k*4));
//				printf("test_buf:%d\r\n", test_buf[norParams.flashSize-k*4-2]);
//				printf("test_buf:%d\r\n", test_buf[norParams.flashSize-k*4-1]);
//				printf("test_buf:%d\r\n", test_buf[norParams.flashSize-k*4-0]);

			//	Mtimer_Delay(35000);
			}

//#define ERROR_POS 15655456
//			if(k*4 > ERROR_POS)
//			{
//				printf(" error pos test \r\n");
//				//printf("k*4:%d norParams.flashSize-k*4:%d\r\n",(k*4),(norParams.flashSize-k*4));
//				status = Csu_QspiRead(ERROR_POS, test_buf, 10000);
//				if(AL_SUCCESS != status)
//				{
//					printf("!!!!!!!Csu_QspiRead error:%d\r\n", status);
//				}
//
//				for(l = 0; l < 10000; l++)
//				{
//
//					temp1 = (ERROR_POS+ l)/4;
//					temp2 = (ERROR_POS+ l)%4;
//
//					if( test_buf[l] != ((temp1>>(temp2*8)) &0xff) )
//					{
//						printf("i:%d, j:%d \r\n",i, j);
//
//						printf("test_buf[%d]:%x\r\n",l, test_buf[l]);
//						printf("%x\r\n", ((temp1>>(temp2*8)) &0xff));
//
//						printf("error Unequal data\r\n");
//					}
//				}
//			}


//			printf("test_buf:");
//			for(i = 0; i < WRITE_SIZE*4; i++)
//			{
//				printf("%d ",test_buf[i]);
//			}
//			printf("\r\n");

//			if((k*4+WRITE_SIZE*4 > 0xeeee00) && (k*4 < 0xeeee00))
//			{
//				printf("test_buf:");
//				for(l = 0xeeee00-k*4; l < 0xeeee00-k*4+200; l++)
//				{
//						printf("%x ", test_buf[l]);
//				}
//				printf("\r\n");
//			}


	//    	status = Test_QspiProgram(0, test_buf, 500);
			if(AL_SUCCESS != status)
			{
				printf("Test_QspiProgram error:%d\r\n", status);
				while(1);
			}

			printf("Test_QspiProgram End\r\n");
		}
	}


#endif



//#define MICRN_NOR 1
#ifdef MICRN_NOR
	printf("Qspi_ChipErase Start\r\n");

	EnableWrite();

	Qspi_ChipErase();
//
	for(i=0;i<6385;i++)
	{
		test_buf[i] = 0;
	}

	test_buf[5638-1] = 88;

	printf("Test_QspiProgram Start\r\n");
	status = Test_QspiProgram(16771578, test_buf, 5638);
	printf("Test_QspiProgram End\r\n");

	if(AL_SUCCESS != status)
	{
		printf("Test_QspiProgram error:%d\r\n", status);
		while(1);
	}

	printf("Micorn_Test Start\r\n");
	Micorn_Test();
	printf("Micorn_Test End\r\n");

	Mtimer_Delay(35000);

	status = Csu_QspiRead(16771578, test_buf, 5638);
	if(AL_SUCCESS != status)
	{
		printf("!!!!!!!Csu_QspiRead error:%d\r\n", status);
	}

	printf("test_buf:");
	for(i=5638-50;i<5638;i++)
	{
		printf("%d ", test_buf[i]);
	}
	printf("\r\n");

	while(1);
#endif


//#define TEST_CODE 1
#ifdef TEST_CODE

#define TEST_WRITE_SIZE 20000

	printf("Qspi_ChipErase Start\r\n");

	EnableWrite();

	Qspi_ChipErase();

	value =15654810/4;
	printf("Test_QspiProgram Start addr:0x%x\r\n",value);

	for(k = value; value < (k+TEST_WRITE_SIZE); value++)
	{
		*((uint32_t *) (&test_buf[(value-k)*4])) = value;
	}

//    	printf("offset:%d length:%d \r\n",k*4, WRITE_SIZE*4);

	status = Test_QspiProgram(k*4, test_buf, TEST_WRITE_SIZE*4);
//    	status = Test_QspiProgram(0, test_buf, 500);
	if(AL_SUCCESS != status)
	{
		printf("Test_QspiProgram error:%d\r\n", status);
		while(1);
	}

	printf("Test_QspiProgram End\r\n");


	status = Csu_QspiRead(0xeeee00, test_buf, 50);
	if(AL_SUCCESS != status)
	{
		printf("Csu_QspiRead error:%d\r\n", status);
	}

	printf("test_buf:");
	for(l = 0; l < 50; l++)
	{
			printf("%x ", test_buf[l]);
	}
	printf("\r\n");

	i = 5596;
	j = 15654810;
	printf("Csu_QspiRead Start addr:%d\r\n", j);

	status = Csu_QspiRead(j, test_buf, i);
	if(AL_SUCCESS != status)
	{
		j += i;
		printf("Csu_QspiRead error:%d\r\n", status);
	}

//	printf("test_buf[0]:%x ", test_buf[0]);
	//		printf("data:");
	//		for(k = 0;k < i;k++)
	//		{
	//			printf("%d ", test_buf[k]);
	//		}
	//		printf("\r\n");


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

	while(1);

#endif

#define READ_PAGE 1
#ifdef READ_PAGE

//    status = Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_32);
//    if(AL_SUCCESS != status)
//   	{
//       	printf("Csu_QspiSetMode error:%d\r\n", status);
//       	while(1);
//   	}

   // WriteTest();



	for(j =0;j < norParams.flashSize;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);

		if(j + i <norParams.flashSize)
			status = Csu_QspiRead(j, test_buf, i);
		else
		{
			i = norParams.flashSize - j;
			status = Csu_QspiRead(j, test_buf, i);
		}

		nor_printf("\r\n\r\n\r\n");
		nor_printf("Csu_QspiRead:  ");
		print_log();
		nor_printf("\r\n\r\n\r\n");

		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
		}


//		printf("data:");
//		for(k = 0;k < i;k++)
//		{
//			printf("%d ", test_buf[k]);
//		}
//		printf("\r\n");


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





//	status = Csu_QspiRead(0, test_buf, WRITE_SIZE);
//	if(AL_SUCCESS != status)
//	{
//		printf("Csu_QspiRead error:%d\r\n", status);
//	}
//
//	printf("data:");
//	for(i = 0;i < WRITE_SIZE;i++)
//	{
//		printf("%d ", test_buf[i]);
//	}
//	printf("\r\n");




#endif
	if(*TEMP_DDR_3 != 123456)
	{
		printf(" wdt_nor_init \r\n");
		wdt_nor_init();
	}

	*TEMP_DDR_3 = 123456;

	printf(" end \r\n");
	while(1);




    return 0;

}
