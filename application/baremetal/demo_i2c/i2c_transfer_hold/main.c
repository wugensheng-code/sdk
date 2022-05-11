/*
 * main.c
 *
 *  Created on: Feb.11.2022
 *  Author: haitao.liu
 *  Function":1. I2C0(master) transmit data to I2C1(slave),
 *  		   with SCL hold low, interrupt,10-bit;
 *  		  2. bus clear feature(SDA stuck at low)
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"
#include "board_nuclei_fpga_eval.h"

uint8_t amount0;	//byte_data transfer number
uint8_t rdata0_0;	//
uint8_t ckdata0_0[64];	// check data array
uint8_t ckdata0_1[64];	// check data array

/****************function definition******************/
/*
* master transmitter, slave receiver in 10-bit with SCL_hold_low
* test point: 10-bit;master-tx, slave-rx, interrupt, SCL_hold_low(mas tx-fifo empty, slave rx-fifo full)
*/
int i2c_transfer_hold(I2C_TypeDef *master,I2C_TypeDef *slave)
{
	amount0 = 20;
	uint8_t index0 = 0;
	uint8_t data_cnt0 = 0;//record data number has been transfered
	// MASTER-TX config
	AlI2c_Disable(master);
	AlI2c_ClockSetup(master, I2C_CLOCK); // scl setup
	AlI2c_SetSpeedMode(master, I2c_speed_fast); // 400Kb/s
	AlI2c_SetMasterAddressMode(master, I2c_10bit_address); // 10-bit
	AlI2c_EnableMaster(master); //master
	AlI2c_SetTargetAddress(master,SLAVE_ADDRESS);//target addr
	//set extra options
	AlI2c_IrqMask(master,TX_EMPTY_UNMASK|STOP_DET_UNMASK); // unmask TX_EMPTY/STOP_DET
	AlI2c_TxThr(master,8); // tx-fifo-threhold 8
	AlI2c_SdaHoldTx(master,3);// tx-hold=3*ic_clk
	AlI2c_Spklen(master,4);//spike length=4*ic_clk
	AlI2c_IrqClrAll(master);//clear all interrupt
	AlI2c_Enable(master);

	// SLAVE-RX config
	AlI2c_Disable(slave);
	AlI2c_SetSpeedMode(slave, I2c_speed_fast); // 400Kb/s
	AlI2c_SetSlaveAddressMode(slave, I2c_10bit_address); // 10-bit
	AlI2c_EnableSlave(slave); //slave
	AlI2c_SetSlaveAddress(slave,SLAVE_ADDRESS);//slave addr
	AlI2c_RxFullSclHoldcfg(slave, RxFullSclHoldLowEn);
	//set extra options
	AlI2c_IrqMask(master,RX_FULL_UNMASK|STOP_DET_UNMASK); // unmask RX_FULL/STOP_DET
	AlI2c_RxThr(slave,3); // tx-fifo-threhold 3+1
	AlI2c_SdaHoldTx(slave,3);// tx-hold=3*ic_clk
	AlI2c_Spklen(slave,4);//spike length=4*ic_clk
	AlI2c_IrqClrAll(slave);//clear all interrupt
	AlI2c_Enable(slave);

	//--------------------------------------
	// master-tx start: mas tx data to slv with SCL hold low
	//--------------------------------------
	AlI2c_Write(master, normal, 0x00);//send first data
	ckdata0_0[0] = 0; // store first data
	data_cnt0++;
	for(int i=0; i<20; i++)
	{
		if((AlI2c_Status(master) & BIT_MST_HOLD_TX_FIFO_EMPTY) != 0)
		{
			printf("SCL Hold Low(master tx-fifo empty) test PASS\n");
			break;
		}
		if(i==19)
		{
			printf("SCL Hold Low(master tx-fifo empty) test FAIL\n");
			return -1;
		}
	}

	// send the reset data from 0x0 to amount0-1
	do{
		rdata0_0 = AlI2c_IrqStat(master); // read intr stat
		//STOP_DET not detected
		if((rdata0_0 & STOP_DET_UNMASK) == 0)
		{
			//master tx operation
			if(data_cnt0 < amount0)
			{
				//TX_EMPTY detected
				if((rdata0_0 & TX_EMPTY_UNMASK) != 0)
				{
					//write 4 data into tx-fifo
					for(int i = 0; i < 4; i++)
					{
						data_cnt0++;
						//store sent data into array for data check
						ckdata0_0[data_cnt0-1] = data_cnt0-1;
						//the last data byte
						if(data_cnt0 == amount0)
						{
							AlI2c_Write(master, stop, data_cnt0-1);//last data
							break;
						}
						else
						{
							AlI2c_Write(master, normal, data_cnt0-1);//tx data
						}
					}
				}
			}
			//i2c rx operation
			//RX_FULL detected
			if((rdata0_0 & RX_FULL_UNMASK) != 0)
			{
				//read 4 data from rx-fifo
				for(int i = 0; i < 4; i++)
				{
					ckdata0_1[index0] = AlI2c_Read(slave); // read rx-fifo data
					index0++;
				}
			}
		}
	}while((rdata0_0 & STOP_DET_UNMASK) == 0); // loop shen STOP_DET not detected
	AlI2c_IrqClrAll(master);// clr all int
	AlI2c_IrqClrAll(slave);// clr all int
	//wait MST_ACTIVITY returns to 0
	while((AlI2c_Status(master) & BIT_MST_ACTIVITY) != 0); // until master is completed
	while((AlI2c_Status(slave) & BIT_SLV_ACTIVITY) != 0); // until slave is completed
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

	AlI2c_Disable(master);
	AlI2c_Disable(slave);
	return 0;
}

#if 1
/*
* e2prom write with SDA stuck at low
* test point: 7-bit;master-tx, slave-rx, interrupt, bus-clear-feature(SDA stuck at low)
*/
int i2c_bus_recovery(I2C_TypeDef *master)
{
	amount0 = 4;
	uint8_t index0 = 0;
	uint8_t data_cnt0 = 0;//record data number has been transfered
	// MASTER-TX config
	AlI2c_Disable(master);
	AlI2c_ClockSetup(master, I2C_CLOCK); // scl setup
	AlI2c_SetSpeedMode(master, I2c_speed_fast); // 400Kb/s
	AlI2c_SetMasterAddressMode(master, I2c_7bit_address); // 7-bit
	AlI2c_EnableMaster(master); //master
	AlI2c_SetTargetAddress(master,SLAVE_ADDRESS);//target addr
	//bus clear cfg
	AlI2c_BusClrCfg(master,BUS_CLEAR_FEATURE_ENABLE);//enable
	AlI2c_SclTimeoutCfg(master,1000); // SCL timeout 1000*ic_clk
	AlI2c_SdaTimeoutCfg(master,1500); // SCL timeout 1500*ic_clk
	//set extra options
	AlI2c_IrqMask(master,STOP_DET_UNMASK|TX_ABRT_UNMASK); // unmask STOP_DET|TX_ABRT
//	i2c_txThr(master,8); // tx-fifo-threhold 8
	AlI2c_SdaHoldTx(master,3);// tx-hold=3*ic_clk
	AlI2c_Spklen(master,4);//spike length=4*ic_clk
	AlI2c_IrqClrAll(master);//clear all interrupt
	AlI2c_Enable(master);

	//--------------------------------------
	// master-tx start: to page-write e2prom
	//--------------------------------------
	AlI2c_Write(master, normal, 0x00);//send addr
	//send page-write data 0x0 to test SDA stuck at low
	for(int i=0; i<amount0; i++)
	{
		//the last data byte
		if(i == amount0-1)
		{
			AlI2c_Write(master, stop, data_cnt0-1);//last data
			break;
		}
		else
		{
			AlI2c_Write(master, normal, data_cnt0-1);//tx data
		}
	}

	// SDA stuck at low test
	do{
		rdata0_0 = AlI2c_IrqStat(master); // read intr stat
	}while((rdata0_0 & STOP_DET_UNMASK)==0 && (rdata0_0 & TX_ABRT_UNMASK)==0);

	if((rdata0_0 & TX_ABRT_UNMASK)==0) // TX_ABORT not detected
	{
		printf("FAIL:SDA not stuck in low\n");
		return -1;
	}
	else// TX_ABORT detected
	{
		rdata0_0 = ((master->IC_TX_ABRT_SOURCE)>>17) & 1;// read ABRT_SDA_STUCK_AT_LOW
		if(rdata0_0 == 1)
		{
			printf("SDA stuck at low is detected\n");
			AlI2c_SdaStuckRecvEnCfg(master,0x1);//enable
			while(master->IC_ENABLE.SDA_STUCK_RECOVERY_ENABLE ==1);//until SDA_STUCK_RECOVERY_ENABLE is 0
			if(master->IC_STATUS.BITS.SDA_STUCK_NOT_RECOVERED ==0)
			{
				printf("SDA stuck at low recovery test PASS\n");
			}
			else
			{
				printf("SDA stuck at low recovery test FAIL\n");
				return -1;
			}
		}
		else
		{
			printf("SDA stuck at low is not detected\n");
			return -1;
		}
	}

	//wait MST_ACTIVITY returns to 0
	while((AlI2c_Status(master) & BIT_MST_ACTIVITY) != 0); // until master is completed
	AlI2c_IrqClrAll(master);// clr all int
	AlI2c_Disable(master);
	return 0;
}
#endif

/*
 * main
 */
int main(void)
{
	int retval = 0;
    printf("I2C test start\n");
    // poll-driven master-transmitter example
//    retval |= i2c_transfer_hold(I2C0, I2C1);
    //e2prom write with SDA stuck at low example
    retval |= i2c_bus_recovery(I2C0);

    if(retval == -1)
    {
    	printf("FAIL\n");
    	return -1;
    }
    else
    {
    	printf("PASS\n");
    	while(1);
    }

}
