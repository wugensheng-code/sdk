
#include "al_emmc_write.h"

/***************************************************************************/
/**
 * @brief	write multi block size data 
 *
 * @param	Writebuff reading data buffer 
 * @param	WriteAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
uint32_t AlEmmc_WriteSingleBlock(uint8_t *Writebuff, uint32_t WriteAddr, uint16_t BlockSize)
{
    uint32_t status = MMC_SUCCESS;
	uint32_t* Buffer_SingleBlock = (uint32_t* )Writebuff;
    CMD_R__XFER_MODE_R reg = {.d32 = 0,};
    uint32_t arg_r = 0;
    BLOCKCOUNT_R__BLOCKSIZE_R block = {.d32 = 0,};
    MMC_CMD23_PARAM r3 = {.d32 = 0,};

    MMC_PRINT("AlEmmc_WriteSingleBlock\r\n");
#ifdef _USE_SDMA
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1 = {.d32 = 0,};
    r1.d32 = REG_READ((unsigned long)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1));
    MMC_PRINT("r1 is  %x, %d\r\n", r1.d32, r1.d32);
    r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;
    REG_WRITE((unsigned long)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    REG_WRITE((unsigned long)&(eMMC->sdmasa_r), (unsigned long)Buffer_SingleBlock);
#endif

	// send command 16
    MMC_PRINT("write send cmd 16\r\n");
    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);
    arg_r = BlockSize;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    block.d32 = 0;
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 0x1;
    reg.d32 = 0;
    reg.bit.data_xfer_dir = DATA_WRITE;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    reg.bit.cmd_crc_chk_enable = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE((unsigned long)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_16_ERR);

    // send command 23
    MMC_PRINT("send cmd 23\r\n");
    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);
    r3.d32 = 0;
    r3.bit.block_num = 0x1;
    arg_r = r3.d32;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = 0;
    reg.bit.data_xfer_dir = DATA_WRITE;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCK_COUNT;
    reg.bit.cmd_crc_chk_enable = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable = MMC_C_CMD_IDX_CHECK_ENABLE;
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
    MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_23_ERR);


	// send command 24
    MMC_PRINT("send cmd 24\r\n");
    MMC_CHECK_LINE_AND_CLEAR_STATUS(eMMC);
    arg_r = WriteAddr;
    REG_WRITE((unsigned long)&(eMMC->argument_r), arg_r);
    reg.d32 = 0;
#ifdef _USE_SDMA
    reg.bit.dma_en = MMC_XM_DMA_ENABLE;
#endif
    reg.bit.data_xfer_dir = DATA_WRITE;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
    reg.bit.cmd_crc_chk_enable = MMC_C_CMD_CRC_CHECK_ENABLE;
    reg.bit.cmd_idx_chk_enable = MMC_C_CMD_IDX_CHECK_ENABLE;
    reg.bit.cmd_index = SD_CMD_WRITE_SINGLE_BLOCK;
    block.d32 = 0;
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 0x1;
    REG_WRITE((unsigned long)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((unsigned long)&(eMMC->cmd_r__xfer_mode), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
	MMC_WAIT_CMD_COMPLETE(eMMC, MMC_CMD_24_ERR);
#ifdef _USE_SDMA
    MMC_WAIT_TRANSFER_COMPLETE(eMMC, MMC_CMD_24_XFER_ERR);
#else
    MMC_TRANSFER_WITHOUT_DMA(eMMC, Buffer_SingleBlock, MMC_CMD_24_XFER_ERR);
#endif
	return MMC_SUCCESS;
}


