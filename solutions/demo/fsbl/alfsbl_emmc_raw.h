/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ALFSBL_EMMC_RAW_H_
#define ALFSBL_EMMC_RAW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"
#include <alfsbl_secure.h>


typedef enum{
    EMMC_FREQ_400K,
    EMMC_FREQ_10M,
    EMMC_FREQ_MAX
}EMMC_FREQ;

typedef struct{
    uint32_t EmmcId;     //emmc device ID
    uint32_t EmmcSize;   //emmc size in kBytes
    EMMC_FREQ EmmcFreq;  //emmc freq set
}RawEmmcParam_t;

uint32_t AlFsbl_EmmcRawInit(void);
uint32_t AlFsbl_EmmcRawCopy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo);
uint32_t AlFsbl_EmmcRawRelease(void);

#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_EMMC_RAW_H_ */
