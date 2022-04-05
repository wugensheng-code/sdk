#include "Onfi.h"
#include "demosoc.h"


void Onfi_CmdReadId(uint8_t Address)
{
	//SmcSendCommand(ONFI_CMD_READ_ID1, ONFI_CMD_READ_ID2, ONFI_CMD_READ_ID_CYCLES,ONFI_CMD_READ_ID_END_TIMING, ONFI_PAGE_NOT_VALID, Address);
}


void SmcReadData(uint8_t endCmd, uint8_t endCmdPhase, uint8_t *Buf, uint8_t Length)
{
	uint32_t endCmdReq = 0;
	uint32_t dataPhaseAddr = 0;
	uint32_t eccLast = 0;
	uint32_t clearCs = 0;
	uint32_t Index = 0;

	if(endCmdPhase == ONFI_ENDIN_DATA_PHASE)
	{
		endCmdReq = 1;
	}

	/*
	 * data phase address
	 */
	dataPhaseAddr  = NAND_BASE 		|
			(clearCs 	<< 21)		|
			(endCmdReq 	<< 20)		|
			NAND_DATA_PHASE_FLAG	|
			(endCmd 	<< 11)		|
			(eccLast 	<< 10);

	/* Read Data */
	for(Index = 0;Index < Length;Index++)
	{
		Buf[Index] = *((uint8_t *)dataPhaseAddr);
	}

}


void SmcReadData_ClearCs(uint8_t endCmd, uint8_t endCmdPhase, uint8_t *Buf, uint8_t Length)
{
	uint32_t endCmdReq = 0;
	uint32_t dataPhaseAddr = 0;
	uint32_t eccLast = 0;
	uint32_t clearCs = 1;
	uint32_t Index = 0;

	if(endCmdPhase == ONFI_ENDIN_DATA_PHASE)
	{
		endCmdReq = 1;
	}

	/*
	 * data phase address
	 */
	dataPhaseAddr  = NAND_BASE 		|
			(clearCs 	<< 21)		|
			(endCmdReq 	<< 20)		|
			NAND_DATA_PHASE_FLAG	|
			(endCmd 	<< 11)		|
			(eccLast 	<< 10);

	/* Read Data */
	for(Index = 0;Index < Length;Index++)
	{
		Buf[Index] = *((uint8_t *)dataPhaseAddr);
	}

}


void SmcSendCommand(uint8_t startCmd, uint8_t endCmd, uint8_t addrCycles, uint8_t endCmdPhase, int Page, int Column)
{
	uint32_t endCmdReq = 0;

	uint32_t cmdPhaseAddr  = 0;
	uint32_t cmdPhaseData  = 0;
	/*
	uint32_t eccLast = 0;
	uint32_t clearCs = 0;
	uint32_t pageOffset;
	 */
	if(endCmdPhase == ONFI_ENDIN_CMD_PHASE)
	{
		endCmdReq = 1;
	}

	/*
	 * command phase address
	 */
	cmdPhaseAddr  = NAND_BASE 		|
			(addrCycles << 21)		|
			(endCmdReq 	<< 20)		|
			NAND_COMMAND_PHASE_FLAG	|
			(endCmd 	<< 11)			|
			(startCmd 	<< 3);


	/*
	 * command phase data
	 */
	if (Column != NAND_COLUMN_NOT_VALID && Page != NAND_PAGE_NOT_VALID)
	{
		cmdPhaseData = Column;

		cmdPhaseData |= Page << (3*8);
		if (addrCycles > 4)
		{
			/*
			 * Send lower byte of page address
			 */
			SMC_WriteReg((void *)cmdPhaseAddr, cmdPhaseData);
			/*
			 * build upper byte
			 */
			cmdPhaseData = Page >> (32 - (3*8));
		}
	}
	else if (Page != NAND_PAGE_NOT_VALID)
	{
		cmdPhaseData = Page;
	}
	else
	{
		cmdPhaseData = Column;
	}
	/*
 	 * Send command phase
 	 */
	SMC_WriteReg((void *)cmdPhaseAddr, cmdPhaseData);
}



uint8_t Onfi_CmdReadStatus(void)
{
	uint8_t Status;
	uint32_t dataPhaseAddr = 0;

	SmcSendCommand(ONFI_CMD_READ_STATUS1, ONFI_CMD_READ_STATUS2, ONFI_CMD_READ_STATUS_CYCLES,
			ONFI_CMD_READ_STATUS_END_TIMING, ONFI_PAGE_NOT_VALID, ONFI_COLUMN_NOT_VALID);

	/*
	 * data phase address
	 */
	dataPhaseAddr  = NAND_BASE 		|
			(0 	<< 21)				|
			(0 	<< 20)				|
			NAND_DATA_PHASE_FLAG	|
			(0 	<< 11)				|
			(0 	<< 10);
	Status = *((uint8_t *)dataPhaseAddr);

	return Status;
}

void Onfi_CmdReset(void)
{
	uint8_t Status;

	SmcSendCommand(ONFI_CMD_RESET1, ONFI_CMD_RESET2, ONFI_CMD_RESET_CYCLES,
			ONFI_CMD_RESET_END_TIMING, ONFI_PAGE_NOT_VALID, ONFI_COLUMN_NOT_VALID);

	/*
	 * Check the Status Register SR[6]
	 * The device will be busy for a maximum of 1ms.
	 */
	do
	{
		Status = Onfi_CmdReadStatus();
	}
	while (!(Status & ONFI_STATUS_RDY));

}










