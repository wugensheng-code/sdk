#include "nand_drv.h"
#include "smc_drv.h"
#include "mtimer.h"
#include "string.h"


#define EFUSE_NAND_TIMING_REG1 (0xF8806250ULL)
#define EFUSE_NAND_TIMING_REG2 (0xF8806254ULL)


uint8_t __attribute__((aligned(4), weak)) FlashSharedBuf[4096] = {0};
static MtimerParams NandTimer;





static uint8_t Onfi_CmdReadStatus(void)
{
	uint8_t Status = 0;
	unsigned long dataPhaseAddr = 0;

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

/* Foresee On Die Ecc Max Page Size 4096/512=8  */
static uint8_t __attribute__((aligned(4))) EccStatus[8] = {0};


static void Onfi_CmdEccReadStatus(Nand_TypeDef *nand)
{
	uint8_t i = 0;

	unsigned long dataPhaseAddr = 0;

	SmcSendCommand(ONFI_CMD_ECC_READ_STATUS1, ONFI_CMD_ECC_READ_STATUS2, ONFI_CMD_ECC_READ_STATUS_CYCLES,
			ONFI_CMD_ECC_READ_STATUS_END_TIMING, ONFI_PAGE_NOT_VALID, ONFI_COLUMN_NOT_VALID);

	/* data phase address */
	dataPhaseAddr  = NAND_BASE 					|
			(0 	<< NAND_ADDR_CYCLES_SHIFT)		|
			(0 	<< NAND_END_CMD_VALID_SHIFT)	|
			NAND_DATA_PHASE_FLAG				|
			(0 	<< NAND_END_CMD_SHIFT)			|
			(0 	<< NAND_ECC_LAST_SHIFT);

	for(i = 0; i < nand->dataBytesPerPage / 512; i++)
	{
		EccStatus[i] = SMC_Read8BitReg(dataPhaseAddr);
	}
}

static uint8_t Onfi_CmdReset(void)
{
	uint8_t Status = 0;

	SmcSendCommand(ONFI_CMD_RESET1, ONFI_CMD_RESET2, ONFI_CMD_RESET_CYCLES,
			ONFI_CMD_RESET_END_TIMING, ONFI_PAGE_NOT_VALID, ONFI_COLUMN_NOT_VALID);

	Mtimer_Start(&NandTimer, 500*1000);

	 /*Check the Status Register SR[6]*/
	do
	{
		branch_put_char("(2) -> ");
		Status = Onfi_CmdReadStatus();
		if(Mtimer_TimerOut(&NandTimer) == 1)
		{
			branch_put_char("(3) -> ");
			return SmcResetErr;
		}
		branch_put_char("(4) -> ");
	} while(!(Status & ONFI_STATUS_RDY));
	branch_put_char("(1) -> ");

	return SmcSuccess;
}


static uint32_t Nand_CrcCheck(uint8_t *buf)
{
	const uint32_t Polynom = CRC16_POLYNOM;
	uint32_t Crc = CRC16_INIT;
	uint32_t Index = 0;
	uint32_t j = 0;
	uint32_t Bit = 0;
	uint32_t Data = 0;
	uint32_t CrcMask = 0xFFFF;
	uint32_t CrcHighBit = 0x8000;

	/* ONFI 1.0 Spec 5.1.4.36
	 * The Integrity CRC (Cyclic Redundancy Check) field is used to verify
	 * that the contents of the parameters page were
	 * transferred correctly to the host.
	 * CRC-16/IBM: G(X) = X16 + X15 + X2 + 1 	*/

	for(Index = 0; Index < CRC16_LEN; Index++)
	{
		Data = buf[Index];
		for(j = 0x80; j; j >>= 1)
		{
			Bit = Crc & CrcHighBit;
			Crc <<= 1;
			if(Data & j)
			{
				Bit ^= CrcHighBit;
			}
			if(Bit)
			{
				Crc ^= Polynom;
			}
		}
		Crc &= CrcMask;
	}

	return Crc;
}


static uint8_t Onfi_CmdReadParameter(Nand_TypeDef *nand)
{
	uint8_t Status = 0;
	uint8_t __attribute__((aligned(4))) Temp[ONFI_PARAM_LEN] = {0};
	uint32_t Crc = 0;
	uint32_t Index = 0;
	unsigned long cmdPhaseAddr = 0;

	SmcSendCommand(ONFI_CMD_READ_PARAMETER1, ONFI_CMD_READ_PARAMETER2, ONFI_CMD_READ_PARAMETER_CYCLES,
			ONFI_CMD_READ_PARAMETER_END_TIMING, ONFI_PAGE_NOT_VALID, 0x00);

	/* look datasheet send readparem cmd need 25us delay. */
	
	Mtimer_Delay(200);

	for (Index = 0; Index < 3; Index++)
	{
		branch_put_char("(10) -> ");
		SmcReadPara(ONFI_CMD_READ_PARAMETER2, ONFI_CMD_READ_PARAMETER_END_TIMING, Temp, ONFI_PARAM_LEN, nand);

		Crc = Nand_CrcCheck(Temp);

		if(((Crc&0xff) == Temp[CRC16_LEN])
		&&(((Crc >> 8)&0xff) == Temp[CRC16_LEN+1]))
		{
			branch_put_char("(7) -> ");
			break;
		}
		branch_put_char("(8) -> ");
	}

	if(Index == 3)
	{
		branch_put_char("(9) -> ");
		return SmcCrcErr;
	}


	/* Bit0 set to 1.data bus width 16bits.
	 * Bit0 set to 0.data bus width 8bits. 	*/

	nand->dataBytesPerPage = 	*((uint32_t *)(&Temp[DATA_PER_PAGE_POS]));
	nand->spareBytesPerPage = 	*((uint16_t *)(&Temp[SPARE_PER_PAGE_POS]));
	nand->pagesPerBlock = 		*((uint32_t *)(&Temp[PAGE_PER_BLOCK_POS]));
	nand->blocksPerUnit =		*((uint32_t *)(&Temp[BLOCKS_PER_UINT_POS]));
	nand->totalUnit =			Temp[TOTAL_UINT_POS];
	nand->eccNum =				Temp[ECC_NUM_POS];

	nand_printf("nand->dataBytesPerPage  :%u\r\n",nand->dataBytesPerPage);
	nand_printf(" nand->spareBytesPerPage :%d\r\n",nand->spareBytesPerPage);
	nand_printf(" nand->pagesPerBlock :%u\r\n",nand->pagesPerBlock);
	nand_printf(" nand->blocksPerUnit :%u\r\n",nand->blocksPerUnit);
	nand_printf(" nand->totalUnit :%d\r\n",nand->totalUnit);
	nand_printf(" nand->eccNum :%d\r\n",nand->eccNum);

	if((nand->dataBytesPerPage > SMC_MAX_PAGE_SIZE) ||
		(nand->spareBytesPerPage > SMC_MAX_SPARE_SIZE)||
		(nand->pagesPerBlock > SMC_MAX_PAGES_PER_BLOCK) ||
		(nand->blocksPerUnit > SMC_MAX_BLOCKS)||
		(nand->totalUnit > SMC_MAX_LUNS))
	{
		branch_put_char("(11) -> ");
		return SmcParameterOver;
	}
	branch_put_char("(12) -> ");

	return SmcSuccess;
}


static uint8_t Nand_IsBusy(void)
{
	uint32_t Status = (SMC_ReadReg(SMC_BASE+SMC_REG_MEMC_STATUS) & (1 << SMC_MemcStatus_SmcInt1RawStatus_FIELD));


	if(Status)
		return NAND_READY;
	else
		return NAND_BUSY;
}

static void Onfi_CmdReadId(uint8_t Address, uint8_t *Id, uint8_t idSize, Nand_TypeDef *nand)
{
	SmcSendCommand(ONFI_CMD_READ_ID1, ONFI_CMD_READ_ID2, ONFI_CMD_READ_ID_CYCLES,ONFI_CMD_READ_ID_END_TIMING, ONFI_PAGE_NOT_VALID, Address);
	SmcReadPara(ONFI_CMD_READ_ID2, ONFI_CMD_READ_ID_END_TIMING, Id, idSize, nand);
}


static uint8_t Onfi_CmdReadPage(uint32_t Page, uint32_t Column, Nand_TypeDef *nand)
{

	SmcSendCommand(ONFI_CMD_READ_PAGE1, ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_CYCLES,
				ONFI_CMD_READ_PAGE_END_TIMING, Page, Column);

	Mtimer_Start(&NandTimer, 500 * 1000);

	while(Nand_IsBusy() == NAND_BUSY)
	{
		branch_put_char("(48) -> ");
		if(Mtimer_TimerOut(&NandTimer) == 1)
		{
			branch_put_char("(49) -> ");
			return SmcReadCmdTimeOutErr;
		}
		branch_put_char("(50) -> ");
	}
	branch_put_char("(47) -> ");

	return SmcSuccess;

}












static uint32_t __attribute__((aligned(4))) NandOob64[12] = {52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};
static uint32_t __attribute__((aligned(4))) NandOob32[6] = {26, 27, 28, 29, 30, 31};
static uint32_t __attribute__((aligned(4))) NandOob16[3] = {13, 14, 15};		/* data size 512bytes */






static uint8_t Nand_HwCalculateEcc(uint8_t *Dst, uint8_t eccDataNums)
{
	uint8_t Status = 0,Count = 0;
	uint8_t eccReg = 0;
	uint32_t eccValue = 0;

	Mtimer_Start(&NandTimer, 500*1000);

	/* Check busy signal if it is busy to poll*/
	do {
		branch_put_char("(83) -> ");
		Status = (SMC_ReadReg(SMC_BASE+SMC_REG_ECC1_STATUS) & (1 << SMC_EccStatus_EccStatus_FIELD));

		if(Mtimer_TimerOut(&NandTimer) == 1)
		{
			branch_put_char("(86) -> ");
			return SmcCalEccBusyErr;
		}
		branch_put_char("(85) -> ");
	}while(Status);

	branch_put_char("(84) -> ");

	for(eccReg=0; eccReg < eccDataNums/3; eccReg++)
	{
		branch_put_char("(87) -> ");
		eccValue = SMC_ReadReg(SMC_BASE+SMC_REG_ECC1_BLOCK0+eccReg*4);
		nand_printf("eccValue:%x\r\n",eccValue);

		if((eccValue) & (1 << SMC_EccBlock_ISCheckValueValid_FIELD))
		{
			branch_put_char("(90) -> ");
			for(Count = 0; Count < 3; Count++)
			{
				*Dst = eccValue & 0xFF;
				nand_printf("Dst: %d\r\n",*Dst);
				eccValue = eccValue >> 8;
				Dst++;
			}
		}
		else
		{
			branch_put_char("(89) -> ");
			return SmcEccDataInvalidErr;
		}
	}
	branch_put_char("(88) -> ");

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
		branch_put_char("(75) -> ");
		nand_printf("0 bit error\r\n");
		return SmcSuccess;
	}
	branch_put_char("(76) -> ");


	/* One bit Error */
	if (eccOdd == (~eccEven & 0xfff))
	{
		branch_put_char("(77) -> ");
		bytePos = (eccOdd >> 3) & 0x1ff;
		bitPos = eccOdd & 0x07;
		/* Toggling error bit */
		buf[bytePos] ^= (1 << bitPos);

		nand_printf("one bit error\r\n");
		nand_printf("bytePos:%u bitPos:%u \r\n",bytePos,bitPos);

		return SmcSuccess;
	}
	branch_put_char("(78) -> ");

	/* Two bits Error */
	if (OneHot((eccOdd | eccEven)) == SmcSuccess)
	{
		branch_put_char("(79) -> ");
		nand_printf("Two bits Error\r\n");
		return SmcSuccess;
	}
	branch_put_char("(80) -> ");

	nand_printf("Multiple bits error\r\n");
	/* Multiple bits error */
	return SmcMultipleBitsErr;

}






static uint8_t Nand_EccHwInit(Nand_TypeDef *nand)
{
	uint32_t ecc1Config;

	/* Set SMC_REG_ECC1_MEMCMD0 Reg*/
	SMC_WriteReg((SMC_BASE + SMC_REG_ECC1_MEMCMD0),
			(SMC_EccMemCmd0_InitWriteCmd| SMC_EccMemCmd0_InitReadCmd|
					SMC_EccMemCmd0_EndReadCmd| SMC_EccMemCmd0_UseEndCmd));

	/* Set SMC_REG_ECC1_MEMCMD1 Reg*/
	SMC_WriteReg((SMC_BASE + SMC_REG_ECC1_MEMCMD1),
			(SMC_EccMemCmd1_ColChangeWRCmd| SMC_EccMemCmd1_ColChangeRDCmd|
				SMC_EccMemCmd1_ColChangeEndCmd| SMC_EccMemCmd1_UseEndCmd));




	switch(nand->dataBytesPerPage)
	{
		case(512):
			branch_put_char("(118) -> ");
			ecc1Config = SMC_EccCfg_One_PageSize;
			break;
		case(1024):
			branch_put_char("(119) -> ");
			ecc1Config = SMC_EccCfg_Two_PageSize;
			break;
		case(2048):
			branch_put_char("(24) -> ");
			ecc1Config = SMC_EccCfg_Four_PageSize;
			break;
		default:
			branch_put_char("(23) -> ");
			/* Page size 256 bytes & 4096 bytes not supported by ECC block */
			return SmcHwInitSizeErr;
	}

	Mtimer_Start(&NandTimer, 500*1000);

	/* Check Ecc Busy */
	while(SMC_ReadReg((SMC_BASE + SMC_REG_ECC1_STATUS)) & SMC_EccStatus_EccBusy)
	{
		branch_put_char("(26) -> ");
		if(Mtimer_TimerOut(&NandTimer) == 1)
		{
			branch_put_char("(28) -> ");
			return SmcHwInitEccBusyErr;
		}
		branch_put_char("(27) -> ");
	}
	branch_put_char("(25) -> ");


	SMC_WriteReg((SMC_BASE + SMC_REG_ECC1_CFG),
			(SMC_EccCfg_EccModeAPB| SMC_EccCfg_EccValue_InPageEnd| SMC_EccCfg_EccNoJump| ecc1Config));
	return SmcSuccess;
}


static uint8_t Nand_EccHwDisable(void)
{
	Mtimer_Start(&NandTimer, 500*1000);

	/* Check Ecc Busy */
	while(SMC_ReadReg((SMC_BASE + SMC_REG_ECC1_STATUS)) & SMC_EccStatus_EccBusy)
	{
		branch_put_char("(30) -> ");
		if(Mtimer_TimerOut(&NandTimer) == 1)
		{
			branch_put_char("(31) -> ");
			return SmcHwDisEccBusyErr;
		}
		branch_put_char("(32) -> ");
	}
	branch_put_char("(29) -> ");

	/* Set Ecc Model is bypass */
	//*((volatile uint32_t *)(SMC_BASE+SMC_REG_ECC1_CFG)) &= (~(3 << SMC_EccCfg_EccMode_FIELD));
	SMC_WriteReg((SMC_BASE+SMC_REG_ECC1_CFG),
	(SMC_ReadReg(SMC_BASE + SMC_REG_ECC1_CFG) & (~(3 << SMC_EccCfg_EccMode_FIELD))));

	return SmcSuccess;
}


static uint8_t Onfi_CmdSetFeature(uint8_t Address, uint8_t *Value, Nand_TypeDef *nand)
{
	uint8_t Status = 0;

	SmcSendCommand(ONFI_CMD_SET_FEATURES1, ONFI_CMD_SET_FEATURES2, ONFI_CMD_SET_FEATURES_CYCLES,
			ONFI_CMD_SET_FEATURES_END_TIMING, ONFI_PAGE_NOT_VALID, Address);

	/* Xilink use write uint8_t loop 4 */
	SmcWriteBuf(ONFI_CMD_SET_FEATURES2, ONFI_CMD_SET_FEATURES_CYCLES, Value,
			4, NO_CLEAR_CS, NO_ECC_LAST);

	Mtimer_Start(&NandTimer, 500 * 1000);
	/* Check Nand Status */
	do
	{
		branch_put_char("(36) -> ");
		Status = Onfi_CmdReadStatus();
		if(Mtimer_TimerOut(&NandTimer) == 1)
		{
			branch_put_char("(38) -> ");
			return SmcFeatBusyErr;
		}
		branch_put_char("(37) -> ");
	} while (!(Status & ONFI_STATUS_RDY));

	branch_put_char("(35) -> ");
	return SmcSuccess;
}

static uint8_t Onfi_CmdGetFeature(uint8_t Address, uint8_t *Value, Nand_TypeDef *nand)
{
	uint8_t Status = 0;
	unsigned long cmdPhaseAddr = 0;
	SmcSendCommand(ONFI_CMD_GET_FEATURES1, ONFI_CMD_GET_FEATURES2, ONFI_CMD_GET_FEATURES_CYCLES,
			ONFI_CMD_GET_FEATURES_END_TIMING, ONFI_PAGE_NOT_VALID, Address);


	Mtimer_Start(&NandTimer, 500*1000);

	/* Check Nand Status */
	do
	{
		branch_put_char("(40) -> ");
		Status = Onfi_CmdReadStatus();
		if(Mtimer_TimerOut(&NandTimer) == 1)
		{
			branch_put_char("(41) -> ");
			return SmcFeatBusyErr;
		}
		branch_put_char("(42) -> ");
	} while (!(Status & ONFI_STATUS_RDY));

	branch_put_char("(39) -> ");

	/* re-issue a command value of 00h to start reading data */
	cmdPhaseAddr  = NAND_BASE 					|
			(0 	<< NAND_ADDR_CYCLES_SHIFT)		|
			(0 	<< NAND_END_CMD_VALID_SHIFT)	|
			NAND_COMMAND_PHASE_FLAG				|
			(0 	<< NAND_END_CMD_SHIFT)			|
			(0 	<< NAND_ECC_LAST_SHIFT);

	SMC_WriteReg(cmdPhaseAddr, 0x00);

	SmcReadPara(ONFI_CMD_GET_FEATURES2, ONFI_CMD_GET_FEATURES_CYCLES, Value, 4, nand);

	return SmcSuccess;
}


/* Only Micron */
static uint8_t Nand_EnableOnDieEcc(Nand_TypeDef *nand)
{
	uint8_t Temp = 0;
	uint8_t __attribute__((aligned(4))) EccSetFeature[4] = {0x08, 0x00, 0x00, 0x00};
	uint8_t __attribute__((aligned(4))) EccGetFeature[4] = {0};

	if(nand->deviceId[0] != NAND_MFR_AMD)
	{
		branch_put_char("(35) -> ");
		Temp = 0x08;
	}
	else
	{
		branch_put_char("(36) -> ");
		EccSetFeature[0] = 0x10;
		Temp = 0x10;
	}

	Onfi_CmdSetFeature(0x90, &EccSetFeature[0], nand);
	Onfi_CmdGetFeature(0x90, &EccGetFeature[0], nand);

	if(EccGetFeature[0] == Temp)
	{
		branch_put_char("(43) -> ");
		return SmcSuccess;
	}
	else
	{
		branch_put_char("(44) -> ");
		return SmcWriteEccFeatErr;
	}
}





uint8_t Nand_ReadSpareBytes(uint32_t Page, uint32_t Column, Nand_TypeDef *nand)
{
	unsigned long cmdPhaseAddr = 0, i = 0;
	uint8_t Status = 0;
	uint8_t Buf = 0;
	uint8_t __attribute__((aligned(4))) TempBuf[4] = {0};

	if(nand->eccNum == 1)
	{
		branch_put_char("(45) -> ");
		Status = Nand_EccHwDisable();
		
		if(Status != SmcSuccess)
		{
			return Status;
		}
	}
	branch_put_char("(46) -> ");

	Column = nand->dataBytesPerPage;


	/* Read Current page data bytes */
	Status =  Onfi_CmdReadPage(Page, Column, nand);
	if(Status != SmcSuccess)
	{
		return Status;
	}

	/*  Clear SMC Interrupt 1, as an alternative to an AXI read */
	//*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEM_CFG_CLR)) |= SMC_MemCfgClr_ClrSmcInt1;
	SMC_WriteReg((SMC_BASE+SMC_REG_MEM_CFG_CLR),
	(SMC_ReadReg(SMC_BASE + SMC_REG_MEM_CFG_CLR) | SMC_MemCfgClr_ClrSmcInt1));
	
	if(nand->deviceId[0] == NAND_MFR_MICRON)
	{
		branch_put_char("(52) -> ");
		/* Check Nand Status */
		Status = Onfi_CmdReadStatus();

		if(Status & ONFI_STATUS_FAIL)
		{
			branch_put_char("(53) -> ");
			return SmcMultipleBitsErr;
		}
		branch_put_char("(54) -> ");

		/* re-issue a command value of 00h to start reading data */
		cmdPhaseAddr  = NAND_BASE 					|
				(0 	<< NAND_ADDR_CYCLES_SHIFT)		|
				(0 	<< NAND_END_CMD_VALID_SHIFT)	|
				NAND_COMMAND_PHASE_FLAG				|
				(0 	<< NAND_END_CMD_SHIFT)			|
				(0 	<< NAND_ECC_LAST_SHIFT);

		SMC_WriteReg(cmdPhaseAddr, 0x00);
	}
	branch_put_char("(51) -> ");

	/* Read Spare data */
	/* keep ONFI_AXI_DATA_WIDTH bytes read to clear cs */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, TempBuf, 4, NO_CLEAR_CS, NO_ECC_LAST);

	Buf = TempBuf[0];


	for(i = 0; i < (nand->spareBytesPerPage-4-ONFI_AXI_DATA_WIDTH)/4; i++)
	{
		SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, TempBuf, 4, NO_CLEAR_CS, NO_ECC_LAST);
	}

	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, TempBuf, ONFI_AXI_DATA_WIDTH, CLEAR_CS, NO_ECC_LAST);

	if(nand->eccNum == 1)
	{
		branch_put_char("(56) -> ");
		Status = Nand_EccHwInit(nand);
		if(Status != SmcSuccess)
		{
			return Status;
		}

	}
	branch_put_char("(55) -> ");

	if(Buf != NANDP_BB_FLAG)
	{
		branch_put_char("(58) -> ");
		nand_printf("Find Bad Block:%d\r\n",Page);
		return SmcBadBlock;
	}
	else
	{
		branch_put_char("(57) -> ");
		return SmcSuccess;
	}
}







static uint8_t Nand_ReadPage_HwEcc(uint8_t *Buf, uint8_t *SpareBuf, Nand_TypeDef *nand)
{
	uint8_t *ptrBuf  = Buf;
	uint8_t *TempBuf = Buf;
	uint8_t *TempSpareBuf = SpareBuf;
	uint8_t Status = 0,  eccDataNums = 0, EccOffset = 0;
	uint32_t i = 0;
	uint8_t __attribute__((aligned(4))) eccData[12] = {0};
	uint8_t __attribute__((aligned(4))) ReadEccData[12] = {0};
	uint32_t *dataOffsetPtr = NULL;

	//uint32_t *oobPtr[4] = {NandOob16, NandOob32, 0, NandOob64};

	/* Read data Page */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, ptrBuf, nand->dataBytesPerPage-ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, NO_ECC_LAST);
	ptrBuf += (nand->dataBytesPerPage-ONFI_AXI_DATA_WIDTH);

	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, ptrBuf, ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, ECC_LAST);
	ptrBuf += ONFI_AXI_DATA_WIDTH;

	switch(nand->spareBytesPerPage)
	{
		case(64):
			branch_put_char("(69) -> ");
			eccDataNums = 12;
			dataOffsetPtr = NandOob64;
			break;
		case(32):
			branch_put_char("(70) -> ");
			eccDataNums = 6;
			dataOffsetPtr = NandOob32;
			break;
		case(16):
			branch_put_char("(71) -> ");
			eccDataNums = 3;
			dataOffsetPtr = NandOob16;
			break;
		default:
			branch_put_char("(72) -> ");
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
		nand_printf("Cal ecc: %d ",eccData[i]);

	}
	nand_printf(" \r\n");


	/* Read Spare data */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, TempSpareBuf, nand->spareBytesPerPage-ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, NO_ECC_LAST);
	TempSpareBuf += (nand->spareBytesPerPage - ONFI_AXI_DATA_WIDTH);

	/* keep ONFI_AXI_DATA_WIDTH bytes read to clear cs */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, TempSpareBuf, ONFI_AXI_DATA_WIDTH, CLEAR_CS, NO_ECC_LAST);

	for(i = 0; i < eccDataNums; i++)
	{
		ReadEccData[i] = ~SpareBuf[dataOffsetPtr[i]];

		nand_printf("Read Ecc: %d ",ReadEccData[i]);
	}

	nand_printf("\r\n");

	i = nand->dataBytesPerPage / NAND_ECC_BLOCK_SIZE;
	for(; i; i--)
	{
		branch_put_char("(73) -> ");
		Status = Nand_HwCorrectEcc(&ReadEccData[EccOffset], &eccData[EccOffset], TempBuf);
		if (Status != SmcSuccess)
		{
			return Status;
		}

		EccOffset += NAND_ECC_BYTES;
		TempBuf += NAND_ECC_BLOCK_SIZE;
	}

	branch_put_char("(74) -> ");

	return SmcSuccess;

}



static uint8_t Nand_ReadPage(uint8_t *Buf, uint8_t *SpareBuf, Nand_TypeDef *nand)
{
	uint8_t * ptrBuf = Buf;
	uint8_t * TempSpareBuf = SpareBuf;
	unsigned long cmdPhaseAddr = 0;
	uint8_t Status = 0;
	uint8_t i = 0;

	if(nand->deviceId[0] == NAND_MFR_MICRON)
	{
		branch_put_char("(91) -> ");
		/* Check Nand Status */
		Status = Onfi_CmdReadStatus();

		if (Status & ONFI_STATUS_FAIL)
		{
			branch_put_char("(93) -> ");
			return 	SmcMultipleBitsErr;			/* ReadPage Cmd之后发送读取状态寄存器的命令 */
		}
		branch_put_char("(94) -> ");
		/* re-issue a command value of 00h to start reading data */
		cmdPhaseAddr  = NAND_BASE 					|
				(0 	<< NAND_ADDR_CYCLES_SHIFT)		|
				(0 	<< NAND_END_CMD_VALID_SHIFT)	|
				NAND_COMMAND_PHASE_FLAG				|
				(0 	<< NAND_END_CMD_SHIFT)			|
				(0 	<< NAND_ECC_LAST_SHIFT);

		SMC_WriteReg(cmdPhaseAddr, 0x00);
	}

	branch_put_char("(92) -> ");

	/* Read data Page */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, ptrBuf, nand->dataBytesPerPage, NO_CLEAR_CS, NO_ECC_LAST);

	/* Read Spare data */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, TempSpareBuf, nand->spareBytesPerPage-ONFI_AXI_DATA_WIDTH, NO_CLEAR_CS, NO_ECC_LAST);
	TempSpareBuf += (nand->spareBytesPerPage-ONFI_AXI_DATA_WIDTH);

	/* keep ONFI_AXI_DATA_WIDTH bytes read to clear cs */
	SmcReadBuf(ONFI_CMD_READ_PAGE2, ONFI_CMD_READ_PAGE_END_TIMING, TempSpareBuf, ONFI_AXI_DATA_WIDTH, CLEAR_CS, NO_ECC_LAST);

	if(nand->deviceId[0] != NAND_MFR_MICRON)
	{
		branch_put_char("(95) -> ");
		if (nand->deviceId[0] == NAND_MFR_SAMSUNG)
		{
			branch_put_char("(98) -> ");
			Onfi_CmdEccReadStatus(nand);

			for(i = 0; i < nand->dataBytesPerPage/512; i++)
			{
				branch_put_char("(99) -> ");
				if ((((EccStatus[i]&0xf0) >> 4) != i) || ((EccStatus[i]&0xf) > 4))
				{
					branch_put_char("(101) -> ");
					return SmcMultipleBitsErr;			/* Mul Bits Error */
				}
				branch_put_char("(102) -> ");
			}
			branch_put_char("(100) -> ");
		}
		else if(nand->deviceId[0] == NAND_MFR_AMD)
		{
			branch_put_char("(97) -> ");
			branch_put_char("(104) -> ");
			Status = Onfi_CmdReadStatus();

			if (Status & ONFI_STATUS_PAGE_UNCORRECT)
			{
				branch_put_char("(107) -> ");
				return 	SmcMultipleBitsErr;			/* Mul Bits Error */
			}
			branch_put_char("(108) -> ");
		}
		else //if(nand->deviceId[0] == NAND_MFR_MACRONIX)
		{
			branch_put_char("(97) -> ");
			branch_put_char("(103) -> ");
			Status = Onfi_CmdReadStatus();

			if (Status & MUL_BITS_ERR)
			{
				branch_put_char("(106) -> ");
				return 	SmcMultipleBitsErr;			/* Mul Bits Error */
			}
			branch_put_char("(105) -> ");
		}
	}

	branch_put_char("(96) -> ");

	return SmcSuccess;
}



/*
 *进行必要的nand controller初始化，driver初始化，nand chip配置流程；
 *通过NandParams结构体指针返回Nand ID、Size信息
 *函数返回成功或错误类型
 */
uint32_t Csu_NandInit(Nand_TypeDef *nand)
{
	uint8_t Status = 0;
	int extid = 0;
	uint32_t Reg1 = 0, Reg2 = 0;

	Mtimer_Init(&NandTimer);

	Reg1 = REG32_READ(EFUSE_NAND_TIMING_REG1);
	Reg2 = REG32_READ(EFUSE_NAND_TIMING_REG2);

	if((Reg1 != 0)&&(Reg2 != 0))
	{
		branch_put_char("(120) -> ");
		/* Setup the timings */
		SMC_WriteReg((SMC_BASE + SMC_REG_SET_CYCLES), (Reg1 | Reg2));
		//SMC_WriteReg((SMC_BASE + SMC_REG_DIRCT_CMD), (SMC_DirectCmd_SelChip_Interface1Chip1 | SMC_DirectCmd_CmdType_UpdateRegs));
		SMC_WriteReg((SMC_BASE + SMC_REG_DIRCT_CMD), (SMC_DirectCmd_SelChip_Interface1Chip1 | SMC_DirectCmd_CmdType_UpdateRegsAndAXI));
	}

	/* Reset NandFlash */
	branch_put_char("Csu_NandInit branch path: (0) -> ");
	Status = Onfi_CmdReset();
	if(Status != SmcSuccess)
	{
		return Status;
	}

	/* Read NandFlash ID */
	Onfi_CmdReadId(0x00, &(nand->deviceId[0]), 5, nand);

	nand_printf("[nand_id]:");
	nand_printf("%x ",nand->deviceId[0]);
	nand_printf("%x ",nand->deviceId[1]);
	nand_printf("%x ",nand->deviceId[2]);
	nand_printf("%x ",nand->deviceId[3]);
	nand_printf("%x\r\n",nand->deviceId[4]);

	

	/* Read NandFlash Parameter */
	if((nand->deviceId[0] != NAND_MFR_SAMSUNG) && (nand->deviceId[0] != NAND_MFR_ESMT))
	{
		branch_put_char("(6) -> ");
		Status = Onfi_CmdReadParameter(nand);

		if(Status != SmcSuccess)
		{
			branch_put_char("(13) -> ");
			return Status;
		}
		branch_put_char("(14) -> ");
	}
	else
	{
		branch_put_char("(5) -> ");
		/* from kernel */
		extid = nand->deviceId[3];

		nand->dataBytesPerPage = 1024 << (extid & 0x03);
		extid >>= 2;
		// oob size
		nand->spareBytesPerPage = (8 << (extid & 0x01)) * (nand->dataBytesPerPage >> 9);

		extid >>= 2;
		nand->pagesPerBlock = ((64 * 1024) << (extid & 0x03)) /nand->dataBytesPerPage;

		/* myself */
		extid = nand->deviceId[4] >> 4;

		nand->blocksPerUnit = (((8 * 1024 * 1024) << (extid & 0x07))/(nand->pagesPerBlock * nand->dataBytesPerPage))
				<< ((nand->deviceId[4] >> 2) & 0x03);

		nand->totalUnit = 1;

		if((nand->dataBytesPerPage > SMC_MAX_PAGE_SIZE) ||
				(nand->spareBytesPerPage > SMC_MAX_SPARE_SIZE)||
				(nand->pagesPerBlock > SMC_MAX_PAGES_PER_BLOCK))
		{
			branch_put_char("(13) -> ");
			return SmcSamsungParamOver;
		}

		nand_printf("nand->dataBytesPerPage  :%u\r\n",nand->dataBytesPerPage);
		nand_printf(" nand->spareBytesPerPage :%d\r\n",nand->spareBytesPerPage);
		nand_printf(" nand->pagesPerBlock :%u\r\n",nand->pagesPerBlock);
		nand_printf(" nand->blocksPerUnit :%u\r\n",nand->blocksPerUnit);
		nand_printf(" nand->totalUnit :%d\r\n",nand->totalUnit);
		nand_printf(" nand->eccNum :%d\r\n",nand->eccNum);

		/* form myself */
		if(nand->deviceId[0] == NAND_MFR_SAMSUNG)
		{
			branch_put_char("(15) -> ");
			nand->eccNum = 4;
		}
		else if(nand->deviceId[0] == NAND_MFR_ESMT)
		{
			branch_put_char("(16) -> ");
			nand->eccNum = 1;
			branch_put_char("(18) -> ");
		}
		branch_put_char("(17) -> ");
	}

	if((nand->deviceId[0] == NAND_MFR_AMD)&&(nand->eccNum == 0))
	{
		branch_put_char("(20) -> ");
		/* Because SkyHigh ecc nums is zero */
		nand->eccNum = 4;

	}
	branch_put_char("(19) -> ");

	nand_printf("eccNum:%d\r\n",nand->eccNum);

	/* ecc Num = 1.Use SMC IP ECC */
	if(nand->eccNum == 1)
	{
		branch_put_char("(22) -> ");
		/* Enable SMC IP Hardware */
		Status = Nand_EccHwInit(nand);

		if(Status != SmcSuccess)
		{
			return Status;
		}
	}
	else
	{
		branch_put_char("(21) -> ");
		/* Close SMC IP ECC */
		Status = Nand_EccHwDisable();

		if(Status != SmcSuccess)
		{
			return Status;
		}

		if((nand->deviceId[0] == NAND_MFR_AMD)||
			(nand->deviceId[0] == NAND_MFR_MICRON))
		{
			branch_put_char("(33) -> ");
			/* Enable On Die ECC */
			Status = Nand_EnableOnDieEcc(nand);

			if(Status != SmcSuccess)
			{
				return Status;
			}
		}
	}
	branch_put_char("(34) -> ");

	return SmcSuccess;

}






static uint8_t __attribute__((aligned(4))) SpareBuf[SMC_MAX_SPARE_SIZE] = {0};



/*
 *按照offset和length读取nand数据；
 *能够根据offset计算block位置
 *返回成功或错误类型
 */
uint32_t Csu_NandRead(uint32_t offset, uint8_t* dest, uint32_t length, Nand_TypeDef *nand)
{
	uint8_t *Ptr = (uint8_t *)dest;
	uint8_t Status = 0;
	uint32_t Page = 0, Col = 0;
	uint32_t NumOfBytes = 0, PartialBytes = 0, CopyOffset = 0, PartialPageRead = 0;
	uint8_t *BufPtr = NULL;

	branch_put_char("Csu_NandRead branch path: (0) -> ");

	Page = (uint32_t)(offset / nand->dataBytesPerPage);

	Status = Nand_ReadSpareBytes(Page - Page%nand->pagesPerBlock, 0, nand);

	if(Status != SmcSuccess)
	{
		return Status;
	}

	Col = (uint32_t)(offset & (nand->dataBytesPerPage - 1));
	PartialBytes = nand->dataBytesPerPage - Col;
	NumOfBytes = (PartialBytes < length) ? PartialBytes:length;
	CopyOffset = nand->dataBytesPerPage - PartialBytes;

	/* Restore the ECC mem command1 and ECC mem command2 register if the previous command is read page cache */
	/* Set SMC_REG_ECC1_MEMCMD0 Reg*/
	SMC_WriteReg((SMC_BASE + SMC_REG_ECC1_MEMCMD0),
			(SMC_EccMemCmd0_InitWriteCmd| SMC_EccMemCmd0_InitReadCmd| SMC_EccMemCmd0_EndReadCmd| SMC_EccMemCmd0_UseEndCmd));

	/* Set SMC_REG_ECC1_MEMCMD1 Reg*/
	SMC_WriteReg((SMC_BASE + SMC_REG_ECC1_MEMCMD1),
			(SMC_EccMemCmd1_ColChangeWRCmd| SMC_EccMemCmd1_ColChangeRDCmd| SMC_EccMemCmd1_ColChangeEndCmd| SMC_EccMemCmd1_UseEndCmd));


	while(length)
	{
		branch_put_char("(59) -> ");
		if((Page%nand->pagesPerBlock)==0)
		{
			branch_put_char("(62) -> ");

			Status = Nand_ReadSpareBytes(Page, 0, nand);
			if(Status != SmcSuccess)
			{
				nand_printf("Spare Faild\r\n");
				return Status;
			}
		}
		branch_put_char("(61) -> ");

		/*
		* Check if partial read
		*/
		if (NumOfBytes < nand->dataBytesPerPage)
		{
			branch_put_char("(63) -> ");
			BufPtr = FlashSharedBuf;
			PartialPageRead = 1;
		}
		else
		{
			branch_put_char("(64) -> ");
			BufPtr = (uint8_t *)Ptr;
			PartialPageRead = 0;
		}

		Status =  Onfi_CmdReadPage(Page, 0, nand);
		if(Status != SmcSuccess)
		{
			return Status;
		}

		/*  Clear SMC Interrupt 1, as an alternative to an AXI read */
		//*((volatile uint32_t *)(SMC_BASE+SMC_REG_MEM_CFG_CLR)) |= SMC_MemCfgClr_ClrSmcInt1;
		SMC_WriteReg((SMC_BASE+SMC_REG_MEM_CFG_CLR),
		(SMC_ReadReg(SMC_BASE + SMC_REG_MEM_CFG_CLR) | SMC_MemCfgClr_ClrSmcInt1));

		//add bad flag
		/* ecc Num = 1.Use SMC IP ECC */
		if(nand->eccNum == 1)
		{
			branch_put_char("(65) -> ");

			Status = Nand_ReadPage_HwEcc(BufPtr, SpareBuf, nand);
		}
		/* ecc Num != 1.Use On Die ECC */
		else
		{
			branch_put_char("(66) -> ");

			Status = Nand_ReadPage(BufPtr, SpareBuf, nand);
		}

		if(Status != SmcSuccess)
		{
			nand_printf("Read Faild\r\n");
			return Status;
		}

		if (PartialPageRead)
		{
			branch_put_char("(67) -> ");
			memcpy(Ptr, (BufPtr + CopyOffset), NumOfBytes);
		}
		branch_put_char("(68) -> ");
		Ptr += NumOfBytes;
		length -= NumOfBytes;
		Page++;
		NumOfBytes = (length > nand->dataBytesPerPage) ?
		nand->dataBytesPerPage:length;
		CopyOffset = 0;
	}

	branch_put_char("(60) -> ");

	return SmcSuccess;
}









