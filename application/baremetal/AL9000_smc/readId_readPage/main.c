/*
 * main.c
 *
 *  Created on: 2022年6月7日
 *      Author: jian.huang
 */
#include "nand_drv.h"
#include "smc_drv.h"










Nand_TypeDef nand;

#define READ_SIZE 2048

uint8_t Buf[READ_SIZE*2-887]={1,2,3,5};



int main(void)
{




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
		/*SMC_WriteReg(PS_MIO15, 0x02);
		SMC_WriteReg(PS_MIO16, 0x02);
		SMC_WriteReg(PS_MIO17, 0x02);
		SMC_WriteReg(PS_MIO18, 0x02);

		SMC_WriteReg(PS_MIO19, 0x02);



		SMC_WriteReg(PS_MIO20, 0x02);
		SMC_WriteReg(PS_MIO21, 0x02);
		SMC_WriteReg(PS_MIO22, 0x02);
		SMC_WriteReg(PS_MIO23, 0x02);
	*/




/*
	for(j=0; j<17; j++)
	{
		for(i=0; i<256; i++)
		{
			Buf[j*256+i]	= i;
		}
	}
*/
	volatile uint32_t i=0,j=0,temp=0;
	uint8_t Status = 0;

 	/* Set Nand Timing Efuse */
    *(volatile uint32_t *)(0xF8801000ULL + 0x150ULL) =0x0024ABCCULL;
	*(volatile uint32_t *)(0xF8801000ULL + 0x154ULL) =0x0024ABCCULL;

	printf("G\r\n");

	Status = Csu_NandInit(&nand);

	if(Status != SmcSuccess)
	{
		printf("init error:%d\r\n",Status);
		return Status;
	}

	Status = Csu_NandRead(888, Buf, READ_SIZE*2-887, &nand);
	if(Status != SmcSuccess)
	{
		printf("Read error\r\n");
	}

	for(i = 0; i < READ_SIZE*2-887; i++)
	{
		printf(" %d",Buf[i]);
	}

	//printf(" %d",temp);


	printf(" end \r\n");
	return 0;
}

