/*
 * alfsbl_sd.c
 *
 *  Created on: May 18, 2022
 *      Author: tfcao
 */

#include <stdio.h>

#include "demosoc.h"

#include "alfsbl_sd.h"
#include "alfsbl_misc.h"

#include "driver/sd_emmc/al_mmc.h"
#include "driver/sd_emmc/al_sd.h"
#include "driver/sd_emmc/FATFS/ff.h"

const char *SdImageFiles[1] = {"0:/image.bin"};

static FIL fil;
static FATFS fs;

uint32_t AlFsbl_SdInit(void)
{
	FRESULT rc = FR_OK;
	rc = f_mount(&fs, "0:/", 1);

	return rc;
}


uint32_t AlFsbl_SdCopy(uint32_t SrcAddress, PTRSIZE DestAddress, uint32_t Length)
{
	FRESULT rc = FR_OK;
	uint32_t br = 0;

	rc = f_open(&fil, SdImageFiles, FA_READ);
	if(rc != FR_OK){
		return rc;
	}

	rc = f_lseek(&fil, SrcAddress - IMAGE_FLASH_OFFSET);
	if(rc != FR_OK){
		return rc;
	}
	
	rc = f_read(&fil, (void *)DestAddress, Length, &br);
	if(rc != FR_OK){
		return rc;
	}

	rc = f_close(&fil);
	if(rc != FR_OK){
		return rc;
	}

	return rc;
}


uint32_t AlFsbl_SdRelease(void)
{
	return 0;
}
