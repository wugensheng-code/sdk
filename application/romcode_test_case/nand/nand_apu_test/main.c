/*
 * main.c
 *
 *  Created on: 2022年6月7日
 *      Author: jian.huang
 */
#include "nand_drv.h"
#include "smc_drv.h"
#include "onfi.h"



#define SATRT_BLOCK_ADDR 0
#define END_BLOCK_ADDR (nand.blocksPerUnit *nand.totalUnit)


//extern uint8_t SpareBuf[];
extern uint8_t SpareData[512];

Nand_TypeDef nand;

#define TEMP_DDR_4 ((volatile uint32_t *)(0X6103E000UL+0X10UL))	//int_count_num

uint8_t __attribute__((aligned(4))) Buf[4096]={20,22,11,6};
uint8_t __attribute__((aligned(4))) Buf1[4096]={0};

#define READ_MAX_SIZE sizeof(Buf1)
//#define








int main(void)
{
//	branch_put_char("12789->");
//	branch_put_char("branch path: (1) -> ");
//	branch_put_char("(2) -> ");
//	printf("char_ptr:%s",branch_log_array);

#define PIN_MUX_BASE	(0xF8803000UL)

#define PS_MIO0			(0x00+PIN_MUX_BASE)
#define PS_MIO1			(0x04+PIN_MUX_BASE)
#define PS_MIO2			(0x08+PIN_MUX_BASE)
#define PS_MIO3			(0x0c+PIN_MUX_BASE)
#define PS_MIO4			(0x10+PIN_MUX_BASE)
#define PS_MIO5			(0x14+PIN_MUX_BASE)
#define PS_MIO6			(0x18+PIN_MUX_BASE)
#define PS_MIO7			(0x1c+PIN_MUX_BASE)
#define PS_MIO8			(0x20+PIN_MUX_BASE)
#define PS_MIO9			(0x24+PIN_MUX_BASE)
#define PS_MIO10		(0x28+PIN_MUX_BASE)
#define PS_MIO11		(0x2c+PIN_MUX_BASE)
#define PS_MIO12		(0x30+PIN_MUX_BASE)
#define PS_MIO13		(0x34+PIN_MUX_BASE)
#define PS_MIO14		(0x38+PIN_MUX_BASE)
#define PS_MIO15		(0x3c+PIN_MUX_BASE)
#define PS_MIO16		(0x40+PIN_MUX_BASE)
#define PS_MIO17		(0x44+PIN_MUX_BASE)
#define PS_MIO18		(0x48+PIN_MUX_BASE)
#define PS_MIO19		(0x4c+PIN_MUX_BASE)
#define PS_MIO20		(0x50+PIN_MUX_BASE)
#define PS_MIO21		(0x54+PIN_MUX_BASE)
#define PS_MIO22		(0x58+PIN_MUX_BASE)
#define PS_MIO23		(0x5c+PIN_MUX_BASE)
#define PS_MIO24		(0x60+PIN_MUX_BASE)
#define PS_MIO25		(0x64+PIN_MUX_BASE)
#define PS_MIO26		(0x68+PIN_MUX_BASE)
#define PS_MIO27		(0x6c+PIN_MUX_BASE)
#define PS_MIO28		(0x70+PIN_MUX_BASE)

	SMC_WriteReg(PS_MIO0, 0x02);
		//SMC_WriteReg(PS_MIO1, 0x02);
		SMC_WriteReg(PS_MIO2, 0x02);
		SMC_WriteReg(PS_MIO3, 0x02);
		SMC_WriteReg(PS_MIO4, 0x02);
		SMC_WriteReg(PS_MIO5, 0x02);
		SMC_WriteReg(PS_MIO6, 0x02);
		SMC_WriteReg(PS_MIO7, 0x02);
		SMC_WriteReg(PS_MIO8, 0x02);
		SMC_WriteReg(PS_MIO9, 0x02);

		SMC_WriteReg(PS_MIO10, 0x02);
		SMC_WriteReg(PS_MIO11, 0x02);
		SMC_WriteReg(PS_MIO12, 0x02);
		SMC_WriteReg(PS_MIO13, 0x02);
		SMC_WriteReg(PS_MIO14, 0x02);

	volatile uint32_t i=0,j=0, temp1 =0,temp2 = 0,status =0;
	volatile uint8_t Status = 0;
	uint8_t* bufPtr =Buf;
	volatile uint32_t k = 0,l = 0, m = 1;


	//printf("nand rpu test start.\r\n");

	Status = Csu_NandInit(&nand);
	if(Status != SmcSuccess)
	{
		printf("init error:%d\r\n",Status);
		while(1);
		return Status;
	}

	//printf("Csu_NandInit end\r\n");


	//*TEMP_DDR_4 = 123456;

	if((*TEMP_DDR_4%4) == 0)
	{
	//	printf("start erase block:%d\r\n",i);
		*TEMP_DDR_4 = *TEMP_DDR_4 + 1;
		wdt_apu_init();

		for(i = 0; i <= END_BLOCK_ADDR; i++)
		{

			Status = Onfi_CmdBlockErase(i*nand.pagesPerBlock);

			if(Status != SUCCESS)
			{
				printf("BlockErase error\r\n");
				printf("bad blcok num:%d !!!!!!!!!!!!!!!!!!!!\r\n",i);

				if(nand.eccNum == 1)
				{
					Status = Nand_EccHwDisable();

					if(Status != SmcSuccess)
					{
						return Status;
					}
				}

				SpareData[0] = 0x00;
				SpareData[1] = 0x00;

				Status = Onfi_CmdProgramPage((i*nand.pagesPerBlock), 0);

				if(Status != SUCCESS)
				{
					printf("signed bad block ProgramPageCmd error\r\n");
					while(1);
				}


				Status  = Nand_ProgramPage_WithOob(bufPtr, &nand);
				if(Status != SUCCESS)
				{
					printf("signed bad block ProgramPage_HwEcc error\r\n");
					while(1);
				}

				SpareData[0] = 0xff;
				SpareData[1] = 0xff;

				if(nand.eccNum == 1)
				{
					Status = Nand_EccHwInit(nand);
					if(Status != SmcSuccess)
					{
						return Status;
					}

				}

				continue;
			}
		}
	}
	else if((*TEMP_DDR_4%4) == 2)
	{
	//	printf("start program block\r\n");
		*TEMP_DDR_4 = *TEMP_DDR_4 + 1;
		wdt_apu_init();

		for(i = 0; i <= END_BLOCK_ADDR; i++)
		{
			for(j = 0;j < nand.pagesPerBlock; j++)
			{
				for(k = 0; k < (nand.dataBytesPerPage/4); k++)
				{
					*((uint32_t *) (&Buf[k*4])) = (((i*nand.pagesPerBlock) + j)*nand.dataBytesPerPage/4 + k);
				}

				Status = Onfi_CmdProgramPage(((i*nand.pagesPerBlock) + j), 0);

				if(Status != SUCCESS)
				{
					printf("ProgramPageCmd error\r\n");
					while(1);
				}

				if(nand.eccNum != 1)
				{
					Status  = Nand_ProgramPage_EOob(bufPtr, &nand);
					if(Status != SUCCESS)
					{
						printf("Nand_ProgramPage_WithOob error\r\n");
						while(1);
					}
				}
				else
				{
					Status  = Nand_ProgramPage_HwEcc(bufPtr, &nand);
					if(Status != SUCCESS)
					{
						printf("ProgramPage_HwEcc error\r\n");
						while(1);
					}
				}
			}
		}
	}

	else if(((*TEMP_DDR_4%4) == 1)||((*TEMP_DDR_4%4) == 3))
	{
	//	printf("start read\r\n");
		*TEMP_DDR_4 = *TEMP_DDR_4 + 1;
		wdt_apu_init();

		for(i = 0; i <= END_BLOCK_ADDR; i++)
		{
			for(j = 0;j < nand.pagesPerBlock*nand.dataBytesPerPage;)
			{
				Status = Csu_NandRead( (i*nand.pagesPerBlock)*nand.dataBytesPerPage+j , Buf1, m, &nand);
				if(Status != SmcSuccess)
				{
					j += m;
					printf("read error:%d\r\n",Status);
					continue;
				}

				printf("d:");
				for(l = 0; l < m; l++)
				{
					printf("%x ", Buf1[l]);
				}
				printf("\r\n");
				j += m;

				m++;
				if(m > READ_MAX_SIZE)
				{
					m = 1;
				}
			}
		}
	}






	//wdt_init();
	while(1);

	printf(" end \r\n");

	return 0;
}

