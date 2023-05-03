/*
 * alfsbl_init.h
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#ifndef ALFSBL_INIT_H_
#define ALFSBL_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"
#include "alfsbl_err_code.h"

uint32_t AlFsbl_Initialize(AlFsblInfo *FsblInstancePtr);


#ifdef __cplusplus
}
#endif


#endif /* ALFSBL_INIT_H_ */
