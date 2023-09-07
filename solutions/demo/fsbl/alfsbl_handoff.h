/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ALFSBL_HANDOFF_H_
#define __ALFSBL_HANDOFF_H_

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

#endif /* AL_ALFSBL_HANDOFF_H */
