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
#include "mtimer.h"
#include "AL_emmc.h"

static unsigned int rca = 0;

/***************************************************************************/
/**
 * @brief	send the initial command to the sd/emmc device
 *
 * @param	None
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 SendInitCmdEmmc()
{
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    volatile unsigned int errorstatus;
    int Status;
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;
    
    // send command 0
    arg_r = 0;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ(&(eMMC->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_GO_IDLE_STATE;
    reg.bit.data_xfer_dir = DATA_READ;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(eMMC);
    //sleep(1000);
    SDIO_EMMC_DELAY_MS(100);
    
    validvoltage = 0;
    while (!validvoltage)
    {
    	//CMD1
        arg_r = 0x40000080;
        REG_WRITE(&(eMMC->argument_r), arg_r);
        reg.d32 = REG_READ(&(eMMC->cmd_r__xfer_mode));
        reg.bit.cmd_index = SD_CMD_SEND_OP_COND;
        reg.bit.resp_type_select = SDIO_Response_Short;
        reg.bit.data_xfer_dir = DATA_READ;
        REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
        wait_command_complete(eMMC);
        response01 = REG_READ(&(eMMC->resp01));
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);
    	if (validvoltage == 1)
    	{
    	    break;
    	}
    }

    // send command 2
    arg_r = 0;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ(&(eMMC->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_ALL_SEND_CID;
    reg.bit.block_count_enable = 0x1;
    reg.bit.data_xfer_dir = DATA_READ;
    reg.bit.resp_err_chk_enable = 0x1;
    reg.bit.multi_blk_sel = 0x1;
    reg.bit.resp_type_select = SDIO_Response_Long;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(eMMC);

    // send command 3
    arg_r = 0x10000;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_SET_REL_ADDR;
    reg.bit.resp_type_select = SDIO_Response_Short;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(eMMC);
    rca = REG_READ(&(eMMC->resp01)) & 0xFFFF0000;

    // send command 9
    arg_r = rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_SEND_CSD;
    reg.bit.resp_type_select = SDIO_Response_Long;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(eMMC);
    CSD_Tab[3] = REG_READ(&(eMMC->resp01));
    CSD_Tab[2] = REG_READ(&(eMMC->resp23));
    CSD_Tab[1] = REG_READ(&(eMMC->resp45));
    CSD_Tab[0] = REG_READ(&(eMMC->resp67));

    // Set buswidth to 1 bit clock to 48MHZ
    errorstatus = SD_GetCardInfo(&SDCardInfo);

    // send command 10
    arg_r = rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_SEND_CID;
    reg.bit.resp_type_select = SDIO_Response_Long;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(eMMC);

    //Set Freq 10M
    Status = HostControllerClockSetup(eMMC, FREQ_10M);
    
    // send command 7
    arg_r = rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_SEL_DESEL_CARD;
    reg.bit.resp_type_select = SDIO_Response_Short_48B;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(eMMC);

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
u32 SwitchDataWidthEmmc()
{
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;

    // send command 6
    arg_r = 0x03b70200;     //set sd model data width=4
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_HS_SWITCH;
    reg.bit.resp_type_select = SDIO_Response_Short;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    wait_command_complete(eMMC);
    r1.d32 = REG_READ(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
    r1.bit.extdat_xfer = 0x1;
    REG_WRITE(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    //sleep(2000);
    SDIO_EMMC_DELAY_MS(100);
    return AL_SUCCESS;
}


/*********************************************END OF FILE**********************/
