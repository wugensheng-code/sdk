/*****************************************************************************/
/**
*
* @file sd_test.c
* @addtogroup sdps_v3_13
* @{
*
* The implementation of the XSdPs component's static initialization
* functionality.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ---    -------- -----------------------------------------------
* 1.00a hk/sg  10/17/13 Initial release
*       kvn    07/15/15 Modified the code according to MISRAC-2012.
* 3.7   aru    03/12/19 Modified the code according to MISRAC-2012.
*
* </pre>
*
******************************************************************************/

#include "sdio_test.h"
#include <string.h>
#include <stdio.h>
#include "ff.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define BLOCK_SIZE            512 /* Block Size in Bytes */
#define NUMBER_OF_BLOCKS      10  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)

#define vfwp printf
#define TOP_NS__CFG_CTRL_SDIO0_ADDR 0xF8800154
#define SDIO_WRAP__SDIO0__BASE_ADDR 0xF804A000ULL
#define TUBE_ADDRESS ((volatile char *) 0xF8800010u)

SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 1;
static uint32_t CardType =  SDIO_HIGH_CAPACITY_SD_CARD;
FATFS fs;
FRESULT res_sd;
uint8_t flag = 0;
static unsigned int rca = 0;
SD_CardInfo SDCardInfo;
static volatile DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint32_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];

static void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
static void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);

unsigned int reg_read(unsigned long long reg_address)
{
    return *((volatile unsigned int *)reg_address);
}

void reg_write(unsigned long long reg_address, unsigned reg_wdata)
{
    *((volatile unsigned *)reg_address) = reg_wdata;
}

static void sleep(int tick)
{
    for (int i = 0; i < tick; i++)
    {
        asm volatile("NOP");
    }
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferLength: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  * @retval None
  */
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++)
  {
    pBuffer[index] = index + Offset;
  }
}

static void wait_command_complete()
{
    for (;;)
    {
        if (SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE == 1)
        {
            SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE = 1;
            break;
        }
    }
}

static void wait_transfer_complete()
{
    for (;;)
    {
        if (SDIO->NORMAL_INT_STAT_R.XFER_COMPLETE == 1)
        {
            SDIO->NORMAL_INT_STAT_R.XFER_COMPLETE = 1;
            break;
        }
    }
}

static void wait_buffer_read_ready_complete()
{
    for (;;)
    {
        if (SDIO->NORMAL_INT_STAT_R.BUF_RD_READY == 1)
        {
            SDIO->NORMAL_INT_STAT_R.BUF_RD_READY = 1;
            break;
        }
    }
}

/*****************************************************************************/
/**
*
* @brief    Performs an output operation for a memory location by writing the
*           32 bit Value to the the specified address.
*
* @param	Addr contains the address to perform the output operation
* @param	Value contains the 32 bit Value to be written at the specified
*           address.
*
* @return	None.
*
******************************************************************************/
static INLINE void Xil_Out32(UINTPTR Addr, u32 Value)
{
    /* write 32 bit value to specified address */

    volatile u32 *LocalAddr = (volatile u32 *)Addr;
    *LocalAddr = Value;
}

/***************************************************************************/
/**
 * @brief	Write to the register
 *
 * @param	BaseAddress   - Contains the base address of the device
 * @param	RegOffset     - Contains the offset from the base address of the
 *				device
 * @param	RegisterValue - Is the value to be written to the register
 *
 ******************************************************************************/
static inline void XSecure_WriteReg(u32 BaseAddress,
        u32 RegOffset, u32 RegisterValue)
{
    Xil_Out32((volatile u32 *)(BaseAddress + RegOffset), RegisterValue);
}

/***************************************************************************/
/**
 * @brief	check the card inserted or not inserted
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 CardDetection()
{
    u32 Status = XST_FAILURE;
    //  Card Detection
    SDIO->NORMAL_INT_STAT_EN_R = 0x2FF;
    SDIO->ERROR_INT_STAT_EN_R = 0x0;
    SDIO->NORMAL_INT_SIGNAL_EN_R = 0xC0;
    SDIO->ERROR_INT_SIGNAL_EN_R = 0x0;
    SDIO->NORMAL_INT_STAT_R.D16 = 0xC0;
    SDIO->ERROR_INT_STAT_R.D16 = 0x0;
    sleep(200);
    u32 CardStatus = 0;
    while (!CardStatus)
    {
        CardStatus = (((SDIO->PSTATE_REG_R.CARD_INSERTED) == 1) ? 1:0);
    	if (CardStatus == 1)
    	{
            Status = XST_SUCCESS;
    	    break;
    	}
    }
    	
    return Status;
}

/***************************************************************************/
/**
 * @brief	init the SD/EMMC host controller
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 HostControllerSetup()
{
    u32 Status;
    //  Host Controller Setup
    SDIO->HOST_CTRL1_R = 0x10;
    SDIO->PWR_CTRL_R = 0xBF;
    SDIO->BGAP_CTRL_R = 0;
    SDIO->WUP_CTRL_R = 0;

    SDIO->CLK_CTRL_R = 0xB;
    SDIO->TOUT_CTRL_R = 0;
    SDIO->SW_RST_R = 0;

    SDIO->AUTO_CMD_STAT_R = 0;
    SDIO->HOST_CTRL2_R = 0;

    Status = XST_SUCCESS;
    return Status;
}

/***************************************************************************/
/**
 * @brief	set the SD/EMMC host clock 
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 HostControllerClockSetup()
{
    //  Host Controller Clock Setup
    SDIO->CLK_CTRL_R = 0xB;
    SDIO->TOUT_CTRL_R = 0;
    SDIO->SW_RST_R = 0;
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);
    SDIO->CLK_CTRL_R = 0xF;
    SDIO->CLK_CTRL_R = 0xF;

    return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	set interrupt controller
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 InitInterruptSetting()
{
    SDIO->NORMAL_INT_STAT_EN_R = 0x2FF;
    SDIO->NORMAL_INT_SIGNAL_EN_R = 0x00FF;
    SDIO->ERROR_INT_STAT_EN_R = 0x00FB;
    SDIO->AUTO_CMD_STAT_R = 0;

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x540, 0x0FFF0000);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x510, 0x01010004);

    return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	send the initial command to the sd/emmc device
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SendInitCmd()
{
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    
     // send command 0
    SDIO->ARGUMENT_R = 0;
    SDIO->XFER_MODE_R.D16 = 0x10;
    SDIO->CMD_R.D16 = 0;
    wait_command_complete();

    // send command 8
    wait_command_complete();
    SDIO->ARGUMENT_R = 0x1AA;
    SDIO->XFER_MODE_R.D16 = 0x10;
    SDIO->CMD_R.D16 = 0x0802;
    wait_command_complete();
    
    // send command 55
    SDIO->ARGUMENT_R = 0;
    SDIO->XFER_MODE_R.D16 = 0xb2;
    SDIO->CMD_R.D16 = 0x3702;
    wait_command_complete();

    validvoltage = 0;
    while (!validvoltage)
    {
    	// CMD55
    	SDIO->ARGUMENT_R = 0;
    	SDIO->XFER_MODE_R.D16 = 0xb2;
        SDIO->CMD_R.D16 = 0x3702;
    	wait_command_complete();

    	//CMD41
        SDIO->ARGUMENT_R = 0xC0100000;
        SDIO->XFER_MODE_R.D16 = 0xb2;
        SDIO->CMD_R.D16 = 0x2902;
    	wait_command_complete();

        response01 = SDIO->RESP01_R;
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);
    	if (validvoltage == 1)
    	{
    	    break;
    	}
    }

    // send command 2
    SDIO->ARGUMENT_R = 0;
    SDIO->XFER_MODE_R.D16 = 0xb2;
    SDIO->CMD_R.D16 = 0x0201;
    wait_command_complete();

    // send command 3
    SDIO->ARGUMENT_R = 0;
    SDIO->XFER_MODE_R.D16 = 0xb2;
    SDIO->CMD_R.D16 = 0x0302;
    wait_command_complete();     
    rca = SDIO->RESP01_R & 0xFFFF0000;

    // send command 9
    SDIO->ARGUMENT_R = rca;
    SDIO->CMD_R.D16 = 0x0901;
    wait_command_complete();
    CSD_Tab[3] = SDIO->RESP01_R;
    CSD_Tab[2] = SDIO->RESP23_R;
    CSD_Tab[1] = SDIO->RESP45_R;
    CSD_Tab[0] = SDIO->RESP67_R;    

    // Set buswidth to 1 bit clock to 48MHZ
    errorstatus = SD_GetCardInfo(&SDCardInfo);
    
    // send command 7
    SDIO->ARGUMENT_R = rca;
    SDIO->CMD_R.D16 = 0x0703;
    wait_command_complete();

    return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	change the bit width
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SwitchDataWidth()
{
    // send command 55  SET BUSWITHD TO 4 BIT
   	SDIO->ARGUMENT_R = rca;
    SDIO->XFER_MODE_R.D16 = 0xb2;
    SDIO->CMD_R.D16 = 0x3702;
    wait_command_complete();

    // send command 6
    SDIO->ARGUMENT_R = 0x2; //set sd model data width=4
    SDIO->CMD_R.BIT.CMD_INDEX = 0x6;
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x060200b2);
    SDIO->HOST_CTRL1_R.DAT_XFER_WIDTH = 0x1;
    wait_command_complete();
    sleep(2000);

    return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	SD init sequence
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_Init(void)
{
    int Status = XST_FAILURE;

    Status = CardDetection();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = HostControllerSetup();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = HostControllerClockSetup();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = InitInterruptSetting();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = SendInitCmd();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = SwitchDataWidth();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    
    Status = XST_SUCCESS;
END:
	return Status;
}

/***************************************************************************/
/**
 * @brief	read multi block size data 
 *
 * @param	readbuff reading data buffer 
 * @param	ReadAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )readbuff;

	REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF02);
	REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, Buffer_SingleBlock);
	REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, Buffer_SingleBlock);
	REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

	// send command 16
	SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
	SDIO->BLOCKCOUNT_R = 0x8;
    SDIO->ARGUMENT_R = 0x200;
    SDIO->XFER_MODE_R.D16 = 0x82;
    SDIO->CMD_R.D16 = 0x1002;
    wait_command_complete();

	// send command 17 read single block
	SDIO->ARGUMENT_R = ReadAddr;
    SDIO->XFER_MODE_R.D16 = 0x91;
    SDIO->CMD_R.D16 = 0x1122;
	wait_command_complete();
    wait_transfer_complete();

	return XST_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	write multi block size data 
 *
 * @param	writebuff reading data buffer 
 * @param	WriteAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF02);
    SDIO->SDMASA_R = Buffer_SingleBlock;
    SDIO->ADMA_SA_LOW_R = Buffer_SingleBlock;
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

    // send command 16
    SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
    SDIO->BLOCKCOUNT_R = 0x8;
    SDIO->ARGUMENT_R = 0x200;
    SDIO->XFER_MODE_R.D16 = 0x82;
    SDIO->CMD_R.D16 = 0x1002;
    wait_command_complete();

	// send command 24
	REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);
	SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = BlockSize;
	SDIO->BLOCKCOUNT_R = NumberOfBlocks;
	SDIO->ARGUMENT_R = WriteAddr;
	REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x18220083);
	value = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0xC);
    wait_command_complete();
    wait_transfer_complete();

	return XST_SUCCESS;
}

u32 SD_WaitReadOperation()
{
    for (;;)
    {
        if (SDIO->NORMAL_INT_STAT_R.XFER_COMPLETE == 1)
        {
            SDIO->NORMAL_INT_STAT_R.XFER_COMPLETE = 1;
            break;
        }
    }
    return SD_OK;
}

/***************************************************************************/
/**
 * @brief	Returns information about specific card.
 *
 * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD card
 *         information.
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  u32 errorstatus = XST_SUCCESS;
  uint8_t tmp = 0;

  cardinfo->CardType = (uint8_t)CardType;
  cardinfo->RCA = (uint16_t)RCA;

  /*adjust postion*/
  CSD_Tab[0] = CSD_Tab[0] << 8;
  tmp = (CSD_Tab[1] & 0xFF000000) >> 24;
  memcpy(((uint8_t *)&CSD_Tab[0]), &tmp, 1);
  CSD_Tab[1] = CSD_Tab[1] << 8;
  tmp = (CSD_Tab[2] & 0xFF000000) >> 24;
  memcpy(((uint8_t *)&CSD_Tab[1]), &tmp, 1);
  CSD_Tab[2] = CSD_Tab[2] << 8;
  tmp = (CSD_Tab[3] & 0xFF000000) >> 24;
  memcpy(((uint8_t *)&CSD_Tab[2]), &tmp, 1);
  CSD_Tab[3] = CSD_Tab[3] << 8;

  /*!< Byte 0 */
  tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;

  /*!< Byte 1 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  /*!< Byte 4 */
  tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  /*!< Byte 5 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  /*!< Byte 6 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; /*!< Reserved */

  if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
  {
      cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

      /*!< Byte 7 */
      tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
      cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

      /*!< Byte 8 */
      tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
      cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

      cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
      cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

      /*!< Byte 9 */
      tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
      cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
      cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
      cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
      /*!< Byte 10 */
      tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
      cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;

      cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
      cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
      cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
      cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }
  else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);

    cardinfo->SD_csd.DeviceSize |= (tmp << 8);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);

    cardinfo->SD_csd.DeviceSize |= (tmp);

    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);

    cardinfo->CardCapacity = ((uint64_t)cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
    cardinfo->CardBlockSize = 512;
  }

  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /*!< Byte 11 */
  tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /*!< Byte 12 */
  tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /*!< Byte 13 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  /*!< Byte 14 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  /*!< Byte 15 */
  tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;

  /*!< Byte 0 */
  tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;

  /*!< Byte 1 */
  tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  /*!< Byte 2 */
  tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  /*!< Byte 4 */
  tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  /*!< Byte 5 */
  tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  /*!< Byte 6 */
  tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  /*!< Byte 7 */
  tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  /*!< Byte 8 */
  tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  /*!< Byte 10 */
  tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  /*!< Byte 11 */
  tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /*!< Byte 14 */
  tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  /*!< Byte 15 */
  tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;

  return(errorstatus);
}

/***************************************************************************/
/**
 * @brief	test SD/EMMC read/write
 *
 * @param  None
 * @return XST_SUCCESS
 *
 ******************************************************************************/
u32 SD_Test(void)
{
	UINT fnum;            			  /* 文件成功读写数量 */
	BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
	BYTE WriteBuffer[] = "welcome777777777777777\r\n";
	FIL fnew;

	res_sd = f_mount(&fs,"0:",1);
#if 0
	    if(res_sd == FR_NO_FILESYSTEM)
	    {
	    	res_sd=f_mkfs("0:",0,0);
	    	if(res_sd == FR_OK)
	    	{
	    		res_sd = f_mount(NULL,"0:",1);
	    		res_sd = f_mount(&fs,"0:",1);
	    	}
	    }
#endif
    res_sd = f_open(&fnew, "0:FatFs.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
        //printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
        res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if(res_sd==FR_OK)
        {
            //printf("》文件写入成功，写入字节数据：%d\n",fnum);
            //printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
        }
        else
        {
            //printf("！！文件写入失败：(%d)\n",res_sd);
        }
        f_close(&fnew);
    }
    else
    {
    }
	/*------------------- 文件系统测试：读测试 ------------------------------------*/
    //printf("****** 即将进行文件读取测试... ******\r\n");
    res_sd = f_open(&fnew, "0:FatFs.txt", FA_OPEN_EXISTING | FA_READ);
    if(res_sd == FR_OK)
    {
        //printf("》打开文件成功。\r\n");
        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if(res_sd==FR_OK)
        {
            //printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
            //printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            //printf("！！文件读取失败：(%d)\n",res_sd);
        }
    }
    else
    {
        //LED_RED;
        //printf("！！打开文件失败。\r\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&fnew);

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,"0:",1);

    return Status;
}
/*********************************************END OF FILE**********************/
