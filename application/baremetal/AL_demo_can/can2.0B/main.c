/*
 * main.c
 *
 *  Created on: 2021年12月17日
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "AL_uart.h"
#include "AL_can.h"
#include <time.h>

uint8_t tx_buf1[64];
uint8_t tx_buf2[64];
uint8_t tx_buf3[64];
uint8_t tx_buf4[64];
uint8_t tx_buf5[64];
uint8_t rx_buf1[64];
uint8_t tx_buf5[64];
uint8_t rx_buf2[64];

AL_CAN_TypeDef *AL_CAN_CUR = AL_CAN1;

void SOC_CAN0_HANDLER(void)
{
 uint8_t count = 0;
 count++;
 //CAN_Tx_Complete(al_CAN0);
 //Can_Receive_Msg(al_CAN0,rx_buf1,data_length_20);
}

void SOC_CAN1_HANDLER(void)
{
	if(((AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT & (1 << 15)) >> 15) == 1){
		printf("rcv msg!\r\n");
		AlCan_ReceiveMsg(AL_CAN_CUR,rx_buf1,data_length_8);
		AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT |= (0x1 << 15);
		printf("cur register is %x!\r\n", AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT);
	}else if(((AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT & (1 << 11)) >> 11) == 1){
		printf("none, cur register is %x!\r\n", AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT);
		AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT |= (0x1 << 11);
	}else if(((AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT & (1 << 1)) >> 1) == 1){
		printf("error int\r\n");
		printf("none, cur register is %x!\r\n", AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT);
		AL_CAN_CUR->RTIE_RTIF_ERRINT_LIMIT |= (0x1 << 1);
	}
}

int main(){
	for (volatile uint8_t i = 0 ;i < 64 ; i++)
	{
		tx_buf1[i]=i;
		tx_buf2[i]=i+1;
		tx_buf3[i]=i+2;
		tx_buf4[i]=i+3;
		tx_buf5[i]=i+4;
	}
	//__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    ECLIC_Register_IRQ(SOC_INT92_IRQn , ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_CAN1_HANDLER);
	__enable_irq();
	AlCan_SetResetMode(AL_CAN_CUR);
	//AlCan_InterruptEnable(AL_CAN_CUR, RIE);
	//AlCan_InterruptEnable(AL_CAN_CUR, TPIE);
	//AlCan_SetRxFillter(AL_CAN_CUR,AE0,0x15);
	AlCan_DeviceDriverBittimeConfiguration(AL_CAN_CUR,Sarbrate_0_25Mbit,AL_TOP0,can);
	AlCan_TxMode(AL_CAN_CUR,NORMAL);
	AlCan_TmitMode(AL_CAN_CUR,XMIT_PTB_MODE);
	while(1){
		AlCan_SendMsg(AL_CAN_CUR,tx_buf1,XMIT_PTB_MODE,data_length_8,0x11);
		_delay_ms(100);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf2,XMIT_PTB_MODE,data_length_8,0x12);
		_delay_ms(100);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf3,XMIT_PTB_MODE,data_length_8,0x13);
		_delay_ms(100);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf5,XMIT_PTB_MODE,data_length_8,0x14);
		_delay_ms(100);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf4,XMIT_PTB_MODE,data_length_8,0x15);
		_delay_ms(100);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf5,XMIT_PTB_MODE,data_length_8,0x16);
		_delay_ms(100);
		AlCan_SendMsg(AL_CAN_CUR,tx_buf5,XMIT_PTB_MODE,data_length_8,0x17);
		_delay_ms(100);
	}
	while(0){
		AlCan_ReceiveMsg(AL_CAN_CUR,rx_buf1,data_length_8);
	}
	return 0;
}
