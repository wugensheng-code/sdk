/*
 * alfsbl_emmc_raw.c
 *
 *  Created on: Sept 28, 2022
 *      Author: tfcao
 */

#include <stdio.h>

#include "demosoc.h"

#include "alfsbl_sd.h"
#include "alfsbl_misc.h"
#include "alfsbl_boot.h"

#include "driver/sd_emmc/al_mmc.h"
#include "driver/sd_emmc/al_emmc.h"
#include "driver/sd_emmc/FATFS/ff.h"

static RawEmmcParam_t RawEmmcParam;

uint32_t AlFsbl_EmmcRawInit(void)
{
	uint32_t status = MMC_SUCCESS;
	status = Csu_RawEmmcInit(&RawEmmcParam);

	if(status != 0) {
		status = status | (ALFSBL_BOOTMODE_EMMC << 16);
	}
	return status;
}

uint32_t AlFsbl_EmmcRawCopy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo)
{
	uint32_t status = 0;
	status = Csu_RawEmmcRead(SrcAddress - IMAGE_FLASH_OFFSET, DestAddress, Length);

	if(status != 0) {
		status = status | (ALFSBL_BOOTMODE_EMMC << 16);
	}

	return status;
}

uint32_t AlFsbl_EmmcRawRelease(void)
{
	return 0;
}
