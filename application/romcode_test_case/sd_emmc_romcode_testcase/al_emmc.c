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
#include "al_emmc.h"
#include "al_mmc.h"

static MtimerParams EmmcMtimer;
SD_CardInfo EmmcCardInfo;
static uint8_t __attribute__((aligned(4))) ext_csd_buf[512];

/**
 * @brief set host controller clock frequency 
 * 
 * @param Ptr mshc structure
 * @param freq frequency 400K or 10M
 * @return uint32_t status
 */
uint32_t AlEmmc_HostControllerClockSetup(volatile DWC_mshc_block_registers* Ptr, uint32_t freq)
{
    MMC_ERR_TYPE status                         = MMC_SUCCESS;
    __IO SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r1   = {.d32 = 0,};

    //  Host Controller Clock Setup
    MMC_PRINT("AlEmmc_HostControllerClockSetup\r\n");

    if (freq != MMC_FREQ_10M) {
        MMC_BRANCHTEST_PRINT(BRANCH_EMMC_FREQ_400K);
        return MMC_WRONG_FREQ;
    }
    MMC_BRANCHTEST_PRINT(BRANCH_EMMC_FREQ_10M);
    MMC_PRINT("MMC_FREQ_10M\r\n");

    //disable sd_clk_on
    r1.d32              = REG_READ(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
    r1.bit.sd_clk_en    = MMC_CC_SD_CLK_DISABLE;
    REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    //clk soft rst
    uint32_t top_reg = REG_READ(TOP_NS__CFG_CTRL_SDIO1_ADDR);
    top_reg |= TOP_CFG_REG_CLK_RST;
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO1_ADDR, top_reg);     //clk soft reset
    MMC_DELAY_MS(10);
    //set programerable_mode
    r1.bit.clk_gen_select   = MMC_CC_CLK_GEN_SEL_PROGRAM;        //Programmable Clock Mode
    r1.bit.freq_sel         = 0;
    r1.bit.tout_cnt         = MMC_TC_TOUT_CNT_2_27;
    REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
    MMC_WAIT_CLK_STABLE(Ptr);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);
    //soft rst release
    top_reg &= TOP_CFG_REG_CLK_RST_RELEASE;
    REG_WRITE(TOP_NS__CFG_CTRL_SDIO1_ADDR, top_reg);
    MMC_DELAY_MS(MMC_DELAY_SCALE*EfuseDelayParam);
    //sd_clk_on
    r1.bit.sd_clk_en        = MMC_CC_SD_CLK_ENABLE;             //Enable SDCLK/RCLK
    REG_WRITE(&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
    MMC_WAIT_CLK_STABLE(Ptr);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

    return status;
}

/**
 * @brief send init cmd to emmc card
 * 
 * @return uint32_t status
 */
uint32_t AlEmmc_SendInitCmd()
{
    uint32_t status             = MMC_SUCCESS;
    uint32_t arg_r              = 0;
    __IO uint32_t response01    = 0;
    __IO uint32_t validvoltage  = 0;
    __IO CMD_R__XFER_MODE_R reg = {.d32 = 0,};
    __IO OCR_R r1               = {.d32 = 0,};

    MMC_PRINT("SendInitCmdEmmc\r\n");
    // send command 0   go idle state
    MMC_PRINT("send command 0\r\n");

    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

    arg_r = EMMC_CMD0_PARA_GO_IDLE_STATE;
    REG_WRITE(&(eMMC->argument_r), arg_r);

    reg.d32                 = 0;
    reg.bit.cmd_index       = SD_CMD_GO_IDLE_STATE;
    reg.bit.data_xfer_dir   = DATA_READ;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_0_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    MMC_DELAY_MS(10);

    MTIMER_OUT_CONDITION(EMMC_GET_VALID_VOLTAGE_TIMEOUT_VAL, &EmmcMtimer, validvoltage != 1) {
        MMC_BRANCHTEST_PRINT(BRANCH_EMMC_VOLT_VALID_REG_READ);
        //CMD1  arg = OCR register check voltage valid
        MMC_PRINT("send command 1\r\n");

        MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

        r1.d32                  = 0;
#ifdef USE_ERROR_BRANCH
    if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_EMMC_IOBANK1_1V8)) {
        IoBank1Ref |= 0x1;
        ERROR_BRANCH_BIT_SET(BERROR_BRANCH_EMMC_IOBANK1_1V8);
    }
#endif
        if (MMC_IO_BANK1_SUPPORT_1V8(IoBank1Ref) == 1) {
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_IOBANK1_1V8);
            r1.bit.voltage_mode = EMMC_OCR_DUAL_VOLTAGE;   //1.8V and 3v3
            MMC_PRINT("io bank1 support dual volt\r\n");
        } else {
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_IOBANK1_3V3);
            r1.bit.voltage_mode = EMMC_OCR_HIGH_VOLTAGE;    //3v3 only
            MMC_PRINT("io bank1 support high volt\r\n");
        }
        r1.bit.voltage2v7_3v6   = EMMC_OCR_VOLTAGE_2V7_3V6;  
        r1.bit.access_mode      = EMMC_OCR_ACCESS_MODE_SECTOR_MODE;  //sector mode
        arg_r                   = r1.d32;
        REG_WRITE(&(eMMC->argument_r), arg_r);
        MMC_PRINT("arg_r is %x, %d\r\n", arg_r, arg_r);

        reg.d32                     = 0;
        reg.bit.cmd_index           = SD_CMD_SEND_OP_COND;
        reg.bit.data_xfer_dir       = DATA_READ;
        reg.bit.resp_type_select    = MMC_Response_Short;
        REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

        MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_1_ERR);
        MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

        response01 = REG_READ(&(eMMC->resp01));
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);

        MMC_DELAY_MS(MMC_DELAY_SCALE*EfuseDelayParam);
#ifdef USE_ERROR_BRANCH
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_EMMC_VOLT_VALID_TIMEOUT)) {
            if (validvoltage == 1) {
                EmmcMtimer.IsTimerOut = 1;
                break;
            }
        }
#endif
    }
    MMC_BRANCHTEST_PRINT(BRANCH_EMMC_VOLT_VALID_DONE);

    if (Mtimer_IsTimerOut(&EmmcMtimer)) {
#ifdef USE_ERROR_BRANCH
        if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_EMMC_VOLT_VALID_TIMEOUT)) {
            ERROR_BRANCH_BIT_SET(BERROR_BRANCH_EMMC_VOLT_VALID_TIMEOUT);
        }
#endif
        MMC_BRANCHTEST_PRINT(BRANCH_EMMC_VOLT_VALID_TIMEOUT);
        status = MMC_CHECK_VOLT_TIMEOUT;
        return status;
    }
    MMC_BRANCHTEST_PRINT(BRANCH_EMMC_VOLT_VALID_SUCCESS);

    // send command 2 get CID   Device IDentification
    MMC_PRINT("send command 2\r\n");

    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

    arg_r = 0;
    REG_WRITE(&(eMMC->argument_r), arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_ALL_SEND_CID;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Long;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_2_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    // send command 3 set relative device address
    MMC_PRINT("send command 3\r\n");

    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

    Rca     = EMMC_CMD3_PARA_DEFAULT_VAL;
    arg_r   = Rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_SET_REL_ADDR;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Short;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_3_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    // send command 9 get addressed device's CSD on CMD line
    MMC_PRINT("send command 9\r\n");

    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

    arg_r = Rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_SEND_CSD;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Long;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_9_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    CsdTab[3] = REG_READ(&(eMMC->resp01));
    CsdTab[2] = REG_READ(&(eMMC->resp23));
    CsdTab[1] = REG_READ(&(eMMC->resp45));
    CsdTab[0] = REG_READ(&(eMMC->resp67));
    MMC_PRINT("CsdTab 0~4 is %x, %x, %x, %x\r\n", CsdTab[0], CsdTab[1], CsdTab[2], CsdTab[3]);

    // send command 10  get addressed device's CID on CMD line
    MMC_PRINT("send command 10\r\n");

    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

    arg_r = Rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_SEND_CID;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Long;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_10_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    CidTab[3] = REG_READ(&(eMMC->resp01));
    CidTab[2] = REG_READ(&(eMMC->resp23));
    CidTab[1] = REG_READ(&(eMMC->resp45));
    CidTab[0] = REG_READ(&(eMMC->resp67));
    MMC_PRINT("CidTab 0~4 is %x, %x, %x, %x\r\n", CidTab[0], CidTab[1], CidTab[2], CidTab[3]);
    EmmcCardInfo.SD_cid.ManufacturerID =  ((CidTab[0] >> 16)&0xFF);
    
    // send command 7   selected/deselected card
    MMC_PRINT("send command 7\r\n");

    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

    arg_r = Rca;
    REG_WRITE(&(eMMC->argument_r), arg_r);

    reg.d32                     = 0;
    reg.bit.cmd_index           = SD_CMD_SEL_DESEL_CARD;
    reg.bit.data_xfer_dir       = DATA_READ;
    reg.bit.resp_type_select    = MMC_Response_Short_48B;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
    
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_7_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    MMC_DELAY_MS(MMC_DELAY_SCALE*EfuseDelayParam);

    return MMC_SUCCESS;
}

#if _USE_8BIT
/**
 * @brief switch data width to 8 bit
 * 
 * @return uint32_t status
 */
uint32_t AlEmmc_SwitchDataWidth()
{
    uint32_t status = MMC_SUCCESS;
    uint32_t arg_r = 0;
    CMD_R__XFER_MODE_R reg = {.d32 = 0,};
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1{.d32 = 0,};

    // send command 6
    arg_r = EMMC_CMD6_PARA_8_BIT_WIDTH_BUS;     //set sd model data width=8
    REG_WRITE(&(eMMC->argument_r), arg_r);

    reg.bit.cmd_index           = SD_CMD_HS_SWITCH;
    reg.bit.resp_type_select    = MMC_Response_Short;
    reg.bit.data_xfer_dir       = DATA_WRITE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_6_ERR);

    r1.d32              = REG_READ(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
    r1.bit.extdat_xfer  = 0x1;
    REG_WRITE(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);

    MMC_DELAY_MS(10);

    return MMC_SUCCESS;
}
#endif

/**
 * @brief read single block size data 
 * 
 * @param Readbuff reading data buffer 
 * @param ReadAddr read start address
 * @param BlockSize read data block size
 * @return uint32_t status
 */
uint32_t AlEmmc_ReadSingleBlock(uint8_t *Readbuff, uint32_t ReadAddr, uint16_t BlockSize)
{
    uint32_t status                         = MMC_SUCCESS;
    uint32_t arg_r                          = 0;
	uint32_t* Buffer_SingleBlock            = (uint32_t* )Readbuff;
    __IO CMD_R__XFER_MODE_R reg             = {.d32 = 0,};
    __IO BLOCKCOUNT_R__BLOCKSIZE_R block    = {.d32 = 0,};
    __IO MMC_CMD23_PARAM r3                 = {.d32 = 0,};

#ifdef _USE_SDMA
    __IO WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1 = {.d32 = 0,};

    r1.d32 = REG_READ(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
    MMC_PRINT("r1 is  %x, %d\r\n", r1.d32, r1.d32);
    r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;
    REG_WRITE(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);

    REG_WRITE(&(eMMC->sdmasa_r), Buffer_SingleBlock);
#endif

	// send command 16
    MMC_PRINT("read send cmd 16\r\n");

    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);
    
    arg_r = BlockSize;  //block length  512
    REG_WRITE(&(eMMC->argument_r), arg_r);

    block.d32                   = 0;
    block.bit.xfer_block_size   = BlockSize;
    block.bit.blockcount_r      = 0x1;
    REG_WRITE(&(eMMC->blockcount_r__blocksize), block.d32);

    reg.d32                     = 0;
    reg.bit.block_count_enable  = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir       = MMC_XM_DATA_XFER_DIR_READ;
    reg.bit.resp_type_select    = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index           = SD_CMD_SET_BLOCKLEN;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_16_ERR);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

    // send command 23
    MMC_PRINT("read send cmd 23\r\n");

    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

    r3.d32              = 0;
    r3.bit.block_num    = 0x1;
    arg_r               = r3.d32;
    REG_WRITE(&(eMMC->argument_r), arg_r);

    reg.d32                     = 0;
    reg.bit.block_count_enable  = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir       = DATA_WRITE;
    reg.bit.resp_type_select    = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index           = SD_CMD_SET_BLOCK_COUNT;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_23_ERR);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

	// send command 17 read single block
    MMC_PRINT("send cmd 17\r\n");

    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

    arg_r = ReadAddr;
    REG_WRITE(&(eMMC->argument_r), arg_r);

    block.d32 = 0;
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 0x1;
    REG_WRITE(&(eMMC->blockcount_r__blocksize), block.d32);

    reg.d32                     = 0;
#ifdef _USE_SDMA
    reg.bit.dma_en              = MMC_XM_DMA_ENABLE;
#endif
    reg.bit.block_count_enable  = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir       = MMC_XM_DATA_XFER_DIR_READ;
    reg.bit.resp_type_select    = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel    = MMC_C_DATA_PRESENT;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
    reg.bit.cmd_index = SD_CMD_READ_SINGLE_BLOCK;
    REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

	MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_17_ERR);
#ifdef _USE_SDMA
    MMC_WAIT_TRANSFER_COMPLETE(eMMC, MMC_CMD_17_XFER_ERR);
#else
    MMC_TRANSFER_WITHOUT_DMA(eMMC, Buffer_SingleBlock, MMC_CMD_17_XFER_ERR);
#endif
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

	return MMC_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	emmc init sequence
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
uint32_t AlEmmc_Init(void)
{
    int status = MMC_SUCCESS;

    HostControllerSetup(eMMC);

    /*status = Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    MMC_BRANCHTEST_PRINT(BRANCH_EMMC_SETDEFAULTMODE_SUCCESS);*/

    InitInterruptSetting(eMMC);

    status = AlEmmc_SendInitCmd();
    if (status != MMC_SUCCESS) {
		goto END;
	}
    MMC_BRANCHTEST_PRINT(BRANCH_EMMC_SENDINITCMD_SUCCESS);

    status = AlEmmc_GetCardInfo(&EmmcCardInfo);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    MMC_BRANCHTEST_PRINT(BRANCH_EMMC_GETCARDINFO_SUCCESS);
    
END:
    MMC_BRANCHTEST_PRINT(BRANCH_EMMC_INIT_ERROR);
	return status;
}

/**
 * @brief This function initializes raw EMMC flash controller, driver and device.
 * 
 * @param Param the pointer to EmmcParam_t 
 * @return uint32_t status
 */
uint32_t Csu_RawEmmcInit(RawEmmcParam_t *Param) 
{  
    uint32_t status = MMC_SUCCESS;

    status = AlEmmc_Init();
    if (status != MMC_SUCCESS) {
        MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_INIT_ERROR);
        return status;
    }
    MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_INIT_SUCCESS);

    Param->EmmcId = EmmcCardInfo.SD_cid.ManufacturerID;
    Param->EmmcSize = EmmcCardInfo.CardCapacity / EmmcCardInfo.CardBlockSize / 2;
    MMC_PRINT("emmc ID is %d, capacity is %ld kBytes!\r\n", Param->EmmcId, Param->EmmcSize);

    return status;
}

/**
 * @brief This function reads data from raw EMMC flash. 
 * 
 * @param Offset the offset within flash to be read from. 
 * @param Dest to the pointer to buffer which stores read data 
 * @param Length the length of bytes to be read 
 * @return uint32_t status
 */
uint32_t Csu_RawEmmcRead(uint32_t Offset, uint8_t* Dest, uint32_t Length) 
{  
    uint32_t status             = MMC_SUCCESS;
    uint8_t *pdestaddr          = Dest;
    uint32_t blocksize          = EmmcCardInfo.CardBlockSize;       //block size
    uint32_t startblock         = Offset / blocksize;
    uint32_t endpoint           = Offset + Length - 1;
    uint32_t endblock           = endpoint / blocksize;
    uint32_t firstblockoffset   = Offset % blocksize;
    uint32_t firstblockstore    = blocksize - firstblockoffset;
    uint32_t firstblockbytes    = (firstblockstore > Length)? Length : firstblockstore;
    uint32_t lastblockbytes     = endpoint % blocksize + 1;

    MMC_PRINT("offset = %d, Length = %d\r\n", Offset, Length);
    MMC_PRINT("startblock: %d\tfirstblockoffset: %d\tfirstblockbytes: %d\r\n", startblock, firstblockoffset, firstblockbytes);
    MMC_PRINT("endblock: %d\tlastblockbytes: %d\t\r\n", endblock, lastblockbytes);

    for (uint32_t i = startblock; i <= endblock; i++) {
        if (i == startblock) {
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_READ_STARTBLOCK);
#ifdef USE_ERROR_BRANCH
            if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_EMMC_RAW_READ_STARTBLOCK_ERROR)) {
                if (ERROR_BRANCH_CHECK_BIT_SET(BERROR_BRANCH_XFER_COMPLETE_ERROR)) {
                    ERROR_BRANCH_BIT_RESET(BERROR_BRANCH_XFER_COMPLETE_ERROR);
                    ERROR_BRANCH_BIT_SET(BERROR_BRANCH_EMMC_RAW_READ_STARTBLOCK_ERROR);
                }
            }
#endif
            status = AlEmmc_ReadSingleBlock(FlashSharedBuf, i, blocksize);
            if (status != MMC_SUCCESS) {
                MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_READ_STARTBLOCK_ERROR);
                return status;
            }
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_READ_STARTBLOCK_SUCCESS);

            memcpy(pdestaddr, &FlashSharedBuf[firstblockoffset], firstblockbytes);
            pdestaddr += firstblockbytes;
        } else if (i == endblock) {
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_READ_ENDBLOCK);
#ifdef USE_ERROR_BRANCH
            if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_EMMC_RAW_READ_ENDBLOCK_ERROR)) {
                if (ERROR_BRANCH_CHECK_BIT_SET(BERROR_BRANCH_XFER_COMPLETE_ERROR)) {
                    ERROR_BRANCH_BIT_RESET(BERROR_BRANCH_XFER_COMPLETE_ERROR);
                    ERROR_BRANCH_BIT_SET(BERROR_BRANCH_EMMC_RAW_READ_ENDBLOCK_ERROR);
                }
            }
#endif
            status = AlEmmc_ReadSingleBlock(FlashSharedBuf, i, blocksize);
            if (status != MMC_SUCCESS) {
                MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_READ_ENDBLOCK_ERROR);
                return status;
            }
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_READ_ENDBLOCK_SUCCESS);

            memcpy(pdestaddr, FlashSharedBuf, lastblockbytes);
            pdestaddr += lastblockbytes;
        } else {
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_READ_MIDDLEBLOCK);
#ifdef USE_ERROR_BRANCH
            if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_EMMC_RAW_READ_MIDDLEBLOCK_ERROR)) {
                if (ERROR_BRANCH_CHECK_BIT_SET(BERROR_BRANCH_XFER_COMPLETE_ERROR)) {
                    ERROR_BRANCH_BIT_RESET(BERROR_BRANCH_XFER_COMPLETE_ERROR);
                    ERROR_BRANCH_BIT_SET(BERROR_BRANCH_EMMC_RAW_READ_MIDDLEBLOCK_ERROR);
                }
            }
#endif
            status = AlEmmc_ReadSingleBlock(pdestaddr, i, blocksize);
            if (status != MMC_SUCCESS) {
                MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_READ_MIDDLEBLOCK_ERROR);
                return status;
            }
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_READ_MIDDLEBLOCK_SUCCESS);

            pdestaddr += blocksize;
        }
    }

    return status;
}

/**
 * @brief raw set emmc mode, apply for csu
 * 
 * @param Mode only freq now
 * @param Data mode data
 * @return uint32_t status
 */
uint32_t Csu_RawEmmcSetMode(uint32_t Mode, uint32_t Data)
{
    uint32_t status = MMC_SUCCESS;

#ifdef USE_ERROR_BRANCH
    if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_EMMC_RAW_SET_MODE_DEFAULT)) {
        Mode = MMC_MODE_MAX;
        ERROR_BRANCH_BIT_SET(BERROR_BRANCH_EMMC_RAW_SET_MODE_DEFAULT);
    }
#endif
    switch (Mode) {
        case MMC_MODE_FREQ:
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_SET_MODE_FREQ);
            MMC_PRINT("set emmc freq %d\r\n", Data);

#ifdef USE_ERROR_BRANCH
            if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_EMMC_RAW_SET_MODE_FREQ_INVALIDFREQ)) {
                Data = MMC_FREQ_MAX;
                ERROR_BRANCH_BIT_SET(BERROR_BRANCH_EMMC_RAW_SET_MODE_FREQ_INVALIDFREQ);
            }
#endif
            if (Data >= MMC_FREQ_MAX) {
                MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_SET_MODE_FREQ_INVALIDFREQ);
                status = MMC_WRONG_FREQ;
                break;
            }
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_SET_MODE_FREQ_VALIDFREQ);

            status = AlEmmc_HostControllerClockSetup(eMMC, Data);
            if (status != MMC_SUCCESS) {
                MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_SET_MODE_FREQ_ERROR);
                return status;                                          
            }
            MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_SET_MODE_FREQ_SUCCESS);
            break;
    default:
        MMC_BRANCHTEST_PRINT(BRANCH_EMMC_RAW_SET_MODE_DEFAULT);
        status = MMC_MODE_ERROR;
        break;
    }

    return status;
}

/**
 * @brief Get EMMC capicity
 * 
 * @param Cardinfo EMMC Card info
 * @return uint32_t status
 */
uint32_t AlEmmc_GetCardInfo(SD_CardInfo *Cardinfo)
{
    MMC_ERR_TYPE status                         = MMC_SUCCESS;
    uint32_t tmp_rdblen                         = 0;
    uint32_t tmp_devsizemul                     = 0;
    uint32_t sec_count                          = 0;
    uint32_t arg_r                              = 0;
    uint32_t *ext_csd_addr                      = (uint32_t *)ext_csd_buf;
    __IO BLOCKCOUNT_R__BLOCKSIZE_R block        = {.d32 = 0,};
    __IO CMD_R__XFER_MODE_R reg                 = {.d32 = 0,};
    __IO MMC_CMD23_PARAM r3                     = {.d32 = 0,};

    Cardinfo->CardBlockSize     = 512;
    Cardinfo->SD_csd.DeviceSize = ((CsdTab[1]&0x3)<<10)|((CsdTab[2]>>22)&0x3FF);

    MMC_PRINT("csize is %d\r\n", Cardinfo->SD_csd.DeviceSize);
#ifdef USE_ERROR_BRANCH
    if (ERROR_BRANCH_CHECK_BIT_NOTSET(BERROR_BRANCH_EMMC_CARDSIZE_LESS2G)) {
        Cardinfo->SD_csd.DeviceSize = 0xFFE;
        ERROR_BRANCH_BIT_SET(BERROR_BRANCH_EMMC_CARDSIZE_LESS2G);
    }
#endif    
    if (Cardinfo->SD_csd.DeviceSize != 0xFFF) {   //less than 2GB
        MMC_BRANCHTEST_PRINT(BRANCH_EMMC_CARDSIZE_LESS2G);

        Cardinfo->SD_csd.DeviceSizeMul  = (uint8_t)((CsdTab[2]>>7)&0x7);
        Cardinfo->SD_csd.RdBlockLen     = ((CsdTab[1]>>8)&0xF);
        tmp_rdblen                      = (1 << Cardinfo->SD_csd.RdBlockLen);
        tmp_devsizemul                  = (1 << (Cardinfo->SD_csd.DeviceSizeMul+2));
        Cardinfo->CardCapacity          = (Cardinfo->SD_csd.DeviceSize + 1) * tmp_devsizemul * tmp_rdblen;
        
        MMC_PRINT("rd_block_len is %d\r\n", tmp_rdblen);
        MMC_PRINT("device size mult is %d\r\n", tmp_devsizemul);
    } else {  //greater than 2GB, use ext_csd register
        MMC_BRANCHTEST_PRINT(BRANCH_EMMC_CARDSIZE_MORE2G);

#ifdef _USE_SDMA
        __IO WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1 = {.d32 = 0,};

        r1.d32 = REG_READ(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
        r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;
        REG_WRITE(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);

        REG_WRITE(&(eMMC->sdmasa_r), ext_csd_addr);
        REG_WRITE(&(eMMC->adma_sa_low_r), ext_csd_addr);
#endif
        // send command 16
        MMC_PRINT("send cmd 16\r\n");

        MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

        arg_r = Cardinfo->CardBlockSize;  //block length  512
        REG_WRITE(&(eMMC->argument_r), arg_r);

        block.d32                   = 0;
        block.bit.xfer_block_size   = Cardinfo->CardBlockSize;
        block.bit.blockcount_r      = 0x1;
        REG_WRITE(&(eMMC->blockcount_r__blocksize), block.d32);

        reg.d32                     = 0;
        reg.bit.block_count_enable  = MMC_XM_BLOCK_COUNT_ENABLE;
        reg.bit.data_xfer_dir       = DATA_WRITE;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
        reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
        reg.bit.resp_type_select    = MMC_C_RESP_LEN_48;
        reg.bit.cmd_index           = SD_CMD_SET_BLOCKLEN;
        REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);
        
        MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_16_ERR);
        MMC_PRINT("block.d32 is %x\r\n", block.d32);
        MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

        // send command 23
        MMC_PRINT("send cmd 23\r\n");

        MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

        r3.d32              = 0;
        r3.bit.block_num    = 0x1;
        arg_r               = r3.d32;
        REG_WRITE(&(eMMC->argument_r), arg_r);

        block.d32                   = 0;
        block.bit.xfer_block_size   = Cardinfo->CardBlockSize;
        block.bit.blockcount_r      = 0x1;
        REG_WRITE(&(eMMC->blockcount_r__blocksize), block.d32);

        reg.d32                     = 0;
        reg.bit.block_count_enable  = MMC_XM_BLOCK_COUNT_ENABLE;
        reg.bit.data_xfer_dir       = DATA_WRITE;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
        reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
        reg.bit.resp_type_select    = MMC_C_RESP_LEN_48;
        reg.bit.cmd_index           = SD_CMD_SET_BLOCK_COUNT;
        REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

        MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_23_ERR);
        MMC_PRINT("block.d32 is %x\r\n", block.d32);
        MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

        // send command 8
        MMC_PRINT("send cmd 8\r\n");
        
        MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);

        arg_r = 0;
        REG_WRITE(&(eMMC->argument_r), arg_r);

        block.d32 = 0;
        block.bit.xfer_block_size = Cardinfo->CardBlockSize;
        block.bit.blockcount_r = 0x1;
        REG_WRITE(&(eMMC->blockcount_r__blocksize), block.d32);

        reg.d32                     = 0;
#ifdef _USE_SDMA
        reg.bit.dma_en              = MMC_XM_DMA_ENABLE;
#endif
        reg.bit.block_count_enable  = MMC_XM_BLOCK_COUNT_ENABLE;
        reg.bit.data_xfer_dir       = MMC_XM_DATA_XFER_DIR_READ;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        reg.bit.resp_type_select    = MMC_C_RESP_LEN_48;
        reg.bit.data_present_sel    = MMC_C_DATA_PRESENT;
        reg.bit.cmd_crc_chk_enable  = MMC_C_CMD_CRC_CHECK_ENABLE;
        reg.bit.cmd_idx_chk_enable  = MMC_C_CMD_IDX_CHECK_ENABLE;
        reg.bit.cmd_index           = SD_CMD_HS_SEND_EXT_CSD;
        REG_WRITE(&(eMMC->cmd_r__xfer_mode), reg.d32);

        MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_8_ERR);
#ifdef _USE_SDMA
        MMC_WAIT_TRANSFER_COMPLETE(eMMC, MMC_CMD_8_XFER_ERR);
#else
        MMC_TRANSFER_WITHOUT_DMA(eMMC, ext_csd_addr, MMC_CMD_8_XFER_ERR);
#endif
        MMC_PRINT("block.d32 is %x\r\n", block.d32);
        MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

        MMC_PRINT("get ext_csd\r\n");
        MMC_PRINT("%x, %x, %x, %x\r\n",ext_csd_buf[215], ext_csd_buf[214], ext_csd_buf[213], ext_csd_buf[212]);
        MMC_PRINT("%d, %d, %d, %d\r\n",ext_csd_buf[215], ext_csd_buf[214], ext_csd_buf[213], ext_csd_buf[212]);
        sec_count = (ext_csd_buf[215] << 24)|(ext_csd_buf[214] << 16)|(ext_csd_buf[213] << 8)|ext_csd_buf[212];
        MMC_PRINT("sec count is %d\r\n", sec_count);
        Cardinfo->CardCapacity = (uint64_t)sec_count * Cardinfo->CardBlockSize;
    }
    MMC_PRINT("emmc capicity is %lld, %llx\r\n", Cardinfo->CardCapacity, Cardinfo->CardCapacity);

    return status;
}

/*********************************************END OF FILE**********************/
