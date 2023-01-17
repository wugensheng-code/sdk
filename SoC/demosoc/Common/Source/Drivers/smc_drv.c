/*
 * smc_drv.c
 *
 *  Created on: 2022年6月6日
 *      Author: jian.huang
 */

#include "smc_drv.h"

#include "nand_drv.h"




#ifdef NAND_BRANCH_PUT_CHAR

volatile char branch_log_array[512];
volatile char *branch_log_ptr = branch_log_array;

void branch_put_char(char *char_ptr)
{
	//printf("char_ptr:%s",char_ptr);

	while(*char_ptr != '\0')
	{
		*branch_log_ptr = *char_ptr;
		branch_log_ptr++;
		char_ptr++;
	}
	*branch_log_ptr = '\0';


}
void print_log(void)
{

	printf("%s\r\n",branch_log_array);

	branch_log_ptr = &branch_log_array[0];
//	memset(branch_log_array, 0, sizeof(branch_log_array));
	printf("\r\n\r\n\r\n\r\n");

}

#endif






void SmcSendCommand(uint8_t startCmd, uint8_t endCmd, uint8_t addrCycles, uint8_t endCmdPhase, int Page, int Column )
{
	uint32_t endCmdReq = 0;
	unsigned long cmdPhaseAddr  = 0;
	uint32_t cmdPhaseData  = 0;

	if(endCmdPhase == ONFI_ENDIN_CMD_PHASE)
	{
		branch_put_char("(109) -> ");
		endCmdReq = 1;
	}
	branch_put_char("(110) -> ");

	/* command phase address */
	cmdPhaseAddr  = NAND_BASE 							|
			(addrCycles << NAND_ADDR_CYCLES_SHIFT)		|
			(endCmdReq 	<< NAND_END_CMD_VALID_SHIFT)	|
			NAND_COMMAND_PHASE_FLAG						|
			(endCmd 	<< NAND_END_CMD_SHIFT)			|
			(startCmd 	<< NAND_START_CMD_SHIFT);

	/*
	 * command phase data
	 */
	if ((Column != NAND_COLUMN_NOT_VALID) && (Page != NAND_PAGE_NOT_VALID))
	{
		branch_put_char("(111) -> ");
		/* After Reset Cmd And Read Id Cmd */
		cmdPhaseData = Column;

		cmdPhaseData |= Page << (2*8);
		if (addrCycles > 4)
		{
			branch_put_char("(113) -> ");
			/*
			 * Send lower byte of page address
			 */
			SMC_WriteReg(cmdPhaseAddr, cmdPhaseData);
			/*
			 * build upper byte
			 */
			cmdPhaseData = Page >> (32 - (2*8));
		}
		branch_put_char("(114) -> ");
	}
	else if (Page != NAND_PAGE_NOT_VALID)
	{
		branch_put_char("(112) -> ");
		branch_put_char("(115) -> ");
		cmdPhaseData = Page;
	}
	else
	{
		branch_put_char("(112) -> ");
		branch_put_char("(116) -> ");
		cmdPhaseData = Column;
	}


	/* Send command phase */
	SMC_WriteReg(cmdPhaseAddr, cmdPhaseData);
}



void SmcReadPara(uint8_t endCmd, uint8_t endCmdPhase, uint8_t *Buf, uint32_t Length, Nand_TypeDef *nand)
{
	 uint32_t endCmdReq = 0;
	 unsigned long dataPhaseAddr = 0;
	 uint32_t eccLast = 0;
	 uint32_t clearCs = 0;
	 uint32_t Index = 0;

	if(endCmdPhase == ONFI_ENDIN_DATA_PHASE)
	{
		endCmdReq = 1;
	}

	/* data phase address */
	dataPhaseAddr  = NAND_BASE 							|
			(clearCs 	<< NAND_ADDR_CYCLES_SHIFT)		|
			(endCmdReq 	<< NAND_END_CMD_VALID_SHIFT)	|
			NAND_DATA_PHASE_FLAG						|
			(endCmd 	<< NAND_END_CMD_SHIFT)			|
			(eccLast 	<< NAND_ECC_LAST_SHIFT);

	/* Read Data */
	for(Index = 0;Index < Length;Index++)
	{
		Buf[Index] = SMC_Read8BitReg(dataPhaseAddr);
	}

}






void SmcWriteBuf(uint8_t endCmd, uint8_t endCmdPhase, uint8_t *Buf, uint32_t Length, uint32_t clearCs, uint32_t eccLast)
{
	uint32_t *tempBuff = (uint32_t *)Buf;
	uint32_t tempLength = Length >> 2;

	uint32_t endCmdReq = 0;
	unsigned long dataPhaseAddr = 0;
	uint32_t Index = 0;

	if(endCmdPhase == ONFI_ENDIN_DATA_PHASE)
	{
		endCmdReq = 1;
	}

	/* data phase address */
	dataPhaseAddr  = NAND_BASE 							|
			(clearCs 	<< NAND_ADDR_CYCLES_SHIFT)		|
			(endCmdReq 	<< NAND_END_CMD_VALID_SHIFT)	|
			NAND_DATA_PHASE_FLAG						|
			(endCmd 	<< NAND_END_CMD_SHIFT)			|
			(eccLast 	<< NAND_ECC_LAST_SHIFT);

	/* Write Data */
	for(Index = 0;Index < tempLength;Index++)
	{
		SMC_WriteReg(dataPhaseAddr, tempBuff[Index]);
	}
}


void SmcReadBuf(uint8_t endCmd, uint8_t endCmdPhase, uint8_t *Buf, uint32_t Length, uint32_t clearCs, uint32_t eccLast)
{
	uint32_t endCmdReq = 0;
	unsigned long dataPhaseAddr = 0;
	uint32_t Index = 0;

	uint32_t *tempBuff = (uint32_t *)Buf;
	uint32_t tempLength = Length >> 2;

	if(endCmdPhase == ONFI_ENDIN_DATA_PHASE)
	{
		endCmdReq = 1;
	}

	/* data phase address */
	dataPhaseAddr  = NAND_BASE 							|
			(clearCs 	<< NAND_ADDR_CYCLES_SHIFT)		|
			(endCmdReq 	<< NAND_END_CMD_VALID_SHIFT)	|
			NAND_DATA_PHASE_FLAG						|
			(endCmd 	<< NAND_END_CMD_SHIFT)			|
			(eccLast 	<< NAND_ECC_LAST_SHIFT);

	/* Read Data */
	for(Index = 0;Index < tempLength;Index++)
	{
		tempBuff[Index] = SMC_ReadReg(dataPhaseAddr);
	}

}












