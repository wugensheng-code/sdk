
#include "AL_emmc_write.h"

/***************************************************************************/
/**
 * @brief	write multi block size data 
 *
 * @param	writebuff reading data buffer 
 * @param	WriteAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	MMC_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_WriteSingleBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize)
{
    uint32_t status = MMC_SUCCESS;
	volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;
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
    arg_r = EMMC_CMD16_PARA_BLOCK_LEN_512;
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


	// send command 24
    arg_r = WriteAddr;
    REG_WRITE((u32*)&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ((u32*)&(eMMC->cmd_r__xfer_mode));
    reg.bit.dma_en = MMC_XM_DMA_ENABLE;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_WRITE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
    reg.bit.cmd_index = SD_CMD_WRITE_SINGLE_BLOCK;
    block.d32 = REG_READ((u32*)&(eMMC->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 0x1;
    REG_WRITE((u32*)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((u32*)&(eMMC->cmd_r__xfer_mode), reg.d32);
	MMC_WAIT_CMD_COMPLETE(eMMC);
    MMC_WAIT_TRANSFER_COMPLETE(eMMC);

	return MMC_SUCCESS;
}


