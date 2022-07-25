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
//    for(i=0; i<256; i++)
    for(i=0; i<30; i++)
    {
    	rddata_a[i] = REG8(QSPI_XIP0_START_ADDR+i);
    }
    // print read data
    printf("xip read datas as follow:\r\n");
//    for(i=0; i<256; i++)
	for(i=0; i<30; i++)
	{
		printf("%d",rddata_a[i]); // print write data
	}
	printf("...");
	printf("\n");

    //--------------------data check-------
////    for(i=0;i<256;i++)
    for(i=0; i<30; i++)
    if(rddata_a[i] != i)
    {
    	printf("FAIL: datas are not same!\r\n");
    	return -1;
    }
    printf("PASS: qspi boot mode xip read test success!\r\n");

    printf("test finish\r\n");
    while(1);
}
