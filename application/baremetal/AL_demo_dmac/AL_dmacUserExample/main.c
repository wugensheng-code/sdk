/*
 * main.c
 *
 *  Created on: 2021年11月10日
 *      Author: wei.pang
 */
#if 1
#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "AL_uart.h"
#include "AL_dmac.h"
#include <time.h>
/*
 *instruction
 *test_mode		:reload mode
 *importannt parameter	:
 *
 *
 *
*/
#define ddr_data_base 0x00010000
#define ddr_src_base  0x00100000
#define LLP_BASE_ADDR      0x61030200
#define LLP_1_ADDR         0x61030200
#define LLP_2_ADDR         0x61030300
#define LLP_3_ADDR         0x61030400
#define LLP_4_ADDR         0x61030500
#define DDR_B00        	   0x00000b00
uint8_t amount0;	//byte_data transfer number
uint32_t rdata0_0;	//
uint8_t ckdata0_0[64];	// check data array
uint8_t ckdata0_1[64];	// check data array
uint8_t tx_buf1[64];
uint32_t block_number_now = 20;

void mem_data_check(uint32_t count){
	volatile i = 0;
		printf("OCM write data is :\r\n");
	for(i = 0 ; i < count ; i++){
		printf("MEM1_DATA = %3d",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
		printf("\r\n");
		printf("MEM2_DATA = %3d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
		printf("\r\n");
		if((*(uint32_t*)(MEM_BASE1_ADDR + i*4)) == (*(uint32_t*)(MEM_BASE2_ADDR + i*4))){
					printf("check data is right\r\n");
				}
				else{
					printf("check data is fail please check you dma set data\r\n");
				}

	printf("\r\n");
	}
}
void printf_mem_data(uint32_t count){
	volatile i = 0;
	printf("OCM write data is :\r\n");
		for(i = 0 ; i < count ; i++){
			printf("%4d",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
		}
		printf("\r\n");
}
void printf_block_data(uint32_t address,uint32_t count){
	volatile i = 0;
	for(i = 0 ; i < count*5 ; i ++){
		if(i%5 == 0 && i != 0){
			printf("\r\n");
		}
		if(i%5 == 0 || i == 0){
				printf("srcaddress = %08x",(*(uint32_t*)(address + i*4)));
		}
		if(i%5 == 1 ){
			printf("     dstaddress = %08x",(*(uint32_t*)(address + i*4)));
		}
		if(i%5 == 2 ){
			printf("     llpaddress = %08x",(*(uint32_t*)(address + i*4)));
		}
		if(i%5 == 3 ){
			printf("    parl = %08x",(*(uint32_t*)(address + i*4)));
		}
		if(i%5 == 4){
			printf("    parh = %08x",(*(uint32_t*)(address + i*4)));
		}

	}
		printf("\r\n");
}



void clear_MEM(){
	uint32_t Clear_buffer[4096];
	for(uint32_t i = 0 ; i < 4096 ; i++){
		Clear_buffer[i] = 0;
	}
	write_To_OCM((uint32_t*)Clear_buffer,4096,(uint32_t*)MEM_BASE1_ADDR);
	write_To_OCM((uint32_t*)Clear_buffer,4096,(uint32_t*)MEM_BASE2_ADDR);
}
    volatile uint16_t count = 0;
void SOC_DMA_AHB_HANDLER(void){
		count ++;
		uint32_t block_number_complete = 0;
		uint32_t block_last_number = 0;
		uint32_t DST_ADDR_NOW = 0;
		printf("enter irq success!!!!!!!\r\n");
		printf("Enter irq count =  %d\r\n",count);
		block_number_complete = cpu_dma_read_block_num(LLP_BASE_ADDR,200);
		block_last_number = block_number_now - block_number_complete;
		DST_ADDR_NOW = (*(uint32_t*)(LLP_BASE_ADDR + 4));
		printf("LLP_ADDR_NOW %08x\r\n",DST_ADDR_NOW);
		printf("block_number_complete  = %d\r\n",block_number_complete);
		printf("block_last_number =  %d\r\n",block_last_number);
		//printf("status block interrput %d\n\r",GET_BITS());
		printf("\r\n");
		AlDma_ClearIrq(AL_DMAC);
    }
int main(){
	uint32_t i = 0;
	uint32_t j = 0;
	 volatile uint32_t buffer[256];
	 for (volatile uint32_t i = 0 ; i < 256 ; i++)
	 {
		 buffer[i] = i;
	 }
	*(uint32_t *)(0xf8801074) |= 0x4000;
	AlMpu_Disable(AL_MPU_DDRS0);
	AlMpu_Disable(AL_MPU_DDRS1);
	//Enablepinmux1();
   /***********************AL***************************/
  printf("test start\r\n");
   /*variable init*/
   uint16_t txfifo_start_level = 0;
   uint16_t rxdata_num = 0;
   uint8_t wrdata_a[256] = {0};
   uint8_t rddata_a[256] = {0};
   uint32_t wrdata_tmp = 0;
   uint32_t rddata_tmp = 0;
   for(i=0; i<256; i++)
   {
   	wrdata_a[i] = i;
   }

	uint32_t flash_id = 0;
	//CFG SSIENR
	/*dis ssi*/
	//DisableDCache();
	//__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
	//DisableDCache();
	//EnableDCache();
#if 1
    ECLIC_Register_IRQ(SOC_INT65_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_DMA_AHB_HANDLER);
    __enable_irq();
    AlDma_UnmaskIrq(AL_DMAC,AL_dmac_channel_num_0,block_1);
    AlDma_EnableChannelIrq(AL_DMAC_channel_0);
	AlDma_SetListPointerAddress(AL_DMAC_channel_0,LMS_master1,LLP_BASE_ADDR);
	AlCan_SetResetMode(AL_CAN1);
	AlCan_DeviceDriverBittimeConfiguration(AL_CAN1,rate_2Mbit,AL_TOP0,can_fd);
	AlCan_TxMode(AL_CAN1,NORMAL);
	//AlUart_SetRxTrigger(AL_UART0, RX_FIFO_1_2full);
	//AlUart_SetDmaMode(AL_UART0);
	//AlUart_DmaInit(AL_DMAC_channel_0,uart0,rx,Dmac_transfer_row10,32);
	AlCan_DmaInit(AL_DMAC_channel_0,can1,Dmac_transfer_row10);
	//AlDma_Disable(AL_DMAC);
	//AlDma_DisableChannel(AL_DMAC,AL_dmac_channel_num_0);
	/*for(volatile uint32_t i = 0 ; i < 16 ; i++){
		printf("ocm_data MEM_BASE1_ADDR = %08x\r\n",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
	}*/
	//printf("* Example: DMA_e2prom write and read test.\n");
    // poll-driven master-transmitter example
	//write_iic_data(normal,16);
    //i2c_e2prom(I2C0);
    //iic_data_cheak(15);
	clear_MEM();
	AlDma_blocklist(AL_CAN1_BASE,ddr_data_base,rx,20,LLP_BASE_ADDR,ALCAN);
	//write_To_OCM((uint32_t*)buffer1,50,(uint32_t*)MEM_BASE1_ADDR);
	/*for(i = 0 ; i < 20 ; i ++){
		if(i%5 == 0 && i != 0){
			printf("\r\n");
		}
		printf("	dst_data = %d",(*(uint32_t*)(MEM_BASE1_ADDR + i*4)));
	}*/
	printf("\r\n");
	for(i = 0 ; i < 32*5 ; i ++){
		if(i%5 == 0 && i != 0){
			printf("\r\n");
		}
		if(i%5 == 0 || i == 0){
				printf("srcaddress = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
		}
		if(i%5 == 1 ){
			printf("     dstaddress = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
		}
		if(i%5 == 2 ){
			printf("     llpaddress = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
		}
		if(i%5 == 3 ){
			printf("    parl = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
		}
		if(i%5 == 4){
			printf("    parh = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
		}

	}
	printf("\r\n");
	 AlDma_Enable(AL_DMAC);
	 AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_0);
	 while(AlDma_CheckChannelBusy(AL_DMAC));
	 uint32_t block_number = 0;
		block_number = cpu_dma_read_block_num(LLP_BASE_ADDR,200);
		//printf("block_number %d\n\r",count);
	printf("complete_DMAC_transfer %d\n\r",block_number);
	 printf("dma data write back\r\n");
		for(i = 0 ; i < 100 ; i ++){
			if(i%5 == 0 && i != 0){
				printf("\r\n");
			}
			if(i%5 == 0 || i == 0){
					printf("srcaddress = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
			}
			if(i%5 == 1 ){
				printf("     dstaddress = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
			}
			if(i%5 == 2 ){
				printf("     llpaddress = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
			}
			if(i%5 == 3 ){
				printf("    parl = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
			}
			if(i%5 == 4){
				printf("    parh = %08x",(*(uint32_t*)(LLP_BASE_ADDR + i*4)));
			}

		}
		printf("\r\n");
		for( j = 0 ; j<20 ;j++){
		for(i = 0 ; i < 20 ; i ++){
			if(i%5 == 0 && i != 0){
				printf("\r\n");
			}
			printf("	dst_data = %08x",(*(uint32_t*)((ddr_data_base+80 *j) + i*4)));
		}
	}
	return 0;
#endif
}
#endif
#if 0
/*
 * main.c
 *
 *  Created on: 2021年11月10日
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
#define ddr_data_base 0x00010000
#define ddr_src_base  0x00100000
#define LLP_1_ADDR         0x61030200
#define LLP_2_ADDR         0x61030300
#define LLP_3_ADDR         0x61030400
#define LLP_4_ADDR         0x61030500
#define DDR_B00        	   0x00000b00
uint8_t amount0;	//byte_data transfer number
uint32_t rdata0_0;	//
uint8_t ckdata0_0[64];	// check data array
uint8_t ckdata0_1[64];	// check data array
uint8_t tx_buf1[64];
uint32_t block_number_now = 20;
#define S25FL256S_ID	0x00190201  // flash ID




void clear_MEM(){
	uint32_t Clear_buffer[4096];
	for(uint32_t i = 0 ; i < 4096 ; i++){
		Clear_buffer[i] = 0;
	}
	write_To_OCM((uint32_t*)Clear_buffer,4096,(uint32_t*)MEM_BASE1_ADDR);
	write_To_OCM((uint32_t*)Clear_buffer,4096,(uint32_t*)MEM_BASE2_ADDR);
}
    volatile uint16_t count = 0;
void SOC_DMA_AHB_HANDLER(void){
		count ++;
		uint32_t block_number_complete = 0;
		uint32_t block_last_number = 0;
		uint32_t DST_ADDR_NOW = 0;
		printf("enter irq success!!!!!!!\r\n");
		printf("Enter irq count =  %d\r\n",count);
		block_number_complete = cpu_dma_read_block_num(LLP_BASE_ADDR,200);
		block_last_number = block_number_now - block_number_complete;
		DST_ADDR_NOW = (*(uint32_t*)(LLP_BASE_ADDR + 4));
		printf("LLP_ADDR_NOW %08x\r\n",DST_ADDR_NOW);
		printf("block_number_complete  = %d\r\n",block_number_complete);
		printf("block_last_number =  %d\r\n",block_last_number);
		//printf("status block interrput %d\n\r",GET_BITS());
		printf("\r\n");
		AlDma_ClearIrq(AL_DMAC);
    }
int main(){
	uint32_t i = 0;
	uint32_t j = 0;
    volatile uint32_t buffer[128];
    volatile uint32_t buffer1[128];
	for (volatile uint32_t i = 0 ; i < 128 ; i++){
		 buffer[i] = 0xaa559966;
		 buffer1[i] = 0x20559966;
	 }
   /***********************AL***************************/
  printf("test start\r\n");
   /*variable init*/
   uint16_t txfifo_start_level = 0;
   uint16_t rxdata_num = 0;
   uint8_t wrdata_a[256] = {0};
   uint8_t rddata_a[256] = {0};
   uint32_t wrdata_tmp = 0;
   uint32_t rddata_tmp = 0;
   for(i=0; i<256; i++)
   {
   	wrdata_a[i] = i;
   }
   *(uint32_t *)(0xf8801074) |= 0x4000;
   AlMpu_Disable(AL_MPU_DDRS0);
   AlMpu_Disable(AL_MPU_DDRS1);
	clear_MEM();
	printf("\r\n");
	while(1){
		clear_MEM();
		write_To_OCM((uint32_t*)buffer,100,(uint32_t*)DDR_B00);
		write_To_OCM((uint32_t*)buffer1,100,(uint32_t*)ddr_src_base);
		test_blocklist(ddr_src_base,ddr_data_base,tx,100,DDR_BASE);
		for(i = 0 ; i < 50 ; i ++){
			if(i%5 == 0 && i != 0){
				printf("\r\n");
			}
			printf("	src_data = %08x",(*(uint32_t*)(ddr_src_base + i*4)));
		}
		printf("\r\n");
		for(i = 0 ; i < 500 ; i ++){
			if(i%5 == 0 && i != 0){
				printf("\r\n");
			}
			if(i%5 == 0 || i == 0){
					printf("srcaddress = %08x",(*(uint32_t*)(DDR_BASE + i*4)));
			}
			if(i%5 == 1 ){
				printf("     dstaddress = %08x",(*(uint32_t*)(DDR_BASE + i*4)));
			}
			if(i%5 == 2 ){
				printf("     llpaddress = %08x",(*(uint32_t*)(DDR_BASE + i*4)));
			}
			if(i%5 == 3 ){
				printf("    parl = %08x",(*(uint32_t*)(DDR_BASE + i*4)));
			}
			if(i%5 == 4){
				printf("    parh = %08x",(*(uint32_t*)(DDR_BASE + i*4)));
			}

		}
		printf("\r\n");
	}

}

#endif
