
/*
 * main.c
 *
 *  Created on: 2021�?11�?10�?
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
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
     volatile uint32_t buffer[256];
	 for (volatile uint32_t i = 0 ; i < 256; i++)
	 {
		 buffer[i] = i;
	 }
	 write_To_OCM((uint32_t*)buffer,256,(uint32_t*)MEM_BASE1_ADDR);
     Enablepinmux1();
	 AlUart_SetTxTrigger(AL_UART0,FIFO_1_2full);
	 AlUart_SetDmaMode(AL_UART0);
	 AlUart_DmaInit(AL_DMAC_channel_1,uart0,tx,Dmac_transfer_row1,50);
	 AlDma_Enable(AL_DMAC);
	 AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_1);
	return 0;
}



