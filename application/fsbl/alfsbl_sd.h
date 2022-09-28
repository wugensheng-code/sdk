/*
 * alfsbl_qspi.h
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#ifndef ALFSBL_SD_H_
#define ALFSBL_SD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"

uint32_t AlFsbl_SdInit(void);
uint32_t AlFsbl_SdCopy(uint32_t SrcAddress, PTRSIZE DestAddress, uint32_t Length);
uint32_t AlFsbl_SdRelease(void);



#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_QSPI_H_ */
