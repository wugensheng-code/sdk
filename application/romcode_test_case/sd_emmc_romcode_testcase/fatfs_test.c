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

#define BLOCK_LEN           0x200
#define BLOCK_NUM           1
#define BUF_SIZE            (BLOCK_LEN*BLOCK_NUM)
#define CSU_TEST_LENGTH1    300
#define CSU_TEST_LENGTH2    300
#define CSU_TEST_LENGTH3    600
#define FIL_PT_OFFSET       5
#define FIL_LARGE_RDWR_SIZE 8845488
#define DDR_TEST_LENGTH     1000


// #define SD_TRAVERSETEST
//#define SD_BRANCHTEST
// #define SD_BOUNDARYTEST
#define SD_FATFSTEST
// #define SD_PRINTREGTEST
// #define SD_DDRREADWRITETEST

static BYTE *WriteBuffer = (uint8_t *)0x1003ddf0;
static BYTE *ReadBuffer = (uint8_t *)0x1003edf0;
extern void Enablepinmux1(void);

uint32_t FatfsTestCase01(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/BOOT.bin";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase02(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2/3//1/ubuntu.iso";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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



uint32_t FatfsTestCase03(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2/3/1/[]=";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase04(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:end.xlsx";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase05(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:end.xlsx";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase06(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2/3/1/#$%^&.iso";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase07(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2/3/1/fffffffffffffffffffff.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase08(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase09(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"0:",0);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase10(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"2:",1);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase11(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"a:",1);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase12(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"*:",1);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase13(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"",1);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase14(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"*:",1);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase15(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(NULL,"",1);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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



uint32_t FatfsTestCase16(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"0:",2);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase17(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"0:",2);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase18(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2/3/1";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase19(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(NULL,"",0);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase20(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"0:",-1);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase21(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"0:",9);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase22(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(NULL,"0:",1);
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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase23(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase24(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
    res_sd = f_open(NULL, filename, FA_OPEN_EXISTING | FA_READ);
    if (res_sd == FR_OK)
    {
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase25(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
    res_sd = f_open(NULL, filename, 0xffff);
    if (res_sd == FR_OK)
    {
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase26(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
    res_sd = f_open(NULL, NULL, 0xffff);
    if (res_sd == FR_OK)
    {
        res_sd = f_lseek(&fnew, FIL_PT_OFFSET);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase27(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, -1);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase28(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(NULL, -1);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase29(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(NULL, 100000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase30(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(NULL, (void *)NULL, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase31(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(NULL, (void *)NULL, 0, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase32(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(NULL, (void *)NULL, -1, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase33(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

         res_sd = f_read(NULL, (void *)NULL, -1, NULL);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase34(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(NULL, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase35(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase36(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",1);
    return status;
}

uint32_t FatfsTestCase37(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",0);
    return status;
}

uint32_t FatfsTestCase38(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "1:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",0);
    return status;
}

uint32_t FatfsTestCase39(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "1:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",0);
    return status;
}

uint32_t FatfsTestCase40(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",0);
    return status;
}

uint32_t FatfsTestCase41(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",0);
    return status;
}

uint32_t FatfsTestCase42(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "-1:";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",0);
    return status;
}

uint32_t FatfsTestCase43(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2//3//1//1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
    res_sd = f_open(&fnew, filename, FA_OPEN_EXISTING | FA_WRITE);
    if (res_sd == FR_OK)
    {
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",0);
    return status;
}

uint32_t FatfsTestCase44(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2/3//1/ubuntu.iso";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
    res_sd = f_open(&fnew, filename, FA_OPEN_EXISTING | FA_WRITE);
    if (res_sd == FR_OK)
    {
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, 900000, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",1);
    return status;
}

uint32_t FatfsTestCase45(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "0:/2/3//1/ubuntu.iso";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
    res_sd = f_open(&fnew, filename, FA_OPEN_EXISTING | FA_WRITE);
    if (res_sd == FR_OK)
    {
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, 900000, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_close(NULL);
    f_mount(NULL,"0:",1);
    return status;
}


uint32_t FatfsTestCase100(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "1:/1/1.txt";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"1:",1);
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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_mount(NULL,"1:",1);
    return status;
}

uint32_t FatfsTestCase101(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "1:/2//3//1//ubuntu.iso";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"1:",1);
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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_mount(NULL,"1:",1);
    return status;
}

uint32_t FatfsTestCase102(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "1:/2//3//1//ubuntu.iso";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

    MMC_GPRINT("[G]:==========Sd Fatfs Test!==========\r\n");
    res_sd = f_mount(&fs,"1:",1);
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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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

uint32_t FatfsTestCase103(void)
{
    uint32_t status = MMC_SUCCESS;
    FRESULT res_sd;
    uint32_t fnum = 0;   
    FATFS fs;
    FIL fnew;
    FILINFO fno;
    const char *filename = "1:/2//3//1//ubuntu.iso";
    uint8_t *writebuffer = (uint8_t *)0x20000000;
    uint8_t *readbuffer = (uint8_t *)0x30000000;

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
        res_sd = f_lseek(&fnew, 10000);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File lseek success!\r\n");
        } else {
            MMC_GPRINT("[G]:File lseek fail! Error code is %d\r\n", res_sd);
        }

        res_sd = f_read(&fnew, (void *)readbuffer, fno.fsize, &fnum);
        if (res_sd==FR_OK)
        {
            MMC_GPRINT("[G]:File read success, data byte num is %d\r\n",fnum);
            status = strncmp((const char *)writebuffer, (const char *)readbuffer, FIL_LARGE_RDWR_SIZE);
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
    f_mount(NULL,"1:",1);
    return status;
}

uint32_t Sd_FatfsBoundaryCoverageTest(void)
{
    uint32_t status = MMC_SUCCESS;
    status = FatfsTestCase40();
    status = FatfsTestCase41();
    status = FatfsTestCase42();
    status = FatfsTestCase43();
    status = FatfsTestCase44();
    status = FatfsTestCase01();
    status = FatfsTestCase02();
    status = FatfsTestCase03();
    status = FatfsTestCase04();
    status = FatfsTestCase05();
    status = FatfsTestCase06();
    status = FatfsTestCase07();
    status = FatfsTestCase08();
    status = FatfsTestCase09();
    status = FatfsTestCase10();
    status = FatfsTestCase11();
    status = FatfsTestCase12();
    status = FatfsTestCase13();
    status = FatfsTestCase14();
    status = FatfsTestCase15();
    status = FatfsTestCase16();
    status = FatfsTestCase17();
    status = FatfsTestCase18();
    status = FatfsTestCase19();
    status = FatfsTestCase20();
    status = FatfsTestCase21();
    status = FatfsTestCase22();
    status = FatfsTestCase23();
    status = FatfsTestCase24();
    status = FatfsTestCase25();
    status = FatfsTestCase26();
    status = FatfsTestCase27();
    status = FatfsTestCase28();
    status = FatfsTestCase29();
    status = FatfsTestCase30();
    status = FatfsTestCase31();
    status = FatfsTestCase32();
    status = FatfsTestCase33();
    status = FatfsTestCase34();
    status = FatfsTestCase35();
    status = FatfsTestCase36();
    status = FatfsTestCase37();
    status = FatfsTestCase38();
    status = FatfsTestCase39();
    status = FatfsTestCase100();

    return status;

}

uint32_t Emmc_FatfsBoundaryCoverageTest(void)
{
    uint32_t status = MMC_SUCCESS;
    status = FatfsTestCase40();
    status = FatfsTestCase41();
    status = FatfsTestCase42();
    status = FatfsTestCase43();
    status = FatfsTestCase44();
    status = FatfsTestCase01();
    status = FatfsTestCase02();
    status = FatfsTestCase03();
    status = FatfsTestCase04();
    status = FatfsTestCase05();
    status = FatfsTestCase06();
    status = FatfsTestCase07();
    status = FatfsTestCase08();
    status = FatfsTestCase09();
    status = FatfsTestCase10();
    status = FatfsTestCase11();
    status = FatfsTestCase12();
    status = FatfsTestCase13();
    status = FatfsTestCase14();
    status = FatfsTestCase15();
    status = FatfsTestCase16();
    status = FatfsTestCase17();
    status = FatfsTestCase18();
    status = FatfsTestCase19();
    status = FatfsTestCase20();
    status = FatfsTestCase21();
    status = FatfsTestCase22();
    status = FatfsTestCase23();
    status = FatfsTestCase24();
    status = FatfsTestCase25();
    status = FatfsTestCase26();
    status = FatfsTestCase27();
    status = FatfsTestCase28();
    status = FatfsTestCase29();
    status = FatfsTestCase30();
    status = FatfsTestCase31();
    status = FatfsTestCase32();
    status = FatfsTestCase33();
    status = FatfsTestCase34();
    status = FatfsTestCase35();
    status = FatfsTestCase36();
    status = FatfsTestCase37();
    status = FatfsTestCase38();
    status = FatfsTestCase39();
    status = FatfsTestCase100();

    return status;
    
}


/*********************************************END OF FILE**********************/
