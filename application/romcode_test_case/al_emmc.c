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

SD_CardInfo EmmcCardInfo;
static __IO uint32_t rca = 0;
__attribute__((aligned(4)))
static __IO uint8_t ext_csd_buf[512];
__attribute__((packed))

/***************************************************************************/
/**
 * @brief	set the SD/EMMC host clock 
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
uint32_t AlEmmc_HostControllerClockSetup(volatile DWC_mshc_block_registers* Ptr, uint32_t freq)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r1;
    MMC_PRINT("AlEmmc_HostControllerClockSetup\r\n");

    //  Host Controller Clock Setup
    if (freq == MMC_FREQ_10M)
    {
        MMC_PRINT("MMC_FREQ_10M\r\n");
        /*r1.d32 = 0;
        r1.bit.internal_clk_en = MMC_CC_INTER_CLK_ENABLE;       //Oscillate
        r1.bit.clk_gen_select = MMC_CC_CLK_GEN_SEL_PROGRAM;        //Programmable Clock Mode
        r1.bit.tout_cnt = MMC_TC_TOUT_CNT_2_27;
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
        r1.d32 = REG_READ((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl));
        r1.bit.pll_enable = MMC_CC_PLL_ENABLE;            //PLL enabled
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
        MMC_PRINT("r1.d32 is %x\r\n", r1.d32);

        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);     //clk soft reset
        _delay_us(1);
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);

        r1.d32 = 0;
        r1.bit.internal_clk_en = MMC_CC_INTER_CLK_ENABLE;       //Oscillate
        r1.bit.clk_gen_select = MMC_CC_CLK_GEN_SEL_PROGRAM;        //Programmable Clock Mode
        r1.bit.tout_cnt = MMC_TC_TOUT_CNT_2_27;
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
        r1.d32 = REG_READ((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.pll_enable = MMC_CC_PLL_ENABLE;            //PLL enabled
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
        r1.d32 = REG_READ((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.sd_clk_en = MMC_CC_SD_CLK_ENABLE;             //Enable SDCLK/RCLK
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_PRINT("r1.d32 is %x\r\n", r1.d32);*/
    }else{  //default
        MMC_PRINT("MMC_FREQ_400K\r\n");
        /*r1.d32 = 0;
        r1.bit.internal_clk_en = MMC_CC_INTER_CLK_ENABLE;       //Oscillate
        r1.bit.tout_cnt = MMC_TC_TOUT_CNT_2_27;
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
        r1.d32 = REG_READ((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.pll_enable = MMC_CC_PLL_ENABLE;            //PLL enabled
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
        MMC_PRINT("r1.d32 is %x\r\n", r1.d32);
        
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000008);     //clk soft reset
        _delay_us(1);
        REG_WRITE(TOP_NS__CFG_CTRL_SDIO0_ADDR, 0x00000000);*/

        /*r1.d32 = 0;
        r1.bit.internal_clk_en = MMC_CC_INTER_CLK_ENABLE;       //Oscillate
        r1.bit.tout_cnt = MMC_TC_TOUT_CNT_2_27;
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
        r1.d32 = REG_READ((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.pll_enable = MMC_CC_PLL_ENABLE;            //PLL enabled
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_WAIT_CLK_STABLE(Ptr);
        r1.d32 = REG_READ((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32));
        r1.bit.sd_clk_en = MMC_CC_SD_CLK_ENABLE;             //Enable SDCLK/RCLK
        REG_WRITE((unsigned long)&(Ptr->sw_rst_r__tout_ctrl_r__clk_ctrl.d32), r1.d32);
        MMC_PRINT("r1.d32 is %x\r\n", r1.d32);*/
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
uint32_t AlEmmc_SendInitCmd()
{
    volatile unsigned int response01;
    volatile unsigned int validvoltage;
    uint32_t status = MMC_SUCCESS;
    __IO CMD_R__XFER_MODE_R reg;
    __IO OCR_R r1;
    __IO uint32_t arg_r;
    __IO uint32_t r;

    MMC_PRINT("SendInitCmdEmmc\r\n");
    // send command 0   go idle state
    MMC_PRINT("send command 0\r\n");
    arg_r = EMMC_CMD0_PARA_GO_IDLE_STATE;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = 0;
    reg.bit.cmd_index = SD_CMD_GO_IDLE_STATE;
    reg.bit.data_xfer_dir = DATA_READ;
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_0_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);
    MMC_DELAY_MS(1);

    validvoltage = 0;
    MTIMER_OUT_CONDITION(EMMC_GET_VALID_VOLTAGE_TIMEOUT_VAL, &MmcMtimer, validvoltage != 1){
        //CMD1  arg = OCR register check voltage valid
        MMC_PRINT("send command 1\r\n");
        r1.d32 = 0;
        r = REG_READ((unsigned long)IO_BANK1_REF);
        MMC_PRINT("r is %x\r\n", r);
        if(MMC_IO_BANK1_SUPPORT_1V8(r) == 1){
            r1.bit.voltage_mode = EMMC_OCR_DUAL_VOLTAGE;   //1.8V and 3v3
        }else{
            r1.bit.voltage_mode = EMMC_OCR_HIGH_VOLTAGE;    //3v3 only
        }
        r1.bit.voltage2v7_3v6 = EMMC_OCR_VOLTAGE_2V7_3V6;  
        r1.bit.access_mode = EMMC_OCR_ACCESS_MODE_SECTOR_MODE;  //sector mode
        arg_r = r1.d32;
        REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
        reg.d32 = REG_READ((unsigned long)&(eMMC->cmd_r__xfer_mode));
        reg.bit.cmd_index = SD_CMD_SEND_OP_COND;
        reg.bit.resp_type_select = MMC_Response_Short;
        reg.bit.data_xfer_dir = DATA_READ;
        REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
        MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_1_ERR);
        MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);
        response01 = REG_READ((unsigned long)&(eMMC->resp01));
    	validvoltage = (((response01 >> 31) == 1) ? 1:0);
    }
    if(Mtimer_IsTimerOut(&MmcMtimer)){
        status = MMC_CHECK_VOLT_TIMEOUT;
        return status;
    }

    // send command 2 get CID   Device IDentification
    MMC_PRINT("send command 2\r\n");
    arg_r = 0;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ((unsigned long)&(eMMC->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_ALL_SEND_CID;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir = DATA_READ;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.multi_blk_sel = MMC_XM_SEL_SINGLE_BLOCK;             //?
    reg.bit.resp_type_select = MMC_Response_Long;
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_2_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    // send command 3 set relative device address
    MMC_PRINT("send command 3\r\n");
    arg_r = EMMC_CMD3_PARA_DEFAULT_VAL;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ((unsigned long)&(eMMC->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SET_REL_ADDR;
    reg.bit.resp_type_select = MMC_Response_Short;
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_3_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);
    rca = REG_READ((unsigned long)&(eMMC->resp01)) & 0xFFFF0000;
    rca = EMMC_CMD3_PARA_DEFAULT_VAL;

    // send command 9 get addressed device's CSD on CMD line
    MMC_PRINT("send command 9\r\n");
    arg_r = rca;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ((unsigned long)&(eMMC->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SEND_CSD;
    reg.bit.resp_type_select = MMC_Response_Long;
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_9_ERR);
    CsdTab[3] = REG_READ((unsigned long)&(eMMC->resp01));
    CsdTab[2] = REG_READ((unsigned long)&(eMMC->resp23));
    CsdTab[1] = REG_READ((unsigned long)&(eMMC->resp45));
    CsdTab[0] = REG_READ((unsigned long)&(eMMC->resp67));
    MMC_PRINT("CsdTab 0~4 is %x, %x, %x, %x\r\n", CsdTab[0], CsdTab[1], CsdTab[2], CsdTab[3]);

    // send command 10  get addressed device's CID on CMD line
    arg_r = rca;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ((unsigned long)&(eMMC->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SEND_CID;
    reg.bit.resp_type_select = MMC_Response_Long;
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_10_ERR);
    CidTab[3] = REG_READ((unsigned long)&(eMMC->resp01));
    CidTab[2] = REG_READ((unsigned long)&(eMMC->resp23));
    CidTab[1] = REG_READ((unsigned long)&(eMMC->resp45));
    CidTab[0] = REG_READ((unsigned long)&(eMMC->resp67));
    MMC_PRINT("CidTab 0~4 is %x, %x, %x, %x\r\n", CidTab[0], CidTab[1], CidTab[2], CidTab[3]);
    EmmcCardInfo.SD_cid.ManufacturerID =  ((CidTab[0] >> 16)&0xFF);
    
    // send command 7   selected/deselected card
    MMC_PRINT("send command 7\r\n");
    arg_r = rca;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ((unsigned long)&(eMMC->cmd_r__xfer_mode));
    reg.bit.cmd_index = SD_CMD_SEL_DESEL_CARD;
    reg.bit.resp_type_select = MMC_Response_Short_48B;
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_7_ERR);
    MMC_PRINT("reg.d32 is %x, %d\r\n", reg.d32, reg.d32);

    return MMC_SUCCESS;
}

#if 0
/***************************************************************************/
/**
 * @brief	change the bit width
 *
 * @param	None
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
uint32_t AlEmmc_SwitchDataWidth()
{
    uint32_t status = MMC_SUCCESS;
    CMD_R__XFER_MODE_R reg;
    uint32_t arg_r;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;

    // send command 6
    arg_r = EMMC_CMD6_PARA_8_BIT_WIDTH_BUS;     //set sd model data width=8
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.bit.cmd_index = SD_CMD_HS_SWITCH;
    reg.bit.resp_type_select = MMC_Response_Short;
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC);
    r1.d32 = REG_READ((unsigned long)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
    r1.bit.extdat_xfer = 0x1;
    REG_WRITE((unsigned long)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    //sleep(2000);
    MMC_DELAY_MS(1);
    return MMC_SUCCESS;
}
#endif

/***************************************************************************/
/**
 * @brief	read multi block size data 
 *
 * @param	Readbuff reading data buffer 
 * @param	ReadAddr read start address
 * @param	BlockSize read data block size
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
uint32_t AlEmmc_ReadSingleBlock(uint8_t *Readbuff, uint32_t ReadAddr, uint16_t BlockSize)
{
    uint32_t status = MMC_SUCCESS;
    volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )Readbuff;
    __IO CMD_R__XFER_MODE_R reg;
    __IO WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    __IO HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    __IO uint32_t arg_r;
    __IO BLOCKCOUNT_R__BLOCKSIZE_R block;
    __IO MMC_CMD23_PARAM r3;

    r1.d32 = REG_READ((unsigned long)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
    MMC_PRINT("r1 is  %x, %d\r\n", r1.d32, r1.d32);
    r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;
    REG_WRITE((unsigned long)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    REG_WRITE((unsigned long)&(eMMC->sdmasa_r), (uint32_t)Buffer_SingleBlock);
    REG_WRITE((unsigned long)&(eMMC->adma_sa_low_r), (uint32_t)Buffer_SingleBlock);

	// send command 16
    MMC_PRINT("send cmd 16\r\n");
    arg_r = EMMC_CMD16_PARA_BLOCK_LEN_512;  //block length  512
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    //block.d32 = REG_READ((unsigned long)&(eMMC->blockcount_r__blocksize));
    block.d32 = 0;
    block.bit.xfer_block_size = EMMC_CMD16_PARA_BLOCK_LEN_512;
    block.bit.blockcount_r = 0x1;
    reg.d32 = 0;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    REG_WRITE((unsigned long)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_16_ERR);

    // send command 23
    MMC_PRINT("send cmd 23\r\n");
    r3.d32 = 0;
    r3.bit.block_num = 0x1;
    arg_r = r3.d32;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = 0;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCK_COUNT;
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_23_ERR);

	// send command 17 read single block
    MMC_PRINT("send cmd 17\r\n");
    arg_r = ReadAddr;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = 0;//REG_READ((unsigned long)&(eMMC->cmd_r__xfer_mode));
    reg.bit.dma_en = MMC_XM_DMA_ENABLE;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_READ;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    //reg.bit.resp_int_disable = MMC_XM_RESP_INT_DISABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
    reg.bit.cmd_crc_chk_enable = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable = MMC_C_CMD_IDX_CHECK_ENABLE;
    reg.bit.cmd_index = SD_CMD_READ_SINGLE_BLOCK;
    block.d32 = REG_READ((unsigned long)&(eMMC->blockcount_r__blocksize));
    block.bit.xfer_block_size = EMMC_CMD16_PARA_BLOCK_LEN_512;
    block.bit.blockcount_r = 0x1;
    REG_WRITE((unsigned long)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
	MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_17_ERR);
    //Resp[0] = REG_READ((unsigned long)&(eMMC->resp01));
    //MMC_PRINT("resp01 %x\r\n", Resp[0]);
    MMC_WAIT_TRANSFER_COMPLETE(eMMC, MMC_CMD_17_XFER_ERR);

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

    status = HostControllerSetup(eMMC);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    /*status = Csu_RawEmmcSetMode(MMC_MODE_FREQ, EMMC_FREQ_400K);
    //status = AlEmmc_HostControllerClockSetup(eMMC, MMC_FREQ_400K);
    if (status != MMC_SUCCESS) {
		goto END;
	}*/
    MMC_DELAY_MS(10);
    status = InitInterruptSetting(eMMC);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    MMC_DELAY_MS(10);
    status = AlEmmc_SendInitCmd();
    if (status != MMC_SUCCESS) {
		goto END;
	}
    status = AlEmmc_GetCardInfo(&EmmcCardInfo);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    
    status = MMC_SUCCESS;
END:
	return status;
}

/**  
 * This function initializes raw EMMC flash controller, driver and device. 
 * @param Param is the pointer to EmmcParam_t 
 * @return AL_SUCCESS on success, error number on failed. 
 * @note none 
*/ 
uint32_t Csu_RawEmmcInit(RawEmmcParam_t *Param) 
{  
    uint32_t status = MMC_SUCCESS;
    status = AlEmmc_Init();
    if(status != MMC_SUCCESS){
        return status;
    }
    Param->EmmcId = EmmcCardInfo.SD_cid.ManufacturerID;
    Param->EmmcSize = EmmcCardInfo.CardCapacity / EmmcCardInfo.CardBlockSize / 2;
    printf("emmc ID is %d, capacity is %ld kBytes!\r\n", Param->EmmcId, Param->EmmcSize);
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
uint32_t Csu_RawEmmcRead(uint32_t Offset, uint8_t* Dest, uint32_t Length) 
{  
    uint32_t status = MMC_SUCCESS;
    uint32_t blocksize = EmmcCardInfo.CardBlockSize;       //block size
    uint32_t offsetblock = Offset/blocksize;             //block offset
    uint32_t offsetbytes = Offset%blocksize;             //bytes offset in block
    uint32_t restorebytes = blocksize - offsetbytes;    //valid bytes in first block
    uint8_t *pdestaddr = Dest;
    uint32_t restorelength = Length % blocksize; //length needy last bytes
    uint32_t blocknum = 1 + Length / blocksize;  //block num need to read
    uint32_t lastblocklength = (restorelength > restorebytes)? \
                        restorelength - restorebytes : restorelength + blocksize - restorebytes;    //valid bytes in last block
    if(restorelength > restorebytes){
        blocknum += 1;
    }
    MMC_PRINT("offsetblock: %d\toffsetbytes: %d\trestorebytes: %d\r\n", offsetblock, offsetbytes, restorebytes);
    MMC_PRINT("blocknum: %d\tlastblocklength: %d\t\r\n", blocknum, lastblocklength);
    for(uint32_t i = offsetblock; i < blocknum; i++){
        if(i == offsetblock){
            status = AlEmmc_ReadSingleBlock(FlashSharedBuf, i, blocksize);
            if(status != MMC_SUCCESS){
                return status;
            }
            memcpy(pdestaddr, &FlashSharedBuf[offsetbytes], restorebytes);
            pdestaddr += restorebytes;
        }else if(i == blocknum - 1){
            status = AlEmmc_ReadSingleBlock(FlashSharedBuf, i, blocksize);
            if(status != MMC_SUCCESS){
                return status;
            }
            memcpy(pdestaddr, FlashSharedBuf, lastblocklength);
            pdestaddr += lastblocklength;
        }else{
            status = AlEmmc_ReadSingleBlock(pdestaddr, i, blocksize);
            if(status != MMC_SUCCESS){
                return status;
            }
            pdestaddr += blocksize;
        }
    }
    return status;
}

/**
 * @brief 
 * 
 * @param Mode 
 * @param Data 
 * @return uint32_t 
 */
uint32_t Csu_RawEmmcSetMode(uint32_t Mode, uint32_t Data)
{
    uint32_t status = MMC_SUCCESS;
#if 0
    if(Mode == EMMC_MODE_FREQ){
        if(Data == EMMC_FREQ_400K){
            status = AlEmmc_HostControllerClockSetup(eMMC, MMC_FREQ_400K);
            if (status != MMC_SUCCESS) {
                return status;
            }
        }
    }
#endif
#if 1
    switch(Mode){
        case MMC_MODE_FREQ:
            /*if(Data == EMMC_FREQ_400K){
                status = AlEmmc_HostControllerClockSetup(eMMC, MMC_FREQ_400K);
                if (status != MMC_SUCCESS) {
                    return status;
                }
            }*/
            switch(Data){
                case EMMC_FREQ_400K:
                    printf("EMMC_FREQ_400K\r\n");
                    status = AlEmmc_HostControllerClockSetup(eMMC, MMC_FREQ_400K);
                    if (status != MMC_SUCCESS) {
                        return status;
                    }
                    break;

                case EMMC_FREQ_10M:
#if 0
                    printf("EMMC_FREQ_10M\r\n");
                    status = AlEmmc_HostControllerClockSetup(eMMC, MMC_FREQ_10M);
#endif
                    if (status != MMC_SUCCESS) {
                        return status;
                    }

                    break;

                default:
                    status = MMC_WRONG_FREQ;
                    break;

            }
            break;
    default:
        status = MMC_MODE_ERROR;
        break;
    }
#endif
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
    MMC_ERR_TYPE status = MMC_SUCCESS;
    uint32_t tmp_rdblen = 1;
    uint32_t tmp_devsizemul = 1;
    uint32_t i = 0;
    uint32_t sec_count = 0;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R r4;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;
    CMD_R__XFER_MODE_R reg;
    __IO MMC_CMD23_PARAM r3;
    uint32_t *ext_csd_addr = (uint32_t *)ext_csd_buf;

    Cardinfo->CardBlockSize = 512;
    Cardinfo->SD_csd.DeviceSize = ((CsdTab[1]&0x3)<<10)|((CsdTab[2]>>22)&0x3FF);
    MMC_PRINT("csize is %d\r\n", Cardinfo->SD_csd.DeviceSize);
    if(Cardinfo->SD_csd.DeviceSize != 0xFFF){   //less than 2GB
        Cardinfo->SD_csd.DeviceSizeMul = (uint8_t)((CsdTab[2]>>7)&0x7);
        Cardinfo->SD_csd.RdBlockLen = ((CsdTab[1]>>8)&0xF);
        for(i = 0; i < Cardinfo->SD_csd.RdBlockLen; i++){
            tmp_rdblen *= 2;
        }
        for(i = 0; i < Cardinfo->SD_csd.DeviceSizeMul+2; i++){  //C_SIZE_MULT < 8, MULT = 2^(C_SIZE_MULT+2)
            tmp_devsizemul *= 2;
        }
        MMC_PRINT("rd_block_len is %d\r\n", tmp_rdblen);
        MMC_PRINT("device size mult is %d\r\n", tmp_devsizemul);
        Cardinfo->CardCapacity = (Cardinfo->SD_csd.DeviceSize + 1) * tmp_devsizemul * tmp_rdblen;
    }else{  //greater than 2GB, use ext_csd register
        r1.d32 = REG_READ((unsigned long)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
        r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;
        REG_WRITE((unsigned long)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
        REG_WRITE((unsigned long)&(eMMC->sdmasa_r), (uint32_t *)ext_csd_addr);
        REG_WRITE((unsigned long)&(eMMC->adma_sa_low_r), (uint32_t *)ext_csd_addr);

        // send command 16
        MMC_PRINT("send cmd 16\r\n");
        arg_r = EMMC_CMD16_PARA_BLOCK_LEN_512;  //block length  512
        REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
        //block.d32 = REG_READ((unsigned long)&(eMMC->blockcount_r__blocksize));
        block.d32 = 0;
        block.bit.xfer_block_size = Cardinfo->CardBlockSize;
        block.bit.blockcount_r = 0x1;
        //reg.d32 = REG_READ((unsigned long)&(eMMC->cmd_r__xfer_mode));
        reg.d32 = 0;
        reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
        reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
        REG_WRITE((unsigned long)&(eMMC->blockcount_r__blocksize), block.d32);
        REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
        MMC_PRINT("block.d32 is %x\r\n", block.d32);
        MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
        MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_16_ERR);

        // send command 23
        MMC_PRINT("send cmd 23\r\n");
        r3.d32 = 0;
        r3.bit.block_num = 0x1;
        arg_r = r3.d32;
        REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
        block.d32 = 0;
        block.bit.xfer_block_size = Cardinfo->CardBlockSize;
        block.bit.blockcount_r = 0x1;
        reg.d32 = 0;
        reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
        reg.bit.cmd_index = SD_CMD_SET_BLOCK_COUNT;
        REG_WRITE((unsigned long)&(eMMC->blockcount_r__blocksize), block.d32);
        REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
        MMC_PRINT("block.d32 is %x\r\n", block.d32);
        MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
        MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_23_ERR);

        r4.d32 = REG_READ((unsigned long)&(eMMC->sw_rst_r__tout_ctrl_r__clk_ctrl));
        MMC_PRINT("r4.d32 is %x\r\n", r4.d32);

        // send command 8
        MMC_PRINT("send cmd 8\r\n");
        arg_r = 0;
        REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
        reg.d32 = 0;//REG_READ((unsigned long)&(eMMC->cmd_r__xfer_mode));
        reg.bit.dma_en = MMC_XM_DMA_ENABLE;
        reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
        reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_READ;
        reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
        reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
        reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
        reg.bit.cmd_crc_chk_enable = MMC_C_CMD_CRC_CHECK_ENABLE;
        reg.bit.cmd_idx_chk_enable = MMC_C_CMD_IDX_CHECK_ENABLE;
        reg.bit.cmd_index = SD_CMD_HS_SEND_EXT_CSD;
        block.d32 = 0;//REG_READ((unsigned long)&(eMMC->blockcount_r__blocksize));
        block.bit.xfer_block_size = Cardinfo->CardBlockSize;
        block.bit.blockcount_r = 0x1;
        REG_WRITE((unsigned long)&(eMMC->blockcount_r__blocksize), block.d32);
        REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
        MMC_PRINT("block.d32 is %x\r\n", block.d32);
        MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
        MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_8_ERR);
        MMC_WAIT_TRANSFER_COMPLETE(eMMC, MMC_CMD_8_XFER_ERR);

        MMC_PRINT("get ext_csd\r\n");
        MMC_PRINT("%x, %x, %x, %x\r\n",ext_csd_buf[215], ext_csd_buf[214], ext_csd_buf[213], ext_csd_buf[212]);
        MMC_PRINT("%d, %d, %d, %d\r\n",ext_csd_buf[215], ext_csd_buf[214], ext_csd_buf[213], ext_csd_buf[212]);
        sec_count = (ext_csd_buf[215] << 24)|(ext_csd_buf[214] << 16)|(ext_csd_buf[213] << 8)|ext_csd_buf[212];
        MMC_PRINT("sec count is %d\r\n", sec_count);
        Cardinfo->CardCapacity = (uint64_t)sec_count * Cardinfo->CardBlockSize;

        r4.d32 = REG_READ((unsigned long)&(eMMC->sw_rst_r__tout_ctrl_r__clk_ctrl));
        MMC_PRINT("r4.d32 is %x\r\n", r4.d32);

        //Csu_RawEmmcSetMode(EMMC_MODE_FREQ, EMMC_FREQ_400K);

        r4.d32 = REG_READ((unsigned long)&(eMMC->sw_rst_r__tout_ctrl_r__clk_ctrl));
        MMC_PRINT("r4.d32 is %x\r\n", r4.d32);

    }
    MMC_PRINT("emmc capicity is %lld, %llx\r\n", Cardinfo->CardCapacity, Cardinfo->CardCapacity);
    return status;
}

/*********************************************END OF FILE**********************/
