/*
 * alfsbl_emmc_raw.h
 *
 *  Created on: Sept 28, 2022
 *      Author: tfcao
 */

#ifndef ALFSBL_EMMC_RAW_H_
#define ALFSBL_EMMC_RAW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"
#include "al9000_secure.h"


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
