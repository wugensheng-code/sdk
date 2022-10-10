/*
 * alfsbl_emmc.c
 *
 *  Created on: Sept 28, 2022
 *      Author: tfcao
 */

#include <stdio.h>

#include "demosoc.h"

#include "alfsbl_emmc.h"
#include "alfsbl_sd.h"
#include "alfsbl_misc.h"

#include "driver/sd_emmc/al_mmc.h"
#include "driver/sd_emmc/al_emmc.h"
#include "driver/sd_emmc/FATFS/ff.h"

const char *EmmcImageFiles[1] = {"1:/BOOT.bin"};

uint32_t AlFsbl_EmmcInit(void)
{
	FRESULT rc = FR_OK;
	rc = f_mount(&fs, "1:", 1);

	return rc;
}


uint32_t AlFsbl_EmmcCopy(uint32_t SrcAddress, PTRSIZE DestAddress, uint32_t Length)
{
	FRESULT rc = FR_OK;
	uint32_t br = 0;

	rc = f_open(&fil, "1:/BOOT.bin", FA_OPEN_EXISTING | FA_READ);
	if(rc == FR_OK){
		//printf("sd read f_open %d\r\n", rc);
		rc = f_lseek(&fil, SrcAddress - IMAGE_FLASH_OFFSET);
		if(rc != FR_OK){
			return rc;
		}
		rc = f_read(&fil, (uint8_t *)DestAddress, Length, &br);
		if(rc != FR_OK){
			return rc;
		}
	}else{
		return rc;
	}

	rc = f_close(&fil);
	if(rc != FR_OK){
		return rc;
	}

	return rc;
}


uint32_t AlFsbl_EmmcRelease(void)
{
	return 0;
}
