/*
 * alfsbl_handoff.h
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */

#ifndef ALFSBL_HANDOFF_H_
#define ALFSBL_HANDOFF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_err_code.h"
#include "alfsbl_data.h"
#include "alfsbl_boot.h"

void AlFsbl_HandoffExit(uint64_t HandoffAddress);

uint32_t AlFsbl_Handoff(const AlFsblInfo *FsblInstancePtr);


#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_HANDOFF_H_ */
