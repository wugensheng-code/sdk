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

#define SPI_SLAVE_EN
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
	*(uint32_t *)(0xf8800160) = 0x1; //top enable spi slave
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
#ifdef SPI_SLAVE_EN
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
		sr = *(uint32_t *)(SPI0_BASE + 0x130);
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
	printf("Naver run to here\r\n");

}
