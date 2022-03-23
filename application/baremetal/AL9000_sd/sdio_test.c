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

#define NUMBER_OF_BLOCKS      100  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)

#define vfwp printf
#define TOP_NS__CFG_CTRL_SDIO0_ADDR 0xF8800150
SDIO_CmdInitTypeDef SDIO_CmdInitStructure;

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
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
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
#define SDIO_WRAP__SDIO0__BASE_ADDR 0xF8049000ULL
#define TUBE_ADDRESS ((volatile char *) 0xF8800010u)
void SD_Init_n()
{
    volatile unsigned rdata0;
    volatile unsigned rdata1;
    volatile unsigned rdata2;
    volatile unsigned rdata3;
    volatile unsigned rdata4;
    volatile unsigned rdata5;
    volatile unsigned error_flag;
    DWC_mshc_block_registers* SDIO = (DWC_mshc_block_registers*)SDIO_WRAP__SDIO0__BASE_ADDR;
    

    error_flag = 0;
    int mpidr = 0x0;

    int cpunum = mpidr & 0x00ff;

    //DWC_mshc__BLOCKSIZE_R__ACC_T reg0;

    if (cpunum == 0) {
    // Ensure all writes to system registers have taken place

    //// REG_WRITE(0xf8800010u, 0x11111111);
    //rdata0 = REG_READ(0xf8800010u);
   // // REG_WRITE(0xf8800014u, 0x22222222);
   // rdata0 = REG_READ(0xf8800014u);
   // // REG_WRITE(0xf8800018u, 0x22222222);
   // rdata0 = REG_READ(0xf8800018u);
    // Ensure all writes to system registers have taken place
 //   asm volatile("dsb     sy");
 //   asm volatile("isb     sy");
 //   asm volatile("dmb     sy");
 //   // REG_WRITE(0xdd000000u, 0x00002222);
 //   rdata0 = REG_READ(0xdd000000u);
 //   // REG_WRITE(0xdd000004u, 0xffff2222);
 //   rdata0 = REG_READ(0xdd000004u);
 //   // REG_WRITE(0xdd000040u, 0x0ff00220);
 //   rdata0 = REG_READ(0xdd000040u);
 //   // REG_WRITE(0xdd000048u, 0x03030303);
 //   rdata0 = REG_READ(0xdd000048u);



    char  p[] = "** CPU0 TEST PASSED OK **\n";
    char* c   = p;

    while (*c)
    {
      *TUBE_ADDRESS = *c;
      c++;
    }

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


//  Card Detection
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x34, 0x000000C0);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x38, 0x000000C0);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x30, 0x000000C0);


    for(int i = 0; i < 2; i++)
    {
        asm volatile("NOP");
    }



    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x24);
    vfwp("** REG  0x24 = %x",rdata0);


//  Host Controller Setup
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF10);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000000B);

    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0xFC);
    vfwp("** REG  0xFC = %x",rdata0);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);


//  Host Controller Clock Setup

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000002B);
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000002F);

    rdata1 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C);
    vfwp("** REG  0x2C = %x",rdata1);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000002F);

    for(int i = 0; i < 20; i++)
        {
           asm volatile("NOP");
        }




    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x40);
    vfwp("** REG  0x40 = %x",rdata0);
    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x44);
    vfwp("** REG  0x44 = %x",rdata0);
    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
    vfwp("** REG  0x08 = %x",rdata0);

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x34, 0x000002CF);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x38, 0x000000FF);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x34, 0x00FB02CF);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0xDC010000);
    //REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, 0x00000004);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x540, 0x0FFF0000);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x510, 0x01010004);

    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x0, 0x00000008);//
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00000001);// useless
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000000);

    // send command 0
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x00000010);
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x00C00010);
    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
    vfwp("** command0 REG  0x0C = %x",rdata0);

    for(int i = 0; i < 50; i++)
            {
               asm volatile("NOP");
            }


        // send command 8
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x08020010);
    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
    vfwp("** command8 REG  0x0C = %x",rdata0);


    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x30);
//    while (GET_BIT(rdata0, 0) == 0){
//    	asm volatile("NOP");
//    }

    // send command 55
    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x370200b3);
    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
    vfwp("** command55 REG  0x0C = %x",rdata0);
    rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
    vfwp("** command55 REG  0x10 = %x",rdata0);

        for(int i = 0; i < 50; i++)
                    {
                       asm volatile("NOP");
                    }


        // send command 41
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x290200b3);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command41 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command41 REG  0x10 = %x",rdata0);

        for(int i = 0; i < 50; i++)
                    {
                       asm volatile("NOP");
                    }



        // send command 2
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x020100b3);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00000200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command2 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command2 REG  0x10 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x14);
        vfwp("** command2 REG  0x14 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x18);
        vfwp("** command2 REG  0x18 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x1C);
        vfwp("** command2 REG  0x1C = %x",rdata0);


        for(int i = 0; i < 50; i++)
                    {
                       asm volatile("NOP");
                    }


        // send command 3
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x030200b3);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command3 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command3 REG  0x10 = %x",rdata0);

        for(int i = 0; i < 50; i++)
                    {
                       asm volatile("NOP");
                    }



        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x30, 0x00090000);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0200002F);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0000002F);
    //    REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x50C00000);



        // send command 7

        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00200000);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x072200b3);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
        vfwp("** command7 REG  0x08 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command7 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command7 REG  0x10 = %x",rdata0);




        // send command 6
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000002); //set sd model data width=4
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x062200b3);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
        vfwp("** command6 REG  0x08 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command6 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command6 REG  0x10 = %x",rdata0);


        for(int i = 0; i < 50; i++)
                    {
                       asm volatile("NOP");
                    }

        // ADMA2 start
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x28, 0x0000BF12);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C);
        vfwp("** SDMA REG  0x3C = %x",rdata0);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x00, 0x00000008);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x58, OCM__BASE1_ADDR);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x04, 0x00040004);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x08, 0x00200000);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x3C, 0x00000000);


        // send command 16

        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00000001);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x102201b3);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
        vfwp("** command16 REG  0x08 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command16 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command16 REG  0x10 = %x",rdata0);

        for(int i = 0; i < 50; i++)
                    {
                       asm volatile("NOP");
                    }

        // send command 18

        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x4, 0x00080200);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x8, 0x00001100);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x122201b3);
        REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0xC, 0x122201b3);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x08);
        vfwp("** command17 REG  0x08 = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x0C);
        vfwp("** command17 REG  0x0C = %x",rdata0);
        rdata0 = REG_READ(SDIO_WRAP__SDIO0__BASE_ADDR+0x10);
        vfwp("** command17 REG  0x10 = %x",rdata0);
        vfwp("** command17 REG  0x10 = %x",rdata0);

        for(int i = 0; i < 800; i++)
                    {
                       asm volatile("NOP");
                    }
     //   REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0600020F);
     //   REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x2C, 0x0008020F);
     //   REG_WRITE(SDIO_WRAP__SDIO0__BASE_ADDR+0x30, 0x00400000);



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

        for(int i = 0; i < 50; i++)
                    {
                       asm volatile("NOP");
                    }
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

        for(int i = 0; i < 100; i++)
                    {
                       asm volatile("NOP");
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


        for(int i = 0; i < 50; i++)
        {
           asm volatile("NOP");
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

        for(int i = 0; i < 800; i++)
            {
               asm volatile("NOP");
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


        for(int i = 0; i < 50; i++)
        {
           asm volatile("NOP");
        }


        rdata0 = REG_READ(OCM__BASE5_ADDR+0x00);
        vfwp("** Read Data OCM__BASE5_ADDR+0x00 = %x",rdata0);
        rdata1 = REG_READ(OCM__BASE5_ADDR+0x04);
        vfwp("** Read Data OCM__BASE5_ADDR+0x04 = %x",rdata1);
        rdata2 = REG_READ(OCM__BASE5_ADDR+0x08);
        vfwp("** Read Data OCM__BASE5_ADDR+0x08 = %x",rdata2);
        rdata3 = REG_READ(OCM__BASE5_ADDR+0x0C);
        vfwp("** Read Data OCM__BASE5_ADDR+0x0C = %x",rdata3);


        }

       if (rdata0 == 0x12345678){
       error_flag = 0;}
       else{
       error_flag = 1;
       }
       if (rdata1 == 0x87654321){
       error_flag = 0;}
       else{
       error_flag = 1;
       }
       if (rdata2 == 0xabcddcba){
       error_flag = 0;}
       else{
       error_flag = 1;
       }
       if (rdata3 == 0xa1b2c3d4){
       error_flag = 0;}
       else{
       error_flag = 1;
       }
       while(1);

}

void SD_Test(void)
{
	SD_Init_n();
	//LED_BLUE;
  /*------------------------------ SD Init ---------------------------------- */
	/* SD��ʹ��SDIO�жϼ�DMA�жϽ������ݣ��жϷ������λ��bsp_sdio_sd.c�ļ�β*/
/*
  if((Status = SD_Init()) != SD_OK)
  {    
		//LED_RED;
    printf("SD����ʼ��ʧ�ܣ���ȷ��SD������ȷ���뿪���壬��һ��SD�����ԣ�\n");
  }
  else
  {
    printf("SD����ʼ���ɹ���\n");		 
  }
 */

}


/*********************************************END OF FILE**********************/
