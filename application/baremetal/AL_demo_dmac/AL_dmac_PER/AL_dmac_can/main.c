
/*
 * main.c
 *
 *  Created on: 2021年11月10日
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
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

/*void SOC_DMA_AHB_HANDLER(void)
{
    //printf("enter_irq_number%d\n\r",count); 
    //dma_interrupt_readfunction(LLP_BASE_ADDR);      
    dw_dmac_clearIrq(AL_DMAC,dw_dmac_channel_num_1,block_1);
}*/
void can_data_cheak(uint32_t count){
	volatile i = 0 ;
	for(i = 0 ; i < count ; i ++ ){
		printf("MEM1_DATA = %3d",tx_buf2[i]);
					printf("\r\n");
					printf("MEM2_DATA = %3d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
					printf("\r\n");
		if(tx_buf2[i] == (*(uint32_t*)(MEM_BASE2_ADDR + i*4)))
		{
			printf("MEM1_DATA = %3d",tx_buf2[i]);
			printf("\r\n");
			printf("MEM2_DATA = %3d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
			printf("\r\n");
			printf("check data is right\r\n");
			}
		else{
			printf("check data is fail please check you dma set data\r\n");
		}

			printf("\r\n");
	}
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
    __RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    AlCan_SetResetMode(AL_CAN0);
    AlCan_SetResetMode(AL_CAN1);
 #if 0
	/*mem2men mode
	 *If you want to use interrupts to count the number of blocks
 	 *You can open this part
 	 *But we do not recommend this
 	 *interrupts_count_blocks_mode will stay in interrupt fo a long time,reduce DMA efficiency
 	 *
 	 */
        ECLIC_Register_IRQ(SOC_INT65_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_DMA_AHB_HANDLER);
	__enable_irq();
	/*
 	 *dw_dmac_enableChannelIrq()
 	 */
         dw_dmac_enableChannelIrq(AL_DMAC_channel_0,dw_dmac_channel_num_1);
	/*
	 *dw_dmac_unmaskIrq()
	 */
    dw_dmac_unmaskIrq(AL_DMAC,dw_dmac_channel_num_1,block_1);

    #endif
    AlCan_SetResetMode(AL_CAN0);
    AlCan_DeviceDriverBittimeConfiguration(AL_CAN0,rate_5Mbit,AL_TOP0,can_fd);
    AlCan_TxMode(AL_CAN0,NORMAL);
    AlCan_TmitMode(AL_CAN0,XMIT_PTB_MODE);
    AlDma_Enable(AL_DMAC);
    AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_1);
    while(1){
    	//Can_Receive_Msg(AL_CAN0,rx_buf2,data_length_8);
    	AlCan_DmaInit(AL_DMAC_channel_1,can0,Dmac_transfer_row1);
    	//while(dw_dmac_checkChannelBusy(AL_DMAC));
    	can_data_cheak(64);
    }
        	return 0;
}








