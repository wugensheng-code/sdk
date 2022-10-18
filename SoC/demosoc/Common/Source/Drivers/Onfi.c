#include "Onfi.h"




#define csu_printf do{}while(0);


static uint32_t __attribute__((aligned(4))) NandOob64[12] = {52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};
static uint32_t __attribute__((aligned(4))) NandOob16[3] = {13, 14, 15};
static uint32_t __attribute__((aligned(4))) NandOob32[6] = {26, 27, 28, 29, 30, 31};
static uint8_t Nand_IsBusy(void)
{
	uint32_t Status = ( SMC_ReadReg(SMC_BASE+SMC_REG_MEMC_STATUS) & (1 << SMC_MemcStatus_SmcInt1RawStatus_FIELD));

	//csu_csu_printf("bb\r\n");
	if(Status)
		return NAND_READY;
	else
		return NAND_BUSY;
}





static uint8_t Nand_HwCalculateEcc(uint8_t *Dst, uint8_t eccDataNums)
{
	uint8_t Count = 0;
	volatile uint8_t eccReg = 0;
	uint32_t eccValue = 0;

	/* Check busy signal if it is busy to poll*/
	while((SMC_ReadReg(SMC_BASE+SMC_REG_ECC1_STATUS) & (1 << SMC_EccStatus_EccStatus_FIELD)));

	for(eccReg=0; eccReg < eccDataNums/3; eccReg++)
	{
		eccValue = SMC_ReadReg(SMC_BASE+SMC_REG_ECC1_BLOCK0+eccReg*4);
		csu_printf("eccValue:%x\r\n",eccValue);

		if((eccValue) & (1 << SMC_EccBlock_ISCheckValueValid_FIELD))
		{
			for(Count=0; Count < 3; Count++)
			{
				*Dst = eccValue & 0xFF;
				csu_printf("Dst: %d\r\n",*Dst);
				eccValue = eccValue >> 8;
				Dst++;
			}
		}
		else
		{
			csu_printf("EccInvalidErr\r\n");
			return SmcEccDataInvalidErr;
		}
	}
	csu_printf("cal ecc end\r\n");

	return SmcSuccess;
}




static uint8_t Nand_HwCorrectEcc(uint8_t *eccCode, uint8_t *eccCalc, uint8_t *buf)
{
	uint8_t bitPos = 0;
	uint32_t bytePos = 0;
	uint16_t eccOdd = 0, eccEven = 0;
	uint16_t readEccLow = 0, readEccHigh = 0;
	uint16_t calcEccLow = 0, calcEccHigh = 0;

	/* Lower 12 bits of ECC Read */
	readEccLow = (eccCode[0] | (eccCode[1] << 8)) & 0xfff;

	/* Upper 12 bits of ECC Read */
	readEccHigh = ((eccCode[1] >> 4) | (eccCode[2] << 4)) & 0xfff;

	/* Lower 12 bits of ECC calculated */
	calcEccLow = (eccCalc[0] | (eccCalc[1] << 8)) & 0xfff;

	/* Upper 12 bits of ECC Calculated */
	calcEccHigh = ((eccCalc[1] >> 4) | (eccCalc[2] << 4)) & 0xfff;

	eccOdd = readEccLow ^ calcEccLow;
	eccEven = readEccHigh ^ calcEccHigh;

	/* No Error */
	if ((eccOdd == 0) && (eccEven == 0))
	{
		csu_printf("0 bit error\r\n");
		return SmcSuccess;
	}


	/* One bit Error */
	if (eccOdd == (~eccEven & 0xfff))
	{
		bytePos = (eccOdd >> 3) & 0x1ff;
		bitPos = eccOdd & 0x07;
		/* Toggling error bit */
		buf[bytePos] ^= (1 << bitPos);

		csu_printf("one bit error\r\n");
		csu_printf("bytePos:%u bitPos:%u \r\n",bytePos,bitPos);

		return SmcSuccess;
	}

	/* Two bits Error */
	if (OneHot((eccOdd | eccEven)) == SmcSuccess)
	{
		csu_printf("Two bits Error\r\n");
		return SmcTwoBitsErr;
	}

	csu_printf("Multiple bits error\r\n");
	/* Multiple bits error */
	return SmcMultipleBitsErr;

}



 uint8_t Nand_ReadPage_HwEcc(uint8_t *Buf, uint8_t *SpareBuf, Nand_TypeDef *nand)
{
	uint8_t * ptrBuf = Buf;
	uint8_t *TempBuf = Buf;
	uint8_t * TempSpareBuf = SpareBuf;
	uint8_t Status = 0,  eccDataNums = 0, EccOffset = 0;
	volatile uint32_t i = 0;
	uint8_t __attribute__((aligned(4))) eccData[12] = {0};
	uint8_t __attribute__((aligned(4))) ReadEccData[12] = {0};
	uint32_t *dataOffsetPtr = NULL;

	/* Read data Page */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, ptrBuf, nand->dataBytesPerPage-ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, NO_ECC_LAST);
	ptrBuf += (nand->dataBytesPerPage-ONFI_AXI_DATA_WIDTH);

	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, ptrBuf, ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, ECC_LAST);
	ptrBuf += ONFI_AXI_DATA_WIDTH;

	switch(nand->spareBytesPerPage)
	{
		case(64):
			eccDataNums = 12;
			dataOffsetPtr = NandOob64;
			break;
		case(32):
			eccDataNums = 6;
			dataOffsetPtr = NandOob32;
			break;
		case(16):
			eccDataNums = 3;
			dataOffsetPtr = NandOob16;
			break;
		default:
			/* Page size 256 bytes & 4096 bytes not supported by ECC block*/
			return SmcHwReadSizeOver;
	}

/*
	if((nand->spareBytesPerPage == 64)||
		(nand->spareBytesPerPage == 32)||
		(nand->spareBytesPerPage == 16))
	{
		eccDataNums = nand->spareBytesPerPage/16*3;
		dataOffsetPtr = oobPtr[nand->spareBytesPerPage/16-1];
	}
	else
	{
		return SmcHwReadSizeOver;
	}
*/

	/* Calculate Ecc */
	Status =  Nand_HwCalculateEcc(eccData, eccDataNums);
	if(Status != SmcSuccess)
	{
		return Status;
	}

	for(i = 0; i < eccDataNums; i++)
	{
		csu_printf("Cal ecc: %d ",eccData[i]);

	}
	csu_printf(" \r\n");


	/* Read Spare data */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, TempSpareBuf, nand->spareBytesPerPage-ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, NO_ECC_LAST);
	TempSpareBuf += (nand->spareBytesPerPage-ONFI_AXI_DATA_WIDTH);

	/* keep ONFI_AXI_DATA_WIDTH bytes read to clear cs */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, TempSpareBuf, ONFI_AXI_DATA_WIDTH, CLEAR_CS, NO_ECC_LAST);

	for(i = 0; i < eccDataNums; i++)
	{
		ReadEccData[i] = ~SpareBuf[dataOffsetPtr[i]];

		csu_printf("Read Ecc: %d ",ReadEccData[i]);
	}

	csu_printf("\r\n");

	i = nand->dataBytesPerPage/NAND_ECC_BLOCK_SIZE;
	for(;i;i--)
	{
		Status = Nand_HwCorrectEcc(&ReadEccData[EccOffset], &eccData[EccOffset], TempBuf);
		if(Status != SmcSuccess)
		{
			return Status;
		}

		EccOffset += NAND_ECC_BYTES;
		TempBuf += NAND_ECC_BLOCK_SIZE;
	}

	return SmcSuccess;

}




 uint8_t Onfi_CmdReadPage(uint32_t Page, uint32_t Column, Nand_TypeDef *nand)
{

	SmcSendCommand(ONFI_CMD_READ_PAGE1, ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_CYCLES,
				ONFI_CMD_READ_PAGE_END_TIMING, Page, Column);


	while(Nand_IsBusy() == NAND_BUSY);

	return SmcSuccess;

}


static uint8_t Onfi_CmdReadStatus(void)
{
	uint8_t Status = 0;
	volatile unsigned long dataPhaseAddr = 0;

	SmcSendCommand(ONFI_CMD_READ_STATUS1, ONFI_CMD_READ_STATUS2, ONFI_CMD_READ_STATUS_CYCLES,
			ONFI_CMD_READ_STATUS_END_TIMING, ONFI_PAGE_NOT_VALID, ONFI_COLUMN_NOT_VALID);

	/* data phase address */
	dataPhaseAddr  = NAND_BASE 					|
			(0 	<< NAND_ADDR_CYCLES_SHIFT)		|
			(0 	<< NAND_END_CMD_VALID_SHIFT)	|
			NAND_DATA_PHASE_FLAG				|
			(0 	<< NAND_END_CMD_SHIFT)			|
			(0 	<< NAND_ECC_LAST_SHIFT);
	Status = SMC_Read8BitReg(dataPhaseAddr);

	return Status;
}




//extern Nand_Size_TypeDef nandSize;

		/* data size 512bytes */



uint8_t Onfi_CmdBlockErase(uint32_t Page)
{
	uint8_t Status;

	Status = Onfi_CmdReadStatus();
	if (!(Status & ONFI_STATUS_WP))
	{
		return NAND_WRITE_PROTECTED;
	}

	SmcSendCommand(ONFI_CMD_ERASE_BLOCK1, ONFI_CMD_ERASE_BLOCK2, ONFI_CMD_ERASE_BLOCK_CYCLES,
			ONFI_CMD_ERASE_BLOCK_END_TIMING, Page, ONFI_COLUMN_NOT_VALID);

	while(Nand_IsBusy() == NAND_BUSY);

	/*  Clear SMC Interrupt 1, as an alternative to an AXI read */
	*((uint32_t *)(SMC_BASE+SMC_REG_MEM_CFG_CLR)) |= SMC_MemCfgClr_ClrSmcInt1;

	Status = Onfi_CmdReadStatus();
	if (Status & ONFI_STATUS_FAIL)
	{
		return FAILED;
	}

	return SUCCESS;
}




uint8_t Onfi_CmdProgramPage(uint32_t Page, uint32_t Column)
{
	uint8_t Status;

	/* Check Nand Status */
	Status = Onfi_CmdReadStatus();

	if (!(Status & ONFI_STATUS_WP))
	{
		return FAILED;
	}

	SmcSendCommand(ONFI_CMD_PROGRAM_PAGE1, ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_CYCLES,
			ONFI_CMD_PROGRAM_PAGE_END_TIMING, Page, Column);

	return SUCCESS;
}







uint8_t Onfi_CmdProgramCachePage(uint32_t Page, uint32_t Column)
{
	uint8_t Status;

	/* Check Nand Status */
	Status = Onfi_CmdReadStatus();

	if (!(Status & ONFI_STATUS_WP))
	{
		return FAILED;
	}

	SmcSendCommand(ONFI_CMD_PROGRAM_CACHE1, ONFI_CMD_PROGRAM_CACHE2, ONFI_CMD_PROGRAM_CACHE_CYCLES,
			ONFI_CMD_PROGRAM_CACHE_END_TIMING, Page, Column);

	return SUCCESS;

}












uint8_t Nand_ProgramPage(uint32_t Page, uint32_t Column, uint8_t *Buf, Nand_TypeDef *nandSize)
{
	uint8_t Status;

	//Nand_EccHwDisable();


	SmcWriteBuf(ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_END_TIMING, Buf,
			nandSize->dataBytesPerPage, NO_CLEAR_CS, NO_ECC_LAST);

	Buf += (nandSize->dataBytesPerPage);

	SmcWriteBuf(ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_END_TIMING, Buf,
			nandSize->spareBytesPerPage-ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, NO_ECC_LAST);


	Buf += (nandSize->spareBytesPerPage-ONFI_AXI_DATA_WIDTH);

	SmcWriteBuf( ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_END_TIMING, Buf,
			ONFI_AXI_DATA_WIDTH, CLEAR_CS, NO_ECC_LAST);

	while(Nand_IsBusy() == NAND_BUSY);

	/*  Clear SMC Interrupt 1, as an alternative to an AXI read */
	*((uint32_t *)(SMC_BASE+SMC_REG_MEM_CFG_CLR)) |= SMC_MemCfgClr_ClrSmcInt1;

	/* Check Nand Status */
	Status = Onfi_CmdReadStatus();

	if(Status & ONFI_STATUS_FAIL)
	{
		return FAILED;
	}

	return SUCCESS;
}





uint8_t Nand_ProgramPage_EOob(uint32_t Page, uint32_t Column, uint8_t *Buf, Nand_TypeDef *nandSize)
{
	uint8_t Status;


	SmcWriteBuf(ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_END_TIMING, Buf,
			nandSize->dataBytesPerPage-ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, NO_ECC_LAST);

	Buf += (nandSize->dataBytesPerPage-ONFI_AXI_DATA_WIDTH);

	SmcWriteBuf(ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_END_TIMING, Buf,
			ONFI_AXI_DATA_WIDTH, CLEAR_CS, NO_ECC_LAST);

	while(Nand_IsBusy() == NAND_BUSY);

	/*  Clear SMC Interrupt 1, as an alternative to an AXI read */
	*((uint32_t *)(SMC_BASE+SMC_REG_MEM_CFG_CLR)) |= SMC_MemCfgClr_ClrSmcInt1;

	/* Check Nand Status */
	Status = Onfi_CmdReadStatus();

	if(Status & ONFI_STATUS_FAIL)
	{
		return FAILED;
	}

	return SUCCESS;
}

static uint8_t __attribute__((aligned(4))) SpareData[512] = {0xff,0xff};


/* Variable Definitions */
/* ECC data position in the spare data area  for different page sizes */
	/* Ecc offset for 16 bytes spare area */
	/* Ecc offset for 32 bytes spare area */
/* Ecc offset for 64 bytes spare area */

uint8_t Nand_ProgramPage_HwEcc(uint32_t Page, uint32_t Column, uint8_t *Buf, Nand_TypeDef *nandSize)
{
	uint8_t Status,i,eccDataNums;
	uint8_t eccData[12]={0};
	uint32_t *dataOffsetPtr;
	uint8_t *TempBuf=Buf;
	uint8_t *TempSpareBuf=SpareData;

	/* Write dataBytesPerPage*/
	SmcWriteBuf(ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_END_TIMING, Buf, nandSize->dataBytesPerPage-ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, NO_ECC_LAST);
	Buf += (nandSize->dataBytesPerPage-ONFI_AXI_DATA_WIDTH);

	SmcWriteBuf(ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_END_TIMING, Buf, ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, ECC_LAST);
	Buf += ONFI_AXI_DATA_WIDTH;


	switch(nandSize->spareBytesPerPage)
	{
		case(16):
			eccDataNums = 3;
			dataOffsetPtr = NandOob16;
			break;
		case(32):
			eccDataNums = 6;
			dataOffsetPtr = NandOob32;
			break;
		case(64):
			eccDataNums = 12;
			dataOffsetPtr = NandOob64;
			break;
		case(224):
			eccDataNums = 12;
			dataOffsetPtr = NandOob64;
			break;
		default:
			/* Page size 256 bytes & 4096 bytes not supported by ECC block */
			csu_printf("[smc hw write page]:Page size error\r\n");
			return FAILED;
	}

	/* Calculate Ecc and Fill Spare data */
	Nand_HwCalculateEcc(eccData, eccDataNums);

	for(i = 0; i < eccDataNums; i++) {
		TempSpareBuf[dataOffsetPtr[i]] = (~eccData[i]);
	}

	SmcWriteBuf(ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_END_TIMING, TempSpareBuf, nandSize->spareBytesPerPage-ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, NO_ECC_LAST);
	TempSpareBuf += (nandSize->spareBytesPerPage-ONFI_AXI_DATA_WIDTH);

	SmcWriteBuf( ONFI_CMD_PROGRAM_PAGE2, ONFI_CMD_PROGRAM_PAGE_END_TIMING, TempSpareBuf, ONFI_AXI_DATA_WIDTH, CLEAR_CS, NO_ECC_LAST);

	while(Nand_IsBusy() == NAND_BUSY);

	/*  Clear SMC Interrupt 1, as an alternative to an AXI read */
	*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEM_CFG_CLR)) |= SMC_MemCfgClr_ClrSmcInt1;

	/* Check Nand Status */
	Status = Onfi_CmdReadStatus();

	if(Status & ONFI_STATUS_FAIL) {
		return FAILED;
	}

	return SUCCESS;
}


































