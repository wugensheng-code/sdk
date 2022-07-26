/*
 * qspi_test.c
 *
 *  Created on: 2022年5月5日
 *      Author: Administrator
 */
#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"
#include "delay.h"
#define FLASH_RDID
#define FLASH_ERASE
#define FLASH_X1
#define FLASH_X1_DFS_8
#define FLASH_X1_DFS_32
#define FLASH_X2
#define FLASH_X4
#define S25FL256S_ID	0x00190201  // flash ID
uint8_t qspi_wrdata_a[256] = {0};
uint8_t qspi_rddata_a[256] = {0};
/****************function define******************************/
/*
* Read Manufacture ID/ Device ID - in x1-QSPI mode
*/
uint32_t test_SReadID(void)
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
void test_flash_write_enable()
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
	//qspi_data_transmit(QSPI0,CMD_RESET); // tx wren cmd
	//while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	//while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	//for(volatile int delay_ms = 0;delay_ms < 10000;delay_ms++){}
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
uint32_t test_flash_wait_wip(void)
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
	//printf("state is %x", statreg1);
	}while(statreg1 & SR1_WIP); // check if flash operation complete 
	return statreg1;
}

uint32_t flash_get_sr(void)
{
	uint32_t statreg1 = 0xFF;
	uint32_t i=0;
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

	qspi_data_transmit(QSPI0,CMD_RDSR); // tx read status register cmd
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0))
		printf("sr_tfe"); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0))
		printf("sr_busy");  // check busy or idle,wait BUSY returns to 0

	statreg1 = qspi_data_read(QSPI0); // read status register1
	printf("flash_wait_wip sr:0x%x\r\n",statreg1);

	return statreg1;
}

uint32_t flash_get_cr(void)
{
	uint32_t statreg1 = 0xFF;
	uint32_t i=0;
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

	qspi_data_transmit(QSPI0,0x35); // tx read status register cmd
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0

	statreg1 = qspi_data_read(QSPI0); // read status register1
	printf("flash_wait_wip cr:0x%x\r\n",statreg1);

	return statreg1;
}

void flash_reset()
{
	u32 tempcr = 0;
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
	qspi_data_transmit(QSPI0,CMD_RESET); // tx wren cmd
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	//qspi_data_transmit(QSPI0,0x99); // tx wren cmd
		/*check status--wait busy returns to idle*/
	//while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	//while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	_delay_ms(1);	//35us in spec
	tempcr = flash_get_cr();
	test_flash_write_enable();
	dwc_ssi_disable(QSPI0);
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
	qspi_data_transmit(QSPI0,CMD_WRR);
	qspi_data_transmit(QSPI0,0x0);		//清空写保护
	qspi_data_transmit(QSPI0,tempcr);		//写BPNV域
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));
	test_flash_wait_wip();
}

void flash_clear_sr()
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
	//qspi_data_transmit(QSPI0,CMD_CLSR); // tx wren cmd
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0

	//qspi_data_transmit(QSPI0,0x99); // tx wren cmd
		/*check status--wait busy returns to idle*/
	//while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	//while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	_delay_ms(30);
}

uint32_t qspi_test(){
	uint16_t i = 0;
	uint32_t tempcr, tempsr = 0;
	uint32_t wrdata_tmp = 0;
    uint32_t rddata_tmp = 0;
	uint16_t rxdata_num = 0;
	uint16_t txfifo_start_level = 0;

	for(i=0; i<256; i++)
    {
    	qspi_wrdata_a[i] = i;
    }
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");
    printf("***************************QSPI TEST*****************************\r\n");
    /******************** flash x1 mode id-check**********************/
        /***************************************************************/
        /************************flash ID check****************************/
        /***************************************************************/
    	uint32_t flash_id = 0;
    	//CFG SSIENR
    	/*dis ssi*/
    	dwc_ssi_disable(QSPI0);

    	//-----------------------------------------------------------
    	//program CTRLR0 register
    	//8-bit data frame, x1 mode, EEPROM read
    	//-----------------------------------------------------------
    	/*cfg ssi as spi mode*/
    	//qspi_frf_spi(QSPI0);
    	/*cfig spi master*/
    	//qspi_mas_en(QSPI0);
    	/*cfg QSPI Data Frame Size.*/
    	qspi_dfs(QSPI0,DFS_BYTE);   // byte
    	/*Slave Select Toggle disable*/
    //	qspi_sste_dis(QSPI0);
    	/*cfg SPI Frame Format*/
    	qspi_x1_mode(QSPI0);
    	/*Transfer Mode.*/
    	qspi_tmod_e2prom(QSPI0); // EEPROM read

    	//-----------------------------------------------------------
    	//program CTRLR1 register
    	//receive 3 data items
    	//-----------------------------------------------------------
    	qspi_ctrl1_ndf(QSPI0, 2); // receive 2+1 data items
		qspi_rxftl_cfg(QSPI0,0x3F);
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
    	//program SSIENR register
    	//enable qspi
    	//-----------------------------------------------------------
    	/*en ssi*/
    	dwc_ssi_enable(QSPI0);

    	//-----------------------------------------------------------
    	//sent:
    	//inst(0x): 9f (RDID)
    	//receive: 3 data items
    	//-----------------------------------------------------------
		
    	flash_id = test_SReadID();
    	printf("flash_id= 0x%08x\r\n",flash_id); // print flash ID
    	/*flash ID check*/
    	if(flash_id != S25FL256S_ID)
    	{
    		printf("FAIL: flash ID check not ok!\r\n");
			flash_reset();
			tempsr = flash_get_sr();
			tempcr = flash_get_cr();
			dwc_ssi_disable(QSPI0);
			qspi_dfs(QSPI0,DFS_BYTE);
			qspi_x1_mode(QSPI0);
			qspi_tmod_e2prom(QSPI0);
			qspi_ctrl1_ndf(QSPI0, 2); // receive 2+1 data items
			qspi_rxftl_cfg(QSPI0,0x3F);
			qspi_ser(QSPI0,SER_SS0_EN);
			qspi_sckdiv_cfg(QSPI0, 0x2);
			dwc_ssi_enable(QSPI0);
			flash_id = test_SReadID();
			printf("flash_id= 0x%08x\r\n",flash_id);
    		//return -1;
    	}
    	else
    	{
    		printf("PASS: flash ID check ok!\r\n");
    	}
    //	/******************** check flash status(WIP)*********************/
    //	flash_wait_wip();
    	/********************flash x1 mode erase/write/read**********************/
        /***************************************************************/
        /************************flash erase****************************/
        /***************************************************************/
    	printf("flash erase start...\r\n");
    	//send WREN
    	test_flash_write_enable();
    	//-----------------------------------------------------------
    	//The Chip Erase (CE) command is used to erase all the data of the chip.
    	//sent:
    	//inst(0x): 60 (chip erase)
    	//-----------------------------------------------------------
    	//chip erase
    	qspi_data_transmit(QSPI0,CMD_CHIP_ERASE); // tx sector erase cmd

    	/*check status--wait busy returns to idle*/
    	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
    	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0

    	/******************** check flash status(WIP)*********************/
    	test_flash_wait_wip();
    	printf("flash erase finish.\r\n");

	/***************************************************************/
	/******************flash write(X1 8-bit/32-bit) *********************/
	/***************************************************************/
#ifdef FLASH_X1
#ifdef FLASH_X1_DFS_8
	//send WREN
	test_flash_write_enable();

	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
	//-----------------------------------------------------------
	//program CTRLR0 register
	//8-bit data frame,
	//-----------------------------------------------------------
	/*cfg QSPI Data Frame Size.*/
	qspi_dfs(QSPI0,DFS_BYTE);   // Byte
	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 34 data items is present in tx fifo: 1 inst + 3 addr + 30 datas
    //-----------------------------------------------------------
    /*cfg QSPI Transmit FIFO Threshold Level*/
	qspi_txftl_tft(QSPI0, 0); // default
    /*cfg QSPI Transfer start FIFO level.*/
//	qspi_txfifo_start_level_set(QSPI0, 0xFF); // 256 start data--FIFO MAX level=64
	qspi_txfifo_start_level_set(QSPI0, 0x21); // 30+1+3 start data
	/*en ssi*/
	dwc_ssi_enable(QSPI0);

	//-----------------------------------------------------------
	//write flash: start when 8 data items is present in tx fifo: 1 inst + 3 addr + 30 datas
	//sent:
	//inst(0x): 02 (PP)
	//addr(0x): 00 00 00
	//data(0x): 30 bytes
	//-----------------------------------------------------------
	qspi_data_transmit(QSPI0,CMD_PP); // tx Page Program cmd
	qspi_data_transmit(QSPI0,0x00); // tx addr[23:16]
	qspi_data_transmit(QSPI0,0x00); // tx addr[15:8]
	qspi_data_transmit(QSPI0,0x00); // tx addr[7:0]

	//30 bytes
	for(i=0; i<30; i++)
	{
		qspi_data_transmit(QSPI0,qspi_wrdata_a[i]); // tx data wrdata_a[i]
	}
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	for(i=0; i<30; i++)
	{
//		printf("0x%x",wrdata_a[i]); // print write data
		printf("%3d",qspi_wrdata_a[i]); // print write data
	}
	printf("...");
	printf(" \n");
	printf("flash X1 write finish. \n");
#endif
#ifdef FLASH_X1_DFS_32
	printf("flash X1-mode page-program write start... \n");
	//send WREN
	test_flash_write_enable();

	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
	//-----------------------------------------------------------
	//program CTRLR0 register
	//32-bit data frame,
	//-----------------------------------------------------------
	/*cfg QSPI Data Frame Size.*/
	qspi_dfs(QSPI0,DFS_WORD);   // Word
	//-----------------------------------------------------------
    //program TXFTLR register--Page Program 256B
    //start when 260B data items is present in tx fifo: 1 inst + 3 addr + 256 datas
    //-----------------------------------------------------------
	txfifo_start_level = 0x41; // 65W=260B
	/*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, txfifo_start_level-1); // 65W=260B
//	qspi_txfifo_start_level_set(QSPI0, 0x21); // 30+1+3 start data
	/*en ssi*/
	dwc_ssi_enable(QSPI0);
	//-----------------------------------------------------------
	//write flash: start when 260B data items is present in tx fifo: 1 inst + 3 addr + 256 datas
	//sent:
	//inst(0x): 02 (PP)
	//addr(0x): 00 00 00
	//data(0x): 256 bytes
	//-----------------------------------------------------------
	//1 word
	qspi_data_transmit(QSPI0,0x00000000|CMD_PP); // addr[7:0]+addr[15:8]+addr[23:16]+inst

	//64 Words
	for(i=0; i<256; i+=4)
	{
		wrdata_tmp = (qspi_wrdata_a[i+3]<<24)|(qspi_wrdata_a[i+2]<<16)|(qspi_wrdata_a[i+1]<<8)|qspi_wrdata_a[i]; // 4 byte
		qspi_data_transmit(QSPI0,wrdata_tmp); // tx data wrdata_tmp
	}
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	for(i=0; i<256; i++)
	{
		printf("%3d",qspi_wrdata_a[i]); // print write data
	}
	printf(" \n");
	printf("flash X1-mode page-program write finish. \n");
#endif
	/******************** check flash status(WIP)*********************/
	test_flash_wait_wip();
	/***************************************************************/
	/************************ flash read ***************************/
	/***************************************************************/
	/*****cfg reg for read******/
	printf("flash X1-mode read start... \n");
#ifdef FLASH_X1_DFS_32
	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
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
    //start when 1 word data items is present in tx fifo: 1word= 1 inst + 3 addr
    //-----------------------------------------------------------
    /*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, 1-1); // start data tx after transmit 1 data

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
//	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0

	// read 256 bytes
	i=0;   // must init i=0; in case of RXFIFO still empty;
	do{
		if(qspi_sr_rfne(QSPI0))  // RXFIFO not empty
		{
			rddata_tmp= qspi_data_read(QSPI0);
			qspi_rddata_a[i]= (uint8_t)(rddata_tmp & 0xFF);
			qspi_rddata_a[i+1]= (uint8_t)((rddata_tmp>>8) & 0xFF);
			qspi_rddata_a[i+2]= (uint8_t)((rddata_tmp>>16) & 0xFF);
			qspi_rddata_a[i+3]= (uint8_t)((rddata_tmp>>24) & 0xFF);
			i+=4;
		}
	}while(i<256);

	for(i=0; i<256; i++)
	{
		printf("%3d",qspi_rddata_a[i]); // print read data
	}
	printf("\n");
	printf("flash X1-mode read finish. \n");
	//----------- data check----------
	// 256 bytes
	for(i=0; i<256; i++)
	{
		if(qspi_wrdata_a[i] != qspi_rddata_a[i])
		{
			printf("FAIL: datas are not same!\r\n");
			return -1;
		}
	}
#endif

	printf("PASS: qspi x1-mode write/read test success!\r\n");

#endif

	/************************ flash X2 mode read****************************/
#ifdef FLASH_X2  // this type flash not support X2 Page Program
	/*****************flash read(X2) ****************/
	printf("flash x2-mode read start...\n");
	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);

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
	printf("flash x2 1111...\n");
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
//	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	// read 256 bytes
	printf("flash x2 2222...\n");
	for(i=0; i<256; i++)
	    {
		qspi_rddata_a[i] = 0;
	    }
	i=0;   // must init i=0; in case of RXFIFO still empty;
	do{
		if(qspi_sr_rfne(QSPI0))  // RXFIFO not empty
		{
			rddata_tmp = qspi_data_read(QSPI0);
			qspi_rddata_a[i]= (uint8_t)(rddata_tmp & 0xFF);
			qspi_rddata_a[i+1]= (uint8_t)((rddata_tmp >>8) & 0xFF);
			qspi_rddata_a[i+2]= (uint8_t)((rddata_tmp >>16) & 0xFF);
			qspi_rddata_a[i+3]= (uint8_t)((rddata_tmp >>24) & 0xFF);
			i+=4;
		}
	}while(i<256);

	for(i=0; i<256; i++)
	{
		printf("%3d",qspi_rddata_a[i]); // print write data
	}
	printf("\n");
	printf("flash x2-mode read finish. \n");

	//----------- data check----------
	// 256 bytes
	for(i=0; i<256; i++)
	if(qspi_wrdata_a[i] != qspi_rddata_a[i])
	{
		printf("FAIL: datas are not same!\r\n");
		return -1;
	}
	printf("PASS: qspi x2-mode read test success!\r\n");

#endif
    	/************************ flash X4 ****************************/
    	/***************************************************************/
    	/*********************X4 mode enable (QE=1) ********************/
    	/***************************************************************/
    	printf("X4 mode enable\r\n");
		tempcr = flash_get_cr();
		tempcr |= 0x2;
    	//send WREN
    	test_flash_write_enable();
    	//CFG SSIENR
    	/*dis ssi*/
    	dwc_ssi_disable(QSPI0);
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
    	//inst(0x): 01 (WRSR)
    	//data(0x): 02 02(quad=1,enable)
    	//-----------------------------------------------------------
    	qspi_data_transmit(QSPI0,CMD_WRR); // write register cmd
    	qspi_data_transmit(QSPI0,0x02); // write status register
    	qspi_data_transmit(QSPI0,tempcr); // QE,write configuration register-quad=1
    	/*check status--wait busy returns to idle*/
    	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
    	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
    	
		flash_get_cr();
		/******************** check flash status(WIP)*********************/
    	test_flash_wait_wip();
    	/******************** flash write enable*********************/
    	//send WREN
    	test_flash_write_enable();
    	/***************************************************************/
    	/************X4 mode write (QPP:0x32h) *********/
    	printf("flash X4-mode write start...\r\n");
    	//CFG SSIENR
    	/*dis ssi*/
    	dwc_ssi_disable(QSPI0);
    	//-----------------------------------------------------------
    	//program CTRLR0 register
    	//8-bit data frame, x4 mode, RX only
    	//-----------------------------------------------------------
    	/*cfg QSPI Data Frame Size.*/
    	qspi_dfs(QSPI0,DFS_BYTE);   // byte
    	/*Slave Select Toggle disable*/
    	/*cfg SPI Frame Format*/
    	qspi_x4_mode(QSPI0);
    	/*Transfer Mode.*/
    	qspi_tmod_tx(QSPI0); // TX only

    //	//-----------------------------------------------------------
    //	//program CTRLR1 register
    //	//receive 30 data items
    //	//-----------------------------------------------------------
    	qspi_ctrl1_ndf(QSPI0, 0xFF); // stretching=en & tmod=tx, nCS invalid after tx 0xFF data items.
    	//-----------------------------------------------------------
    	//program TXFTLR register
    	//start when 2 data items is present in tx fifo: 1 inst + 1 addr
    	//-----------------------------------------------------------
    	/*cfg QSPI Transmit FIFO Threshold Level*/
    	qspi_txftl_tft(QSPI0, 0); // default
    	/*cfg QSPI Transfer start FIFO level.*/
    	qspi_txfifo_start_level_set(QSPI0, 0x3); // 3+1 start data

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
    	//-----------------------------------------------------------
    	//program SSIENR register
    	//enable qspi
    	//-----------------------------------------------------------
    	/*en ssi*/
    	dwc_ssi_enable(QSPI0);

    	//-----------------------------------------------------------
    	//write flash - X4
    	//sent:
    	//inst(0x): 32h
    	//addr(0x): 00 00 00
    	//transmit(0x): N data items
    	//-----------------------------------------------------------
    	qspi_data_transmit(QSPI0,CMD_QPP); // x4 write cmd
    	qspi_data_transmit(QSPI0,0x000000); // tx addr[23:0]
    	// write 256 bytes
    	for(i=0; i<256; i++)
    	{
    		qspi_data_transmit(QSPI0,qspi_wrdata_a[i]); // tx data wrdata_a[i]
    	}

    	/*check status--wait busy returns to idle*/
    	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
    	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
    	printf("flash x4-mode write finish.\r\n");
    //	/******************** check flash status(WIP)*********************/
    //	flash_wait_wip();
    	/************X4 mode read (0x6B) *********/
    	printf("flash X4-mode read start...\r\n");
    	//CFG SSIENR
    	/*dis ssi*/
    	dwc_ssi_disable(QSPI0);
    	//-----------------------------------------------------------
    	//program CTRLR0 register
    	//8-bit data frame, x4 mode, RX only
    	//-----------------------------------------------------------
    	/*cfg QSPI Data Frame Size.*/
    	qspi_dfs(QSPI0,DFS_BYTE);   // byte
    	/*Slave Select Toggle disable*/
    //	qspi_sste_dis(QSPI0);
    	/*cfg SPI Frame Format*/
    	qspi_x4_mode(QSPI0);
    	/*Transfer Mode.*/
    	qspi_tmod_rx(QSPI0); // RX only

    	//-----------------------------------------------------------
    	//program CTRLR1 register
    	//receive 256 data items
    	//-----------------------------------------------------------
    	qspi_ctrl1_ndf(QSPI0, 0xFF); // receive 255+1 data items
    	//-----------------------------------------------------------
    	//program TXFTLR register
    	//start when 2 data items is present in tx fifo: 1 inst + 1 addr
    	//-----------------------------------------------------------
    	/*cfg QSPI Transmit FIFO Threshold Level*/
    	qspi_txftl_tft(QSPI0, 0); // default
    	/*cfg QSPI Transfer start FIFO level.*/
    	qspi_txfifo_start_level_set(QSPI0, 0x3); // 3+1 start data
    	//-----------------------------------------------------------
    	//program RXFTLR register
    	//must cfg rxftl(>0) when rx data num > rxfifo level max, otherwise read will not ok.
    	//-----------------------------------------------------------
    	qspi_rxftl_cfg(QSPI0,0x3F); // 63=128/2-1
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
    	dwc_ssi_enable(QSPI0);

    	//-----------------------------------------------------------
    	//read flash - X4
    	//sent:
    	//inst(0x): 6B
    	//addr(0x): 000000
    	//receive(0x): 30 data items
    	//-----------------------------------------------------------
    	qspi_data_transmit(QSPI0,CMD_QREAD); // x4 read cmd
    	qspi_data_transmit(QSPI0,0x000000); // tx addr[23:0]
    	/*check status--wait busy returns to idle*/
    	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
    	// read 256 bytes
    	i=0;   // must init i=0; in case of RXFIFO still empty;
    	do{
    		if(qspi_sr_rfne(QSPI0))
    		{
    			qspi_rddata_a[i]= qspi_data_read(QSPI0);
    			i++;
    		}
    	}while(i<256);
    	printf("flash x4-mode read finish.\r\n");

    	//----------- data check----------
    	// 256 bytes
    	for(i=0; i<256; i++)
    	if(qspi_wrdata_a[i] != qspi_rddata_a[i])
    	{
    		printf("FAIL: datas are not same!\r\n");
    		return -1;
    	}
    	printf("PASS: qspi x4-mode write/read test success!\r\n");
    	printf("Qspi X4 test finish\r\n");
}


