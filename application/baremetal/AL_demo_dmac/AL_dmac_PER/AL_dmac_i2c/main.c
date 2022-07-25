/*
 * main.c
 *
 *  Created on: Feb.11.2022
 *  Author: haitao.liu
 *  Function": e2prom write and read;
 */

#include <stdio.h>
#include "nuclei_sdk_soc.h"

uint8_t amount0;	//byte_data transfer number
uint32_t rdata0_0;	//
uint8_t ckdata0_0[64];	// check data array
uint8_t ckdata0_1[64];	// check data array
uint8_t tx_buf1[64];
void iic_data_cheak(uint32_t count){
	volatile uint8_t i = 0 ;
	for(i = 0 ; i < count ; i ++ ){
		if(tx_buf1[i] == (*(uint32_t*)(MEM_BASE2_ADDR + i*4)))
		{
			printf("MEM1_DATA = %3d",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
			printf("\r\n");
			printf("MEM2_DATA = %3d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
			printf("\r\n");
			printf("check data is right\r\n");
			}
		else{
			printf("MEM1_DATA = %3d",*(uint32_t*)(MEM_BASE1_ADDR + i*4));
			printf("\r\n");
			printf("MEM2_DATA = %3d",*(uint32_t*)(MEM_BASE2_ADDR + i*4));
			printf("\r\n");
			printf("check data is fail please check you dma set data\r\n");
		}

			printf("\r\n");
	}
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
	AlI2c_Disable(i2c);
	AlI2c_ClockSetup(i2c, I2C_CLOCK); // scl setup
	AlI2c_SetSpeedMode(i2c, I2c_speed_fast); // 400Kb/s
	AlI2c_SetMasterAddressMode(i2c, I2c_7bit_address); // 7-bit
	AlI2c_EnableMaster(i2c); //master
	AlI2c_SetTargetAddress(i2c,SLAVE_ADDRESS);//target addr
	//set extra options
	AlI2c_DmaInit(AL_DMAC_channel_0,i2c0,tx,0,0,Dmac_transfer_row1,16);
	AlI2c_IrqMask(i2c,TX_EMPTY_UNMASK|STOP_DET_UNMASK); // unmask TX_EMPTY/STOP_DET
	AlI2c_TxThr(i2c,amount0); // tx-fifo-threhold amount0
	AlI2c_SdaHoldTx(i2c,3);// tx-hold=3*ic_clk
	AlI2c_Spklen(i2c,4);//spike length=4*ic_clk
	AlI2c_IrqClrAll(i2c);//clear all interrupt
	AlI2c_Enable(i2c);
	//--------------------------------------
	// master-tx start: to page-write e2prom
	//--------------------------------------
	// tx e2prom word addr[15:0]
	AlI2c_Write(i2c, normal, 0x00);//send 1st word-addr byte [15:8]
	AlI2c_Write(i2c, normal, 0x00);//send 2nd word-addr byte [7:0]
	//
	AlDma_Enable(AL_DMAC);
	AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_0);
	AlI2c_IrqClrAll(i2c);// clr all int
	//wait MST_ACTIVITY returns to 0
	/*do{
			rdata0_0 = AlI2c_Status(i2c); // read intr stat
			printf("rdata0 = %08x\r\n",rdata0_0);
		}while((AlI2c_Status(i2c) & STOP_DET_UNMASK) == 0); // loop shen STOP_DET not detected*/
	while((AlI2c_Status(i2c) & BIT_MST_ACTIVITY) != 0); // until master is completed
	while(AlDma_CheckChannelBusy(AL_DMAC));
	AlDma_Disable(AL_DMAC);
	AlDma_DisableChannel(AL_DMAC,AL_dmac_channel_num_0);
	/*tWR-write cycle,
	while the data word is being programmed into the nonvolatile
	EEPROM. All inputs are disabled during this write cycle and the EEPROM will not respond until the write is
	complete.
	*/
	do{
		delay_cnt++;
	}while(delay_cnt < 3000); // delay > 10ms
	printf("DMA tx complete\r\n");
#if 1
	//--------------------------------------
	// master-rx start: to sequential read e2prom
	//--------------------------------------
	// MASTER-RX config
	AlI2c_Disable(i2c);
	AlI2c_ClockSetup(i2c, I2C_CLOCK); // scl setup
	AlI2c_SetSpeedMode(i2c, I2c_speed_fast); // 400Kb/s
	AlI2c_SetMasterAddressMode(i2c, I2c_7bit_address); // 7-bit
	AlI2c_DmaInit(AL_DMAC_channel_0,i2c0,rx,0,0,Dmac_transfer_row1,15);
	AlI2c_EnableMaster(i2c); //master
	//set extra options
	AlI2c_IrqMask(i2c,TX_EMPTY_UNMASK|STOP_DET_UNMASK|RX_FULL_UNMASK); // unmask TX_EMPTY/STOP_DET/RX_FULL
	AlI2c_TxThr(i2c,amount0); // tx-fifo-threhold amount0
	AlI2c_RxThr(i2c,amount0-1); // rx-fifo-threhold amount0
	AlI2c_SdaHoldTx(i2c,5);// tx-hold=5*ic_clk
	AlI2c_Spklen(i2c,5);//spike length=5*ic_clk
	AlI2c_IrqClrAll(i2c);//clear all interrupt
	AlI2c_Enable(i2c);
//	//send e2prom data read addr
//	i2c_write(i2c, normal, 0x00);//send addr
	//send e2prom data read addr[15:0]
	AlI2c_Write(i2c, normal, 0x00);//send 1st word-addr byte [15:8]
	AlI2c_Write(i2c, normal, 0x00);//send 2nd word-addr byte [7:0]
	data_cnt0 = 0;
		do{
			rdata0_0 =  AlI2c_IrqStat(i2c); // read intr stat
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
								AlI2c_IssueRead(i2c, stop);//last read CMD
								break;
							}
							else
							{
								AlI2c_IssueRead(i2c, normal);//tx read CMD
							}
						}
					}
				}
			}
			printf("wait read CMD...\r\n");
		}while((rdata0_0 & STOP_DET_UNMASK) == 0);// loop shen STOP_DET not detected

	printf("read has been completed\r\n");
	//dw_dmac_enable(AL_DMAC);
	//dw_dmac_enableChannel(AL_DMAC,dw_dmac_channel_num_0);
	//RX_FULL detected
	rdata0_0 = AlI2c_IrqStat(i2c); // read intr stat
	if((rdata0_0 & RX_FULL_UNMASK) != 0)
		{
			AlDma_Enable(AL_DMAC);
			AlDma_EnableChannel(AL_DMAC,AL_dmac_channel_num_0);
		}
	AlI2c_IrqClrAll(i2c);//clear all interrupt
	//wait MST_ACTIVITY returns to 0
	while((AlI2c_Status(i2c) & BIT_MST_ACTIVITY) != 0); // until master is completed
	while(AlDma_CheckChannelBusy(AL_DMAC));
	AlDma_Disable(AL_DMAC);
	AlDma_DisableChannel(AL_DMAC,AL_dmac_channel_num_0);
#endif
	AlI2c_Disable(i2c);
	printf("DMA IIC rx complete\r\n");
	return 0;
}


/*
 * main
 */
int main(void)
{
	int retval = 0;
	uint32_t buffer[64];
	for (volatile uint8_t i = 0 ;i < 64 ; i++)
		{
			buffer[i] = 0;
		}
	Enablepinmux1();
	write_To_OCM((uint32_t *)buffer,64,(uint32_t*)MEM_BASE1_ADDR);
	write_To_OCM((uint32_t *)buffer,64,(uint32_t*)MEM_BASE2_ADDR);
	for (volatile uint8_t i = 0 ;i < 16 ; i++)
		{
			tx_buf1[i] = i;
			if(15 == i){
				tx_buf1[i] = i|(uint32_t)0x00000200;
			}
		}
	printf("* Example: DMA_e2prom write and read test.\n");
    // poll-driven master-transmitter example
	write_iic_data(normal,16);
    i2c_e2prom(I2C0);
    iic_data_cheak(16);
}
