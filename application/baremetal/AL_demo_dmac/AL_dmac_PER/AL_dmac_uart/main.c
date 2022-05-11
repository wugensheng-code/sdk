
/*
 * main.c
 *
 *  Created on: 2021�?11�?10�?
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al_uart.h"
#include "al_dmac.h"
#include <time.h>
/*
 *instruction
 *test_mode		:reload mode
 *importannt parameter	:
 *
 *
 *
*/
volatile uint16_t count = 0;
void SOC_DMA_AHB_HANDLER(void)
{
    //printf("enter_irq_number%d\n\r",count);        
      count ++;
     if(count == 10){
       AlDma_SetSingle(AL_dmac_channel_num_1,Dmac_src_dst);
       AlDma_ClearIrq(AL_DMAC);
       AlDma_Enable(AL_DMAC);
       //count = 0;
       //printf("complete_DMAC_transfer\n\r");    
    }
    else{
           AlDma_ClearIrq(AL_DMAC);
         }  
}
    int main(void){
	 AlDma_SetChannelConfig(AL_dmac_channel_num_1);
	 AlDma_SetAddress(AL_dmac_channel_num_1,Dmac_src,MEM_BASE1_ADDR);
     AlDma_SetAddress(AL_dmac_channel_num_1,Dmac_dst,AL_UART0_BASE);
	 AlDma_SetAddressInc(AL_dmac_channel_num_1,Dmac_dst,FIX);
	 //AlDma_SetChannelConfig(AL_dmac_channel_num_1);
	 AlDma_SetTransferType(AL_dmac_channel_num_1,Dmac_transfer_row1);
	 AlDma_SetMemPeriphFlowCtl(AL_dmac_channel_num_1,MEM2PER);
	 AlDma_SetTransWidth(AL_dmac_channel_num_1,Dmac_src_dst,WIDTH_16);
	 AlDma_SetBlockTransSize(AL_dmac_channel_num_1,50);
	 AlDma_SetBurstTransLength(AL_dmac_channel_num_1,Dmac_src_dst,AL_DMA_MSIZE_32);
          volatile uint32_t buffer[128];
	 for (volatile uint32_t i = 0 ; i < 100 ; i++)
	 {
		 buffer[i] = i;
	 }
	 write_To_OCM((uint32_t*)buffer,128,(uint32_t*)MEM_BASE1_ADDR);
  	 /*
	  * AlDma_Enable
	  */
	 AlUart_SetTxTrigger(AL_UART0,FIFO_1_2full);
	 AlUart_SetDmaMode(AL_UART0);
	 AlDma_Enable(AL_DMAC);
	 /*
	  * AlDma_EnableChannel
	  */
	 AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_1);
	return 0;
}



