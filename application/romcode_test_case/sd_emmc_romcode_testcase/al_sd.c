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
#include "al_sd.h"
#include "al_mmc.h"

SD_CardInfo SDCardInfo;
static MtimerParams SdMtimer;
uint32_t CardType =  SDIO_HIGH_CAPACITY_SD_CARD;

/**
 * @brief set host controller clock frequency 
 * 
 * @param Ptr mshc structure
 * @param freq frequency 400K or 10M
 * @return uint32_t status
 */
uint32_t AlSd_HostControllerClockSetup(volatile DWC_mshc_block_registers* Ptr, uint32_t freq)
{
    MMC_ERR_TYPE status                         = MMC_SUCCESS;
    __IO SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r1   = {.d32 = 0,};

    MMC_PRINT("AlSd_HostControllerClockSetup\r\n");

    //  Host Controller Clock Setup
    r1.d32 = 0;
    REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    REG_WRITE(TOP_NS__CFG_CTRL_SDIO1_ADDR, 0x00000008);     //clk soft reset
    MMC_DELAY_MS(10);
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO1_ADDR, 0x00000000);
    MMC_DELAY_MS(MMC_DELAY_SCALE*EfuseDelayParam);

    if (freq == MMC_FREQ_10M) {
        MMC_BRANCHTEST_PRINT(BRANCH_SD_FREQ_SET,0);
        MMC_PRINT("MMC_FREQ_10M\r\n");

        r1.d32                  = 0;
        r1.bit.internal_clk_en  = MMC_CC_INTER_CLK_ENABLE;       //Oscillate
        r1.bit.clk_gen_select   = MMC_CC_CLK_GEN_SEL_PROGRAM;        //Programmable Clock Mode
        r1.bit.tout_cnt         = MMC_TC_TOUT_CNT_2_27;
        REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);

        r1.bit.pll_enable       = MMC_CC_PLL_ENABLE;            //PLL enabled
        REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);

        r1.bit.sd_clk_en        = MMC_CC_SD_CLK_ENABLE;             //Enable SDCLK/RCLK
        REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
    } else {  //default 400k
        MMC_BRANCHTEST_PRINT(BRANCH_SD_FREQ_SET,1);
        MMC_PRINT("MMC_FREQ_400K\r\n");
        r1.d32                  = 0;
        r1.bit.internal_clk_en  = MMC_CC_INTER_CLK_ENABLE;       //Oscillate
        r1.bit.tout_cnt         = MMC_TC_TOUT_CNT_2_27;
        REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);

        r1.bit.pll_enable       = MMC_CC_PLL_ENABLE;            //PLL enabled
        REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);

        r1.bit.sd_clk_en        = MMC_CC_SD_CLK_ENABLE;             //Enable SDCLK/RCLK
        REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
    }

    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    return status;
}

#ifdef _USE_4BIT
/**
 * @brief switch data width to 4 bit
 * 
 * @return uint32_t status
 */
uint32_t AlSd_SwitchDataWidth()
{
    uint32_t status         = MMC_SUCCESS;
    uint32_t arg_r          = 0;
    CMD_R__XFER_MODE_R reg  = {.d32 = 0,};

    // CMD55
    MMC_PRINT("send command 55\r\n");
    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    arg_r = Rca;
    REG_WRITE(&(SDIO->argument_r), arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_APP_CMD;
    reg.bit.resp_type_select    = MMC_Response_Short;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);

    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_55_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    // ACMD6
    MMC_PRINT("send append command 6\r\n");
    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    arg_r = SD_ACMD6_PARA_4BIT_MODE;     //set sd model data width=4
    REG_WRITE(&(SDIO->argument_r), arg_r);

    reg.bit.cmd_index           = SD_CMD_HS_SWITCH;
    reg.bit.resp_type_select    = MMC_Response_Short;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_6_ERR);

    return status;
}
#endif

/**
 * @brief send init cmd to sd card
 * 
 * @return uint32_t status
 */
uint32_t AlSd_SendInitCmdSd()
{
    MMC_ERR_TYPE status         = MMC_SUCCESS;
    uint32_t arg_r              = 0;
    __IO uint32_t response01    = 0;
    __IO uint32_t validvoltage  = 0;
    __IO CMD_R__XFER_MODE_R reg = {.d32 = 0,};
    
    MMC_PRINT("SendInitCmdSD\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_INIT, 7, 32);
#endif

    // send command 0
    MMC_PRINT("send command 0\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_INIT, 7, 8);
#endif

    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    arg_r = 0;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    MMC_PRINT("arg is %x\r\n", arg_r);

    reg.d32                 = 0;
    reg.bit.cmd_index       = SD_CMD_GO_IDLE_STATE;
    reg.bit.data_xfer_dir   = DATA_READ;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);

    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_0_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    // send command 8
    MMC_PRINT("send command 8\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_INIT, 9, 10);
#endif

    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    arg_r = SD_CMD8_PARA_2V7_3V6;//0x1AA;  //1:2v7~3v6 AA:check pattern (any 8 bit pattern is ok)   
    REG_WRITE(&(SDIO->argument_r), arg_r);
    MMC_PRINT("arg is %x\r\n", arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_HS_SEND_EXT_CSD;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Short;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);

    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_8_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    MMC_DELAY_MS(10);
    MMC_DELAY_MS(MMC_DELAY_SCALE*EfuseDelayParam);

#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_INIT, 11, 20);
#endif

    MTIMER_OUT_CONDITION(SD_GET_VALID_VOLTAGE_TIMEOUT_VAL, &SdMtimer, validvoltage != 1) {
        MMC_BRANCHTEST_PRINT(BRANCH_SD_VOLT_CHECK,0);
    	// CMD55
        MMC_PRINT("send command 55\r\n");
        MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);
        arg_r = 0;
        REG_WRITE(&(SDIO->argument_r), arg_r);
        MMC_PRINT("arg is %x\r\n", arg_r);

        reg.d32                     = 0;
        reg.bit.cmd_index           = SD_CMD_APP_CMD;
        reg.bit.resp_type_select    = MMC_Response_Short;
        reg.bit.data_xfer_dir       = DATA_READ;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
        reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
        REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);

        MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_55_ERR);
        MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    	//ACMD41
        MMC_PRINT("send command 41\r\n");
        MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

        arg_r = 0x40FF8000;//0x40300000;//[31]reserved,[30]HCS=1,voltage window 3.2~3.4
        REG_WRITE(&(SDIO->argument_r), arg_r);
        MMC_PRINT("arg is %x\r\n", arg_r);

        reg.d32                     = 0;
        reg.bit.cmd_index           = SD_CMD_SD_APP_OP_COND;
        reg.bit.resp_type_select    = MMC_Response_Short;
        reg.bit.data_xfer_dir       = DATA_READ;
        REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);

        MMC_WAIT_CMD_COMPLETE(SDIO, MMC_ACMD_41_ERR);
        MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

        response01 = REG_READ(&(SDIO->resp01));
    	validvoltage = (((response01 >> 31) == 1) ? 1: 0);
        if (validvoltage == 1) {
            if (((response01 >> 30) & 0x1) == 1) {
                MMC_BRANCHTEST_PRINT(BRANCH_SD_CARD_TYPE_CHECK,0);
                CardType =  SDIO_HIGH_CAPACITY_SD_CARD;
                MMC_PRINT("This card is SDIO_HIGH_CAPACITY_SD_CARD\r\n");
            } else {
                MMC_BRANCHTEST_PRINT(BRANCH_SD_CARD_TYPE_CHECK,1);
                CardType =  SDIO_STD_CAPACITY_SD_CARD_V2_0;
                MMC_PRINT("This card is SDIO_STD_CAPACITY_SD_CARD_V2_0\r\n");
            }
        }
    }
    MMC_BRANCHTEST_PRINT(BRANCH_SD_VOLT_CHECK,1);

    if (Mtimer_IsTimerOut(&SdMtimer)) {
        MMC_BRANCHTEST_PRINT(BRANCH_SD_VOLT_CHECK_TIMEOUT,0);
        return MMC_CHECK_VOLT_TIMEOUT;
    }
    MMC_BRANCHTEST_PRINT(BRANCH_SD_VOLT_CHECK_TIMEOUT,1);

    // send command 2
    MMC_PRINT("send command 2\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_INIT, 21, 22);
#endif

    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    arg_r = 0;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    MMC_PRINT("arg is %x\r\n", arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_ALL_SEND_CID;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Long;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);

    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_2_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    CidTab[3] = REG_READ(&(SDIO->resp01));
    CidTab[2] = REG_READ(&(SDIO->resp23));
    CidTab[1] = REG_READ(&(SDIO->resp45));
    CidTab[0] = REG_READ(&(SDIO->resp67));
    SDCardInfo.SD_cid.ManufacturerID = ((CidTab[0] >> 16)&0xFF);
    MMC_PRINT("CidTab 0~4 is %x, %x, %x, %x\r\n", CidTab[0], CidTab[1], CidTab[2], CidTab[3]);
    
    // send command 3
    MMC_PRINT("send command 3\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_INIT, 23, 24);
#endif

    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    arg_r = 0x0;    //Stuff bits
    REG_WRITE(&(SDIO->argument_r), arg_r);
    MMC_PRINT("arg is %x\r\n", arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_SET_REL_ADDR;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Short;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);

    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_3_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    Rca = REG_READ(&(SDIO->resp01)) & 0xFFFF0000;
    MMC_PRINT("rca is %x\r\n", Rca);

    // send command 9
    MMC_PRINT("send command 9\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_INIT, 25, 26);
#endif

    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    arg_r = Rca;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    MMC_PRINT("arg is %x\r\n", arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_SEND_CSD;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Long;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);

    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_9_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    CsdTab[3] = REG_READ(&(SDIO->resp01));
    CsdTab[2] = REG_READ(&(SDIO->resp23));
    CsdTab[1] = REG_READ(&(SDIO->resp45));
    CsdTab[0] = REG_READ(&(SDIO->resp67));
    MMC_PRINT("CsdTab 0~4 is %x, %x, %x, %x\r\n", CsdTab[0], CsdTab[1], CsdTab[2], CsdTab[3]);

    //set card infomation
    MMC_PRINT("set card infomation\r\n");
    status = AlSd_GetCardInfo(&SDCardInfo);
    if (status != MMC_SUCCESS) {
        MMC_BRANCHTEST_PRINT(BRANCH_SD_GET_CARD_INFO,0);
        return status;
    }
    MMC_BRANCHTEST_PRINT(BRANCH_SD_GET_CARD_INFO,1);

    // send command 7
    MMC_PRINT("send command 7\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_INIT, 30, 32);
#endif

    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    arg_r = Rca;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    MMC_PRINT("arg is %x\r\n", arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_SEL_DESEL_CARD;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Short_48B;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);

    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_7_ERR);
    MMC_WAIT_TRANSFER_COMPLETE(SDIO, MMC_CMD_7_XFER_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);
    
    return status;
}

/**
 * @brief init mshc controller and sd card
 * 
 * @return uint32_t status
 */
uint32_t AlSd_Init(void)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;

    REG_WRITE(TOP_NS__CFG_CTRL_SDIO1_ADDR, 0x000000b0); //enable write protection, disable card detection

    status = HostControllerSetup(SDIO);
    if (status != MMC_SUCCESS) {
        MMC_BRANCHTEST_PRINT(BRANCH_SD_HOST_CONTROLLER_SETUP,0);
		goto END;
	}
    MMC_BRANCHTEST_PRINT(BRANCH_SD_HOST_CONTROLLER_SETUP,1);

    status = InitInterruptSetting(SDIO);
    if (status != MMC_SUCCESS) {
        MMC_BRANCHTEST_PRINT(BRANCH_SD_INTR_INIT,0);
		goto END;
	}
    MMC_BRANCHTEST_PRINT(BRANCH_SD_INTR_INIT,1);

    status = AlSd_SendInitCmdSd();
    if (status != MMC_SUCCESS) {
        MMC_BRANCHTEST_PRINT(BRANCH_SD_INIT_CMD,0);
		goto END;
	}
    MMC_BRANCHTEST_PRINT(BRANCH_SD_INIT_CMD,1);

#ifdef _USE_4BIT
    status = AlSd_SwitchDataWidth();
    if (status != MMC_SUCCESS) {
		goto END;
	}
#endif

END:
	return status;
}

/**
 * @brief read single block size data 
 * 
 * @param readbuff reading data buffer 
 * @param ReadAddr read start address
 * @param BlockSize read data block size
 * @return uint32_t status
 */
uint32_t AlSd_ReadSingleBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize)
{
    MMC_ERR_TYPE status                     = MMC_SUCCESS;
    uint32_t arg_r                          = 0;
	uint32_t* Buffer_SingleBlock            = (uint32_t *)readbuff;
    __IO CMD_R__XFER_MODE_R reg             = {.d32 = 0,};
    __IO BLOCKCOUNT_R__BLOCKSIZE_R block    = {.d32 = 0,};
    __IO MMC_CMD23_PARAM r3                 = {.d32 = 0,};

#ifdef _USE_SDMA
    __IO WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1 = {.d32 = 0,};

    r1.d32 = REG_READ(&(SDIO->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
    r1.bit.dma_sel          = MMC_HC1_DMA_SEL_SDMA;
#ifdef _USE_4BIT
    r1.bit.dat_xfer_width   = MMC_HC1_DAT_XFER_WIDTH_4BIT;
#endif
    REG_WRITE(&(SDIO->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    REG_WRITE(&(SDIO->sdmasa_r), (uint32_t *)Buffer_SingleBlock);
#endif

	// send command 16
    MMC_PRINT("send command 16\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_BLOCKREAD, 1, 2);
#endif

    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    arg_r = BlockSize;
    REG_WRITE(&(SDIO->argument_r), arg_r);

    block.d32 = 0;
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 0x1;
    REG_WRITE(&(SDIO->blockcount_r__blocksize), block.d32);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_SET_BLOCKLEN;
    reg.bit.data_xfer_dir       = DATA_WRITE;
    reg.bit.resp_type_select    = MMC_C_RESP_LEN_48;
    reg.bit.block_count_enable  = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    
    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_16_ERR);
    MMC_PRINT("arg is %x\r\n", arg_r);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_BLOCKREAD, 3, 5);
#endif
    if (CardType != SDIO_STD_CAPACITY_SD_CARD_V2_0 && CardType != SDIO_STD_CAPACITY_SD_CARD_V1_1) {
        // send command 23
        MMC_PRINT("send cmd 23\r\n");

        MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

        r3.d32              = 0;
        r3.bit.block_num    = 0x1;
        arg_r               = r3.d32;
        REG_WRITE(&(SDIO->argument_r), arg_r);
        
        reg.d32                     = 0;
        reg.bit.cmd_index           = SD_CMD_SET_BLOCK_COUNT;
        reg.bit.data_xfer_dir       = DATA_WRITE;
        reg.bit.block_count_enable  = MMC_XM_BLOCK_COUNT_ENABLE;
        reg.bit.resp_type_select    = MMC_C_RESP_LEN_48;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
        reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
        REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);

        MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_23_ERR);
        MMC_PRINT("arg is %x\r\n", arg_r);
        MMC_PRINT("block.d32 is %x\r\n", block.d32);
        MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
    }

	// send command 17 read single block
    MMC_PRINT("send command 17\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_BLOCKREAD, 6, 9);
#endif

    MMC_CHECK_LINE_AND_CLEAR_STATUS(SDIO);

    if (CardType != SDIO_STD_CAPACITY_SD_CARD_V2_0 && CardType != SDIO_STD_CAPACITY_SD_CARD_V1_1)
        arg_r = ReadAddr;
    else
        arg_r = ReadAddr*BlockSize;
    REG_WRITE(&(SDIO->argument_r), arg_r);

    block.d32 = 0;
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 1;
    REG_WRITE(&(SDIO->blockcount_r__blocksize), block.d32);

    reg.d32                     = 0;
#ifdef _USE_SDMA
    reg.bit.dma_en              = MMC_XM_DMA_ENABLE;
#endif
    reg.bit.cmd_index           = SD_CMD_READ_SINGLE_BLOCK;
    reg.bit.data_xfer_dir       = MMC_XM_DATA_XFER_DIR_READ;
    reg.bit.resp_type_select    = MMC_C_RESP_LEN_48;
    reg.bit.block_count_enable  = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.data_present_sel    = MMC_C_DATA_PRESENT;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    
	MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_17_ERR);
#ifdef _USE_SDMA
    MMC_WAIT_TRANSFER_COMPLETE(SDIO, MMC_CMD_17_XFER_ERR);
#else
    MMC_TRANSFER_WITHOUT_DMA(SDIO, Buffer_SingleBlock, MMC_CMD_17_XFER_ERR);
#endif

    MMC_PRINT("arg is %x\r\n", arg_r);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
	return MMC_SUCCESS;
}

/**
 * @brief decode card info from CSD register
 * 
 * @param cardinfo card info structure
 * @return uint32_t status
 */
uint32_t AlSd_GetCardInfo(SD_CardInfo *cardinfo)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    uint8_t tmp         = 0;

    MMC_PRINT("SD_GetCardInfo\r\n");
#ifdef BRANCH_SD_FLOW_PRINT
    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_INIT, 27, 29);
#endif

    cardinfo->CardType  = (uint8_t)CardType;
    cardinfo->RCA       = (uint16_t)Rca;

    /*adjust postion*/
    CsdTab[0]   = CsdTab[0] << 8;
    tmp         = (CsdTab[1] & 0xFF000000) >> 24;
    memcpy(((uint8_t *)&CsdTab[0]), &tmp, 1);
    CsdTab[1]   = CsdTab[1] << 8;
    tmp         = (CsdTab[2] & 0xFF000000) >> 24;
    memcpy(((uint8_t *)&CsdTab[1]), &tmp, 1);
    CsdTab[2]   = CsdTab[2] << 8;
    tmp         = (CsdTab[3] & 0xFF000000) >> 24;
    memcpy(((uint8_t *)&CsdTab[2]), &tmp, 1);
    CsdTab[3]   = CsdTab[3] << 8;

    /*!< Byte 0 */
    tmp = (uint8_t)((CsdTab[0] & 0xFF000000) >> 24);
    cardinfo->SD_csd.CSDStruct      = (tmp & 0xC0) >> 6;
    cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
    cardinfo->SD_csd.Reserved1      = tmp & 0x03;

    /*!< Byte 1 */
    tmp = (uint8_t)((CsdTab[0] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.TAAC = tmp;

    /*!< Byte 2 */
    tmp = (uint8_t)((CsdTab[0] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.NSAC = tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(CsdTab[0] & 0x000000FF);
    cardinfo->SD_csd.MaxBusClkFrec = tmp;

    /*!< Byte 4 */
    tmp = (uint8_t)((CsdTab[1] & 0xFF000000) >> 24);
    cardinfo->SD_csd.CardComdClasses = tmp << 4;

    /*!< Byte 5 */
    tmp = (uint8_t)((CsdTab[1] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.CardComdClasses    |= (tmp & 0xF0) >> 4;
    cardinfo->SD_csd.RdBlockLen         = tmp & 0x0F;
    MMC_PRINT("rdblocklen is %d\r\n", cardinfo->SD_csd.RdBlockLen);

    /*!< Byte 6 */
    tmp = (uint8_t)((CsdTab[1] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.PartBlockRead      = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.WrBlockMisalign    = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.RdBlockMisalign    = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.DSRImpl            = (tmp & 0x10) >> 4;
    cardinfo->SD_csd.Reserved2          = 0; /*!< Reserved */

    if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0)) {
        MMC_BRANCHTEST_PRINT(BRANCH_SD_CARD_SIZE,0);
        MMC_PRINT("SD card STD catacity V1.1 or V2.0\r\n");

        cardinfo->SD_csd.DeviceSize         = (tmp & 0x03) << 10;

        /*!< Byte 7 */
        tmp = (uint8_t)(CsdTab[1] & 0x000000FF);
        cardinfo->SD_csd.DeviceSize         |= (tmp) << 2;

        /*!< Byte 8 */
        tmp = (uint8_t)((CsdTab[2] & 0xFF000000) >> 24);
        cardinfo->SD_csd.DeviceSize         |= (tmp & 0xC0) >> 6;
        

        cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
        cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

        /*!< Byte 9 */
        tmp = (uint8_t)((CsdTab[2] & 0x00FF0000) >> 16);
        cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
        cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
        cardinfo->SD_csd.DeviceSizeMul      = (tmp & 0x03) << 1;
        /*!< Byte 10 */
        tmp = (uint8_t)((CsdTab[2] & 0x0000FF00) >> 8);
        cardinfo->SD_csd.DeviceSizeMul      |= (tmp & 0x80) >> 7;

        cardinfo->CardCapacity  = (cardinfo->SD_csd.DeviceSize + 1) ;
        cardinfo->CardCapacity  *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
        cardinfo->CardBlockSize = 512;
        cardinfo->CardCapacity  *= (1 << cardinfo->SD_csd.RdBlockLen);
    }else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
        MMC_BRANCHTEST_PRINT(BRANCH_SD_CARD_SIZE,1);
        MMC_PRINT("SD card high capacity\r\n");

        /*!< Byte 7 */
        tmp = (uint8_t)(CsdTab[1] & 0x000000FF);
        cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

        /*!< Byte 8 */
        tmp = (uint8_t)((CsdTab[2] & 0xFF000000) >> 24);

        cardinfo->SD_csd.DeviceSize |= (tmp << 8);

        /*!< Byte 9 */
        tmp = (uint8_t)((CsdTab[2] & 0x00FF0000) >> 16);

        cardinfo->SD_csd.DeviceSize |= (tmp);
        MMC_PRINT("csize is %d\r\n", cardinfo->SD_csd.DeviceSize);

        /*!< Byte 10 */
        tmp = (uint8_t)((CsdTab[2] & 0x0000FF00) >> 8);

        cardinfo->CardCapacity  = ((uint64_t)cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
        cardinfo->CardBlockSize = 512;
    }

    cardinfo->SD_csd.EraseGrSize    = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.EraseGrMul     = (tmp & 0x3F) << 1;

    /*!< Byte 11 */
    tmp = (uint8_t)(CsdTab[2] & 0x000000FF);
    cardinfo->SD_csd.EraseGrMul         |= (tmp & 0x80) >> 7;
    cardinfo->SD_csd.WrProtectGrSize    = (tmp & 0x7F);

    /*!< Byte 12 */
    tmp = (uint8_t)((CsdTab[3] & 0xFF000000) >> 24);
    cardinfo->SD_csd.WrProtectGrEnable  = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.ManDeflECC         = (tmp & 0x60) >> 5;
    cardinfo->SD_csd.WrSpeedFact        = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.MaxWrBlockLen      = (tmp & 0x03) << 2;

    /*!< Byte 13 */
    tmp = (uint8_t)((CsdTab[3] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrBlockLen          |= (tmp & 0xC0) >> 6;
    cardinfo->SD_csd.WriteBlockPaPartial    = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.Reserved3              = 0;
    cardinfo->SD_csd.ContentProtectAppli    = (tmp & 0x01);

    /*!< Byte 14 */
    tmp = (uint8_t)((CsdTab[3] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.FileFormatGrouop   = (tmp & 0x80) >> 7;
    cardinfo->SD_csd.CopyFlag           = (tmp & 0x40) >> 6;
    cardinfo->SD_csd.PermWrProtect      = (tmp & 0x20) >> 5;
    cardinfo->SD_csd.TempWrProtect      = (tmp & 0x10) >> 4;
    cardinfo->SD_csd.FileFormat         = (tmp & 0x0C) >> 2;
    cardinfo->SD_csd.ECC                = (tmp & 0x03);

    /*!< Byte 15 */
    tmp = (uint8_t)(CsdTab[3] & 0x000000FF);
    cardinfo->SD_csd.CSD_CRC    = (tmp & 0xFE) >> 1;
    cardinfo->SD_csd.Reserved4  = 1;

    /*!< Byte 0 */
    tmp = (uint8_t)((CidTab[0] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ManufacturerID = tmp;

    /*!< Byte 1 */
    tmp = (uint8_t)((CidTab[0] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.OEM_AppliID = tmp << 8;

    /*!< Byte 2 */
    tmp = (uint8_t)((CidTab[0] & 0x000000FF00) >> 8);
    cardinfo->SD_cid.OEM_AppliID |= tmp;

    /*!< Byte 3 */
    tmp = (uint8_t)(CidTab[0] & 0x000000FF);
    cardinfo->SD_cid.ProdName1 = tmp << 24;

    /*!< Byte 4 */
    tmp = (uint8_t)((CidTab[1] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdName1 |= tmp << 16;

    /*!< Byte 5 */
    tmp = (uint8_t)((CidTab[1] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.ProdName1 |= tmp << 8;

    /*!< Byte 6 */
    tmp = (uint8_t)((CidTab[1] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ProdName1 |= tmp;

    /*!< Byte 7 */
    tmp = (uint8_t)(CidTab[1] & 0x000000FF);
    cardinfo->SD_cid.ProdName2 = tmp;

    /*!< Byte 8 */
    tmp = (uint8_t)((CidTab[2] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdRev = tmp;

    /*!< Byte 9 */
    tmp = (uint8_t)((CidTab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.ProdSN = tmp << 24;

    /*!< Byte 10 */
    tmp = (uint8_t)((CidTab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ProdSN |= tmp << 16;

    /*!< Byte 11 */
    tmp = (uint8_t)(CidTab[2] & 0x000000FF);
    cardinfo->SD_cid.ProdSN |= tmp << 8;

    /*!< Byte 12 */
    tmp = (uint8_t)((CidTab[3] & 0xFF000000) >> 24);
    cardinfo->SD_cid.ProdSN |= tmp;

    /*!< Byte 13 */
    tmp = (uint8_t)((CidTab[3] & 0x00FF0000) >> 16);
    cardinfo->SD_cid.Reserved1      |= (tmp & 0xF0) >> 4;
    cardinfo->SD_cid.ManufactDate   = (tmp & 0x0F) << 8;

    /*!< Byte 14 */
    tmp = (uint8_t)((CidTab[3] & 0x0000FF00) >> 8);
    cardinfo->SD_cid.ManufactDate |= tmp;

    /*!< Byte 15 */
    tmp = (uint8_t)(CidTab[3] & 0x000000FF);
    cardinfo->SD_cid.CID_CRC    = (tmp & 0xFE) >> 1;
    cardinfo->SD_cid.Reserved2  = 1;
    
    MMC_PRINT("cardinfo->CardBlockSize is %d\r\n", cardinfo->CardBlockSize);
    MMC_PRINT("cardinfo->CardCapacity is %llu\r\n", cardinfo->CardCapacity);

    return status;
}

/**
 * @brief raw set sd mode, apply for csu
 * 
 * @param Mode only freq now
 * @param Data mode data
 * @return uint32_t status
 */
uint32_t Csu_RawSdSetMode(uint32_t Mode, uint32_t Data)
{
    uint32_t status = MMC_SUCCESS;

    Mmc_BranchFlowPrint(BRANCH_FLOW_MODULE_MODESET, 1, 7);

    switch (Mode) {
        case MMC_MODE_FREQ:
            status = AlSd_HostControllerClockSetup(SDIO, Data);
            if (status != MMC_SUCCESS) {
                MMC_BRANCHTEST_PRINT(BRANCH_RAW_SD_SET_MODE_FREQ,0);
                return status;
            }
            MMC_BRANCHTEST_PRINT(BRANCH_RAW_SD_SET_MODE_FREQ,1);
            break;
        default:
            status = MMC_MODE_ERROR;
            break;
    }
    
    return status;
}
