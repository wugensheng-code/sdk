
#include "mtimer.h"
#include "qspi_drv.h"
#include "qspi_flash_drv.h"

/************************** Variable Definitions *****************************/

static MtimerParams Timer;

static MtimerParams *QspiTimer = &Timer;

static u32 QspiFlashSize = 0U;
static u32 QspiFlashMake = 0U;
static u32 ReadCommand = 0U;

static QspiFlash_Msg FlashMsg;
static u8 ReadBuffer[10] __attribute__((aligned(32)));
static u8 WriteBuffer[10] __attribute__((aligned(32)));

QSPI_CORE_HANDLE QSPI_dev;
QSPI_CORE_HANDLE *g_pdev = &QSPI_dev;

#define EFUSE_QSPI_CLK_DIV_AND_DELAY_REG (0xF8806258ULL)

static void Config_QspiTx(QSPI_CORE_HANDLE *pdev, u32 start_level)
{
	Qspi_TxfifoStartLevelSet(
		pdev, start_level - 1); //transfer will start on serial line
}

static void Config_QspiRx(QSPI_CORE_HANDLE *pdev, u32 rx_numfame)
{
	Qspi_Ctrl1Ndf(pdev, rx_numfame - 1);
}

static u32 Wait_TxCompl(QSPI_CORE_HANDLE *pdev)
{
	MTIMER_OUT_CONDITION(300000, QspiTimer, !Qspi_TxfifoEmpty(pdev));

	NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num7))
	{
		QspiTimer->IsTimerOut = 1;
		NOR_SET_NUM_TO_ONE(nor_error_branch_num7);
	}

	if (Mtimer_IsTimerOut(QspiTimer) == 1) {
		branch_put_char("(120) -> ");
		return TIMER_OUT_ERR0;
	}
	branch_put_char("(119) -> ");
	return AL_SUCCESS;
}

void Config_EhanceMode(QSPI_CORE_HANDLE *pdev)
{
	Qspi_TransType(
		pdev,
		SPI_TRANSFER_TYPE0); //Instruction and Address will be sent in Standard SPI Mode.  the next transfer data will be in enhance mode spi

	Qspi_InstructionLength(pdev, 0x2);

	Qspi_WaitCycles(
		pdev, 0x8); //different vendor and device , different wait cycle
}

u32 Qspi_ResetFlashMode0(QSPI_CORE_HANDLE *pdev)
{
	uint32_t Reg = 0, EfuseDelayParam = 0;

	Qspi_Disable(pdev);
	Qspi_Mode(pdev, TMOD_TX_ONLY);
	Config_QspiTx(pdev, 1);
	Config_QspiRx(pdev, 1);
	Qspi_Enable(pdev);

	Qspi_DataTransmit(pdev, 0x66);
	Wait_TxCompl(pdev);

	MTIMER_OUT_CONDITION(300000, QspiTimer, Qspi_Busy(pdev));

	branch_put_char("(122) -> ");

	NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num8))
	{
		QspiTimer->IsTimerOut = 1;
		NOR_SET_NUM_TO_ONE(nor_error_branch_num8);
	}

	if (Mtimer_IsTimerOut(QspiTimer) == 1) {
		branch_put_char("(124) -> ");
		return TIMER_OUT_ERR1;
	}
	branch_put_char("(123) -> ");

	Qspi_Disable(pdev);
	Qspi_Mode(pdev, TMOD_TX_ONLY);
	Config_QspiTx(pdev, 1);
	Config_QspiRx(pdev, 1);
	Qspi_Enable(pdev);

	Qspi_DataTransmit(pdev, 0x99);
	Wait_TxCompl(pdev);

	MTIMER_OUT_CONDITION(300000, QspiTimer, Qspi_Busy(pdev));

	NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num9))
	{
		QspiTimer->IsTimerOut = 1;
		NOR_SET_NUM_TO_ONE(nor_error_branch_num9);
	}

	branch_put_char("(126) -> ");
	if (Mtimer_IsTimerOut(QspiTimer) == 1) {
		branch_put_char("(127) -> ");
		return TIMER_OUT_ERR2;
	}
	branch_put_char("(128) -> ");

	Mtimer_Delay(300 * 1000);

	Reg = REG32_READ(EFUSE_QSPI_CLK_DIV_AND_DELAY_REG);
	EfuseDelayParam = ((Reg >> 8)  & 0xffff);
	//printf("Qspi_ResetFlashMode0 EfuseDelayParam:%d\r\n",EfuseDelayParam);
	Mtimer_Delay(EfuseDelayParam * 5000);

	return AL_SUCCESS;
}

u32 Qspi_ResetFlashMode1(QSPI_CORE_HANDLE *pdev)
{
	uint32_t Reg = 0, EfuseDelayParam = 0;

	Qspi_Disable(pdev);
	Qspi_Mode(pdev, TMOD_TX_ONLY);
	Config_QspiTx(pdev, 1);
	Config_QspiRx(pdev, 1);
	Qspi_Enable(pdev);

	Qspi_DataTransmit(pdev, 0xF0);
	Wait_TxCompl(pdev);

	MTIMER_OUT_CONDITION(300000, QspiTimer, Qspi_Busy(pdev));

	branch_put_char("(130) -> ");

	NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num10))
	{
		QspiTimer->IsTimerOut = 1;
		NOR_SET_NUM_TO_ONE(nor_error_branch_num10);
	}

	if (Mtimer_IsTimerOut(QspiTimer) == 1) {
		branch_put_char("(131) -> ");
		return TIMER_OUT_ERR3;
	}
	branch_put_char("(132) -> ");
	Mtimer_Delay(300 * 1000);

	Reg = REG32_READ(EFUSE_QSPI_CLK_DIV_AND_DELAY_REG);
	EfuseDelayParam = ((Reg >> 8)  & 0xffff);
	//printf("Qspi_ResetFlashMode1 EfuseDelayParam:%d\r\n",EfuseDelayParam);
	Mtimer_Delay(EfuseDelayParam * 5000);

	return AL_SUCCESS;
}

//for gd, disable QPI, 0xff cmd
u32 Qspi_ReturnCase1(QSPI_CORE_HANDLE *pdev)
{
	Qspi_Disable(pdev);
	Qspi_Mode(pdev, TMOD_TX_ONLY);
	Qspi_TransType(pdev, SPI_TRANSFER_TYPE2);
	Qspi_AddrMode(pdev, SPI_FLASH_ADDR0);
	Config_QspiTx(pdev, 1);
	Config_QspiRx(pdev, 1);
	Qspi_Enable(pdev);
	Qspi_DataTransmit(pdev, 0xFF);
	Wait_TxCompl(pdev);

	MTIMER_OUT_CONDITION(300000, QspiTimer, Qspi_Busy(pdev));

	NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num11))
	{
		QspiTimer->IsTimerOut = 1;
		NOR_SET_NUM_TO_ONE(nor_error_branch_num11);
	}

	branch_put_char("(134) -> ");
	if (Mtimer_IsTimerOut(QspiTimer) == 1) {
		branch_put_char("(135) -> ");
		return TIMER_OUT_ERR6;
	}
	branch_put_char("(136) -> ");

	return AL_SUCCESS;
}

//for mxic, five 0xff exit ehnace mode, 0xf5 exit QPI
u32 Qspi_ReturnCase2(QSPI_CORE_HANDLE *pdev)
{
	Qspi_Disable(pdev);
	Qspi_Mode(pdev, TMOD_TX_ONLY);
	Qspi_TransType(pdev, SPI_TRANSFER_TYPE2);
	Qspi_AddrMode(pdev, SPI_FLASH_ADDR0);
	Config_QspiTx(pdev, 1);
	Config_QspiRx(pdev, 1);
	Qspi_Enable(pdev);

	Qspi_DataTransmit(pdev, 0xF5);
	Wait_TxCompl(pdev);

	MTIMER_OUT_CONDITION(300000, QspiTimer, Qspi_Busy(pdev));
	branch_put_char("(138) -> ");

	NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num12))
	{
		QspiTimer->IsTimerOut = 1;
		NOR_SET_NUM_TO_ONE(nor_error_branch_num12);
	}

	if (Mtimer_IsTimerOut(QspiTimer) == 1) {
		branch_put_char("(139) -> ");
		return TIMER_OUT_ERR8;
	}
	branch_put_char("(140) -> ");

	return AL_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function performs a transfer on the bus in polled mode. The messages
* passed are all transferred on the bus between one CS assert and de-assert.
*
* @param    pdev is a pointer to the QSPI instance.
* @param    Msg is a pointer to the structure containing transfer data.
*
* @return
*
* @note     None.
* becase spi width and dfs, address mode 24/32, three factor impact the spi format, so wo can only support
* spi x1: 24/32, dfs = 8
* spi x2/x4: 24/31,dfs = 8
*
******************************************************************************/
static u32 Qspi_PolledTransfer(QSPI_CORE_HANDLE *pdev, QspiFlash_Msg *Msg)
{
	u32 Index;
	u32 Tdata = 0;
	u32 Rdata = 0;
	u32 Status = AL_SUCCESS;

	/*
     * Set the busy flag, which will be cleared when the transfer is
     * entirely done.
     */

	Index = 0;
	if (Msg->BusWidth == QSPI_WIDTH_X1) {
		branch_put_char("(61) -> ");

		MTIMER_OUT_CONDITION(500000, QspiTimer, Msg->txByteCount > 0)
		{
			branch_put_char("(63) -> ");
			Qspi_DataTransmit(pdev, Msg->TxBfrPtr[Index]);
			Index++;
			Msg->txByteCount--;
		}
		branch_put_char("(64) -> ");

		NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num3))
		{
			QspiTimer->IsTimerOut = 1;
			NOR_SET_NUM_TO_ONE(nor_error_branch_num3);
		}

		if (Mtimer_IsTimerOut(QspiTimer) == 1) {
			branch_put_char("(66) -> ");
			return DATA_XFER_ERR0;
		}
		branch_put_char("(65) -> ");

		Status = Wait_TxCompl(pdev);
		Index = 0;

		MTIMER_OUT_CONDITION(500000, QspiTimer, Msg->rxByteCount > 0)
		{
			branch_put_char("(67) -> ");
			if (Qspi_RxNotempty(pdev)) // RXFIFO not empty
			{
				branch_put_char("(70) -> ");
				Msg->RxBfrPtr[Index] =
					Qspi_DataRead(pdev) & 0xFF;
				Index++;
				Msg->rxByteCount--;
			}
			NOR_TEST_ELSE
			{
				branch_put_char("(69) -> ");
			}
		}
		branch_put_char("(68) -> ");

		NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num4))
		{
			QspiTimer->IsTimerOut = 1;
			NOR_SET_NUM_TO_ONE(nor_error_branch_num4);
		}

		if (Mtimer_IsTimerOut(QspiTimer) == 1) {
			branch_put_char("(72) -> ");
			return DATA_XFER_ERR1;
		}
		branch_put_char("(71) -> ");
	} else if ((Msg->BusWidth == QSPI_WIDTH_X2) ||
		   (Msg->BusWidth == QSPI_WIDTH_X4)) {
		branch_put_char("(62) -> ");


		NOR_TEST_IF(Msg->BusWidth == QSPI_WIDTH_X2)
		{
			branch_put_char("(51) -> ");
		}

		NOR_TEST_IF(Msg->BusWidth == QSPI_WIDTH_X4)
		{
			branch_put_char("(146) -> ");
		}

		MTIMER_OUT_CONDITION(1000000, QspiTimer, Msg->txByteCount > 0)
		{
			branch_put_char("(74) -> ");

			if (Index == 0) {
				branch_put_char("(75) -> ");
				Tdata = Msg->TxBfrPtr[0];
				Qspi_DataTransmit(pdev, Tdata);
				Index++;
				Msg->txByteCount--;
			} else if (Index == 1) {
				branch_put_char("(76) -> ");
				branch_put_char("(79) -> ");

				if (pdev->cfg.addr_mode == SPI_FLASH_ADDR32) {
					branch_put_char("(81) -> ");

					Tdata = (Msg->TxBfrPtr[1] << 24) |
						(Msg->TxBfrPtr[2] << 16) |
						(Msg->TxBfrPtr[3] << 8) |
						(Msg->TxBfrPtr[4]);
					Index += 4;
					Msg->txByteCount -= 4;
				} else if (pdev->cfg.addr_mode ==
					   SPI_FLASH_ADDR24) {
					branch_put_char("(82) -> ");
					branch_put_char("(84) -> ");

					Tdata = (Msg->TxBfrPtr[1] << 16) |
						(Msg->TxBfrPtr[2] << 8) |
						(Msg->TxBfrPtr[3]);
					Index += 3;
					Msg->txByteCount -= 3;
				}
				NOR_TEST_ELSE
				{
					branch_put_char("(82) -> ");
					branch_put_char("(83) -> ");
				}

				Qspi_DataTransmit(pdev, Tdata);
			} 
		}
		branch_put_char("(73) -> ");

		NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num5))
		{
			QspiTimer->IsTimerOut = 1;
			NOR_SET_NUM_TO_ONE(nor_error_branch_num5);
		}

		if (Mtimer_IsTimerOut(QspiTimer) == 1) {
			branch_put_char("(77) -> ");
			return DATA_XFER_ERR2;
		}
		branch_put_char("(78) -> ");

		Status = Wait_TxCompl(pdev);
		Index = 0;

		MTIMER_OUT_CONDITION(1000000, QspiTimer, Msg->rxByteCount > 0)
		{
			branch_put_char("(85) -> ");
			if (Qspi_RxNotempty(
				    pdev)) // RXFIFO not empty, is must 4byte aligned
			{
				branch_put_char("(88) -> ");
				Rdata = Qspi_DataRead(pdev);
				Msg->RxBfrPtr[Index] = Rdata & 0xFF;
				Index++;
				Msg->rxByteCount--;
			}
			NOR_TEST_ELSE
			{
				branch_put_char("(87) -> ");
			}

		}
		branch_put_char("(86) -> ");

		NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num6))
		{
			QspiTimer->IsTimerOut = 1;
			NOR_SET_NUM_TO_ONE(nor_error_branch_num6);
		}

		if (Mtimer_IsTimerOut(QspiTimer) == 1) {
			branch_put_char("(89) -> ");
			return DATA_XFER_ERR3;
		}
		branch_put_char("(90) -> ");
	}

	return Status;
}

/******************************************************************************
*
* This function reads serial FLASH ID connected to the SPI interface.
* It then deduces the make and size of the flash
* The flash driver will function based on this and
* it presently supports Micron and Spansion - 128, 256 and 512Mbit and
* Winbond 128Mbit
*
* @param    pdev
*
* @return   QSPI_SUCCESS OR UNSUPPORTED_QSPI
*
* @note     None.
*
******************************************************************************/
static u32 Flash_ReadID(QSPI_CORE_HANDLE *pdev)
{
	uint32_t index;
	uint32_t flashID = 0;
	uint32_t UStatus = AL_SUCCESS;

	Qspi_Disable(pdev);
	Config_QspiRx(pdev, 3);
	Config_QspiTx(
		pdev,
		1); //when rdid cmd send,transfer will start on serial line
	Qspi_Enable(pdev);

	//bellow read flash ID

	Qspi_DataTransmit(pdev, READ_ID_CMD);

	UStatus = Wait_TxCompl(pdev);

	index = 0;

	MTIMER_OUT_CONDITION(500000, QspiTimer, index < 3)
	{
		branch_put_char("(27) -> ");
		if (Qspi_RxNotempty(pdev)) {
			branch_put_char("(29) -> ");
			ReadBuffer[index] = Qspi_DataRead(pdev) & 0xff;
			index++;
		}
		NOR_TEST_ELSE
		{
			branch_put_char("(30) -> ");
		}
	}
	branch_put_char("(28) -> ");

	NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num2))
	{
		QspiTimer->IsTimerOut = 1;
		NOR_SET_NUM_TO_ONE(nor_error_branch_num2);
	}

	if (Mtimer_IsTimerOut(QspiTimer) == 1) {
		branch_put_char("(31) -> ");
		return FLASH_ID_ERR0;
	}
	branch_put_char("(32) -> ");

	flashID = ReadBuffer[0] | (ReadBuffer[1] << 8) | (ReadBuffer[2] << 16);
	pdev->cfg.flashID = flashID;

	NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num8)||
			NOR_NUM_UNEQUAL_ONE(nor_error_branch_num9)||
			NOR_NUM_UNEQUAL_ONE(nor_error_branch_num10)||
			NOR_NUM_UNEQUAL_ONE(nor_error_branch_num11)||
			NOR_NUM_UNEQUAL_ONE(nor_error_branch_num12))
	{
		ReadBuffer[0] = 0x00;
	}

	/*
     * Deduce flash make
     */
	if ((ReadBuffer[0] == 0x00) || (ReadBuffer[0] == 0xff)) {

		NOR_TEST_IF(ReadBuffer[0] == 0x00)
		{
			branch_put_char("(33) -> ");
		}

		NOR_TEST_IF(ReadBuffer[0] == 0xff)
		{
			branch_put_char("(147) -> ");
		}

		UStatus = FLASH_ID_ERR1;
	} else {
		branch_put_char("(34) -> ");
		QspiFlashMake = ReadBuffer[0];
	}

	/*
     * Deduce flash Size
     */

	QspiFlashSize = FLASH_SIZE_128M;

	if (ReadBuffer[2] == FLASH_SIZE_ID_2M) {
		branch_put_char("(35) -> ");
		QspiFlashSize = FLASH_SIZE_2M;
	} else if (ReadBuffer[2] == FLASH_SIZE_ID_4M) {
		branch_put_char("(37) -> ");
		QspiFlashSize = FLASH_SIZE_4M;
	} else if (ReadBuffer[2] == FLASH_SIZE_ID_8M) {
		branch_put_char("(39) -> ");
		QspiFlashSize = FLASH_SIZE_8M;
	} else if (ReadBuffer[2] == FLASH_SIZE_ID_16M) {
		branch_put_char("(41) -> ");
		QspiFlashSize = FLASH_SIZE_16M;
	} else if (ReadBuffer[2] == FLASH_SIZE_ID_32M) {
		branch_put_char("(43) -> ");
		QspiFlashSize = FLASH_SIZE_32M;
	} else if (ReadBuffer[2] == FLASH_SIZE_ID_64M) {
		branch_put_char("(45) -> ");
		QspiFlashSize = FLASH_SIZE_64M;
	} else if (ReadBuffer[2] == FLASH_SIZE_ID_128M) {
		branch_put_char("(47) -> ");
		QspiFlashSize = FLASH_SIZE_128M;
	} else if ((ReadBuffer[2] == FLASH_SIZE_ID_256M) ||
		   (ReadBuffer[2] == MACRONIX_FLASH_1_8_V_MX25_ID_256)) {

		NOR_TEST_IF(ReadBuffer[2] == FLASH_SIZE_ID_256M)
		{
			branch_put_char("(49) -> ");
		}

		NOR_TEST_IF(ReadBuffer[2] == MACRONIX_FLASH_1_8_V_MX25_ID_256)
		{
			branch_put_char("(148) -> ");
		}

		QspiFlashSize = FLASH_SIZE_256M;
	} else if ((ReadBuffer[2] == FLASH_SIZE_ID_512M) ||
		   (ReadBuffer[2] == MACRONIX_FLASH_SIZE_ID_512M) ||
		   (ReadBuffer[2] == MACRONIX_FLASH_1_8_V_MX66_ID_512)) {

		NOR_TEST_IF(ReadBuffer[2] == FLASH_SIZE_ID_512M)
		{
			branch_put_char("(54) -> ");
		}

		NOR_TEST_IF(ReadBuffer[2] == MACRONIX_FLASH_SIZE_ID_512M)
		{
			branch_put_char("(150) -> ");
		}

		NOR_TEST_IF(ReadBuffer[2] == MACRONIX_FLASH_1_8_V_MX66_ID_512)
		{
			branch_put_char("(149) -> ");
		}


		QspiFlashSize = FLASH_SIZE_512M;
	} else if ((ReadBuffer[2] == FLASH_SIZE_ID_1G) ||
		   (ReadBuffer[2] == MACRONIX_FLASH_SIZE_ID_1G) ||
		   (ReadBuffer[2] == MACRONIX_FLASH_1_8_V_SIZE_ID_1G)) {

		NOR_TEST_IF(ReadBuffer[2] == FLASH_SIZE_ID_1G)
		{
			branch_put_char("(56) -> ");
		}

		NOR_TEST_IF(ReadBuffer[2] == MACRONIX_FLASH_SIZE_ID_1G)
		{
			branch_put_char("(151) -> ");
		}

		NOR_TEST_IF(ReadBuffer[2] == MACRONIX_FLASH_1_8_V_SIZE_ID_1G)
		{
			branch_put_char("(152) -> ");
		}

		QspiFlashSize = FLASH_SIZE_1G;
	} else if ((ReadBuffer[2] == FLASH_SIZE_ID_2G) ||
		   (ReadBuffer[2] == MACRONIX_FLASH_SIZE_ID_2G) ||
		   (ReadBuffer[2] == MACRONIX_FLASH_1_8_V_SIZE_ID_2G)) {

		NOR_TEST_IF(ReadBuffer[2] == FLASH_SIZE_ID_2G)
		{
			branch_put_char("(57) -> ");
		}

		NOR_TEST_IF(ReadBuffer[2] == MACRONIX_FLASH_SIZE_ID_2G)
		{
			branch_put_char("(153) -> ");
		}

		NOR_TEST_IF(ReadBuffer[2] == MACRONIX_FLASH_1_8_V_SIZE_ID_2G)
		{
			branch_put_char("(154) -> ");
		}

		QspiFlashSize = FLASH_SIZE_2G;
	} else {
		//UStatus = FLASH_ID_ERR2;
		branch_put_char("(58) -> ");
		QspiFlashSize = FLASH_SIZE_128M;
	}

	if (QspiFlashSize > FLASH_SIZE_128M) {
		branch_put_char("(60) -> ");
		ReadCommand = BASE_READ_CMD_32BIT;
	} else {
		branch_put_char("(59) -> ");
		ReadCommand = BASE_READ_CMD_24BIT;
	}

	pdev->cfg.flashSize = QspiFlashSize;

	return UStatus;
}

static u32 Qspi_Copy(u32 SrcAddress, u8 *DestAddress, u32 Length)
{
	u32 Status = AL_SUCCESS;
	u32 DiscardByteCnt = 0;

	WriteBuffer[COMMAND_OFFSET] = (u8)ReadCommand;

	if (ReadCommand == BASE_READ_CMD_24BIT ||
	    ReadCommand == DUAL_READ_CMD_24BIT ||
	    ReadCommand == QUAD_READ_CMD_24BIT) {

		NOR_TEST_IF(ReadCommand == BASE_READ_CMD_24BIT)
		{
			branch_put_char("(12) -> ");
		}

		NOR_TEST_IF(ReadCommand == DUAL_READ_CMD_24BIT)
		{
			branch_put_char("(141) -> ");
		}

		NOR_TEST_IF(ReadCommand == QUAD_READ_CMD_24BIT)
		{
			branch_put_char("(142) -> ");
		}

		WriteBuffer[ADDRESS_1_OFFSET] =
			(u8)((SrcAddress & 0xFF0000U) >> 16);
		WriteBuffer[ADDRESS_2_OFFSET] =
			(u8)((SrcAddress & 0xFF00U) >> 8);
		WriteBuffer[ADDRESS_3_OFFSET] = (u8)(SrcAddress & 0xFFU);
		DiscardByteCnt = 4;
	} else if (ReadCommand == BASE_READ_CMD_32BIT ||
		   ReadCommand == DUAL_READ_CMD_32BIT ||
		   ReadCommand == QUAD_READ_CMD_32BIT) {
		branch_put_char("(11) -> ");

		NOR_TEST_IF(ReadCommand == BASE_READ_CMD_32BIT)
		{
			branch_put_char("(14) -> ");
		}

		NOR_TEST_IF(ReadCommand == DUAL_READ_CMD_32BIT)
		{
			branch_put_char("(143) -> ");
		}

		NOR_TEST_IF(ReadCommand == QUAD_READ_CMD_32BIT)
		{
			branch_put_char("(144) -> ");
		}

		WriteBuffer[ADDRESS_1_OFFSET] =
			(u8)((SrcAddress & 0xFF000000U) >> 24);
		WriteBuffer[ADDRESS_2_OFFSET] =
			(u8)((SrcAddress & 0xFF0000U) >> 16);
		WriteBuffer[ADDRESS_3_OFFSET] =
			(u8)((SrcAddress & 0xFF00U) >> 8);
		WriteBuffer[ADDRESS_4_OFFSET] = (u8)(SrcAddress & 0xFFU);
		DiscardByteCnt = 5;
	}
	NOR_TEST_ELSE
	{
		branch_put_char("(11) -> ");
		branch_put_char("(13) -> ");
	}

	FlashMsg.TxBfrPtr = WriteBuffer;
	FlashMsg.RxBfrPtr = (u8 *)DestAddress;
	FlashMsg.txByteCount = DiscardByteCnt;
	FlashMsg.rxByteCount = Length;
	FlashMsg.BusWidth = g_pdev->cfg.width;
	FlashMsg.Flags = XQSPIPSU_MSG_FLAG_TX | XQSPIPSU_MSG_FLAG_RX;

	if (g_pdev->cfg.width == SPI_FRF_X1_MODE) {
		branch_put_char("(16) -> ");
		Qspi_Disable(g_pdev);
		Config_QspiTx(g_pdev, DiscardByteCnt);
		Config_QspiRx(g_pdev, Length);
		Qspi_Enable(g_pdev);
	} else if (g_pdev->cfg.width == SPI_FRF_X2_MODE ||
		   g_pdev->cfg.width == SPI_FRF_X4_MODE) {
		branch_put_char("(15) -> ");


		NOR_TEST_IF(g_pdev->cfg.width == SPI_FRF_X2_MODE)
		{
			branch_put_char("(17) -> ");
		}

		NOR_TEST_IF(g_pdev->cfg.width == SPI_FRF_X4_MODE)
		{
			branch_put_char("(161) -> ");
		}

		Qspi_Disable(g_pdev);
		Config_QspiTx(g_pdev, DiscardByteCnt / 4 + 1);
		Config_QspiRx(g_pdev, Length);
		Qspi_Enable(g_pdev);
	}
	NOR_TEST_ELSE
	{
		branch_put_char("(15) -> ");
		branch_put_char("(18) -> ");
	}

	/**
    * Send the read command to the Flash to read the specified number
    * of bytes from the Flash, send the read command and address and
    * receive the specified number of bytes of data in the data buffer
    */
	Status = Qspi_PolledTransfer(g_pdev, &FlashMsg);

	return Status;
}

/*!
    \brief       Csu_QspiInit：QSPI Initlization, Read flash ID and parse the size of flash.
    \param[in]  : params is the point of the flash information
    \param[out] : flash information  connected with the spi
    \retval     : 0
*/
u32 Csu_QspiInit(QspiParams *params)
{
	u32 Status = AL_SUCCESS;

	Mtimer_Init(QspiTimer);

	Qspi_Init(g_pdev);

	Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24);

	Status = Flash_ReadID(g_pdev);

	if (Status != AL_SUCCESS) {
		branch_put_char("(2) -> ");
		Qspi_ResetFlashMode0(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24);
		Status = Flash_ReadID(g_pdev);	
	}
	branch_put_char("(1) -> ");

	if (Status != AL_SUCCESS) {
		branch_put_char("(4) -> ");
		Qspi_ResetFlashMode1(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24);
		Status = Flash_ReadID(g_pdev);
	}
	branch_put_char("(3) -> ");

	if (Status != AL_SUCCESS) {
		branch_put_char("(6) -> ");
		Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_24);
		Qspi_ReturnCase1(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24);
		Qspi_ResetFlashMode0(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24);
		Status = Flash_ReadID(g_pdev);
	}
	branch_put_char("(5) -> ");

	if (Status != AL_SUCCESS) {
		branch_put_char("(8) -> ");
		Csu_QspiSetMode(QSPI_WIDTH_X4, QSPI_ADDR_24);
		Qspi_ReturnCase2(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24);
		Qspi_ResetFlashMode0(g_pdev);
		Csu_QspiSetMode(QSPI_WIDTH_X1, QSPI_ADDR_24);
		Status = Flash_ReadID(g_pdev);
	}
	branch_put_char("(7) -> ");

	params->flashID = g_pdev->cfg.flashID;
	params->flashSize = g_pdev->cfg.flashSize; //return size in byte

	return Status;
}

/*!
    \brief       Csu_QspiSetMode：QSPI width set.
    \param[in]  : Mode is spi width:
                  QSPI_WIDTH_X1: QSPI X1
                  QSPI_WIDTH_X2: QSPI X2
                  QSPI_WIDTH_X4: QSPI X4
                : AddrMode
                 QSPI_ADDR_24: force 24bit address width
                 QSPI_ADDR_32: force 32bit address width

    \param[out] : no
    \retval     : 0
*/
u32 Csu_QspiSetMode(u32 Mode, u32 AddrMode)
{
	uint8_t Reg = 0;

	Qspi_Disable(g_pdev);

	if (Mode == QSPI_WIDTH_X1 || Mode == QSPI_WIDTH_X2 ||
	    Mode == QSPI_WIDTH_X4) {

		branch_put_char("(91) -> ");

		Reg = (REG32_READ(EFUSE_QSPI_CLK_DIV_AND_DELAY_REG) >> 24);

		Qspi_Width(g_pdev, Mode);

		if (Mode == QSPI_WIDTH_X1) {
			branch_put_char("(93) -> ");
			Qspi_Mode(g_pdev, TMOD_EEPROM);
			Qspi_Dfs(g_pdev, DFS_BYTE);
			Qspi_Ser(g_pdev, SLAVE_CS0_SEL);
			Qspi_TransType(g_pdev, SPI_TRANSFER_TYPE0);
			Qspi_AddrMode(g_pdev, SPI_FLASH_ADDR24); /* useless */
			Qspi_RxftlCfg(g_pdev, 0x3F);
			if (Reg != 0) {
				branch_put_char("(177) -> ");
				Qspi_SckdivCfg(g_pdev, Reg);
			}
			NOR_TEST_ELSE
			{
				branch_put_char("(178) -> ");
			}
		} else if (Mode == QSPI_WIDTH_X2 || Mode == QSPI_WIDTH_X4) {

			branch_put_char("(94) -> ");
			NOR_TEST_IF(Mode == QSPI_WIDTH_X2)
			{
				branch_put_char("(96) -> ");
			}

			NOR_TEST_IF(Mode == QSPI_WIDTH_X4)
			{
				branch_put_char("(145) -> ");
			}

			Qspi_Mode(g_pdev, TMOD_RX_ONLY);
			Qspi_Dfs(g_pdev, DFS_BYTE);
			Qspi_Ser(g_pdev, SLAVE_CS0_SEL);
			Qspi_RxftlCfg(
				g_pdev,
				0x3F); //rx fifo = 128 frame, set half of depth generate rx fifo full interrupt
			if (Reg != 0) {
				branch_put_char("(176) -> ");
				Qspi_SckdivCfg(g_pdev, Reg);
			}
			NOR_TEST_ELSE
			{
				branch_put_char("(175) -> ");
			}
			Qspi_TransType(g_pdev, SPI_TRANSFER_TYPE0);

			if (AddrMode == QSPI_ADDR_32) {
				branch_put_char("(97) -> ");
				Qspi_AddrMode(g_pdev, SPI_FLASH_ADDR32);
			} else if (AddrMode == QSPI_ADDR_24) {
				branch_put_char("(98) -> ");
				branch_put_char("(99) -> ");
				Qspi_AddrMode(g_pdev, SPI_FLASH_ADDR24);
			}
			NOR_TEST_ELSE
			{
				branch_put_char("(98) -> ");
				branch_put_char("(100) -> ");
			}

			Config_EhanceMode(g_pdev);
		}
		NOR_TEST_ELSE
		{
			branch_put_char("(94) -> ");
			branch_put_char("(95) -> ");
		}
	}
	NOR_TEST_ELSE
	{
		branch_put_char("(92) -> ");
	}

	if (Mode == QSPI_WIDTH_X1) {
		branch_put_char("(101) -> ");

		if (AddrMode == QSPI_ADDR_24) { //force address to 24BIT
			branch_put_char("(103) -> ");
			ReadCommand = BASE_READ_CMD_24BIT;
		} else if (AddrMode == QSPI_ADDR_32) { //force address to 32BIT
			branch_put_char("(104) -> ");
			branch_put_char("(105) -> ");
			ReadCommand = BASE_READ_CMD_32BIT;
		}
		NOR_TEST_ELSE
		{
			branch_put_char("(104) -> ");
			branch_put_char("(106) -> ");
		}

	} else if (Mode == QSPI_WIDTH_X2) {
		branch_put_char("(102) -> ");
		branch_put_char("(107) -> ");

		if (AddrMode == QSPI_ADDR_24) { //force address to 24BIT
			branch_put_char("(109) -> ");
			ReadCommand = DUAL_READ_CMD_24BIT;
		} else if (AddrMode == QSPI_ADDR_32) { //force address to 32BIT
			branch_put_char("(110) -> ");
			branch_put_char("(117) -> ");
			ReadCommand = DUAL_READ_CMD_32BIT;
		}
		NOR_TEST_ELSE
		{
			branch_put_char("(110) -> ");
			branch_put_char("(118) -> ");
		}

	} else if (Mode == QSPI_WIDTH_X4) {
		branch_put_char("(102) -> ");
		branch_put_char("(108) -> ");
		branch_put_char("(111) -> ");

		if (AddrMode == QSPI_ADDR_24) { //force address to 24BIT
			branch_put_char("(113) -> ");
			ReadCommand = QUAD_READ_CMD_24BIT;
		} else if (AddrMode == QSPI_ADDR_32) { //force address to 32BIT
			branch_put_char("(114) -> ");
			branch_put_char("(115) -> ");
			ReadCommand = QUAD_READ_CMD_32BIT;
		}
		NOR_TEST_ELSE
		{
			branch_put_char("(114) -> ");
			branch_put_char("(116) -> ");
		}
	}
	NOR_TEST_ELSE
	{
		branch_put_char("(102) -> ");
		branch_put_char("(108) -> ");
		branch_put_char("(112) -> ");
	}

	Qspi_Enable(g_pdev);

	return AL_SUCCESS;
}

/*!
    \brief       Csu_QspiRead：QSPI flash data read.
    \param[in]  : offset-address of flash space
                  dest-the user buffer point
                  length-read data length
    \param[out] : no
    \retval     : 0 or -2,-3
*/

u32 Csu_QspiRead(u32 offset, u8 *dest, u32 length)
{
	u32 RemainingBytes = 0;
	u32 TransferBytes = 0;
	u32 Status = AL_SUCCESS;

	NOR_ERROR_IF(NOR_NUM_UNEQUAL_ONE(nor_error_branch_num1))
	{
		QspiFlashSize = (offset + length)-1;
		NOR_SET_NUM_TO_ONE(nor_error_branch_num1);
	}

	/**
     * Check the read length with Qspi flash size
     */
	if ((offset + length) > QspiFlashSize) {
		branch_put_char("(19) -> ");
		return FLASH_READ_ERR0;
	}
	branch_put_char("(20) -> ");

	RemainingBytes = length;

	while (RemainingBytes > 0U) {
		branch_put_char("(173) -> ");
		if (RemainingBytes > QSPI_FIFO_MAX_SIZE) {
			branch_put_char("(21) -> ");
			TransferBytes = QSPI_FIFO_MAX_SIZE;
		} else {
			branch_put_char("(22) -> ");
			TransferBytes = RemainingBytes;
		}
		Status = Qspi_Copy(offset, dest, TransferBytes);

		if (Status == AL_SUCCESS) {
			branch_put_char("(23) -> ");
			/**
            * Update the variables
            */
			RemainingBytes -= TransferBytes;
			dest += TransferBytes;
			offset += TransferBytes;
		} else {
			branch_put_char("(24) -> ");

			return Status;
		}
	}

	branch_put_char("(26) -> ");
	return AL_SUCCESS;
}
