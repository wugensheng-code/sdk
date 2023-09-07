/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_ALFSBL_BOOT_H_
#define __AL_ALFSBL_BOOT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "alfsbl_data.h"
#include "alfsbl_hw.h"
/// Boot Modes Definition
#define ALFSBL_BOOTMODE_JTAG      (0x0U)
#define ALFSBL_BOOTMODE_QSPI24    (0x1U)
#define ALFSBL_BOOTMODE_EMMC      (0x2U)
#define ALFSBL_BOOTMODE_QSPI32    (0x3U)
#define ALFSBL_BOOTMODE_NAND      (0x4U)
#define ALFSBL_BOOTMODE_SD        (0x5U)
#define ALFSBL_BOOTMODE_EMMC_RAW  (0x6U)
#define ALFSBL_BOOTMODE_JTAG_QSPI (0x7U)

uint32_t AlFsbl_PrimaryBootDeviceInit(AlFsblInfo *FsblInstancePtr);


#ifdef __cplusplus
}
#endif

#endif /* AL_ALFSBL_BOOT_H */
