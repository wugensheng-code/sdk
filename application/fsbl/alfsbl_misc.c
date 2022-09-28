/*
 * alfsbl_misc.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */


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
