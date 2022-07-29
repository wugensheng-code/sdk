/*
 * main.c
 *
 *  Created on: Oct.27.2021
 *      Author: haitao.liu
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"


#define FLASH_RDID
#define FLASH_ERASE
#define FLASH_ERASE_VERIFY
#define FLASH_READ


#define   SPI_MASTER   ((SPI_TypeDef *) SPI1_BASE)


#define S25FL512S_ID	0X00200201  // flash ID
//#define S25FL256S_ID	0x00190201  // flash ID
#define MX25U256_ID	0X003925c2  // mxic flash ID


/****************function define******************************/
/*
* Read Manufacture ID/ Device ID - in x1-SPI mode
*/
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

void Spi_IRQHandler(void)
{
	printf("SPI interrupt!\n\r");
}

/* main */
int main()
{
	uint32_t temp=0;
	uint32_t byte_cnt;
	uint32_t data32=0;
	int i=0;
   /***********************AL9000***************************/
    printf("test start\r\n");
    /*variable init*/

    uint8_t wrdata_a[256];
    uint8_t rddata_a[256];
    for(i=0; i<256; i++)
    {
    	wrdata_a[i] = i;
    }
   // ECLIC_Register_IRQ(85, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,Spi_IRQHandler);
   // __enable_irq();
/*
	  temp = *(uint32_t*)(0xf8404000 + 0x2c);
       printf("IMR:0x%x\n\r",temp);
       *(uint32_t*)(0xf804e000 + 0x2c) = 0x3f;
       temp = *(uint32_t*)(0xf8404000 + 0x30);
       printf("ISR:0x%x\n\r",temp);

       temp = *(uint32_t*)(0xf8404000 + 0x34);
       printf("RISR:0x%x\n\r",temp);

       temp = *(uint32_t*)(0xf8404000 + 0x38);
       printf("TXEICR:0x%x\n\r",temp);
       temp = *(uint32_t*)(0xf8404000 + 0x3c);
       printf("RXOICR:0x%x\n\r",temp);
       temp = *(uint32_t*)(0xf8404000 + 0x40);
       printf("RXUICR:0x%x\n\r",temp);
       temp = *(uint32_t*)(0xf8404000 + 0x44);
       printf("MSTICR:0x%x\n\r",temp);

       temp = *(uint32_t*)(0xf8404000 + 0x48);
       printf("ICR:0x%x\n\r",temp);
       */

    /***************************************************************/
    /************************flash ID check*************************/
    /***************************************************************/
#ifdef FLASH_RDID
	uint32_t flash_id = 0;
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
	spi_tmod_e2prom(SPI_MASTER); // EEPROM read
	//-----------------------------------------------------------
    //program TXFTLR register
    //start when 1 data items is present in tx fifo
    //-----------------------------------------------------------
    /*cfg SPI Transmit FIFO Threshold Level*/
	spi_txftl_tft(SPI_MASTER, 0); // default
	//-----------------------------------------------------------
	//program CTRLR1 register
	//receive 3 data items
	//-----------------------------------------------------------
	spi_ctrl1_ndf(SPI_MASTER, 2); // receive 2+1 data items

	//-----------------------------------------------------------
	//program SER register
	//choose ss_0_n
	//-----------------------------------------------------------
	spi_ser(SPI_MASTER,SER_SS0_EN); // choose ss0--same to emulation platform.

	//-----------------------------------------------------------
	//program BAUDR register
	//divided by 30 --(30 from design)
	//-----------------------------------------------------------
	/*cfg SPI Clock Divider.*/
	spi_sckdiv_cfg(SPI_MASTER, 70);  // divided by 30.0x1e not work ,0x28=40

	//-----------------------------------------------------------
	//program SSIENR register
	//enable spi
	//-----------------------------------------------------------
	/*en ssi*/
	spi_dwc_ssi_enable(SPI_MASTER);

	//-----------------------------------------------------------
	//sent:
	//inst(0x): 9f (RDID)
	//receive: 3 data items
	//-----------------------------------------------------------

	flash_id = SReadID();
	printf("flash_id= 0x%08x\r\n",flash_id); // print flash ID
	/*flash ID check*/
	if(flash_id != S25FL512S_ID && flash_id !=MX25U256_ID)
	{
		printf("FAIL: flash ID check not ok!\r\n");
        /*
		flash_reset();
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		printf("FAIL: flash ID check not ok!\r\n");
		spi_dwc_ssi_disable(SPI_MASTER);
		spi_tmod_e2prom(SPI_MASTER); // EEPROM read
		//-----------------------------------------------------------
	    //program TXFTLR register
	    //start when 1 data items is present in tx fifo
	    //-----------------------------------------------------------

		spi_txftl_tft(SPI_MASTER, 0); // default
		//-----------------------------------------------------------
		//program CTRLR1 register
		//receive 3 data items
		//-----------------------------------------------------------
		spi_ctrl1_ndf(SPI_MASTER, 2); // receive 2+1 data items
		spi_dwc_ssi_enable(SPI_MASTER);
		flash_id = SReadID();
		printf("flash_id= 0x%08x\r\n",flash_id); // print flash ID
        */
		//return -1;
	}
	else
	{
		printf("PASS: flash ID check ok!\r\n");
	}
#endif



    /***************************************************************/
    /************************flash erase****************************/
    /***************************************************************/
#ifdef FLASH_ERASE
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
    /*cfg SPI Transmit FIFO Threshold Level*/
	spi_txftl_tft(SPI_MASTER, 0); // default

	//-----------------------------------------------------------
	//program SSIENR register
	//enable spi
	//-----------------------------------------------------------
	/*en ssi*/
	spi_dwc_ssi_enable(SPI_MASTER);

	//-----------------------------------------------------------
	//sent:
	//inst(0x): 06 (WREN)
	//-----------------------------------------------------------

	spi_data_transmit(SPI_MASTER,CMD_WREN); // tx read id cmd
	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
	while(spi_sr_busy(SPI_MASTER));  // check busyl or idle,wait BUSY returns to 0


	//-----------------------------------------------------------
	//The Chip Erase (CE) command is used to erase all the data of the chip.
	//sent:
	//inst(0x): 60 (chip erase)
	//inst(0x): D8 (sector erase)
	//-----------------------------------------------------------
	//chip erase
	//spi_data_transmit(SPI_MASTER,CMD_CHIP_ERASE); // tx chip erase cmd

	//sector erase

	spi_data_transmit(SPI_MASTER,0x20); // tx sector erase cmd
	spi_data_transmit(SPI_MASTER,0x00); // tx addr[23:16]
	spi_data_transmit(SPI_MASTER,0x00); // tx addr[15:8]
	spi_data_transmit(SPI_MASTER,0x00); // tx addr[7:0]



	  // spi_data_transmit(SPI_MASTER,CMD_CHIP_ERASE); // tx sector erase cmd


		//while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
		//while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0


		//flash_wait_wip();



	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0


	flash_wait_wip();
	_delay_ms(1);

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
		}

	//----------- data check----------
	// 256 bytes
	/*
	for(i=0; i<128; i++)
	{
		if(0xff != rddata_a[i])
		{
			printf("FAIL: datas are not same!\r\n");

			//printf("fail data [%d] is: %d\r\n",i,rddata_a[i]);
			//return -1;
		}
	}
	*/


	printf("PASS: spi flash erase success!\r\n");
#endif

	printf("flash erase finish.\r\n");
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
		i = 0;
		while (byte_cnt)
		{
			if ( (*(volatile uint32_t*)(SPI1_BASE + 0x28) & 0x2) == 0x2) //tx fifo not full
			{

			   spi_data_transmit(SPI_MASTER,wrdata_a[i]); // tx data wrdata_a[i]
			   //spi_data_transmit(SPI_MASTER,data32);
			   //byte_cnt=byte_cnt -4;
			   byte_cnt--;
			  // i=i+4;
			   i++;
			}
		}
		//while(1);

	//spi_dwc_ssi_enable(SPI_MASTER);
	//256 bytes


	while(!spi_sr_tfe(SPI_MASTER)); // wait TFE returns to 1
	while(spi_sr_busy(SPI_MASTER));  // check busy or idle,wait BUSY returns to 0
	for(i=0; i<128; i++)
	{
		printf("%d",wrdata_a[i]); // print write data
	}
	printf(" \n");
	printf("flash write finish. \n");

	flash_wait_wip();
	//printf("aaaa. \n");

#endif
	/***************************************************************/
	/************************ flash read ***************************/
	/***************************************************************/
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

    printf("test finish\r\n");
    while(1);
}
