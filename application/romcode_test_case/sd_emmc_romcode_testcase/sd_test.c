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
#define FIL_LARGE_RDWR_SIZE 36480


#define SD_TRAVERSETEST
#define SD_BRANCHTEST
#define SD_BOUNDARYTEST
#define SD_FATFSTEST
#define SD_PRINTREGTEST

uint32_t DebugCurType = DEBUG_GENERAL;
//static FRESULT res_sd;
//static FATFS fs;
static BYTE *WriteBuffer = (char *)0x1003ddf0;
static BYTE *ReadBuffer = (char *)0x1003edf0;
//extern char *logaddr;

uint32_t RawReadWriteTestSD()
{
    int status;
    int result;
    uint64_t blocknum = 0;
    MMC_PRINT("[SD]:Read/Write Buf Set!\r\n");
    for (int i = 0; i < BUF_SIZE; i++) {
        WriteBuffer[i] = i%10+0x32;
    }
    memset(ReadBuffer, 0, BUF_SIZE);
    MMC_PRINT("[SD]:Init!\r\n");
    status = AlSd_Init();
    if (status != MMC_SUCCESS) {
        return status;
    }
    
    blocknum = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;
    printf("block num is %d\r\n", blocknum);
    blocknum = 3;
    for (int k = 0; k < 0x20; k+=8) {
        //WriteBuffer = 0x6103ddf0 + k;
        //ReadBuffer = 0x6103edf0 + k;
        WriteBuffer = 0x1003edf0 + k;
        ReadBuffer = 0x1003f6f0 + k;
        printf("[SD]:Block write/read addr offset is %x\r\n", k);
        //switch 10M
        Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M); 
        printf("[SD]:set 10M!\r\n");
        for (int i = 0; i < blocknum; i++) {
            printf("[SD]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[SD]:Single Block Write! %d\r\n", i);
            status = AlSd_WriteSingleBlock(WriteBuffer,i,SDCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            MMC_PRINT("[SD]:Single Block Read! %d\r\n", i);
            status = AlSd_ReadSingleBlock(ReadBuffer, i, SDCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            for (int j = 0; j < SDCardInfo.CardBlockSize; j++) {
                if (WriteBuffer[j] != ReadBuffer[j]) {
                    printf("[SD]:10M write read data error %d\r\n", j);
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++) {
                        printf("%3d\t", WriteBuffer[j]);
                    }
                    printf("\r\n");
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++) {
                        printf("%3d\t", ReadBuffer[j]);
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
        for (int i = 0; i < blocknum; i++) {
            printf("[SD]:Block write/read check! %d\r\n", i);
            MMC_PRINT("[SD]:Single Block Write! %d\r\n", i);
            status = AlSd_WriteSingleBlock(WriteBuffer,i,SDCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            MMC_PRINT("[SD]:Single Block Read! %d\r\n", i);
            status = AlSd_ReadSingleBlock(ReadBuffer, i, SDCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                return status;
            }
            #if 0
            for (int j = 0; j < SDCardInfo.CardBlockSize; j++) {
                MMC_PRINT("%d\t", ReadBuffer[j]);
            }
            MMC_PRINT("\n");
            #endif
            for (int j = 0; j < SDCardInfo.CardBlockSize; j++) {
                if (WriteBuffer[j] != ReadBuffer[j]) {
                    printf("[SD]:400k write read data error %d\r\n", j);
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++) {
                        printf("%3d\t", WriteBuffer[j]);
                    }
                    printf("\r\n");
                    for (int j = 0; j < SDCardInfo.CardBlockSize; j++) {
                        printf("%3d\t", ReadBuffer[j]);
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

void __attribute__((inline)) Sd_PrintBS(uint64_t num)
{
    static uint32_t prenum = 0;
    if ((num >> 8) != prenum) {
        prenum = num;
        do{
            MMC_GPRINT("\b");
            num >>=4;
        }while(num);
        MMC_GPRINT("%x", prenum);
        prenum >>= 8;
    }
}

//Traverse test: write, read, check every block
uint32_t Sd_TraverseTest(void)
{
    uint32_t status = MMC_SUCCESS;
    uint64_t blocknum = 0;
    char *writebuffer = (char *)0x10040000;
    char *readbuffer = (char *)0x10041000;
    MMC_GPRINT("[G]:==========Sd Traverse Test!==========\r\n");
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        writebuffer[i] = i%10+0x32;
    }
    MMC_GPRINT("[G]:Sd Init!\r\n");
    status = AlSd_Init();
    if (status != MMC_SUCCESS) {
        return status;
    }
    MMC_GPRINT("[G]:Sd Change Freq 10M!\r\n");
    Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);

    blocknum = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;
    MMC_GPRINT("[G]:Sd <blocknum> is 0x%x!\r\n", blocknum);
    blocknum = BLOCK_128MB_NUM;
    MMC_GPRINT("[G]:Sd <blocknum> checked is 0x%x!\r\n", blocknum);
    MMC_GPRINT("[G]:Sd Write, Read Block!\r\n");
    uint64_t prei = 0, tempi = 0;
    uint64_t i = 0;
    for (; i < blocknum; i++) {
        MMC_GPRINT("[G]:Write, Read Block 0x%x\r\n", i);
        //Sd_PrintBS(i);
        /*if ((i >> 8) != prei) {
            tempi = prei;
            MMC_GPRINT("\b\b");
            do{
                MMC_GPRINT("\b");
                tempi >>=4;
            }while(tempi);
            MMC_GPRINT("%x", i);
            prei = (i >> 8);
        }*/
        status = AlSd_WriteSingleBlock(writebuffer,i,SDCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            goto END;
        }
        status = AlSd_ReadSingleBlock(readbuffer, i, SDCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            goto END;
        }
        status = strncmp(writebuffer, readbuffer, SDCardInfo.CardBlockSize);
        if (status != MMC_SUCCESS) {
            goto END;
        }
        memset(readbuffer, 0, SDCardInfo.CardBlockSize);
    }

END:
    MMC_GPRINT("\r\n");
    MMC_GPRINT("[G]:Sd write, read check error block num is 0x%x\r\n", i);
    return status;
}

//Boundary Test: raw write, read, check data with memory cross boundary
uint32_t Sd_BoundaryTest(void)
{
    uint32_t status = MMC_SUCCESS;
    uint64_t blocknum = 0;
    char *writebuffer = (char *)0x1003ddf0;
    char *readbuffer = (char *)0x1003edf0;
    MMC_GPRINT("[G]:==========Sd Boundary Test!==========\r\n");
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        writebuffer[i] = i%10+0x32;
    }

    MMC_GPRINT("[G]:Sd Init!\r\n");
    status = AlSd_Init();
    if (status != MMC_SUCCESS) {
        return status;
    }
    MMC_GPRINT("[G]:Sd Change Freq 10M!\r\n");
    Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
    blocknum = 3;

    for (uint32_t k = 0; k < 0x200; k+=8) {
        writebuffer += k;
        readbuffer += k;
        MMC_GPRINT("[G]:writebuffer is %x, readbuffer is %x\r\n", writebuffer, readbuffer);
        for (uint64_t i = 0; i < blocknum; i++) {
            MMC_GPRINT("[G]:write, read test block%d\r\n", i);
            status = AlSd_WriteSingleBlock(writebuffer,i,SDCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                goto END;
            }
            status = AlSd_ReadSingleBlock(readbuffer, i, SDCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                goto END;
            }
            status = strncmp(writebuffer, readbuffer, SDCardInfo.CardBlockSize);
            if (status != MMC_SUCCESS) {
                goto END;
            }
            memset(readbuffer, 0, SDCardInfo.CardBlockSize);
        }
    }
END:
    return status;
}

//Fatfs Test: write, read file
uint32_t Sd_FatfsTest(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    char *filename = "0:/BOOT.bin";
    char *writebuffer = (char *)0x20000000;
    char *readbuffer = (char *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"0:",1);
    MMC_GPRINT("[G]:res_sd is %d\r\n", res_sd);
    if (res_sd == FR_NO_FILESYSTEM)
    {
#if _USE_MKFS
        MMC_GPRINT("[G]:sd no file system, Wait for sd mkfs...\r\n");
        res_sd=f_mkfs("0:", FM_FAT32, 0, (void *)0x11000000, _MAX_SS);
        MMC_GPRINT("[G]:res_sd is %d\r\n", res_sd);
        if (res_sd == FR_OK)
        {
            res_sd = f_mount(NULL,"0:",1);
            MMC_GPRINT("[G]:res_sd is %d\r\n", res_sd);
            res_sd = f_mount(&fs,"0:",1);
            MMC_GPRINT("[G]:res_sd is %d\r\n", res_sd);
        }
#endif
    }
    Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
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
END:
    f_close(&fnew);
    f_mount(NULL,"0:",1);
    return status;
}

//Branch test: print branch log
uint32_t Sd_BranchTest(void)
{
    uint32_t status = MMC_SUCCESS;
    uint64_t blocknum = 0;
    char *writebuffer = (char *)0x10040000;
    char *readbuffer = (char *)0x10041000;

    MMC_GPRINT("[G]:==========Sd Branch Test!==========\r\n");
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        writebuffer[i] = i%10+0x32;
    }

    MMC_GPRINT("[G]:Sd Init!\r\n");
    status = AlSd_Init();
    if (status != MMC_SUCCESS) {
        return status;
    }
    MMC_GPRINT("[G]:Sd Change Freq 10M!\r\n");
    Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);

    status = AlSd_WriteSingleBlock(writebuffer,0,SDCardInfo.CardBlockSize);
    if (status != MMC_SUCCESS) {
        goto END;
    }
    status = AlSd_ReadSingleBlock(readbuffer, 0, SDCardInfo.CardBlockSize);
    if (status != MMC_SUCCESS) {
        goto END;
    }
    status = strncmp(writebuffer, readbuffer, SDCardInfo.CardBlockSize);
    if (status != MMC_SUCCESS) {
        goto END;
    }
    MMC_GPRINT("[G]:Sd Branch Count!\r\n");
    for (uint32_t i = 0; i < BRANCH_NUM; i++) {
        MMC_GPRINT("[G]: [%d]\t", i);
        for (uint32_t j = 0; j < BRANCH_BRANCH; j++) {
            MMC_GPRINT("[%d]\t", BranchTestCount[i][j]);
        }
        MMC_GPRINT("\r\n");
    }
END:
    return status;
}

//Print Reg test: print branch log
uint32_t Sd_PrintRegTest(void)
{
    uint32_t status = MMC_SUCCESS;
    char *writebuffer = (char *)0x10040000;
    char *readbuffer = (char *)0x10041000;

    MMC_GPRINT("[G]:==========Sd Print Reg Test!==========\r\n");
    for (uint32_t i = 0; i < BUF_SIZE; i++) {
        writebuffer[i] = i%10+0x32;
    }

    MMC_GPRINT("[G]:Sd Init!\r\n");
    status = AlSd_Init();
    if (status != MMC_SUCCESS) {
        return status;
    }
    MMC_GPRINT("[G]:Sd Change Freq 10M!\r\n");
    Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);

    status = AlSd_WriteSingleBlock(writebuffer,0,SDCardInfo.CardBlockSize);
    if (status != MMC_SUCCESS) {
        goto END;
    }
    status = AlSd_ReadSingleBlock(readbuffer, 0, SDCardInfo.CardBlockSize);
    if (status != MMC_SUCCESS) {
        goto END;
    }
    status = strncmp(writebuffer, readbuffer, SDCardInfo.CardBlockSize);
    if (status != MMC_SUCCESS) {
        goto END;
    }

END:
    return status;
}

uint32_t SD_Test(void)
{
    uint32_t status = 0;

#ifdef SD_BRANCHTEST
    DebugCurType = ((DEBUG_BRANCHTEST) | (DEBUG_GENERAL));
    status = Sd_BranchTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Sd Branch Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Sd Branch Test Success\r\n");
    }
#endif

#ifdef SD_PRINTREGTEST
    DebugCurType = ((DEBUG_INFO) | (DEBUG_GENERAL));
    status = Sd_PrintRegTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Sd Print Reg Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Sd Print Reg Test Success\r\n");
    }
#endif

#ifdef SD_BOUNDARYTEST
    DebugCurType = ((DEBUG_GENERAL));
    status = Sd_BoundaryTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Sd Boundary Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Sd Boundary Test Success\r\n");
    }
#endif

#ifdef SD_FATFSTEST
    DebugCurType = ((DEBUG_GENERAL));
    status = Sd_FatfsTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Sd Fatfs Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Sd Fatfs Test Success\r\n");
    }
#endif

#ifdef SD_TRAVERSETEST
    DebugCurType = ((DEBUG_GENERAL));
    status = Sd_TraverseTest();
    if (status != MMC_SUCCESS) {
        MMC_GPRINT("[G]:Sd Traverse Test Error %d\r\n", status);
    } else {
        MMC_GPRINT("[G]:Sd Traverse Test Success\r\n");
    }
#endif
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
/*uint32_t SD_Test(void)
{
	uint32_t fnum = 0;            			  
	//char ReadBuffer[1024]={0};
	//char WriteBuffer[] = "welcome777777777777777\r\n";
    char *ReadBuffer = (char *)0x12000000;
    char *WriteBuffer = (char *)0x11000000;
    FIL fnew;
    FILINFO fno;
	uint32_t status = MMC_SUCCESS;

    do{
    memset(&fnew, 0, sizeof(FIL));
    memset(&fno, 0, sizeof(FILINFO));

    //clear ddr log
    //memset((char *)(0x10000000), 0, 0x100000);
    //logaddr = (char *)0x10000000;
#if 1
    printf("[START]:<SD>\r\n");
    status = RawReadWriteTestSD();
    if (status != MMC_SUCCESS) {
        printf("[FAIL]:<SD>, [ERRORCODE]:<%d>\r\n", status);
        return status;
    } else {
        printf("[SUCCESS]:<SD>\r\n");   //方括号[]内为关键词，尖括号<>内为参数
    }
    printf("[DONE]:<SD>\r\n");
#endif

#if 1
    char *filename = "0:/BOOT.bin";
	res_sd = f_mount(&fs,"0:",1);  //SD test
    printf("res_sd is %d\r\n", res_sd);
    if (res_sd == FR_NO_FILESYSTEM)
    {
#if _USE_MKFS
        printf("sd no file system, Wait for sd mkfs...\r\n");
        res_sd=f_mkfs("0:", FM_FAT32, 0, (void *)0x10000000, _MAX_SS);
        printf("res_sd is %d\r\n", res_sd);
        if (res_sd == FR_OK)
        {
            res_sd = f_mount(NULL,"0:",1);
            printf("res_sd is %d\r\n", res_sd);
            res_sd = f_mount(&fs,"0:",1);
            printf("res_sd is %d\r\n", res_sd);
        }
#endif
    }
    Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
#if !_FS_READONLY
    res_sd = f_open(&fnew, filename,FA_CREATE_ALWAYS | FA_WRITE );
    if ( res_sd == FR_OK )
    {
        res_sd=f_write(&fnew,(const void *)WriteBuffer, FIL_LARGE_RDWR_SIZE,&fnum);
        if (res_sd==FR_OK)
        {
            printf("File write success, data byte num is %d\r\n",fnum);
        } else {
            printf("File write fail (%d)\r\n",res_sd);
        }
        f_close(&fnew);
    } else {
        printf("File open fail!(%d)\r\n", res_sd);
    }//"test/READ.TXT"   /rt_files/weight.bin
#endif
    res_sd = f_open(&fnew, filename, FA_OPEN_EXISTING | FA_READ);
    if (res_sd == FR_OK)
    {
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            printf("File lseek success!\r\n");
        } else {
            printf("File lseek fail! Error code is %d\r\n", res_sd);
        }
        //memset(ReadBuffer, 0, sizeof(ReadBuffer));
        res_sd = f_stat(filename, &fno) ;
        if (res_sd == FR_OK) {
            printf("stat completed.\r\n");
        } else {
            printf("stat error: %d\r\n", res_sd);
            return -1;
        }
        res_sd = f_read(&fnew, (const void *)ReadBuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            printf("File read success, data byte num is %d\r\n",fnum);
            for (int i = 0; i < FIL_LARGE_RDWR_SIZE; i++) {
                if (WriteBuffer[i] != ReadBuffer[i]) {
                    printf("File read error, data not match\r\n");
                    break;
                }
            }
        } else {
            printf("File read fail (%d)\n",res_sd);
        }
    } else {
        printf("File open fail!(%d)\r\n", res_sd);
    }
    f_close(&fnew);
    f_mount(NULL,"0:",1);
#endif
}while(0);
    return status;
}*/

/*********************************************END OF FILE**********************/
