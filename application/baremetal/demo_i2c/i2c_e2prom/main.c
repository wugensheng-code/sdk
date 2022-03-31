/*
 * main.c
 *
 *  Created on: Feb.11.2022
 *  Author: haitao.liu
 *  Function": e2prom write and read;
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"

uint8_t amount0;	//byte_data transfer number
uint32_t rdata0_0;	//
uint8_t ckdata0_0[64];	// check data array
uint8_t ckdata0_1[64];	// check data array

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
		printf("PASS: test success!\r\n");
    	while(1);
    }

}
