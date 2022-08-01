/*
 * main.c
 *
 *  Created on: 2021年12月17日
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"

uint8_t tx_buf1[64];
uint8_t tx_buf2[64];
uint8_t tx_buf3[64];
uint8_t tx_buf4[64];
uint8_t tx_buf5[64];
uint8_t rx_buf1[64];
uint8_t tx_buf5[64];
uint8_t rx_buf2[64];

void SOC_CAN0_HANDLER(void)
{
 uint8_t count = 0;
 count++;
 //CAN_Tx_Complete(al_CAN0);
 //Can_Receive_Msg(al_CAN0,rx_buf1,data_length_20);
}

void SOC_CAN1_HANDLER(void)
{

}

int main(){
	/*
	uint8_t tx_buf1[64];
	uint8_t rx_buf1[20];
	uint8_t tx_buf2[64];
	uint8_t rx_buf2[20];
	*/
	for (volatile uint8_t i = 0 ;i < 64 ; i++)
	{
		tx_buf1[i]=i;
		tx_buf2[i]=i+1;
		tx_buf3[i]=i+2;
		tx_buf4[i]=i+3;
		tx_buf5[i]=i+4;
	}
	//__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    //ECLIC_Register_IRQ(SOC_CAN_0_IRQn , ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_CAN0_HANDLER);
	//__enable_irq();
	AlCan_SetResetMode(AL_CAN1);
	//AlCan_SetRxFillter(AL_CAN1,AE0,0x15);
	AlCan_DeviceDriverBittimeConfiguration(AL_CAN1,can_rate_1Mbit,AL_TOP0,can);
	AlCan_TxMode(AL_CAN1,NORMAL);
	AlCan_TmitMode(AL_CAN1,XMIT_PTB_MODE);
	while(1){
	//for(uint8_t i = 0 ; i < 100 ; i++){

		AlCan_SendMsg(AL_CAN1,tx_buf1,XMIT_PTB_MODE,data_length_8,0x11);
		AlCan_SendMsg(AL_CAN1,tx_buf2,XMIT_PTB_MODE,data_length_8,0x12);
		AlCan_SendMsg(AL_CAN1,tx_buf3,XMIT_PTB_MODE,data_length_8,0x13);
		AlCan_SendMsg(AL_CAN1,tx_buf5,XMIT_PTB_MODE,data_length_8,0x14);
		AlCan_SendMsg(AL_CAN1,tx_buf4,XMIT_PTB_MODE,data_length_8,0x15);
		AlCan_SendMsg(AL_CAN1,tx_buf5,XMIT_PTB_MODE,data_length_8,0x16);
		AlCan_SendMsg(AL_CAN1,tx_buf5,XMIT_PTB_MODE,data_length_8,0x17);

		//printf("111111");
	for (volatile uint8_t i = 0 ;i < 8 ; i++)
	{
		//AlCan_Receive_Msg(AL_CAN1,rx_buf);
	}
	}
	return 0;
}
