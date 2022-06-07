/*
 * al_i2c.c
 *
 *  Created on: Feb.10.2022
 *      Author: haitao.liu
 */
#include "demosoc.h"
#include "nuclei_sdk_soc.h"
/**************************function definition******************************/
/*!
    \brief      Disable I2C
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_Disable(I2C_TypeDef *i2c)
{
    i2c->IC_ENABLE.ENABLE = I2C_DISABLE;
}

/*!
    \brief      Enable I2C
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_Enable(I2C_TypeDef *i2c)
{
    i2c->IC_ENABLE.ENABLE = I2C_ENABLE;
}

/*!
    \brief      This function set the scl count value for a particular speed mode(standard, fast, high) and clock phase (low, high).
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SetSclCount(I2C_TypeDef *i2c,enum i2c_speed_mode
        mode, enum i2c_scl_phase phase, uint16_t count)
{
	if(mode == I2c_speed_high) {
		if(phase == I2c_scl_low)
			i2c->IC_HS_SCL_LCNT = count;
		else
			i2c->IC_HS_SCL_HCNT = count;
	}
	else if(mode == I2c_speed_fast) {
		if(phase == I2c_scl_low)
			i2c->IC_FS_SCL_LCNT = count;
		else
			i2c->IC_FS_SCL_HCNT = count;
	}
	else if(mode == I2c_speed_standard) {
		if(phase == I2c_scl_low)
			i2c->IC_SS_SCL_LCNT = count;
		else
			i2c->IC_SS_SCL_HCNT = count;
	}
	else
	{}
}

/*!
    \brief      setup I2C speed mode
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SetSpeedMode(I2C_TypeDef *i2c,enum i2c_speed_mode mode)
{
    i2c->IC_CON.SPEED = mode;
}

/*!
    \brief      setup I2C-master address mode
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SetMasterAddressMode(I2C_TypeDef *i2c,enum i2c_address_mode mode)
{
    i2c->IC_CON.IC_10BITADDR_MASTER = mode;
}

/*!
    \brief      setup I2C-slave address mode
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SetSlaveAddressMode(I2C_TypeDef *i2c,enum i2c_address_mode mode)
{
    i2c->IC_CON.IC_10BITADDR_SLAVE = mode;
}

/*!
    \brief      setup I2C as master
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_EnableMaster(I2C_TypeDef *i2c)
{
    i2c->IC_CON.MASTER_MODE = I2C_MAS_ENABLE;
}

/*!
    \brief      setup I2C as slave
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_EnableSlave(I2C_TypeDef *i2c)
{
    i2c->IC_CON.IC_SLAVE_DISABLE = I2C_SLV_DISABLE_EN;
}


/*!
    \brief 	This function sets the target address used by the I2C master.  When
 	 	 	not issuing a general call or using a start byte, this is the
 	 	 	address the master uses when performing transfers over the I2C bus.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SetTargetAddress(I2C_TypeDef *i2c, uint16_t address)
{
    i2c->IC_TAR.IC_TAR = address;
}

/*!
    \brief 	This function sets the slave address to which the I2C slave responds, when Enabled.
    \param[out]
    \retval
*/
void AlI2c_SetSlaveAddress(I2C_TypeDef *i2c, uint16_t address)
{
    i2c->IC_SAR.IC_SAR = address;
}

/*!
    \brief 	This function writes a single byte to the I2C transmit FIFO.
    \param[in]  i2c: struct of I2C; data: byte to write to FIFO
    \param[out]
    \retval
*/
void AlI2c_Write(I2C_TypeDef *i2c, enum i2c_cmd_type type, uint8_t data)
{
    if(type == normal)
    {
//        i2c->IC_DATA_CMD.DAT = data; //[7..0]
//        i2c->IC_DATA_CMD.CMD = I2C_MAS_WRITE; //[8]
//    	i2c->IC_DATA_CMD.STOP = 0x0;//[9]
//    	i2c->IC_DATA_CMD.RESTART = 0x0;//[10]
    	i2c->IC_DATA_CMD.all = RESTART_DISABLE|STOP_DISABLE|CMD_WRITE|data;
    }
    else if(type == stop)
    {
//    	i2c->IC_DATA_CMD.STOP = 0x1; //with stop
//    	i2c->IC_DATA_CMD.RESTART = 0x0;
    	i2c->IC_DATA_CMD.all = RESTART_DISABLE|STOP_ENABLE|CMD_WRITE|data;
    }
    else if(type == restart)
    {
//    	i2c->IC_DATA_CMD.STOP = 0x0;
//    	i2c->IC_DATA_CMD.RESTART = 0x1; // with restart
    	i2c->IC_DATA_CMD.all = RESTART_ENABLE|STOP_DISABLE|CMD_WRITE|data;
    }
    else if(type == stop_restart)
    {
//    	i2c->IC_DATA_CMD.STOP = 0x1; //with stop
//    	i2c->IC_DATA_CMD.RESTART = 0x1; // with restart
    	i2c->IC_DATA_CMD.all = RESTART_ENABLE|STOP_ENABLE|CMD_WRITE|data;
    }
    else
    { }
}

/*!
    \brief 	This function returns whether the receive FIFO is empty or not.
    \param[in]  i2c: struct of I2C
    \param[out] true-- the receive FIFO is empty;false-- the receive FIFO is not empty
    \retval
*/
BOOL AlI2c_IsRxFifoEmpty(I2C_TypeDef *i2c)
{
	BOOL retval;
    if(i2c->IC_STATUS.BITS.RFNE == 0)
    	retval = true;
    else
    	retval = false;
    return retval;
}

/*!
    \brief 	This function reads a single byte from the I2C receive FIFO.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
uint8_t AlI2c_Read(I2C_TypeDef *i2c)
{
	// read a byte from the DATA_CMD register
	return ((i2c->IC_DATA_CMD.BITS.DAT) & 0xff);
}

/*!
    \brief This function writes a read command to the I2C transmit FIFO.
    This is used during master-receiver/slave-transmitter transfers and is
    typically followed by a read from the master receive FIFO after the
    slave responds with data.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_IssueRead(I2C_TypeDef *i2c, enum i2c_cmd_type type)
{
    if(type == normal)
    {
    	i2c->IC_DATA_CMD.all = RESTART_DISABLE|STOP_DISABLE|CMD_READ;
    }
    else if(type == stop)
    {
    	i2c->IC_DATA_CMD.all = RESTART_DISABLE|STOP_ENABLE|CMD_READ;
    }
    else if(type == restart)
    {
    	i2c->IC_DATA_CMD.all = RESTART_ENABLE|STOP_DISABLE|CMD_READ;
    }
    else if(type == stop_restart)
    {
    	i2c->IC_DATA_CMD.all = RESTART_ENABLE|STOP_ENABLE|CMD_READ;
    }
    else
    { }
}

/*!
    \brief 	Masks specified I2C interrupt(s).
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_IrqMask(I2C_TypeDef *i2c,uint32_t mask)
{
	i2c->IC_INTR_MASK.all = mask;
}

/*!
    \brief 	Transmit FIFO Threshold Level register cfg.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_TxThr(I2C_TypeDef *i2c, uint8_t waterlevel)
{
	i2c->IC_TX_TL = waterlevel;
}

/*!
    \brief 	Receive FIFO Threshold Level register cfg.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_RxThr(I2C_TypeDef *i2c, uint8_t waterlevel)
{
	i2c->IC_RX_TL = waterlevel;
}

/*!
    \brief 	SDA Hold Time Length Register cfg - As receiver.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SdaHoldRx(I2C_TypeDef *i2c, uint8_t value)
{
	i2c->IC_SDA_HOLD.IC_SDA_RX_HOLD = value;
}

/*!
    \brief 	SDA Hold Time Length Register cfg - As transmitter.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SdaHoldTx(I2C_TypeDef *i2c, uint16_t value)
{
	i2c->IC_SDA_HOLD.IC_SDA_TX_HOLD = value;
}

/*!
    \brief 	SDA Setup Time Length Register cfg - only used in slave transmitter.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SdaSetup(I2C_TypeDef *i2c, uint8_t value)
{
	i2c->IC_SDA_SETUP = value;
}

/*!
    \brief 	I2C SS, FS or FM+ Spike Suppression Limit.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_Spklen(I2C_TypeDef *i2c, uint8_t value)
{
	i2c->IC_FS_SPKLEN = value;
}

/*!
    \brief 	Clears specified I2C interrupt(s).
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_IrqClrAll(I2C_TypeDef *i2c)
{
	uint8_t retval;
	retval = (i2c->IC_CLR_INTR.CLR_INTR);
}


/*!
    \brief 	I2C interrupt(s) status.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
uint32_t AlI2c_IrqStat(I2C_TypeDef *i2c)
{
	return (i2c->IC_INTR_STAT.all);
}

/*!
    \brief 	I2C status check.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
uint32_t AlI2c_Status(I2C_TypeDef *i2c )
{
	return (i2c->IC_STATUS.all);
}

// Utility function which programs the clock count registers for a given
// input clock frequency.
void AlI2c_ClockSetup(I2C_TypeDef *i2c, unsigned ic_clk)
{
    uint16_t ss_scl_high, ss_scl_low;
    uint16_t fs_scl_high, fs_scl_low;
    uint16_t hs_scl_high, hs_scl_low;

    // ic_clk is the clock speed (in MHz) that is being supplied to the
    // DW_apb_i2c device.  The correct clock count values are determined
    // by using this inconjunction with the minimum high and low signal
    // hold times as per the I2C bus specification.
    ss_scl_high = ((uint16_t) (((SS_MIN_SCL_HIGH * ic_clk) / 1000) + (25-7))); //+25 non-uniqueness
    ss_scl_low = ((uint16_t) (((SS_MIN_SCL_LOW * ic_clk) / 1000) + 25));
    fs_scl_high = ((uint16_t) (((FS_MIN_SCL_HIGH * ic_clk) / 1000) + (25-7)));
    fs_scl_low = ((uint16_t) (((FS_MIN_SCL_LOW * ic_clk) / 1000) + 25));
    hs_scl_high = ((uint16_t) (((HS_MIN_SCL_HIGH_100PF * ic_clk) / 1000) + (25-7)));
    hs_scl_low = ((uint16_t) (((HS_MIN_SCL_LOW_100PF * ic_clk) / 1000) + 25));

    AlI2c_SetSclCount(i2c, I2c_speed_standard, I2c_scl_high, ss_scl_high);
    AlI2c_SetSclCount(i2c, I2c_speed_standard, I2c_scl_low, ss_scl_low);
    AlI2c_SetSclCount(i2c, I2c_speed_fast, I2c_scl_high, fs_scl_high);
    AlI2c_SetSclCount(i2c, I2c_speed_fast, I2c_scl_low, fs_scl_low);
    AlI2c_SetSclCount(i2c, I2c_speed_high, I2c_scl_high, hs_scl_high);
    AlI2c_SetSclCount(i2c, I2c_speed_high, I2c_scl_low, hs_scl_low);
}

/*!
    \brief 	I2C Hold bus when RX_FIFO is full.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_RxFullSclHoldCfg(I2C_TypeDef *i2c, uint8_t value)
{
	i2c->IC_CON.RX_FIFO_FULL_HLD_CTRL = value;
}

/*!
    \brief 	I2C BUS_CLEAR_FEATURE_CTRL Bus Clear Feature cfg.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_BusClrCfg(I2C_TypeDef *i2c, uint8_t value)
{
	i2c->IC_CON.BUS_CLEAR_FEATURE_CTRL = value;
}

/*!
    \brief 	I2C SCL Stuck at Low Timeout register cfg.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SclTimeoutCfg(I2C_TypeDef *i2c, uint32_t value)
{
	i2c->IC_SCL_STUCK_AT_LOW_TIMEOUT = value;
}

/*!
    \brief 	I2C SDA Stuck at Low Timeout register cfg.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SdaTimeoutCfg(I2C_TypeDef *i2c, uint32_t value)
{
	i2c->IC_SDA_STUCK_AT_LOW_TIMEOUT = value;
}

/*!
    \brief 	I2C SDA_STUCK_RECOVERY_ENABLE Master initiates the SDA stuck at low recovery mechanism cfg.
    \param[in]  i2c: struct of I2C
    \param[out]
    \retval
*/
void AlI2c_SdaStuckRecvEnCfg(I2C_TypeDef *i2c, uint8_t value)
{
	i2c->IC_ENABLE.SDA_STUCK_RECOVERY_ENABLE = value;
}
