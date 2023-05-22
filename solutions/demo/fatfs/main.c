#include "al_reg_io.h"
#include "al_errno.h"
#include "ff.h"
#include "ffconf.h"
#include "al_sd_write.h"
#include "al_sd.h"

#define AL_FATFS_DEMO_WRITE_BUFFER_PTR  (0x10000000UL)
#define AL_FATFS_DEMO_READ_BUFFER_PTR   (0x20000000UL)
#define AL_FATFS_DEMO_WORK_BUFFER_PTR   (0x30000000UL)
#define AL_FATFS_DEMO_READ_WRITE_SIZE   (0x100000UL)    //1MB
#define AL_FATFS_DEMO_LSEEK_OFFSET      (0x1000UL)

AL_S32 AlFatfs_Demo_SdReadWrite(AL_VOID);

AL_U32 main(AL_VOID)
{
    AlFatfs_Demo_SdReadWrite();
    return 0;
}

AL_S32 AlFatfs_Demo_SdReadWrite(AL_VOID)
{
    FRESULT Ret = FR_OK;
    FATFS FileSystem;
    FIL File;
    FILINFO FileInfo;
    AL_U32 OpsNum = 0;
    MKFS_PARM MkfsParam = {
        .fmt        = FM_FAT32,
        .n_fat      = 1,            /* 1 or 2 file system, defalut 1 */
        .align      = 0,            /* set 0, fat will use ioctl to get block size for align */
        .n_root     = 0,            /* align to sector_size/size_root_dir(512/32)=16 */
        .au_size    = 0x4000,       /* max size of cluster in byte, max 64kB, buf if card capacity is to small ,max 32kB */
    };
    const char *FileName = "0:/npu_demo/input_resize.txt";

    /* destroy file system for pass f_mkfs */
    AlSd_Init();
    Csu_RawSdSetMode(MMC_MODE_FREQ, MMC_FREQ_10M);
    AlSd_WriteSingleBlock((uint8_t *)AL_FATFS_DEMO_WRITE_BUFFER_PTR, 0, 512);

    AL_LOG(AL_ERR_LEVEL_DEBUG, "Sd read/write test!\r\n");
    /* Mount immediately */
    Ret = f_mount(&FileSystem, "0:", 1);
    if (Ret == FR_NO_FILESYSTEM) {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Sd no file system, Wait for sd mkfs...\r\n");
        Ret = f_mkfs("0:", &MkfsParam, (void *)AL_FATFS_DEMO_WORK_BUFFER_PTR, FF_MAX_SS);
        if (Ret == FR_OK) {
            Ret = f_mount(&FileSystem, "0:", 1);
            if (Ret != FR_OK) {
                AL_LOG(AL_ERR_LEVEL_ERROR, "Mount error!\r\n");
                goto ERROR_MOUNT;
            }
        } else {
            AL_LOG(AL_ERR_LEVEL_ERROR, "Mkfs Error!\r\n");
            goto ERROR_MKFS;
        }
    } else if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Mount Error!\r\n");
        goto ERROR_MOUNT;
    } else {
        AL_LOG(AL_ERR_LEVEL_DEBUG, "Mount sd card success!\r\n");
    }

    Ret = f_mkdir("0:/npu_demo");
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Create directory error!\r\n");
        goto ERROR_MKDIR;
    }
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Create directory success!\r\n");

    Ret = f_open(&File, FileName, FA_CREATE_ALWAYS | FA_WRITE);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Open/create file error!\r\n");
        goto ERROR_OPEN;
    }
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Open/create file success!\r\n");

    Ret = f_write(&File, (const void *)AL_FATFS_DEMO_WRITE_BUFFER_PTR, AL_FATFS_DEMO_READ_WRITE_SIZE, &OpsNum);
    if (Ret != AL_OK || OpsNum != AL_FATFS_DEMO_READ_WRITE_SIZE) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Write file error!\r\n");
        goto ERROR_WRITE;
    }
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Write data in byte %d in file %s success!\r\n", OpsNum, FileName);

    Ret = f_close(&File);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Close after write file error!\r\n");
        goto ERROR_CLOSE;
    }
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Close after write file success!\r\n");

    memset(&File, 0, sizeof(FIL));
    OpsNum = 0;
    Ret = f_open(&File, FileName, FA_OPEN_EXISTING | FA_READ);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Open existing file error!\r\n");
        goto ERROR_OPEN;
    }
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Open existing file success!\r\n");

    Ret = f_lseek(&File, AL_FATFS_DEMO_LSEEK_OFFSET);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Lseek file error!\r\n");
        goto ERROR_LSEEK;
    }
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Lseek size in byte %d in file %s success!\r\n", AL_FATFS_DEMO_LSEEK_OFFSET, FileName);

    Ret = f_read(&File, (const void *)AL_FATFS_DEMO_READ_BUFFER_PTR, AL_FATFS_DEMO_READ_WRITE_SIZE - AL_FATFS_DEMO_LSEEK_OFFSET, &OpsNum);
    if (Ret != AL_OK || OpsNum != AL_FATFS_DEMO_READ_WRITE_SIZE - AL_FATFS_DEMO_LSEEK_OFFSET) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Read file error!\r\n");
        goto ERROR_READ;
    }
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Read data in byte %d in file %s success!\r\n", OpsNum, FileName);

    Ret = f_close(&File);
    if (Ret != AL_OK) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Close after write file error!\r\n");
        goto ERROR_CLOSE;
    }
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Close after read file success!\r\n");

    Ret = memcmp(AL_FATFS_DEMO_WRITE_BUFFER_PTR + AL_FATFS_DEMO_LSEEK_OFFSET, AL_FATFS_DEMO_READ_BUFFER_PTR, AL_FATFS_DEMO_READ_WRITE_SIZE - AL_FATFS_DEMO_LSEEK_OFFSET);
    if (Ret != 0) {
        AL_LOG(AL_ERR_LEVEL_ERROR, "Read/write data check error!\r\n");
        goto ERROR_DATA;
    }
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Read/write data check success!\r\n");

    f_mount(NULL, "0:", 1);

    goto SUCCESS;


ERROR_WRITE:
ERROR_READ:
ERROR_LSEEK:
    f_close(&File);
ERROR_MKDIR:
ERROR_OPEN:
ERROR_CLOSE:
ERROR_DATA:
    f_mount(NULL, "0:", 1);
ERROR_MKFS:
ERROR_MOUNT:
SUCCESS:
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Ret is 0x%x\r\n", Ret);
    AL_LOG(AL_ERR_LEVEL_DEBUG, "Sd read/write test done!\r\n", Ret);
    return Ret;
}