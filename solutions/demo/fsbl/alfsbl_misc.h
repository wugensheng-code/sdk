/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ALFSBL_MISC_H_
#define ALFSBL_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define ALFSBL_SD_DRV_NUM_0     0x5U
#define ALFSBL_SD_DRV_NUM_1     0x2U

#define ALFSBL_NUM_IN_FILE_NAME 0x6U

void *AlFsbl_MemCpy(void * DestPtr, const void * SrcPtr, uint32_t Len);

void delay_us(unsigned int us);

uint32_t endian_convert(uint32_t data);

void AlFsbl_MakeSdFileName(char *FileName, uint32_t MultiBootReg, uint32_t DrvNum);

#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_MISC_H_ */
