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
#include "AL_sd_write.h"

#define BLOCK_LEN   0x200
#define BLOCK_NUM   2
#define BUF_SIZE    (BLOCK_LEN*BLOCK_NUM)
#define CSU_TEST_LENGTH1    300
#define CSU_TEST_LENGTH2    300
#define CSU_TEST_LENGTH3    600
#define FIL_PT_OFFSET       5

static FRESULT res_sd;
static FATFS fs;
static volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;


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

u32 CsuRawReadWriteTestSD()
{
    int status;
    BYTE WriteBuffer[BUF_SIZE];
    BYTE ReadBuffer[BUF_SIZE*2];
    int result;
    RawSDParam_t sdparam;
    u32 i = 0;

    printf("[CSU_SD]:Read/Write Buf Set!\r\n");
    for(int i = 0; i < BUF_SIZE; i++){
        WriteBuffer[i] = i%256;
    }
    memset(ReadBuffer, 0, BUF_SIZE);
    printf("[CSU_SD]:Init!\r\n");
    status = Csu_RawSDInit(&sdparam);
    if(status != MMC_SUCCESS){
        return status;
    }
    do{ //wait for optimize
        printf("[CSU_SD]:Single Block Write!\r\n");
        status = SD_WriteSingleBlock(WriteBuffer,0,SDCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }
        status = SD_WriteSingleBlock(WriteBuffer,1,SDCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }
        status = SD_WriteSingleBlock(WriteBuffer,2,SDCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }

        printf("[CSU_SD]:Byte Read with no cross area!\r\n");
        Csu_RawSDRead(200, ReadBuffer, CSU_TEST_LENGTH1);
        if(status != MMC_SUCCESS){
            return status;
        }
        for(i = 0; i < CSU_TEST_LENGTH1; i++){
            printf("%d\t", ReadBuffer[i]);
        }
        printf("\r\n");
        memset(ReadBuffer, 0, sizeof(ReadBuffer));
        printf("[CSU_SD]:Byte Read with cross area 1!\r\n");
        Csu_RawSDRead(300, ReadBuffer, CSU_TEST_LENGTH2);
        if(status != MMC_SUCCESS){
            return status;
        }
        for(i = 0; i < CSU_TEST_LENGTH2; i++){
            printf("%d\t", ReadBuffer[i]);
        }
        printf("\r\n");
        memset(ReadBuffer, 0, sizeof(ReadBuffer));
        printf("[CSU_SD]:Byte Read with cross area 2!\r\n");
        Csu_RawSDRead(300, ReadBuffer, CSU_TEST_LENGTH3);
        if(status != MMC_SUCCESS){
            return status;
        }
        for(i = 0; i < CSU_TEST_LENGTH3; i++){
            printf("%d\t", ReadBuffer[i]);
        }
        printf("\r\n");
    }while(1);
    
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
    

    /*printf("[START]:<SD>\r\n");
    status = RawReadWriteTestSD();
    if(status != MMC_SUCCESS){
        printf("[FAIL]:<SD>, [ERRORCODE]:<%d>\r\n", status);
        return status;
    }else{
        printf("[SUCCESS]:<SD>\r\n");   //方括号[]内为关键词，尖括号<>内为参数
    }
    printf("[DONE]:<SD>\r\n");*/

    CsuRawReadWriteTestSD();


#if 1
	res_sd = f_mount(&fs,"0:",1);  //SD test
    MMC_PRINT("res_sd is %d\r\n", res_sd);
    if(res_sd == FR_NO_FILESYSTEM)
    {
        printf("sd no file system, Wait for sd mkfs...");
        res_sd=f_mkfs("0:",0,0);
        MMC_PRINT("res_sd is %d\r\n", res_sd);
        if(res_sd == FR_OK)
        {
            res_sd = f_mount(NULL,"0:",1);
            MMC_PRINT("res_sd is %d\r\n", res_sd);
            res_sd = f_mount(&fs,"0:",1);
            MMC_PRINT("res_sd is %d\r\n", res_sd);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if(res_sd==FR_OK)
        {
            printf("File lseek success!\r\n");
        }else{
            printf("File lseek fail! Error code is %d\r\n", res_sd);
        }
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
