/*
 * main.c
 *
 *  Created on: 2022/3/20
 *  Author: pang.wei
 *  Function": demo_per
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"
#include "../../../SoC/demosoc/Common/Include/demosoc_xec.h"
#include "al9000_test.h"
#include "FATFS/ff.h"

extern u32 EMMC_Test(void);

#define BLOCK_SIZE            512 /* Block Size in Bytes */
#define NUMBER_OF_BLOCKS      10  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)

/*
//SDIO_TEST
static void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
static TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);*/

//uint32_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
//volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;


uint32_t main(){
    //EMMC_Test();
	//SD_Test();
	//iic_test();
	qspi_test();
	//spi_test();
	//dma_test();
	//gbe_test();
	//can_test();
	//timer_test();
	//gpio_test();

#if __riscv
	benchmark_test();
#endif

	//plps_test();
	//usb_test();
}
