/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ALFSBL_ERR_LOCKDOWN_H_
#define ALFSBL_ERR_LOCKDOWN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "alfsbl_data.h"

#define RPU_LOOP_ADDR           (0x6101fffc)
#define RPU_LOOP_INSTRUCTION    (0xa001a001)
#define APU_LOOP_ADDR           (0x6101ffec)
#define APU_LOOP_INSTRUCTION    (0x14000000)

void AlFsbl_ErrorLockDown(AlFsblInfo *FsblInstancePtr, uint32_t ErrorStatus);

#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_ERR_LOCKDOWN_H_ */
