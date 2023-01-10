#include "qspi_test_driver.h"

QspiParams norParams;


uint8_t ReadBuf[100] = {0};
uint8_t WriteBuf[100] = {0};


#define TEMP_DDR_3 ((volatile uint32_t *)(0X6103E000UL+0X8UL))	//int_count_num

uint8_t __attribute__ ((aligned(32))) test_buf[88888] = {0};
#define READ_SIZE sizeof(test_buf)
#define WRITE_SIZE 2000

void main(void)
{
    volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0, value = 0;


   // Qspi_PinmuxInit();

    printf(" Nor Branch Test Start \r\n");


#ifdef NOR_ERROR_CODE_BRANCH_TEST

	printf(" wdt_nor_init \r\n");
	wdt_nor_init();


#endif

#ifdef NOR_OTHER_BRANCH_TEST

	printf(" wdt_nor_init \r\n");
	wdt_nor_init();


#endif


    status = Csu_QspiInit(&norParams);


    printf("flashID:%x\r\n", norParams.flashID);
    printf("flashSize:0x%x\r\n", norParams.flashSize);
    printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

    nor_printf("\r\n\r\n\r\n");
    nor_printf("Csu_QspiInit:  ");
	print_log();
	nor_printf("\r\n\r\n\r\n");

	if(status != AL_SUCCESS)
    {
        printf("Csu_QspiInit error:%d\r\n",status);
        while(1);
    }

	status = Csu_QspiInit(&norParams);


	printf("flashID:%x\r\n", norParams.flashID);
	printf("flashSize:0x%x\r\n", norParams.flashSize);
	printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

	nor_printf("\r\n\r\n\r\n");
	nor_printf("Csu_QspiInit:  ");
	print_log();
	nor_printf("\r\n\r\n\r\n");

	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiInit error:%d\r\n",status);
//        while(1);
	}

if(*TEMP_DDR_3 == 0)
{
	*TEMP_DDR_3 = 1;

	status = Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24);

    nor_printf("Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24)");
	print_log();
	nor_printf("\r\n\r\n\r\n");

	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiSetMode error:%d\r\n",status);
		while(1);

	}

	i = 1;

	for(j =0;j < WRITE_SIZE;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);
		if(j + i <norParams.flashSize)
			status = Csu_QspiRead(j, test_buf, i);
		else
		{
			i = norParams.flashSize - j;
			status = Csu_QspiRead(j, test_buf, i);
		}

		nor_printf("Csu_QspiRead:  ");
		print_log();
		nor_printf("\r\n\r\n\r\n");

		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
			while(1);
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

}








if(*TEMP_DDR_3 == 1)
{
	*TEMP_DDR_3 = 2;

	status = Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_32);

    nor_printf("Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_32);");
	print_log();
	nor_printf("\r\n\r\n\r\n");


	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiSetMode error:%d\r\n",status);
		while(1);

	}

	i = 1;

	for(j =0;j < WRITE_SIZE;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);
		if(j + i <norParams.flashSize)
			status = Csu_QspiRead(j, test_buf, i);
		else
		{
			i = norParams.flashSize - j;
			status = Csu_QspiRead(j, test_buf, i);
		}

		nor_printf("Csu_QspiRead:  ");
		print_log();
		nor_printf("\r\n\r\n\r\n");

		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
			while(1);
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

}








if(*TEMP_DDR_3 == 2)
{
	*TEMP_DDR_3 = 3;

	status = Csu_QspiSetMode(QSPI_WIDTH_X2, QSPI_ADDR_24);

    nor_printf("Csu_QspiSetMode(QSPI_WIDTH_X2, QSPI_ADDR_24)");
	print_log();
	nor_printf("\r\n\r\n\r\n");


	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiSetMode error:%d\r\n",status);
		while(1);

	}

	i = 1;

	for(j =0;j < WRITE_SIZE;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);
		if(j + i <norParams.flashSize)
			status = Csu_QspiRead(j, test_buf, i);
		else
		{
			i = norParams.flashSize - j;
			status = Csu_QspiRead(j, test_buf, i);
		}

		nor_printf("Csu_QspiRead:  ");
		print_log();
		nor_printf("\r\n\r\n\r\n");

		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
			while(1);
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
}







if(*TEMP_DDR_3 == 3)
{
	*TEMP_DDR_3 = 4;

	status = Csu_QspiSetMode(QSPI_WIDTH_X2, QSPI_ADDR_32);

    nor_printf("Csu_QspiSetMode(QSPI_WIDTH_X2, QSPI_ADDR_32)");
	print_log();
	nor_printf("\r\n\r\n\r\n");

	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiSetMode error:%d\r\n",status);
		while(1);

	}

	i = 1;

	for(j =0;j < WRITE_SIZE;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);
		if(j + i <norParams.flashSize)
			status = Csu_QspiRead(j, test_buf, i);
		else
		{
			i = norParams.flashSize - j;
			status = Csu_QspiRead(j, test_buf, i);
		}

		nor_printf("Csu_QspiRead:  ");
		print_log();
		nor_printf("\r\n\r\n\r\n");

		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
			while(1);
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

}





if(*TEMP_DDR_3 == 4)
{
	*TEMP_DDR_3 = 5;

	status = Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_24);

    nor_printf("Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_24)");
	print_log();
	nor_printf("\r\n\r\n\r\n");

	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiSetMode error:%d\r\n",status);
		while(1);

	}

	i = 1;

	for(j =0;j < WRITE_SIZE;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);
		if(j + i <norParams.flashSize)
			status = Csu_QspiRead(j, test_buf, i);
		else
		{
			i = norParams.flashSize - j;
			status = Csu_QspiRead(j, test_buf, i);
		}

		nor_printf("Csu_QspiRead:  ");
		print_log();
		nor_printf("\r\n\r\n\r\n");

		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
			while(1);
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

}




if(*TEMP_DDR_3 == 5)
{
	*TEMP_DDR_3 = 0;

	status = Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_32);

    nor_printf("Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_32)");
	print_log();
	nor_printf("\r\n\r\n\r\n");

	if(status != AL_SUCCESS)
	{
		printf("Csu_QspiSetMode error:%d\r\n",status);
		while(1);

	}

	i = 1;

	for(j =0;j < WRITE_SIZE;)
	{
		printf("Csu_QspiRead Start addr:%d\r\n", j);
		if(j + i <norParams.flashSize)
			status = Csu_QspiRead(j, test_buf, i);
		else
		{
			i = norParams.flashSize - j;
			status = Csu_QspiRead(j, test_buf, i);
		}

		nor_printf("Csu_QspiRead:  ");
		print_log();
		nor_printf("\r\n\r\n\r\n");

		if(AL_SUCCESS != status)
		{
			j += i;
			printf("Csu_QspiRead error:%d\r\n", status);
			while(1);
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

}






	printf(" end \r\n");
	while(1);




    return 0;

}
