/*
 * main.c
 *
 *  Created on: 2021年12月17日
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_uart.h"
#include "al9000_can.h"
#include <time.h>
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
 //CAN_TX_COMPLETE(AL9000_CAN0);
 //Can_Receive_Msg(AL9000_CAN0,rx_buf1,data_length_20);
}

void SOC_CAN1_HANDLER(void)
{

}

uint8_t delay(uint32_t val){
	volatile uint32_t i ;
	for(i= 0 ; i < val ; i ++);
		if(i == val){
			i = 0;
		}
    	return 0;
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
	set_reset_mode(AL9000_CAN0);
	//set_reset_mode(AL9000_CAN1);
	set_rx_fillter(AL9000_CAN0,AE0,0x15);
	//canfd_device_driver_bittime_configuration(AL9000_CAN1,rate_5Mbit,AL9000_TOP0,can_fd);
	canfd_device_driver_bittime_configuration(AL9000_CAN0,rate_5Mbit,AL9000_TOP0,can_fd);
	can_tx_mode(AL9000_CAN0,NORMAL);
	//canfd_device_driver_bittime_configuration(AL9000_CAN1,rate_5Mbit,AL9000_TOP0,can_fd);
	//CAN_RX_INTERRUPT_ENABLE(AL9000_CAN1);
	//canfd_tx_interrupt(AL9000_CAN0);
	//canfd_rx_poll(AL9000_CAN0, int quota);
	//canfd_rxfull_interrupt(AL9000_CAN0,uint8_t isr);
	set_canfd_xmit_mode(AL9000_CAN0,XMIT_PTB_MODE);
	//set_canfd_xmit_mode(AL9000_CAN1,XMIT_PTB_MODE);
	//get_of_chip_status(AL9000_CAN0);
	//canfd_error_interrupt(AL9000_CAN0);
	//canfd_interrupt(AL9000_CAN0);
	//canfd_chip_start(AL9000_CAN0,Sarbrate_0_5Mbit);
	CAN_TransmitStatus(AL9000_CAN0);
	while(1){
	//for(uint8_t i = 0 ; i < 50 ; i++){
		//delay(1280);
		Can_Send_Msg(AL9000_CAN0,tx_buf1,XMIT_PTB_MODE,data_length_12,0x11);
		//delay(1200);
		Can_Send_Msg(AL9000_CAN0,tx_buf2,XMIT_PTB_MODE,data_length_20,0x12);
		//delay(1200);
		Can_Send_Msg(AL9000_CAN0,tx_buf3,XMIT_PTB_MODE,data_length_24,0x13);
		//delay(1200);
		Can_Send_Msg(AL9000_CAN0,tx_buf5,XMIT_PTB_MODE,data_length_32,0x14);
		//delay(1200);
		Can_Send_Msg(AL9000_CAN0,tx_buf4,XMIT_PTB_MODE,data_length_48,0x15);
		//delay(1200);
		Can_Send_Msg(AL9000_CAN0,tx_buf5,XMIT_PTB_MODE,data_length_16,0x16);
		//delay(1200);
		Can_Send_Msg(AL9000_CAN0,tx_buf5,XMIT_PTB_MODE,data_length_64,0x17);
	//}
	}
	//Can_Receive_Msg(AL9000_CAN0,rx_buf1,data_length_20);
	//}
	//Can_Receive_Msg(AL9000_CAN0,rx_buf1,data_length_20);
	//Can_Send_Msg(AL9000_CAN0,tx_buf1,XMIT_PTB_MODE,data_length_20);
	//Can_Receive_Msg(AL9000_CAN1,rx_buf2,data_length_20);
	//Can_Send_Msg(AL9000_CAN1,tx_buf2,XMIT_PTB_MODE,data_length_20);
        //Can_Receive_Msg(AL9000_CAN0,rx_buf1,data_length_20);
	for (volatile uint8_t i = 0 ;i < 8 ; i++)
	{
		//Can_Receive_Msg(AL9000_CAN0,rx_buf);
	}

 //test(AL9000_CAN0);
		return 0;
}
