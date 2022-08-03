#include "AL_sdio_emmc_common.h"
#include "FATFS/ff.h"
#include "AL_emmc.h"

static FRESULT res_sd;
static FATFS fs;
static volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
/***************************************************************************/
/**
 * @brief	read multi block size data 
 *
 * @param	readbuff reading data buffer 
 * @param	ReadAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    uint32_t status = AL_SUCCESS;
    volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )readbuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    r1.d32 = 0;
    r1.bit.dat_xfer_width = 0x1;   //4-bit mode
    r1.bit.sd_bus_pwr_vdd1 = 0x1;   //PWR ON
    r1.bit.sd_bus_vol_vdd1 = 0x7;   //3V
    r1.bit.sd_bus_pwr_vdd2 = 0x1;   //PWR ON
    r1.bit.sd_bus_vol_vdd2 = 0x5;   //1.8V
    REG_WRITE((u32*)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    REG_WRITE((u32*)&(eMMC->sdmasa_r), *Buffer_SingleBlock);
    REG_WRITE((u32*)&(eMMC->adma_sa_low_r), *Buffer_SingleBlock);
    r2.d32 = 0;
    REG_WRITE((u32*)&(eMMC->host_ctrl2_r__auto_cmd_stat), r2.d32);

	// send command 16
    arg_r = 0x200;
    REG_WRITE((u32*)&(eMMC->argument_r), arg_r);
    block.d32 = REG_READ((u32*)&(eMMC->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = NumberOfBlocks;
    reg.d32 = REG_READ((u32*)&(eMMC->cmd_r__xfer_mode));
    reg.bit.block_count_enable = 0x1;
    reg.bit.resp_err_chk_enable = 0x1;
    reg.bit.resp_type_select = SDIO_Response_Short;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    REG_WRITE((u32*)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((u32*)&(eMMC->cmd_r__xfer_mode), reg.d32);
    SD_EMMC_WAIT_CMD_COMPLETE(eMMC);

	// send command 17 read single block
	eMMC->argument_r = ReadAddr;
    reg.d32 = REG_READ((u32*)&(eMMC->cmd_r__xfer_mode));
    reg.bit.dma_en = 0x1;
    reg.bit.data_xfer_dir = 0x1;
    reg.bit.resp_err_chk_enable = 0x1;
    reg.bit.resp_type_select = SDIO_Response_Short;
    reg.bit.data_present_sel = 0x1;
    reg.bit.cmd_index = SD_CMD_READ_SINGLE_BLOCK;
    block.d32 = REG_READ((u32*)&(eMMC->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = NumberOfBlocks;
    REG_WRITE((u32*)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((u32*)&(eMMC->cmd_r__xfer_mode), reg.d32);
	SD_EMMC_WAIT_CMD_COMPLETE(eMMC);
    SD_EMMC_WAIT_TRANSFER_COMPLETE(eMMC);

	return AL_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	write multi block size data 
 *
 * @param	writebuff reading data buffer 
 * @param	WriteAddr read start address
 * @param	BlockSize read data block size
 * @param	NumberOfBlocks data block number
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks)
{
    uint32_t status = AL_SUCCESS;
	volatile unsigned int value = 0;
	uint32_t* Buffer_SingleBlock = (uint32_t* )writebuff;
    CMD_R__XFER_MODE_R reg;
    WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R r1;
    HOST_CTRL2_R__AUTO_CMD_STAT_R r2;
    uint32_t arg_r;
    BLOCKCOUNT_R__BLOCKSIZE_R block;

    r1.d32 = 0;
    r1.bit.dat_xfer_width = 0x1;   //4-bit mode
    r1.bit.sd_bus_pwr_vdd1 = 0x1;   //PWR ON
    r1.bit.sd_bus_vol_vdd1 = 0x7;   //3V
    r1.bit.sd_bus_pwr_vdd2 = 0x1;   //PWR ON
    r1.bit.sd_bus_vol_vdd2 = 0x5;   //1.8V
    REG_WRITE((u32*)&(eMMC->wup_ctrl_r__bgap_ctrl_r__pwr_ctrl_r__host_ctrl1), r1.d32);
    REG_WRITE((u32*)&(eMMC->sdmasa_r), *Buffer_SingleBlock);
    REG_WRITE((u32*)&(eMMC->adma_sa_low_r), *Buffer_SingleBlock);
    r2.d32 = 0;
    REG_WRITE((u32*)&(eMMC->host_ctrl2_r__auto_cmd_stat), r2.d32);

	// send command 16
    arg_r = BlockSize;
    REG_WRITE((u32*)&(eMMC->argument_r), arg_r);
    block.d32 = REG_READ((u32*)&(eMMC->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = NumberOfBlocks;
    reg.d32 = REG_READ((u32*)&(eMMC->cmd_r__xfer_mode));
    reg.bit.block_count_enable = 0x1;
    reg.bit.resp_err_chk_enable = 0x1;
    reg.bit.resp_type_select = SDIO_Response_Short;
    reg.bit.cmd_index = SD_CMD_SET_BLOCKLEN;
    REG_WRITE((u32*)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((u32*)&(eMMC->cmd_r__xfer_mode), reg.d32);
    SD_EMMC_WAIT_CMD_COMPLETE(eMMC);


	// send command 24
    arg_r = WriteAddr;
    REG_WRITE((u32*)&(eMMC->argument_r), arg_r);
    reg.d32 = REG_READ((u32*)&(eMMC->cmd_r__xfer_mode));
    reg.bit.dma_en = 0x1;
    reg.bit.block_count_enable = 0x1;
    reg.bit.data_xfer_dir = DATA_WRITE;
    reg.bit.resp_err_chk_enable = 0x1;
    reg.bit.resp_type_select = SDIO_Response_Short;
    reg.bit.data_present_sel = 0x1;
    reg.bit.cmd_index = SD_CMD_WRITE_SINGLE_BLOCK;
    block.d32 = REG_READ((u32*)&(eMMC->blockcount_r__blocksize));
    block.bit.xfer_block_size = BlockSize;
    block.bit.blockcount_r = NumberOfBlocks;
    REG_WRITE((u32*)&(eMMC->blockcount_r__blocksize), block.d32);
    REG_WRITE((u32*)&(eMMC->cmd_r__xfer_mode), reg.d32);
	SD_EMMC_WAIT_CMD_COMPLETE(eMMC);
    SD_EMMC_WAIT_TRANSFER_COMPLETE(eMMC);

	return AL_SUCCESS;
}

/***************************************************************************/
/**
 * @brief	emmc init sequence
 *
 * @param	None
 * @return	AL_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_Init(void)
{
    int status = AL_SUCCESS;

    status = HostControllerSetup(eMMC);
    if (status != AL_SUCCESS) {
		goto END;
	}
    status = HostControllerClockSetup(eMMC, SD_EMMC_FREQ_400K);
    if (status != AL_SUCCESS) {
		goto END;
	}
    //sleep(200);
    SD_EMMC_DELAY_MS(10);
    status = InitInterruptSetting(eMMC);
    if (status != AL_SUCCESS) {
		goto END;
	}
    //sleep(200);
    SD_EMMC_DELAY_MS(10);
    status = SendInitCmdEmmc();
    if (status != AL_SUCCESS) {
		goto END;
	}
    status = SwitchDataWidthEmmc();
    if (status != AL_SUCCESS) {
		goto END;
	}
    
    status = AL_SUCCESS;
END:
	return status;
}

u32 RawReadWriteTestEmmc()
{
    int status = AL_SUCCESS;
    int result;
    BYTE WriteBuffer[] = "welcomewelcome\r\n";
    BYTE ReadBuffer[1024]={0};   
    status = EMMC_Init();
    if(status != AL_SUCCESS){
        return status;
    }
    status = EMMC_WriteMultiBlocks(WriteBuffer, 80,SDCardInfo.CardBlockSize,1);
    if(status != AL_SUCCESS){
        return status;
    }
    status = EMMC_ReadMultiBlocks(ReadBuffer, 80, SDCardInfo.CardBlockSize,1);
    if(status != AL_SUCCESS){
        return status;
    }

    result = strcmp(WriteBuffer, ReadBuffer);
    if (result == 0)
    {
        status = AL_SUCCESS;
    }
    else
    {
        status = AL_FAILURE;
    }

    return status;
}



/***************************************************************************/
/**
 * @brief	test SD/EMMC read/write
 *
 * @param  None
 * @return AL_SUCCESS
 *
 ******************************************************************************/
u32 EMMC_Test(void)
{
	u32 fnum;            			  
	char ReadBuffer[1024]={0};
	char WriteBuffer[] = "welcome777777777777777\r\n";
	FIL fnew;
	u32 status;

    RawReadWriteTestEmmc();

    for(;;);
#if 0
	res_sd = f_mount(&fs,"0:",1);  //SD test
#if 0
	    if(res_sd == FR_NO_FILESYSTEM)
	    {
	    	res_sd=f_mkfs("0:",0,0);
	    	if(res_sd == FR_OK)
	    	{
	    		res_sd = f_mount(NULL,"0:",1);
	    		res_sd = f_mount(&fs,"0:",1);
	    	}
	    }
#endif
    res_sd = f_open(&fnew, "0:FatFs1.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
        res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if(res_sd==FR_OK)
        {
            //printf("》文件写入成功，写入字节数据：%d\n",fnum);
            //printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
        }
        else
        {
            //printf("！！文件写入失败：(%d)\n",res_sd);
        }
        f_close(&fnew);
    }
    else
    {
    }
	/*------------------- 文件系统测试：读测试 ------------------------------------*/
    //printf("****** 即将进行文件读取测试... ******\r\n");
    res_sd = f_open(&fnew, "0:FatFs1.txt", FA_OPEN_EXISTING | FA_READ);
    if(res_sd == FR_OK)
    {
        //printf("》打开文件成功。\r\n");
        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if(res_sd==FR_OK)
        {
            //printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
            //printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            //printf("！！文件读取失败：(%d)\n",res_sd);
        }
    }
    else
    {
        //LED_RED;
        //printf("！！打开文件失败。\r\n");
    }
    f_close(&fnew);
    f_mount(NULL,"0:",1);
#endif

    res_sd = f_mount(&fs,"1:",1);  //EMMC test
#if 1
            if(res_sd == FR_NO_FILESYSTEM)
            {
                res_sd=f_mkfs("1:",0,0);
                if(res_sd == FR_OK)
                {
                    res_sd = f_mount(NULL,"1:",1);
                    res_sd = f_mount(&fs,"1:",1);
                }
            }
#endif
        res_sd = f_open(&fnew, "1:FatFs1.txt",FA_CREATE_ALWAYS | FA_WRITE );
        if ( res_sd == FR_OK )
        {
            res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
            if(res_sd==FR_OK)
            {
                //printf("》文件写入成功，写入字节数据：%d\n",fnum);
                //printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
            }
            else
            {
                //printf("！！文件写入失败：(%d)\n",res_sd);
            }
            f_close(&fnew);
        }
        else
        {
        }
        /*------------------- 文件系统测试：读测试 ------------------------------------*/
        //printf("****** 即将进行文件读取测试... ******\r\n");
        res_sd = f_open(&fnew, "1:FatFs1.txt", FA_OPEN_EXISTING | FA_READ);
        if(res_sd == FR_OK)
        {
            //printf("》打开文件成功。\r\n");
            res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
            if(res_sd==FR_OK)
            {
                //printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
                //printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
            }
            else
            {
                //printf("！！文件读取失败：(%d)\n",res_sd);
            }
        }
        else
        {
            //LED_RED;
            //printf("！！打开文件失败。\r\n");
        }
        f_close(&fnew);
        f_mount(NULL,"1:",1);

    

    return status;
}


