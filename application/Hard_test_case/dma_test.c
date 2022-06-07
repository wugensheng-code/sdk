/*
 * dma_test.c
 *
 *  Created on: 2022年5月5日
 *      Author: Administrator
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"
volatile uint16_t count = 0;
void DMA_SOC_DMA_AHB_HANDLER(void)
{
    //printf("enter_irq_number%d\n\r",count);
    printf("enter_irq_sucess\n\r");
    printf("DMAC_MEM2MEM_SINGLE_MODE has been completed\n\r");
	 for(int i = 0 ; i<10; i++)
	 {
	 	//printf("ocm_data1 = %.2x\r\n",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
	 	//printf("ocm_data2 = %.2x\r\n",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
    	if((*(uint32_t*)(MEM_BASE1_ADDR + i*4)) == (*(uint32_t*)(MEM_BASE2_ADDR + i*4)))
    	{
    		printf("dmac_transmissiom_pass\r\n");
    	}
    	else
    	{
    		printf("dmac_transmissiom_fail\r\n");
    	}
	 }
	 AlDma_ClearIrq(AL_DMAC);
}

void dma_test(){
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");
    printf("***************************DMA TEST*****************************\r\n");
    ECLIC_Register_IRQ(SOC_INT65_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,DMA_SOC_DMA_AHB_HANDLER);
    __enable_irq();
    AlDma_EnableChannelIrq(AL_DMAC_channel_0);
    AlDma_UnmaskIrq(AL_DMAC,AL_dmac_channel_num_0,tfr_l);
	AlDma_SetAddress(AL_DMAC_channel_0,Dmac_src,MEM_BASE1_ADDR);
    AlDma_SetAddress(AL_DMAC_channel_0,Dmac_dst,MEM_BASE2_ADDR);
	AlDma_SetChannelConfig(AL_DMAC_channel_0);
	AlDma_SetTransferType(AL_DMAC_channel_0,Dmac_transfer_row1);
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
	for (volatile uint32_t i = 0 ; i < 10 ; i++){
	      buffer[i] = i;
	}
	write_To_OCM((uint32_t*)buffer,10,(uint32_t*)MEM_BASE1_ADDR);
	AlDma_Enable(AL_DMAC);
	AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_0);
	printf("DMA test success\r\n");
}
