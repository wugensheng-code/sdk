/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <stdio.h>
#include "alfsbl_emmc_raw.h"
#include "alfsbl_nand.h"
#include "alfsbl_boot.h"
#include "alfsbl_err_code.h"
#include "alfsbl_qspi.h"
#include "alfsbl_sd.h"
#include "al_reg_io.h"
#include "al_utils_def.h"

uint32_t AlFsbl_PrimaryBootDeviceInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	uint32_t BootMode;

	BootMode = AL_REG32_READ(SYSCTRL_NS_BOOT_MODE);

	BootMode = 1;

	AL_LOG(AL_LOG_LEVEL_INFO, "Boot Mode: 0x%08x\r\n", BootMode);

	FsblInstancePtr->PrimaryBootDevice = BootMode;

	if(BootMode != ALFSBL_BOOTMODE_JTAG) {
//		Status = AlFsbl_InitWdt(); /// todo
		if(ALFSBL_SUCCESS != Status) {
			goto END;
		}
	}

	switch(BootMode) {
	case ALFSBL_BOOTMODE_JTAG:
		Status = ALFSBL_STATUS_JTAG;
		break;

	case ALFSBL_BOOTMODE_QSPI24:
		AL_LOG(AL_LOG_LEVEL_INFO, "QSPI 24 bit Boot Mode\r\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_Qspi24Init;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_Qspi24Copy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_Qspi24Release;
		break;

	case ALFSBL_BOOTMODE_QSPI32:
		AL_LOG(AL_LOG_LEVEL_INFO, "QSPI 32 bit Boot Mode\r\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_Qspi32Init;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_Qspi32Copy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_Qspi24Release;
		break;

	case ALFSBL_BOOTMODE_NAND:
		AL_LOG(AL_LOG_LEVEL_INFO, "NAND Boot Mode\r\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_NandInit;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_NandCopy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_NandRelease;
		break;

	case ALFSBL_BOOTMODE_EMMC:
	case ALFSBL_BOOTMODE_SD:
		AL_LOG(AL_LOG_LEVEL_INFO, "EMMC or SD Boot Mode\r\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_SdInit;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_SdCopy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_SdRelease;
		break;

	case ALFSBL_BOOTMODE_EMMC_RAW:
		AL_LOG(AL_LOG_LEVEL_INFO, "EMMC Raw Boot Mode\r\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_EmmcRawInit;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_EmmcRawCopy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_EmmcRawRelease;
		break;

	case ALFSBL_BOOTMODE_JTAG_QSPI:
		break;

	default:
		AL_LOG(AL_LOG_LEVEL_ERROR, "Unsupported boot mode\r\n");
		Status = ALFSBL_ERROR_UNSUPPORTED_BOOT_MODE;
	}

	if(ALFSBL_SUCCESS != Status) {
		goto END;
	}

	Status = FsblInstancePtr->DeviceOps.DeviceInit();
	if(ALFSBL_SUCCESS != Status) {
		AL_LOG(AL_LOG_LEVEL_ERROR, "Device init failed: %x\r\n", Status);
		Status = ALFSBL_ERROR_DEVICE_INIT_FAILED;
		goto END;
	}


END:
	return Status;
}
