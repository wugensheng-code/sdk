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
#include "al_mmc.h"
#include "FATFS/ff.h"
#include "mtimer.h"
#include "al_sd.h"
#include "al_sd_write.h"

#define BLOCK_LEN   0x200
#define BLOCK_NUM   1
#define BUF_SIZE    (BLOCK_LEN*BLOCK_NUM)
#define CSU_TEST_LENGTH1    300
#define CSU_TEST_LENGTH2    300
#define CSU_TEST_LENGTH3    600
#define FIL_PT_OFFSET       5

static __IO FRESULT res_sd;
static __IO FATFS fs;
BYTE *WriteBuffer = (char *)0x6103ddf0;
BYTE *ReadBuffer = (char *)0x6103edf0;
//extern char *logaddr;

uint32_t RawReadWriteTestSD()
{
    int status;
    int result;
    uint64_t blocknum = 0;
    MMC_PRINT("[SD]:Read/Write Buf Set!\r\n");
    for(int i = 0; i < BUF_SIZE; i++){
        WriteBuffer[i] = i%10+0x32;
    }
    memset(ReadBuffer, 0, BUF_SIZE);
    MMC_PRINT("[SD]:Init!\r\n");
    status = AlSd_Init();
    if(status != MMC_SUCCESS){
        return status;
    }
    
    blocknum = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;
    printf("block num is %d\r\n", blocknum);
    blocknum = 3;
    for(int k = 0; k < 0x200; k+=8){
        //WriteBuffer = 0x6103ddf0 + k;
        //ReadBuffer = 0x6103edf0 + k;
        WriteBuffer = 0x61038df0 + k;
        ReadBuffer = 0x61039df0 + k;
        printf("[SD]:Block write/read addr offset is %x\r\n", k);
        //raw 400k
        //Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
        printf("[SD]:raw 400k!\r\n");
        for (int i = 0; i < blocknum; i++){
            printf("[SD]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[SD]:Single Block Write! %d\r\n", i);
            status = AlSd_WriteSingleBlock(WriteBuffer,i,SDCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            MMC_PRINT("[SD]:Single Block Read! %d\r\n", i);
            status = AlSd_ReadSingleBlock(ReadBuffer, i, SDCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            #if 0
            for (int j = 0; j < SDCardInfo.CardBlockSize; j++){
                MMC_PRINT("%d\t", ReadBuffer[j]);
            }
            MMC_PRINT("\n");
            #endif
            for(int j = 0; j < SDCardInfo.CardBlockSize; j++){
                if(WriteBuffer[j] != ReadBuffer[j]){
                    printf("[SD]:Raw 400K write read data error %d\r\n", j);
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++){
                        printf("%d\t", WriteBuffer[j]);
                    }
                    printf("\r\n");
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++){
                        printf("%d\t", ReadBuffer[j]);
                    }
                    printf("\r\n");
                    status = MMC_FAILURE;
                    return status;
                }
            }
        }
        //switch 10M
        Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M); 
        printf("[SD]:set 10M!\r\n");
        for (int i = 0; i < blocknum; i++){
            printf("[SD]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[SD]:Single Block Write! %d\r\n", i);
            status = AlSd_WriteSingleBlock(WriteBuffer,i,SDCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            MMC_PRINT("[SD]:Single Block Read! %d\r\n", i);
            status = AlSd_ReadSingleBlock(ReadBuffer, i, SDCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            #if 0
            for (int j = 0; j < SDCardInfo.CardBlockSize; j++){
                MMC_PRINT("%d\t", ReadBuffer[j]);
            }
            MMC_PRINT("\n");
            #endif
            for(int j = 0; j < SDCardInfo.CardBlockSize; j++){
                if(WriteBuffer[j] != ReadBuffer[j]){
                    printf("[SD]:10M write read data error %d\r\n", j);
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++){
                        printf("%d\t", WriteBuffer[j]);
                    }
                    printf("\r\n");
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++){
                        printf("%d\t", ReadBuffer[j]);
                    }
                    printf("\r\n");
                    status = MMC_FAILURE;
                    return status;
                }
            }
        }
        //switch 400k
        Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
        printf("[SD]:set 400k!\r\n");
        for (int i = 0; i < blocknum; i++){
            printf("[SD]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[SD]:Single Block Write! %d\r\n", i);
            status = AlSd_WriteSingleBlock(WriteBuffer,i,SDCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            MMC_PRINT("[SD]:Single Block Read! %d\r\n", i);
            status = AlSd_ReadSingleBlock(ReadBuffer, i, SDCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            #if 0
            for (int j = 0; j < SDCardInfo.CardBlockSize; j++){
                MMC_PRINT("%d\t", ReadBuffer[j]);
            }
            MMC_PRINT("\n");
            #endif
            for(int j = 0; j < SDCardInfo.CardBlockSize; j++){
                if(WriteBuffer[j] != ReadBuffer[j]){
                    printf("[SD]:400k write read data error %d\r\n", j);
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++){
                        printf("%d\t", WriteBuffer[j]);
                    }
                    printf("\r\n");
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++){
                        printf("%d\t", ReadBuffer[j]);
                    }
                    printf("\r\n");
                    status = MMC_FAILURE;
                    return status;
                }
            }
        }
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
uint32_t SD_Test(void)
{
	__IO uint32_t fnum = 0;            			  
	__IO char ReadBuffer[1024]={0};
	__IO char WriteBuffer[] = "welcome777777777777777\r\n";
    FIL fnew;
	uint32_t status = MMC_SUCCESS;

while(1){
    memset(&fnew, 0, sizeof(FIL));

    //clear ddr log
    //memset((char *)(0x10000000), 0, 0x100000);
    //logaddr = (char *)0x10000000;
#if 1
    printf("[START]:<SD>\r\n");
    status = RawReadWriteTestSD();
    if(status != MMC_SUCCESS){
        printf("[FAIL]:<SD>, [ERRORCODE]:<%d>\r\n", status);
        return status;
    }else{
        printf("[SUCCESS]:<SD>\r\n");   //方括号[]内为关键词，尖括号<>内为参数
    }
    printf("[DONE]:<SD>\r\n");
#endif

#if 1
	res_sd = f_mount(&fs,"0:",1);  //SD test
    printf("res_sd is %d\r\n", res_sd);
    if(res_sd == FR_NO_FILESYSTEM)
    {
        Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
        printf("sd no file system, Wait for sd mkfs...");
        res_sd=f_mkfs("0:",0,0);
        printf("res_sd is %d\r\n", res_sd);
        if(res_sd == FR_OK)
        {
            res_sd = f_mount(NULL,"0:",1);
            printf("res_sd is %d\r\n", res_sd);
            res_sd = f_mount(&fs,"0:",1);
            printf("res_sd is %d\r\n", res_sd);
        }
    }
    res_sd = f_open(&fnew, "0:FatFs2.txt",FA_CREATE_ALWAYS | FA_WRITE );
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
    res_sd = f_open(&fnew, "0:FatFs2.txt", FA_OPEN_EXISTING | FA_READ);
    if(res_sd == FR_OK)
    {
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if(res_sd==FR_OK)
        {
            printf("File lseek success!\r\n");
        }else{
            printf("File lseek fail! Error code is %d\r\n", res_sd);
        }
        memset(ReadBuffer, 0, sizeof(ReadBuffer));
        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if(res_sd==FR_OK)
        {
            printf("File read success, data byte num is %d\r\n",fnum);
            for(int i = FIL_PT_OFFSET; i < sizeof(WriteBuffer)-FIL_PT_OFFSET; i++){
                if(WriteBuffer[i] != ReadBuffer[i-FIL_PT_OFFSET]){
                    printf("File read error, data not match\r\n");
                    break;
                }
            }
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
}
    return status;
}

/*********************************************END OF FILE**********************/
