/*
 * main.c
 *
 *  Created on: Oct.27.2021
 *      Author: haitao.liu
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"

//#define FLASH
#define FLASH_RDID
#define FLASH_ERASE_WRITE
#define FLASH_READ

#define SPI_MAS
#define SPI_SALVE ((SPI_TypeDef *)(SPI0_BASE + 0x100))

#define S25FL512S_ID 0X00200201 // flash ID
//#define S25FL256S_ID	0x00190201  // flash ID

/****************function define******************************/
/*
 * Read Manufacture ID/ Device ID - in x1-SPI mode
 */
uint32_t SReadID(void)
{
	//-----------------------------------------------------------
	// sent:
	// inst(0x): 9f (RDID)
	// receive: 3 data items
	//-----------------------------------------------------------
	uint32_t temp = 0;
	spi_data_transmit(SPI_SALVE, CMD_RDID); // tx read id cmd
	while (!spi_sr_tfe(SPI_SALVE))
		;
	while (spi_sr_busy(SPI_SALVE))
		; // check busy or idle,wait BUSY returns to 0
	temp |= spi_data_read(SPI_SALVE);
	temp |= spi_data_read(SPI_SALVE) << 8;
	temp |= spi_data_read(SPI_SALVE) << 16;
	return temp;
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
	// CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_SALVE);
	//-----------------------------------------------------------
	// program CTRLR0 register
	// 8-bit data frame, x1 mode, EEPROM read
	//-----------------------------------------------------------
	/*cfg SPI Data Frame Size.*/
	spi_dfs(SPI_SALVE, SPI_DFS_BYTE); // byte
	/*Slave Select Toggle disable*/
	spi_sste_dis(SPI_SALVE);
	/*cfg SPI Frame Format*/
	spi_x1_mode(SPI_SALVE);
	/*Transfer Mode.*/
	spi_tmod_e2prom(SPI_SALVE); // e2prom read
	//-----------------------------------------------------------
	// program CTRLR1 register
	// receive 1 data items
	//-----------------------------------------------------------
	spi_ctrl1_ndf(SPI_SALVE, 0);
	//-----------------------------------------------------------
	// program TXFTLR register
	// start when 1 data items is present in tx fifo
	//-----------------------------------------------------------
	/*cfg SPI Transmit FIFO Threshold Level*/
	spi_txftl_tft(SPI_SALVE, 0); // default

	/*en ssi*/
	spi_dwc_ssi_enable(SPI_SALVE);
	//-----------------------------------------------------------
	// read flash status register to check if flash operation complete
	// sent:
	// inst(0x): 05 (RDSR)
	// receive: 1 data items
	//-----------------------------------------------------------
	do
	{
		spi_data_transmit(SPI_SALVE, CMD_RDSR); // tx read status register cmd
		/*check status--wait busy returns to idle*/
		while (!spi_sr_tfe(SPI_SALVE))
			; // wait TFE returns to 1
		while (spi_sr_busy(SPI_SALVE))
			;								 // check busy or idle,wait BUSY returns to 0
		statreg1 = spi_data_read(SPI_SALVE); // read status register1
	} while (statreg1 & SR1_WIP);			 // check if flash operation complete
	return statreg1;
}

/* main */
int main()
{
	uint32_t i = 0;
	uint32_t sr = 0;
	uint32_t rddata = 0;
	/***********************AL9000***************************/
	*(uint32_t *)(0xf8800160) = 0x1;
	printf("test start\r\n");
	/*variable init*/
	/*
	  uint8_t wrdata_a[256] = {0};
	  uint8_t rddata_a[256] = {0};
	  for(i=0; i<256; i++)
	  {
		  wrdata_a[i] = i;
	  }
	  */
#ifdef SPI_MAS
	// CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_SALVE);
	//-----------------------------------------------------------
	// program CTRLR0 register
	// 8-bit data frame, x1 mode, TX only
	//-----------------------------------------------------------
	/*cfg SPI Data Frame Size.*/
	spi_dfs(SPI_SALVE, SPI_DFS_BYTE); // byte
	/*Slave Select Toggle disable*/
	// spi_sste_dis(SPI_SALVE+0x100);
	/*cfg SPI Frame Format*/
	// spi_x1_mode(SPI_SALVE+0x100);
	/*Transfer Mode.*/
	// spi_tmod_tx(SPI_SALVE+0x100); // TX only
	spi_tmod_rx(SPI_SALVE);
	//-----------------------------------------------------------
	// program TXFTLR register
	// start when 1 data items is present in tx fifo
	//-----------------------------------------------------------
	/*cfg SPI Transmit FIFO Threshold Level*/
	spi_txftl_tft(SPI_SALVE, 0); // default
	//-----------------------------------------------------------
	// program SER register
	// choose ss_0_n
	//-----------------------------------------------------------
	spi_ser(SPI_SALVE, SER_SS0_EN); // choose ss0--same to emulation platform.
	spi_frf_spi(SPI_SALVE);
	//-----------------------------------------------------------
	// program SSIENR register
	// enable spi
	//-----------------------------------------------------------
	/*en ssi*/
	spi_dwc_ssi_enable(SPI_SALVE);
	spi_rxftl_cfg(SPI_SALVE, 7);
	//-----------------------------------------------------------
	// TX data
	//-----------------------------------------------------------
	//	//256 bytes
	//	for(i=0; i<256; i++)
	//	{
	//		spi_data_transmit(SPI_SALVE,wrdata_a[i]); // tx data wrdata_a[i]
	//	}

	// 256 bytes
	while (1)
	{
		sr = *(uint32_t *)(0xf8404000 + 0x130);
		// printf("isr:0x%x\n\r",sr);
		if ((sr & 0x10) == 0x10)
		{
			printf("isr:0x%x\n\r", sr);
			// spi_data_transmit(SPI_SALVE+0x100,i); // tx data i
			for (i = 0; i < 8; i++)
			{
				rddata = spi_data_read(SPI_SALVE);
				printf("spi slave rdata:0x%x\n\r", rddata);
			}
		}

		i++;
	}
#endif

#ifdef FLASH
	/***************************************************************/
	/************************flash ID check*************************/
	/***************************************************************/
#ifdef FLASH_RDID
	uint32_t flash_id = 0;
	// CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_SALVE);

	//-----------------------------------------------------------
	// program CTRLR0 register
	// 8-bit data frame, x1 mode, EEPROM read
	//-----------------------------------------------------------
	/*cfg SPI Data Frame Size.*/
	spi_dfs(SPI_SALVE, SPI_DFS_BYTE); // byte
	/*Slave Select Toggle disable*/
	spi_sste_dis(SPI_SALVE);
	/*cfg SPI Frame Format*/
	spi_x1_mode(SPI_SALVE);
	/*Transfer Mode.*/
	spi_tmod_e2prom(SPI_SALVE); // EEPROM read
	//-----------------------------------------------------------
	// program TXFTLR register
	// start when 1 data items is present in tx fifo
	//-----------------------------------------------------------
	/*cfg SPI Transmit FIFO Threshold Level*/
	spi_txftl_tft(SPI_SALVE, 0); // default
	//-----------------------------------------------------------
	// program CTRLR1 register
	// receive 3 data items
	//-----------------------------------------------------------
	spi_ctrl1_ndf(SPI_SALVE, 2); // receive 2+1 data items

	//-----------------------------------------------------------
	// program SER register
	// choose ss_0_n
	//-----------------------------------------------------------
	spi_ser(SPI_SALVE, SER_SS0_EN); // choose ss0--same to emulation platform.

	//-----------------------------------------------------------
	// program BAUDR register
	// divided by 30 --(30 from design)
	//-----------------------------------------------------------
	/*cfg SPI Clock Divider.*/
	spi_sckdiv_cfg(SPI_SALVE, 0x1E); // divided by 30.

	//-----------------------------------------------------------
	// program SSIENR register
	// enable spi
	//-----------------------------------------------------------
	/*en ssi*/
	spi_dwc_ssi_enable(SPI_SALVE);

	//-----------------------------------------------------------
	// sent:
	// inst(0x): 9f (RDID)
	// receive: 3 data items
	//-----------------------------------------------------------
	flash_id = SReadID();
//	printf("flash_id= 0x%08x\r\n",flash_id); // print flash ID
//	/*flash ID check*/
//	if(flash_id != S25FL512S_ID)
//	{
//		printf("FAIL: flash ID check not ok!\r\n");
//		return -1;
//	}
//	else
//	{
//		printf("PASS: flash ID check ok!\r\n");
//	}
#endif

	/***************************************************************/
	/************************flash erase****************************/
	/***************************************************************/
#ifdef FLASH_ERASE_WRITE
	// CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_SALVE);

	//-----------------------------------------------------------
	// program CTRLR0 register
	// 8-bit data frame, x1 mode, TX only
	//-----------------------------------------------------------
	/*cfg SPI Data Frame Size.*/
	spi_dfs(SPI_SALVE, SPI_DFS_BYTE); // byte
	/*Slave Select Toggle disable*/
	spi_sste_dis(SPI_SALVE);
	/*cfg SPI Frame Format*/
	spi_x1_mode(SPI_SALVE);
	/*Transfer Mode.*/
	spi_tmod_tx(SPI_SALVE); // TX only
	//-----------------------------------------------------------
	// program TXFTLR register
	// start when 1 data items is present in tx fifo
	//-----------------------------------------------------------
	/*cfg SPI Transmit FIFO Threshold Level*/
	spi_txftl_tft(SPI_SALVE, 0); // default

	//-----------------------------------------------------------
	// program SSIENR register
	// enable spi
	//-----------------------------------------------------------
	/*en ssi*/
	spi_dwc_ssi_enable(SPI_SALVE);

	//-----------------------------------------------------------
	// sent:
	// inst(0x): 06 (WREN)
	//-----------------------------------------------------------
	spi_data_transmit(SPI_SALVE, CMD_WREN); // tx read id cmd
	/*check status--wait busy returns to idle*/
	while (!spi_sr_tfe(SPI_SALVE))
		; // wait TFE returns to 1
	while (spi_sr_busy(SPI_SALVE))
		; // check busyl or idle,wait BUSY returns to 0
	//-----------------------------------------------------------
	// The Chip Erase (CE) command is used to erase all the data of the chip.
	// sent:
	// inst(0x): 60 (chip erase)
	// inst(0x): D8 (sector erase)
	//-----------------------------------------------------------
	// chip erase
	//	spi_data_transmit(SPI_SALVE,CMD_CHIP_ERASE); // tx chip erase cmd

	// sector erase
	spi_data_transmit(SPI_SALVE, CMD_SECTOR_ERASE); // tx sector erase cmd
	spi_data_transmit(SPI_SALVE, 0x00);				// tx addr[23:16]
	spi_data_transmit(SPI_SALVE, 0x00);				// tx addr[15:8]
	spi_data_transmit(SPI_SALVE, 0x00);				// tx addr[7:0]

	/*check status--wait busy returns to idle*/
	while (!spi_sr_tfe(SPI_SALVE))
		; // wait TFE returns to 1
	while (spi_sr_busy(SPI_SALVE))
		; // check busy or idle,wait BUSY returns to 0

	/******************** check flash status(WIP)*********************/
	flash_wait_wip();
	/***************************************************************/
	/************************flash write ***************************/
	/***************************************************************/
	// CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_SALVE);
	//-----------------------------------------------------------
	// program CTRLR0 register
	// 8-bit data frame, x1 mode, TX only
	//-----------------------------------------------------------
	/*cfg SPI Data Frame Size.*/
	spi_dfs(SPI_SALVE, SPI_DFS_BYTE); // byte
	/*Slave Select Toggle disable*/
	spi_sste_dis(SPI_SALVE);
	/*cfg SPI Frame Format*/
	spi_x1_mode(SPI_SALVE);
	/*Transfer Mode.*/
	spi_tmod_tx(SPI_SALVE); // TX only

	//-----------------------------------------------------------
	// program TXFTLR register
	// start when 1 data items is present in tx fifo
	//-----------------------------------------------------------
	spi_txftl_tft(SPI_SALVE, 0); // default
	/*en ssi*/
	spi_dwc_ssi_enable(SPI_SALVE);

	//-----------------------------------------------------------
	// sent:
	// inst(0x): 06 (WREN)
	//-----------------------------------------------------------
	spi_data_transmit(SPI_SALVE, CMD_WREN); // tx write en cmd
	/*check status--wait busy returns to idle*/
	while (!spi_sr_tfe(SPI_SALVE))
		; // wait TFE returns to 1
	while (spi_sr_busy(SPI_SALVE))
		; // check busy or idle,wait BUSY returns to 0

	// CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_SALVE);
	//-----------------------------------------------------------
	// program TXFTLR register
	//-----------------------------------------------------------
	/*cfg SPI Transmit FIFO Threshold Level*/
	spi_txftl_tft(SPI_SALVE, 0); // default

	/*en ssi*/
	spi_dwc_ssi_enable(SPI_SALVE);

	//-----------------------------------------------------------
	// write flash: 1 inst + 3 addr + 256 datas
	// sent:
	// inst(0x): 02 (PP)
	// addr(0x): 00 00 00
	// data(0x): 256 bytes
	//-----------------------------------------------------------
	spi_data_transmit(SPI_SALVE, CMD_PP); // tx Page Program cmd
	spi_data_transmit(SPI_SALVE, 0x00);	  // tx addr[23:16]
	spi_data_transmit(SPI_SALVE, 0x00);	  // tx addr[15:8]
	spi_data_transmit(SPI_SALVE, 0x00);	  // tx addr[7:0]

	// 256 bytes
	for (i = 0; i < 256; i++)
	{
		spi_data_transmit(SPI_SALVE, wrdata_a[i]); // tx data wrdata_a[i]
	}
	/*check status--wait busy returns to idle*/
	while (!spi_sr_tfe(SPI_SALVE))
		; // wait TFE returns to 1
	while (spi_sr_busy(SPI_SALVE))
		; // check busy or idle,wait BUSY returns to 0
		  //	for(i=0; i<256; i++)
		  //	{
		  //		printf("%3d",wrdata_a[i]); // print write data
		  //	}
		  //	printf(" \n");
		  //	printf("flash write finish. \n");
	/******************** check flash status(WIP)*********************/
	flash_wait_wip();
#endif
	/***************************************************************/
	/************************ flash read ***************************/
	/***************************************************************/
#ifdef FLASH_READ
	/*****cfg reg for read******/
	// CFG SSIENR
	/*dis ssi*/
	spi_dwc_ssi_disable(SPI_SALVE);
	//-----------------------------------------------------------
	// program CTRLR0 register
	// 8-bit data frame, x1 mode, EEPROM read
	//-----------------------------------------------------------
	/*cfg SPI Data Frame Size.*/
	spi_dfs(SPI_SALVE, SPI_DFS_BYTE); // byte
	/*Slave Select Toggle disable*/
	spi_sste_dis(SPI_SALVE);
	/*cfg SPI Frame Format*/
	spi_x1_mode(SPI_SALVE);
	/*Transfer Mode.*/
	spi_tmod_e2prom(SPI_SALVE); // e2prom read
	//-----------------------------------------------------------
	// program CTRLR1 register
	// receive 256 data items
	//-----------------------------------------------------------
	spi_ctrl1_ndf(SPI_SALVE, 0xFF); // receive 256 data items--FIFO MAX level=64
									//	spi_ctrl1_ndf(SPI_SALVE, 0x1D); // receive 30 data items

	/*en ssi*/
	spi_dwc_ssi_enable(SPI_SALVE);

	//-----------------------------------------------------------
	// read flash
	// sent:
	// inst(0x): 03 (X1 READ)
	// addr(0x): 00 00 00
	// receive(0x): 256 data items
	//-----------------------------------------------------------
	spi_data_transmit(SPI_SALVE, CMD_SREAD); // tx x1 read cmd
	spi_data_transmit(SPI_SALVE, 0x00);		 // tx addr[23:16]
	spi_data_transmit(SPI_SALVE, 0x00);		 // tx addr[15:8]
	spi_data_transmit(SPI_SALVE, 0x00);		 // tx addr[7:0]
	/*check status--wait busy returns to idle*/
	while (!spi_sr_tfe(SPI_SALVE))
		; // wait TFE returns to 1
		  //	while(spi_sr_busy(SPI_SALVE));  // check busy or idle,wait BUSY returns to 0
	// read 256 bytes
	i = 0; // must init i=0; in case of RXFIFO still empty;
	do
	{
		if (spi_sr_rfne(SPI_SALVE)) // RXFIFO not empty
		{
			rddata_a[i] = spi_data_read(SPI_SALVE);
			i++;
		}
	} while (i < 256);

	for (i = 0; i < 256; i++)
	{
		printf("%3d", rddata_a[i]); // print write data
	}
	printf("\n");
	printf("flash read finish. \n");

	//----------- data check----------
	// 256 bytes
	for (i = 0; i < 256; i++)
	{
		if (wrdata_a[i] != rddata_a[i])
		{
			printf("FAIL: datas are not same!\r\n");
			return -1;
		}
	}
	printf("PASS: spi x1 write / read test success!\r\n");
#endif

#endif
	printf("test finish\r\n");
	while (1)
		;
}
