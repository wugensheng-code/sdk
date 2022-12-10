/*
 * alfsbl_nand.h
 *
 *  Created on: 10 10, 2022
 *      Author: hj
 */

#ifndef ALFSBL_NAND_H_
#define ALFSBL_NAND_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"



uint32_t AlFsbl_NandInit(void);
uint32_t AlFsbl_NandCopy(uint64_t SrcAddress, PTRSIZE DestAddress, uint32_t Length);
uint32_t AlFsbl_NandRelease(void);


#ifdef __cplusplus
}
#endif

#endif 
