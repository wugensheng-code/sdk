/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_ALFSBL_NAND_H_
#define __AL_ALFSBL_NAND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"



uint32_t AlFsbl_NandInit(void);
uint32_t AlFsbl_NandCopy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo);
uint32_t AlFsbl_NandRelease(void);


#ifdef __cplusplus
}
#endif

#endif 
