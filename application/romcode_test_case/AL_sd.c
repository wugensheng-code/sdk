/*****************************************************************************/
/**
*
* @file sd_test.c
* @addtogroup sdps_v3_13
* @{
*
* The implementation of the XSdPs component's static initialization
* functionality.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ---    -------- -----------------------------------------------
* 1.00a hk/sg  10/17/13 Initial release
*       kvn    07/15/15 Modified the code according to MISRAC-2012.
* 3.7   aru    03/12/19 Modified the code according to MISRAC-2012.
*
* </pre>
*
******************************************************************************/

#include <string.h>
#include <stdio.h>
//#include "ff.h"
#include "mtimer.h"
#include "AL_sd.h"



//FATFS fs;
static unsigned int rca = 0;
static volatile MtimerParams sd_mtimer;


/***************************************************************************/
/**
 * @brief	check the card inserted or not inserted
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 CardDetection()
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    u32 CardStatus = 0;
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r1;
    ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R r2;
    ERROR_INT_STAT_R__NORMAL_INT_STAT_R r3;
    PSTATE_REG_R r4;
    
    //  Card Detection
    r1.d32 = 0;
    r1.bit.cmd_complete_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.xfer_complete_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.bgap_event_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.dma_interrupt_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.buf_wr_ready_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.buf_rd_ready_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.card_insertion_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.card_removal_stat_en = MMC_NORMAL_INT_STAT_EN;
    r1.bit.int_a_stat_en = MMC_NORMAL_INT_STAT_EN;
    REG_WRITE(&(SDIO->error_int_stat_en_r__normal_int_stat_en.d32), r1.d32);
    r2.d32 = 0;
    r2.bit.card_insertion_signal_en = MMC_NORMAL_INT_SIGN_EN;
    r2.bit.card_removal_signal_en = MMC_NORMAL_INT_SIGN_EN;
    REG_WRITE(&(SDIO->error_int_signal_en_r__normal_int_signal_en.d32), r2.d32);

    r3.d32 = 0;
    r3.bit.card_insertion = MMC_NORMAL_INT_STAT_EN;
    r3.bit.card_removal = MMC_NORMAL_INT_STAT_EN;
    REG_WRITE(&(SDIO->error_int_stat_r__normal_int_stat.d32), r3.d32);

    while (!CardStatus)
    {
        r4.d32 = REG_READ(&(SDIO->pstate_reg));
        CardStatus = (((r4.bit.card_inserted) == 1) ? 1:0);
    	if (CardStatus == 1)
    	{
            status = MMC_SUCCESS;
    	    break;
    	}
    }
    	
    return status;
}

/***************************************************************************/
/**
 * @brief	send the initial command to the sd/emmc device
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 SendInitCmdSD()
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;
    
    MMC_PRINT("SendInitCmdSD\r\n");
    // send command 0
    MMC_PRINT("send command 0\r\n");
    arg_r = 0;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.d32 = 0;
    reg.bit.cmd_index = SD_CMD_GO_IDLE_STATE;
    reg.bit.data_xfer_dir = DATA_READ;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    // send command 8
    MMC_PRINT("send command 8\r\n");
    arg_r = 0x100;//0x1AA;  //1:2v7~3v6 AA:check pattern (any 8 bit pattern is ok)   
    REG_WRITE(&(SDIO->argument_r), arg_r);
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.d32 = 0;
    reg.bit.cmd_index = SD_CMD_HS_SEND_EXT_CSD;
    reg.bit.data_xfer_dir = DATA_READ;
    reg.bit.resp_type_select = MMC_Response_Short;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);
    
    validvoltage = 0;
    MTIMER_OUT_CONDITION(SD_GET_VALID_VOLTAGE_TIMEOUT_VAL, &sd_mtimer, validvoltage != 1){
    	// CMD55
        MMC_PRINT("send command 55\r\n");
        arg_r = 0;
        REG_WRITE(&(SDIO->argument_r), arg_r);
        //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
        reg.d32 = 0;
        reg.bit.cmd_index = SD_CMD_APP_CMD;
        reg.bit.resp_type_select = MMC_Response_Short;
        reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
        reg.bit.data_xfer_dir = DATA_READ;
        reg.bit.multi_blk_sel = MMC_XM_SEL_MULTI_BLOCK;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
        MMC_WAIT_CMD_COMPLETE(SDIO);
        MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    	//ACMD41
        MMC_PRINT("send command 41\r\n");
        arg_r = 0x40100000;//0xC0100000; //[31]reserved,[30]HCS=1,voltage window 3.5~3.6
        REG_WRITE(&(SDIO->argument_r), arg_r);
        //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
        reg.d32 = 0;
        reg.bit.cmd_index = SD_CMD_SD_APP_OP_COND;
        reg.bit.resp_type_select = MMC_Response_Short;
        reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
        reg.bit.data_xfer_dir = DATA_READ;
        reg.bit.multi_blk_sel = MMC_XM_SEL_MULTI_BLOCK;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
        MMC_WAIT_CMD_COMPLETE(SDIO);
        MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

        response01 = REG_READ(&(SDIO->resp01));
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);
    }
    if(Mtimer_IsTimerOut(&mtimer)){
        status = SD_GET_VALID_VOLTAGE_TIMEOUT_ERROR;
        return status;
    }

    // send command 2
    MMC_PRINT("send command 2\r\n");
    arg_r = 0;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_ALL_SEND_CID;
    reg.bit.resp_type_select = MMC_Response_Long;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);
    CID_Tab[0] = REG_READ(&(SDIO->resp01));
    CID_Tab[1] = REG_READ(&(SDIO->resp23));
    CID_Tab[2] = REG_READ(&(SDIO->resp45));
    CID_Tab[3] = REG_READ(&(SDIO->resp67));
    MMC_PRINT("CID_Tab 0~4 is %x, %x, %x, %x\r\n", CID_Tab[0], CID_Tab[1], CID_Tab[2], CID_Tab[3]);
    
    // send command 3
    MMC_PRINT("send command 3\r\n");
    arg_r = 0x0;    //0x10000;Stuff bits
    REG_WRITE(&(SDIO->argument_r), arg_r);
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SET_REL_ADDR;
    reg.bit.resp_type_select = MMC_Response_Short;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);
    rca = REG_READ(&(SDIO->resp01)) & 0xFFFF0000;
    MMC_PRINT("rca is %x\r\n", rca);

    // send command 9
    MMC_PRINT("send command 9\r\n");
    arg_r = rca;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SEND_CSD;
    reg.bit.resp_type_select = MMC_Response_Long;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    CSD_Tab[3] = REG_READ(&(SDIO->resp01));
    CSD_Tab[2] = REG_READ(&(SDIO->resp23));
    CSD_Tab[1] = REG_READ(&(SDIO->resp45));
    CSD_Tab[0] = REG_READ(&(SDIO->resp67));
    MMC_PRINT("CSD_Tab 0~4 is %x, %x, %x, %x\r\n", CSD_Tab[0], CSD_Tab[1], CSD_Tab[2], CSD_Tab[3]);

    //set card infomation
    MMC_PRINT("set card infomation\r\n");
    status = SD_GetCardInfo(&SDCardInfo);
    if(status != MMC_SUCCESS){
        return status;
    }
    //Set Freq 10M
    MMC_PRINT("Set Freq 10M\r\n");
    status = HostControllerClockSetup(SDIO, MMC_FREQ_10M);
    if(status != MMC_SUCCESS){
        return status;
    }
    
    // send command 7
    MMC_PRINT("send command 7\r\n");
    arg_r = rca;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SEL_DESEL_CARD;
    reg.bit.resp_type_select = MMC_Response_Short_48B;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    return status;
}

/***************************************************************************/
/**
 * @brief	change the bit width
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 SwitchDataWidthSD()
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;

    MMC_PRINT("SwitchDataWidthSD\r\n");
    // send command 55  SET BUSWITHD TO 4 bit
    MMC_PRINT("send command 55\r\n");
    arg_r = rca;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.d32 = 0;
    reg.bit.cmd_index = SD_CMD_APP_CMD;
    reg.bit.resp_type_select = MMC_Response_Short;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir = DATA_READ;
    reg.bit.multi_blk_sel = MMC_XM_SEL_MULTI_BLOCK;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    // send command 6
    MMC_PRINT("send command 6\r\n");
    arg_r = 0x2;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_HS_SWITCH;
    reg.bit.resp_type_select = MMC_Response_Short;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    return status;
}


/*
u32 SD_WaitReadOperation(volatile DWC_mshc_block_registers* ptr)
{
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r;
    MTIMER_OUT_CONDITION(100, &Mtimer, r.bit.xfer_complete != 1){
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat));
    }
    if(Mtimer_IsTimerOut(&Mtimer)){
        return MMC_XFER_TIMEROUT;
    }else{
        return MMC_SUCCESS;
    }
}*/

/***************************************************************************/
/**
 * @brief	SD init sequence
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 SD_Init(void)
{
    MMC_ERR_TYPE status = MMC_FAILURE;

    status = CardDetection();
    if (status != MMC_SUCCESS) {
		goto END;
	}
    status = HostControllerSetup(SDIO);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    status = HostControllerClockSetup(SDIO, MMC_FREQ_400K);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    status = InitInterruptSetting(SDIO);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    status = SendInitCmdSD();
    if (status != MMC_SUCCESS) {
		goto END;
	}
    status = SwitchDataWidthSD();
    if (status != MMC_SUCCESS) {
		goto END;
	}
    
    status = MMC_SUCCESS;
END:
	return status;
}

/***************************************************************************/
/**
 * @brief	read single block size data 
 *
 * @param	readbuff reading data buffer 
 * @param	ReadAddr read start address
 * @param	BlockSize read data block size
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 SD_ReadSingleBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )readbuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    r1.d32 = 0;
    //r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;
    r1.bit.dat_xfer_width = MMC_HC1_DAT_XFER_WIDTH_4BIT;   //4-bit mode
    r1.bit.sd_bus_pwr_vdd1 = MMC_PC_SBP_VDD1_ON;   //PWR ON
    r1.bit.sd_bus_vol_vdd1 = MMC_PC_SBV_VDD1_3V3;   //3V
    REG_WRITE(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    REG_WRITE(&(eMMC->sdmasa_r), Buffer_SingleBlock);
    REG_WRITE(&(eMMC->adma_sa_low_r), Buffer_SingleBlock);
    r2.d32 = 0;
    REG_WRITE(&(eMMC->host_ctrl2_r__auto_cmd_stat), r2.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);
    MMC_PRINT("r2.d32 is %x\r\n", r2.d32);

	// send command 16
    MMC_PRINT("send command 16\r\n");
    arg_r = BlockSize;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    block.d32 = REG_READ(&(SDIO->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 1;
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    reg.bit.multi_blk_sel = MMC_XM_SEL_SINGLE_BLOCK;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_READ;
    REG_WRITE(&(SDIO->blockcount_r__blocksize), block.d32);
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

	// send command 17 read single block
    MMC_PRINT("send command 17\r\n");
    arg_r = ReadAddr;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.dma_en = MMC_XM_DMA_ENABLE;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_READ;
    reg.bit.multi_blk_sel = MMC_XM_SEL_SINGLE_BLOCK;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
    reg.bit.cmd_index = SD_CMD_READ_SINGLE_BLOCK;
    block.d32 = REG_READ(&(SDIO->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 1;
    REG_WRITE(&(SDIO->blockcount_r__blocksize), block.d32);
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
	MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_WAIT_TRANSFER_COMPLETE(SDIO);

	return MMC_SUCCESS;
}

/**  
 * This function initializes raw EMMC flash controller, driver and device. 
 * @param Param is the pointer to SDParam_t 
 * @return AL_SUCCESS on success, error number on failed. 
 * @note none 
*/ 
u32 Csu_RawSDInit(RawSDParam_t *Param) 
{  
    u32 status = MMC_SUCCESS;
    status = SD_Init();
    if(status != MMC_SUCCESS){
        return status;
    }
    Param->SDId = SDCardInfo.SD_cid.ManufacturerID;
    Param->SDSize = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize/2;
    printf("SD ID is %d, capacity is %d kBytes!\r\n", Param->SDId, Param->SDSize);
    return status;
}

/**  
 * This function reads data from raw EMMC flash. 
 * @param offset is the offset within flash to be read from. 
 * @param dest to the pointer to buffer which stores read data 
 * @param length is the length of bytes to be read 
 * @return MMC_SUCCESS on success, error number on failed. 
 * @note none 
 */
u32 Csu_RawSDRead(u32 Offset, u8* Dest, u32 Length) 
{  
    u32 status = MMC_SUCCESS;
    u32 blocksize = SDCardInfo.CardBlockSize;
    u32 offsetblock = Offset/blocksize;
    u32 offsetbytes = Offset%blocksize;
    u32 restorebytes = blocksize - offsetbytes;
    u8 *pdestaddr = Dest;
    u32 restorelength = Length % blocksize;
    u32 blocknum = 1 + Length / blocksize;
    u32 lastblocklength = (restorelength > restorebytes)? \
                        restorelength - restorebytes : restorelength + blocksize - restorebytes;
    if(restorelength > restorebytes){
        blocknum += 1;
    }

    for(u32 i = offsetblock; i < blocknum; i++){
        if(i == 0){
            status = SD_ReadSingleBlock(blockbuf, i, blocksize);
            if(status != MMC_SUCCESS){
                return status;
            }
            memcpy(pdestaddr, &blockbuf[offsetbytes], restorebytes);
            pdestaddr += restorebytes;
            memset(blockbuf, 0, blocksize);
        }else if(i == blocknum - 1){
            status = SD_ReadSingleBlock(blockbuf, i, blocksize);
            if(status != MMC_SUCCESS){
                return status;
            }
            memcpy(pdestaddr, blockbuf, lastblocklength);
            pdestaddr += lastblocklength;
            memset(blockbuf, 0, blocksize);
        }else{
            status = SD_ReadSingleBlock(pdestaddr, i, blocksize);
            if(status != MMC_SUCCESS){
                return status;
            }
            pdestaddr += blocksize;
        }
    }
    return status;  
}

