/*
 * main.c
 *
 *  Created on: 2021年11月10日
 *      Author: wei.pang
 */
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include <time.h>
/*
 *
 *
 *
 *
 *
*/
#define LLP_BASE_ADDR      0x61030200
#define LLP_1_ADDR         0x61030200
#define LLP_2_ADDR         0x61030300
#define LLP_3_ADDR         0x61030400
#define LLP_4_ADDR         0x61030500
#define base0 0x61030000
#define base1 0x61030100
void SOC_DMA_AHB_HANDLER(void)
{
    // printf("enter_irq_sucess\n\r");
    // printf("DMAC_MEM2MEM_LLP_MODE has been completed\n\r");
    for(int i = 0 ; i< 5 ; i++)
    {
		// printf("ocm_data LLP_1_ADDR = %.2x\r\n",*(uint32_t*)(LLP_1_ADDR + i*4));
		// printf("ocm_data LLP_2_ADDR = %.2x\r\n",*(uint32_t*)(LLP_2_ADDR + i*4));
		// printf("ocm_data LLP_3_ADDR = %.2x\r\n",*(uint32_t*)(LLP_3_ADDR + i*4));
		// printf("ocm_data LLP_4_ADDR = %.2x\r\n",*(uint32_t*)(LLP_4_ADDR + i*4));
		// printf("ocm_data MEM_BASE1_ADDR = %.2x\r\n",*(uint32_t*)(base0 + i*4));
		// printf("ocm_data MEM_BASE2_ADDR = %.2x\r\n",*(uint32_t*)(base1 + i*4));
    	if((*(uint32_t*)(base0 + i*4)) == (*(uint32_t*)(base1 + i*4)))
    	{
    		// printf("dmac_transmissiom_pass\r\n");
    	}
    	else
    	{
    		printf("[AUTOTEST]:[DMAC]:[LLP_MODE]:[FAIL]\r\n");
			while(1);
    	}
    }
	printf("[AUTOTEST]:[DMAC]:[LLP_MODE]:[PASS]\r\n");
    while(1);
}
    int main(void){
#if __riscv
	__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
#endif

    ECLIC_Register_IRQ(SOC_INT65_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_DMA_AHB_HANDLER);
	__enable_irq();
    AlDma_EnableChannelIrq(AL_DMAC_channel_0);
    AlDma_UnmaskIrq(AL_DMAC,AL_dmac_channel_num_0,block_1);
	AlDma_SetAddress(AL_DMAC_channel_0,Dmac_src,base0);
    AlDma_SetAddress(AL_DMAC_channel_0,Dmac_dst,base1);
	AlDma_SetChannelConfig(AL_DMAC_channel_0);
	AlDma_SetTransferType(AL_DMAC_channel_0,Dmac_transfer_row10);
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
	AlDma_CheckChannelBusy(AL_DMAC);
	AlDma_SetListPointerAddress(AL_DMAC_channel_0,LMS_master1,LLP_BASE_ADDR);
    volatile uint32_t buffer[10];
    volatile uint32_t buffer_list_parm[4][5]={{0x61030000,0x61030100,0x61030300,0x18004825,0x00000008},
						    					{0x61030000,0x61030100,0x61030400,0x18004825,0x00000008},
												{0x61030000,0x61030100,0x61030500,0x00004825,0x00000008},
												{0x61030000,0x61030100,0x61030600,0x00004825,0x00000008}};
	 for (volatile uint32_t i = 0 ; i < 10 ; i++)
	 {
		 buffer[i] = i;
	 }
	 write_To_OCM((uint32_t*)buffer,10,(uint32_t*)base0);
    for (volatile uint32_t i = 0 ; i < 4 ; i++){
		for (volatile uint32_t j = 0 ; j < 5 ; j++){
 			buffer[j] = (uint32_t)buffer_list_parm[i][j];
 		}
    switch(i){
		case 0:
		      write_To_OCM((uint32_t*)buffer,5,(uint32_t*)LLP_1_ADDR );
		break;
		case 1:
		      write_To_OCM((uint32_t*)buffer,5,(uint32_t*)LLP_2_ADDR);
		break;
		case 2:
			  write_To_OCM((uint32_t*)buffer,5,(uint32_t*)LLP_3_ADDR);
	    break;
		default:
		      write_To_OCM((uint32_t*)buffer,5,(uint32_t*)LLP_4_ADDR);
		break;
		}
	}
	 AlDma_Enable(AL_DMAC);
	 AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_0);
	return 0;
}


