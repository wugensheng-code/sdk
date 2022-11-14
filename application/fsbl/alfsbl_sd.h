/*
 * alfsbl_sd.h
 *
 *  Created on: Sept 28, 2022
 *      Author: tfcao
 */

#ifndef ALFSBL_SD_H_
#define ALFSBL_SD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"
#include "driver/sd_emmc/FATFS/ff.h"

extern FIL fil;
extern FATFS fs;

uint32_t AlFsbl_SdInit(void);
uint32_t AlFsbl_SdCopy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo);
uint32_t AlFsbl_SdRelease(void);



#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_SD_H_ */
