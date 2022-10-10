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

#include "driver/sd_emmc/al_mmc.h"
#include "driver/sd_emmc/al_emmc.h"
#include "driver/sd_emmc/FATFS/ff.h"

static RawEmmcParam_t RawEmmcParam;

uint32_t AlFsbl_EmmcRawInit(void)
{
	uint32_t status = MMC_SUCCESS;
	status = Csu_RawEmmcInit(&RawEmmcParam);

	return status;
}

uint32_t AlFsbl_EmmcRawCopy(uint32_t SrcAddress, PTRSIZE DestAddress, uint32_t Length)
{
	uint32_t status = 0;
	status = Csu_RawEmmcRead(SrcAddress, DestAddress, Length);

	return status;
}

uint32_t AlFsbl_EmmcRawRelease(void)
{
	return 0;
}
