#include <stdint.h>
#include <stdio.h>
#include "alfsbl_emmc_raw.h"
#include "alfsbl_nand.h"
#include "alfsbl_boot.h"
#include "alfsbl_err_code.h"
#include "alfsbl_qspi.h"
#include "alfsbl_sd.h"
#include "al_reg_io.h"


uint32_t AlFsbl_PrimaryBootDeviceInit(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	uint32_t BootMode;
	uint32_t BlockSizeMax;

	BootMode = AL_REG32_READ(SYSCTRL_NS_BOOT_MODE);

	printf("Boot Mode: 0x%08x\r\n", BootMode);

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
		printf("QSPI 24 bit Boot Mode\r\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_Qspi24Init;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_Qspi24Copy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_Qspi24Release;
		break;

	case ALFSBL_BOOTMODE_QSPI32:
		break;

	case ALFSBL_BOOTMODE_NAND:
		printf("NAND Boot Mode\r\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_NandInit;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_NandCopy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_NandRelease;
		break;

	case ALFSBL_BOOTMODE_EMMC:
	case ALFSBL_BOOTMODE_SD:
		printf("EMMC or SD Boot Mode\r\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_SdInit;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_SdCopy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_SdRelease;
		break;

	case ALFSBL_BOOTMODE_EMMC_RAW:
		printf("EMMC Raw Boot Mode\r\n");
		FsblInstancePtr->DeviceOps.DeviceInit    = AlFsbl_EmmcRawInit;
		FsblInstancePtr->DeviceOps.DeviceCopy    = AlFsbl_EmmcRawCopy;
		FsblInstancePtr->DeviceOps.DeviceRelease = AlFsbl_EmmcRawRelease;
		break;

	case ALFSBL_BOOTMODE_JTAG_QSPI:
		break;

	default:
		printf("Unsupported boot mode\r\n");
		Status = ALFSBL_ERROR_UNSUPPORTED_BOOT_MODE;
	}

	if(ALFSBL_SUCCESS != Status) {
		goto END;
	}

	Status = FsblInstancePtr->DeviceOps.DeviceInit(&BlockSizeMax);
	if(ALFSBL_SUCCESS != Status) {
		printf("Device init failed: %x\r\n", Status);
		Status = ALFSBL_ERROR_DEVICE_INIT_FAILED;
		goto END;
	}

	FsblInstancePtr->DeviceOps.BlockSizeMax = BlockSizeMax;

END:
	return Status;
}
