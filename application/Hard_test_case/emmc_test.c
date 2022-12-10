#include "AL_mmc.h"
#include "FATFS/ff.h"
#include "AL_emmc.h"
#include "AL_emmc_write.h"
#include <string.h>

static FRESULT res_emmc;
static FATFS fs;
static volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;


u32 RawReadWriteTestEmmc()
{
    int status = MMC_SUCCESS;
    int result;
    BYTE WriteBuffer[] = "welcomewelcome\r\n";
    BYTE ReadBuffer[1024]={0};   
    status = EMMC_Init();
    if(status != MMC_SUCCESS){
        return status;
    }
    status = EMMC_WriteSingleBlock(WriteBuffer, 80,SDCardInfo.CardBlockSize);
    if(status != MMC_SUCCESS){
        return status;
    }
    status = EMMC_ReadSingleBlock(ReadBuffer, 80, SDCardInfo.CardBlockSize);
    if(status != MMC_SUCCESS){
        return status;
    }

    result = strcmp(WriteBuffer, ReadBuffer);
    if (result == 0)
    {
        status = MMC_SUCCESS;
    }
    else
    {
        status = MMC_FAILURE;
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
u32 EMMC_Test(void)
{
	u32 fnum;            			  
	char ReadBuffer[1024]={0};
	char WriteBuffer[] = "welcome777777777777777\r\n";
	FIL fnew;
	u32 status;

    RawReadWriteTestEmmc();

#if 1
    res_emmc = f_mount(&fs,"1:",1);  //EMMC test

            if(res_emmc == FR_NO_FILESYSTEM)
            {
                res_emmc=f_mkfs("1:",0,0);
                if(res_emmc == FR_OK)
                {
                    res_emmc = f_mount(NULL,"1:",1);
                    res_emmc = f_mount(&fs,"1:",1);
                }
            }
        res_emmc = f_open(&fnew, "1:FatFs1.txt",FA_CREATE_ALWAYS | FA_WRITE );
        if ( res_emmc == FR_OK )
        {
            res_emmc=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
            if(res_emmc==FR_OK)
            {
                printf("File write success, data byte num is %d\r\n",fnum);
                printf("data is:\r\n%s\r\n",WriteBuffer);
            }
            else
            {
                printf("File write fail (%d)\r\n",res_emmc);
            }
            f_close(&fnew);
        }
        else
        {
        }
        /*------------------- 文件系统测试：读测试 ------------------------------------*/
        //printf("****** 即将进行文件读取测试... ******\r\n");
        res_emmc = f_open(&fnew, "1:FatFs1.txt", FA_OPEN_EXISTING | FA_READ);
        if(res_emmc == FR_OK)
        {
            //printf("》打开文件成功。\r\n");
            res_emmc = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
            if(res_emmc==FR_OK)
            {
                printf("File read success, data byte num is %d\r\n",fnum);
                printf("data is:\r\n%s\r\n", ReadBuffer);
            }
            else
            {
                printf("File read fail (%d)\n",res_emmc);
            }
        }
        else
        {
            printf("File open fail!\r\n");
        }
        f_close(&fnew);
        f_mount(NULL,"1:",1);
#endif
    

    return status;
}


