/*
 * main.c
 *
 *  Created on: 2021年11月10日
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
//    printf("enter_irq_number = %d\n\r",count);
     if(count == 10){
       AlDma_SetSingle(AL_DMAC_channel_0,Dmac_src_dst);
       AlDma_ClearIrq(AL_DMAC);
       AlDma_Enable(AL_DMAC);
       //count = 0;
  	 for(int i = 0 ; i< 10; i++)
  	 {
//  	 	 printf("ocm_data1 = %.2x\r\n",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
//  	 	 printf("ocm_data2 = %.2x\r\n",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
  	 	if((*(uint32_t*)(MEM_BASE1_ADDR + i*4)) == (*(uint32_t*)(MEM_BASE2_ADDR + i*4)))
  	 	    	{
  	 	    		// printf("dmac_transmissiom_pass\r\n");
  	 	    	}
  	 	    	else
  	 	    	{
  	 	    		printf("[AUTOTEST]:[DMAC]:[RELOAD_MODE]:[FAIL]\r\n");
  	 	    		while(1);
  	 	    	}
  	 }
       printf("[AUTOTEST]:[DMAC]:[RELOAD_MODE]:[PASS]\r\n");
    }
    else{
           AlDma_ClearIrq(AL_DMAC);
         }
     count ++;
}
    int main(void){
	__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
	__enable_irq();
    //setvbuf(stdout, NULL, _IONBF, 0);
	/*mem2men mode
	 *If you want to use interrupts to count the number of blocks
 	 *You can open this part
 	 *But we do not recommend this
 	 *interrupts_count_blocks_mode will stay in interruptttttt fo a long time,reduce DMA efficiency
 	 *
 	 */

        ECLIC_Register_IRQ(SOC_INT65_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_DMA_AHB_HANDLER);
	__enable_irq();

     AlDma_EnableChannelIrq(AL_DMAC_channel_0);
     AlDma_UnmaskIrq(AL_DMAC,AL_dmac_channel_num_0,block_1);
	 AlDma_SetAddress(AL_DMAC_channel_0,Dmac_src,MEM_BASE1_ADDR);
     AlDma_SetAddress(AL_DMAC_channel_0,Dmac_dst,MEM_BASE2_ADDR);
	 AlDma_SetChannelConfig(AL_DMAC_channel_0);
	 AlDma_SetTransferType(AL_DMAC_channel_0,Dmac_transfer_row4);
	 AlDma_SetMemPeriphFlowCtl(AL_DMAC_channel_0,MEM2MEM);
	 AlDma_SetTransWidth(AL_DMAC_channel_0,Dmac_src_dst,WIDTH_32);
	 AlDma_SetBlockTransSize(AL_DMAC_channel_0,10);
	 AlDma_SetBurstTransLength(AL_DMAC_channel_0,Dmac_src_dst,AL_DMA_MSIZE_8);
	 AlDma_SetMstSelect(AL_DMAC_channel_0,Dmac_src_dst,SMS_master1,DMS_master1);
	 AlDma_SetAddressInc(AL_DMAC_channel_0,Dmac_src_dst,Increments);
	 AlDma_SetHandshakingMode(AL_DMAC_channel_0,Dmac_src_dst,Hardware_handshaking);
	 AlDma_SetHsPolarity(AL_DMAC_channel_0,Dmac_src_dst,ACTIVE_HIGH);
	 AlDma_SetFifoMode(AL_DMAC_channel_0,FIFO_MODE_0);
	 AlDma_SetFlowCtlMode(AL_DMAC_channel_0,fc_mode_0);

          volatile uint32_t buffer[128];
	 for (volatile uint32_t i = 0 ; i < 128 ; i++)
	 {
		 buffer[i] = i;
	 }
	 write_To_OCM((uint32_t*)buffer,128,(uint32_t*)MEM_BASE1_ADDR);
  	 /*
	  * AL_dmac_enable
	  */
	 AlDma_Enable(AL_DMAC);
	 /*
	  * AlDma_EnableChannel
	  */
	 AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_0);

  	 /*for(int i = 0 ; i< 128; i++)
  	 {
  	 	printf("tx_ocm_data = %.2x\r\n",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
  	 }
  	 for(int i = 0 ; i< 128; i++)
  	 {
  	 	printf("rx_ocm_data = %.2x\r\n",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
  	 }*/
	return 0;
}


