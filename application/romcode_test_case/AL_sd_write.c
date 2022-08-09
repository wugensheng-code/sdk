#include "AL_sd_write.h"

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
u32 SD_WriteSingleBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize)
{
    MMC_ERR_TYPE status = MMC_SUCCESS;
	volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    MMC_PRINT("SD_WriteMultiBlocks\r\n");
    r1.d32 = 0;
    r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;
    r1.bit.dat_xfer_width = MMC_HC1_DAT_XFER_WIDTH_4BIT;   //4-bit mode
    r1.bit.sd_bus_pwr_vdd1 = MMC_PC_SBP_VDD1_ON;   //PWR ON
    r1.bit.sd_bus_vol_vdd1 = MMC_PC_SBV_VDD1_3V3;   //3V
    REG_WRITE(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32), r1.d32);
    REG_WRITE(&(eMMC->sdmasa_r), Buffer_SingleBlock);
    REG_WRITE(&(eMMC->adma_sa_low_r), Buffer_SingleBlock);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);
    r2.d32 = 0;
    REG_WRITE(&(eMMC->host_ctrl2_r__auto_cmd_stat.d32), r2.d32);
    MMC_PRINT("r2.d32 is %x\r\n", r2.d32);

	// send command 16  default 512 Bytes
    /*MMC_PRINT("send command 16\r\n");
    arg_r = BlockSize;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    block.d32 = REG_READ(&(SDIO->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 1;
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    reg.bit.multi_blk_sel = MMC_XM_SEL_SINGLE_BLOCK;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_WRITE;
    REG_WRITE(&(SDIO->blockcount_r__blocksize.d32), block.d32);
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);*/

	// send command 24
    MMC_PRINT("send command 24\r\n");
    arg_r = WriteAddr;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.bit.dma_en = MMC_XM_DMA_ENABLE;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_WRITE;
    reg.bit.multi_blk_sel = MMC_XM_SEL_SINGLE_BLOCK;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
    reg.bit.cmd_index = SD_CMD_WRITE_SINGLE_BLOCK;
    block.d32 = REG_READ(&(SDIO->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = 0x1;
    REG_WRITE(&(SDIO->blockcount_r__blocksize.d32), block.d32);
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
	MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_WAIT_TRANSFER_COMPLETE(SDIO);

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
u32 SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t BlockNum)
{
    uint32_t status = MMC_SUCCESS;
    volatile unsigned int value = 0;
    uint32_t blocknum;
	uint32_t* Buffer_SingleBlock = (uint32_t* )readbuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    r1.d32 = 0;
    r1.bit.dma_sel = MMC_HC1_DMA_SEL_SDMA;
    r1.bit.dat_xfer_width = MMC_HC1_DAT_XFER_WIDTH_4BIT;   //4-bit mode
    r1.bit.sd_bus_pwr_vdd1 = MMC_PC_SBP_VDD1_ON;   //PWR ON
    r1.bit.sd_bus_vol_vdd1 = MMC_PC_SBV_VDD1_3V3;   //3V
    REG_WRITE(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    REG_WRITE(&(eMMC->sdmasa_r), Buffer_SingleBlock);
    REG_WRITE(&(eMMC->adma_sa_low_r), Buffer_SingleBlock);
    r2.d32 = 0;
    REG_WRITE(&(eMMC->host_ctrl2_r__auto_cmd_stat), r2.d32);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);
    MMC_PRINT("r2.d32 is %x\r\n", r2.d32);

	// send command 16
    MMC_PRINT("send command 16\r\n");
    arg_r = BlockSize;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    block.d32 = REG_READ(&(SDIO->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = BlockNum;
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.d32 = 0;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    reg.bit.multi_blk_sel = MMC_XM_SEL_MULTI_BLOCK;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_READ;    //Read
    REG_WRITE(&(SDIO->blockcount_r__blocksize), block.d32);
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

	// send command 17 read single block
    MMC_PRINT("send command 17\r\n");
    arg_r = ReadAddr;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    //reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    reg.d32 = 0;
    reg.bit.dma_en = MMC_XM_DMA_ENABLE;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_READ;
    reg.bit.multi_blk_sel = MMC_XM_SEL_MULTI_BLOCK;    //SingleBlock
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
    reg.bit.cmd_index = SD_CMD_READ_MULT_BLOCK;

    block.d32 = REG_READ(&(SDIO->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = BlockNum;
    REG_WRITE(&(SDIO->blockcount_r__blocksize), block.d32);
    REG_WRITE(&(SDIO->cmd_r__xfer_mode), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
	MMC_WAIT_CMD_COMPLETE(SDIO);
    /*for(blocknum = 1;blocknum <= BlockNum; blocknum++){
        status = wait_dma_complete(SDIO);
        if(status != MMC_SUCCESS){
            return status;
        }else{
            REG_WRITE(&(eMMC->sdmasa_r), Buffer_SingleBlock+blocknum*BlockSize);
            REG_WRITE(&(eMMC->adma_sa_low_r), Buffer_SingleBlock+blocknum*BlockSize);
        }
    }*/
    MMC_WAIT_TRANSFER_COMPLETE(SDIO);
    //clear_dma_interrupt(SDIO);

	return MMC_SUCCESS;
}
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
u32 SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t BlockNum)
{
    uint32_t status = MMC_SUCCESS;
	volatile unsigned int value = 0;
    uint32_t blocknum;
	uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    MMC_PRINT("SD_WriteMultiBlocks\r\n");
    r1.d32 = 0;
    //r1.bit.dma_sel = MMC_HC1_DMA_SEL_ADMA2;
    r1.bit.dat_xfer_width = MMC_HC1_DAT_XFER_WIDTH_4BIT;   //4-bit mode
    r1.bit.sd_bus_pwr_vdd1 = MMC_PC_SBP_VDD1_ON;   //PWR ON
    r1.bit.sd_bus_vol_vdd1 = MMC_PC_SBV_VDD1_3V3;   //3V
    REG_WRITE(&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1.d32), r1.d32);
    REG_WRITE(&(eMMC->adma_sa_high_r), Buffer_SingleBlock);
    REG_WRITE(&(eMMC->adma_sa_low_r), Buffer_SingleBlock);
    MMC_PRINT("r1.d32 is %x\r\n", r1.d32);
    r2.d32 = 0;
    REG_WRITE(&(eMMC->host_ctrl2_r__auto_cmd_stat.d32), r2.d32);
    MMC_PRINT("r2.d32 is %x\r\n", r2.d32);

	// send command 16
    MMC_PRINT("send command 16\r\n");
    arg_r = BlockSize;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    block.d32 = REG_READ(&(SDIO->blockcount_r__blocksize));
    //block.d32 = 0;
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = BlockNum;
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    //reg.d32 = 0;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    reg.bit.multi_blk_sel = MMC_XM_SEL_MULTI_BLOCK;    //SingleBlock
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_WRITE;    //Write
    REG_WRITE(&(SDIO->blockcount_r__blocksize.d32), block.d32);
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_WAIT_CMD_COMPLETE(SDIO);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);

	// send command 24
    MMC_PRINT("send command 24\r\n");
    arg_r = WriteAddr;
    REG_WRITE(&(SDIO->argument_r), arg_r);
    reg.d32 = REG_READ(&(SDIO->cmd_r__xfer_mode));
    //reg.d32 = 0;
    reg.bit.dma_en = MMC_XM_DMA_ENABLE;
    reg.bit.block_count_enable = MMC_XM_BLOCK_COUNT_ENABLE;
    reg.bit.data_xfer_dir = MMC_XM_DATA_XFER_DIR_WRITE;
    reg.bit.multi_blk_sel = MMC_XM_SEL_MULTI_BLOCK;
    reg.bit.resp_err_chk_enable = MMC_XM_RESP_ERR_CHK_ENABLE;
    reg.bit.resp_int_disable = MMC_XM_RESP_INT_DISABLE;
    reg.bit.auto_cmd_enable = MMC_XM_AUTO_CMD12_ENABLE;
    reg.bit.resp_type_select = MMC_C_RESP_LEN_48;
    reg.bit.data_present_sel = MMC_C_DATA_PRESENT;
    reg.bit.cmd_index = SD_CMD_WRITE_MULT_BLOCK;
    block.d32 = REG_READ(&(SDIO->blockcount_r__blocksize));
    //block.d32 = 0;
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = BlockNum;
    REG_WRITE(&(SDIO->blockcount_r__blocksize.d32), block.d32);
    REG_WRITE(&(SDIO->cmd_r__xfer_mode.d32), reg.d32);
    MMC_PRINT("block.d32 is %x\r\n", block.d32);
    MMC_PRINT("reg.d32 is %x\r\n", reg.d32);
	MMC_WAIT_CMD_COMPLETE(SDIO);
    
    /*for(blocknum = 1;blocknum < BlockNum; blocknum++){
        status = wait_dma_complete(SDIO);
        if(status != MMC_SUCCESS){
            MMC_PRINT("wait dma complete error!");
            //return status;
        }else{
            REG_WRITE(&(eMMC->sdmasa_r), Buffer_SingleBlock+blocknum*BlockSize);
            REG_WRITE(&(eMMC->adma_sa_low_r), Buffer_SingleBlock+blocknum*BlockSize);
        }
    }*/
    MMC_WAIT_TRANSFER_COMPLETE(SDIO);
    //clear_dma_interrupt(SDIO);

	return MMC_SUCCESS;
}




