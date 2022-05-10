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
//#include "led/bsp_led.h"
//#include "al9000_sdio_sd.h"
#include <stdio.h>
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
static uint32_t CSD_Tab[4], CID_Tab[4], RCA = 0;


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
static DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;

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



void SD_Init_n()
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
    

    error_flag = 0;
    int mpidr = 0x0;

    int cpunum = mpidr & 0x00ff;
    DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;

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
    CSD_Tab[0] = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
    CSD_Tab[1] = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x14);
    CSD_Tab[2] = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x18);
    CSD_Tab[3] = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x1C);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x30, 0x00090000);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0200310F);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000310F);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x50C00000);
    // Set buswidth to 1 bit clock to 48MHZ
    

    // send command 7
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, temp);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x070300b2);
    wait_command_complete();


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

    //sdma start

    memset(Buffer_MultiBlock_Tx, 0x5a, sizeof(Buffer_MultiBlock_Tx));
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF02);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, Buffer_MultiBlock_Tx);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, Buffer_MultiBlock_Tx);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00080200);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00200000);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);


    // send command 16
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
    SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
    SDIO->BLOCKCOUNT_R = 0x1;
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000200);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x10020083);
    wait_command_complete();

    // send command 24
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);
    SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
    SDIO->BLOCKCOUNT_R = 0x1;
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x18220083);
    value = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0xC);
    wait_command_complete();

    wait_transfer_complete();


    memset(Buffer_MultiBlock_Rx, 0, sizeof(Buffer_MultiBlock_Rx));
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF02);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, Buffer_MultiBlock_Rx);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, Buffer_MultiBlock_Rx);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00080200);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00200000);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

        // send command 16
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
        SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
        SDIO->BLOCKCOUNT_R = 0x8;
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x10020083);
        wait_command_complete();

        // send command 17 read single block
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x11220091);
        value = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0xC);
        wait_command_complete();


     wait_transfer_complete();    //sdma start

     memset(Buffer_MultiBlock_Tx, 0x6b, sizeof(Buffer_MultiBlock_Tx));
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF02);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, Buffer_MultiBlock_Tx);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, Buffer_MultiBlock_Tx);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00080200);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00200000);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);


     // send command 16
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
     SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
     SDIO->BLOCKCOUNT_R = 0x1;
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000200);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x10020083);
     wait_command_complete();

     // send command 24
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);
     SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
     SDIO->BLOCKCOUNT_R = 0x1;
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x18220083);
     value = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0xC);
     wait_command_complete();

     wait_transfer_complete();


     memset(Buffer_MultiBlock_Rx, 0, sizeof(Buffer_MultiBlock_Rx));
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF02);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, Buffer_MultiBlock_Rx);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, Buffer_MultiBlock_Rx);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00080200);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00200000);
     REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

         // send command 16
         REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
         SDIO->BLOCKSIZE_R.XFER_BLOCK_SIZE = 0x200;
         SDIO->BLOCKCOUNT_R = 0x8;
         REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000200);
         REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x10020083);
         wait_command_complete();

         // send command 17 read single block
         REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);
         REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x11220091);
         value = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0xC);
         wait_command_complete();


      wait_transfer_complete();









        for(;;)
        	;
    /*
        value = 0;
        for(;;)
        {
        	value = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x30);
        	if (value == 0x10)
        	{
        		break;
        	}
        }
  /*  memset(Buffer_MultiBlock_Tx, 0xaa, sizeof(Buffer_MultiBlock_Tx));

    SDIO->HOST_CTRL1_R.DMA_SEL = 0x1;
    SDIO->HOST_CTRL2_R.HOST_VER4_ENABLE == 0;
    SDIO->SDMASA_R = Buffer_MultiBlock_Tx;
    SDIO->BLOCKSIZE_R = 0x200;
    SDIO->BLOCKCOUNT_R = 0x8;
    SDIO->ARGUMENT_R = 0x0;
    SDIO->XFER_MODE_R.BLOCK_COUNT_ENABLE = 0x1;
    SDIO->XFER_MODE_R.DATA_XFER_DIR = 0x0;
    SDIO->XFER_MODE_R.AUTO_CMD_ENABLE = 0x0;
    SDIO->XFER_MODE_R.DMA_EN = 0x1;
    SDIO->XFER_MODE_R.RESP_ERR_CHK_ENABLE = 0x1;
    SDIO->XFER_MODE_R.RESP_INT_DISABLE = 0x0;
    SDIO->XFER_MODE_R.RESP_TYPE = 0x0;

    SDIO->CMD_R.RESP_TYPE_SELECT = 0x2;
    SDIO->CMD_R.SUB_CMD_FLAG = 0x0;
    SDIO->CMD_R.CMD_CRC_CHK_ENABLE = 0x0;
    SDIO->CMD_R.CMD_IDX_CHK_ENABLE = 0x0;
    SDIO->CMD_R.DATA_PRESENT_SEL = 0x0;
    SDIO->CMD_R.CMD_TYPE = 0x0;

    SDIO->CMD_R.CMD_INDEX = 0x18;    
    memset(Buffer_MultiBlock_Rx, 0, sizeof(Buffer_MultiBlock_Rx));
    int v = 0;
    for(v = 0; v <= 1024; v++)
    {
    	Buffer_MultiBlock_Rx[v] = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x20);
    }
*/
    for(;;)
    	;
/*
    value = 0;
    for(;;)
    {
    	value = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x30);
    	if (value == 0x10)
    	{
    		break;
    	}
    }
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x30, 0x10);*/
    /*
        // ADMA2 start
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF12);

        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, 0x00000008);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, OCM__BASE1_ADDR);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00040004);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00200000);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);
*/

/*        for (;;)
            {
            	if (SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE == 1)
            	{
            		SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE = 1;
            		break;
            	}
            }
*/
/*        // send command 18

        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x0);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x122201b2);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x122201b2);
*/
/*
        for (;;)
            {
            	if (SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE == 1)
            	{
            		SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE = 1;
            		break;
            	}
            }
*/     //   REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0600020F);
     //   REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0008020F);
     //   REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x30, 0x00400000);



        // send command 12

/*        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00010200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00001100);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x0CC30110);

        for (;;)
            {
            	if (SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE == 1)
            	{
            		SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE = 1;
            		break;
            	}
            }

        rdata0 = REG_READ(OCM__BASE5_ADDR+0x00);
        rdata1 = REG_READ(OCM__BASE5_ADDR+0x04);
        rdata2 = REG_READ(OCM__BASE5_ADDR+0x08);
        rdata3 = REG_READ(OCM__BASE5_ADDR+0x0C);
*/

/*
        // ADMA2 start
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF12);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C);
        vfwp("** SDMA REG  0x3C = %x",rdata0);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, 0x00000008);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, OCM__BASE2_ADDR);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00010200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00001100);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

    //    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000B000);




    //    // send command 23
    //
    //    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00010001);
    //    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000001);
    //    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x172201b3);
    //    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
    //    vfwp("** command23 REG  0x08 = %x",rdata0);
    //    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
    //    vfwp("** command23 REG  0x0C = %x",rdata0);
    //    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
    //    vfwp("** command23 REG  0x10 = %x",rdata0);
    //
    //
    //    for(int i = 0; i < 50; i++)
    //    {
    //       asm volatile("nop");
    //    }





        // send command 25
    //    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000B000);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00010200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00001100);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x193a0183);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
        vfwp("** command24 REG  0x08 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command24 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command24 REG  0x10 = %x",rdata0);

        for (;;)
            {
            	if (SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE == 1)
            	{
            		SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE = 1;
            		break;
            	}
            }

        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x30);
        vfwp("** SDMA REG  0x30 = %x",rdata0);



        // send command 12

        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00010200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00001100);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x0CC30110);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
        vfwp("** command12 REG  0x08 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command12 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command12 REG  0x10 = %x",rdata0);


        for (;;)
            {
            	if (SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE == 1)
            	{
            		SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE = 1;
            		break;
            	}
            }

        // SDMA start
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF12);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C);
        vfwp("** SDMA REG  0x3C = %x",rdata0);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, 0x00000008);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, OCM__BASE3_ADDR);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00080200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00001100);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);



        // send command 18

        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00040200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00001100);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x122201b3);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
        vfwp("** command17 REG  0x08 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command17 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command17 REG  0x10 = %x",rdata0);

        for (;;)
            {
            	if (SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE == 1)
            	{
            		SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE = 1;
            		break;
            	}
            }
        // send command 12

        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00010200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00001100);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x0CC30110);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
        vfwp("** command12 REG  0x08 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command12 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command12 REG  0x10 = %x",rdata0);


        for (;;)
            {
            	if (SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE == 1)
            	{
            		SDIO->NORMAL_INT_STAT_R.CMD_COMPLETE = 1;
            		break;
            	}
            }


        rdata0 = REG_READ(OCM__BASE5_ADDR+0x00);
        vfwp("** Read Data OCM__BASE5_ADDR+0x00 = %x",rdata0);
        rdata1 = REG_READ(OCM__BASE5_ADDR+0x04);
        vfwp("** Read Data OCM__BASE5_ADDR+0x04 = %x",rdata1);
        rdata2 = REG_READ(OCM__BASE5_ADDR+0x08);
        vfwp("** Read Data OCM__BASE5_ADDR+0x08 = %x",rdata2);
        rdata3 = REG_READ(OCM__BASE5_ADDR+0x0C);
        vfwp("** Read Data OCM__BASE5_ADDR+0x0C = %x",rdata3);

       if (rdata0 == 0x12345678)
       {
    	   error_flag = 0;
       }
       else{
    	   error_flag = 1;
       }

       if (rdata1 == 0x87654321)
       {
    	   error_flag = 0;
       }
       else
       {
    	   error_flag = 1;
       }
       if (rdata2 == 0xabcddcba)
       {
    	   error_flag = 0;
       }
       else
       {
    	   error_flag = 1;
       }
       if (rdata3 == 0xa1b2c3d4)
       {
    	   error_flag = 0;
       }
       else
       {
    	   error_flag = 1;
       }
*/

}

void SD_Test(void)
{
  SD_Init_n();
  /*------------------------------ SD Init ---------------------------------- */
	/* SD��ʹ��SDIO�жϼ�DMA�жϽ������ݣ��жϷ������λ��bsp_sdio_sd.c�ļ�β*/
#if 0
  if((Status = SD_Init()) != SD_OK)
  {    
      printf("SD����ʼ��ʧ�ܣ���ȷ��SD������ȷ���뿪���壬��һ��SD�����ԣ�\n");
  }
  else
  {
      printf("SD����ʼ���ɹ���\n");		 
  }

  if(Status == SD_OK)
  {
      /*��������*/
      SD_EraseTest();
      SD_SingleBlockTest();
      /*muti block ��д����*/
      SD_MultiBlockTest();
  }
#endif
   
}


/*********************************************END OF FILE**********************/
