
/*
 * main.c
 *
 *  Created on: 2021�?11�?10�?
 *      Author: wei.pang
 */

#include <stdio.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"
#define S25FL512S_ID	0X00200201  // flash ID
#define MXICFLASH_ID    0X003925c2
//#define IO_WRITE
#define FLASH_ERASE_VERIFY
//#define FLASH_READ
/*
 *instruction
 *test_mode		:reload mode
 *importannt parameter	:
 *
 *
 *
*/

#define SPI_MASTER  ((SPI_TypeDef *) SPI1_BASE)
uint32_t SReadID(void)
{
	//-----------------------------------------------------------
	//sent:
	//inst(0x): 9f (RDID)
	//receive: 3 data items
	//-----------------------------------------------------------
	uint32_t temp = 0;
	spi_data_transmit(SPI_MASTER,CMD_RDID); // tx read id cmd
	while(!spi_sr_tfe(SPI_MASTER));
	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0
	temp |= spi_data_read(SPI_MASTER);
	temp |= spi_data_read(SPI_MASTER)<<8;
	temp |= spi_data_read(SPI_MASTER)<<16;
	return temp;
}

uint32_t flash_wait_wip(void)
{
	uint32_t statreg1 = 0;

	//CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_MASTER);
	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame, x1 mode, EEPROM read
	//-----------------------------------------------------------
	/*cfg SPI Data Frame Size.*/
	spi_dfs(SPI_MASTER,SPI_DFS_BYTE);   // byte
	/*Slave Select Toggle disable*/
	spi_sste_dis(SPI_MASTER);
	/*cfg SPI Frame Format*/
	spi_x1_mode(SPI_MASTER);
	/*Transfer Mode.*/
	spi_tmod_e2prom(SPI_MASTER); // e2prom read
	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 1 data items
	//-----------------------------------------------------------
	spi_ctrl1_ndf(SPI_MASTER, 0);
	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 1 data items is present in tx fifo
    //-----------------------------------------------------------
    /*cfg SPI Transmit FIFO Threshold Level*/
	spi_txftl_tft(SPI_MASTER, 0); // default

	/*en ssi*/
	spi_dwc_ssi_enable(SPI_MASTER);
	//-----------------------------------------------------------
	//read flash status register to check if flash operation complete
	//sent:
	//inst(0x): 05 (RDSR)
	//receive: 1 data items
	//-----------------------------------------------------------
	do{
	spi_data_transmit(SPI_MASTER,CMD_RDSR); // tx read status register cmd
	/*check status--wait busy returns to idle*/
	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0
	statreg1 = spi_data_read(SPI_MASTER); // read status register1
	}while(statreg1 & SR1_WIP); // check if flash operation complete
	return statreg1;
}

void flash_reset()
{
	//CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_MASTER);
	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame, x1 mode, TX only
	//-----------------------------------------------------------
	/*cfg QSPI Data Frame Size.*/
	spi_dfs(SPI_MASTER,DFS_BYTE);   // byte
	/*Slave Select Toggle disable*/
//	qspi_sste_dis(QSPI0);
	/*cfg SPI Frame Format*/
	spi_x1_mode(SPI_MASTER);
	/*Transfer Mode.*/
	spi_tmod_tx(SPI_MASTER); // TX only

	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 1 data items is present in tx fifo
    //-----------------------------------------------------------
	spi_txftl_tft(SPI_MASTER, 0); // default
    /*cfg QSPI Transfer start FIFO level.*/
	//spi_txfifo_start_level_set(SPI_MASTER, 1); // default

	/*en ssi*/
	spi_dwc_ssi_enable(SPI_MASTER);

	//-----------------------------------------------------------
	//sent:
	//inst(0x): 06 (WREN)
	//-----------------------------------------------------------
	qspi_data_transmit(SPI_MASTER,0x66); // tx wren cmd
	/*check status--wait busy returns to idle*/
	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0

	spi_data_transmit(SPI_MASTER,0x99); // tx wren cmd
		/*check status--wait busy returns to idle*/
	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0
}
void mem_data_check(uint32_t count){
	volatile i = 0;
		printf("OCM write data is :\r\n");
	for(i = 0 ; i < count ; i++){
		printf("MEM1_DATA = %d",*(uint32_t*)((MEM_BASE1_ADDR+16) + i*4 ));
		printf("\r\n");
		printf("MEM2_DATA = %d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
		printf("\r\n");
		if((*(uint32_t*)(MEM_BASE1_ADDR+16 + i*4 )) == (*(uint32_t*)(MEM_BASE2_ADDR + i*4)))
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
	//AL_dmac_clearIrq(AL_DMAC,AL_dmac_channel_num_1,block_1);
}

void printf_mem_clear_data2(uint32_t count){
	volatile uint32_t i = 0;

	printf("OCM Receive Buffer data is :\r\n");
		for(i = 0 ; i < count ; i++){
			 printf("%d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
			}
		printf("\r\n");
}


void printf_mem_data2(uint32_t count){
	volatile uint32_t i = 0;
	volatile uint32_t data_ok = 1;
	printf("OCMreceive  Buffer data is :\r\n");
		for(i = 0 ; i < count ; i++){
			//printf("%d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
			if (*(uint32_t*)(MEM_BASE2_ADDR + i*4) != (i+1)) {
			    data_ok = 0;
			}
		}

		if (data_ok == 1) {
			for(i = 0 ; i < count ; i++)
	            printf("%d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
			printf("\n\rDMA SPI Test PASS!\n\r");

		}
		else {
			for(i = 0 ; i < count ; i++)
				printf("%d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
			printf("\n\rDMA SPI Test FAILED!\n\r");

		}
		printf("\r\n");
}

/**
 *
 *
 *
 *
 * main()
 *
 *
 *
 * @return
 */

int main(void){

    	volatile uint32_t buffer[128];
    	volatile uint32_t buffer1[128];
    	volatile uint32_t i;
    	uint32_t data32;
    	uint32_t byte_cnt;

 	    uint16_t txfifo_start_level = 0;
 	    uint16_t rxdata_num = 0;
 	    uint8_t wrdata_a[256];
 	    uint8_t rddata_a[256];
 	    uint32_t wrdata_tmp = 0;
 	    uint32_t rddata_tmp = 0;
 	    uint32_t flash_id = 0;
 	    uint32_t erase_ok=0;
    	 //buffer[0] = CMD_PP<<24;
 	   buffer[0] = CMD_PP;
 	   buffer[1] = 0;
 	   buffer[2] = 0;
 	   buffer[3] = 0;
    		 for (i = 4 ; i < 128 ; i++)
    		 {
    			 buffer[i] = i-3;

    		 }
    		 write_To_OCM((uint32_t*)buffer,128,(uint32_t*)MEM_BASE1_ADDR);

    	    /***********************AL***************************/
    	//    printf("test start\r\n");
    	    /*variable init*/

    	    for(i=0; i<256; i++)
    	    {
    	    	wrdata_a[i] = i;
    	    	buffer1[i]  = 0;
    	    }
    	    write_To_OCM((uint32_t*)buffer1,256,(uint32_t*)MEM_BASE2_ADDR);
    	    printf_mem_data(256);
    	    printf_mem_clear_data2(256);
    	    spi_dwc_ssi_disable(SPI_MASTER);

    	    	spi_dfs(SPI_MASTER,SPI_DFS_BYTE);   // byte
    	    	spi_sste_dis(SPI_MASTER);
    	    	spi_x1_mode(SPI_MASTER);
    	    	spi_tmod_e2prom(SPI_MASTER); // EEPROM read
    	    	spi_txftl_tft(SPI_MASTER, 0); // default
    	    	spi_ctrl1_ndf(SPI_MASTER, 2); // receive 2+1 data items
    	    	spi_ser(SPI_MASTER,SER_SS0_EN); // choose ss1 -- for same to simulation code.
    	    	spi_sckdiv_cfg(SPI_MASTER, 0x80);  // divided by 30.
    	    	spi_dwc_ssi_enable(SPI_MASTER);
    	    	flash_id = SReadID();
    	    		printf("flash_id= 0x%08x\r\n",flash_id); // print flash ID
    	    		/*flash ID check*/
    	    		if(flash_id != S25FL512S_ID && flash_id != MXICFLASH_ID)
    	    		{
    	    			printf("FAIL: flash ID check not ok!!\r\n");
    	    			//return -1;
    	    		}
    	    		else
    	    		{
    	    			printf("PASS: flash ID check ok!\r\n");
    	    		}
    	    	spi_dwc_ssi_disable(SPI_MASTER);
    	    	spi_dfs(SPI_MASTER,SPI_DFS_BYTE);   // byte
    	    	spi_sste_dis(SPI_MASTER);
    	    	spi_x1_mode(SPI_MASTER);
    	    	spi_tmod_tx(SPI_MASTER); // TX only
    	    	spi_txftl_tft(SPI_MASTER, 0); // default
    	    	spi_dwc_ssi_enable(SPI_MASTER);
    	    	spi_data_transmit(SPI_MASTER,CMD_WREN); // tx read id cmd
    	    	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
    	    	while(spi_sr_busy(SPI_MASTER));  // check busyl or idle,wait BUSY returns to 0
    	    	//spi_data_transmit(SPI_MASTER,CMD_CHIP_ERASE); // tx chip erase cmd

    	    	spi_data_transmit(SPI_MASTER,0x20); // tx sector erase cmd
    	    	spi_data_transmit(SPI_MASTER,0x00); // tx addr[23:16]
    	    	spi_data_transmit(SPI_MASTER,0x00); // tx addr[15:8]
    	    	spi_data_transmit(SPI_MASTER,0x00); // tx addr[7:0]
    	    	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
    	    	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0

    	    	//delay_1ms(1);

#ifdef FLASH_ERASE_VERIFY
	/*****cfg reg for read******/
	//CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_MASTER);
	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame, x1 mode, EEPROM read
	//-----------------------------------------------------------
	/*cfg SPI Data Frame Size.*/
	spi_dfs(SPI_MASTER,SPI_DFS_BYTE);   // byte
	/*Slave Select Toggle disable*/
	spi_sste_dis(SPI_MASTER);
	/*cfg SPI Frame Format*/
	spi_x1_mode(SPI_MASTER);
	/*Transfer Mode.*/
	spi_tmod_e2prom(SPI_MASTER); // e2prom read
	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 256 data items
	//-----------------------------------------------------------
	spi_ctrl1_ndf(SPI_MASTER, 0x7f); // receive 256 data items--FIFO MAX level=64
//	spi_ctrl1_ndf(SPI_MASTER, 0x1D); // receive 30 data items

	/*en ssi*/
	spi_dwc_ssi_enable(SPI_MASTER);
	//printf(" 1111\n");
	//-----------------------------------------------------------
	//read flash
	//sent:
	//inst(0x): 03 (X1 READ)
	//addr(0x): 00 00 00
	//receive(0x): 256 data items
	//-----------------------------------------------------------
	spi_data_transmit(SPI_MASTER,CMD_SREAD); // tx x1 read cmd
	spi_data_transmit(SPI_MASTER,0x00); // tx addr[23:16]
	spi_data_transmit(SPI_MASTER,0x00); // tx addr[15:8]
	spi_data_transmit(SPI_MASTER,0x00); // tx addr[7:0]
	/*check status--wait busy returns to idle*/
	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
	//printf(" 2222\n");
	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0
	// read 256 bytes
	i=0;   // must init i=0; in case of RXFIFO still empty;
	do{
		if(spi_sr_rfne(SPI_MASTER))  // RXFIFO not empty
		{
			rddata_a[i]= spi_data_read(SPI_MASTER);

			//printf("fail data [%d] is: %d\r\n",i,rddata_a[i]);
			i++;
		}
	}while(i<128);
	for(i=0; i<128; i++)
		{
		   printf("%d",rddata_a[i]); // print write data
		   if (0xff != rddata_a[i]) {
			   erase_ok = 1;
		   }
		}
  if (erase_ok == 1)
	printf("\n\rPASS: spi flash erase failed!\r\n");
  else {
	printf("\n\rPASS: spi flash erase success!\r\n");
  }
#endif


    	    	spi_dwc_ssi_disable(SPI_MASTER);
    	    	spi_dfs(SPI_MASTER,SPI_DFS_BYTE);   // byte
    	    	spi_sste_dis(SPI_MASTER);
    	    	spi_x1_mode(SPI_MASTER);
    	    	spi_tmod_tx(SPI_MASTER); // TX only
    	    	spi_txftl_tft(SPI_MASTER, 0); // default
    	    	spi_dwc_ssi_enable(SPI_MASTER);
    	    	spi_data_transmit(SPI_MASTER,CMD_WREN); // tx read id cmd
    	    	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
    	    	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0
    	    	spi_dwc_ssi_disable(SPI_MASTER);
    	    	spi_txftl_tft(SPI_MASTER, 0); // default

#ifdef IO_WRITE
    	    	/***************************************************************/
    	    		/************************flash write ***************************/
    	    		/***************************************************************/
    	    		//CFG SSIENR
    	    		/*dis ssi*/
    	    		spi_dwc_ssi_disable(SPI_MASTER);
    	    		//-----------------------------------------------------------
    	    		//program CTRLR0 register
    	    		//8-bit data frame, x1 mode, TX only
    	    		//-----------------------------------------------------------
    	    		/*cfg SPI Data Frame Size.*/
    	    		spi_dfs(SPI_MASTER,SPI_DFS_BYTE);   // byte

    	    		/*Slave Select Toggle disable*/
    	    		spi_sste_dis(SPI_MASTER);
    	    		/*cfg SPI Frame Format*/
    	    		spi_x1_mode(SPI_MASTER);
    	    		/*Transfer Mode.*/
    	    		spi_tmod_tx(SPI_MASTER); // TX only

    	    		//-----------------------------------------------------------
    	    	    //program TXFTLR register
    	    	    //start when 1 data items is present in tx fifo
    	    	    //-----------------------------------------------------------
    	    		spi_txftl_tft(SPI_MASTER, 0); // default
    	    		/*en ssi*/
    	    		spi_dwc_ssi_enable(SPI_MASTER);

    	    		//-----------------------------------------------------------
    	    		//sent:
    	    		//inst(0x): 06 (WREN)
    	    		//-----------------------------------------------------------

    	    		spi_data_transmit(SPI_MASTER,CMD_WREN); // tx write en cmd

    	    		while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
    	    		while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0

    	    		//CFG SSIENR

    	    		spi_dwc_ssi_disable(SPI_MASTER);
    	    		//-----------------------------------------------------------
    	    	    //program TXFTLR register
    	    	    //-----------------------------------------------------------

    	    		spi_txftl_tft(SPI_MASTER, 0); // default
    	    		//spi_dfs(SPI_MASTER,SPI_DFS_WORD);   // dword

    	    		/*en ssi*/

    	    		spi_dwc_ssi_enable(SPI_MASTER);
    	    		//-----------------------------------------------------------
    	    		//write flash: 1 inst + 3 addr + 256 datas
    	    		//sent:
    	    		//inst(0x): 02 (PP)
    	    		//addr(0x): 00 00 00
    	    		//data(0x): 256 bytes
    	    		//-----------------------------------------------------------

    	    		spi_data_transmit(SPI_MASTER,CMD_PP); // tx Page Program cmd
    	    		spi_data_transmit(SPI_MASTER,0x00); // tx addr[23:16]
    	    		spi_data_transmit(SPI_MASTER,0x00); // tx addr[15:8]
    	    	    spi_data_transmit(SPI_MASTER,0x00); // tx addr[7:0]
    	    		/*
    	    		spi_data_transmit(SPI_MASTER,wrdata_a[0]);
    	    		spi_data_transmit(SPI_MASTER,wrdata_a[1]);
    	    		spi_data_transmit(SPI_MASTER,wrdata_a[2]);
    	    		spi_data_transmit(SPI_MASTER,wrdata_a[3]);
    	    		spi_data_transmit(SPI_MASTER,wrdata_a[4]);
    	    		spi_data_transmit(SPI_MASTER,wrdata_a[5]);
    	    		*/


    	    		byte_cnt=128;
    	    			i = 8;
    	    			while (byte_cnt)
    	    			{
    	    				if ( (*(volatile uint32_t*)(SPI1_BASE + 0x28) & 0x2) == 0x2) //tx fifo not full
    	    				{

    	    				   spi_data_transmit(SPI_MASTER,0x0); // tx data wrdata_a[i]
    	    				   //spi_data_transmit(SPI_MASTER,data32);
    	    				   //byte_cnt=byte_cnt -4;
    	    				   byte_cnt--;
    	    				  // i=i+4;
    	    				   i++;
    	    				}
    	    			}

    	    		while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
    	    		while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0
    	    		for(i=0; i<128; i++)
    	    		{
    	    			printf("%d",wrdata_a[i]); // print write data
    	    		}
    	    		printf(" \n");
    	    		printf("flash write finish. \n");

    	    		flash_wait_wip();
#endif

#if 1
    	    	AlSpi_DmaInit(AL_DMAC_channel_2,spi1,tx,1,0,Dmac_transfer_row1,64);
    	    	//AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_2);
    	    	spi_dwc_ssi_enable(SPI_MASTER);
    	    	AlDma_Enable(AL_DMAC);
    	    	AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_2);
    	    	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
    	    	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0

    	    	/*check status--wait busy returns to idle*/

    	    	while(AlDma_CheckChannelBusy(AL_DMAC));

    	    	AlDma_Disable(AL_DMAC);
    	    	AlDma_DisableChannel(AL_DMAC,AL_dmac_channel_num_2);
    	    	flash_wait_wip();
#endif
#if 1
    	    	spi_dwc_ssi_disable(SPI_MASTER);
    	    	spi_dfs(SPI_MASTER,SPI_DFS_BYTE);   // byte
    	    	spi_sste_dis(SPI_MASTER);
    	    	spi_x1_mode(SPI_MASTER);
    	    	spi_tmod_e2prom(SPI_MASTER); // e2prom read
    	    	spi_ctrl1_ndf(SPI_MASTER,59); // receive 30 data items
    	    	AlSpi_DmaInit(AL_DMAC_channel_0,spi1,rx,0,59,Dmac_transfer_row1,60);
    	    	spi_dwc_ssi_enable(SPI_MASTER);
    	    	spi_data_transmit(SPI_MASTER,0x03); // tx x1 read cmd
    	    	spi_data_transmit(SPI_MASTER,0x00); // tx addr[23:16]
    	    	spi_data_transmit(SPI_MASTER,0x00); // tx addr[15:8]
    	    	spi_data_transmit(SPI_MASTER,0x00); // tx addr[7:0]
    	    	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
    	    	AlDma_Enable(AL_DMAC);
    	    	AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_0);
    	    	while(AlDma_CheckChannelBusy(AL_DMAC));
    	        AlDma_Disable(AL_DMAC);
    	        AlDma_DisableChannel(AL_DMAC,AL_dmac_channel_num_0);
    	       // mem_data_check(60);
    	       // printf("spi DMA MODE TEST success");
#endif

#ifdef FLASH_READ
	/*****cfg reg for read******/
	//CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_MASTER);
	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame, x1 mode, EEPROM read
	//-----------------------------------------------------------
	/*cfg SPI Data Frame Size.*/
	spi_dfs(SPI_MASTER,SPI_DFS_BYTE);   // byte
	/*Slave Select Toggle disable*/
	spi_sste_dis(SPI_MASTER);
	/*cfg SPI Frame Format*/
	spi_x1_mode(SPI_MASTER);
	/*Transfer Mode.*/
	spi_tmod_e2prom(SPI_MASTER); // e2prom read
	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 256 data items
	//-----------------------------------------------------------
	spi_ctrl1_ndf(SPI_MASTER, 0x7f); // receive 256 data items--FIFO MAX level=64
//	spi_ctrl1_ndf(SPI_MASTER, 0x1D); // receive 30 data items

	/*en ssi*/
	spi_dwc_ssi_enable(SPI_MASTER);
	//printf(" 1111\n");
	//-----------------------------------------------------------
	//read flash
	//sent:
	//inst(0x): 03 (X1 READ)
	//addr(0x): 00 00 00
	//receive(0x): 256 data items
	//-----------------------------------------------------------
	spi_data_transmit(SPI_MASTER,CMD_SREAD); // tx x1 read cmd
	spi_data_transmit(SPI_MASTER,0x00); // tx addr[23:16]
	spi_data_transmit(SPI_MASTER,0x00); // tx addr[15:8]
	spi_data_transmit(SPI_MASTER,0x00); // tx addr[7:0]
	/*check status--wait busy returns to idle*/
	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
	//printf(" 2222\n");
	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0
	// read 256 bytes
	i=0;   // must init i=0; in case of RXFIFO still empty;
	do{
		if(spi_sr_rfne(SPI_MASTER))  // RXFIFO not empty
		{
			rddata_a[i]= spi_data_read(SPI_MASTER);

			//printf("fail data [%d] is: %d\r\n",i,rddata_a[i]);
			i++;
		}
	}while(i<128);
	//printf(" 3333\n");
	for(i=0; i<128; i++)
	{
		printf("%d",rddata_a[i]); // print write data
	}
	printf("\n");
	printf("flash read finish. \n");

	//----------- data check----------
	// 256 bytes
	for(i=0; i<128; i++)
	{
		if(wrdata_a[i] != rddata_a[i])
		{
			printf("FAIL: datas are not same!\r\n");

			printf("fail data [%d] is: %d\r\n",i,rddata_a[i]);
			//return -1;
		}
	}
	printf("PASS: spi x1 write / read test success!\r\n");
#endif

	printf_mem_data2(60);
	while(1);
    	        return 0;
}



