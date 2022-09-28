/*
 * alfsbl_boot.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */
#include <stdint.h>
#include <stdio.h>

#include "alfsbl_boot.h"
#include "alfsbl_err_code.h"
#include "alfsbl_qspi.h"
#include "alfsbl_sd.h"

uint32_t AlFsbl_PrimaryBootDeviceInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	uint32_t BootMode;

	BootMode = REG32(SYSCTRL_NS_BOOT_MODE);

	printf("Boot Mode: 0x%08x\n", BootMode);

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
		printf("QSPI 24 bit Boot Mode\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_Qspi24Init;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_Qspi24Copy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_Qspi24Release;
		break;

	case ALFSBL_BOOTMODE_EMMC:
		break;

	case ALFSBL_BOOTMODE_QSPI32:
		break;

	case ALFSBL_BOOTMODE_NAND:
		break;

	case ALFSBL_BOOTMODE_SD:
		printf("SD Boot Mode\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_SdInit;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_SdCopy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_SdRelease;
		break;

	case ALFSBL_BOOTMODE_EMMC_RAW:
		break;

	case ALFSBL_BOOTMODE_JTAG_QSPI:
		break;

	default:
		printf("unsupported boot mode\n");
		Status = ALFSBL_ERROR_UNSUPPORTED_BOOT_MODE;
	}

	if(ALFSBL_SUCCESS != Status) {
		goto END;
	}

	Status = FsblInstancePtr->DeviceOps.DeviceInit();
	if(ALFSBL_SUCCESS != Status) {
		printf("Device init failed: %x\n", Status);
		Status = ALFSBL_ERROR_DEVICE_INIT_FAILED;
		goto END;
	}

END:
	return Status;
}
