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
#include "AL_mmc.h"
#include "FATFS/ff.h"
#include "mtimer.h"
#include "AL_sd.h"

#define BLOCK_LEN   0x200
#define BLOCK_NUM   2
#define BUF_SIZE    (BLOCK_LEN*BLOCK_NUM)

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
    r1.bit.dma_sel = MMC_HC1_DMA_SEL_ADMA2;
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


u32 RawReadWriteTestSD()
{
    int status;
    BYTE WriteBuffer[BUF_SIZE];
    BYTE ReadBuffer[BUF_SIZE];
    int result;
    MMC_PRINT("[SD]:Read/Write Buf Set!\r\n");
    for(int i = 0; i < BUF_SIZE; i++){
        WriteBuffer[i] = i%256;
    }
    memset(ReadBuffer, 0, BUF_SIZE);
    MMC_PRINT("[SD]:Init!\r\n");
    status = SD_Init();
    if(status != MMC_SUCCESS){
        return status;
    }
    MMC_PRINT("[SD]:Single Block Write!\r\n");
    status = SD_WriteSingleBlock(WriteBuffer,0,SDCardInfo.CardBlockSize);
    if(status != MMC_SUCCESS){
        return status;
    }
    MMC_PRINT("[SD]:Single Block Read!\r\n");
    status = SD_ReadSingleBlock(ReadBuffer, 0, SDCardInfo.CardBlockSize);
    if(status != MMC_SUCCESS){
        return status;
    }
    status = strcmp(WriteBuffer, ReadBuffer);
    if (status != MMC_SUCCESS)
    {
        MMC_PRINT("[SD]:Single Block Read/Write Data Not Match!\r\n");
        return status;
    }else{
        MMC_PRINT("[SD]:Single Block Read/Write Success!\r\n");
    }

    return status;
}

/***************************************************************************/
/**
 * @brief	test SD/EMMC read/write
 *
 * @param  None
 * @return MMC_SUCCESS
 *
 ******************************************************************************/
u32 SD_Test(void)
{
	u32 fnum;            			  
	char ReadBuffer[1024]={0};
	char WriteBuffer[] = "welcome777777777777777\r\n";
	FIL fnew;
	u32 status;
    printf("[START]:<SD>\r\n");
    status = RawReadWriteTestSD();
    if(status != MMC_SUCCESS){
        printf("[FAIL]:<SD>, [ERRORCODE]:<%d>\r\n", status);
        return status;
    }else{
        printf("[SUCCESS]:<SD>\r\n");   //方括号[]内为关键词，尖括号<>内为参数
    }
    printf("[DONE]:<SD>\r\n");

#if 0
	res_sd = f_mount(&fs,"0:",1);  //SD test
    MMC_PRINT("res_sd is %d\r\n", res_sd);
    if(res_sd == FR_NO_FILESYSTEM)
    {
        printf("sd no file system, Wait for sd mkfs...");
        res_sd=f_mkfs("0:",0,0);
        MMC_PRINT("res_sd is %d\r\n", res_sd);
        if(res_sd == FR_OK)
        {
            MMC_PRINT = f_mount(NULL,"0:",1);
            printf("res_sd is %d\r\n", res_sd);
            MMC_PRINT = f_mount(&fs,"0:",1);
            printf("res_sd is %d\r\n", res_sd);
        }
    }
    res_sd = f_open(&fnew, "0:FatFs1.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
        res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if(res_sd==FR_OK)
        {
            printf("File write success, data byte num is %d\r\n",fnum);
            printf("data is:\r\n%s\r\n",WriteBuffer);
        }else{
            printf("File write fail (%d)\r\n",res_sd);
        }
        f_close(&fnew);
    }else{
        printf("File open fail!\r\n");
    }
	/*------------------- 文件系统测试：读测试 ------------------------------------*/
    res_sd = f_open(&fnew, "0:FatFs1.txt", FA_OPEN_EXISTING | FA_READ);
    if(res_sd == FR_OK)
    {
        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if(res_sd==FR_OK)
        {
            printf("File read success, data byte num is %d\r\n",fnum);
            printf("data is:\r\n%s\r\n", ReadBuffer);
        }else{
            printf("File read fail (%d)\n",res_sd);
        }
    }else{
        printf("File open fail!\r\n");
    }
    f_close(&fnew);
    f_mount(NULL,"0:",1);
#endif
    return status;
}

/*********************************************END OF FILE**********************/
