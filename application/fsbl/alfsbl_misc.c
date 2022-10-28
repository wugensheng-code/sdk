/*
 * alfsbl_misc.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#include "demosoc.h"
#include "alfsbl_misc.h"

void *AlFsbl_MemCpy(void *DestPtr, const void *SrcPtr, uint32_t Len)
{
	uint32_t *Dst = DestPtr;
	const uint32_t *Src = SrcPtr;

	uint32_t WordNum = Len / 4;
	uint32_t ByteNum = Len % 4;

	uint8_t *Dst2 = (uint8_t *)(Dst + WordNum);
	const uint8_t *Src2 = (uint8_t *)(Src + WordNum);

	while(WordNum != 0U) {
		*Dst = *Src;
		Dst++;
		Src++;
		WordNum--;
	}
	while(ByteNum != 0U) {
		*Dst2 = *Src2;
		Dst2++;
		Src2++;
		ByteNum--;
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

	start = *((volatile uint32_t *)(__SYSTIMER_BASEADDR));
	do {
		end = *((volatile uint32_t *)(__SYSTIMER_BASEADDR));
		t_consumed = end - start;
	}while(t_consumed < delay_cnt);


	return;
}

void AlFsbl_MakeSdFileName(char *FileName, uint32_t MultiBootReg, uint32_t DrvNum)
{
	uint32_t filenamelen = 0;
	if(0x0 == MultiBootReg) {
		if(DrvNum == ALFSBL_SD_DRV_NUM_0){
			AlFsbl_MemCpy(FileName, "BOOT.bin", sizeof("BOOT.bin"));
		}else{
			AlFsbl_MemCpy(FileName, "1:/BOOT.bin", sizeof("1:/BOOT.bin"));
		}
	}else {
		if(DrvNum == ALFSBL_SD_DRV_NUM_0){
			filenamelen = sizeof("BOOT0.bin");
			AlFsbl_MemCpy(FileName, "BOOT0.bin", filenamelen);
		}else{
			filenamelen = sizeof("1:/BOOT0.bin");
			AlFsbl_MemCpy(FileName, "1:/BOOT0.bin", filenamelen);
		}

		FileName[filenamelen - ALFSBL_NUM_IN_FILE_NAME] = '0' + DrvNum;
	}

	printf("file name is %s\r\n", FileName);
}
