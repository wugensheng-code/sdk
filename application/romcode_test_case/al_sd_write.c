#include "al_sd_write.h"

/***************************************************************************/
/**
 * @brief	write single block size data 
 *
 * @param	writebuff reading data buffer 
 * @param	WriteAddr read start address
 * @param	BlockSize read data block size
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
uint32_t AlSd_WriteSingleBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
	uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;
    __IO CMD_R__XFER_MODE_R reg = {.d32 = 0,};
    __IO uint32_t arg_r = 0;
    __IO BLOCKCOUNT_R__BLOCKSIZE_R block = {.d32 = 0,};
    __IO MMC_CMD23_PARAM r3 = {.d32 = 0,};

    MMC_PRINT("AlSd_WriteSingleBlock\r\n");
#ifdef _USE_SDMA
    __IO WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1 = {.d32 = 0,};
    r1.d32 = REG_READ((unsigned long)&(SDIO->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32));
    MMC_PRINT("r1 is  %x, %d\r\n", r1.d32, r1.d32);
    r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;
    REG_WRITE((unsigned long)&(SDIO->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32), r1.d32);
    REG_WRITE((unsigned long)&(SDIO->sdmasa_r), (uintptr_t)Buffer_SingleBlock);
    //REG_WRITE((unsigned long)&(SDIO->adma_sa_low_r), (uintptr_t)Buffer_SingleBlock);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);
#endif
	// send command 16  default 512 Bytes
    MMC_PRINT("send command 16\r\n");
    MMC_CHECK_LINE_INHIBIT(SDIO);
    MMC_CLEAR_STATUS(SDIO);
    arg_r = BlockSize;
    REG_WRITE((unsigned long)&(SDIO->argument_r), arg_r);
    block.d32 = 0;//REG_READ((unsigned long)&(SDIO->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 1;
    reg.d32 = 0;//REG_READ((unsigned long)&(SDIO->cmd_r__xfer_mode));
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    REG_WRITE((unsigned long)&(SDIO->blockcount_r__blocksize.d32), block.d32);
    REG_WRITE((unsigned long)&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_16_ERR);

    // send command 23
    MMC_PRINT("send cmd 23\r\n");
    MMC_CHECK_LINE_INHIBIT(SDIO);
    MMC_CLEAR_STATUS(SDIO);
    r3.d32 = 0;
    r3.bit.block_num = 0x1;
    arg_r = r3.d32;
    REG_WRITE((unsigned long)&(SDIO->argument_r), arg_r);
    reg.d32 = 0;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCK_COUNT;
    REG_WRITE((unsigned long)&(SDIO->cmd_r__xfer_mode), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_23_ERR);

	// send command 24
    MMC_PRINT("send cmd 24\r\n");
    MMC_CHECK_LINE_INHIBIT(SDIO);
    MMC_CLEAR_STATUS(SDIO);
    arg_r = WriteAddr;
    REG_WRITE((unsigned long)&(SDIO->argument_r), arg_r);
    reg.d32 = 0;//REG_READ((unsigned long)&(SDIO->cmd_r__xfer_mode));
#ifdef _USE_SDMA
    reg.bit.dma_en = MMC_XM_DMA_ENABLE;
#endif
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_WRITE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
    reg.bit.cmd_crc_chk_enable = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable = MMC_C_CMD_IDX_CHECK_ENABLE;
    reg.bit.cmd_index = SD_CMD_WRITE_SINGLE_BLOCK;
    block.d32 = 0;//REG_READ((unsigned long)&(SDIO->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 0x1;
    REG_WRITE((unsigned long)&(SDIO->blockcount_r__blocksize.d32), block.d32);
    REG_WRITE((unsigned long)&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
	MMC_WAIT_CMD_COMPLETE(SDIO, MMC_CMD_24_ERR);
#ifdef _USE_SDMA
    MMC_WAIT_TRANSFER_COMPLETE(SDIO, MMC_CMD_24_XFER_ERR);
#else
    MMC_TRANSFER_WITHOUT_DMA(SDIO, Buffer_SingleBlock, MMC_CMD_24_XFER_ERR);
#endif

	return MMC_SUCCESS;
}

