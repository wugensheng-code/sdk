/*
 * main.c
 *
 *  Created on: Dec 30, 2021
 *      Author: htliu
 */

#include <stdio.h>
//#include <time.h>
#include "nuclei_sdk_soc.h"

#define FLASH_RDID
#define FLASH_ERASE


//#define ADDR_24
#define ADDR_32

//#define S25FL512S_ID	0X00200201  // flash ID
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



/* main */
int main()
{
   /***********************AL9000***************************/
    printf("test start\r\n");
    /*variable init*/
    uint16_t i = 0;
    uint8_t wrdata_a[256] = {0};
    uint8_t rddata_a[256] = {0};
    for(i=0; i<256; i++)
    {
    	wrdata_a[i] = i;
    }

	/******************** flash x1 mode id-check**********************/
#ifdef FLASH_RDID
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
//	/******************** check flash status(WIP)*********************/
//	flash_wait_wip();
#endif
	/********************flash x1 mode erase/write/read**********************/
#ifdef FLASH_ERASE
    /***************************************************************/
    /************************flash erase****************************/
    /***************************************************************/
	printf("flash erase start...\r\n");
	//send WREN
	flash_write_enable();
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
	flash_wait_wip();
	printf("flash erase finish.\r\n");
#endif

	/***************************************************************/
	/************************flash write(X1) ***************************/
	/***************************************************************/
#ifdef ADDR_24
	printf("flash X1-mode write start... \n");
	//send WREN
	flash_write_enable();
	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
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
		qspi_data_transmit(QSPI0,wrdata_a[i]); // tx data wrdata_a[i]
	}
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	for(i=0; i<30; i++)
	{
//		printf("0x%x",wrdata_a[i]); // print write data
		printf("%3d",wrdata_a[i]); // print write data
	}
	printf(" \n");
	printf("flash X1-mode write finish. \n");
	/******************** check flash status(WIP)*********************/
	flash_wait_wip();
	/***************************************************************/
	/************************ flash read ***************************/
	/***************************************************************/
	/*****cfg reg for read******/
	printf("flash X1-mode read start... \n");
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
	//receive 256 data items
	//-----------------------------------------------------------
//	qspi_ctrl1_ndf(QSPI0, 0xFF); // receive 256 data items--FIFO MAX level=64
	qspi_ctrl1_ndf(QSPI0, 0x1D); // receive 30 data items

	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 4 data items is present in tx fifo: 1 inst + 3 addr
    //-----------------------------------------------------------
    /*cfg QSPI Transfer start FIFO level.*/
	qspi_txfifo_start_level_set(QSPI0, 3); // start data tx at 4th data

	/*en ssi*/
	dwc_ssi_enable(QSPI0);

	//-----------------------------------------------------------
	//read flash
	//sent:
	//inst(0x): 03 (X1 READ)
	//addr(0x): 00 00 00
	//receive(0x): 30 data items
	//-----------------------------------------------------------
	qspi_data_transmit(QSPI0,CMD_SREAD); // tx x1 read cmd
	qspi_data_transmit(QSPI0,0x00); // tx addr[23:16]
	qspi_data_transmit(QSPI0,0x00); // tx addr[15:8]
	qspi_data_transmit(QSPI0,0x00); // tx addr[7:0]
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
//	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0

	// read 30 bytes
	i=0;   // must init i=0; in case of RXFIFO still empty;
	do{
		if(qspi_sr_rfne(QSPI0))  // RXFIFO not empty
		{
			rddata_a[i]= qspi_data_read(QSPI0);
			i++;
		}
	}while(i<30);

	for(i=0; i<30; i++)
	{
//		printf("0x%x",rddata_a[i]); // print write data
		printf("%3d",rddata_a[i]); // print write data
	}
	printf("\n");
	printf("flash X1-mode read finish. \n");

	//----------- data check----------
	// 30 bytes
	for(i=0; i<30; i++)
	{
		if(wrdata_a[i] != rddata_a[i])
		{
			printf("FAIL: datas are not same!\r\n");
			return -1;
		}
	}
	printf("PASS: qspi x1 write/read test success!\r\n");
#endif

#ifdef ADDR_32
	printf("flash X1-mode write start... \n");
	//send WREN
	flash_write_enable();
	//CFG SSIENR
	/*dis ssi*/
	dwc_ssi_disable(QSPI0);
	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 34 data items is present in tx fifo: 1 inst + 4 addr + 30 datas
    //-----------------------------------------------------------
    /*cfg QSPI Transmit FIFO Threshold Level*/
	qspi_txftl_tft(QSPI0, 0); // default
    /*cfg QSPI Transfer start FIFO level.*/
//	qspi_txfifo_start_level_set(QSPI0, 0xFF); // 256 start data--FIFO MAX level=64
//	qspi_txfifo_start_level_set(QSPI0, 0x21); // 30+1+3 start data
	qspi_txfifo_start_level_set(QSPI0, 0x22); // 30+1+4 start data
	/*en ssi*/
	dwc_ssi_enable(QSPI0);

	//-----------------------------------------------------------
	//write flash: start when 8 data items is present in tx fifo: 1 inst + 4 addr + 30 datas
	//sent:
	//inst(0x): 12 (4PP)
	//addr(0x): 00 00 00 00
	//data(0x): 30 bytes
	//-----------------------------------------------------------
	qspi_data_transmit(QSPI0,0x12); // tx 4-Page Program cmd
	qspi_data_transmit(QSPI0,0x00); // tx addr[31:24]
	qspi_data_transmit(QSPI0,0x00); // tx addr[23:16]
	qspi_data_transmit(QSPI0,0x00); // tx addr[15:8]
	qspi_data_transmit(QSPI0,0x00); // tx addr[7:0]

	//30 bytes
	for(i=0; i<30; i++)
	{
		qspi_data_transmit(QSPI0,wrdata_a[i]); // tx data wrdata_a[i]
	}
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
	for(i=0; i<30; i++)
	{
//		printf("0x%x",wrdata_a[i]); // print write data
		printf("%3d",wrdata_a[i]); // print write data
	}
	printf(" \n");
	printf("flash X1-mode write finish. \n");
	/******************** check flash status(WIP)*********************/
	flash_wait_wip();
	/***************************************************************/
	/************************ flash read ***************************/
	/***************************************************************/
	/*****cfg reg for read******/
	printf("flash X1-mode read start... \n");
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
	//receive 256 data items
	//-----------------------------------------------------------
//	qspi_ctrl1_ndf(QSPI0, 0xFF); // receive 256 data items--FIFO MAX level=64
	qspi_ctrl1_ndf(QSPI0, 0x1D); // receive 30 data items(1D=30-1)

	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 4 data items is present in tx fifo: 1 inst + 4 addr
    //-----------------------------------------------------------
    /*cfg QSPI Transfer start FIFO level.*/
//	qspi_txfifo_start_level_set(QSPI0, 3); // start data tx at 4th data
	qspi_txfifo_start_level_set(QSPI0, 4); // start data tx at (1+4=5)th data,4=5-1

	/*en ssi*/
	dwc_ssi_enable(QSPI0);

	//-----------------------------------------------------------
	//read flash
	//sent:1 inst + 4 addr
	//inst(0x): 13 (X1 4READ)
	//addr(0x): 00 00 00 00
	//receive(0x): 30 data items
	//-----------------------------------------------------------
	qspi_data_transmit(QSPI0,0x13); // tx x1 read cmd
	qspi_data_transmit(QSPI0,0x00); // tx addr[31:24]
	qspi_data_transmit(QSPI0,0x00); // tx addr[23:16]
	qspi_data_transmit(QSPI0,0x00); // tx addr[15:8]
	qspi_data_transmit(QSPI0,0x00); // tx addr[7:0]
	/*check status--wait busy returns to idle*/
	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
//	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0

	// read 30 bytes
	i=0;   // must init i=0; in case of RXFIFO still empty;
	do{
		if(qspi_sr_rfne(QSPI0))  // RXFIFO not empty
		{
			rddata_a[i]= qspi_data_read(QSPI0);
			i++;
		}
	}while(i<30);

	for(i=0; i<30; i++)
	{
//		printf("0x%x",rddata_a[i]); // print write data
		printf("%3d",rddata_a[i]); // print write data
	}
	printf("\n");
	printf("flash X1-mode read finish. \n");

	//----------- data check----------
	// 30 bytes
	for(i=0; i<30; i++)
	{
		if(wrdata_a[i] != rddata_a[i])
		{
			printf("FAIL: datas are not same!\r\n");
			return -1;
		}
	}
	printf("PASS: qspi x1 write/read test success!\r\n");
#endif

    printf("test finish\r\n");
    while(1);
}
