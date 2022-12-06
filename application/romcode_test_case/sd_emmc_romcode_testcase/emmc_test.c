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
#define FIL_LARGE_RDWR_SIZE 8845488

static BYTE *WriteBuffer = (char *)0x6103ddf0;
static BYTE *ReadBuffer = (char *)0x6103edf0;
extern void Enablepinmux1_mode2(void);

// #define EMMC_TRAVERSETEST
// #define EMMC_BRANCHTEST
#define EMMC_BOUNDARYTEST
#define EMMC_FATFSTEST
#define EMMC_PRINTREGTEST
#define EMMC_BYTEREADTEST

#define EMMC_OFFSETSTEP         0x5700    //random none aligned num
#define EMMC_LENGTHSTEP         0x21210    //random none aligned num
#define EMMC_READ_LENGTH        0x2100  
#define EMMC_OFFSET_START       100
#define EMMC_LENGTH_START       100
#define BRANCHTEST_EMMC_READ_OFFSET 400
#define BRANCHTEST_EMMC_READ_LENGTH 700

uint32_t RawReadWriteTestEmmc()
{
    int status = MMC_SUCCESS;
    int result;
    uint64_t blocknum = 0;
    RawEmmcParam_t emmcparam;

    MMC_PRINT("[EMMC]:Read/Write Buf Set!\r\n");
    for (int i = 0; i < BLOCK_LEN; i++) {
        WriteBuffer[i] = i%26+0x30;
    }
    memset(ReadBuffer, 0, BUF_SIZE);
    MMC_PRINT("[EMMC]:Init!\r\n");
    status = Csu_RawEmmcInit(&emmcparam);
    if (status != MMC_SUCCESS) {
        return status;
    }

    blocknum = EmmcCardInfo.CardCapacity / EmmcCardInfo.CardBlockSize;
    blocknum = 3;
    //EmmcCardInfo.CardBlockSize = 512;
    printf("[EMMC]:Read/Write Block num : %d\r\n", blocknum);
    for (int k = 0; k < 0x10; k+=8) {
        //WriteBuffer = 0x6103ddf0 + k;
        //ReadBuffer = 0x6103edf0 + k;
        WriteBuffer = 0x10034df0 + k;
        ReadBuffer = 0x10035df0 + k;
        printf("[EMMC]:Block write/read addr offset is %x\r\n", k);
        
        //raw 400k
        Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
        printf("[EMMC]:raw 400k!\r\n");
        for (int i = 0; i < blocknum; i++) {
            printf("[EMMC]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[EMMC]:Single Block Write! %d\r\n", i);
            status = AlEmmc_WriteSingleBlock(WriteBuffer,i,EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            MMC_PRINT("[EMMC]:Single Block Read! %d\r\n", i);
            status = AlEmmc_ReadSingleBlock(ReadBuffer, i, EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++) {
                if (WriteBuffer[j] != ReadBuffer[j]) {
                    printf("[EMMC]:Raw 400K write read data error %d\r\n", j);
                    for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                        printf("%d\t", WriteBuffer[m]);
                    }
                    printf("\r\n");
                    for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                        printf("%d\t", ReadBuffer[m]);
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
        for (int i = 0; i < blocknum; i++) {
            printf("[EMMC]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[EMMC]:Single Block Write! %d\r\n", i);
            status = AlEmmc_WriteSingleBlock(WriteBuffer,i,EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            MMC_PRINT("[EMMC]:Single Block Read! %d\r\n", i);
            status = AlEmmc_ReadSingleBlock(ReadBuffer, i, EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            #if 0
            for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                MMC_PRINT("%d\t", ReadBuffer[m]);
            }
            MMC_PRINT("\n");
            #endif
            for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++) {
                if (WriteBuffer[j] != ReadBuffer[j]) {
                    printf("[EMMC]:10M write read data error %d\r\n", j);
                    for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                        printf("%d\t", WriteBuffer[m]);
                    }
                    printf("\r\n");
                    for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                        printf("%d\t", ReadBuffer[m]);
                    }
                    printf("\r\n");
                    status = MMC_FAILURE;
                    return status;
                }
            }
            memset(ReadBuffer, 0, BUF_SIZE);
        }
        /*
        //switch 400k
        Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
        printf("[EMMC]:set 400k!\r\n");
        for (int i = 0; i < blocknum; i++) {
            printf("[EMMC]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[EMMC]:Single Block Write! %d\r\n", i);
            status = AlEmmc_WriteSingleBlock(WriteBuffer,i,EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            MMC_PRINT("[EMMC]:Single Block Read! %d\r\n", i);
            status = AlEmmc_ReadSingleBlock(ReadBuffer, i, EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            #if 0
            for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                MMC_PRINT("%d\t", ReadBuffer[m]);
            }
            MMC_PRINT("\n");
            #endif
            for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++) {
                if (WriteBuffer[j] != ReadBuffer[j]) {
                    printf("[EMMC]:400k write read data error %d\r\n", j);
                    for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                        printf("%d\t", WriteBuffer[m]);
                    }
                    printf("\r\n");
                    for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                        printf("%d\t", ReadBuffer[m]);
                    }
                    printf("\r\n");
                    status = MMC_FAILURE;
                    return status;
                }
            }
            memset(ReadBuffer, 0, BUF_SIZE);
        }*/
    }
#if 0
    printf("[EMMC]:Write Block\r\n");
    for (uint64_t i = 0; i < blocknum; i++) {
        status = AlEmmc_WriteSingleBlock(WriteBuffer, i,EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            return status;
        }
        printf("%d\t", i);
    }
    printf("[EMMC]:Read Block\r\n");
    for (uint64_t i = 0; i < blocknum; i++) {
        status = AlEmmc_ReadSingleBlock(WriteBuffer, i,EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            return status;
        }
        printf("%d\t", i);
        status = strcmp(WriteBuffer, ReadBuffer);
        if (status == MMC_SUCCESS)
        {
            MMC_PRINT("[EMMC]:Single Block Read/Write Success!\r\n");
        } else {
            printf("[EMMC]:Single Block Read/Write Data Not Match!\r\n");
            return status;
        }
    }
    
#endif
#if 0   //one write one read 
    blocknum = 1024;
    for (uint64_t i = 0; i < blocknum; i++) {
        printf("[EMMC]:Read/Write Block count : %d\r\n", i);
        //MMC_PRINT("[EMMC]:Single Block Write!\r\n");
        status = AlEmmc_WriteSingleBlock(WriteBuffer, i,EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            return status;
        }
        //_delay_ms(100);
        //MMC_PRINT("[EMMC]:Single Block Read!\r\n");
        status = AlEmmc_ReadSingleBlock(ReadBuffer, i, EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            return status;
        }
        for (int j = 0; j < EmmcCardInfo.CardBlockSize; j++) {
            if (WriteBuffer[j] != ReadBuffer[j]) {
                printf("[EMMC]:400k write read data error %d\r\n", j);
                for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                    printf("%d\t", WriteBuffer[m]);
                }
                printf("\r\n");
                for (int m = 0; m < EmmcCardInfo.CardBlockSize; m++) {
                    printf("%d\t", ReadBuffer[m]);
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
    for (int i = 0; i < BUF_SIZE; i++) {
        WriteBuffer[i] = i%26+0x30;
    }
    memset(ReadBuffer, 0, BUF_SIZE);
    printf("[CSU_EMMC]:Init!\r\n");
    status = Csu_RawEmmcInit(&emmcparam);
    if (status != MMC_SUCCESS) {
        return status;
    }
    

    do{ //wait for optimize
        printf("[CSU_EMMC]:Single Block Write!\r\n");
        status = AlEmmc_WriteSingleBlock(WriteBuffer,0,EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            return status;
        }
        status = AlEmmc_WriteSingleBlock(WriteBuffer,1,EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            return status;
        }
        status = AlEmmc_WriteSingleBlock(WriteBuffer,2,EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            return status;
        }

        printf("[CSU_EMMC]:Byte Read with no cross area!\r\n");
        Csu_RawEmmcRead(200, ReadBuffer, CSU_TEST_LENGTH1);
        if (status != MMC_SUCCESS) {
            return status;
        }
        for (i = 0; i < CSU_TEST_LENGTH1; i++) {
            MMC_PRINT("%d\t", ReadBuffer[i]);
        }
        MMC_PRINT("\r\n");
        memset(ReadBuffer, 0, sizeof(ReadBuffer));
        printf("[CSU_EMMC]:Byte Read with cross area 1!\r\n");
        Csu_RawEmmcRead(300, ReadBuffer, CSU_TEST_LENGTH2);
        if (status != MMC_SUCCESS) {
            return status;
        }
        for (i = 0; i < CSU_TEST_LENGTH2; i++) {
            MMC_PRINT("%d\t", ReadBuffer[i]);
        }
        MMC_PRINT("\r\n");
        memset(ReadBuffer, 0, sizeof(ReadBuffer));
        printf("[CSU_EMMC]:Byte Read with cross area 2!\r\n");
        Csu_RawEmmcRead(300, ReadBuffer, CSU_TEST_LENGTH3);
        if (status != MMC_SUCCESS) {
            return status;
        }
        for (i = 0; i < CSU_TEST_LENGTH3; i++) {
            MMC_PRINT("%d\t", ReadBuffer[i]);
        }
        MMC_PRINT("\r\n");
        memset(ReadBuffer, 0, BUF_SIZE);
        j++;
        if (j == 1) {
            Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
        }else if (j == 2) {
            Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
        }
    }while(j<3);
    
    return status;
}

//Traverse test: write, read, check every block
uint32_t Emmc_TraverseTest(void)
{
    uint32_t status = MMC_SUCCESS;
    uint64_t blocknum = 0;
    char *writebuffer = (char *)0x6103f000;
    char *readbuffer = (char *)0x6103f800;
    MMC_GPRINT("[G]:==========Emmc Traverse Test!==========\r\n");
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        writebuffer[i] = i%10+0x32;
    }
    MMC_GPRINT("[G]:Emmc Init!\r\n");
    status = AlEmmc_Init();
    if (status != MMC_SUCCESS) {
        return status;
    }
    
    blocknum = EmmcCardInfo.CardCapacity / EmmcCardInfo.CardBlockSize;
    MMC_GPRINT("[G]:Emmc <blocknum> is 0x%x!\r\n", blocknum);
    blocknum = BLOCK_128MB_NUM;
    MMC_GPRINT("[G]:Emmc <blocknum> checked is 0x%x!\r\n", blocknum);
    MMC_GPRINT("[G]:Emmc Write, Read Block!\r\n");
    uint64_t prei = 0, tempi = 0;
    uint64_t i = 0;
    for (; i < blocknum; i++) {
        if (i == blocknum/2) {
            MMC_GPRINT("[G]:Emmc Change Freq 10M!\r\n");
            Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
        }

        if(((i+1) % 0x40) ==0 )
            MMC_GPRINT("[G]:Write, Read Block 0x%x\r\n", i);

        status = AlEmmc_WriteSingleBlock(writebuffer,i,EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            goto END;
        }
        status = AlEmmc_ReadSingleBlock(readbuffer, i, EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            goto END;
        }
        status = strncmp(writebuffer, readbuffer, EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            goto END;
        }
        memset(readbuffer, 0, EmmcCardInfo.CardBlockSize);
    }
    MMC_GPRINT("[G]:==========Emmc Traverse Test Success!==========\r\n");
END:
    MMC_GPRINT("\r\n");
    if (i != blocknum)
        MMC_GPRINT("[G]:Emmc write, read check error block num is 0x%x\r\n", i);
    return status;
}

//Boundary Test: raw write, read, check data with memory cross boundary
uint32_t Emmc_BoundaryTest(void)
{
    uint32_t status = MMC_SUCCESS;
    uint64_t blocknum = 0;
    char *writebuffer = (char *)0x1003ddf0;
    char *readbuffer = (char *)0x1003edf0;
    MMC_GPRINT("[G]:==========Emmc Boundary Test!==========\r\n");
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        writebuffer[i] = i%10+0x32;
    }

    MMC_GPRINT("[G]:Emmc Init!\r\n");
    status = AlEmmc_Init();
    if (status != MMC_SUCCESS) {
        return status;
    }
    MMC_GPRINT("[G]:Emmc Change Freq 10M!\r\n");
    Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
    blocknum = 3;

    for (uint32_t k = 0; k < 0x200; k+=8) {
        writebuffer += k;
        readbuffer += k;
        MMC_GPRINT("[G]:writebuffer is %x, readbuffer is %x\r\n", writebuffer, readbuffer);
        for (uint64_t i = 0; i < blocknum; i++) {
            MMC_GPRINT("[G]:write, read test block%d\r\n", i);
            status = AlEmmc_WriteSingleBlock(writebuffer,i,EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                goto END;
            }
            status = AlEmmc_ReadSingleBlock(readbuffer, i, EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                goto END;
            }
            status = strncmp(writebuffer, readbuffer, EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                goto END;
            }
            memset(readbuffer, 0, EmmcCardInfo.CardBlockSize);
        }
    }
    MMC_GPRINT("[G]:==========Emmc Boundary Test Success!==========\r\n");
END:
    return status;
}

//Fatfs Test: write, read file
uint32_t Emmc_FatfsTest(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    char *filename = "1:/BOOT.bin";
    char *writebuffer = (char *)0x20000000;
    char *readbuffer = (char *)0x30000000;

    MMC_GPRINT("[G]:Emmc print MSHC reg before test flow!\r\n");
    PrintfMshcBlock(eMMC);

    MMC_GPRINT("[G]:==========Emmc Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"1:",1);
    MMC_GPRINT("[G]:res_sd is %d\r\n", res_sd);
    if (res_sd == FR_NO_FILESYSTEM)
    {
#if _USE_MKFS
        MMC_GPRINT("[G]:sd no file system, Wait for sd mkfs...\r\n");
        res_sd=f_mkfs("1:", FM_FAT32, 0, (void *)0x11000000, _MAX_SS);
        MMC_GPRINT("[G]:res_sd is %d\r\n", res_sd);
        if (res_sd == FR_OK)
        {
            res_sd = f_mount(NULL,"1:",1);
            MMC_GPRINT("[G]:res_sd is %d\r\n", res_sd);
            res_sd = f_mount(&fs,"1:",1);
            MMC_GPRINT("[G]:res_sd is %d\r\n", res_sd);
        }
#endif
    }
    Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
#if !_FS_READONLY
    res_sd = f_open(&fnew, filename,FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
        res_sd=f_write(&fnew,(const void *)writebuffer, FIL_LARGE_RDWR_SIZE, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File write success, data byte num is %d\r\n",fnum);
        } else {
            MMC_GPRINT("[G]:File write fail (%d)\r\n",res_sd);
        }
        f_close(&fnew);
    } else {
        MMC_GPRINT("[G]:File open fail!(%d)\r\n", res_sd);
    }
#endif
    res_sd = f_open(&fnew, filename, FA_OPEN_EXISTING | FA_READ);
    if (res_sd == FR_OK)
    {
        /*res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }*/

        res_sd = f_stat(filename, &fno) ;
        if (res_sd == FR_OK) {
            MMC_GPRINT("[G]:stat completed.\r\n");
        } else {
            MMC_GPRINT("[G]:stat error: %d\r\n", res_sd);
            return -1;
        }
        res_sd = f_read(&fnew, (const void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp(writebuffer, readbuffer, FIL_LARGE_RDWR_SIZE);
            if (status != MMC_SUCCESS) {
                MMC_GPRINT("[G]:File read error, data not match!\r\n");
                goto END;
            }
        } else {
            MMC_GPRINT("[G]:File read fail (%d)\r\n",res_sd);
        }
    } else {
        MMC_GPRINT("[G]:File open fail!(%d)\r\n", res_sd);
    }

    MMC_GPRINT("[G]:Emmc print MSHC reg after test flow!\r\n");
    PrintfMshcBlock(eMMC);
    MMC_GPRINT("[G]:==========Emmc Fatfs Test Success!==========\r\n");
END:
    f_close(&fnew);
    f_mount(NULL,"1:",1);
    MMC_GPRINT("[G]:Emmc print MSHC reg after test flow!\r\n");
    PrintfMshcBlock(eMMC);
    return status;
}

//Branch test: print branch log
uint32_t Emmc_BranchTest(void)
{
    uint32_t status = MMC_SUCCESS;
    uint64_t blocknum = 0;
    char *writebuffer = (char *)0x10040000;
    char *readbuffer = (char *)0x10041000;
    RawEmmcParam_t emmcparam;

    MMC_GPRINT("[G]:==========Emmc Branch Test!==========\r\n");
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        writebuffer[i] = i%10+0x32;
    }
#ifdef USE_ERROR_BRANCH
    ERROR_BRANCH_STOP();
#endif
    MMC_GPRINT("[G]:==========Correct Branch!==========\r\n");
    status = Csu_RawEmmcInit(&emmcparam);
    if (status != MMC_SUCCESS) {
        return status;
    }

    Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);

    for (uint32_t i = 0; i < 3; i++) {
        status = AlEmmc_WriteSingleBlock(writebuffer,i,EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            goto END;
        }
    }
    Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
    status = Csu_RawEmmcRead(BRANCHTEST_EMMC_READ_OFFSET, readbuffer, BRANCHTEST_EMMC_READ_LENGTH);
    if (status != MMC_SUCCESS) {
        goto END;
    }
    
    for (uint32_t i = 0; i < BRANCHTEST_EMMC_READ_LENGTH; i++) {
        if (writebuffer[(i+BRANCHTEST_EMMC_READ_OFFSET)%EmmcCardInfo.CardBlockSize] != readbuffer[i]) {
            MMC_GPRINT("[G]:Emmc barnch test data check failed!\r\n");
            status = MMC_FAILURE;
            goto END;
        }
    }
#ifdef AL_DEBUG_PRINT
    MMC_GPRINT("[G]:Emmc Branch Count!\r\n");
    for (uint32_t i = 0; i < BRANCH_MAX; i++) {
        MMC_GPRINT("[BRANCH_COUNT]: [%d]\t[%d]\r\n", i, BranchTestCount[i]);
    }
#endif
#ifdef USE_ERROR_BRANCH
    //coverage error branch
    MMC_GPRINT("[G]:==========Error Branch!==========\r\n");
    ERROR_BRANCH_START();
    while (EMMC_ERROR_BRANCH_CHECK_NOTDONE(BERROR_MAX)) {
        MMC_GPRINT("[ERROR_BRANCH]:0x%x\r\n", ErrBranchCtrl);

        ResetHostComtroller(eMMC);

        status = Csu_RawEmmcInit(&emmcparam);
        if (status != MMC_SUCCESS) {
            goto ERROR_STATUS;
        }

        Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);

        for (uint32_t i = 0; i < 3; i++) {
            status = AlEmmc_WriteSingleBlock(writebuffer,i,EmmcCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                goto ERROR_STATUS;
            }
        }
        Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_400K);
        status = Csu_RawEmmcRead(BRANCHTEST_EMMC_READ_OFFSET, readbuffer, BRANCHTEST_EMMC_READ_LENGTH);
        if (status != MMC_SUCCESS) {
            goto ERROR_STATUS;
        }
        
        for (uint32_t i = 0; i < BRANCHTEST_EMMC_READ_LENGTH; i++) {
            if (writebuffer[(i+BRANCHTEST_EMMC_READ_OFFSET)%EmmcCardInfo.CardBlockSize] != readbuffer[i]) {
                MMC_GPRINT("[G]:Emmc barnch test data check failed!\r\n");
                status = MMC_FAILURE;
                goto ERROR_STATUS;
            }
        }
ERROR_STATUS:
        MMC_GPRINT("[G]:Status = %d\r\n", status);
    }
#endif
    MMC_GPRINT("[G]:==========Emmc Branch Test Done!==========\r\n");

#ifdef AL_DEBUG_PRINT
    MMC_GPRINT("[G]:Emmc Branch Count!\r\n");
    for (uint32_t i = 0; i < BRANCH_MAX; i++) {
        MMC_GPRINT("[BRANCH_COUNT]: [%d]\t[%d]\r\n", i, BranchTestCount[i]);
    }
#endif
    MMC_GPRINT("[G]:==========Emmc Branch Test Success!==========\r\n");
END:
    return status;
}

//Byte read test: use csu raw read func
uint32_t Emmc_ByteReadTest(void)
{
    uint32_t status = MMC_SUCCESS;
    RawEmmcParam_t emmcparam;
    char *writebuffer = (char *)0x10040000;
    char *readbuffer = (char *)0x20000000;

    MMC_GPRINT("[G]:==========Emmc Byte Read Test!==========\r\n");
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        writebuffer[i] = i%10+0x32;
    }

    MMC_GPRINT("[G]:Emmc Init!\r\n");
    status = Csu_RawEmmcInit(&emmcparam);
    if (status != MMC_SUCCESS) {
        return status;
    }
    MMC_GPRINT("[G]:Emmc Change Freq 10M!\r\n");
    Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);

    MMC_GPRINT("[G]:Emmc Write Block 1MB!\r\n");
    for (uint32_t i = 0; i < BLOCK_1MB_NUM; i++) {
        status = AlEmmc_WriteSingleBlock(writebuffer, i, EmmcCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            goto END;
        }
    }
    uint64_t bytelength = BLOCK_1MB_NUM * EmmcCardInfo.CardBlockSize;
    MMC_GPRINT("[G]:Emmc Read ByteLength %llu!\r\n", bytelength);
    for (uint64_t i = EMMC_OFFSET_START; i < bytelength - EMMC_LENGTH_START; i+=i) {
        for (uint64_t j = EMMC_LENGTH_START; i + j < bytelength - i; j+=j) {
            MMC_GPRINT("[G]:Emmc Read Byte offset %llu, length %llu!\r\n", i, j);
            Csu_RawEmmcRead(i, readbuffer, j);
            if (status != MMC_SUCCESS) {
                return status;
            }
            for (uint64_t k = 0; k < j; k++) {
                if (readbuffer[k] != writebuffer[(i+k)%EmmcCardInfo.CardBlockSize]) {
                    status = MMC_FAILURE;
                    goto END;
                }
            }
        }
    }
    /*for (uint64_t i = 0; i < bytelength - EMMC_LENGTHSTEP; i+=EMMC_OFFSETSTEP) {
        for (uint64_t j = EMMC_LENGTHSTEP; j < bytelength - i; j+=EMMC_LENGTHSTEP) {
            MMC_GPRINT("[G]:Emmc Read Byte offset %llu, length %llu!\r\n", i, j);
            Csu_RawEmmcRead(i, readbuffer, j);
            if (status != MMC_SUCCESS) {
                return status;
            }
            for (uint64_t k = 0; k < j; k++) {
                if (readbuffer[k] != writebuffer[(i+k)%EmmcCardInfo.CardBlockSize]) {
                    status = MMC_FAILURE;
                    goto END;
                }
            }
        }
    }*/
    MMC_GPRINT("[G]:==========Emmc Byte Read Test Success!==========\r\n");

END:
    return status;
}

//Print Reg test: print branch log
uint32_t Emmc_PrintRegTest(void)
{
    uint32_t status = MMC_SUCCESS;
    char *writebuffer = (char *)0x10040000;
    char *readbuffer = (char *)0x10041000;
    uint32_t j = 0;

    MMC_GPRINT("[G]:==========Emmc Print Reg Test!==========\r\n");
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        writebuffer[i] = i%10+0x32;
    }

    MMC_GPRINT("[G]:Emmc print MSHC reg before test flow!\r\n");
    PrintfMshcBlock(eMMC);

    MMC_GPRINT("[G]:Emmc Init!\r\n");
    status = AlEmmc_Init();
    if (status != MMC_SUCCESS) {
        return status;
    }
    MMC_GPRINT("[G]:Emmc Change Freq 10M!\r\n");
    Csu_RawEmmcSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);

    status = AlEmmc_WriteSingleBlock(writebuffer,0,EmmcCardInfo.CardBlockSize);
    if (status != MMC_SUCCESS) {
        goto END;
    }
    status = AlEmmc_ReadSingleBlock(readbuffer, 0, EmmcCardInfo.CardBlockSize);
    if (status != MMC_SUCCESS) {
        goto END;
    }
    status = strncmp(writebuffer, readbuffer, EmmcCardInfo.CardBlockSize);
    if (status != MMC_SUCCESS) {
        goto END;
    }

    MMC_GPRINT("[G]:Emmc print MSHC reg after test flow!\r\n");
    PrintfMshcBlock(eMMC);
    
    MMC_GPRINT("[G]:==========Emmc Print Reg Test Success!==========\r\n");
END:
    return status;
}

uint32_t EMMC_Test(void)
{
    uint32_t status = 0;

    Enablepinmux1_mode2();

#ifdef USE_ERROR_BRANCH
    ERROR_BRANCH_STOP();
#endif

#ifdef EMMC_BRANCHTEST
#ifdef AL_DEBUG_PRINT
    DebugCurType = ((DEBUG_BRANCHTEST) | (DEBUG_GENERAL));
#endif
    ResetHostComtroller(eMMC);
    status = Emmc_BranchTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Emmc Branch Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Emmc Branch Test Success\r\n");
    }
#endif

#ifdef EMMC_PRINTREGTEST
#ifdef AL_DEBUG_PRINT
    DebugCurType = ((DEBUG_INFO) | (DEBUG_GENERAL));
#endif
    ResetHostComtroller(eMMC);
    status = Emmc_PrintRegTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Emmc Print Reg Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Emmc Print Reg Test Success\r\n");
    }
#endif

#ifdef EMMC_BOUNDARYTEST
#ifdef AL_DEBUG_PRINT
    DebugCurType = ((DEBUG_GENERAL));
#endif
    ResetHostComtroller(eMMC);
    status = Emmc_BoundaryTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Emmc Boundary Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Emmc Boundary Test Success\r\n");
    }
#endif

#ifdef EMMC_FATFSTEST
#ifdef AL_DEBUG_PRINT
    DebugCurType = ((DEBUG_GENERAL));
#endif
    ResetHostComtroller(eMMC);
    status = Emmc_FatfsTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Emmc Fatfs Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Emmc Fatfs Test Success\r\n");
    }
#endif

#ifdef EMMC_BYTEREADTEST
#ifdef AL_DEBUG_PRINT
    DebugCurType = ((DEBUG_GENERAL));
#endif
    ResetHostComtroller(eMMC);
    status = Emmc_ByteReadTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Emmc Byte Read Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Emmc Byte Read Test Success\r\n");
    }
#endif

#ifdef EMMC_TRAVERSETEST
#ifdef AL_DEBUG_PRINT
    DebugCurType = ((DEBUG_GENERAL));
#endif
    ResetHostComtroller(eMMC);
    status = Emmc_TraverseTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Emmc Traverse Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Emmc Traverse Test Success\r\n");
    }
#endif
    return status;
}

/***************************************************************************/
/**
 * @brief	test EMMC/EMMC read/write
 *
 * @param  None
 * @return MMC_SUCCESS
 *
 ******************************************************************************/
/*uint32_t EMMC_Test(void)
{
	uint32_t fnum;            			  
	char *ReadBuffer = (char *)0x12000000;
	char *WriteBuffer =  (char *)0x11000000;
	FIL fnew;
    FILINFO fno;
	uint32_t status = MMC_SUCCESS;
    //memset((void *)0x10000000, 0, 0x10000000);
    while(1) {
#if 0
    printf("[START]:<EMMC>\r\n");
    status = RawReadWriteTestEmmc();
    if (status != MMC_SUCCESS) {
        printf("[FAIL]:<EMMC>, [ERRORCODE]:<%d>\r\n", status);
        return status;
    } else {
        printf("[SUCCESS]:<EMMC>\r\n");   //方括号[]内为关键词，尖括号<>内为参数
    }
    printf("[DONE]:<EMMC>\r\n");
#endif
#if 0
    CsuRawReadWriteTestEMMC();
#endif
#if 1
    char *filename = "0:/test.bin";
    memset(&fnew, 0, sizeof(FIL));
    memset(&fno, 0, sizeof(FILINFO));
    res_emmc = f_mount(&fs,"1:",1);  //EMMC test
    printf("res_emmc is %d\r\n", res_emmc);
    if (res_emmc == FR_NO_FILESYSTEM) {
#if _USE_MKFS
        printf("sd no file system, Wait for sd mkfs...");
        res_emmc=f_mkfs("0:", FM_FAT32, 0, (void *)0x10000000, _MAX_SS);
        printf("res_emmc is %d\r\n", res_emmc);
        if (res_emmc == FR_OK) {
            res_emmc = f_mount(NULL,"1:",1);
            printf("res_emmc is %d\r\n", res_emmc);
            res_emmc = f_mount(&fs,"1:",1);
            printf("res_emmc is %d\r\n", res_emmc);
        }
#endif
    }
#if !_FS_READONLY
    res_emmc = f_open(&fnew, filename,FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_emmc == FR_OK )
    {
        res_emmc=f_write(&fnew,(const void *)WriteBuffer,FIL_LARGE_RDWR_SIZE,&fnum);
        if (res_emmc==FR_OK) {
            printf("File write success, data byte num is %d\r\n",fnum);
        } else {
            printf("File write fail (%d)\r\n",res_emmc);
        }
        f_close(&fnew);
    } else {
        printf("File open fail!(%d)\r\n", res_emmc);
    }
#endif
    res_emmc = f_open(&fnew, filename, FA_OPEN_EXISTING | FA_READ);
    if (res_emmc == FR_OK)
    {
        res_emmc = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_emmc==FR_OK)
        {
            printf("File lseek success!\r\n");
        } else {
            printf("File lseek fail! Error code is %d\r\n", res_emmc);
        }
        res_emmc = f_stat(filename, &fno) ;
        if (res_emmc == FR_OK) {
            printf("stat completed.\r\n");
        } else {
            printf("stat error: %d\r\n", res_emmc);
            return -1;
        }
        //memset(ReadBuffer, 0, sizeof(ReadBuffer));
        res_emmc = f_read(&fnew, (const void *)ReadBuffer, fno.fsize, &fnum);
        if (res_emmc==FR_OK)
        {
            printf("File read success, data byte num is %d\r\n",fnum);
            for (int i = FIL_PT_OFFSET; i < sizeof(WriteBuffer)-FIL_PT_OFFSET; i++) {
                if (WriteBuffer[i] != ReadBuffer[i-FIL_PT_OFFSET]) {
                    printf("File read error, data not match\r\n");
                    break;
                }
            }
        } else {
            printf("File read fail (%d)\n",res_emmc);
        }
    } else {
        printf("File open fail!(%d)\r\n", res_emmc);
    }
    f_close(&fnew);
    f_mount(NULL,"1:",1);
#endif
    }
    

    return status;
}

*/
