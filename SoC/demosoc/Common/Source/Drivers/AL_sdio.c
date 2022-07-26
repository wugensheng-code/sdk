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
#include "AL_sdio.h"

//FATFS fs;
static unsigned int rca = 0;


/***************************************************************************/
/**
 * @brief	check the card inserted or not inserted
 *
 * @param	None
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 CardDetection()
{
    u32 Status = AL_FAILURE;
    u32 CardStatus = 0;
    ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R r1;
    ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R r2;
    ERROR_INT_STAT_R__NORMAL_INT_STAT_R r3;
    PSTATE_REG_R r4;
    
    //  Card Detection
    r1.d32 = 0;
    r1.bit.cmd_complete_stat_en = 0x1;
    r1.bit.xfer_complete_stat_en = 0x1;
    r1.bit.bgap_event_stat_en = 0x1;
    r1.bit.dma_interrupt_stat_en = 0x1;
    r1.bit.buf_wr_ready_stat_en = 0x1;
    r1.bit.buf_rd_ready_stat_en = 0x1;
    r1.bit.card_insertion_stat_en = 0x1;
    r1.bit.card_removal_stat_en = 0x1;
    r1.bit.int_a_stat_en = 0x1;
    REG_WRITE(&(SDIO->error_int_stat_en_r__normal_int_stat_en), r1.d32);
    r2.d32 = 0;
    r2.bit.card_insertion_signal_en = 0x1;
    r2.bit.card_removal_signal_en = 0x1;
    REG_WRITE(&(SDIO->error_int_signal_en_r__normal_int_signal_en), r2.d32);

    r3.d32 = 0;
    r3.bit.card_insertion = 0x1;
    r3.bit.card_removal = 0x1;
    REG_WRITE(&(SDIO->error_int_stat_r__normal_int_stat), r3.d32);

    //sleep(200);
    SDIO_EMMC_DELAY_MS(10);

    while (!CardStatus)
    {
        r4.d32 = REG_READ(&(SDIO->pstate_reg));
        CardStatus = (((r4.bit.card_inserted) == 1) ? 1:0);
    	if (CardStatus == 1)
    	{
            Status = AL_SUCCESS;
    	    break;
    	}
    }
    	
    return Status;
}

/***************************************************************************/
/**
 * @brief	send the initial command to the sd/emmc device
 *
 * @param	None
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 SendInitCmdSD()
{
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    volatile unsigned int errorstatus;
    int Status;
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;
    
    // send command 0
    arg_r = 0;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_GO_IDLE_STATE;
    reg.bit.data_xfer_dir = DATA_READ;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(SDIO);

    // send command 8
    arg_r = 0x1AA;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_HS_SEND_EXT_CSD;
    reg.bit.data_xfer_dir = DATA_READ;
    reg.bit.resp_type_select = SDIO_Response_Short;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(SDIO);

    // send command 55
    arg_r = 0;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_APP_CMD;
    reg.bit.resp_type_select = SDIO_Response_Short;
    reg.bit.block_count_enable = 0x1;
    reg.bit.data_xfer_dir = DATA_READ;
    reg.bit.multi_blk_sel = 0x1;
    reg.bit.resp_err_chk_enable = 0x1;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(SDIO);
    
    validvoltage = 0;
    while (!validvoltage)
    {
    	// CMD55
        arg_r = 0;
        REG_WRITE(&(SDIO->argument_r), arg_r);
        reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
        reg.bit.cmd_index = SD_CMD_APP_CMD;
        reg.bit.resp_type_select = SDIO_Response_Short;
        reg.bit.block_count_enable = 0x1;
        reg.bit.data_xfer_dir = DATA_READ;
        reg.bit.multi_blk_sel = 0x1;
        reg.bit.resp_err_chk_enable = 0x1;
        REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
        wait_command_complete(SDIO);

    	//CMD41
        arg_r = 0xC0100000;
        REG_WRITE(&(SDIO->argument_r), arg_r);
        reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
        reg.bit.cmd_index = SD_CMD_SD_APP_OP_COND;
        reg.bit.resp_type_select = SDIO_Response_Short;
        reg.bit.block_count_enable = 0x1;
        reg.bit.data_xfer_dir = DATA_READ;
        reg.bit.multi_blk_sel = 0x1;
        reg.bit.resp_err_chk_enable = 0x1;
        REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
        wait_command_complete(SDIO);

        response01 = REG_READ(&(SDIO->resp01));
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);
    	if (validvoltage == 1)
    	{
    	    break;
    	}
    }

    // send command 2
    arg_r = 0;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_ALL_SEND_CID;
    reg.bit.resp_type_select = SDIO_Response_Long;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(SDIO);
    CID_Tab[0] = REG_READ(&(SDIO->resp01));
    CID_Tab[1] = REG_READ(&(SDIO->resp23));
    CID_Tab[2] = REG_READ(&(SDIO->resp45));
    CID_Tab[3] = REG_READ(&(SDIO->resp67));
    
    // send command 3
    arg_r = 0x10000;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SET_REL_ADDR;
    reg.bit.resp_type_select = SDIO_Response_Short;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(SDIO);
    rca = REG_READ(&(SDIO->resp01)) & 0xFFFF0000;

    // send command 9
    arg_r = rca;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SEND_CSD;
    reg.bit.resp_type_select = SDIO_Response_Long;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(SDIO);

    CSD_Tab[3] = REG_READ(&(SDIO->resp01));
    CSD_Tab[2] = REG_READ(&(SDIO->resp23));
    CSD_Tab[1] = REG_READ(&(SDIO->resp45));
    CSD_Tab[0] = REG_READ(&(SDIO->resp67));

    errorstatus = SD_GetCardInfo(&SDCardInfo);
    //Set Freq 10M
    Status = HostControllerClockSetup(SDIO, FREQ_10M);
    
    // send command 7
    arg_r = rca;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SEL_DESEL_CARD;
    reg.bit.resp_type_select = SDIO_Response_Short_48B;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(SDIO);

    return AL_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	change the bit width
 *
 * @param	None
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 SwitchDataWidthSD()
{
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;

    // send command 55  SET BUSWITHD TO 4 bit
    arg_r = rca;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_APP_CMD;
    reg.bit.resp_type_select = SDIO_Response_Short;
    reg.bit.block_count_enable = 0x1;
    reg.bit.data_xfer_dir = DATA_READ;
    reg.bit.multi_blk_sel = 0x1;
    reg.bit.resp_err_chk_enable = 0x1;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(SDIO);

    // send command 6
    arg_r = 0x2;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_HS_SWITCH;
    reg.bit.resp_type_select = SDIO_Response_Short;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(SDIO);

    //sleep(2000);
    SDIO_EMMC_DELAY_MS(100);

    return AL_SUCCESS;
}


/*
u32 SD_WaitReadOperation(volatile DWC_mshc_block_registers* ptr)
{
    __IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R r;
    MTIMER_OUT_CONDITION(100, &Mtimer, r.bit.xfer_complete != 1){
        r.d32 = REG_READ(&(ptr->error_int_stat_r__normal_int_stat));
    }
    if(Mtimer_IsTimerOut(&Mtimer)){
        return SD_EMMC_XFER_TIMEROUT;
    }else{
        return AL_SUCCESS;
    }
}*/


