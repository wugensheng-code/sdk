/*
 * al_i2c.h
 *
 *  Created on: Feb.10.2022
 *      Author: haitao.liu
 */


#ifndef _AL_I2C_H_
#define _AL_I2C_H_

#include "demosoc.h"

/****************mcaro******************/
#define SIMULATION

// I2C clocks in MHz.
#ifdef SIMULATION
	#define I2C_CLOCK 100
#else
	#define I2C_CLOCK 20
#endif

// master and slave I2C bus addresses
// 24XX08 Device Address format:bit[6:3]=1010 constant,bit[2:0] for block-selected;
// bit6  bit5  bit4  bit3  bit2  bit1  bit0
//   1     0     1     0     X     B1   B2
#define MASTER_ADDRESS          0x50
#define SLAVE_ADDRESS           0x50

/* bits definitions */
#define I2C_ENABLE 				1   /*Enable I2C*/
#define I2C_DISABLE 			0	/*Disable I2C*/
#define I2C_MAS_ENABLE 			1   /*I2C master mode Enable*/
#define I2C_MAS_DISABLE 		0	/*I2C master mode Disable*/
#define I2C_SLV_DISABLE_DIS 	1   /*I2C slave mode Disable*/
#define I2C_SLV_DISABLE_EN 		0	/*I2C slave mode Enable*/
#define CMD_WRITE				(0<<8)   /*I2C Master Write Command*/
#define CMD_READ				(1<<8)   /*I2C Master Read Command*/
#define STOP_ENABLE				(1<<9)   /*I2C Master Issue STOP after this command*/
#define STOP_DISABLE			(0<<9)   /*I2C Master Do not Issue STOP after this command*/
#define RESTART_ENABLE			(1<<10)   /*I2C Master Issue RESTART before this command*/
#define RESTART_DISABLE			(0<<10)   /*I2C Master Do not Issue RESTART before this command*/

#ifndef true
#define true  1
#endif

#ifndef false
#define false 0
#endif

#define RxFullSclHoldLowEn			1
#define RxFullSclHoldLowDis			0
#define BUS_CLEAR_FEATURE_ENABLE	1
#define BUS_CLEAR_FEATURE_DISABLE	0
/*
 * These minimum high and low times are in nanoseconds.  They represent
 * the minimum amount of time a bus signal must remain either high or
 * low to be interpreted as a logical high or low as per the I2C bus
 * protocol.  These values are used in conjunction with an I2C input
 * clock frequency to determine the correct values to be written to the
 * clock count registers.
 */
#define SS_MIN_SCL_HIGH         4000
#define SS_MIN_SCL_LOW          4700
#define FS_MIN_SCL_HIGH         600
#define FS_MIN_SCL_LOW          1300
#define HS_MIN_SCL_HIGH_100PF   60
#define HS_MIN_SCL_LOW_100PF    120

//bit definition:int mask
#define TX_EMPTY_UNMASK			(1<<4)
#define RX_FULL_UNMASK			(1<<2)
#define RD_REQ_UNMASK			(1<<5)
#define STOP_DET_UNMASK			(1<<9)
#define TX_ABRT_UNMASK			(1<<6)

//i2c status BIT define
#define BIT_ACTIVITY				(1<<0) //I2C is active;
#define BIT_TFNF					(1<<1) //Tx FIFO not full
#define BIT_TFE						(1<<2) //Tx FIFO is empty
#define BIT_RFNE					(1<<3) //Rx FIFO not empty;
#define BIT_RFF						(1<<4) //Rx FIFO is full
#define BIT_MST_ACTIVITY			(1<<5) //Master activity(not idle)
#define BIT_SLV_ACTIVITY			(1<<6) //Slave not idle
#define BIT_MST_HOLD_TX_FIFO_EMPTY	(1<<7)
#define BIT_MST_HOLD_RX_FIFO_FULL	(1<<8)
#define BIT_SLV_HOLD_TX_FIFO_EMPTY  (1<<9)
#define BIT_SLV_HOLD_RX_FIFO_FULL	(1<<10)
/****d* i2c.data/dw_i2c_speed_mode
 * DESCRIPTION
 *  This is the data type used for setting and getting the speed mode.
 *  It is also used when specifying the scl count values.
 * NOTES
 *  This data type relates to the following register bit field(s):
 *   - ic_con/speed
 *   - ic_ss_scl_lcnt/ic_ss_scl_lcnt
 *   - ic_ss_scl_hcnt/ic_ss_scl_hcnt
 *   - ic_fs_scl_lcnt/ic_fs_scl_lcnt
 *   - ic_fs_scl_hcnt/ic_fs_scl_hcnt
 *   - ic_hs_scl_lcnt/ic_hs_scl_lcnt
 *   - ic_hs_scl_hcnt/ic_hs_scl_hcnt
 * SEE ALSO
 *  dw_i2c_setSpeedMode(), dw_i2c_getSpeedMode(), dw_i2c_setSclCount(),
 *  dw_i2c_getSclCount()
 * SOURCE
 */
enum i2c_speed_mode {
    I2c_speed_standard = 0x1,   // standard speed (100 kbps)
    I2c_speed_fast = 0x2,       // fast speed (400 kbps)
    I2c_speed_high = 0x3        // high speed (3400 kbps)
};

enum i2c_address_mode {
    I2c_7bit_address = 0x0,     // 7-bit address mode.  Only the 7 LSBs
                                // of the slave and/or target address
                                // are relevant.
    I2c_10bit_address = 0x1     // 10-bit address mode.  The 10 LSBs of
                                // the slave and/or target address are
                                // relevant.
};

enum i2c_scl_phase{
	I2c_scl_low = 0x0,          // SCL clock count low phase
	I2c_scl_high = 0x1          // SCL clock count high phase
};

enum i2c_cmd_type{
	normal = 0x0,
	stop = 0x1,
	restart = 0x2,
	stop_restart = 0x3
};



/*Func Declaration*/
void AlI2c_Disable(I2C_TypeDef *i2c);
void AlI2c_Enable(I2C_TypeDef *i2c);
void AlI2c_SetSclCount(I2C_TypeDef *i2c,enum i2c_speed_mode mode, enum i2c_scl_phase phase, uint16_t count);
void AlI2c_SetSpeedMode(I2C_TypeDef *i2c,enum i2c_speed_mode mode);
void AlI2c_SetMasterAddressMode(I2C_TypeDef *i2c,enum i2c_address_mode mode);
void AlI2c_SetSlaveAddressMode(I2C_TypeDef *i2c,enum i2c_address_mode mode);
void AlI2c_EnableMaster(I2C_TypeDef *i2c);
void AlI2c_EnableSlave(I2C_TypeDef *i2c);
void AlI2c_SetTargetAddress(I2C_TypeDef *i2c, uint16_t address);
void AlI2c_SetSlaveAddress(I2C_TypeDef *i2c, uint16_t address);
void AlI2c_Write(I2C_TypeDef *i2c, enum i2c_cmd_type type, uint8_t data);
BOOL AlI2c_IsRxFifoEmpty(I2C_TypeDef *i2c);
BOOL AlI2c_IsTxFifoEmpty(I2C_TypeDef *i2c);
uint8_t AlI2c_Read(I2C_TypeDef *i2c);
void AlI2c_IssueRead(I2C_TypeDef *i2c, enum i2c_cmd_type type);
void AlI2c_IrqMask(I2C_TypeDef *i2c, uint32_t mask);
void AlI2c_TxThr(I2C_TypeDef *i2c, uint8_t waterlevel);
void AlI2c_RxThr(I2C_TypeDef *i2c, uint8_t waterlevel);
void AlI2c_SdaHoldRx(I2C_TypeDef *i2c, uint8_t value);
void AlI2c_SdaHoldTx(I2C_TypeDef *i2c, uint16_t value);
void AlI2c_SdaSetup(I2C_TypeDef *i2c, uint8_t value);
void AlI2c_Spklen(I2C_TypeDef *i2c, uint8_t value);
void AlI2c_IrqClrAll(I2C_TypeDef *i2c);
uint32_t AlI2c_IrqStat(I2C_TypeDef *i2c);
uint32_t AlI2c_Status(I2C_TypeDef *i2c );
void AlI2c_ClockSetup(I2C_TypeDef *i2c, unsigned ic_clk);
void AlI2c_RxFullSclHoldcfg(I2C_TypeDef *i2c, uint8_t value);
void AlI2c_BusClrCfg(I2C_TypeDef *i2c, uint8_t value);
void AlI2c_SclTimeoutCfg(I2C_TypeDef *i2c, uint32_t value);
void AlI2c_SdaTimeoutCfg(I2C_TypeDef *i2c, uint32_t value);
void AlI2c_SdaStuckRecvEnCfg(I2C_TypeDef *i2c, uint8_t value);

#endif /* _AL9000_I2C_H_ */
