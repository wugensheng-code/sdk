/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SDIO sd�����������������ļ�ϵͳ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "sdio_test.h"
#include <string.h>
//#include "led/bsp_led.h"
//#include "al9000_sdio_sd.h"
#include <stdio.h>
#include "ff.h"
//#include "mshc_regs.h"
//#include "usart/bsp_debug_usart.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BLOCK_SIZE            512 /* Block Size in Bytes */

#define NUMBER_OF_BLOCKS      10  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)

#define vfwp printf
#define TOP_NS__CFG_CTRL_SDIO0_ADDR 0xF8800154
SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 1;
static uint32_t CardType =  SDIO_HIGH_CAPACITY_SD_CARD;

SD_CardInfo SDCardInfo;


unsigned int reg_read(unsigned long long reg_address)
{
    return *((volatile unsigned int *)reg_address);
}

void reg_write(unsigned long long reg_address, unsigned reg_wdata)
{
    *((volatile unsigned *)reg_address) = reg_wdata;
}



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint32_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
//SD_Error Status = SD_OK;

/* Private function prototypes -----------------------------------------------*/
static void SD_EraseTest(void);
static void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
static void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);

/* Private functions ---------------------------------------------------------*/
#define SDIO_WRAP__SDIO0__BASE_ADDR 0xF804A000ULL
#define TUBE_ADDRESS ((volatile char *) 0xF8800010u)



static void sleep(int tick)
{
    for (int i = 0; i < tick; i++)
    {
        asm volatile("NOP");
    }
}

uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];

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
volatile DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;

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


FATFS fs;
FRESULT res_sd;

uint8_t flag = 0;

SD_Error SD_Init(void)
{
	if (flag == 1)
	{
		return SD_OK;
	}
	flag = 1;
	volatile unsigned rdata0;
    volatile unsigned int rdata9;
    volatile unsigned rdata1;
    volatile unsigned rdata2;
    volatile unsigned rdata3;
    volatile unsigned rdata4;
    volatile unsigned rdata5;
    volatile unsigned error_flag;
    volatile unsigned int status;
    volatile unsigned int response2;
    volatile unsigned int validvoltage;
    volatile unsigned int count = 0;
    volatile unsigned int value = 0;
    volatile unsigned int value1 = 0;
    __IO SD_Error errorstatus = SD_OK;
    
    error_flag = 0;
    int mpidr = 0x0;

    int cpunum = mpidr & 0x00ff;


    //DWC_mshc__BLOCKSIZE_R__ACC_T reg0;

    if (cpunum == 0) {
    }

    char  p[] = "** CPU0 TEST PASSED OK **\n";
    char* c   = p;

    while (*c)
    {
      *TUBE_ADDRESS = *c;
      c++;
    }
    // open GPIO
    REG_WRITE(0xF8411000, 0xFFFFFFFF);

//  Write data to OCM
     #define OCM_L 0x61000000
     int OCM__BASE1_ADDR = OCM_L+0x00010000;
     int OCM__BASE2_ADDR = OCM_L+0x00012000;
     int OCM__BASE3_ADDR = OCM_L+0x00014000;
     int OCM__BASE4_ADDR = OCM_L+0x00020000;
     int OCM__BASE5_ADDR = OCM_L+0x00030000;

     REG_WRITE(OCM__BASE4_ADDR+0x00, 0x12345678);
     REG_WRITE(OCM__BASE4_ADDR+0x04, 0x87654321);
     REG_WRITE(OCM__BASE4_ADDR+0x08, 0xabcddcba);
     REG_WRITE(OCM__BASE4_ADDR+0x0C, 0xa1b2c3d4);
//  Write ADMA2 instruction to OCM
     REG_WRITE(OCM__BASE1_ADDR+0x00, 0x04000027);
     REG_WRITE(OCM__BASE1_ADDR+0x04, 0x61030000);

     REG_WRITE(OCM__BASE2_ADDR+0x00, 0x04000027);
     REG_WRITE(OCM__BASE2_ADDR+0x04, 0x61020000);

     REG_WRITE(OCM__BASE3_ADDR+0x00, 0x04000027);
     REG_WRITE(OCM__BASE3_ADDR+0x04, 0x61030000);
     value1 =  REG_READ(OCM__BASE3_ADDR+0x00);

     REG_WRITE(OCM__BASE5_ADDR+0x00, 0xa5a5a5a5);
     REG_WRITE(OCM__BASE5_ADDR+0x04, 0xa5a5a5a5);
     REG_WRITE(OCM__BASE5_ADDR+0x08, 0xa5a5a5a5);
     REG_WRITE(OCM__BASE5_ADDR+0x0C, 0xa5a5a5a5);



//  Card Detection
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x34, 0x000002FF);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x38, 0x000000C0);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x30, 0x000000C0);

    sleep(200);



    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x24);
    //vfwp("** REG  0x24 = %x",rdata0);


//  Host Controller Setup
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF10);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000000B);

    //rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0xFC);
    //vfwp("** REG  0xFC = %x",rdata0);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

//  Host Controller Clock Setup

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000000B);
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000000F);

    rdata1 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000000F);

    sleep(200);

    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x40);
    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x44);
    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x34, 0x000002FF);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x38, 0x000000FF);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x34, 0x00FB02FF);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0xDC010000);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, 0x00000004);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x540, 0x0FFF0000);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x510, 0x01010004);

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x0, 0x00000008);//
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00000001);// useless
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);
    sleep(200);
//for(;;)
{
    // send command 0
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x00000010);

    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
    rdata1 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x30);

    wait_command_complete();
}
//for (;;)

{
    // send command 8
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x1AA);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x08020010);
    rdata9 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
    rdata1 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x30);


    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x30);

    wait_command_complete();

}

    // send command 55
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x0);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x370200b2);
    wait_command_complete();
    status = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
    validvoltage = 0;
    //if (status > 0)
    {
    	while (!validvoltage)
    	{
    		// CMD55
    		REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x0);
    		REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x370200b2);
    		wait_command_complete();

    		//CMD41
    	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0xC0100000);
    	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xc, 0x290200b2);
    	    wait_command_complete();

    	    response2 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
    	    validvoltage = (((response2 >> 31) == 1) ? 1:0);
    	    if (validvoltage == 1)
    	    {
    	    	break;
    	    }
    	}

    }




//for(;;)
{
    // send command 2
	REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x0);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x020100b2);
}
    wait_command_complete();

//while (0)

    // send command 3
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x0);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x030200b2);
    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
    rdata9 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
    //rdata9 &= rdata9

    wait_command_complete();

    // send command 9
    unsigned int temp = 0x10000;
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, temp);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x090100b2);
    wait_command_complete();
    CSD_Tab[3] = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
    CSD_Tab[2] = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x14);
    CSD_Tab[1] = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x18);
    CSD_Tab[0] = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x1C);


    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x30, 0x00090000);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0200310F);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000310F);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x50C00000);
    // Set buswidth to 1 bit clock to 48MHZ
    errorstatus = SD_GetCardInfo(&SDCardInfo);
    


    // send command 7
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, temp);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x070300b2);
    wait_command_complete();
    //sleep(2000);

    // send command 55  SET BUSWITHD TO 4 BIT
   	REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, temp);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x370200b2);

    wait_command_complete();

    // send command 6
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000002); //set sd model data width=4
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x060200b2);
    SDIO->HOST_CTRL1_R.DAT_XFER_WIDTH = 0x1;
    wait_command_complete();
    sleep(2000);
    return SD_OK;
}

SD_Error SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	 volatile unsigned rdata0;
	 volatile unsigned int rdata9;
	 volatile unsigned rdata1;
	 volatile unsigned rdata2;
	 volatile unsigned rdata3;
	 volatile unsigned rdata4;
	 volatile unsigned rdata5;
	 volatile unsigned error_flag;
	 volatile unsigned int status;
	 volatile unsigned int response2;
	 volatile unsigned int validvoltage;
	 volatile unsigned int count = 0;
	 volatile unsigned int value = 0;
	 volatile unsigned int value1 = 0;
	 volatile unsigned int reg_value = 0;
	 uint32_t* Buffer_SingleBlock = (uint32_t* )readbuff;
#if 0

	    //sdma start
	    memset(Buffer_MultiBlock_Rx, 0xa5, sizeof(Buffer_MultiBlock_Rx));
	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF02);
	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, readbuff);
	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, readbuff);
	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00080200);
	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00001100);
	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

	    // send command 16
	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000200);
	    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x100200A2);
	    wait_command_complete();

		 // send command 18
		 REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00010200);
		 SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 512;
		 SDIO->BLOCKCOUNT_R = NumberOfBlocks;
		 //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);
		 SDIO->ARGUMENT_R = ReadAddr;
		 //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x192200B3);
		 REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x122200B3);
		// REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x122200B2);
		 wait_command_complete();
		 //sleep(2000);

		 SD_WaitReadOperation();

		 // SEND CMD12
		 REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00010200);
		 REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00001100);
		 REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x0C2200B3);

		 for (;;)
		 {
		     if (SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE == 1)
		     {
		           SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE = 1;
		           break;
		     }
		 }

#endif
	     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF02);
	     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, Buffer_SingleBlock);
	     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, Buffer_SingleBlock);
	     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00080200);
	     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00200000);
	     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

	        // send command 16
	        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
	        SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
	        SDIO->BLOCKCOUNT_R = 0x8;
	        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000200);
	        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x10020082);
	        wait_command_complete();

	        // send command 17 read single block
	        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);
	        SDIO->ARGUMENT_R = ReadAddr;
	        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x11220091);
	        value = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0xC);
	        wait_command_complete();

	     wait_transfer_complete();

	return SD_OK;
	
}

SD_Error SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
	 volatile unsigned rdata0;
	 volatile unsigned int rdata9;
	 volatile unsigned rdata1;
	 volatile unsigned rdata2;
	 volatile unsigned rdata3;
	 volatile unsigned rdata4;
	 volatile unsigned rdata5;
	 volatile unsigned error_flag;
	 volatile unsigned int status;
	 volatile unsigned int response2;
	 volatile unsigned int validvoltage;
	 volatile unsigned int count = 0;
	 volatile unsigned int value = 0;
	 volatile unsigned int value1 = 0;
	 volatile unsigned int reg_value = 0;
	 uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;

     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF02);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, Buffer_SingleBlock);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, Buffer_SingleBlock);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00080200);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00200000);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

	 // send command 16
	     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
	     SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
	     SDIO->BLOCKCOUNT_R = 0x1;
	     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000200);
	     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x10020083);
	     SDIO->XFER_MODE_R.DATA_XFER_DIR = 0x0;
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

	return SD_OK;

}

SD_Error SD_WaitReadOperation()
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



/**
  * @brief  Returns information about specific card.
  * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD card
  *         information.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error errorstatus = SD_OK;
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

void SD_Test(void)
{
	volatile unsigned rdata0;
	    volatile unsigned int rdata9;
	    volatile unsigned rdata1;
	    volatile unsigned rdata2;
	    volatile unsigned rdata3;
	    volatile unsigned rdata4;
	    volatile unsigned rdata5;
	    volatile unsigned error_flag;
	    volatile unsigned int status;
	    volatile unsigned int response2;
	    volatile unsigned int validvoltage;
	    volatile unsigned int count = 0;
	    volatile unsigned int value = 0;
	    volatile unsigned int value1 = 0;
	    __IO SD_Error errorstatus = SD_OK;
	    UINT fnum;            					  /* 文件成功读写数量 */
	    BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
	    BYTE WriteBuffer[] = "welcome777777777777777\r\n";
	    FIL fnew;


	    res_sd = f_mount(&fs,"0:",1);
#if 1
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
#if 1
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
#endif
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
}


/*********************************************END OF FILE**********************/
