/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_core.h"
#include "alfsbl_misc.h"
#include "al_utils_def.h"
#include "al_systimer.h"

void *AlFsbl_MemCpy(void *DestPtr, const void *SrcPtr, uint32_t Len)
{
	uint8_t *Dst = DestPtr;
	const uint8_t *Src = SrcPtr;

	while(Len != 0U) {
		*Dst = *Src;
		Dst++;
		Src++;
		Len--;
	}
	return DestPtr;
}



uint32_t endian_convert(uint32_t data)
{
	uint32_t data_c = 0;
	data_c += (data & 0x000000ff) << 24;
	data_c += (data & 0x0000ff00) <<  8;
	data_c += (data & 0x00ff0000) >>  8;
	data_c += (data & 0xff000000) >> 24;
	return data_c;
}



void delay_us(unsigned int us)
{
	uint32_t delay_cnt;
	uint32_t start;
	volatile uint32_t end;
	uint32_t t_consumed;

	delay_cnt = us * 10;

	start = *((volatile uint32_t *)(SYSTIMER__BASE_ADDR));
	do {
		end = *((volatile uint32_t *)(SYSTIMER__BASE_ADDR));
		t_consumed = end - start;
	}while(t_consumed < delay_cnt);


	return;
}

#define NUM2STR_RANGE	2

/**
 * @brief num to str
 *
 * @param str
 * @param num range 0~999
 */
static void AlFsbl_Num2Str(char *Str, uint32_t Num, uint32_t *StrLen)
{
	uint32_t i = 1, j = 0;
	uint32_t k = 0, res = 0;
	uint32_t strlen = 0;

	while (i <= NUM2STR_RANGE) {
		k = 1;
		j = 0;
		while (j < NUM2STR_RANGE - i) {
			k *= 10;
			j++;
		}
		res = Num / k % 10;
		if (res != 0) {
			Str[strlen] = res + '0';
			strlen++;
		}
		i++;
	}
	*StrLen = strlen;
}

const char disk1name[] = "1:/";
const char disk3name[] = "3:/";
const char bootname[] = "BOOT";
const char filesuffix[] = ".bin";

void AlFsbl_MakeSdFileName(char *FileName, uint32_t MultiBootReg, uint32_t DrvNum)
{
	uint32_t filenamelen = 0;
	char strnum[5] = {0};
	uint32_t num2strlen = 0;
	if (0x0 == MultiBootReg) {
		if (DrvNum == ALFSBL_SD_DRV_NUM_0) {
			AlFsbl_MemCpy(FileName, "BOOT.bin", sizeof("BOOT.bin"));
		} else if (DrvNum == ALFSBL_SD_DRV_NUM_1) {
			AlFsbl_MemCpy(FileName, "1:/BOOT.bin", sizeof("1:/BOOT.bin"));
		} else {
			AlFsbl_MemCpy(FileName, "3:/BOOT.bin", sizeof("3:/BOOT.bin"));
		}
	} else {
		AlFsbl_Num2Str(strnum, MultiBootReg, &num2strlen);

		if (DrvNum == ALFSBL_SD_DRV_NUM_1) {
			AlFsbl_MemCpy(FileName, disk1name, sizeof(disk1name)-1);
			filenamelen = sizeof(disk1name)-1;
		} else if (DrvNum == ALFSBL_SD_DRV_NUM_2) {
			AlFsbl_MemCpy(FileName, disk3name, sizeof(disk3name)-1);
			filenamelen = sizeof(disk3name)-1;
		}

		AlFsbl_MemCpy(&FileName[filenamelen], bootname, sizeof(bootname)-1);
		filenamelen += sizeof(bootname)-1;

		AlFsbl_MemCpy(&FileName[filenamelen], strnum, num2strlen);
		filenamelen += num2strlen;

		FileName[filenamelen++] = 0x2e;
		FileName[filenamelen++] = 'b';
		FileName[filenamelen++] = 'i';
		FileName[filenamelen++] = 'n';

		// AlFsbl_MemCpy(&FileName[filenamelen], filesuffix, sizeof(filesuffix)-1);
		// filenamelen += sizeof(filesuffix)-1;
	}

	AL_LOG(AL_LOG_LEVEL_INFO, "file name is %s\r\n", FileName);
}


void print_time_stamp(void)
{
	uint64_t TimerFreq = 0;
	volatile uint64_t CurrTimer;
	uint64_t CurrTimeMs;
	TimerFreq = AlSys_GetTimerFreq();
	CurrTimer = AlSys_GetTimerTickCount();
	CurrTimeMs = CurrTimer * 1000 / TimerFreq;
	printf("Current Time: %lu ms\r\n", CurrTimeMs);
}