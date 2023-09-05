/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef ALFSBL_PARTITION_LOAD_H_
#define ALFSBL_PARTITION_LOAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "alfsbl_data.h"
#include "alfsbl_hw.h"
#include "alfsbl_err_code.h"
#include "alfsbl_secure.h"

#define INIT_DONE_WAIT_SECS (90)


uint32_t AlFsbl_PartitionLoad(AlFsblInfo *FsblInstancePtr, uint32_t PartitionIdx);


#ifdef __cplusplus
}
#endif


#endif /* ALFSBL_PARTITION_LOAD_H_ */
