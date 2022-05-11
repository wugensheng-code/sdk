
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
#include "al9000_spi.h"
#include "al9000_qspi.h"
#include <time.h>
/*
 *instruction
 *test_mode		:reload mode
 *importannt parameter	:
 *
 *
 *
*/
#define S25FL256S_ID	0x00190201  // flash ID


/****************function define******************************/
/*
* Read Manufacture ID/ Device ID - in x1-QSPI mode
*/
uint32_t SReadID(void)
{
	//-----------------------------------------------------------
	//sent:
	//inst(0x): 9f (RDID)
	//receive: 3 data items
	//-----------------------------------------------------------
	uint32_t temp = 0;
	qspi_data_transmit(QSPI0,CMD_RDID); // tx read id cmd
	while(!qspi_sr_tfe(QSPI0));
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	temp |= qspi_data_read(QSPI0);
	temp |= qspi_data_read(QSPI0)<<8;
	temp |= qspi_data_read(QSPI0)<<16;
	return temp;
}

/*
 * send flash WREN instr
 */
void flash_write_enable()
{
	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame, x1 mode, TX only
	//-----------------------------------------------------------
	/*cfg QSPI Data Frame Size.*/
	qspi_dfs(QSPI0,DFS_BYTE);   // byte
	/*Slave Select Toggle disable*/
//	qspi_sste_dis(QSPI0);
	/*cfg SPI Frame Format*/
	qspi_x1_mode(QSPI0);
	/*Transfer Mode.*/
	qspi_tmod_tx(QSPI0); // TX only

	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 1 data items is present in tx fifo
    //-----------------------------------------------------------
	qspi_txftl_tft(QSPI0, 0); // default
    /*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, 0); // default

	/*en ssi*/
	dwc_ssi_enable(QSPI0);

	//-----------------------------------------------------------
	//sent:
	//inst(0x): 06 (WREN)
	//-----------------------------------------------------------
	qspi_data_transmit(QSPI0,CMD_WREN); // tx wren cmd
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
}
/******************** check flash status(WIP)*********************/
/*
* check flash status of WIP-write in progress
* 8-bit data frame, x1 mode, EEPROM read
* 	//inst(0x): 05 (RDSR)
*	//receive: 1 data items
*/
uint32_t flash_wait_wip(void)
{
	uint32_t statreg1 = 0;
	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame, x1 mode, EEPROM read
	//-----------------------------------------------------------
	/*cfg QSPI Data Frame Size.*/
	qspi_dfs(QSPI0,DFS_BYTE);   // byte
	/*Slave Select Toggle disable*/
//	qspi_sste_dis(QSPI0);
	/*cfg SPI Frame Format*/
	qspi_x1_mode(QSPI0);
	/*Transfer Mode.*/
	qspi_tmod_e2prom(QSPI0); // e2prom read
	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 1 data items
	//-----------------------------------------------------------
	qspi_ctrl1_ndf(QSPI0, 0);
	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 1 data items is present in tx fifo
    //-----------------------------------------------------------
    /*cfg QSPI Transmit FIFO Threshold Level*/
	qspi_txftl_tft(QSPI0, 0); // default
    /*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, 0); // default

	/*en ssi*/
	dwc_ssi_enable(QSPI0);
	//-----------------------------------------------------------
	//read flash status register to check if flash operation complete
	//sent:
	//inst(0x): 05 (RDSR)
	//receive: 1 data items
	//-----------------------------------------------------------
	do{
	qspi_data_transmit(QSPI0,CMD_RDSR); // tx read status register cmd
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	statreg1 = qspi_data_read(QSPI0); // read status register1
	}while(statreg1 & SR1_WIP); // check if flash operation complete
	return statreg1;
}
enum TDS{
	source,
	destation
};

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
#if 0
	else{
		printf("OCM read data is :\r\n");
		for(i = 0 ; i < count ; i++){
				printf("%3d",*(uint32_t*)(pSnAddr + i));
			}
	}
	printf("\r\n");
	for(i = 0 ; i < count ; i++){
		if((*(uint32_t*)(MEM_BASE1_ADDR + i*4)) == (*(uint32_t*)(pSnAddr + i)))
		{
			printf("check data is right\r\n");
		}
		else{
			printf("check data is fail please check you dma set data\r\n");
		}
		}
#endif

//}
void SOC_DMA_AHB_HANDLER(void)
{
#if 0
    //printf("enter_irq_number%d\n\r",count);        
      count ++;
     if(count == 10){
       dw_dmac_setSingle(AL_DMAC_channel_0,AlDma_Channel_num_1,Dmac_src_dst);
       AlDma_ClearIrq(AL_DMAC,dw_dmac_channel_num_1,tfr_l);
       AlDma_Enable(AL_DMAC);
       //count = 0;
       //printf("complete_DMAC_transfer\n\r");    
    }
    else{
           AlDma_ClearIrq(AL_DMAC,dw_dmac_channel_num_1,block_1);
         }  
#endif
}
int main(void){
	 volatile uint32_t buffer[256];
	 for (volatile uint32_t i = 0 ; i < 256 ; i++)
	 {
		 buffer[i] = i;
	 }
	write_To_OCM((uint32_t*)buffer,256,(uint32_t*)MEM_BASE1_ADDR);
	printf_mem_data(256);
    /***********************AL***************************/
   printf("test start\r\n");
    /*variable init*/
    uint16_t i = 0;
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
	dwc_ssi_disable(QSPI0);

	qspi_dfs(QSPI0,DFS_BYTE);   // byte
	qspi_x1_mode(QSPI0);
	qspi_tmod_e2prom(QSPI0); // EEPROM read
	qspi_ctrl1_ndf(QSPI0, 2); // receive 2+1 data items
	qspi_ser(QSPI0,SER_SS0_EN); // choose ss0
	qspi_sckdiv_cfg(QSPI0, 0x2);  // divided by 4. (2<<1)
	dwc_ssi_enable(QSPI0);
	flash_id = SReadID();
	printf("flash_id= 0x%08x\r\n",flash_id); // print flash ID
		/*flash ID check*/
		if(flash_id != S25FL256S_ID)
		{
			printf("FAIL: flash ID check not ok!\r\n");
			return -1;
		}
		else
		{
			printf("PASS: flash ID check ok!\r\n");
		}
	printf("flash erase start...\r\n");
	/*erase flash*/
	flash_write_enable();
	qspi_data_transmit(QSPI0,CMD_CHIP_ERASE); // tx sector erase cmd
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	flash_wait_wip();
	printf("flash erase finish.\r\n");
	//send WREN
#if 0
	flash_write_enable();
	dwc_ssi_disable(QSPI0);
	qspi_dfs(QSPI0,DFS_BYTE);   // Byte
	qspi_txftl_tft(QSPI0, 0); // default
	qspi_txfifo_start_level_set(QSPI0, 0x21); // 30+1+3 start data
	AlQspi_DmaInit(AL_DMAC_channel_0,tx,40,10,Dmac_transfer_row1,64);
	dwc_ssi_enable(QSPI0);
	qspi_data_transmit(QSPI0,CMD_PP); // tx Page Program cmd
	qspi_data_transmit(QSPI0,0x00); // tx addr[23:16]
	qspi_data_transmit(QSPI0,0x00); // tx addr[15:8]
	qspi_data_transmit(QSPI0,0x00); // tx addr[7:0]
    AlDma_Enable(AL_DMAC);
    AlDma_EnableChannel(AL_DMAC,AlDma_Channel_num_0);
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	printf("DMA start transimate data\r\n");
	while(AlDma_CheckChannelBusy(AL_DMAC));
	printf("DMA QSPI X1 transimate data has been completed\r\n");
	printf_mem_data(30);
#endif
#if 0
	/*FLASH_X1_DFS_32*///word write mode
	flash_write_enable();
	dwc_ssi_disable(QSPI0);
    AlDma_Disable(AL_DMAC);
    AlDma_DisableChannel(AL_DMAC,dw_dmac_channel_num_0);
	qspi_dfs(QSPI0,DFS_WORD);   // Word
	txfifo_start_level = 0x41; // 65W=260B
	qspi_txfifo_start_level_set(QSPI0, txfifo_start_level-1); // 65W=260B
	AlQspi_DmaInit(AL_DMAC_channel_0,tx,64,10,Dmac_transfer_row1,64);
	dwc_ssi_enable(QSPI0);
	qspi_data_transmit(QSPI0,0x00000000|CMD_PP); // addr[7:0]+addr[15:8]+addr[23:16]+inst
	//64 Words
    AlDma_Enable(AL_DMAC);
    AlDma_EnableChannel(AL_DMAC,dw_dmac_channel_num_0);
    printf("DMA start transimate data\r\n");
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	while(AlDma_CheckChannelBusy(AL_DMAC));
	printf("DMA QSPI X1_DFS32 transimate data has been completed\r\n");
	printf_mem_data(64);
#endif
#if 0
/*FLASH_X1_DFS_8         read data*/

	/*cfg reg for read*/
	flash_wait_wip();
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
    AlDma_Disable(AL_DMAC);
    AlDma_DisableChannel(AL_DMAC,dw_dmac_channel_num_0);
	qspi_dfs(QSPI0,DFS_BYTE);   // byte
	qspi_x1_mode(QSPI0);
	/*Transfer Mode.*/
	qspi_tmod_e2prom(QSPI0); // e2prom read
	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 256 data items
	//-----------------------------------------------------------
	qspi_ctrl1_ndf(QSPI0, 0xFF); // receive 256=255+1 data items

    /*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, 4-1); // start data tx at 4th data
	AlQspi_DmaInit(AL_DMAC_channel_1,rx,32,10,Dmac_transfer_row1,32);
	/*en ssi*/
	dwc_ssi_enable(QSPI0);
	qspi_data_transmit(QSPI0,CMD_SREAD); // tx x1 read cmd
	qspi_data_transmit(QSPI0,0x00); // tx addr[23:16]
	qspi_data_transmit(QSPI0,0x00); // tx addr[15:8]
	qspi_data_transmit(QSPI0,0x00); // tx addr[7:0]
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
//	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	// read 256 bytes
	printf("DMA start receive data\r\n");
	AlDma_Enable(AL_DMAC);
    AlDma_EnableChannel(AL_DMAC,dw_dmac_channel_num_1);
	while(AlDma_CheckChannelBusy(AL_DMAC));
	printf("DMA QSPI X1_DFS8 receive data has been completed\r\n");
	mem_data_check(30);
#endif
#if 0
/*FLASH_X1_DFS_32      read data*/
	dwc_ssi_disable(QSPI0);
	AlDma_Disable(AL_DMAC);
    AlDma_DisableChannel(AL_DMAC,dw_dmac_channel_num_0);

	//-----------------------------------------------------------
	//program CTRLR0 register
	//32-bit data frame, x1 mode, EEPROM read
	//-----------------------------------------------------------
	/*cfg QSPI Data Frame Size.*/
	qspi_dfs(QSPI0,DFS_WORD);   // word
	/*Slave Select Toggle disable*/
//	qspi_sste_dis(QSPI0);
	/*cfg SPI Frame Format*/
	qspi_x1_mode(QSPI0);
	/*Transfer Mode.*/
	qspi_tmod_e2prom(QSPI0); // e2prom read
	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 256-bytes data items
	//-----------------------------------------------------------
	rxdata_num = 0x40; // 64W=256B
	qspi_ctrl1_ndf(QSPI0, rxdata_num-1); // 64W=256B

	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 1 data items is present in tx fifo: 1 data=1 inst + 3 addr
    //-----------------------------------------------------------
    /*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, 1-1); // start data tx after transmit 1 data
	AlQspi_DmaInit(AL_DMAC_channel_0,rx,64,0,Dmac_transfer_row1,64);
	/*en ssi*/
	dwc_ssi_enable(QSPI0);

	//-----------------------------------------------------------
	//read flash
	//sent:
	//inst(0x): 03 (X1 READ)
	//addr(0x): 02 01 00
	//receive(0x): 256 data items
	//-----------------------------------------------------------
	//1 word
	qspi_data_transmit(QSPI0,0x00000000|CMD_SREAD); // addr[7:0]+addr[15:8]+addr[23:16]+inst
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	AlDma_Enable(AL_DMAC);
    AlDma_EnableChannel(AL_DMAC,dw_dmac_channel_num_0);
    while(AlDma_CheckChannelBusy(AL_DMAC));
    printf("DMA start receive data\r\n");
	AlDma_Disable(AL_DMAC);
    AlDma_DisableChannel(AL_DMAC,dw_dmac_channel_num_0);
    printf("DMA QSPI X1_DFS32 receive data has been completed\r\n");
    mem_data_check(64);
#endif
//	printf("PASS: qspi x1 write / read test success!\r\n");
//
//	FLASH_X2  // this type flash not support X2 Page Program

	/*****************flash read(X2) ****************/
	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
	AlDma_Disable(AL_DMAC);
    AlDma_DisableChannel(AL_DMAC,AL_dmac_channel_num_0);
#if 0
	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame, x2 mode, RX only
	//-----------------------------------------------------------
	/*cfg QSPI Data Frame Size.*/
	qspi_dfs(QSPI0,DFS_WORD);   // word
	/*Slave Select Toggle disable*/
//	qspi_sste_dis(QSPI0);
	/*cfg SPI Frame Format*/
	qspi_x2_mode(QSPI0);
	/*Transfer Mode.*/
	qspi_tmod_rx(QSPI0); // RX only

	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 256-bytes data items
	//-----------------------------------------------------------
	rxdata_num = 0x40; // 64W=256B
	qspi_ctrl1_ndf(QSPI0, rxdata_num-1); // 64W=256B
	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 2 data items is present in tx fifo: 1 inst + 1 addr
    //-----------------------------------------------------------
    /*cfg QSPI Transmit FIFO Threshold Level*/
	qspi_txftl_tft(QSPI0, 0); // default
    /*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, 1); // tx need transmit 2 data-fram
	//-----------------------------------------------------------
	//program SER register
	//choose ss_0_n
	//-----------------------------------------------------------
	qspi_ser(QSPI0,SER_SS0_EN); // choose ss0

	//-----------------------------------------------------------
	//program BAUDR register
	//divided by 4
	//-----------------------------------------------------------
	/*cfg QSPI Clock Divider.*/
	qspi_sckdiv_cfg(QSPI0, 0x2);  // divided by 4. (2<<1)
	//-----------------------------------------------------------
    //program SPI_CTRLR0 register
    //8-bit inst, 24-bit addr, both inst and addr sent by x1 mode
	//8 dummy cycles, no xip
    //-----------------------------------------------------------
	qspi_spictrlr0_trans_type(QSPI0, TRANS_TYPE_TT0);
	qspi_spictrlr0_addr_l(QSPI0, 0x06); // 24-bit addr
	qspi_spictrlr0_xip_mod(QSPI0, XIP_MD_DIS); // disable xip mode
	qspi_spictrlr0_inst_l(QSPI0, 0x02); //8-bit inst
	qspi_spictrlr0_wait_cycles(QSPI0, 0x8); //8 dummy cycles
	qspi_spictrlr0_xip_dfs_hc(QSPI0, XIP_DFS_HC_DIS);//xip data-frame-size not hardcode
	qspi_spictrlr0_xip_inst_en(QSPI0, XIP_INST_DISABLE); // xip inst phase disable
	qspi_spictrlr0_xip_cont_xfer_en(QSPI0, XIP_CONT_XFER_DIS);//disable continuous transfer in XIP mode
	qspi_spictrlr0_xip_mbl(QSPI0, XIP_MBL_4); //xip4-bit mode bit length
	qspi_spictrlr0_xip_prefetch_en(QSPI0, XIP_PREFETCH_DISABLE); // xip prefetch disable
	qspi_spictrlr0_clk_stretch_en(QSPI0, CLK_STRETCH_DISABLE); // clk stretch dis

	//-----------------------------------------------------------
	//program SSIENR register
	//enable qspi
	//-----------------------------------------------------------
	/*en ssi*/
	AlQspi_DmaInit(AL_DMAC_channel_0,rx,32,10,Dmac_transfer_row1,32);
	dwc_ssi_enable(QSPI0);

	//-----------------------------------------------------------
	//read flash - X2
	//sent:
	//inst(0x): 3b (X2 READ)
	//addr(0x): 000000
	//receive(0x): 256 data items
	//-----------------------------------------------------------
//	qspi_data_transmit(QSPI0,CMD_DREAD); // x2 read cmd
//	qspi_data_transmit(QSPI0,0x000000); // tx addr[23:0]
	//2 word
	qspi_data_transmit(QSPI0,CMD_DREAD<<24); // inst+24'b0
	qspi_data_transmit(QSPI0,0x00000000); // addr[7:0]+addr[15:8]+addr[23:16]+8'b0
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	AlDma_Enable(AL_DMAC);
    AlDma_EnableChannel(AL_DMAC,dw_dmac_channel_num_0);

    printf("DMA X2_MODE start receive data\r\n");
	//send WREN
	flash_write_enable();




	//CFG SSIENR
	//
	//
	//	/************************ flash X4 ****************************/

	/*dis ssi*/
	while(dw_dmac_checkChannelBusy(AL_DMAC));
	 printf("DMA QSPI X2_mode receive data has been completed\r\n");
	dwc_ssi_disable(QSPI0);
	AlDma_Disable(AL_DMAC);
    AlDma_DisableChannel(AL_DMAC,dw_dmac_channel_num_0);
    mem_data_check(64);
#endif
#if 1
	/*cfg QSPI Data Frame Size.*/
	qspi_dfs(QSPI0,DFS_BYTE);   // byte
	//-----------------------------------------------------------
	//program TXFTLR register
	//start when 3 data items is present in tx fifo: 1 inst + 2 datas
	//-----------------------------------------------------------
	/*cfg QSPI Transmit FIFO Threshold Level*/
	qspi_txftl_tft(QSPI0, 0); // default
	/*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, 0x2); // 3 start data
	/*en ssi*/
	dwc_ssi_enable(QSPI0);
	//-----------------------------------------------------------
	//sent:
	//inst(0x): 01 (WRR)
	//data(0x): 02 02(quad=1,enable)
	//-----------------------------------------------------------
	qspi_data_transmit(QSPI0,CMD_WRR); // write register cmd
	qspi_data_transmit(QSPI0,0x02); // write status register
	qspi_data_transmit(QSPI0,0x02); // QE,write configuration register-quad=1
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	/******************** check flash status(WIP)*********************/
	flash_wait_wip();
	/******************** flash write enable*********************/
//	printf("flash write enable\r\n");
	//send WREN
	flash_write_enable();
	/***************************************************************/
	/************X4 mode write (QPP:0x32h) *********/
//	printf("flash X4-mode write start...\r\n");
	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame, x4 mode, RX only
	//-----------------------------------------------------------
	/*cfg QSPI Data Frame Size.*/
	qspi_dfs(QSPI0,DFS_WORD);   // word
	/*Slave Select Toggle disable*/
//	qspi_sste_dis(QSPI0);
	/*cfg SPI Frame Format*/
	qspi_x4_mode(QSPI0);
	/*Transfer Mode.*/
	qspi_tmod_tx(QSPI0); // TX only

	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 256-bytes data items
	// stretching=en & tmod=tx, nCS invalid after tx 0xFF data items.
	//-----------------------------------------------------------
	rxdata_num = 0x40; // 64W=256B
	qspi_ctrl1_ndf(QSPI0, rxdata_num-1); // 64W=256B
	//-----------------------------------------------------------
    //program TXFTLR register
    //-----------------------------------------------------------
	/*cfg QSPI Transfer start FIFO level.*/
	// when stretch=en, strat level >= inst+addr+1 data
	qspi_txfifo_start_level_set(QSPI0, 3-1); // tx need tramsmit 3 data

	//-----------------------------------------------------------
	//program SPI_CTRLR0 register
	//8-bit inst, 24-bit addr, both inst and addr sent by x1 mode
	//8 dummy cycles, no xip
	//-----------------------------------------------------------
	qspi_spictrlr0_trans_type(QSPI0, TRANS_TYPE_TT0);
	qspi_spictrlr0_addr_l(QSPI0, 0x06); // 24-bit addr
	qspi_spictrlr0_xip_mod(QSPI0, XIP_MD_DIS); // disable xip mode
	qspi_spictrlr0_inst_l(QSPI0, 0x02); //8-bit inst
	qspi_spictrlr0_wait_cycles(QSPI0, 0x0); //0 dummy cycles
	qspi_spictrlr0_xip_dfs_hc(QSPI0, XIP_DFS_HC_DIS);//xip data-frame-size not hardcode
	qspi_spictrlr0_xip_inst_en(QSPI0, XIP_INST_DISABLE); // xip inst phase disable
	qspi_spictrlr0_xip_cont_xfer_en(QSPI0, XIP_CONT_XFER_DIS);//disable continuous transfer in XIP mode
	qspi_spictrlr0_xip_mbl(QSPI0, XIP_MBL_4); //xip 4-bit mode bit length
	qspi_spictrlr0_xip_prefetch_en(QSPI0, XIP_PREFETCH_DISABLE); // xip prefetch disable
	qspi_spictrlr0_clk_stretch_en(QSPI0, CLK_STRETCH_ENABLE); // clk stretch en
	/*en ssi*/
	AlQspi_DmaInit(AL_DMAC_channel_0,tx,32,10,Dmac_transfer_row1,64);
	dwc_ssi_enable(QSPI0);

	//2 word
	qspi_data_transmit(QSPI0,CMD_QPP<<24); // inst+24'b0
	qspi_data_transmit(QSPI0,0x00000000); // addr[7:0]+addr[15:8]+addr[23:16]+8'b0
	//64 Words-write 256 bytes
	AlDma_Enable(AL_DMAC);
    AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_0);
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	printf_mem_data(64);
	printf("DMA start transimate data\r\n");
	while(AlDma_CheckChannelBusy(AL_DMAC));
	//CFG SSIENR
	/*dis ssi*/
	 printf("DMA QSPI X4_MODE transimate data has been completed\r\n");

	dwc_ssi_disable(QSPI0);
	AlDma_Disable(AL_DMAC);
    AlDma_DisableChannel(AL_DMAC,AL_dmac_channel_num_0);

	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame, x4 mode, RX only
	//-----------------------------------------------------------
	/*cfg QSPI Data Frame Size.*/
	qspi_dfs(QSPI0,DFS_WORD);   // byte
	/*Slave Select Toggle disable*/
//	qspi_sste_dis(QSPI0);
	/*cfg SPI Frame Format*/
	qspi_x4_mode(QSPI0);
	/*Transfer Mode.*/
	qspi_tmod_rx(QSPI0); // RX only

	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 256-bytes data items
	// stretching=en & tmod=tx, nCS invalid after tx 0xFF data items.
	//-----------------------------------------------------------
	rxdata_num = 0x40; // 64W=256B
	qspi_ctrl1_ndf(QSPI0, rxdata_num-1); // 64W=256B
	//-----------------------------------------------------------
	//program TXFTLR register
	//start when 2 data items is present in tx fifo: 1 inst + 1 addr
	//-----------------------------------------------------------
//	/*cfg QSPI Transmit FIFO Threshold Level*/
//	qspi_txftl_tft(QSPI0, 0); // default
	/*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, 1); // tx need transmit 2 data-fram

	//-----------------------------------------------------------
	//program SPI_CTRLR0 register
	//8-bit inst, 24-bit addr, both inst and addr sent by x1 mode
	//8 dummy cycles, no xip
	//-----------------------------------------------------------
	qspi_spictrlr0_trans_type(QSPI0, TRANS_TYPE_TT0);
	qspi_spictrlr0_addr_l(QSPI0, 0x06); // 24-bit addr
	qspi_spictrlr0_xip_mod(QSPI0, XIP_MD_DIS); // disable xip mode
	qspi_spictrlr0_inst_l(QSPI0, 0x02); //8-bit inst
	qspi_spictrlr0_wait_cycles(QSPI0, 0x8); //8 dummy cycles(1 byte)
	qspi_spictrlr0_xip_dfs_hc(QSPI0, XIP_DFS_HC_DIS);//xip data-frame-size not hardcode
	qspi_spictrlr0_xip_inst_en(QSPI0, XIP_INST_DISABLE); // xip inst phase disable
	qspi_spictrlr0_xip_cont_xfer_en(QSPI0, XIP_CONT_XFER_DIS);//disable continuous transfer in XIP mode
	qspi_spictrlr0_xip_mbl(QSPI0, XIP_MBL_4); //xip 4-bit mode bit length
	qspi_spictrlr0_xip_prefetch_en(QSPI0, XIP_PREFETCH_DISABLE); // xip prefetch disable
	qspi_spictrlr0_clk_stretch_en(QSPI0, CLK_STRETCH_ENABLE); // clk stretch en
	//-----------------------------------------------------------
	//program SSIENR register
	//enable qspi
	//-----------------------------------------------------------
	/*en ssi*/
	AlQspi_DmaInit(AL_DMAC_channel_6,rx,32,10,Dmac_transfer_row1,32);
	dwc_ssi_enable(QSPI0);

	//-----------------------------------------------------------
	//read flash - X4
	//sent:
	//inst(0x): 6B
	//addr(0x): 000000
	//receive(0x): 256 data items
	//-----------------------------------------------------------
//	qspi_data_transmit(QSPI0,CMD_QREAD); // x4 read cmd
//	qspi_data_transmit(QSPI0,0x000000); // tx addr[23:0]
	//2 word
	qspi_data_transmit(QSPI0,CMD_QREAD<<24); // inst+24'b0
	qspi_data_transmit(QSPI0,0x00000000); // addr[7:0]+addr[15:8]+addr[23:16]+8'b0
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	AlDma_Enable(AL_DMAC);
    AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_6);
    printf("DMA start receive data\r\n");
    while(AlDma_CheckChannelBusy(AL_DMAC));
    printf("DMA QSPI X4_REV_MODE transimate data has been completed\r\n");
    printf("qspi_dma_test\r\n");
    mem_data_check(64);
    printf("dma test qspi X4 mode transfer case has been completed");
#endif
    while(1);
	return 0;
}



