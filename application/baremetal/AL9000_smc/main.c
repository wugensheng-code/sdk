#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "Onfi.h"
#include "al9000_smc.h"



Nand_Size_TypeDef nandSize;
uint8_t Buf[5000];


typedef struct{
	uint8_t ecc_read:5;
	uint8_t ecc_can_correct:5;
	uint8_t ecc_fail:5;
	uint8_t ecc_value_valid:5;
	uint8_t ecc_rd_n_wr:1;
	uint8_t ecc_last_status:2;
	uint8_t ecc_status:1;
	uint8_t ecc_int_status:6;
}test;

#define TEST ((test *)(SMC_BASE+SMC_REG_ECC1_STATUS))


volatile int count=0;

void SmcHandler(void)
{
	//printf("e\r\n");
	/* smc busy interrupt*/
	if(*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEMC_STATUS)) &
			(1 << SMC_MemcStatus_SmcInt1Status_FIELD))
	{
		printf("interrupt\r\n");



		while(!(*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEMC_STATUS))  & (1 << SMC_MemcStatus_SmcInt1RawStatus_FIELD)))
		{
			count++;
			printf("ccc:%d  \r\n",count);
		}
		printf("interrupt\r\n");
	}
	/* smc ecc interrupt*/
	else if(*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEMC_STATUS)) &
			(1 << SMC_MemcStatus_EccInt1Status_FIELD))
	{

		printf("k\r\n");
		printf("S:%d  \r\n",*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEMC_STATUS)));
	}

	/* clear interrupt flag */
	*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEM_CFG_CLR)) |= SMC_MemCfgClr_ClrSmcInt1;
}



int main(void)
{


#if 0

		uint8_t device_id[5]={0};

		//*((uint32_t *)(SMC_BASE+SMC_REG_MEM_CFG_CLR)) = SMC_MemCfgClr_SmcInt1_DISABLE | SMC_MemCfgClr_ClrSmcInt1 | SMC_MemCfgClr_EccInt1_DISENABLE;
		*((volatile uint32_t *)(SMC_BASE+SMC_REG_SET_CYCLES)) =
				((2 <<	SMC_SetCycles_Trr_FIELD		)|		//busy to data out
				(1 <<	SMC_SetCycles_Tar_FIELD   	)|		//ale to data out
				(1 << 	SMC_SetCycles_Tclr_FIELD 	)|		//cle to data out
				(2 << 	SMC_SetCycles_Twp_FIELD    	)|	 //we width
				(1 << 	SMC_SetCycles_Trea_FIELD    )|	//RE before output data
				(3 <<	SMC_SetCycles_Twc_FIELD     )|	//WE (cmd/addr) width
				(3 << 	SMC_SetCycles_Trc_FIELD		));	//RE width


		//*((volatile uint32_t *)(SMC_BASE+SMC_REG_DIRCT_CMD)) = SMC_DirectCmd_SelChip_Interface1Chip1 | SMC_DirectCmd_CmdType_UpdateRegsAndAXI;


		Onfi_CmdReset();
		while(1)
		{
		Onfi_CmdReadId(0x00, &device_id[0], 5);

		printf("%x %x %x %x %x\r\n",device_id[0],device_id[1],device_id[2],device_id[3],device_id[4]);


		}
#endif


#if 0



		volatile uint32_t i=0,j=0;
		uint8_t temp[4]={20,22,05,19};

		*((volatile uint32_t *)(SMC_BASE+SMC_REG_SET_CYCLES)) =
						((0 <<	SMC_SetCycles_Trr_FIELD		)|		//busy to data out
						(0 <<	SMC_SetCycles_Tar_FIELD   	)|		//ale to data out
						(0 << 	SMC_SetCycles_Tclr_FIELD 	)|		//cle to data out
						(1 << 	SMC_SetCycles_Twp_FIELD    	)|	 //we width
						(1 << 	SMC_SetCycles_Trea_FIELD    )|	//RE before output data
						(2 <<	SMC_SetCycles_Twc_FIELD     )|	//WE (cmd/addr) width
						(2 << 	SMC_SetCycles_Trc_FIELD		));	//RE width

		for(j=0; j<17; j++)
		{
			for(i=0; i<256; i++)
			{
				Buf[j*256+i]	= i;
			}
		}

		/*
		for(i=0; i<224; i++)
		{
			Buf[i+4096]	= 0xff;
		}
		*/


		*((volatile uint32_t *)(SMC_BASE+SMC_REG_DIRCT_CMD)) = SMC_DirectCmd_SelChip_Interface1Chip1 | SMC_DirectCmd_CmdType_UpdateRegsAndAXI;

		Onfi_CmdReset();


		Onfi_CmdReadParameter(&nandSize);
		printf("blocksPerUint:%d dataBytesPerPage:%d pagesPerBlock:%d spareBytesPerPage:%d \r\n",
		nandSize.blocksPerUint,nandSize.dataBytesPerPage,nandSize.pagesPerBlock,nandSize.spareBytesPerPage);


		Onfi_CmdBlockErase(0);
		//Onfi_CmdBlockErase(1);
		//Onfi_CmdBlockErase(2);
		//Onfi_CmdBlockErase(3);


		Nand_ProgramPage(0, 0, Buf, &nandSize);

		Nand_ReadPage(0, 0, Buf, &nandSize);
		for(i=0; i<1024*4+224; i++)
		{
			printf("%d ",Buf[i]);
		}
		printf("\r\n");
		printf("\r\n");
		printf("\r\n");
		printf("\r\n");
		printf("\r\n");


		//Onfi_CmdBlockErase(0);
		//Nand_ReadPage(0, 0, Buf, &nandSize);
		//for(i=0; i<1024*4+224; i++)
		//{
			//printf("%d ",Buf[i]);
		//}
		//printf("\r\n");
#endif

#if 0

		volatile uint32_t i=0,j=0;
		/*
		*((volatile uint32_t *)(SMC_BASE+SMC_REG_SET_CYCLES)) =
						((0 <<	SMC_SetCycles_Trr_FIELD		)|		//busy to data out
						(0 <<	SMC_SetCycles_Tar_FIELD   	)|		//ale to data out
						(0 << 	SMC_SetCycles_Tclr_FIELD 	)|		//cle to data out
						(2 << 	SMC_SetCycles_Twp_FIELD    	)|	 //we width
						(1 << 	SMC_SetCycles_Trea_FIELD    )|	//RE before output data
						(3 <<	SMC_SetCycles_Twc_FIELD     )|	//WE (cmd/addr) width
						(3 << 	SMC_SetCycles_Trc_FIELD		));	//RE width


		*((volatile uint32_t *)(SMC_BASE+SMC_REG_DIRCT_CMD)) = SMC_DirectCmd_SelChip_Interface1Chip1 | SMC_DirectCmd_CmdType_UpdateRegsAndAXI;
*/
		Onfi_CmdReset();

		Onfi_CmdReadParameter(&nandSize);
		printf("blocksPerUint:%d dataBytesPerPage:%d pagesPerBlock:%d spareBytesPerPage:%d \r\n",
		nandSize.blocksPerUint,nandSize.dataBytesPerPage,nandSize.pagesPerBlock,nandSize.spareBytesPerPage);


		for(j=0;j<2;j++)
		{
			printf("start\r\n\r\n\r\n");
			Nand_ReadPage(j, 0, Buf, &nandSize);
			for(i=0; i<1024*4+224; i++)
			{
				printf("%d ",Buf[i]);
			}
			printf("\r\n\r\n\r\n");
		}
#endif

#if 0
		volatile uint64_t i=0;
		volatile uint64_t j=0;
		unsigned long p;
		volatile uint8_t status=0;


		Onfi_CmdReset();

		Onfi_CmdReadParameter(&nandSize);
		printf("blocksPerUint:%d dataBytesPerPage:%d pagesPerBlock:%d spareBytesPerPage:%d \r\n",
		nandSize.blocksPerUint,nandSize.dataBytesPerPage,nandSize.pagesPerBlock,nandSize.spareBytesPerPage);

		//if(FAILED == Onfi_CmdBlockErase(0))
		//{
		//	printf("ERROR: bad block\r\n");
		//}


		printf("/***********   A row represents a block   ***********/");
		printf("\r");
		printf("/* Two carriage returns in the middle of each plane */");
		printf("\r");
		printf("/* '*' represent good block,'!' represent bad block */");
		printf("\r\n");

		printf("pppp:%d\r\n",sizeof(p));


		printf("\r\n");
/*
		Nand_ReadPage(15, 0, Buf, &nandSize);
		for(i=0; i<1024*4+224; i++)
		{
			printf("%d ",Buf[i]);
		}
		printf("\r\n");

		Nand_ReadPage(17, 0, Buf, &nandSize);
		for(i=0; i<1024*4+224; i++)
		{
			printf("%d ",Buf[i]);
		}
		printf("\r\n");
*/
		for(j=0; j<nandSize.blocksPerUint; j++)
		{
			//for(i=0; i<nandSize.pagesPerBlock; i++)
			//{
			status = Nand_CheakIsBadBlock((j>>10), (j<<22)&0xffffffff, Buf, &nandSize);
				if(status == BAD_BLOCK)
				{
					printf("j:%d! \r\n",j*nandSize.pagesPerBlock);
				}
				else if(status == FAILED)
				{
					printf("FAILD \r\n");
				}
				else
				{
					//printf("* \r\n");
				}
			//}
			//printf("1\r\n");
		}

		printf("\r\n");
#endif




#if 0


		volatile uint32_t i=0,j=0;
		uint8_t temp[4]={20,22,05,19},status;

		*((volatile uint32_t *)(SMC_BASE+SMC_REG_SET_CYCLES)) =
						((0 <<	SMC_SetCycles_Trr_FIELD		)|		//busy to data out
						(0 <<	SMC_SetCycles_Tar_FIELD   	)|		//ale to data out
						(0 << 	SMC_SetCycles_Tclr_FIELD 	)|		//cle to data out
						(1 << 	SMC_SetCycles_Twp_FIELD    	)|	 //we width
						(1 << 	SMC_SetCycles_Trea_FIELD    )|	//RE before output data
						(2 <<	SMC_SetCycles_Twc_FIELD     )|	//WE (cmd/addr) width
						(2 << 	SMC_SetCycles_Trc_FIELD		));	//RE width
/*
		for(j=0; j<17; j++)
		{
			for(i=0; i<256; i++)
			{
				Buf[j*256+i]	= i;
			}
		}


		for(i=0; i<224; i++)
		{
			Buf[i+4096]	= 0xff;
		}
		*/
/**/
		for(i=0; i<512*2; i++)
		{
			Buf[i*4]	= temp[0];
			Buf[i*4+1]	= temp[1];
			Buf[i*4+2]	= temp[2];
			Buf[i*4+3]	= temp[3];
		}

		for(; i<1024+56; i++)
		{
			Buf[i*4]	= 0xff;
			Buf[i*4+1]	= 0xff;
			Buf[i*4+2]	= 0xff;
			Buf[i*4+3]	= 0xff;
		}


		*((volatile uint32_t *)(SMC_BASE+SMC_REG_DIRCT_CMD)) = SMC_DirectCmd_SelChip_Interface1Chip1 | SMC_DirectCmd_CmdType_UpdateRegsAndAXI;

		Onfi_CmdReset();


		Onfi_CmdReadParameter(&nandSize);
		printf("blocksPerUint:%d dataBytesPerPage:%d pagesPerBlock:%d spareBytesPerPage:%d \r\n",
		nandSize.blocksPerUint,nandSize.dataBytesPerPage,nandSize.pagesPerBlock,nandSize.spareBytesPerPage);

		/*status = Nand_EccHwInit(&nandSize);
		if(status == FAILED)
		{
			printf("EccHwInit error\r\n");
		}*/

		Onfi_CmdBlockErase(0);
		//Onfi_CmdBlockErase(1);
		//Onfi_CmdBlockErase(2);
		//Onfi_CmdBlockErase(3);

		Onfi_CmdProgramPage( 0,  0);
		status = Nand_ProgramPage(0, 0, Buf, &nandSize);
		if(status == FAILED)
		{
			printf("Program error\r\n");
		}

		for(j=0; j<17; j++)
		{
			for(i=0; i<256; i++)
			{
				Buf[j*256+i]	= i;
			}
		}


		for(i=0; i<224; i++)
		{
			Buf[i+4096]	= 0xff;
		}

		Onfi_CmdProgramCachePage( 0,  1<<16);
		status = Nand_ProgramPage(0, 0, Buf, &nandSize);
		if(status == FAILED)
		{
			printf("Program error\r\n");
		}




		Nand_ReadPage(0, 0, Buf, &nandSize);
		for(i=0; i<1024*4+224; i++)
		{
			printf("%d ",Buf[i]);
		}
		printf("\r\n");
		printf("\r\n");
		printf("\r\n");
		printf("\r\n");
		printf("\r\n");

		Nand_ReadPage(0, 1<<16, Buf, &nandSize);
		for(i=0; i<1024*4+224; i++)
		{
			printf("%d ",Buf[i]);
		}
		printf("\r\n");
		printf("\r\n");
		printf("\r\n");



		//Onfi_CmdBlockErase(0);
		//Nand_ReadPage(0, 0, Buf, &nandSize);
		//for(i=0; i<1024*4+224; i++)
		//{
			//printf("%d ",Buf[i]);
		//}
		//printf("\r\n");
#endif


#if 0
	while(1)
	{
		SmcSendCommand(ONFI_CMD_READ_PAGE1, ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_CYCLES,
						ONFI_CMD_READ_PAGE_END_TIMING, 0x9abcde, 0x12345678);
	}

#endif

#if 0
	volatile uint32_t i=0,j=0;

	*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEM_CFG_SET)) |= SMC_MemCfgSet_EccInt1_ENABLE|
			SMC_MemCfgSet_SmcInt1_ENABLE;

	ECLIC_Register_IRQ(SMC_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1, SmcHandler);
	__enable_irq();

	printf("S:%d  \r\n",*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEMC_STATUS)));


		Onfi_CmdReset();
		/*Onfi_CmdReadParameter(&nandSize);
		printf("blocksPerUint:%d dataBytesPerPage:%d pagesPerBlock:%d spareBytesPerPage:%d \r\n",
		nandSize.blocksPerUint,nandSize.dataBytesPerPage,nandSize.pagesPerBlock,nandSize.spareBytesPerPage);
*/


/*
		for(j=0;j<2;j++)
		{
			printf("start\r\n\r\n\r\n");
			Nand_ReadPage(j, 0, Buf, &nandSize);
			for(i=0; i<1024*4+224; i++)
			{
				printf("%d ",Buf[i]);
			}
			printf("\r\n\r\n\r\n");
		}
		*/
	printf("end\r\n");
	printf("ccc:%d  \r\n",count);
	while(1);

#endif

#if 1

	TEST->ecc_can_correct = 1;
	TEST->ecc_int_status = 1;
	TEST->ecc_last_status = 1;
	TEST->ecc_rd_n_wr = 1;
	TEST->ecc_read = 1;
	TEST->ecc_status = 1;
	TEST->ecc_value_valid = 1;

	printf("%d\r\n",*((uint32_t *)SMC_BASE+SMC_REG_ECC1_STATUS));

#endif

#if 1

#define GIT_BIT
#define SET_BIT

/* reg[end,start] = value */
#define GIT_BITS(start, end, value, reg) ((reg >> start)&(0xffffffff >> (32-(end-start)))

#define SET_BITS

#define CLEAR_BITS

#endif


	return 0;
}





