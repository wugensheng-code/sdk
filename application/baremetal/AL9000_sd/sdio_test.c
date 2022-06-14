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

#define TOP_NS__CFG_CTRL_SDIO0_ADDR 0xF8800154
#define SDIO_WRAP__SDIO0__BASE_ADDR 0xF8049000ULL
#define SDIO_WRAP__SDIO1__BASE_ADDR 0xF804A000ULL

#define TUBE_ADDRESS ((volatile char *) 0xF8800010u)

SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 1;
static uint32_t CardType =  SDIO_HIGH_CAPACITY_SD_CARD;
FATFS fs;
FRESULT res_sd;
uint8_t flag = 0;
static unsigned int rca = 0;
SD_CardInfo SDCardInfo;
static volatile DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO1__BASE_ADDR;
static volatile DWC_mshc_block_registers* eMMC = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;
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

static void wait_command_complete(volatile DWC_mshc_block_registers* ptr)
{
    ERROR_INT_STAT_R__NORMAL_INT_STAT_R reg;
    reg = ptr->ERROR_INT_STAT_R__NORMAL_INT_STAT_R;
    for (;;)
    {
        if (reg.BIT.CMD_COMPLETE == 1)
        {
            reg.BIT.CMD_COMPLETE = 1;
            break;
        }
    }
}

static void wait_transfer_complete(volatile DWC_mshc_block_registers* ptr)
{
    ERROR_INT_STAT_R__NORMAL_INT_STAT_R reg;
    reg = ptr->ERROR_INT_STAT_R__NORMAL_INT_STAT_R;
    for (;;)
    {
        if (reg.BIT.XFER_COMPLETE == 1)
        {
            reg.BIT.XFER_COMPLETE = 1;
            break;
        }
    }
}

static void wait_buffer_read_ready_complete(volatile DWC_mshc_block_registers* ptr)
{
    ERROR_INT_STAT_R__NORMAL_INT_STAT_R reg;
    reg = ptr->ERROR_INT_STAT_R__NORMAL_INT_STAT_R;
    for (;;)
    {
        if (reg.BIT.BUF_RD_READY == 1)
        {
            reg.BIT.BUF_RD_READY = 1;
            break;
        }
    }
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
    u32 CardStatus = 0;
    
    //  Card Detection
    SDIO->ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R = 0x000002FF;
    SDIO->ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R = 0x000000C0;
    SDIO->ERROR_INT_STAT_R__NORMAL_INT_STAT_R = 0x000000C0;
    sleep(200);
    
    while (!CardStatus)
    {
        PSTATE_REG_R reg = SDIO->PSTATE_REG;
        CardStatus = (((reg->PSTATE_REG.CARD_INSERTED) == 1) ? 1:0);
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
u32 HostControllerSetup(volatile DWC_mshc_block_registers* ptr)
{
    u32 Status;
    //  Host Controller Setup
    ptr->WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R = 0x0000BF10;
    ptr->SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R = 0x0000002B;
    ptr->HOST_CTRL2_R__AUTO_CMD_STAT = 0x00000000;

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
u32 HostControllerClockSetup(volatile DWC_mshc_block_registers* ptr)
{
    //  Host Controller Clock Setup
    ptr->SW_RST_R__TOUT_CTRL_R__CLK_CTRL = 0x0000000B;
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);
    ptr->SW_RST_R__TOUT_CTRL_R__CLK_CTRL = 0x0000000F;
    ptr->SW_RST_R__TOUT_CTRL_R__CLK_CTRL = 0x0000000F;

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
u32 InitInterruptSetting(volatile DWC_mshc_block_registers* ptr)
{
    ptr->ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN = 0x000002FF;
    ptr->ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN = 0x000000FF;
    ptr->ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN = 0x00FB02FF;
    ptr->HOST_CTRL2_R__AUTO_CMD_STAT = 0x00000000;

    SDRegWrite(AT_CTRL_R, 0x0FFF0000);
    SDRegWrite(MBIU_CTRL_R, 0x01010004);

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
u32 SendInitCmdSD()
{
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    volatile unsigned int errorstatus;
    int Status;
    CMD_R__XFER_MODE_R reg;
    
    // send command 0
    SDIO->ARGUMENT_R = 0;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.CMD_INDEX = SD_CMD_GO_IDLE_STATE;
    reg.BIT.DATA_XFER_DIR = DATA_READ;
    SDIO->CMD_R__XFER_MODE = reg;
    wait_command_complete();

    // send command 8
    SDIO->ARGUMENT_R = 0x1AA;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.CMD_INDEX = SD_CMD_HS_SEND_EXT_CSD;
    reg.BIT.DATA_XFER_DIR = DATA_READ;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    SDIO->CMD_R__XFER_MODE = reg;
    wait_command_complete();
    
    // send command 55
    SDIO->ARGUMENT_R = 0;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.CMD_INDEX = SD_CMD_APP_CMD;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
    reg.BIT.DATA_XFER_DIR = DATA_READ;
    reg.BIT.MULTI_BLK_SEL = 0x1;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    SDIO->CMD_R__XFER_MODE = reg;
    wait_command_complete();

    validvoltage = 0;
    while (!validvoltage)
    {
    	// CMD55
        SDIO->ARGUMENT_R = 0;
        memset(&reg, 0, sizeof(reg));
        reg.BIT.CMD_INDEX = SD_CMD_APP_CMD;
        reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
        reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
        reg.BIT.DATA_XFER_DIR = DATA_READ;
        reg.BIT.MULTI_BLK_SEL = 0x1;
        reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
        SDIO->CMD_R__XFER_MODE = reg;
        wait_command_complete();

    	//CMD41
        SDIO->ARGUMENT_R = 0xC0100000;
        memset(&reg, 0, sizeof(reg));
        reg.BIT.CMD_INDEX = SD_CMD_SD_APP_OP_COND;
        reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
        reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
        reg.BIT.DATA_XFER_DIR = DATA_READ;
        reg.BIT.MULTI_BLK_SEL = 0x1;
        reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
        SDIO->CMD_R__XFER_MODE = reg;
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
    reg.BIT.CMD_INDEX = SD_CMD_ALL_SEND_CID;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Long;
    SDIO->CMD_R__XFER_MODE = reg;
    wait_command_complete();
    
    // send command 3
    SDIO->ARGUMENT_R = 0;
    reg.BIT.CMD_INDEX = SD_CMD_SET_REL_ADDR;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    SDIO->CMD_R__XFER_MODE = reg;
    wait_command_complete();
    rca = SDIO->RESP01_R & 0xFFFF0000;

    // send command 9
    SDIO->ARGUMENT_R = rca;
    reg.BIT.CMD_INDEX = SD_CMD_SEND_CSD;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Long;
    wait_command_complete();
    CSD_Tab[3] = SDIO->RESP01_R;
    CSD_Tab[2] = SDIO->RESP23_R;
    CSD_Tab[1] = SDIO->RESP45_R;
    CSD_Tab[0] = SDIO->RESP67_R;    

    // Set buswidth to 1 bit clock to 48MHZ
    errorstatus = SD_GetCardInfo(&SDCardInfo);
    
    // send command 7
    SDIO->ARGUMENT_R = rca;
    reg.BIT.CMD_INDEX = SD_CMD_SEL_DESEL_CARD;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short_48B;
    wait_command_complete();

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
u32 SendInitCmdEmmc()
{
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    volatile unsigned int errorstatus;
    int Status;
    CMD_R__XFER_MODE_R reg;
    
    // send command 0
    eMMC->ARGUMENT_R = 0;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.CMD_INDEX = SD_CMD_GO_IDLE_STATE;
    reg.BIT.DATA_XFER_DIR = DATA_READ;
    eMMC->CMD_R__XFER_MODE = reg;
    wait_command_complete();
    sleep(1000);
    
    validvoltage = 0;
    while (!validvoltage)
    {
    	//CMD1
        eMMC->ARGUMENT_R = 0x40000080;
        memset(&reg, 0, sizeof(reg));
        reg.BIT.CMD_INDEX = SD_CMD_SEND_OP_COND;
        reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
        reg.BIT.DATA_XFER_DIR = DATA_READ;
        eMMC->CMD_R__XFER_MODE = reg;
        wait_command_complete();

        response01 = eMMC->RESP01_R;
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);
    	if (validvoltage == 1)
    	{
    	    break;
    	}
    }

    // send command 2
    eMMC->ARGUMENT_R = 0;    
    reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
    reg.BIT.DATA_XFER_DIR = DATA_READ;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    reg.BIT.MULTI_BLK_SEL = 0x1;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Long;
    eMMC->CMD_R__XFER_MODE = reg;
    wait_command_complete();
    
    // send command 3
    eMMC->ARGUMENT_R = 0;
    reg.BIT.CMD_INDEX = SD_CMD_SET_REL_ADDR;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    eMMC->CMD_R__XFER_MODE = reg;
    wait_command_complete();
    rca = eMMC->RESP01_R & 0xFFFF0000;

    // send command 9
    eMMC->ARGUMENT_R = rca;
    reg.BIT.CMD_INDEX = SD_CMD_SEND_CSD;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Long;
    wait_command_complete();
    CSD_Tab[3] = eMMC->RESP01_R;
    CSD_Tab[2] = eMMC->RESP23_R;
    CSD_Tab[1] = eMMC->RESP45_R;
    CSD_Tab[0] = eMMC->RESP67_R;    

    // Set buswidth to 1 bit clock to 48MHZ
    errorstatus = SD_GetCardInfo(&SDCardInfo);

    // send command 10
    eMMC->ARGUMENT_R = rca;
    reg.BIT.CMD_INDEX = SD_CMD_SEND_CID;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Long;
    wait_command_complete();
    
    // send command 7
    eMMC->ARGUMENT_R = rca;
    reg.BIT.CMD_INDEX = SD_CMD_SEL_DESEL_CARD;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short_48B;
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
u32 SwitchDataWidthSD()
{
    CMD_R__XFER_MODE_R reg;
    
    // send command 55  SET BUSWITHD TO 4 BIT
   	SDIO->ARGUMENT_R = rca;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.CMD_INDEX = SD_CMD_APP_CMD;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
    reg.BIT.DATA_XFER_DIR = DATA_READ;
    reg.BIT.MULTI_BLK_SEL = 0x1;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    SDIO->CMD_R__XFER_MODE = reg;
    wait_command_complete();

    // send command 6
    SDIO->ARGUMENT_R = 0x2; //set sd model data width=4
    reg.BIT.CMD_INDEX = SD_CMD_HS_SWITCH;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    SDIO->CMD_R__XFER_MODE = reg;
    wait_command_complete();

    sleep(2000);

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
u32 SwitchDataWidthEmmc()
{
    CMD_R__XFER_MODE_R reg;

    // send command 6
    eMMC->ARGUMENT_R = 0x03b70100; //set sd model data width=4
    reg.BIT.CMD_INDEX = SD_CMD_HS_SWITCH;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    eMMC->CMD_R__XFER_MODE = reg;
    wait_command_complete();
    eMMC->WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1.BIT.DAT_XFER_WIDTH = 0x1;
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
    Status = HostControllerSetup(SDIO);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = HostControllerClockSetup(SDIO);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = InitInterruptSetting(SDIO);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = SendInitCmdSD();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = SwitchDataWidthSD();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    
    Status = XST_SUCCESS;
END:
	return Status;
}

/***************************************************************************/
/**
 * @brief	emmc init sequence
 *
 * @param	None
 * @return	XST_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_Init(void)
{
    int Status = XST_FAILURE;

    Status = HostControllerSetup(eMMC);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = HostControllerClockSetup(eMMC);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = InitInterruptSetting(eMMC);
    if (Status != XST_SUCCESS) {
		goto END;
	}
    sleep(200);
    Status = SendInitCmdEmmc();
    if (Status != XST_SUCCESS) {
		goto END;
	}
    Status = SwitchDataWidthEmmc();
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
    CMD_R__XFER_MODE_R reg;

    SDIO->WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1 = 0x0000BF02;
    SDIO->SDMASA_R = Buffer_SingleBlock;
    SDIO->ADMA_SA_LOW_R = Buffer_SingleBlock;
    SDIO->HOST_CTRL2_R__AUTO_CMD_STAT = 0x0;

	// send command 16
	SDIO->ARGUMENT_R = 0x200;
	BLOCKCOUNT_R__BLOCKSIZE_R block;
    memset(&block, 0, sizeof(block));
    block.XFER_BLOCK_SIZE = BlockSize;
    block.BLOCKCOUNT_R = NumberOfBlocks;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.CMD_INDEX = SD_CMD_SET_BLOCKLEN;
    SDIO->BLOCKCOUNT_R__BLOCKSIZE = block;
    SDIO->CMD_R__XFER_MODE = reg;
    wait_command_complete();

	// send command 17 read single block
	SDIO->ARGUMENT_R = ReadAddr;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.DMA_EN = 0x1;
    reg.BIT.DATA_XFER_DIR = 0x1;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.DATA_PRESENT_SEL = 0x1;
    reg.BIT.CMD_INDEX = SD_CMD_READ_SINGLE_BLOCK;
    BLOCKCOUNT_R__BLOCKSIZE_R block;
    memset(&block, 0, sizeof(block));
    block.XFER_BLOCK_SIZE = BlockSize;
    block.BLOCKCOUNT_R = NumberOfBlocks;
    SDIO->BLOCKCOUNT_R__BLOCKSIZE = block;
    SDIO->CMD_R__XFER_MODE = reg;
	wait_command_complete();
    wait_transfer_complete();

	return XST_SUCCESS;
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
u32 EMMC_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )readbuff;
    CMD_R__XFER_MODE_R reg;

    eMMC->WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1 = 0x0000BF02;
    eMMC->SDMASA_R = Buffer_SingleBlock;
    eMMC->ADMA_SA_LOW_R = Buffer_SingleBlock;
    eMMC->HOST_CTRL2_R__AUTO_CMD_STAT = 0x0;

	// send command 16
	eMMC->ARGUMENT_R = 0x200;
	BLOCKCOUNT_R__BLOCKSIZE_R block;
    memset(&block, 0, sizeof(block));
    block.XFER_BLOCK_SIZE = BlockSize;
    block.BLOCKCOUNT_R = NumberOfBlocks;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.CMD_INDEX = SD_CMD_SET_BLOCKLEN;
    eMMC->BLOCKCOUNT_R__BLOCKSIZE = block;
    eMMC->CMD_R__XFER_MODE = reg;
    wait_command_complete();

	// send command 17 read single block
	eMMC->ARGUMENT_R = ReadAddr;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.DMA_EN = 0x1;
    reg.BIT.DATA_XFER_DIR = 0x1;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.DATA_PRESENT_SEL = 0x1;
    reg.BIT.CMD_INDEX = SD_CMD_READ_SINGLE_BLOCK;
    BLOCKCOUNT_R__BLOCKSIZE_R block;
    memset(&block, 0, sizeof(block));
    block.XFER_BLOCK_SIZE = BlockSize;
    block.BLOCKCOUNT_R = NumberOfBlocks;
    eMMC->BLOCKCOUNT_R__BLOCKSIZE = block;
    eMMC->CMD_R__XFER_MODE = reg;
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
    CMD_R__XFER_MODE_R reg;

    SDIO->WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1 = 0x0000BF02;
    SDIO->SDMASA_R = Buffer_SingleBlock;
    SDIO->ADMA_SA_LOW_R = Buffer_SingleBlock;
    SDIO->HOST_CTRL2_R__AUTO_CMD_STAT = 0x0;

	// send command 16
	SDIO->ARGUMENT_R = 0x200;
	BLOCKCOUNT_R__BLOCKSIZE_R block;
    memset(&block, 0, sizeof(block));
    block.XFER_BLOCK_SIZE = 0x200;
    block.BLOCKCOUNT_R = 0x8;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.CMD_INDEX = SD_CMD_SET_BLOCKLEN;
    SDIO->BLOCKCOUNT_R__BLOCKSIZE = block;
    SDIO->CMD_R__XFER_MODE = reg;
    wait_command_complete();


	// send command 24
    SDIO->ARGUMENT_R = WriteAddr;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.DMA_EN = 0x1;
    reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
    reg.BIT.DATA_XFER_DIR = DATA_WRITE;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.DATA_PRESENT_SEL = 0x1;
    reg.BIT.CMD_INDEX = SD_CMD_WRITE_SINGLE_BLOCK;
    BLOCKCOUNT_R__BLOCKSIZE_R block;
    memset(&block, 0, sizeof(block));
    block.XFER_BLOCK_SIZE = BlockSize;
    block.BLOCKCOUNT_R = NumberOfBlocks;
    SDIO->BLOCKCOUNT_R__BLOCKSIZE = block;
    SDIO->CMD_R__XFER_MODE = reg;
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
u32 EMMC_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;
    CMD_R__XFER_MODE_R reg;

    eMMC->WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1 = 0x0000BF02;
    eMMC->SDMASA_R = Buffer_SingleBlock;
    eMMC->ADMA_SA_LOW_R = Buffer_SingleBlock;
    eMMC->HOST_CTRL2_R__AUTO_CMD_STAT = 0x0;

	// send command 16
	eMMC->ARGUMENT_R = 0x200;
	BLOCKCOUNT_R__BLOCKSIZE_R block;
    memset(&block, 0, sizeof(block));
    block.XFER_BLOCK_SIZE = 0x200;
    block.BLOCKCOUNT_R = 0x8;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.CMD_INDEX = SD_CMD_SET_BLOCKLEN;
    eMMC->BLOCKCOUNT_R__BLOCKSIZE = block;
    eMMC->CMD_R__XFER_MODE = reg;
    wait_command_complete();


	// send command 24
    eMMC->ARGUMENT_R = WriteAddr;
    memset(&reg, 0, sizeof(reg));
    reg.BIT.DMA_EN = 0x1;
    reg.BIT.BLOCK_COUNT_ENABLE = 0x1;
    reg.BIT.DATA_XFER_DIR = DATA_WRITE;
    reg.BIT.RESP_ERR_CHK_ENABLE = 0x1;
    reg.BIT.RESP_TYPE_SELECT = SDIO_Response_Short;
    reg.BIT.DATA_PRESENT_SEL = 0x1;
    reg.BIT.CMD_INDEX = SD_CMD_WRITE_SINGLE_BLOCK;
    BLOCKCOUNT_R__BLOCKSIZE_R block;
    memset(&block, 0, sizeof(block));
    block.XFER_BLOCK_SIZE = BlockSize;
    block.BLOCKCOUNT_R = NumberOfBlocks;
    eMMC->BLOCKCOUNT_R__BLOCKSIZE = block;
    eMMC->CMD_R__XFER_MODE = reg;
	wait_command_complete();
    wait_transfer_complete();

	return XST_SUCCESS;
}


u32 SD_WaitReadOperation(volatile DWC_mshc_block_registers* ptr)
{
    for (;;)
    {
        if (ptr->NORMAL_INT_STAT_R.BIT.XFER_COMPLETE == 1)
        {
            ptr->NORMAL_INT_STAT_R.BIT.XFER_COMPLETE = 1;
            break;
        }
    }
    return XST_SUCCESS;
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

u32 RawReadWriteTestSD()
{
    int Status;
    BYTE WriteBuffer[] = "welcomewelcome\r\n";
    BYTE ReadBuffer[1024]={0};   
    SD_Init();
    SD_WriteMultiBlocks(WriteBuffer, 1,SDCardInfo.CardBlockSize,1);
    SD_ReadMultiBlocks(ReadBuffer, 1, SDCardInfo.CardBlockSize,1);

    result = strcmp(WriteBuffer, ReadBuffer);
    if (result == 0)
    {
        Status = XST_SUCCESS;
    }
    else
    {
        Status = XST_FAILURE;
    }

    return Status;
}

u32 RawReadWriteTestEmmc()
{
    int Status;
    BYTE WriteBuffer[] = "welcomewelcome\r\n";
    BYTE ReadBuffer[1024]={0};   
    EMMC_Init();
    EMMC_WriteMultiBlocks(WriteBuffer, 1,SDCardInfo.CardBlockSize,1);
    EMMC_ReadMultiBlocks(ReadBuffer, 1, SDCardInfo.CardBlockSize,1);

    result = strcmp(WriteBuffer, ReadBuffer);
    if (result == 0)
    {
        Status = XST_SUCCESS;
    }
    else
    {
        Status = XST_FAILURE;
    }

    return Status;
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
	UINT fnum;            			  
	BYTE ReadBuffer[1024]={0};        
	BYTE WriteBuffer[] = "welcome777777777777777\r\n";
	FIL fnew;
	u32 Status;

    RawReadWriteTestSD();
    RawReadWriteTestEmmc();

	res_sd = f_mount(&fs,"0:",1);  //SD test
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
    res_sd = f_open(&fnew, "0:FatFs1.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
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
    res_sd = f_open(&fnew, "0:FatFs1.txt", FA_OPEN_EXISTING | FA_READ);
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
    f_close(&fnew);
    f_mount(NULL,"0:",1);


    res_sd = f_mount(&fs,"1:",1);  //EMMC test
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
        res_sd = f_open(&fnew, "1:FatFs1.txt",FA_CREATE_ALWAYS | FA_WRITE );
        if ( res_sd == FR_OK )
        {
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
        res_sd = f_open(&fnew, "1:FatFs1.txt", FA_OPEN_EXISTING | FA_READ);
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
        f_close(&fnew);
        f_mount(NULL,"1:",1);

    

    return Status;
}
/*********************************************END OF FILE**********************/
