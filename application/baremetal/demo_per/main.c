/*
 * main.c
 *
 *  Created on: 2022/3/20
 *  Author: pang.wei
 *  Function": demo_per
 */
#if 1
#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"

#define FLASH_RDID
#define FLASH_ERASE
#define FLASH_X1
#define FLASH_X2
#define FLASH_X4

//#define S25FL512S_ID	0X00200201  // flash ID
#define S25FL256S_ID	0x00190201  // flash ID
uint8_t amount0;	//byte_data transfer number
uint32_t rdata0_0;	//
uint8_t ckdata0_0[64];	// check data array
uint8_t ckdata0_1[64];	// check data array
volatile uint16_t count = 0;
void SOC_DMA_AHB_HANDLER(void)
{
    //printf("enter_irq_number%d\n\r",count);
    printf("enter_irq_sucess\n\r");
    printf("DMAC_MEM2MEM_SINGLE_MODE has been completed\n\r");
	 for(int i = 0 ; i<10; i++)
	 {
	 	//printf("ocm_data1 = %.2x\r\n",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
	 	//printf("ocm_data2 = %.2x\r\n",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
    	if((*(uint32_t*)(MEM_BASE1_ADDR + i*4)) == (*(uint32_t*)(MEM_BASE2_ADDR + i*4)))
    	{
    		printf("dmac_transmissiom_pass\r\n");
    	}
    	else
    	{
    		printf("dmac_transmissiom_fail\r\n");
    	}
	 }
           dw_dmac_clearIrq(AL9000_DMAC);
}
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

/****************function definition******************/
/*
* master transmitter:7-bit,master,400Kb/s,FS_CNT
* master receiver:7-bit,master,100Kb/s,SS_CNT
*/
int i2c_e2prom(I2C_TypeDef *i2c)
{
	amount0 =15;
	uint8_t index0 = 0;
	uint8_t data_cnt0 = 0;//record data number has been transfered
	uint32_t delay_cnt = 0;
	// MASTER-TX config
	i2c_disable(i2c);
	i2c_clock_setup(i2c, I2C_CLOCK); // scl setup
	i2c_setSpeedMode(i2c, I2c_speed_fast); // 400Kb/s
	i2c_setMasterAddressMode(i2c, I2c_7bit_address); // 7-bit
	i2c_enableMaster(i2c); //master
	i2c_setTargetAddress(i2c,SLAVE_ADDRESS);//target addr
	//set extra options
	i2c_irq_mask(i2c,TX_EMPTY_UNMASK|STOP_DET_UNMASK); // unmask TX_EMPTY/STOP_DET
	i2c_txThr(i2c,amount0); // tx-fifo-threhold amount0
	i2c_sdaHold_tx(i2c,3);// tx-hold=3*ic_clk
	i2c_spklen(i2c,4);//spike length=4*ic_clk
	i2c_irq_clrAll(i2c);//clear all interrupt
	i2c_enable(i2c);

	//--------------------------------------
	// master-tx start: to page-write e2prom
	//--------------------------------------
	// tx e2prom word addr[15:0]
	i2c_write(i2c, normal, 0x00);//send 1st word-addr byte [15:8]
	i2c_write(i2c, normal, 0x00);//send 2nd word-addr byte [7:0]
	// tx page-write data from 0 to amount0-1
	do{
		rdata0_0 = i2c_irq_stat(i2c); // read intr stat
		//STOP_DET not detected
		if((rdata0_0 & STOP_DET_UNMASK) == 0)
		{
			//i2c tx operation
			if(data_cnt0 < amount0)
			{
				//TX_EMPTY detected
				if((rdata0_0 & TX_EMPTY_UNMASK) != 0)
				{
					//write 4 data into tx-fifo
					for(int i = 0; i < amount0; i++)
					{
						data_cnt0++;
						//store sent data into array for data check
						ckdata0_0[data_cnt0-1] = data_cnt0-1;
						//the last data byte
						if(data_cnt0 == amount0)
						{
							i2c_write(i2c, stop, data_cnt0-1);//last data
							break;
						}
						else
						{
							i2c_write(i2c, normal, data_cnt0-1);//tx data
						}
					}
				}
			}
		}

	}while((i2c_irq_stat(i2c) & STOP_DET_UNMASK) == 0); // loop shen STOP_DET not detected
	i2c_irq_clrAll(i2c);// clr all int
	//wait MST_ACTIVITY returns to 0
	while((i2c_status(i2c) & BIT_MST_ACTIVITY) != 0); // until master is completed
	/*tWR-write cycle,
	while the data word is being programmed into the nonvolatile
	EEPROM. All inputs are disabled during this write cycle and the EEPROM will not respond until the write is
	complete.
	*/
	do{
		delay_cnt++;
	}while(delay_cnt < 3000); // delay > 10ms

	//--------------------------------------
	// master-rx start: to sequential read e2prom
	//--------------------------------------
	// MASTER-RX config
	i2c_disable(i2c);
	i2c_clock_setup(i2c, I2C_CLOCK); // scl setup
	i2c_setSpeedMode(i2c, I2c_speed_fast); // 400Kb/s
	i2c_setMasterAddressMode(i2c, I2c_7bit_address); // 7-bit
	i2c_enableMaster(i2c); //master
	//set extra options
	i2c_irq_mask(i2c,TX_EMPTY_UNMASK|STOP_DET_UNMASK|RX_FULL_UNMASK); // unmask TX_EMPTY/STOP_DET/RX_FULL
	i2c_txThr(i2c,amount0); // tx-fifo-threhold amount0
	i2c_rxThr(i2c,amount0-1); // rx-fifo-threhold amount0
	i2c_sdaHold_tx(i2c,5);// tx-hold=5*ic_clk
	i2c_spklen(i2c,5);//spike length=5*ic_clk
	i2c_irq_clrAll(i2c);//clear all interrupt
	i2c_enable(i2c);

//	//send e2prom data read addr
//	i2c_write(i2c, normal, 0x00);//send addr
	//send e2prom data read addr[15:0]
	i2c_write(i2c, normal, 0x00);//send 1st word-addr byte [15:8]
	i2c_write(i2c, normal, 0x00);//send 2nd word-addr byte [7:0]

	//send read CMD
	data_cnt0 = 0;
	do{
		rdata0_0 = i2c_irq_stat(i2c); // read intr stat
		//STOP_DET not detected
		if((rdata0_0 & STOP_DET_UNMASK) == 0)
		{
			//i2c tx operation
			if(data_cnt0 < amount0)
			{
				//TX_EMPTY detected
				if((rdata0_0 & TX_EMPTY_UNMASK) != 0)
				{
					//write amount0 read CMD into tx-fifo
					for(int i = 0; i < amount0; i++)
					{
						data_cnt0++;
						//the last data byte
						if(data_cnt0 == amount0)
						{
							i2c_issueRead(i2c, stop);//last read CMD
							break;
						}
						else
						{
							i2c_issueRead(i2c, normal);//tx read CMD
						}
					}
				}
			}
		}
	}while((rdata0_0 & STOP_DET_UNMASK) == 0);// loop shen STOP_DET not detected

	//RX_FULL detected
	rdata0_0 = i2c_irq_stat(i2c); // read intr stat
	if((rdata0_0 & RX_FULL_UNMASK) != 0)
	{
		//read 2 data from rx-fifo
		for(int i = 0; i < amount0; i++)
		{
			ckdata0_1[index0] = i2c_read(i2c); // read rx-fifo data
			index0++;
		}
	}

	i2c_irq_clrAll(i2c);//clear all interrupt
	//wait MST_ACTIVITY returns to 0
	while((i2c_status(i2c) & BIT_MST_ACTIVITY) != 0); // until master is completed

//	int i=0;
//	for(i=0;i<amount0;i++)
//	{
//		printf("%3d",ckdata0_1[i]); // print write data
//	}
//	printf("\r\n");
	//--------------------------------------
	// data check
	//--------------------------------------
	for(int i=0; i < amount0; i++)
	{
		if(ckdata0_0[i] != ckdata0_1[i])
		{
			return -1;
		}
	}

	i2c_disable(i2c);
	return 0;
}


/*
 * main
 */
int main(void)
{
	int retval = 0;
	/*variable init*/
	uint16_t i = 0;
	uint8_t wrdata_a[256] = {0};
	uint8_t rddata_a[256] = {0};
	uint8_t tx_buf1[64];
		uint8_t rx_buf1[20];
		uint8_t tx_buf2[64];
		uint8_t rx_buf2[20];

		for (volatile uint8_t i = 0 ;i < 20 ; i++)
		{
			tx_buf1[i]=i;
		}

		for (volatile uint8_t i = 0 ;i < 20 ; i++)
		{
			tx_buf2[i]=i%2;
		}
	__RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    ECLIC_Register_IRQ(SOC_INT65_IRQn, ECLIC_NON_VECTOR_INTERRUPT,ECLIC_LEVEL_TRIGGER, 1, 1,SOC_DMA_AHB_HANDLER);
    __enable_irq();
	for(i=0; i<256; i++)
	    {
	    	wrdata_a[i] = i;
	    }
	printf("***************************IIC TEST*****************************\r\n");
	printf("* Example: e2prom write and read test.\n");
    // poll-driven master-transmitter example
    retval = i2c_e2prom(I2C0);

    if(retval == -1)
    {
		printf("FAIL: data are not same!\r\n");
    	return -1;
    }
    else
    {
    	printf("Read and write data are the same.\r\n");
		printf("PASS: IIC e2prom test success!\r\n");
    	//while(1);
    }
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");
    printf("\r\n");
    printf("***************************QSPI TEST*****************************\r\n");
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

    	/************************ flash X4 ****************************/
    #ifdef FLASH_X4
    	/***************************************************************/
    	/*********************X4 mode enable (QE=1) ********************/
    	/***************************************************************/
    	printf("X4 mode enable\r\n");
    	//send WREN
    	flash_write_enable();
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
    	qspi_data_transmit(QSPI0,0x02); // QE,write configuration register-quad=1
    	/*check status--wait busy returns to idle*/
    	while(!qspi_sr_tfe(QSPI0)); // wait TFE returns to 1
    	while(qspi_sr_busy(QSPI0));  // check busy or idle,wait BUSY returns to 0
    	/******************** check flash status(WIP)*********************/
    	flash_wait_wip();
    	/******************** flash write enable*********************/
    	//send WREN
    	flash_write_enable();
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
    		qspi_data_transmit(QSPI0,wrdata_a[i]); // tx data wrdata_a[i]
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
    			rddata_a[i]= qspi_data_read(QSPI0);
    			i++;
    		}
    	}while(i<256);
    	printf("flash x4-mode read finish.\r\n");

    	//----------- data check----------
    	// 256 bytes
    	for(i=0; i<256; i++)
    	if(wrdata_a[i] != rddata_a[i])
    	{
    		printf("FAIL: datas are not same!\r\n");
    		return -1;
    	}
    	printf("PASS: qspi x4-mode write/read test success!\r\n");
    	printf("Qspi X4 test finish\r\n");

    #endif
        printf("\r\n");
        printf("\r\n");
        printf("\r\n");
        printf("\r\n");
        printf("***************************DMA TEST*****************************\r\n");
        dw_dmac_enableChannelIrq(AL9000_DMAC_channel_0);
        dw_dmac_unmaskIrq(AL9000_DMAC,dw_dmac_channel_num_1,tfr_l);
        dw_dmac_setAddress(AL9000_DMAC_channel_0,Dmac_src,MEM_BASE1_ADDR);
        dw_dmac_setAddress(AL9000_DMAC_channel_0,Dmac_dst,MEM_BASE2_ADDR);
        dw_dmac_setChannelConfig(AL9000_DMAC_channel_0);
        dw_dmac_setTransferType(AL9000_DMAC_channel_0,Dmac_transfer_row1);
        dw_dmac_setMemPeriphFlowCtl(AL9000_DMAC_channel_0,MEM2MEM);
        dw_dmac_setTransWidth(AL9000_DMAC_channel_0,Dmac_src_dst,WIDTH_32);
        dw_dmac_setBlockTransSize(AL9000_DMAC_channel_0,10);
        dw_dmac_setBurstTransLength(AL9000_DMAC_channel_0,Dmac_src_dst,DW_DMA_MSIZE_8);
        dw_dmac_setMstSelect(AL9000_DMAC_channel_0,Dmac_src_dst,SMS_master1,DMS_master1);
        dw_dmac_setAddressInc(AL9000_DMAC_channel_0,Dmac_src_dst,Increments);
        dw_dmac_setHandshakingMode(AL9000_DMAC_channel_0,Dmac_src_dst,Hardware_handshaking);
        dw_dmac_setHsPolarity(AL9000_DMAC_channel_0,Dmac_src_dst,ACTIVE_HIGH);
        dw_dmac_setFifoMode(AL9000_DMAC_channel_0,FIFO_MODE_0);
        dw_dmac_setFlowCtlMode(AL9000_DMAC_channel_0,fc_mode_0);
        volatile uint32_t buffer[128];
        for (volatile uint32_t i = 0 ; i < 10 ; i++)
        	 {
        		 buffer[i] = i;
        	 }
        	 write_To_OCM((uint32_t*)buffer,10,(uint32_t*)MEM_BASE1_ADDR);
        	 dw_dmac_enable(AL9000_DMAC);
        	 dw_dmac_enableChannel(AL9000_DMAC,dw_dmac_channel_num_0);

        	 printf("DMA test success\r\n");
        	 printf("\r\n");
        	 printf("\r\n");
        	 printf("\r\n");
        	 printf("\r\n");
        	 printf("***************************CAN TEST*****************************\r\n");
        	 set_reset_mode(AL9000_CAN0);
        	 canfd_device_driver_bittime_configuration(AL9000_CAN0,rate_5Mbit,AL9000_TOP0,can);
        	 can_tx_mode(AL9000_CAN0,IN_LOOPBACK);
        	 CAN_TransmitStatus(AL9000_CAN0);
        	 Can_Send_Msg(AL9000_CAN0,tx_buf1,XMIT_PTB_MODE,data_length_8,0x10);
        	 Can_Receive_Msg(AL9000_CAN0,rx_buf1,data_length_8);
        	 for (volatile uint8_t i = 0 ;i < 8 ; i++)
        	 	{
        	 		if(tx_buf1[i] == rx_buf1[i]){
        	 			printf("can receive data is right\r\n");
        	 		}
        	 		else{printf("can test fail\r\n");}
        	 	}
        	 CAN_TEST_error(AL9000_CAN0);
        	 printf("can test success \r\n");
        while(1);
}
#endif
