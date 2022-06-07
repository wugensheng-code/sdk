/*
 * can_test.c
 *
 *  Created on: 2022年5月5日
 *      Author: Administrator
 */
#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"
void can_test(){
	uint8_t tx_buf1[64];
	uint8_t rx_buf1[20];
	uint8_t tx_buf2[64];
	uint8_t rx_buf2[20];

	for (volatile uint8_t i = 0 ;i < 20 ; i++){
		tx_buf1[i]=i;
	}
	for (volatile uint8_t i = 0 ;i < 20 ; i++){
		tx_buf2[i]=i%2;
	}
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("\r\n");
	printf("***************************CAN TEST*****************************\r\n");
	AlCan_SetResetMode(AL_CAN0);
	AlCan_DeviceDriverBittimeConfiguration(AL_CAN0,rate_5Mbit,AL_TOP0,can);
	AlCan_TxMode(AL_CAN0,IN_LOOPBACK);
	AlCan_SendMsg(AL_CAN0,tx_buf1,XMIT_PTB_MODE,data_length_8,0x10);
	AlCan_ReceiveMsg(AL_CAN0,rx_buf1,data_length_8);
	for (volatile uint8_t i = 0 ;i < 8 ; i++){
		if(tx_buf1[i] == rx_buf1[i]){
			printf("can receive data is right\r\n");
	    	}
	    else{printf("can test fail\r\n");}
	      	}
	AlCan_TestError(AL_CAN0);
	printf("can test success \r\n");
}


