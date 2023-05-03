/*
 * alfsbl_qspi.h
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#ifndef ALFSBL_QSPI_H_
#define ALFSBL_QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <alfsbl_secure.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"



uint32_t AlFsbl_Qspi24Init(uint32_t *pBlockSizeMax);
uint32_t AlFsbl_Qspi24Copy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo);



uint32_t AlFsbl_Qspi24Release(void);

uint32_t AlFsbl_Qspi32Init(uint32_t *pBlockSizeMax);
uint32_t AlFsbl_Qspi32Copy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length, SecureInfo *pSecureInfo);
uint32_t AlFsbl_Qspi32Release(void);



#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_QSPI_H_ */
