/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "alfsbl_secure.h"
#include "alfsbl_main.h"
#include "alfsbl_image_header.h"
#include "alfsbl_partition_load.h"
#include "alfsbl_handoff.h"
#include "alfsbl_err_lockdown.h"
#include "alfsbl_misc.h"
#include "alfsbl_hw.h"
#include "al_utils_def.h"
#include "soc_plat.h"
#include "al_reg_io.h"

/********************* global variables *********************/
uint8_t  ReadBuffer[READ_BUFFER_SIZE] __attribute__((aligned(64)));
uint8_t  AuthBuffer[ALFSBL_AUTH_BUFFER_SIZE] __attribute__((aligned(64))) = {0};
uint8_t  HashBuffer[32]__attribute__((aligned(64)));

AlFsblInfo FsblInstance __attribute__((aligned(64))) = {0x00U, 0, 0, 0};
SecureInfo FsblSecInfo = {0};


int main(void)
{
	uint32_t FsblStage    = ALFSBL_STAGE1;
	uint32_t FsblStatus   = ALFSBL_SUCCESS;
	uint32_t PartitionIdx = 0;

	AL_LOG(AL_LOG_LEVEL_INFO, "fsbl main start: \r\n");

	Soc_PlatInit();

	while (FsblStage <= ALFSBL_STAGE_DFT) {
		switch (FsblStage) {
		case ALFSBL_STAGE1:
			AL_LOG(AL_LOG_LEVEL_INFO, "==================== In Stage 1 ====================\r\n");

			/// system initialize
			FsblStatus = AlFsbl_Initialize(&FsblInstance);
			if(FsblStatus == ALFSBL_SUCCESS) {
				FsblStage = ALFSBL_STAGE2;
			}
			else {
				FsblStatus += ALFSBL_ERROR_STAGE_1;
				FsblStage = ALFSBL_STAGE_ERR;
			}
			break;

		case ALFSBL_STAGE2:
			AL_LOG(AL_LOG_LEVEL_INFO, "==================== In Stage 2 ====================\r\n");

			/// primary boot device initialize
			/// DeviceOps function pointer initialize
			/// read and validate image header
			/// read partition headers

			FsblStatus = AlFsbl_BootDevInitAndHdrValidate(&FsblInstance);

			if(FsblStatus == ALFSBL_STATUS_JTAG) {
				FsblStage = ALFSBL_STAGE4;
			}
			else if(FsblStatus == ALFSBL_SUCCESS) {
				PartitionIdx = 1;
				if(PartitionIdx < FsblInstance.ImageHeader.BootHeader.PartitionNum) {
                    AL_LOG(AL_LOG_LEVEL_INFO, "PartitionNum: %d\r\n", FsblInstance.ImageHeader.BootHeader.PartitionNum);
					FsblStage = ALFSBL_STAGE3;
				}
				else if(PartitionIdx == FsblInstance.ImageHeader.BootHeader.PartitionNum) {
					AL_LOG(AL_LOG_LEVEL_INFO, "Only one partition exists in boot image, which is fsbl itself\r\n");
					AL_LOG(AL_LOG_LEVEL_INFO, "for test only, if this message reported, test pass and finished\r\n");
				}
				else if(PartitionIdx > FsblInstance.ImageHeader.BootHeader.PartitionNum) {
					AL_LOG(AL_LOG_LEVEL_ERROR, "Boot image format invalid or Image Header data incorrect, need to check\r\n");
					FsblStatus = ALFSBL_ERROR_INVALID_PARTITION_NUM;
					FsblStatus += ALFSBL_ERROR_STAGE_2;
					FsblStage = ALFSBL_STAGE_ERR;
				}

			}
			else {
				FsblStatus += ALFSBL_ERROR_STAGE_2;
				FsblStage = ALFSBL_STAGE_ERR;
			}
			break;

		case ALFSBL_STAGE3:
			AL_LOG(AL_LOG_LEVEL_INFO, "==================== In Stage 3 ====================\r\n");

			if(FsblInstance.ImageHeader.BootHeader.PartitionNum <= 1) {
				AL_LOG(AL_LOG_LEVEL_INFO, "Info: no partition to load...\r\n");
				FsblStage = ALFSBL_STAGE4;
				break;
			}
			/// load and validate partitions
			FsblStatus = AlFsbl_PartitionLoad(&FsblInstance, PartitionIdx);
			PartitionIdx++;

			if(FsblStatus != ALFSBL_SUCCESS) {
				FsblStatus += ALFSBL_ERROR_STAGE_3;
				FsblStage = ALFSBL_STAGE_ERR;
			}
			else if(PartitionIdx == FsblInstance.ImageHeader.BootHeader.PartitionNum) {
				FsblStage = ALFSBL_STAGE4;
			}
			else {
				/**do nothing**/
			}
			break;

		case ALFSBL_STAGE4:
			AL_LOG(AL_LOG_LEVEL_INFO, "==================== In Stage 4 ====================\r\n");

			/// handoff to the applications, let the dest cpu run the ps partition.
			FsblStatus = AlFsbl_Handoff(&FsblInstance);

			if(FsblStatus != ALFSBL_SUCCESS) {
				FsblStatus += ALFSBL_ERROR_STAGE_4;
				FsblStage = ALFSBL_STAGE_ERR;
			}
			else {
				/// we should never be here
				FsblStage = ALFSBL_STAGE_DFT;
			}
			break;

		case ALFSBL_STAGE_ERR:
			AL_LOG(AL_LOG_LEVEL_ERROR, "=================== In Stage Err ===================\r\n");
			AL_LOG(AL_LOG_LEVEL_ERROR, "fsbl error: %x\r\n", FsblStatus);
			AlFsbl_ErrorLockDown(&FsblInstance, FsblStatus);

			/// we should never be here
			FsblStage = ALFSBL_STAGE_DFT;
			break;

		case ALFSBL_STAGE_DFT:
		default:
			AL_LOG(AL_LOG_LEVEL_INFO, "================= In Stage Default =================\r\n");
			AL_LOG(AL_LOG_LEVEL_INFO, "In default stage: we should never be here\r\n");
			break;
		}


		if(FsblStage == ALFSBL_STAGE_DFT) {
			break;
		}
	}


	return 0;
}




