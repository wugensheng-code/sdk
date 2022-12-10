/*
 * alfsbl_err_lockdown.h
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#ifndef ALFSBL_ERR_LOCKDOWN_H_
#define ALFSBL_ERR_LOCKDOWN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "alfsbl_data.h"

void AlFsbl_ErrorLockDown(AlFsblInfo *FsblInstancePtr, uint32_t ErrorStatus);

#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_ERR_LOCKDOWN_H_ */
