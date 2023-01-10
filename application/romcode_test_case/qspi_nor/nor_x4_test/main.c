#include "qspi_test_driver.h"

QspiParams norParams;


uint8_t ReadBuf[100] = {0};
uint8_t WriteBuf[100] = {0};


#define TEMP_DDR_3 ((volatile uint32_t *)(0X6103E000UL+0X8UL))	//int_count_num


#define READ_SIZE sizeof(test_buf)



#ifndef NOR_BRANCH_PUT_CHAR
uint8_t __attribute__ ((aligned(32))) test_buf[88888] = {0};
#else
uint8_t __attribute__ ((aligned(32))) test_buf[8888] = {0};
#endif


#define READ_SIZE sizeof(test_buf)

#ifndef NOR_BRANCH_PUT_CHAR
#define WRITE_SIZE 10000
#else
#define WRITE_SIZE 1000
#endif


void main(void)
{
    volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0, value = 0;


   // Qspi_PinmuxInit();

    printf(" Nor X4 mode Test Start \r\n");


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
#ifndef NOR_BRANCH_PUT_CHAR
#define PROGRAM_AND_ERASE_PAGE 1
#endif

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

			nor_printf("\r\n\r\n\r\n");
			nor_printf("Test_QspiProgram:  ");
			print_log();
			nor_printf("\r\n\r\n\r\n");

			if(AL_SUCCESS != status)
			{
				printf("Test_QspiProgram error:%d\r\n", status);
				while(1);
			}

			printf("Test_QspiProgram End\r\n");
		}
	}

#endif



#define READ_PAGE 1
#ifdef READ_PAGE

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
		printf("Csu_QspiSetMode error:%d\r\n",status);
		while(1);
	}

	nor_printf("\r\n\r\n\r\n");
	nor_printf("Csu_QspiSetMode:  ");
	print_log();
	nor_printf("\r\n\r\n\r\n");





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
