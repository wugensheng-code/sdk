/*
 * alfsbl_image_header.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */
#include <stdio.h>
#include "alfsbl_image_header.h"
#include "alfsbl_sec.h"

//extern uint8_t  ReadBuffer[READ_BUFFER_SIZE];
extern uint8_t  AuthBuffer[ALFSBL_AUTH_BUFFER_SIZE];
//extern uint32_t Iv[ALIH_BH_IV_LENGTH / 4U];
//extern SecureInfo FsblSecInfo;


uint32_t AlFsbl_BootDevInitAndHdrValidate(AlFsblInfo *FsblInstancePtr)
{
	uint32_t Status = ALFSBL_SUCCESS;

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
	uint32_t i;
	uint32_t Status = ALFSBL_SUCCESS;
	PTRSIZE  ImageOffsetAddress;
	uint32_t BootHdrAttrb;
	uint32_t AcOffset;
	uint32_t PartitionNum;
	uint32_t PartitionHeaderOffset;
	uint32_t EfuseCtrl;
	uint32_t MultiBoot;
//	uint32_t *ReadBuffer32 = (uint32_t *)(ReadBuffer);

	/// get multi boot value
	MultiBoot = REG32(SYSCTRL_S_MULTI_BOOT);

	/// get image offset address based on multi boot reg value
	FsblInstancePtr->ImageOffsetAddress = MultiBoot * QSPI_FLASH_SEARCH_STEP;

	ImageOffsetAddress = FsblInstancePtr->ImageOffsetAddress;
	printf("FsblInstancePtr->ImageOffsetAddress: 0x%08x\r\n", ImageOffsetAddress);

	/// copy boot header to internal memory
	Status = FsblInstancePtr->DeviceOps.DeviceCopy(
			     ImageOffsetAddress,
				 (PTRSIZE)(&(FsblInstancePtr->ImageHeader.BootHeader)),
				 ALIH_BH_SIZE,
				 NULL);

	if (ALFSBL_SUCCESS != Status) {
		printf("boot header copy failed...\r\n");
		goto END;
	}
	printf("boot header copy finished...\n");

#if (!defined FSBL_SIMU_SKIP_HEADERCHECK)
	Status = AlFsbl_ChecksumCheck(
			(uint8_t *)(&(FsblInstancePtr->ImageHeader.BootHeader.QspiWidthSel)),
			60,
			FsblInstancePtr->ImageHeader.BootHeader.BhChecksum);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
#endif

	/// read partition headers
	PartitionNum = FsblInstancePtr->ImageHeader.BootHeader.PartitionNum;
	PartitionHeaderOffset = FsblInstancePtr->ImageHeader.BootHeader.FirstPartiHdrOffset;
	Status = FsblInstancePtr->DeviceOps.DeviceCopy(
			     PartitionHeaderOffset + ImageOffsetAddress,
				 (PTRSIZE)(&(FsblInstancePtr->ImageHeader.PartitionHeader[0])),
				 ALIH_PH_SIZE * PartitionNum,
				 NULL);
	if (ALFSBL_SUCCESS != Status) {
		printf("partition headers copy failed...\r\n");
		goto END;
	}
	printf("partition headers copy finished...\r\n");

	/// authenticate image header
	BootHdrAttrb = FsblInstancePtr->ImageHeader.BootHeader.BhAttr;
	EfuseCtrl = REG32(EFUSE_SEC_CTRL);

#if (!defined FSBL_SIMU_SKIP_HEADERCHECK)
	/// if authentication enabled
	if(((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) != 0) || ((BootHdrAttrb & ALIH_BH_ATTRB_HD_AC_SEL_MASK) != 0)) {
		printf("image header authentication enabled...\r\n");
		Status = AlFsbl_ImgHdrAuth(FsblInstancePtr, EfuseCtrl);
		if(Status != ALFSBL_SUCCESS) {
			printf("image header authentication failed...\r\n");
			goto END;
		}
		printf("image header authentication passed...\r\n");
	}
	else {
		printf("image header authentication not enabled....\n");
	}
#endif

	printf("qspi width sel        : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.QspiWidthSel);
	printf("image id              : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.ImageId);
	printf("enc status            : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.EncStatus);
	printf("bh attribute          : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.BhAttr);
	printf("bh ac offset          : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.BhAcOffset);
	printf("first parti hdr offset: 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.FirstPartiHdrOffset);
	printf("partition num         : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.PartitionNum);
	printf("bh checksum           : 0x%08x\r\n", FsblInstancePtr->ImageHeader.BootHeader.BhChecksum);

END:
	return Status;
}






/*
uint32_t AlFsbl_ReadPartitionHeader(AlFsblInfo *AlFsblInstancePtr, uint32_t ImageAddr)
{
	uint32_t Status;
	uint32_t PartitionHeaderAddress;
	uint32_t PartitionIndex;
	AlFsbl_PartitionHeader *CurrPartitionHdr;
	uint32_t PartitionNum;

//	PartitionHeaderAddress = (AlFsblInstancePtr->ImageHeader.BootHeader.FirstPartiHdrOffset) * AL_WORD_LENGTH;
	PartitionHeaderAddress = (AlFsblInstancePtr->ImageHeader.BootHeader.FirstPartiHdrOffset);
	PartitionNum = AlFsblInstancePtr->ImageHeader.BootHeader.PartitionNum;

	for(PartitionIndex = 0; PartitionIndex < PartitionNum; PartitionIndex++) {
		printf("read partition header Num %d\r\n", PartitionIndex);

		Status = AlFsblInstancePtr->DeviceOps.DeviceCopy(
					ImageAddr + PartitionHeaderAddress,
					(PTRSIZE)(&(AlFsblInstancePtr->ImageHeader.PartitionHeader[PartitionIndex])),
					ALIH_PH_SIZE);
		if(ALFSBL_SUCCESS != Status) {
			printf("partition header copy failed...\r\n");
			goto END;
		}
		CurrPartitionHdr = &(AlFsblInstancePtr->ImageHeader.PartitionHeader[PartitionIndex]);
		PartitionHeaderAddress = CurrPartitionHdr->NextPartHdrOffset;
//		PartitionHeaderAddress = CurrPartitionHdr->NextPartHdrOffset;

		/// check partition header infomation
		AlFsbl_PrintPartitionHeaderInfo(CurrPartitionHdr);
	}

END:
	return 0;
}
*/


uint32_t AlFsbl_ImgHdrAuth(AlFsblInfo *FsblInstancePtr, uint32_t EfuseCtrl)
{
	PTRSIZE  ImageOffsetAddress;
	uint32_t AcOffset;
	uint32_t BootHdrAttrb;
	uint32_t Status = ALFSBL_SUCCESS;
	uint8_t  AuthType;
	uint8_t HashBuffer[32];
	SecureInfo FsblIHSecInfo = {0};

	ImageOffsetAddress = FsblInstancePtr->ImageOffsetAddress;
	BootHdrAttrb = FsblInstancePtr->ImageHeader.BootHeader.BhAttr;

	printf("Efuse Ctrl:   %08x\n", EfuseCtrl);
	printf("BootHdrAttrb: %08x\n", BootHdrAttrb);

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
	printf("auth type: %x\r\n", AuthType);

	AcOffset = FsblInstancePtr->ImageHeader.BootHeader.BhAcOffset;
	if(AcOffset == 0) {
		/// AC not exists, error
		printf("ALFSBL_ERROR_IMAGE_HEADER_ACOFFSET\r\n");
		Status = ALFSBL_ERROR_IMAGE_HEADER_ACOFFSET;
		goto END;
	}
	else {
		/// AC exists, copy to memory
		printf("Copy Image Header AC\r\n");
		Status = FsblInstancePtr->DeviceOps.DeviceCopy(
				     ImageOffsetAddress + AcOffset,
					 (PTRSIZE)AuthBuffer,
					 ALFSBL_AUTH_BUFFER_SIZE,
					 NULL);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
		printf("Copy Image Header AC Finished\r\n");
	}

	/// ppk hash verify, if necessary
	printf("ppk verify...\r\n");
	Status = AlFsbl_PpkVerification(FsblInstancePtr, BootHdrAttrb, EfuseCtrl);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
	printf("ppk verify passed...\r\n");

	/// spk authenticate
	printf("spk verfy...\r\n");
	Status = AlFsbl_SpkVerification(FsblInstancePtr, &FsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
	printf("spk verify passed...\r\n");

	printf("image header authentication...\r\n");
	FsblIHSecInfo.InputAddr   = (uint32_t)(&(FsblInstancePtr->ImageHeader));
	FsblIHSecInfo.DataLength  = ALIH_BH_SIZE + ALIH_PH_SIZE * (FsblInstancePtr->ImageHeader.BootHeader.PartitionNum);
	FsblIHSecInfo.HashOutAddr = (uint32_t)(HashBuffer);
	Status = AlFsbl_Hash_1(&FsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
	printf("Image Header Hash finished...\r\n");

	FsblIHSecInfo.PubKeyAddr    = (uint32_t)(AuthBuffer + ALAC_SPK_OFFSET);
	FsblIHSecInfo.SignatureAddr = (uint32_t)(AuthBuffer + ALAC_BTHDR_SIGNATURE_OFFSET);
	Status = AlFsbl_Auth_1(&FsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
	printf("Image Header Authentication passed...\r\n");

END:
	return Status;
}



uint32_t AlFsbl_PpkVerification(AlFsblInfo *FsblInstancePtr, uint32_t BootHdrAttrb, uint32_t EfuseCtrl)
{
	uint32_t Status;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM + 64);
	uint8_t HashType;
	uint8_t HashBuffer[32];
	uint8_t *pPpkHashAddr;
	SecureInfo FsblPpkSecInfo = {0};

	if((EfuseCtrl & EFUSE_PPK_HASH_TYPE_MASK) == EFUSE_PPK_HASH_TYPE_SM3) {
		//HashType = HASH_TYPE_SM3;
		FsblPpkSecInfo.HashType = OP_HASH_SM3;
	}
	else if((EfuseCtrl & EFUSE_PPK_HASH_TYPE_MASK) == EFUSE_PPK_HASH_TYPE_SHA256) {
		//HashType = HASH_TYPE_SHA256;
		FsblPpkSecInfo.HashType = OP_HASH_SHA256;
	}
	else if(((EfuseCtrl & EFUSE_PPK_HASH_TYPE_MASK) == EFUSE_PPK_HASH_TYPE_HEADER_SET) &&
			((BootHdrAttrb & ALIH_BH_ATTRB_HD_PPK_HASH_MASK) == ALIH_BH_ATTRB_HD_PPK_HASH_MASK)) {
		/// do not hash ppk
		Status = ALFSBL_SUCCESS;
		goto END;
	}
	else {
		printf("ALFSBL_ERROR_EFUSE_VALUE_INVALID\r\n");
		Status = ALFSBL_ERROR_SEC_PARAM_INVALID;
	}
	FsblPpkSecInfo.InputAddr = (uint32_t)(AuthBuffer + ALAC_PPK_OFFSET);
	FsblPpkSecInfo.DataLength = PPK_BYTE_LENGTH;
	FsblPpkSecInfo.HashOutAddr = (uint32_t)(HashBuffer);
	Status = AlFsbl_Hash_1(&FsblPpkSecInfo);
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


//uint32_t AlFsbl_SpkVerification(AlFsblInfo *FsblInstancePtr, uint32_t BootHdrAttrb, uint32_t EfuseCtrl)
uint32_t AlFsbl_SpkVerification(AlFsblInfo *FsblInstancePtr, SecureInfo *pFsblIHSecInfo)
{
	uint32_t Status;
	uint8_t AuthType;
	uint8_t HashBuffer[32];

	pFsblIHSecInfo->InputAddr = (uint32_t)(AuthBuffer + ALAC_SPK_OFFSET);
	pFsblIHSecInfo->DataLength = SPK_BYTE_LENGTH;
	pFsblIHSecInfo->HashOutAddr = (uint32_t)(HashBuffer);
	Status = AlFsbl_Hash_1(pFsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	pFsblIHSecInfo->PubKeyAddr = (uint32_t)(AuthBuffer + ALAC_PPK_OFFSET);
	pFsblIHSecInfo->SignatureAddr = (uint32_t)(AuthBuffer + ALAC_SPK_SIGNATURE_OFFSET);
	Status = AlFsbl_Auth_1(pFsblIHSecInfo);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
	printf("spk verification passed\r\n");
END:
	return Status;
}





















