/*****************************************************************************/
/**
*
* @file AL_emmc.c
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
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 SendInitCmdEmmc()
{
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    uint32_t status = MMC_SUCCESS;
    __IO CMD_R__XFER_MODE_R reg;
    __IO OCR_R r1;
    uint32_t arg_r;

    MMC_PRINT("SendInitCmdEmmc\r\n");
    // send command 0   go idle state
    arg_r = EMMC_CMD0_PARA_GO_IDLE_STATE;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ(&(eMMC->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_GO_IDLE_STATE;
    reg.bit.data_xfer_dir = DATA_READ;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC);
    
    validvoltage = 0;
    MTIMER_OUT_CONDITION(EMMC_GET_VALID_VOLTAGE_TIMEOUT_VAL, &mtimer, validvoltage != 1){
        //CMD1  arg = OCR register check voltage valid
        r1.d32 = 0;
        r1.bit.voltage_mode = EMMC_OCR_DUAL_VOLTAGE;
        r1.bit.access_mode = EMMC_OCR_ACCESS_MODE_SECTOR_MODE;
        arg_r = r1.d32;
        REG_WRITE(&(eMMC->argument_r), arg_r);
        reg.d32 = REG_READ(&(eMMC->cmd_r__xfer_mode));
        reg.bit.cmd_index = SD_CMD_SEND_OP_COND;
        reg.bit.resp_type_select = MMC_Response_Short;
        reg.bit.data_xfer_dir = DATA_READ;
        REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
        MMC_WAIT_CMD_COMPLETE(eMMC);
        response01 = REG_READ(&(eMMC->resp01));
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);
    }
    if(Mtimer_IsTimerOut(&mtimer)){
        status = EMMC_GET_VALID_VOLTAGE_TIMEOUT_ERROR;
        return status;
    }

    // send command 2 get CID   Device IDentification
    arg_r = 0;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ(&(eMMC->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_ALL_SEND_CID;
    reg.bit.block_count_enable = 0x1;
    reg.bit.data_xfer_dir = DATA_READ;
    reg.bit.resp_err_chk_enable = 0x1;
    reg.bit.multi_blk_sel = 0x1;
    reg.bit.resp_type_select = MMC_Response_Long;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC);
    CID_Tab[0] = REG_READ(&(eMMC->resp01));
    CID_Tab[1] = REG_READ(&(eMMC->resp23));
    CID_Tab[2] = REG_READ(&(eMMC->resp45));
    CID_Tab[3] = REG_READ(&(eMMC->resp67));

    // send command 3 set relative device address
    arg_r = EMMC_CMD3_PARA_DEFAULT_VAL;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_SET_REL_ADDR;
    reg.bit.resp_type_select = MMC_Response_Short;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC);
    rca = REG_READ(&(eMMC->resp01)) & 0xFFFF0000;

    // send command 9 get addressed device's CSD on CMD line
    arg_r = rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_SEND_CSD;
    reg.bit.resp_type_select = MMC_Response_Long;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC);
    CSD_Tab[3] = REG_READ(&(eMMC->resp01));
    CSD_Tab[2] = REG_READ(&(eMMC->resp23));
    CSD_Tab[1] = REG_READ(&(eMMC->resp45));
    CSD_Tab[0] = REG_READ(&(eMMC->resp67));

    // get card infomation
    status = SD_GetCardInfo(&SDCardInfo);
    if(status != MMC_SUCCESS){
        return status;
    }

    // send command 10  get addressed device's CID on CMD line
    arg_r = rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_SEND_CID;
    reg.bit.resp_type_select = MMC_Response_Long;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC);

    //Set Freq 10M
    status = HostControllerClockSetup(eMMC, MMC_FREQ_10M);
    if(status != MMC_SUCCESS){
        return status;
    }
    
    // send command 7   selected/deselected card
    arg_r = rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_SEL_DESEL_CARD;
    reg.bit.resp_type_select = MMC_Response_Short_48B;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC);

    return MMC_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	change the bit width
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 SwitchDataWidthEmmc()
{
    uint32_t status = MMC_SUCCESS;
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;

    // send command 6
    arg_r = EMMC_CMD6_PARA_8_BIT_WIDTH_BUS;     //set sd model data width=8
    REG_WRITE(&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_HS_SWITCH;
    reg.bit.resp_type_select = MMC_Response_Short;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC);
    r1.d32 = REG_READ(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
    r1.bit.extdat_xfer = 0x1;
    REG_WRITE(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    //sleep(2000);
    MMC_DELAY_MS(100);
    return MMC_SUCCESS;
}


/*********************************************END OF FILE**********************/
