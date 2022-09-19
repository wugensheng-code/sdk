#include "al_mmc.h"
#include "FATFS/ff.h"
#include "al_emmc.h"
#include "al_emmc_write.h"
#include <string.h>

#define BLOCK_LEN   0x200
#define BLOCK_NUM   2
#define BUF_SIZE    (BLOCK_LEN*BLOCK_NUM)
#define CSU_TEST_LENGTH1    300
#define CSU_TEST_LENGTH2    300
#define CSU_TEST_LENGTH3    600
#define FIL_PT_OFFSET       5

static __IO FRESULT res_emmc;
static __IO FATFS fs;
static __IO TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
static __IO uint8_t WriteBuffer[BUF_SIZE];
static __IO uint8_t ReadBuffer[BUF_SIZE];

uint32_t RawReadWriteTestEmmc()
{
    int status = MMC_SUCCESS;
    int result;
    uint64_t blocknum = 0;
    RawEmmcParam_t emmcparam;

    MMC_PRINT("[EMMC]:Read/Write Buf Set!\r\n");
    for(int i = 0; i < BLOCK_LEN; i++){
        WriteBuffer[i] = i%26+0x30;
    }
    memset(ReadBuffer, 0, BUF_SIZE);
    MMC_PRINT("[EMMC]:Init!\r\n");
    status = Csu_RawEmmcInit(&emmcparam);
    if(status != MMC_SUCCESS){
        return status;
    }
    //Csu_RawEmmcSetMode(EMMC_MODE_FREQ, EMMC_FREQ_400K);
    blocknum = EmmcCardInfo.CardCapacity / EmmcCardInfo.CardBlockSize;
    //blocknum = 1024;
    //EmmcCardInfo.CardBlockSize = 512;
    printf("[EMMC]:Read/Write Block num : %d\r\n", blocknum);

#if 0
    printf("[EMMC]:Write Block\r\n");
    for(uint64_t i = 0; i < blocknum; i++){
        status = AlEmmc_WriteSingleBlock(WriteBuffer, i,EmmcCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }
        printf("%d\t", i);
    }
    printf("[EMMC]:Read Block\r\n");
    for(uint64_t i = 0; i < blocknum; i++){
        status = AlEmmc_ReadSingleBlock(WriteBuffer, i,EmmcCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }
        printf("%d\t", i);
        status = strcmp(WriteBuffer, ReadBuffer);
        if (status == MMC_SUCCESS)
        {
            MMC_PRINT("[EMMC]:Single Block Read/Write Success!\r\n");
        }else{
            printf("[EMMC]:Single Block Read/Write Data Not Match!\r\n");
            return status;
        }
    }
    
#endif
#if 1   //one write one read 
    for(uint64_t i = 0; i < blocknum; i++){
        printf("[EMMC]:Read/Write Block count : %d\r\n", i);
        //MMC_PRINT("[EMMC]:Single Block Write!\r\n");
        status = AlEmmc_WriteSingleBlock(WriteBuffer, i,EmmcCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }
        //_delay_ms(100);
        //MMC_PRINT("[EMMC]:Single Block Read!\r\n");
        status = AlEmmc_ReadSingleBlock(ReadBuffer, i, EmmcCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }
        //_delay_ms(100);
        status = strcmp(WriteBuffer, ReadBuffer);
        if (status == MMC_SUCCESS){
            MMC_PRINT("[EMMC]:Single Block Read/Write Success!\r\n");
        }else{
            printf("[EMMC]:Single Block Read/Write Data Not Match!\r\n");
            return status;
        }
    }
#endif

    return status;
}

uint32_t CsuRawReadWriteTestEMMC()
{
    int status;
    int result;
    RawEmmcParam_t emmcparam;
    uint32_t i = 0;

    printf("[CSU_EMMC]:Read/Write Buf Set!\r\n");
    for(int i = 0; i < BUF_SIZE; i++){
        WriteBuffer[i] = i%26+0x30;
    }
    memset(ReadBuffer, 0, BUF_SIZE);
    printf("[CSU_EMMC]:Init!\r\n");
    status = Csu_RawEmmcInit(&emmcparam);
    if(status != MMC_SUCCESS){
        return status;
    }
    Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);


    do{ //wait for optimize
        printf("[CSU_EMMC]:Single Block Write!\r\n");
        status = AlEmmc_WriteSingleBlock(WriteBuffer,0,EmmcCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }
        status = AlEmmc_WriteSingleBlock(WriteBuffer,1,EmmcCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }
        status = AlEmmc_WriteSingleBlock(WriteBuffer,2,EmmcCardInfo.CardBlockSize);
        if(status != MMC_SUCCESS){
            return status;
        }

        printf("[CSU_EMMC]:Byte Read with no cross area!\r\n");
        Csu_RawEmmcRead(200, ReadBuffer, CSU_TEST_LENGTH1);
        if(status != MMC_SUCCESS){
            return status;
        }
        for(i = 0; i < CSU_TEST_LENGTH1; i++){
            MMC_PRINT("%d\t", ReadBuffer[i]);
        }
        MMC_PRINT("\r\n");
        memset(ReadBuffer, 0, sizeof(ReadBuffer));
        printf("[CSU_EMMC]:Byte Read with cross area 1!\r\n");
        Csu_RawEmmcRead(300, ReadBuffer, CSU_TEST_LENGTH2);
        if(status != MMC_SUCCESS){
            return status;
        }
        for(i = 0; i < CSU_TEST_LENGTH2; i++){
            MMC_PRINT("%d\t", ReadBuffer[i]);
        }
        MMC_PRINT("\r\n");
        memset(ReadBuffer, 0, sizeof(ReadBuffer));
        printf("[CSU_EMMC]:Byte Read with cross area 2!\r\n");
        Csu_RawEmmcRead(300, ReadBuffer, CSU_TEST_LENGTH3);
        if(status != MMC_SUCCESS){
            return status;
        }
        for(i = 0; i < CSU_TEST_LENGTH3; i++){
            MMC_PRINT("%d\t", ReadBuffer[i]);
        }
        MMC_PRINT("\r\n");
    }while(0);
    
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
uint32_t EMMC_Test(void)
{
	uint32_t fnum;            			  
	char ReadBuffer[1024]={0};
	char WriteBuffer[] = "welcome777777777777777\r\n";
	FIL fnew;
	uint32_t status;
#if 1
    printf("[START]:<EMMC>\r\n");
    status = RawReadWriteTestEmmc();
    if(status != MMC_SUCCESS){
        printf("[FAIL]:<EMMC>, [ERRORCODE]:<%d>\r\n", status);
        return status;
    }else{
        printf("[SUCCESS]:<EMMC>\r\n");   //方括号[]内为关键词，尖括号<>内为参数
    }
    printf("[DONE]:<EMMC>\r\n");
#endif

    //CsuRawReadWriteTestEMMC();

#if 0
    res_emmc = f_mount(&fs,"1:",1);  //EMMC test
    printf("res_sd is %d\r\n", res_emmc);
    if(res_emmc == FR_NO_FILESYSTEM)
    {
        printf("sd no file system, Wait for sd mkfs...");
        res_emmc=f_mkfs("1:",0,0);
        printf("res_sd is %d\r\n", res_emmc);
        if(res_emmc == FR_OK)
        {
            res_emmc = f_mount(NULL,"1:",1);
            printf("res_sd is %d\r\n", res_emmc);
            res_emmc = f_mount(&fs,"1:",1);
            printf("res_sd is %d\r\n", res_emmc);
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
    }else{
        printf("File open fail!\r\n");
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


