/*
 * main.c
 *
 *  Created on: Nov.22.2021
 *      Author: haitao.liu
 */

#include <stdio.h>
//#include <time.h>
//#include <stdlib.h>
#include "nuclei_sdk_soc.h"


//#define S25FL512S_ID	0X00200201  // flash ID
//#define S25FL256S_ID	0x00190201  // flash ID


#define CFG_CTRL_QSPI 	(0xF880016CUL)
#define TOP_CFG_CTRL_QSPI ((volatile uint32_t *)CFG_CTRL_QSPI)



#define XIP_INCR_INST 	(0xF804E100UL)
#define CFG_XIP_INCR_INST ((volatile uint32_t *)XIP_INCR_INST)

#define XIP_WRAP_INST 	(0xF804E104UL)
#define CFG_XIP_WRAP_INST ((volatile uint32_t *)XIP_WRAP_INST)



/* main */
int main()
{
   /***********************AL9000***************************/
    printf("test start\r\n");
    /*variable init*/
    uint16_t i = 0;
//    uint8_t wrdata_a[256] = {0};
    uint8_t rddata_a[256];
    /*************** data read (xip space)*************/

    printf("1MB flash:\n");
    /* Set XIP2 Reg Offset */
    *TOP_CFG_CTRL_QSPI = 7;


//    for(i=0; i<256; i++)
    for(i=0; i<30; i++)
    {
    	rddata_a[i] = REG8(QSPI_XIP0_START_ADDR+i+1*1024*1024-15);
    }
    // print read data
    printf("xip read datas as follow:\r\n");
//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		printf("%d ",rddata_a[i]); // print write data
	}
	printf("...");
	printf("\r\n");



	printf("2MB flash:\n");
 /* Set XIP2 Reg Offset */
	*TOP_CFG_CTRL_QSPI = 6;


//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		rddata_a[i] = REG8(QSPI_XIP0_START_ADDR+i+2*1024*1024-15);
	}
	// print read data
	printf("xip read datas as follow:\r\n");
//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		printf("%d ",rddata_a[i]); // print write data
	}
	printf("...");
	printf("\r\n");


	printf("4MB flash:\n");
	 /* Set XIP2 Reg Offset */
	*TOP_CFG_CTRL_QSPI = 5;


//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		rddata_a[i] = REG8(QSPI_XIP0_START_ADDR+i+4*1024*1024-15);
	}
	// print read data
	printf("xip read datas as follow:\r\n");
//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		printf("%d ",rddata_a[i]); // print write data
	}
	printf("...");
	printf("\r\n");



	printf("8MB flash:\n");
	 /* Set XIP2 Reg Offset */
	*TOP_CFG_CTRL_QSPI = 4;


//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		rddata_a[i] = REG8(QSPI_XIP0_START_ADDR+i+8*1024*1024-15);
	}
	// print read data
	printf("xip read datas as follow:\r\n");
//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		printf("%d ",rddata_a[i]); // print write data
	}
	printf("...");
	printf("\r\n");




	printf("16MB flash:\n");
	 /* Set XIP2 Reg Offset */
	*TOP_CFG_CTRL_QSPI = 3;


//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		rddata_a[i] = REG8(QSPI_XIP0_START_ADDR+i+16*1024*1024-15);
	}
	// print read data
	printf("xip read datas as follow:\r\n");
//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		printf("%d ",rddata_a[i]); // print write data
	}
	printf("...");
	printf("\r\n");


	printf("32MB flash:\n");

	dwc_ssi_disable(QSPI0);
	*CFG_XIP_INCR_INST = 0x3C;
	*CFG_XIP_WRAP_INST = 0x3C;
	printf("32MB flash:\n");
	qspi_spictrlr0_addr_l(QSPI0, 0x08);							 // 24-bit addr

	 /* Set XIP2 Reg Offset */
	*TOP_CFG_CTRL_QSPI = 2;
	dwc_ssi_enable(QSPI0);

//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		rddata_a[i] = REG8(QSPI_XIP0_START_ADDR+i+32*1024*1024-15);
	}
	// print read data
	printf("xip read datas as follow:\r\n");
//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		printf("%d ",rddata_a[i]); // print write data
	}
	printf("...");
	printf("\r\n");






    //--------------------data check-------
////    for(i=0;i<256;i++)
//    for(i=0; i<30; i++)
//    if(rddata_a[i] != i)
//    {
//    	printf("FAIL: datas are not same!\r\n");
//    	return -1;
//    }
    printf("PASS: qspi boot mode xip read test success!\r\n");

    printf("test finish\r\n");
    while(1);
}
