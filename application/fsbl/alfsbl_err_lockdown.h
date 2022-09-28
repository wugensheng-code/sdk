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

void AlFsbl_ErrorLockDown(uint32_t ErrorStatus);

#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_ERR_LOCKDOWN_H_ */
