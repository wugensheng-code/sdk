#include "qspi_test_driver.h"

QspiParams norParams;


uint8_t ReadBuf[100] = {0};
uint8_t WriteBuf[100] = {0};
extern QSPI_CORE_HANDLE    *g_pdev;

#define TEMP_DDR_3 ((volatile uint32_t *)(0X6103E000UL+0X8UL))	//int_count_num

uint8_t __attribute__ ((aligned(32))) test_buf[88888] = {0};
#define READ_SIZE sizeof(test_buf)
#define WRITE_SIZE 10000

void main(void)
{
    volatile uint32_t status, i = 0, j = 0, l = 0, temp1 =0,temp2 = 0,k = 0, value = 0;


   // Qspi_PinmuxInit();

    printf("nor_apu_test start\r\n");


    status = Csu_QspiInit(&norParams);
    if(status != AL_SUCCESS)
    {
        printf("Csu_QspiInit error:%d\r\n",status);
        while(1);
    }

    printf("flashID:%x\r\n", norParams.flashID);
    printf("flashSize:0x%x\r\n", norParams.flashSize);
    printf("flashSize:%dMB\r\n", norParams.flashSize/1024/1024);

     //Qspi_ResetFlashMode0(g_pdev);

    Qspi_ReadStatusAndRemovePortection();


//    if((norParams.flashID&0xff) != 0x01)
//    {
//    	printf("Qspi_ResetFlashMode0\r\n");
//    	Qspi_ResetFlashMode0(g_pdev);
//    }
//    else
//    {
//		printf("Qspi_ResetFlashMode1\r\n");
//		Qspi_ResetFlashMode1(g_pdev);
//    }


#define CHIP_ERASE 1
#ifdef CHIP_ERASE

	if(*TEMP_DDR_3%4 == 0)
	{
		printf("Qspi_ChipErase Start\r\n");

		EnableWrite();

		*TEMP_DDR_3 = *TEMP_DDR_3 + 1;

		wdt_nor_init();

		Qspi_ChipErase();

		while(1);

	}

#endif




#define PROGRAM_PAGE 1
#ifdef PROGRAM_PAGE

	if(*TEMP_DDR_3%4 == 2)
	{

		printf("Test_QspiProgram Start\r\n");

		while(value*4 < norParams.flashSize)
		{

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
			if(*TEMP_DDR_3%4 == 2)
			{
				*TEMP_DDR_3 = *TEMP_DDR_3 + 1;

				wdt_nor_init();
			}

			if(AL_SUCCESS != status)
			{
				printf("Test_QspiProgram error:%d\r\n", status);
				while(1);
			}


		}

		printf("Test_QspiProgram End\r\n");

		while(1);

	}

#endif








#define READ_PAGE_ZERO 1
#ifdef READ_PAGE_ZERO

	if(*TEMP_DDR_3%4 == 1)
	{

		printf("Csu_QspiRead 0xff Start\r\n");

		status = Csu_QspiRead(0, test_buf, 100);

		if(AL_SUCCESS != status)
		{
			printf("Csu_QspiRead error!!!!!!!!!!!!!!!!:%d\r\n", status);
		}


		for(l = 0; l < 100; l++)
		{
			if(test_buf[l] != 0xff)
			{
				printf("test_buf[%d]:%d\r\n",l,test_buf[l]);
				printf("Csu_QspiRead data not equal 0xff error\r\n");
				break;
			}
		}

		printf("Csu_QspiRead 0xff End\r\n");

		*TEMP_DDR_3 = *TEMP_DDR_3 + 1;

		wdt_nor_init();
		while(1);
	}

#endif









#define READ_PAGE 1
#ifdef READ_PAGE

	if(*TEMP_DDR_3%4 == 3)
	{

		printf("Csu_QspiRead Start\r\n");

		j = 0;
		i = 100;

		status = Csu_QspiRead(j, test_buf, i);

		if(AL_SUCCESS != status)
		{
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

		printf("Csu_QspiRead End\r\n");

		*TEMP_DDR_3 = *TEMP_DDR_3 + 1;

		wdt_nor_init();
		while(1);

	}


#endif




    return 0;

}
