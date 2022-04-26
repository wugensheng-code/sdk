
/*
 * main.c
 *
 *  Created on: 2021�?11�?10�?
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#include "al9000_uart.h"
#include "al9000_dmac.h"
#include "al9000_spi.h"
#include <time.h>
#define S25FL512S_ID	0X00200201  // flash ID
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
/*
 *instruction
 *test_mode		:reload mode
 *importannt parameter	:
 *
 *
 *
*/
uint32_t SReadID(void)
{
	//-----------------------------------------------------------
	//sent:
	//inst(0x): 9f (RDID)
	//receive: 3 data items
	//-----------------------------------------------------------
	uint32_t temp = 0;
	spi_data_transmit(SPI0,CMD_RDID); // tx read id cmd
	while(!spi_sr_tfe(SPI0));
	while(spi_sr_busy(SPI0));  // check busy or idle,wait BUSY returns to 0
	temp |= spi_data_read(SPI0);
	temp |= spi_data_read(SPI0)<<8;
	temp |= spi_data_read(SPI0)<<16;
	return temp;
}
void mem_data_check(uint32_t count){
	volatile i = 0;
		printf("OCM write data is :\r\n");
	for(i = 0 ; i < count ; i++){
		printf("MEM1_DATA = %3d",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
		printf("\r\n");
		printf("MEM2_DATA = %3d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
		printf("\r\n");
		if((*(uint32_t*)(MEM_BASE1_ADDR + i*4)) == (*(uint32_t*)(MEM_BASE2_ADDR + i*4)))
				{
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
			printf("%3d",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
		}
		printf("\r\n");
}
void SOC_DMA_AHB_HANDLER(void)
{  
	//dw_dmac_clearIrq(AL9000_DMAC,dw_dmac_channel_num_1,block_1);
}
    int main(void){

    	volatile uint32_t buffer[256];
    		 for (volatile uint32_t i = 0 ; i < 256 ; i++)
    		 {
    			 buffer[i] = i;
    		 	 tx_buf2[i] = i;
    		 }
    		write_To_OCM((uint32_t*)buffer,256,(uint32_t*)MEM_BASE1_ADDR);

    	    /***********************AL9000***************************/
    	//    printf("test start\r\n");
    	    /*variable init*/
    	    uint16_t i = 0;
    	    uint16_t txfifo_start_level = 0;
    	    uint16_t rxdata_num = 0;
    	    uint8_t wrdata_a[256] = {0};
    	    uint8_t rddata_a[256] = {0};
    	    uint32_t wrdata_tmp = 0;
    	    uint32_t rddata_tmp = 0;
    	    uint32_t flash_id = 0;
    	    for(i=0; i<256; i++)
    	    {
    	    	wrdata_a[i] = i;
    	    }

    	    spi_dwc_ssi_disable(SPI0);

    	    	spi_dfs(SPI0,SPI_DFS_BYTE);   // byte
    	    	spi_sste_dis(SPI0);
    	    	spi_x1_mode(SPI0);
    	    	spi_tmod_e2prom(SPI0); // EEPROM read
    	    	spi_txftl_tft(SPI0, 0); // default
    	    	spi_ctrl1_ndf(SPI0, 2); // receive 2+1 data items
    	    	spi_ser(SPI0,SER_SS0_EN); // choose ss1 -- for same to simulation code.
    	    	spi_sckdiv_cfg(SPI0, 0x1E);  // divided by 30.
    	    	spi_dwc_ssi_enable(SPI0);
    	    	flash_id = SReadID();
    	    		printf("flash_id= 0x%08x\r\n",flash_id); // print flash ID
    	    		/*flash ID check*/
    	    		if(flash_id != S25FL512S_ID)
    	    		{
    	    			printf("FAIL: flash ID check not ok!\r\n");
    	    			return -1;
    	    		}
    	    		else
    	    		{
    	    			printf("PASS: flash ID check ok!\r\n");
    	    		}
    	    	spi_dwc_ssi_disable(SPI0);
    	    	spi_dfs(SPI0,SPI_DFS_BYTE);   // byte
    	    	spi_sste_dis(SPI0);
    	    	spi_x1_mode(SPI0);
    	    	spi_tmod_tx(SPI0); // TX only
    	    	spi_txftl_tft(SPI0, 0); // default
    	    	spi_dwc_ssi_enable(SPI0);
    	    	spi_data_transmit(SPI0,CMD_WREN); // tx read id cmd
    	    	while(!spi_sr_tfe(SPI0)); // wait TFE returns to 1
    	    	while(spi_sr_busy(SPI0));  // check busyl or idle,wait BUSY returns to 0
    	    	spi_data_transmit(SPI0,CMD_CHIP_ERASE); // tx chip erase cmd
    	    	spi_data_transmit(SPI0,CMD_SECTOR_ERASE); // tx sector erase cmd
    	    	spi_data_transmit(SPI0,0x00); // tx addr[23:16]
    	    	spi_data_transmit(SPI0,0x00); // tx addr[15:8]
    	    	spi_data_transmit(SPI0,0x00); // tx addr[7:0]
    	    	while(!spi_sr_tfe(SPI0)); // wait TFE returns to 1
    	    	while(spi_sr_busy(SPI0));  // check busy or idle,wait BUSY returns to 0
    	    	spi_dwc_ssi_disable(SPI0);
    	    	spi_dfs(SPI0,SPI_DFS_BYTE);   // byte
    	    	spi_sste_dis(SPI0);
    	    	spi_x1_mode(SPI0);
    	    	spi_tmod_tx(SPI0); // TX only
    	    	spi_txftl_tft(SPI0, 0); // default
    	    	spi_dwc_ssi_enable(SPI0);
    	    	spi_data_transmit(SPI0,CMD_WREN); // tx read id cmd
    	    	while(!spi_sr_tfe(SPI0)); // wait TFE returns to 1
    	    	while(spi_sr_busy(SPI0));  // check busy or idle,wait BUSY returns to 0
    	    	spi_dwc_ssi_disable(SPI0);
    	    	spi_txftl_tft(SPI0, 0); // default
    	    	//spi0_tx_dma_Init(AL9000_DMAC_channel_0,32,3,Dmac_transfer_row1,64);
    	    	spi_dma_Init(AL9000_DMAC_channel_2,spi0,tx,32,0,Dmac_transfer_row1,30);
    	    	spi_dwc_ssi_enable(SPI0);
    	    	spi_data_transmit(SPI0,CMD_PP); // tx Page Program cmd
    	    	spi_data_transmit(SPI0,0x00); // tx addr[23:16]
    	    	spi_data_transmit(SPI0,0x00); // tx addr[15:8]
    	    	spi_data_transmit(SPI0,0x00); // tx addr[7:0]
    	    	while(!spi_sr_tfe(SPI0)); // wait TFE returns to 1
    	    	while(spi_sr_busy(SPI0));  // check busy or idle,wait BUSY returns to 0
    	    	dw_dmac_enable(AL9000_DMAC);
    	    	dw_dmac_enableChannel(AL9000_DMAC,dw_dmac_channel_num_2);
    	    	/*check status--wait busy returns to idle*/
    	    	while(!spi_sr_tfe(SPI0)); // wait TFE returns to 1
    	    	while(spi_sr_busy(SPI0));  // check busy or idle,wait BUSY returns to 0
    	    	while(dw_dmac_checkChannelBusy(AL9000_DMAC));

    	    	dw_dmac_disable(AL9000_DMAC);
    	    	dw_dmac_disableChannel(AL9000_DMAC,dw_dmac_channel_num_2);
    	    	spi_dwc_ssi_disable(SPI0);
    	    	spi_dfs(SPI0,SPI_DFS_BYTE);   // byte
    	    	spi_sste_dis(SPI0);
    	    	spi_x1_mode(SPI0);
    	    	spi_tmod_e2prom(SPI0); // e2prom read
    	    	spi_ctrl1_ndf(SPI0, 0x1D); // receive 30 data items
    	    	spi_dma_Init(AL9000_DMAC_channel_2,spi0,rx,32,0,Dmac_transfer_row1,30);
    	    	spi_dwc_ssi_enable(SPI0);
    	    	spi_data_transmit(SPI0,CMD_SREAD); // tx x1 read cmd
    	    	spi_data_transmit(SPI0,0x00); // tx addr[23:16]
    	    	spi_data_transmit(SPI0,0x00); // tx addr[15:8]
    	    	spi_data_transmit(SPI0,0x00); // tx addr[7:0]
    	    	while(!spi_sr_tfe(SPI0)); // wait TFE returns to 1
    	    	dw_dmac_enable(AL9000_DMAC);
    	    	dw_dmac_enableChannel(AL9000_DMAC,dw_dmac_channel_num_2);
    	    	while(dw_dmac_checkChannelBusy(AL9000_DMAC));
    	        dw_dmac_disable(AL9000_DMAC);
    	        dw_dmac_disableChannel(AL9000_DMAC,dw_dmac_channel_num_2);
    	        while(dw_dmac_checkChannelBusy(AL9000_DMAC));
    	        mem_data_check(30);
    	        printf("spi DMA MODE TEST success");
    	        while(1);
    	        return 0;
}



