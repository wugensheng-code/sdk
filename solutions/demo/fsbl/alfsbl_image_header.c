/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "alfsbl_secure.h"
#include <stdio.h>
#include "alfsbl_image_header.h"
#include "al_reg_io.h"
#include "al_utils_def.h"

extern uint8_t  AuthBuffer[ALFSBL_AUTH_BUFFER_SIZE]__attribute__((aligned(64)));
extern uint8_t  HashBuffer[32]__attribute__((aligned(64)));

uint32_t AlFsbl_BootDevInitAndHdrValidate(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;

	AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

	/// primary boot device init
	Status = AlFsbl_PrimaryBootDeviceInit(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// initialize security irq (RPU2CSU_ACK) as non-vector interrupt
	SecureIrqInit();

	/// read and validate boot header
	Status = AlFsbl_ValidateImageHeader(FsblInstancePtr);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

END:
	return Status;
}



uint32_t AlFsbl_ValidateImageHeader(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;
	PTRSIZE  ImageOffsetAddress;
	uint32_t BootHdrAttrb;
	uint32_t PartitionNum;
	uint32_t PartitionHeaderOffset;
	uint32_t EfuseCtrl;
	uint32_t MultiBoot;
//	uint32_t *ReadBuffer32 = (uint32_t *)(ReadBuffer);

	/// get multi boot value
	MultiBoot = AL_REG32_READ(SYSCTRL_S_MULTI_BOOT);

	/// get image offset address based on multi boot reg value
	if ((FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_EMMC) || \
			(FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_SD)) {
		FsblInstancePtr->ImageOffsetAddress = 0;
	} else {
		FsblInstancePtr->ImageOffsetAddress = MultiBoot * QSPI_FLASH_SEARCH_STEP;
	}

	ImageOffsetAddress = FsblInstancePtr->ImageOffsetAddress;
	AL_LOG(AL_LOG_LEVEL_INFO, "FsblInstancePtr->ImageOffsetAddress: 0x%lx\r\n", ImageOffsetAddress);

	/// copy boot header to internal memory
	Status = FsblInstancePtr->DeviceOps.DeviceCopy(
			     ImageOffsetAddress,
				 (PTRSIZE)(&(FsblInstancePtr->ImageHeader.BootHeader)),
				 ALIH_BH_SIZE,
				 NULL);

	if (ALFSBL_SUCCESS != Status) {
		AL_LOG(AL_LOG_LEVEL_ERROR, "boot header copy failed...\r\n");
		goto END;
	}
	AL_LOG(AL_LOG_LEVEL_INFO, "boot header copy finished...\r\n");

	Status = AlFsbl_ChecksumCheck(
			(uint8_t *)(&(FsblInstancePtr->ImageHeader.BootHeader.QspiWidthSel)),
			60,
			FsblInstancePtr->ImageHeader.BootHeader.BhChecksum);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// read partition headers
	PartitionNum = FsblInstancePtr->ImageHeader.BootHeader.PartitionNum;
	PartitionHeaderOffset = FsblInstancePtr->ImageHeader.BootHeader.FirstPartiHdrOffset;
	Status = FsblInstancePtr->DeviceOps.DeviceCopy(
			     PartitionHeaderOffset + ImageOffsetAddress,
				 (PTRSIZE)(&(FsblInstancePtr->ImageHeader.PartitionHeader[0])),
				 ALIH_PH_SIZE * PartitionNum,
				 NULL);
	if (ALFSBL_SUCCESS != Status) {
		AL_LOG(AL_LOG_LEVEL_ERROR, "partition headers copy failed...\r\n");
		goto END;
	}
	AL_LOG(AL_LOG_LEVEL_INFO, "partition headers copy finished...\r\n");

	/// authenticate image header
	BootHdrAttrb = FsblInstancePtr->ImageHeader.BootHeader.BhAttr;
	EfuseCtrl = AL_REG32_READ(EFUSE_SEC_CTRL);

	/// if authentication enabled
	if(((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) != 0) || ((BootHdrAttrb & ALIH_BH_ATTRB_HD_AC_SEL_MASK) != 0)) {
		AL_LOG(AL_LOG_LEVEL_INFO, "image header authentication enabled...\r\n");
		Status = AlFsbl_ImgHdrAuth(FsblInstancePtr, EfuseCtrl);
		if(Status != ALFSBL_SUCCESS) {
			AL_LOG(AL_LOG_LEVEL_ERROR, "image header authentication failed...\r\n");
			goto END;
		}
		AL_LOG(AL_LOG_LEVEL_INFO, "image header authentication passed...\r\n");
	}
	else {
		AL_LOG(AL_LOG_LEVEL_INFO, "image header authentication not enabled....\r\n");
	}

	AL_LOG(AL_LOG_LEVEL_INFO, "qspi width sel        : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.QspiWidthSel);
	AL_LOG(AL_LOG_LEVEL_INFO, "image id              : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.ImageId);
	AL_LOG(AL_LOG_LEVEL_INFO, "enc status            : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.EncStatus);
	AL_LOG(AL_LOG_LEVEL_INFO, "bh attribute          : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.BhAttr);
	AL_LOG(AL_LOG_LEVEL_INFO, "bh ac offset          : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.BhAcOffset);
	AL_LOG(AL_LOG_LEVEL_INFO, "first parti hdr offset: 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.FirstPartiHdrOffset);
	AL_LOG(AL_LOG_LEVEL_INFO, "partition num         : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.PartitionNum);
	AL_LOG(AL_LOG_LEVEL_INFO, "bh checksum           : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.BhChecksum);

END:
	return Status;
}


uint32_t AlFsbl_ImgHdrAuth(AlFsblInfo *FsblInstancePtr, uint32_t EfuseCtrl)
{
	PTRSIZE  ImageOffsetAddress;
	uint32_t AcOffset;
	uint32_t BootHdrAttrb;
	uint32_t Status = ALFSBL_SUCCESS;
	SecureInfo FsblIHSecInfo = {0};

	ImageOffsetAddress = FsblInstancePtr->ImageOffsetAddress;
	BootHdrAttrb = FsblInstancePtr->ImageHeader.BootHeader.BhAttr;

	AL_LOG(AL_LOG_LEVEL_INFO, "Efuse Ctrl:   %08x\r\n", EfuseCtrl);
	AL_LOG(AL_LOG_LEVEL_INFO, "BootHdrAttrb: %08x\r\n", BootHdrAttrb);

	/// get image header authentication type
	if((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) == EFUSE_AUTH_TYPE_SM2) {
		if((BootHdrAttrb & ALIH_BH_ATTRB_HD_AC_SEL_MASK) != ALIH_BH_ATTRB_HD_AC_SEL_SM2) {
			Status = ALFSBL_AUTHTYPE_NOT_MATCH_EFUSE;
			goto END;
		}
		FsblIHSecInfo.AuthType = OP_AUTH_SM2;
		FsblIHSecInfo.HashType = OP_HASH_SM3;
	}
	else if((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) == EFUSE_AUTH_TYPE_ECC256) {
		if((BootHdrAttrb & ALIH_BH_ATTRB_HD_AC_SEL_MASK) != ALIH_BH_ATTRB_HD_AC_SEL_ECC256) {
			Status = ALFSBL_AUTHTYPE_NOT_MATCH_EFUSE;
			goto END;
		}
		FsblIHSecInfo.AuthType = OP_AUTH_ECC256;
		FsblIHSecInfo.HashType = OP_HASH_SHA256;

	}
	else if((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) == EFUSE_AUTH_TYPE_HEADER_SET) {
		if((BootHdrAttrb & ALIH_BH_ATTRB_HD_AC_SEL_MASK) == ALIH_BH_ATTRB_HD_AC_SEL_SM2) {
			FsblIHSecInfo.AuthType = OP_AUTH_SM2;
			FsblIHSecInfo.HashType = OP_HASH_SM3;
		}
		else if((BootHdrAttrb & ALIH_BH_ATTRB_HD_AC_SEL_MASK) == ALIH_BH_ATTRB_HD_AC_SEL_ECC256) {
			FsblIHSecInfo.AuthType = OP_AUTH_ECC256;
			FsblIHSecInfo.HashType = OP_HASH_SHA256;
		}
	}
	else {
		Status = ALFSBL_ERROR_SEC_PARAM_INVALID;
		goto END;
	}
	AL_LOG(AL_LOG_LEVEL_INFO, "auth type: %x\r\n", FsblIHSecInfo.AuthType);

	AcOffset = FsblInstancePtr->ImageHeader.BootHeader.BhAcOffset;
	if(AcOffset == 0) {
		/// AC not exists, error
		AL_LOG(AL_LOG_LEVEL_ERROR, "ALFSBL_ERROR_IMAGE_HEADER_ACOFFSET\r\n");
		Status = ALFSBL_ERROR_IMAGE_HEADER_ACOFFSET;
		goto END;
	}
	else {
		/// AC exists, copy to memory
		AL_LOG(AL_LOG_LEVEL_INFO, "Copy Image Header AC\r\n");
		Status = FsblInstancePtr->DeviceOps.DeviceCopy(
				     ImageOffsetAddress + AcOffset,
					 (PTRSIZE)AuthBuffer,
					 ALFSBL_AUTH_BUFFER_SIZE,
					 NULL);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
		AL_LOG(AL_LOG_LEVEL_INFO, "Copy Image Header AC Finished\r\n");
	}

	/// ppk hash verify, if necessary
	AL_LOG(AL_LOG_LEVEL_INFO, "ppk verify...\r\n");
	Status = AlFsbl_PpkVerification(FsblInstancePtr, BootHdrAttrb, EfuseCtrl);
	if(Status != ALFSBL_SUCCESS) {
		AL_LOG(AL_LOG_LEVEL_ERROR, "ppk verify failed\r\n");
		goto END;
	}
	AL_LOG(AL_LOG_LEVEL_INFO, "ppk verify passed...\r\n");

	/// spk authenticate
	AL_LOG(AL_LOG_LEVEL_INFO, "spk verfy...\r\n");
	Status = AlFsbl_SpkVerification(FsblInstancePtr, &FsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		AL_LOG(AL_LOG_LEVEL_ERROR, "spk verify failed\r\n");
		goto END;
	}
	AL_LOG(AL_LOG_LEVEL_INFO, "spk verify passed...\r\n");

	AL_LOG(AL_LOG_LEVEL_INFO, "image header authentication...\r\n");
	FsblIHSecInfo.HashDataAddr   = PTR64_TO_UINT32(&(FsblInstancePtr->ImageHeader));
	FsblIHSecInfo.DataLength  = ALIH_BH_SIZE + ALIH_PH_SIZE * (FsblInstancePtr->ImageHeader.BootHeader.PartitionNum);
	FsblIHSecInfo.HashOutAddr = PTR64_TO_UINT32(HashBuffer);
	Status = AlFsbl_Hash(&FsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		AL_LOG(AL_LOG_LEVEL_ERROR, "image header hash failed\r\n");
		goto END;
	}
	AL_LOG(AL_LOG_LEVEL_INFO, "Image Header Hash finished...\r\n");

	FsblIHSecInfo.PubKeyAddr    = PTR64_TO_UINT32(AuthBuffer + ALAC_SPK_OFFSET);
	FsblIHSecInfo.SignatureAddr = PTR64_TO_UINT32(AuthBuffer + ALAC_BTHDR_SIGNATURE_OFFSET);
	Status = AlFsbl_Auth(&FsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		AL_LOG(AL_LOG_LEVEL_ERROR, "image header authentication failed\r\n");
		goto END;
	}
	AL_LOG(AL_LOG_LEVEL_INFO, "Image Header Authentication passed...\r\n");

END:
	return Status;
}



uint32_t AlFsbl_PpkVerification(AlFsblInfo *FsblInstancePtr, uint32_t BootHdrAttrb, uint32_t EfuseCtrl)
{
	uint32_t Status;
	uint8_t *pPpkHashAddr = AL_NULL;
	SecureInfo FsblPpkSecInfo = {0};

	if((EfuseCtrl & EFUSE_PPK_HASH_TYPE_MASK) == EFUSE_PPK_HASH_TYPE_SM3) {
		FsblPpkSecInfo.HashType = OP_HASH_SM3;
	}
	else if((EfuseCtrl & EFUSE_PPK_HASH_TYPE_MASK) == EFUSE_PPK_HASH_TYPE_SHA256) {
		FsblPpkSecInfo.HashType = OP_HASH_SHA256;
	}
	else if((EfuseCtrl & EFUSE_PPK_HASH_TYPE_MASK) == EFUSE_PPK_HASH_TYPE_HEADER_SET) {
		if((BootHdrAttrb & ALIH_BH_ATTRB_HD_PPK_HASH_MASK) == ALIH_BH_ATTRB_HD_PPK_HASH_MASK) {
			/// do not hash ppk
			Status = ALFSBL_SUCCESS;
			goto END;
		}
		else if((BootHdrAttrb & ALIH_BH_ATTRB_HD_AC_SEL_MASK) == ALIH_BH_ATTRB_HD_AC_SEL_SM2) {
			FsblPpkSecInfo.HashType = OP_HASH_SM3;
		}
		else if((BootHdrAttrb & ALIH_BH_ATTRB_HD_AC_SEL_MASK) == ALIH_BH_ATTRB_HD_AC_SEL_ECC256) {
			FsblPpkSecInfo.HashType = OP_HASH_SHA256;
		}
	}
	else {
		AL_LOG(AL_LOG_LEVEL_ERROR, "ALFSBL_ERROR_EFUSE_VALUE_INVALID\r\n");
		Status = ALFSBL_ERROR_SEC_PARAM_INVALID;
		goto END;
	}
	FsblPpkSecInfo.HashDataAddr = PTR64_TO_UINT32(AuthBuffer + ALAC_PPK_OFFSET);
	FsblPpkSecInfo.DataLength = PPK_BYTE_LENGTH;
	FsblPpkSecInfo.HashOutAddr = PTR64_TO_UINT32(HashBuffer);
	Status = AlFsbl_Hash(&FsblPpkSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	if((EfuseCtrl & EFUSE_PPK0_INVALID_MASK) != EFUSE_PPK0_INVALID_MASK) {
		pPpkHashAddr = (uint8_t *)(SYSCTRL_S_EFUSE_GLB00);
	}
	else if((EfuseCtrl & EFUSE_PPK1_INVALID_MASK) != EFUSE_PPK1_INVALID_MASK) {
		pPpkHashAddr = (uint8_t *)(SYSCTRL_S_EFUSE_GLB08);
	}
	else {
		Status = ALFSBL_NO_VALID_PPK;
	}

	Status = AlFsbl_CompareHash(HashBuffer, pPpkHashAddr, HASH_BYTE_LENGTH);
END:
	return Status;

}


uint32_t AlFsbl_SpkVerification(AlFsblInfo *FsblInstancePtr, SecureInfo *pFsblIHSecInfo)
{
	uint32_t Status;

	pFsblIHSecInfo->HashDataAddr = PTR64_TO_UINT32(AuthBuffer + ALAC_SPK_OFFSET);
	pFsblIHSecInfo->DataLength = SPK_BYTE_LENGTH;
	pFsblIHSecInfo->HashOutAddr = PTR64_TO_UINT32(HashBuffer);
	Status = AlFsbl_Hash(pFsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	pFsblIHSecInfo->PubKeyAddr = PTR64_TO_UINT32(AuthBuffer + ALAC_PPK_OFFSET);
	pFsblIHSecInfo->SignatureAddr = PTR64_TO_UINT32(AuthBuffer + ALAC_SPK_SIGNATURE_OFFSET);
	Status = AlFsbl_Auth(pFsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
END:
	return Status;
}











