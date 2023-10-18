/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdint.h>
#include "al_reg_io.h"
#include "soc_plat.h"
#include "alfsbl_secure.h"
#include "alfsbl_misc.h"
#include "alfsbl_err_code.h"
#include "alfsbl_hw.h"
#include "alfsbl_data.h"
#include "alfsbl_boot.h"
#include "alfsbl_partition_load.h"
#include "al_systimer.h"
#include "al_utils_def.h"


extern uint8_t  ReadBuffer[READ_BUFFER_SIZE];

static uint32_t AlFsbl_PartitionHeaderValidation(AlFsblInfo *FsblInstancePtr, uint32_t PartitionIdx, SecureInfo *pSecureInfo);

static uint32_t AlFsbl_LoadPsPartition(AlFsblInfo *FsblInstancePtr, SecureInfo *pSecureInfo, uint32_t PartitionIdx);

static uint32_t AlFsbl_LoadPlPartition(AlFsblInfo *FsblInstancePtr, SecureInfo *pSecureInfo, uint32_t PartitionIdx);

static uint32_t AlFsbl_BitstreamDataTransfer(AlFsblInfo *FsblInstancePtr, SecureInfo *pSecureInfo,
		                                     uint32_t PartitionIdx, uint32_t BufferAddr, uint32_t BlockSizeMax);


static __attribute__((noinline)) uint32_t AlFsbl_CheckPlInitDone(void);
static uint32_t ALFsbl_BitStreamProgDone(void);

static void     AlFsbl_PrintPartitionHeaderInfo(AlFsbl_PartitionHeader *PtHdr);

extern uint8_t  ReadBuffer[READ_BUFFER_SIZE];
extern SecureInfo FsblSecInfo;
extern uint8_t  AuthBuffer[ALFSBL_AUTH_BUFFER_SIZE];

static uint32_t __attribute__((aligned(4))) RpuWait = 0xa001a001;
static uint8_t  RpuWakeUpFlag = 0;

uint32_t AlFsbl_PartitionLoad(AlFsblInfo *FsblInstancePtr, uint32_t PartitionIdx)
{
	uint32_t Status;
	uint32_t DestDev;

	/// todo: restart wdt


	/// partition header validation
	Status = AlFsbl_PartitionHeaderValidation(FsblInstancePtr, PartitionIdx, &FsblSecInfo);
	if(ALFSBL_SUCCESS_NOT_PARTITION_OWNER == Status) {
		Status = ALFSBL_SUCCESS;
		goto END;
	}
	else if(ALFSBL_SUCCESS != Status) {
		goto END;
	}

	DestDev = FsblInstancePtr->ImageHeader.PartitionHeader[PartitionIdx].PartitionAttribute & ALIH_PH_ATTRIB_DEST_DEV_MASK;

	if(DestDev == ALIH_PH_ATTRIB_DEST_DEV_PS) {
		AL_LOG(AL_LOG_LEVEL_INFO, "loading ps partition...\r\n");
		Status = AlFsbl_LoadPsPartition(FsblInstancePtr, &FsblSecInfo, PartitionIdx);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
	}
	else if(DestDev == ALIH_PH_ATTRIB_DEST_DEV_PL) {
		if(FsblInstancePtr->ResetReason == FSBL_PS_ONLY_RESET) {
			AL_LOG(AL_LOG_LEVEL_INFO, "ps only reset, no need to load pl partition\r\n");
			Status = ALFSBL_SUCCESS;
			goto END;
		}
		AL_LOG(AL_LOG_LEVEL_INFO, "loading pl partition...\r\n");
		Status = AlFsbl_LoadPlPartition(FsblInstancePtr, &FsblSecInfo, PartitionIdx);
	}
	else {
		/**do nothing**/
	}
END:
	return Status;
}


static uint32_t AlFsbl_PartitionHeaderValidation(AlFsblInfo *FsblInstancePtr, uint32_t PartitionIdx, SecureInfo *pSecureInfo)
{
	uint32_t Status = 0;
	AlFsbl_PartitionHeader *PtHdr;
	uint32_t PtAttrHashType;
	uint32_t PtAttrAuthType;
	uint32_t PtAttrEncType;
	uint32_t DestCpu;
	uint32_t DestDev;
	uint32_t EfuseCtrl;
	uint32_t PartitionAttr;
	uint32_t RunningCpu;
	uint64_t RpuWaitAddr = 0;

	RunningCpu = FsblInstancePtr->ProcessorID;

	EfuseCtrl = AL_REG32_READ(EFUSE_SEC_CTRL);
	PtHdr = &(FsblInstancePtr->ImageHeader.PartitionHeader[PartitionIdx]);
	PartitionAttr = PtHdr->PartitionAttribute;

	/// check checksum of partition header
	AL_LOG(AL_LOG_LEVEL_INFO, "Partition Header Checksum: 0x%08x\r\n", PtHdr->PartiHdrChecksum);
	Status = AlFsbl_ChecksumCheck(
				(uint8_t *)(PtHdr),
				sizeof(AlFsbl_PartitionHeader) - 4,   /// the last word in PartitionHeader is checksum
				PtHdr->PartiHdrChecksum);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// check partition owner
	if((PartitionAttr & ALIH_PH_ATTRIB_PART_OWNER_MASK) != ALIH_PH_ATTRIB_PART_OWNER_FSBL) {
		AL_LOG(AL_LOG_LEVEL_INFO, "Partition owner: %08x\r\n", PtHdr->PartitionAttribute & ALIH_PH_ATTRIB_PART_OWNER_MASK);
		AL_LOG(AL_LOG_LEVEL_INFO, "Partition owner not fsbl, skip it, partition num: %u\r\n", PartitionIdx);
		Status = ALFSBL_SUCCESS_NOT_PARTITION_OWNER;
		goto END;
	}


	/// check hash type
	PtAttrHashType = PartitionAttr & ALIH_PH_ATTRIB_HASH_TYPE_MASK;
	AL_LOG(AL_LOG_LEVEL_INFO, "Hash Type: 0x%08x\r\n", PtAttrHashType);
	if(PtAttrHashType == ALIH_PH_ATTRIB_HASH_TYPE_NONE) {
		pSecureInfo->HashType = OP_HASH_NONE;
		AL_LOG(AL_LOG_LEVEL_INFO, "Hash NOT enabled.\r\n");
	}
	else if(PtAttrHashType == ALIH_PH_ATTRIB_HASH_TYPE_SHA256) {
		pSecureInfo->HashType = OP_HASH_SHA256;
		AL_LOG(AL_LOG_LEVEL_INFO, "Hash SHA256.\r\n");
	}
	else if(PtAttrHashType == ALIH_PH_ATTRIB_HASH_TYPE_SM3) {
		pSecureInfo->HashType = OP_HASH_SM3;
		AL_LOG(AL_LOG_LEVEL_INFO, "Hash SM3.\r\n");
	}
	else {
		AL_LOG(AL_LOG_LEVEL_INFO, "Invalid Hash Type\r\n");
		Status = ALFSBL_INVALID_HASH_TYPE;
		goto END;
	}

	/// check authentication type
	PtAttrAuthType = PartitionAttr & ALIH_PH_ATTRIB_AUTH_TYPE_MASK;
	AL_LOG(AL_LOG_LEVEL_INFO, "Auth Type       : %x\r\n", PtAttrAuthType);
	AL_LOG(AL_LOG_LEVEL_INFO, "Efuse Auth Type : %x\r\n", EfuseCtrl & EFUSE_AUTH_TYPE_MASK);
	if((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) == EFUSE_AUTH_TYPE_HEADER_SET) {
		if(PtAttrAuthType == ALIH_PH_ATTRIB_AUTH_TYPE_NONE) {
			pSecureInfo->AuthType = OP_AUTH_NONE;
			AL_LOG(AL_LOG_LEVEL_INFO, "Authentication NOT enabled...\r\n");
		}
		else if(PtAttrAuthType == ALIH_PH_ATTRIB_AUTH_TYPE_ECC256) {
			pSecureInfo->AuthType = OP_AUTH_ECC256;
			pSecureInfo->HashType = OP_HASH_SHA256;
			AL_LOG(AL_LOG_LEVEL_INFO, "Authentication ECC256\r\n");
		}
		else if(PtAttrAuthType == ALIH_PH_ATTRIB_AUTH_TYPE_SM2) {
			pSecureInfo->AuthType = OP_AUTH_SM2;
			pSecureInfo->HashType = OP_HASH_SM3;
			AL_LOG(AL_LOG_LEVEL_INFO, "Authentication SM2\r\n");
		}
	}
	else if(((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) == EFUSE_AUTH_TYPE_ECC256) &&
			(PtAttrAuthType == ALIH_PH_ATTRIB_AUTH_TYPE_ECC256)) {
		pSecureInfo->AuthType = OP_AUTH_ECC256;
		pSecureInfo->HashType = OP_HASH_SHA256;
		AL_LOG(AL_LOG_LEVEL_INFO, "Authentication ECC256\r\n");
	}
	else if(((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) == EFUSE_AUTH_TYPE_SM2) &&
			(PtAttrAuthType == ALIH_PH_ATTRIB_AUTH_TYPE_SM2)) {
		pSecureInfo->AuthType = OP_AUTH_SM2;
		pSecureInfo->HashType = OP_HASH_SM3;
		AL_LOG(AL_LOG_LEVEL_INFO, "Authentication SM2\r\n");
	}
	else {
		AL_LOG(AL_LOG_LEVEL_ERROR, "Auth type not match efuse set...\r\n");
		Status = ALFSBL_AUTHTYPE_NOT_MATCH_EFUSE;
		goto END;
	}

	/// check encryption type
	PtAttrEncType  = PartitionAttr & ALIH_PH_ATTRIB_ENC_TYPE_MASK;
	AL_LOG(AL_LOG_LEVEL_INFO, "Enc Type       : %x\r\n", PtAttrEncType);
	AL_LOG(AL_LOG_LEVEL_INFO, "Efuse Enc Type : %x\r\n", EfuseCtrl & EFUSE_ENC_TYPE_MASK);
	if((EfuseCtrl & EFUSE_ENC_TYPE_MASK) == EFUSE_ENC_TYPE_HEADER_SET) {
		if(PtAttrEncType == ALIH_PH_ATTRIB_ENC_TYPE_NONE) {
			pSecureInfo->EncType = OP_ENCRYPT_NONE;
			AL_LOG(AL_LOG_LEVEL_INFO, "Encrypt NOT enabled.\r\n");
		}
		else if(PtAttrEncType == ALIH_PH_ATTRIB_ENC_TYPE_AES256) {
			pSecureInfo->EncType = OP_ENCRYPT_AES256;
			AL_LOG(AL_LOG_LEVEL_INFO, "Encrypt AES256.\r\n");
		}
		else if(PtAttrEncType == ALIH_PH_ATTRIB_ENC_TYPE_SM4) {
			pSecureInfo->EncType = OP_ENCRYPT_SM4;
			AL_LOG(AL_LOG_LEVEL_INFO, "Encrypt SM4.\r\n");
		}
	}
	else if(((EfuseCtrl & EFUSE_ENC_TYPE_MASK) == EFUSE_ENC_TYPE_AES256) &&
			(PtAttrEncType == ALIH_PH_ATTRIB_ENC_TYPE_AES256)) {
		pSecureInfo->EncType = OP_ENCRYPT_AES256;
		AL_LOG(AL_LOG_LEVEL_INFO, "Encrypt AES256.\r\n");
	}
	else if(((EfuseCtrl & EFUSE_ENC_TYPE_MASK) == EFUSE_ENC_TYPE_SM4) &&
			(PtAttrEncType == ALIH_PH_ATTRIB_ENC_TYPE_SM4)) {
		pSecureInfo->EncType = OP_ENCRYPT_SM4;
		AL_LOG(AL_LOG_LEVEL_INFO, "Encrypt SM4.\r\n");
	}
	else {
		AL_LOG(AL_LOG_LEVEL_ERROR, "Encrypt type not match efuse set...\r\n");
		Status = ALFSBL_ENCTYPE_NOT_MATCH_EFUSE;
		goto END;
	}

	/// check enc type, auth type, hash type match
	if((pSecureInfo->EncType == OP_ENCRYPT_SM4) &&
	   ((pSecureInfo->HashType == OP_HASH_SHA256) || (pSecureInfo->AuthType == OP_AUTH_ECC256))) {
		Status = ALFSBL_SEC_TYPE_MISMACTCH;
		goto END;
	}
	else if((pSecureInfo->EncType == OP_ENCRYPT_AES256) &&
	        ((pSecureInfo->HashType == OP_HASH_SM3) || (pSecureInfo->AuthType == OP_AUTH_SM2))) {
		Status = ALFSBL_SEC_TYPE_MISMACTCH;
		goto END;
	}
	else if(((pSecureInfo->HashType == OP_HASH_SM3) && (pSecureInfo->AuthType == OP_AUTH_ECC256)) ||
		    ((pSecureInfo->HashType == OP_HASH_SHA256) && (pSecureInfo->AuthType == OP_AUTH_SM2))) {
		Status = ALFSBL_SEC_TYPE_MISMACTCH;
		goto END;
	}

	/// check destination cpu
	DestCpu = PartitionAttr & ALIH_PH_ATTRIB_DEST_CPU_MASK;
	AL_LOG(AL_LOG_LEVEL_INFO, "destination cpu: 0x%08x\r\n", DestCpu);
	if(DestCpu != ALIH_PH_ATTRIB_DEST_CPU_RPU &&
	   DestCpu != ALIH_PH_ATTRIB_DEST_CPU_APU0 &&
	   DestCpu != ALIH_PH_ATTRIB_DEST_CPU_APU1) {
		Status = ALFSBL_INVALID_DEST_CPU;
		goto END;
	}

	/// check destination device
	DestDev = PartitionAttr & ALIH_PH_ATTRIB_DEST_DEV_MASK;
	AL_LOG(AL_LOG_LEVEL_INFO, "destination dev: 0x%08x\r\n", DestDev);
	if(DestDev != ALIH_PH_ATTRIB_DEST_DEV_PS &&
	   DestDev != ALIH_PH_ATTRIB_DEST_DEV_PL) {
		Status = ALFSBL_INVALID_DEST_DEV;
		goto END;
	}

	/// check partion length
	AL_LOG(AL_LOG_LEVEL_INFO, "check partition length: \r\n");
	if((PtHdr->PartitionLen < PtHdr->ExtractedPartitionLen) ||
	   (PtHdr->TotalPartitionLen < PtHdr->ExtractedPartitionLen)) {
		Status = ALFSBL_INVALID_PARTITION_LENGTH;
		goto END;
	}
	if((!PtAttrAuthType) && (!PtAttrHashType)) {
		if(PtHdr->PartitionLen != PtHdr->TotalPartitionLen) {
			Status = ALFSBL_INVALID_PARTITION_LENGTH;
			goto END;
		}
	}
	else {
		if(PtHdr->PartitionLen >= PtHdr->TotalPartitionLen) {
			Status = ALFSBL_INVALID_PARTITION_LENGTH;
			goto END;
		}
	}
	FsblSecInfo.DataLength = PtHdr->PartitionLen;

	/// check load and exec address
	if((AL_DDR1_BASE_ADDR <= PtHdr->DestLoadAddr) && (PtHdr->DestLoadAddr < AL_DDR1_BASE_ADDR + AL_DDR1_BYTE_LENGTH)) {
		// correct, do nothing
	}
	else if((AL_DDR2_BASE_ADDR <= PtHdr->DestLoadAddr) && (PtHdr->DestLoadAddr < AL_DDR2_BASE_ADDR + AL_DDR2_BYTE_LENGTH)) {
		// correct, do nothing
	}
	else if((AL_RPU_ITCM_BASE_ADDR <= PtHdr->DestLoadAddr) && (PtHdr->DestLoadAddr < AL_RPU_ITCM_BASE_ADDR + AL_RPU_ITCM_BYTE_LENGTH)) {
		if((RunningCpu != ALIH_PH_ATTRIB_DEST_CPU_RPU)  && (!RpuWakeUpFlag)) {
			RpuWaitAddr = (uint64_t)(&RpuWait);
			RpuWakeUpFlag = 1;
			AL_REG32_WRITE(SYSCTRL_S_RPU_RESET_VECTOR_H, RpuWaitAddr >> 32);
			AL_REG32_WRITE(SYSCTRL_S_RPU_RESET_VECTOR_L, RpuWaitAddr & 0xffffffff);
			AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 10, 1);  /// release level reset
			AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 8, 0);   /// trigger pulse reset
		}
	}
	else if((AL_RPU_DTCM_BASE_ADDR <= PtHdr->DestLoadAddr) && (PtHdr->DestLoadAddr < AL_RPU_DTCM_BASE_ADDR + AL_RPU_ITCM_BYTE_LENGTH)) {
		if((RunningCpu != ALIH_PH_ATTRIB_DEST_CPU_RPU)  && (!RpuWakeUpFlag)) {
			RpuWaitAddr = (uint64_t)(&RpuWait);
			RpuWakeUpFlag = 1;
			AL_REG32_WRITE(SYSCTRL_S_RPU_RESET_VECTOR_H, RpuWaitAddr >> 32);
			AL_REG32_WRITE(SYSCTRL_S_RPU_RESET_VECTOR_L, RpuWaitAddr & 0xffffffff);
			AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 10, 1);  /// release level reset
			AL_REG32_SET_BIT(SYSCTRL_S_XPU_SRST, 8, 0);   /// trigger pulse reset
		}
	}
	else if((AL_OCM_BASE_ADDR <= PtHdr->DestLoadAddr) && (PtHdr->DestLoadAddr < AL_OCM_BASE_ADDR + AL_OCM_BYTE_LENGTH)) {
		// correct, do nothing
	}
	else if ((PtHdr->DestLoadAddr == CSU_PCAP_WR_STREAM) || (PtHdr->DestLoadAddr == CSU_PCAP_CSULOCAL_WR_STREAM)) {
		// correct, do nothing
	}
	else {
		Status = ALFSBL_INVALID_LOAD_ADDR;
	}

	if(PtHdr->DestExecAddr == 0xFFFFFFFF) {
		// correct, do nothing, it is a none handoff ps partition
	}
	else if((AL_DDR1_BASE_ADDR <= PtHdr->DestExecAddr) && (PtHdr->DestExecAddr < AL_DDR1_BASE_ADDR + AL_DDR1_BYTE_LENGTH)) {
		// correct, do nothing
	}
	else if((AL_DDR2_BASE_ADDR <= PtHdr->DestExecAddr) && (PtHdr->DestExecAddr < AL_DDR2_BASE_ADDR + AL_DDR2_BYTE_LENGTH)) {
		// correct, do nothing
	}
	else if((AL_RPU_ITCM_BASE_ADDR <= PtHdr->DestExecAddr) && (PtHdr->DestExecAddr < AL_RPU_ITCM_BASE_ADDR + AL_RPU_ITCM_BYTE_LENGTH)) {
		// correct, do nothing
	}
	else if((AL_OCM_BASE_ADDR <= PtHdr->DestExecAddr) && (PtHdr->DestExecAddr < AL_OCM_BASE_ADDR + AL_OCM_BYTE_LENGTH)) {
		// correct, do nothing
	}
	else {
		Status = ALFSBL_INVALID_EXEC_ADDR;
	}



END:
    AlFsbl_PrintPartitionHeaderInfo(PtHdr);
	return Status;
}


static uint32_t AlFsbl_LoadPsPartition(AlFsblInfo *FsblInstancePtr, SecureInfo *pSecureInfo, uint32_t PartitionIdx)
{
	AlFsbl_PartitionHeader *PtHdr;

	uint32_t Status = 0;
	uint8_t  HashBuffer[32];
	uint32_t HashByteLen;
	uint32_t DestCpu;
	uint32_t HandoffNum;
	uint64_t SrcAddress;
	uint64_t LoadAddress;
	uint32_t Length;
	uint32_t PartitionAcOffset;
	PTRSIZE  ImageOffsetAddress;
	uint8_t  PartitionAc[ALFSBL_AUTH_BUFFER_SIZE];

	int i;
	uint32_t *ocmptr = NULL;

	PtHdr = &(FsblInstancePtr->ImageHeader.PartitionHeader[PartitionIdx]);
	DestCpu = PtHdr->PartitionAttribute & ALIH_PH_ATTRIB_DEST_CPU_MASK;
	ImageOffsetAddress = FsblInstancePtr->ImageOffsetAddress;

	SrcAddress = FsblInstancePtr->ImageOffsetAddress + PtHdr->PartitionOffset;
	LoadAddress = PtHdr->DestLoadAddr;
	Length = PtHdr->PartitionLen;

	AL_LOG(AL_LOG_LEVEL_INFO, "partition src address      : 0x%lx\r\n", SrcAddress);
	AL_LOG(AL_LOG_LEVEL_INFO, "partition load dest address: 0x%lx\r\n", LoadAddress);
	AL_LOG(AL_LOG_LEVEL_INFO, "partition length           : 0x%08x\r\n", Length);

	pSecureInfo->HashOutAddr    = (uint32_t)HashBuffer;
	pSecureInfo->KeyMode        = OP_BHDR_KEY;
	pSecureInfo->EncMode        = SYM_ECB;
	pSecureInfo->EncDir         = SYM_DECRYPT;
	pSecureInfo->CsuAddrIncMode = CSUDMA_DST_INCR | CSUDMA_SRC_INCR;
	pSecureInfo->BlockMode      = WHOLE_BLOCK;

	/// check secure info
	AL_LOG(AL_LOG_LEVEL_INFO, "Auth type :  %02x\r\n", pSecureInfo->AuthType);
	AL_LOG(AL_LOG_LEVEL_INFO, "Hash type :  %02x\r\n", pSecureInfo->HashType);
	AL_LOG(AL_LOG_LEVEL_INFO, "Enc type  :  %02x\r\n", pSecureInfo->EncType);
	AL_LOG(AL_LOG_LEVEL_INFO, "Enc mode  :  %02x\r\n", pSecureInfo->EncMode);
	AL_LOG(AL_LOG_LEVEL_INFO, "Key mode  :  %02x\r\n", pSecureInfo->KeyMode);

	/// partition data copy
	if((FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_QSPI24) ||
	   (FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_QSPI32)) {
		Status = FsblInstancePtr->DeviceOps.DeviceCopy(
				     SrcAddress,
					 LoadAddress,
					 Length,
					 pSecureInfo);
		if(ALFSBL_SUCCESS != Status) {
			goto END;
		}
#ifndef QSPI_XIP_THROUTH_CSU_DMA
		if(pSecureInfo->EncType != OP_ENCRYPT_NONE) {
			pSecureInfo->InputAddr  = LoadAddress;
			pSecureInfo->OutputAddr = LoadAddress;
			pSecureInfo->DataLength = Length;
			Status = AlFsbl_DecHash(pSecureInfo);
		}
		if(ALFSBL_SUCCESS != Status) {
			goto END;
		}
#endif
	}
	else if(FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_NAND) {
		Status = FsblInstancePtr->DeviceOps.DeviceCopy(SrcAddress, LoadAddress, Length, NULL);
		if(ALFSBL_SUCCESS != Status) {
			goto END;
		}
		/// if encrypt enabled, do decrypt (and hash if necessary) through csu dma
		if(pSecureInfo->EncType != OP_ENCRYPT_NONE) {
			pSecureInfo->InputAddr  = LoadAddress;
			pSecureInfo->OutputAddr = LoadAddress;
			pSecureInfo->DataLength = Length;
			Status = AlFsbl_DecHash(pSecureInfo);
		}
		if(ALFSBL_SUCCESS != Status) {
			goto END;
		}
	}
	else if((FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_SD)   ||
			(FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_EMMC) ||
			(FsblInstancePtr->PrimaryBootDevice == ALFSBL_BOOTMODE_EMMC_RAW)) {
		Status = FsblInstancePtr->DeviceOps.DeviceCopy(SrcAddress, LoadAddress, Length, NULL);
		if(ALFSBL_SUCCESS != Status) {
			goto END;
		}
		/// if encrypt enabled, do decrypt (and hash if necessary) through csu dma
		if(pSecureInfo->EncType != OP_ENCRYPT_NONE) {
			pSecureInfo->InputAddr  = LoadAddress;
			pSecureInfo->OutputAddr = LoadAddress;
			pSecureInfo->DataLength = Length;
			Status = AlFsbl_DecHash(pSecureInfo);
		}
		if(ALFSBL_SUCCESS != Status) {
			goto END;
		}
	}
	else {
	}

	/// calculate data hash without csu dma
	if((pSecureInfo->EncType == OP_ENCRYPT_NONE) && (pSecureInfo->HashType != OP_HASH_NONE)) {
		AL_LOG(AL_LOG_LEVEL_INFO, "calculate hash\r\n");
		pSecureInfo->HashDataAddr = LoadAddress;
		Status = AlFsbl_Hash(pSecureInfo);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
	}

	/// read hash from boot image and check
	if((pSecureInfo->HashType != OP_HASH_NONE) && (PtHdr->HashDataOffset != 0)) {
		/// read hash from boot image
		//HashByteLen = (pSecureInfo->HashType == OP_HASH_SHA256) ? 32 : 16;
		Status = FsblInstancePtr->DeviceOps.DeviceCopy(
				     ImageOffsetAddress + PtHdr->HashDataOffset,
				     LoadAddress + PtHdr->PartitionLen,
				     32,
				     NULL);
		if(ALFSBL_SUCCESS != Status) {
			goto END;
		}

		/// compare hash
		Status = AlFsbl_CompareHash(
				     HashBuffer,
				     (uint8_t *)(LoadAddress + PtHdr->PartitionLen),
				     32);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
		AL_LOG(AL_LOG_LEVEL_INFO, "Hash check passed...\r\n");
	}

	if(pSecureInfo->AuthType != OP_AUTH_NONE) {
		AL_LOG(AL_LOG_LEVEL_INFO, "Auth\r\n");
		/// copy partition ac to local variate;
		PartitionAcOffset = PtHdr->AcOffset;
		AL_LOG(AL_LOG_LEVEL_INFO, "Partition AC offset: %08x\r\n", PartitionAcOffset);
		if(PartitionAcOffset == 0) {
			AL_LOG(AL_LOG_LEVEL_ERROR, "ALFSBL_ERROR_PARTITION_HEADER_ACOFFSET\r\n");
			Status = ALFSBL_ERROR_PH_ACOFFSET;
			goto END;
		}
		else {
			AL_LOG(AL_LOG_LEVEL_INFO, "Copy Partition Header AC\r\n");
			Status = FsblInstancePtr->DeviceOps.DeviceCopy(
					     ImageOffsetAddress + PartitionAcOffset,
					     (PTRSIZE)PartitionAc,
					     ALFSBL_AUTH_BUFFER_SIZE,
					     NULL);
			if(Status != ALFSBL_SUCCESS) {
				goto END;
			}
		}
		/// initial auth parameters;
		pSecureInfo->PubKeyAddr = (uint32_t)(PartitionAc + ALAC_SPK_OFFSET);
		pSecureInfo->SignatureAddr = (uint32_t)(PartitionAc + ALAC_PART_SIGNATURE_OFFSET);
		Status = AlFsbl_Auth(pSecureInfo);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
		AL_LOG(AL_LOG_LEVEL_INFO, "Authenticat pass\r\n");
	}

	AL_LOG(AL_LOG_LEVEL_INFO, "ps partition copy finished\r\n");

#if 0
	/// check decrypt result:
	ocmptr = (uint32_t *)(LoadAddress);
	for(i = 0; i < 4; i++) {
		AL_LOG(AL_LOG_LEVEL_INFO, "%08x\r\n", *ocmptr);
		ocmptr++;
	}
#endif

	/// update handoff values
	if((PtHdr->DestExecAddr) != 0xFFFFFFFFU) {
		AL_LOG(AL_LOG_LEVEL_INFO, "Update handoff values\r\n");
		HandoffNum = FsblInstancePtr->HandoffCpuNum;
		FsblInstancePtr->HandoffValues[HandoffNum].CpuSettings = DestCpu;
		FsblInstancePtr->HandoffValues[HandoffNum].HandoffAddress = PtHdr->DestExecAddr;
		FsblInstancePtr->HandoffCpuNum += 1;
	}

END:
	return Status;
}



static uint32_t AlFsbl_LoadPlPartition(AlFsblInfo *FsblInstancePtr, SecureInfo *pSecureInfo, uint32_t PartitionIdx)
{

	uint32_t i;
	uint32_t Status = 0;
	uint8_t  HashBuffer[32];
	uint8_t  HashData[32];
	uint32_t HashByteLen;
	uint32_t PartitionAcOffset;
	uint8_t  PartitionAc[ALFSBL_AUTH_BUFFER_SIZE];
	uint32_t ImageOffsetAddress;
	uint32_t BootDevice;
	uint32_t BlockSizeMax;
	AlFsbl_PartitionHeader *PtHdr;
	PtHdr = &(FsblInstancePtr->ImageHeader.PartitionHeader[PartitionIdx]);
	ImageOffsetAddress = FsblInstancePtr->ImageOffsetAddress;

	/// pcap reset
	AL_LOG(AL_LOG_LEVEL_INFO, "PCAP RESET\r\n");
	AL_REG32_WRITE(CSU_PCAP_RESET, 0);
	AL_REG32_WRITE(CSU_PCAP_RESET, 1);

	AL_LOG(AL_LOG_LEVEL_INFO, "Re-enable PCAP\r\n");
	AL_REG32_WRITE(CSU_PCAP_ENABLE, 0);
	AL_REG32_WRITE(CSU_PCAP_ENABLE, 1);

	// check pl init done;
	Status = AlFsbl_CheckPlInitDone();
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	BlockSizeMax = FsblInstancePtr->DeviceOps.BlockSizeMax;
	AL_LOG(AL_LOG_LEVEL_INFO, "Boot device block size max: %x\r\n", BlockSizeMax);

	/// temp test: pl init and done:
	AL_LOG(AL_LOG_LEVEL_INFO, "PL init done\r\n");
	AL_LOG(AL_LOG_LEVEL_INFO, "cfg state: %08x\r\n", AL_REG32_READ(CRP_CFG_STATE));

	pSecureInfo->HashOutAddr    = (uint32_t)HashBuffer;
	pSecureInfo->KeyMode        = OP_BHDR_KEY;
	pSecureInfo->EncMode        = SYM_ECB;
	pSecureInfo->EncDir         = SYM_DECRYPT;
	//pSecureInfo->CsuAddrIncMode = CSUDMA_DST_INCR | CSUDMA_SRC_INCR;
	pSecureInfo->CsuAddrIncMode = CSUDMA_DST_NOINCR | CSUDMA_SRC_INCR;

	/// check secure info
	AL_LOG(AL_LOG_LEVEL_INFO, "Auth type :  %02x\r\n", pSecureInfo->AuthType);
	AL_LOG(AL_LOG_LEVEL_INFO, "Hash type :  %02x\r\n", pSecureInfo->HashType);
	AL_LOG(AL_LOG_LEVEL_INFO, "Enc type  :  %02x\r\n", pSecureInfo->EncType);
	AL_LOG(AL_LOG_LEVEL_INFO, "Enc mode  :  %02x\r\n", pSecureInfo->EncMode);
	AL_LOG(AL_LOG_LEVEL_INFO, "Key mode  :  %02x\r\n", pSecureInfo->KeyMode);

	BootDevice = FsblInstancePtr->PrimaryBootDevice;

	/// if hash enabled and data is not encrypted, read data first then do hash check
	/// if boot device is not xip-qspi, read data first then decrypt and hash check

	if(((pSecureInfo->HashType != OP_HASH_NONE) && (pSecureInfo->EncType == OP_ENCRYPT_NONE)) ||
	   ((BootDevice != ALFSBL_BOOTMODE_QSPI24) && (BootDevice != ALFSBL_BOOTMODE_QSPI32))) {
#ifdef DDR_AVAILABLE
		AL_LOG(AL_LOG_LEVEL_INFO, "to ddr in a whole block, then to pcap\r\n");
		Status = AlFsbl_BitstreamDataTransfer(FsblInstancePtr, pSecureInfo, PartitionIdx, (uint32_t)(0x100000), 0);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
#else
		AL_LOG(AL_LOG_LEVEL_INFO, "to ocm in blocks, Block size: %u\r\n", BlockSizeMax);
		Status = AlFsbl_BitstreamDataTransfer(FsblInstancePtr, pSecureInfo, PartitionIdx, (uint32_t)ReadBuffer, BlockSizeMax);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
#endif
	}
	else {
#ifdef QSPI_XIP_THROUTH_CSU_DMA
		AL_LOG(AL_LOG_LEVEL_INFO, "to pcap, whole block\r\n");
		Status = AlFsbl_BitstreamDataTransfer(FsblInstancePtr, pSecureInfo, PartitionIdx, CSU_PCAP_CSULOCAL_WR_STREAM, 0);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
#elif (defined DDR_AVAILABLE)
		AL_LOG(AL_LOG_LEVEL_INFO, "to ddr in a whole block, then to pcap\r\n");
		Status = AlFsbl_BitstreamDataTransfer(FsblInstancePtr, pSecureInfo, PartitionIdx, (uint32_t)(0x100000), 0);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
#else
		AL_LOG(AL_LOG_LEVEL_INFO, "to ocm in blocks, Block size: %u\r\n", BlockSizeMax);
		Status = AlFsbl_BitstreamDataTransfer(FsblInstancePtr, pSecureInfo, PartitionIdx, (uint32_t)ReadBuffer, BlockSizeMax);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
#endif
	}

	/// read hash from boot image and check
	if((pSecureInfo->HashType != OP_HASH_NONE) && (PtHdr->HashDataOffset != 0)) {
		/// read hash from boot image
		AL_LOG(AL_LOG_LEVEL_INFO, "read hash\r\n");
		//HashByteLen = (pSecureInfo->HashType == OP_HASH_SHA256) ? 32 : 16;
		Status = FsblInstancePtr->DeviceOps.DeviceCopy(
				     ImageOffsetAddress + PtHdr->HashDataOffset,
					 (uint32_t)HashData,
				     32,
				     NULL);
		if(ALFSBL_SUCCESS != Status) {
			goto END;
		}

		/// compare hash
		Status = AlFsbl_CompareHash(
				     HashBuffer,
					 HashData,
				     32);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
		AL_LOG(AL_LOG_LEVEL_INFO, "Hash check passed...\r\n");
	}

	/// auth check if necessary
	if(pSecureInfo->AuthType != OP_AUTH_NONE) {
		AL_LOG(AL_LOG_LEVEL_INFO, "auth\r\n");
		/// copy partition ac to local variate;
		PartitionAcOffset = PtHdr->AcOffset;
		AL_LOG(AL_LOG_LEVEL_INFO, "Partition AC offset: %08x\r\n", PartitionAcOffset);
		if(PartitionAcOffset == 0) {
			AL_LOG(AL_LOG_LEVEL_ERROR, "ALFSBL_ERROR_PARTITION_HEADER_ACOFFSET\r\n");
			Status = ALFSBL_ERROR_PH_ACOFFSET;
			goto END;
		}
		else {
			AL_LOG(AL_LOG_LEVEL_INFO, "Copy Partition Header AC\r\n");
			Status = FsblInstancePtr->DeviceOps.DeviceCopy(
					     ImageOffsetAddress + PartitionAcOffset,
					     (PTRSIZE)PartitionAc,
					     ALFSBL_AUTH_BUFFER_SIZE,
					     NULL);
			if(Status != ALFSBL_SUCCESS) {
				goto END;
			}
		}
		/// initial auth parameters;
		pSecureInfo->PubKeyAddr = (uint32_t)(PartitionAc + ALAC_SPK_OFFSET);
		pSecureInfo->SignatureAddr = (uint32_t)(PartitionAc + ALAC_PART_SIGNATURE_OFFSET);
		Status = AlFsbl_Auth(pSecureInfo);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
		AL_LOG(AL_LOG_LEVEL_INFO, "Authenticate pass\r\n");
	}

	/// temp test: pl init and done:
	AL_LOG(AL_LOG_LEVEL_INFO, "cfg state before progdone: %08x\r\n", AL_REG32_READ(CRP_CFG_STATE));

	/// program_done
	Status = ALFsbl_BitStreamProgDone();
	if(ALFSBL_SUCCESS != Status) {
		goto END;
	}

	/// temp test: pl init and done:
	AL_LOG(AL_LOG_LEVEL_INFO, "cfg state after progdone: %08x\r\n", AL_REG32_READ(CRP_CFG_STATE));

	/// check cfg state after bitstream loaded
	if((AL_REG32_READ(CRP_CFG_STATE)) != 7) {
		Status = ALFSBL_ERROR_PL_CFG_STATE_ERROR & (AL_REG32_READ(CRP_CFG_STATE) | 0xFFFFFFF8UL);
		goto END;
	}

	/// set PCAP not enable, to make the signal to config model not change
	AL_REG32_WRITE(CSU_PCAP_ENABLE, 0);

END:
	if(Status != ALFSBL_SUCCESS) {
		/// if bitstream load fail, clean the loaded pl logic to protect chips
		AL_REG32_SET_BIT(SYSCTRL_S_GLOBAL_SRSTN, 8, 0);
		AL_REG32_SET_BIT(SYSCTRL_S_GLOBAL_SRSTN, 8, 1);
	}
	return Status;
}




static uint32_t AlFsbl_BitstreamDataTransfer(AlFsblInfo *FsblInstancePtr, SecureInfo *pSecureInfo,
		                                     uint32_t PartitionIdx, uint32_t DestAddr,
										     uint32_t BlockSizeMax)
{
	uint32_t Status = 0;
	AlFsbl_PartitionHeader *PtHdr;
	uint32_t ImageOffsetAddress;
	uint32_t SrcAddress;
	uint32_t Length;
	uint32_t BlockLength;
	uint32_t BlockSrcAddr;
	uint32_t BlockDestAddr;
	uint32_t BlockCnt = 0;

	PtHdr = &(FsblInstancePtr->ImageHeader.PartitionHeader[PartitionIdx]);
	ImageOffsetAddress = FsblInstancePtr->ImageOffsetAddress;
	SrcAddress = FsblInstancePtr->ImageOffsetAddress + PtHdr->PartitionOffset;
	Length = PtHdr->PartitionLen;

	AL_LOG(AL_LOG_LEVEL_INFO, "partition src address      : 0x%08x\r\n", SrcAddress);
	AL_LOG(AL_LOG_LEVEL_INFO, "partition load dest address: 0x%08x\r\n", DestAddr);
	AL_LOG(AL_LOG_LEVEL_INFO, "partition length           : 0x%08x\r\n", Length);


	BlockSrcAddr  = SrcAddress;
	BlockDestAddr = DestAddr;
	if(BlockSizeMax == 0) {
		pSecureInfo->BlockMode = WHOLE_BLOCK;
		AL_LOG(AL_LOG_LEVEL_INFO, "whole block\r\n");
		BlockLength = Length;
	}
	else if(Length > BlockSizeMax) {
		pSecureInfo->BlockMode = FIRST_BLOCK;
		AL_LOG(AL_LOG_LEVEL_INFO, "first block\r\n");
		BlockLength = BlockSizeMax;
	}
	else {
		pSecureInfo->BlockMode = WHOLE_BLOCK;
		AL_LOG(AL_LOG_LEVEL_INFO, "whole block\r\n");
		BlockLength = Length;
	}

	while(Length != 0) {
		/// data transfer
		if(((BlockCnt + 1) % 100) == 0) {
			AL_LOG(AL_LOG_LEVEL_DEBUG, "Blk num: %u\r\n", BlockCnt);
		}

		if(DestAddr == CSU_PCAP_CSULOCAL_WR_STREAM) {
			/// transfer to pcap, only when boot device is xip-qspi and not hash only
			Status = FsblInstancePtr->DeviceOps.DeviceCopy(
					     BlockSrcAddr,
						 BlockDestAddr,
						 BlockLength,
						 pSecureInfo);
			if(Status != ALFSBL_SUCCESS) {
				goto END;
			}
		}
		else {
			/// transfer to a buffer, if boot device is none-xip-qspi, sd/emmc, nand, or xip-qspi with hash only
			/// step 1: transfer to buffer,
			Status = FsblInstancePtr->DeviceOps.DeviceCopy(
					     BlockSrcAddr,
						 BlockDestAddr,
						 BlockLength,
						 NULL);
			if(Status != ALFSBL_SUCCESS) {
				goto END;
			}

			/// step 2: transfer to pcap
			/// if enc enabled, do decrypt
			pSecureInfo->HashDataAddr = BlockDestAddr;
			pSecureInfo->InputAddr    = BlockDestAddr;
			pSecureInfo->OutputAddr   = CSU_PCAP_CSULOCAL_WR_STREAM;
			pSecureInfo->DataLength   = BlockLength;
			Status = AlFsbl_DecHash(pSecureInfo);

			/// step 3: if hash enabled without enc, calculate hash
			if((pSecureInfo->EncType == OP_ENCRYPT_NONE) &&
			   (pSecureInfo->HashType != OP_HASH_NONE)) {
				AL_LOG(AL_LOG_LEVEL_INFO, "calculate hash\r\n");
				pSecureInfo->HashDataAddr = BlockDestAddr;
				Status = AlFsbl_Hash(pSecureInfo);
				if(Status != ALFSBL_SUCCESS) {
					goto END;
				}
			}
		}

		if(BlockSizeMax == 0)
			goto END;

		BlockSrcAddr  += BlockLength;
		Length        -= BlockLength;

		pSecureInfo->BlockMode = (Length > BlockSizeMax) ? MIDDLE_BLOCK : LAST_BLOCK;
		BlockLength = (Length > BlockSizeMax) ? BlockSizeMax : Length;
		BlockCnt++;
	}

END:
	return Status;
}



uint32_t AlFsbl_CheckPlInitDone(void)
{
	uint32_t Status = ALFSBL_SUCCESS;
	volatile uint64_t StartTime;
	volatile uint64_t CurrTime;
	uint64_t TimerFreq;
	uint32_t InitDone;
	uint32_t cnt;

	TimerFreq = AlSys_GetTimerFreq();
	StartTime = AlSys_GetTimerTickCount();

	do {
		CurrTime =  AlSys_GetTimerTickCount();
		if((CurrTime - StartTime) > TimerFreq * INIT_DONE_WAIT_SECS) {
			Status = ALFSBL_ERROR_PL_INIT_TIMEOUT;
			goto END;
		}
		InitDone = AL_REG32_READ(CRP_CFG_STATE) & CRP_CFG_STATE_MSK_PL2PS_INITN;

	}while(InitDone != CRP_CFG_STATE_MSK_PL2PS_INITN);

END:
	return Status;

}


uint32_t ALFsbl_BitStreamProgDone(void)
{
	uint32_t Status;
	uint32_t ProgDone[3] = {0x90300002, 0x00000005, 0x1655e833};
	uint32_t BitStreamNoop = 0x80000000;

	AL_REG32_WRITE(CSU_PCAP_WR_STREAM, ProgDone[0]);
	AL_REG32_WRITE(CSU_PCAP_WR_STREAM, ProgDone[1]);
	AL_REG32_WRITE(CSU_PCAP_WR_STREAM, ProgDone[2]);

	Status = AlFsbl_CsuDmaCopy(
			     (uint32_t)(&BitStreamNoop),
				 CSU_PCAP_CSULOCAL_WR_STREAM,
				 8192,   /// 2048 words of noop
				 (CSUDMA_DST_NOINCR | CSUDMA_SRC_NOINCR));

	return Status;
}



void AlFsbl_PrintPartitionHeaderInfo(AlFsbl_PartitionHeader *PtHdr)
{
	AL_LOG(AL_LOG_LEVEL_INFO, "\nPartition Header Infomation:\r\n");
	AL_LOG(AL_LOG_LEVEL_INFO, "Partition Length            : 0x%08x\r\n", PtHdr->PartitionLen);
	AL_LOG(AL_LOG_LEVEL_INFO, "Extracted Partition Length  : 0x%08x\r\n", PtHdr->ExtractedPartitionLen);
	AL_LOG(AL_LOG_LEVEL_INFO, "Total Partition Length      : 0x%08x\r\n", PtHdr->TotalPartitionLen);
	AL_LOG(AL_LOG_LEVEL_INFO, "Next Partition Header Offset: 0x%08x\r\n", PtHdr->NextPartHdrOffset);
	AL_LOG(AL_LOG_LEVEL_INFO, "Dest Execution Address      : 0x%lx\r\n", PtHdr->DestExecAddr);
	AL_LOG(AL_LOG_LEVEL_INFO, "Dest Load Address           : 0x%lx\r\n", PtHdr->DestLoadAddr);
	AL_LOG(AL_LOG_LEVEL_INFO, "Partition Offset            : 0x%08x\r\n", PtHdr->PartitionOffset);
	AL_LOG(AL_LOG_LEVEL_INFO, "Partition Attribute         : 0x%08x\r\n", PtHdr->PartitionAttribute);
	AL_LOG(AL_LOG_LEVEL_INFO, "Hash Data offset            : 0x%08x\r\n", PtHdr->HashDataOffset);
	AL_LOG(AL_LOG_LEVEL_INFO, "AC Offset                   : 0x%08x\r\n", PtHdr->AcOffset);
	AL_LOG(AL_LOG_LEVEL_INFO, "Partition Header Checksum   : 0x%08x\r\n", PtHdr->PartiHdrChecksum);
	AL_LOG(AL_LOG_LEVEL_INFO, "\r\n");

	return;
}

