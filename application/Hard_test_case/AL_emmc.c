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

#define DEF_BLOCK_LEN   0x200

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
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir = DATA_READ;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.multi_blk_sel = MMC_XM_SEL_MULTI_BLOCK;
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

/***************************************************************************/
/**
 * @brief	read multi block size data 
 *
 * @param	readbuff reading data buffer 
 * @param	ReadAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_ReadSingleBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize)
{
    uint32_t status = MMC_SUCCESS;
    volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )readbuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    r1.d32 = 0;
    r1.bit.dat_xfer_width = MMC_HC1_DAT_XFER_WIDTH_4BIT;   //4-bit mode
    r1.bit.sd_bus_pwr_vdd1 = MMC_PC_SBP_VDD1_ON;   //PWR ON
    r1.bit.sd_bus_vol_vdd1 = MMC_PC_SBV_VDD1_3V3;   //3V
    REG_WRITE((u32*)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    REG_WRITE((u32*)&(eMMC->sdmasa_r), *Buffer_SingleBlock);
    REG_WRITE((u32*)&(eMMC->adma_sa_low_r), *Buffer_SingleBlock);
    r2.d32 = 0;
    REG_WRITE((u32*)&(eMMC->host_ctrl2_r__auto_cmd_stat), r2.d32);

	// send command 16
    arg_r = EMMC_CMD16_PARA_BLOCK_LEN_512;  //block length  512
    REG_WRITE((u32*)&(eMMC->argument_r), arg_r);
    block.d32 = REG_READ((u32*)&(eMMC->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 0x1;
    reg.d32 = REG_READ((u32*)&(eMMC->cmd_r__xfer_mode));
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    REG_WRITE((u32*)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((u32*)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC);

	// send command 17 read single block
	eMMC->argument_r = ReadAddr;
    reg.d32 = REG_READ((u32*)&(eMMC->cmd_r__xfer_mode));
    reg.bit.dma_en = MMC_XM_DMA_ENABLE;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_READ;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
    reg.bit.cmd_index = SD_CMD_READ_SINGLE_BLOCK;
    block.d32 = REG_READ((u32*)&(eMMC->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 0x1;
    REG_WRITE((u32*)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((u32*)&(eMMC->cmd_r__xfer_mode), reg.d32);
	MMC_WAIT_CMD_COMPLETE(eMMC);
    MMC_WAIT_TRANSFER_COMPLETE(eMMC);

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
u32 EMMC_Init(void)
{
    int status = MMC_SUCCESS;

    status = HostControllerSetup(eMMC);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    status = HostControllerClockSetup(eMMC, MMC_FREQ_400K);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    //sleep(200);
    MMC_DELAY_MS(10);
    status = InitInterruptSetting(eMMC);
    if (status != MMC_SUCCESS) {
		goto END;
	}
    //sleep(200);
    MMC_DELAY_MS(10);
    status = SendInitCmdEmmc();
    if (status != MMC_SUCCESS) {
		goto END;
	}
    status = SwitchDataWidthEmmc();
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
u32 Csu_RawEmmcInit(RawEmmcParam_t *Param) 
{  
    u32 status = MMC_SUCCESS;
    status = EMMC_Init();
    if(status != MMC_SUCCESS){
        return status;
    }
    Param->EmmcId = SDCardInfo.SD_cid.ManufacturerID;
    Param->EmmcSize = SDCardInfo.CardCapacity/2;
    printf("emmc ID is %d, capacity is %d kBytes!\r\n", Param->EmmcId, Param->EmmcSize);
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
u32 Csu_RawEmmcRead(u32 Offset, u8* Dest, u32 Length) 
{  
    u32 status = MMC_SUCCESS;
    u32 blocksize = SDCardInfo.CardBlockSize;
    u32 offsetblock = Offset/blocksize;
    u32 offsetbytes = Offset%blocksize;
    u32 restorebytes = blocksize - offsetbytes;
    u8 *pdestaddr = Dest;
    u32 restorelength = Length % blocksize;
    u32 blocknum = Length / blocksize + (restorelength > restorebytes)? 1 : 0;
    u32 lastblocklength = (restorelength > restorebytes)? \
                        restorelength - restorebytes : restorelength + blocksize - restorebytes;
    if(restorelength > restorebytes){
        blocknum += 1;
    }

    for(u32 i = offsetblock; i < blocknum; i++){
        if(i == 0){
            status = EMMC_ReadSingleBlock(blockbuf, i, blocksize);
            if(status != MMC_SUCCESS){
                return status;
            }
            memcpy(pdestaddr, &blockbuf[offsetbytes], restorebytes);
            pdestaddr += restorebytes;
            memset(blockbuf, 0, blocksize);
        }else if(i == blocknum - 1){
            status = EMMC_ReadSingleBlock(blockbuf, i, blocksize);
            if(status != MMC_SUCCESS){
                return status;
            }
            memcpy(pdestaddr, blockbuf, lastblocklength);
            pdestaddr += lastblocklength;
            memset(blockbuf, 0, blocksize);
        }else{
            status = EMMC_ReadSingleBlock(pdestaddr, i, blocksize);
            if(status != MMC_SUCCESS){
                return status;
            }
            pdestaddr += blocksize;
        }
    }
    return status;
}


/*********************************************END OF FILE**********************/
