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

static FRESULT res_emmc;
static FATFS fs;
static BYTE *WriteBuffer = (char *)0x6103ddf0;
static BYTE *ReadBuffer = (char *)0x6103edf0;

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
    blocknum = 3;
    //EmmcCardInfo.CardBlockSize = 512;
    printf("[EMMC]:Read/Write Block num : %d\r\n", blocknum);
    for(int k = 0; k < 0x200; k+=8){
        //WriteBuffer = 0x6103ddf0 + k;
        //ReadBuffer = 0x6103edf0 + k;
        WriteBuffer = 0x61038df0 + k;
        ReadBuffer = 0x61039df0 + k;
        printf("[EMMC]:Block write/read addr offset is %x\r\n", k);
        //raw 400k
        //Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
        printf("[EMMC]:raw 400k!\r\n");
        for (int i = 0; i < blocknum; i++){
            printf("[EMMC]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[EMMC]:Single Block Write! %d\r\n", i);
            status = AlEmmc_WriteSingleBlock(WriteBuffer,i,EmmcCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            MMC_PRINT("[EMMC]:Single Block Read! %d\r\n", i);
            status = AlEmmc_ReadSingleBlock(ReadBuffer, i, EmmcCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            #if 0
            for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                MMC_PRINT("%d\t", ReadBuffer[j]);
            }
            MMC_PRINT("\n");
            #endif
            for(int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                if(WriteBuffer[j] != ReadBuffer[j]){
                    printf("[EMMC]:Raw 400K write read data error %d\r\n", j);
                    for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                        printf("%d\t", WriteBuffer[j]);
                    }
                    printf("\r\n");
                    for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                        printf("%d\t", ReadBuffer[j]);
                    }
                    printf("\r\n");
                    status = MMC_FAILURE;
                    return status;
                }
            }
            memset(ReadBuffer, 0, BUF_SIZE);
        }
        
        //switch 10M
        Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M); 
        printf("[EMMC]:set 10M!\r\n");
        for (int i = 0; i < blocknum; i++){
            printf("[EMMC]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[EMMC]:Single Block Write! %d\r\n", i);
            status = AlEmmc_WriteSingleBlock(WriteBuffer,i,EmmcCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            MMC_PRINT("[EMMC]:Single Block Read! %d\r\n", i);
            status = AlEmmc_ReadSingleBlock(ReadBuffer, i, EmmcCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            #if 0
            for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                MMC_PRINT("%d\t", ReadBuffer[j]);
            }
            MMC_PRINT("\n");
            #endif
            for(int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                if(WriteBuffer[j] != ReadBuffer[j]){
                    printf("[EMMC]:10M write read data error %d\r\n", j);
                    for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                        printf("%d\t", WriteBuffer[j]);
                    }
                    printf("\r\n");
                    for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                        printf("%d\t", ReadBuffer[j]);
                    }
                    printf("\r\n");
                    status = MMC_FAILURE;
                    return status;
                }
            }
            memset(ReadBuffer, 0, BUF_SIZE);
        }
        
        //switch 400k
        Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
        printf("[EMMC]:set 400k!\r\n");
        for (int i = 0; i < blocknum; i++){
            printf("[EMMC]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[EMMC]:Single Block Write! %d\r\n", i);
            status = AlEmmc_WriteSingleBlock(WriteBuffer,i,EmmcCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            MMC_PRINT("[EMMC]:Single Block Read! %d\r\n", i);
            status = AlEmmc_ReadSingleBlock(ReadBuffer, i, EmmcCardInfo.CardBlockSize);
            if(status != MMC_SUCCESS){
                return status;
            }
            #if 0
            for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                MMC_PRINT("%d\t", ReadBuffer[j]);
            }
            MMC_PRINT("\n");
            #endif
            for(int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                if(WriteBuffer[j] != ReadBuffer[j]){
                    printf("[EMMC]:400k write read data error %d\r\n", j);
                    for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                        printf("%d\t", WriteBuffer[j]);
                    }
                    printf("\r\n");
                    for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                        printf("%d\t", ReadBuffer[j]);
                    }
                    printf("\r\n");
                    status = MMC_FAILURE;
                    return status;
                }
            }
            memset(ReadBuffer, 0, BUF_SIZE);
        }
    }
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
    blocknum = 1024;
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
        for(int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
            if(WriteBuffer[j] != ReadBuffer[j]){
                printf("[EMMC]:400k write read data error %d\r\n", j);
                for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                    printf("%d\t", WriteBuffer[j]);
                }
                printf("\r\n");
                for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++){
                    printf("%d\t", ReadBuffer[j]);
                }
                printf("\r\n");
                status = MMC_FAILURE;
                return status;
            }
        }
        memset(ReadBuffer, 0, BUF_SIZE);
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
    uint32_t j = 0;

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
        memset(ReadBuffer, 0, BUF_SIZE);
        j++;
        if(j == 1){
            Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
        }else if(j == 2){
            Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
        }
    }while(j<3);
    
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
	uint32_t status = MMC_SUCCESS;
    memset((void *)0x10000000, 0, 0x10000000);
    while(1){
#if 0
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
#if 1
    CsuRawReadWriteTestEMMC();
#endif
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
    res_emmc = f_open(&fnew, "1:FatFs1.txt", FA_OPEN_EXISTING | FA_READ);
    if(res_emmc == FR_OK)
    {
        res_emmc = f_lseek(&fnew, FIL_PT_OFFSET);
        if(res_emmc==FR_OK)
        {
            printf("File lseek success!\r\n");
        }else{
            printf("File lseek fail! Error code is %d\r\n", res_emmc);
        }
        memset(ReadBuffer, 0, sizeof(ReadBuffer));
        res_emmc = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if(res_emmc==FR_OK)
        {
            printf("File read success, data byte num is %d\r\n",fnum);
            for(int i = FIL_PT_OFFSET; i < sizeof(WriteBuffer)-FIL_PT_OFFSET; i++){
                if(WriteBuffer[i] != ReadBuffer[i-FIL_PT_OFFSET]){
                    printf("File read error, data not match\r\n");
                    break;
                }
            }
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
    }
    

    return status;
}


