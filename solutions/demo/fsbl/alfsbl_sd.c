#include <stdio.h>

#include "al_reg_io.h"

#include "alfsbl_sd.h"
#include "alfsbl_misc.h"
#include "alfsbl_boot.h"

#include "al_mmc.h"
#include "al_sd.h"

FIL fil;
FATFS fs;

uint32_t AlFsbl_SdInit(void)
{
	FRESULT rc = FR_OK;
	char buffer[32U]={0U};
	char *boot_file = buffer;
	uint32_t multibootoffset = 0;
	uint32_t drvnum;

	drvnum = AL_REG32_READ(SYSCTRL_NS_BOOT_MODE);
	multibootoffset = AL_REG32_READ(SYSCTRL_S_MULTI_BOOT);
	printf("multi boot offset is %d\r\n", multibootoffset);

	if(drvnum == ALFSBL_SD_DRV_NUM_0){
		rc = f_mount(&fs, "0:", 1);
		printf("drv is sd\r\n");
	}
	else if(drvnum == ALFSBL_SD_DRV_NUM_1){
		rc = f_mount(&fs, "1:", 1);
		printf("drv is emmc\r\n");
	}
	if(rc != FR_OK){
		printf("drv disk error:%d\r\n", rc);
		rc = rc | ((ALFSBL_BOOTMODE_SD << 16));
		return rc;
	}

	AlFsbl_MakeSdFileName(boot_file, multibootoffset, drvnum);

	if(boot_file[0] != 0){
		rc = f_open(&fil, boot_file, FA_OPEN_EXISTING | FA_READ);
		if(rc != FR_OK){
			rc = rc | ((ALFSBL_BOOTMODE_SD << 16));
			return rc;
		}
	}
	else{
		rc = FR_NO_FILE;
	}

	if(rc != 0) {
		rc = rc | ((ALFSBL_BOOTMODE_SD << 16));
	}
	return rc;
}


uint32_t AlFsbl_SdCopy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo)
{
	FRESULT rc = FR_OK;
	uint32_t br = 0;

	rc = f_lseek(&fil, SrcAddress - IMAGE_FLASH_OFFSET);
	if(rc != FR_OK){
		rc = rc | ((ALFSBL_BOOTMODE_SD << 16));
		return rc;
	}
	rc = f_read(&fil, (uint8_t *)DestAddress, Length, &br);
	if(rc != FR_OK){
		rc = rc | ((ALFSBL_BOOTMODE_SD << 16));
		return rc;
	}

	if(rc != 0) {
		rc = rc | ((ALFSBL_BOOTMODE_SD << 16));
	}
	return rc;
}


uint32_t AlFsbl_SdRelease(void)
{
	FRESULT rc = FR_OK;

	rc = f_close(&fil);

	if(rc != FR_OK){
		rc = rc | ((ALFSBL_BOOTMODE_SD << 16));
	}
	return rc;
}
