/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ALFSBL_MAIN_H_
#define ALFSBL_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

#include "alfsbl_err_code.h"
#include "alfsbl_data.h"
#include "alfsbl_init.h"


/// FSBL Stage Definition
#define ALFSBL_STAGE1           (0x1U)
#define ALFSBL_STAGE2           (0x2U)
#define ALFSBL_STAGE3           (0x3U)
#define ALFSBL_STAGE4           (0x4U)
#define ALFSBL_STAGE_ERR        (0x5U)
#define ALFSBL_STAGE_DFT        (0x6U)





#ifdef __cplusplus
}
#endif

#endif  /* ALFSBL_MAIN_H_ */
