
/*
 * main.c
 *
 *  Created on: 2021年11月10日
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "AL_uart.h"
#include "AL_dmac.h"
#include "AL_can.h"
#include <time.h>
uint8_t tx_buf1[64];
uint8_t tx_buf2[64];
uint8_t tx_buf3[64];
uint8_t tx_buf4[64];
uint8_t tx_buf5[64];
uint8_t rx_buf1[64];
uint8_t rx_buf2[64];
uint8_t rx_buf3[64];
uint8_t rx_buf4[64];
uint8_t rx_buf5[64];

void SOC_DMA_AHB_HANDLER(void)
{
    printf("enter_irq_number%d\n\r");
    //dma_interrupt_readfunction(LLP_BASE_ADDR);      
    //dw_dmac_clearIrq(AL_DMAC,dw_dmac_channel_num_1,block_1);
}
void can_data_cheak(uint32_t count){
	volatile uint32_t i = 0 ;
	for(i = 0 ; i < count ; i ++ ){
		printf("MEM1_DATA = %d",tx_buf2[i]);
					printf("\r\n");
					printf("MEM2_DATA = %08x",*(uint8_t*)(MEM_BASE2_ADDR + i+8));
					printf("\r\n");
		if(tx_buf2[i] == (*(uint8_t*)(MEM_BASE2_ADDR + i+8)))
		{
			printf("MEM1_DATA = %d",tx_buf2[i]);
			printf("\r\n");
			printf("MEM2_DATA = %d",*(uint8_t*)(MEM_BASE2_ADDR + i+8));
			printf("\r\n");
			printf("check data is right\r\n");
			}
		else{
			printf("check data is fail please check you dma set data\r\n");
		}

			printf("\r\n");
	}
}
void clear_MEM(){
	uint32_t Clear_buffer[4096];
	for(uint32_t i = 0 ; i < 4096 ; i++){
		Clear_buffer[i] = 0;
	}
	write_To_OCM((uint32_t*)Clear_buffer,4096,(uint32_t*)MEM_BASE1_ADDR);
	write_To_OCM((uint32_t*)Clear_buffer,4096,(uint32_t*)MEM_BASE2_ADDR);
}
int main(void){
	for (volatile uint8_t i = 0 ;i < 64 ; i++)
	{
		tx_buf1[i] = i;
		tx_buf2[i] = i+1;
		tx_buf3[i] = i+2;
		tx_buf4[i] = i+3;
		tx_buf5[i] = i+4;
	}
    Enablepinmux1();
    AlCan_SetResetMode(AL_CAN1);
    ECLIC_Register_IRQ(SOC_INT65_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_DMA_AHB_HANDLER);
	__enable_irq();
	clear_MEM();
    AlDma_EnableChannelIrq(AL_DMAC_channel_1);
    AlDma_UnmaskIrq(AL_DMAC,AL_dmac_channel_num_1,block_1);
    AlCan_SetResetMode(AL_CAN1);
    AlCan_DeviceDriverBittimeConfiguration(AL_CAN1,rate_5Mbit,AL_TOP0,can_fd);
    AlCan_TxMode(AL_CAN1,NORMAL);
    AlCan_TmitMode(AL_CAN1,XMIT_PTB_MODE);
    AlCan_DmaInit(AL_DMAC_channel_1,can1,Dmac_transfer_row1);
    AlDma_Enable(AL_DMAC);
    AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_1);
	while(AlDma_CheckChannelBusy(AL_DMAC));
	can_data_cheak(64);
    return 0;
}








