/*
 * main.c
 *
 *  Created on: 2021年11月10日
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_uart.h"
#include "al9000_dmac.h"
#include <time.h>
#include "anl_printf.h"
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
    printf("enter_irq_number = %d\n\r",count);
     if(count == 10){
       dw_dmac_setSingle(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst);
       dw_dmac_clearIrq(AL9000_DMAC,dw_dmac_channel_num_1,tfr_l);
       dw_dmac_enable(AL9000_DMAC);
       //count = 0;
  	 for(int i = 0 ; i< 10; i++)
  	 {
  	 	printf("ocm_data1 = %.2x\r\n",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
  	 	printf("ocm_data2 = %.2x\r\n",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
  	 	if((*(uint32_t*)(MEM_BASE1_ADDR + i*4)) == (*(uint32_t*)(MEM_BASE2_ADDR + i*4)))
  	 	    	{
  	 	    		printf("dmac_transmissiom_pass\r\n");
  	 	    	}
  	 	    	else
  	 	    	{
  	 	    		printf("dmac_transmissiom_fail\r\n");
  	 	    		while(1);
  	 	    	}
  	 }
       printf("DMAC_MEM2MEM_RELOAD_MODE has been completed\n\r");
    }
    else{
           dw_dmac_clearIrq(AL9000_DMAC,dw_dmac_channel_num_1,block_1);
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
	/*
 	 *dw_dmac_enableChannelIrq()
 	 */
         dw_dmac_enableChannelIrq(AL9000_DMAC_channel_0,dw_dmac_channel_num_1);
	/*
	 *dw_dmac_unmaskIrq()
	 */
         dw_dmac_unmaskIrq(AL9000_DMAC,dw_dmac_channel_num_1,block_1);

	/*
	 * dw_dmac_setAddress
	 *
	 */
	 dw_dmac_setAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src,MEM_BASE1_ADDR);
		/*
		 * dw_dmac_setAddress
		 */
         dw_dmac_setAddress(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_dst,MEM_BASE2_ADDR);

	/*
	 * dw_dmac_setChannelConfig
	 */
	 dw_dmac_setChannelConfig(AL9000_DMAC_channel_0,dw_dmac_channel_num_1);
	 /*
	  * dw_dmac_setTransferType
	  */
	 dw_dmac_setTransferType(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_transfer_row2,Dmac_src);
	 /*
	  * dw_dmac_setMemPeriphFlowCtl
	  */
	 dw_dmac_setMemPeriphFlowCtl(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,MEM2MEM);
	 /*
	  * dw_dmac_setTransWidth
	  * you can set the Transwidth :8/16/32/64/256 MAX_WIDTH=256
	  */
	 dw_dmac_setTransWidth(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,WIDTH_32);
	 /*
	  * dw_dmac_setBlockTransSize
	  * you can set the number of burst in one of the block
	  * range MIN:MAX = 1:4096
	  */
	 dw_dmac_setBlockTransSize(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,10);
	 /*
	  * dw_dmac_setBurstTransLength
	  */
	 dw_dmac_setBurstTransLength(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,DW_DMA_MSIZE_8);
	 /*
	  *  dw_dmac_setMstSelect
	  */
	 dw_dmac_setMstSelect(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,SMS_master1,DMS_master1);
	 /*
	  * dw_dmac_setAddressInc
	  */
	 dw_dmac_setAddressInc(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,Increments);
	 /*
	  * dw_dmac_setHandshakingMode
	  */
	 dw_dmac_setHandshakingMode(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,Hardware_handshaking);
	 /*
	  * dw_dmac_setHsPolarity
	  */
	 dw_dmac_setHsPolarity(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,Dmac_src_dst,ACTIVE_HIGH);
	 /*
	  * dw_dmac_setFifoMode
	  */
	 dw_dmac_setFifoMode(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,FIFO_MODE_0);

	 /*
	  * dw_dmac_setFlowCtlMode
	  */
	 dw_dmac_setFlowCtlMode(AL9000_DMAC_channel_0,dw_dmac_channel_num_1,fc_mode_0);
	 /*
	  * dw_dmac_checkChannelBusy
	  */

          volatile uint32_t buffer[128];
	/* for (volatile uint32_t i = 0 ; i < 128 ; i++)
	 {
		 buffer[i] = i;
	 }
	 write_To_OCM((uint32_t*)buffer,128,(uint32_t*)MEM_BASE1_ADDR);*/
  	 /*
	  * dw_dmac_enable
	  */
	 dw_dmac_enable(AL9000_DMAC);
	 /*
	  * dw_dmac_enableChannel
	  */
	 dw_dmac_enableChannel(AL9000_DMAC,dw_dmac_channel_num_1);

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


