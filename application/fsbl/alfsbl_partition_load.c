/*
 * alfsbl_partition_load.c
 *
 *  Created on: May 18, 2022
 *      Author: qsxu
 */
#include <stdio.h>
#include <stdint.h>
#include "alfsbl_misc.h"
#include "alfsbl_sec.h"
#include "alfsbl_err_code.h"
#include "alfsbl_hw.h"
#include "alfsbl_data.h"
#include "alfsbl_partition_load.h"

static uint32_t AlFsbl_PartitionHeaderValidation(AlFsblInfo *FsblInstancePtr, uint32_t PartitionIdx, SecureInfo *pSecureInfo);
static uint32_t AlFsbl_LoadPsPartition(AlFsblInfo *FsblInstancePtr, SecureInfo *pSecureInfo, uint32_t PartitionIdx);
static uint32_t AlFsbl_LoadPlPartition(AlFsblInfo *FsblInstancePtr, uint32_t PartitionIdx);
static uint32_t AlFsbl_CheckPlInitDone(void);
static void     AlFsbl_PrintPartitionHeaderInfo(AlFsbl_PartitionHeader *PtHdr);

extern SecureInfo FsblSecInfo;
extern uint8_t  AuthBuffer[ALFSBL_AUTH_BUFFER_SIZE];

uint32_t AlFsbl_PartitionLoad(AlFsblInfo *FsblInstancePtr, uint32_t PartitionIdx)
{
	uint32_t Status;
	uint32_t DestDev;

	/// todo: restart wdt

	/// release pl reset
	if(FsblInstancePtr->ResetReason == FSBL_SYSTEM_RESET) {
		REG32(SYSCTRL_S_GLOBAL_SRSTN) = REG32(SYSCTRL_S_GLOBAL_SRSTN) | (SYSCTRL_S_GLOBAL_SRSTN_MSK_GLB_PL_SRST);
	}

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
		printf("loading ps partition...\n");
		Status = AlFsbl_LoadPsPartition(FsblInstancePtr, &FsblSecInfo, PartitionIdx);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
	}
	else if(DestDev == ALIH_PH_ATTRIB_DEST_DEV_PL) {
		if(FsblInstancePtr->ResetReason == FSBL_PS_ONLY_RESET) {
			printf("ps only reset, no need to load pl partition");
			Status = ALFSBL_SUCCESS;
			goto END;
		}
		printf("loading pl partition...\n");
		AlFsbl_LoadPlPartition(FsblInstancePtr, PartitionIdx);
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

	EfuseCtrl = REG32(EFUSE_SEC_CTRL);
	PtHdr = &(FsblInstancePtr->ImageHeader.PartitionHeader[PartitionIdx]);
	PartitionAttr = PtHdr->PartitionAttribute;

	/// check checksum of partition header
	printf("Partition Header Checksum: 0x%08x\n", PtHdr->PartiHdrChecksum);
	Status = AlFsbl_ChecksumCheck(
				(uint8_t *)(PtHdr),
				sizeof(AlFsbl_PartitionHeader) - 4,   /// the last word in PartitionHeader is checksum
				PtHdr->PartiHdrChecksum);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	/// check partition owner
	if((PartitionAttr & ALIH_PH_ATTRIB_PART_OWNER_MASK) != ALIH_PH_ATTRIB_PART_OWNER_FSBL) {
		printf("Partition owner: %08x\n", PtHdr->PartitionAttribute & ALIH_PH_ATTRIB_PART_OWNER_MASK);
		printf("Partition owner not fsbl, skip it, partition num: %d\n", PartitionIdx);
		Status = ALFSBL_SUCCESS_NOT_PARTITION_OWNER;
		goto END;
	}


	/// check hash type
	PtAttrHashType = PartitionAttr & ALIH_PH_ATTRIB_HASH_TYPE_MASK;
	printf("Hash Type: 0x%08x\n", PtAttrHashType);
	if(PtAttrHashType == ALIH_PH_ATTRIB_HASH_TYPE_NONE) {
		pSecureInfo->HashType = OP_HASH_NONE;
		printf("Hash NOT enabled.\n");
	}
	else if(PtAttrHashType == ALIH_PH_ATTRIB_HASH_TYPE_SHA256) {
		pSecureInfo->HashType = OP_HASH_SHA256;
		printf("Hash SHA256.\n");
	}
	else if(PtAttrHashType == ALIH_PH_ATTRIB_HASH_TYPE_SM3) {
		pSecureInfo->HashType = OP_HASH_SM3;
		printf("Hash SM3.\n");
	}
	else {
		printf("Invalid Hash Type\n");
		Status = ALFSBL_INVALID_HASH_TYPE;
		goto END;
	}

	/// check authentication type
	PtAttrAuthType = PartitionAttr & ALIH_PH_ATTRIB_AUTH_TYPE_MASK;
	printf("Auth Type       : %x\n", PtAttrAuthType);
	printf("Efuse Auth Type : %x\n", EfuseCtrl & EFUSE_AUTH_TYPE_MASK);
	if((PtAttrAuthType == ALIH_PH_ATTRIB_AUTH_TYPE_NONE) && ((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) == EFUSE_AUTH_TYPE_HEADER_SET)) {
		pSecureInfo->AuthType = OP_AUTH_NONE;
		printf("Authentication NOT enabled...\n");
	}
	else if((PtAttrAuthType == ALIH_PH_ATTRIB_AUTH_TYPE_ECC256) && ((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) == EFUSE_AUTH_TYPE_ECC256)) {
		pSecureInfo->AuthType = OP_AUTH_ECC256;
		pSecureInfo->HashType = OP_HASH_SHA256;
		printf("Authentication SM2\n");
	}
	else if((PtAttrAuthType == ALIH_PH_ATTRIB_AUTH_TYPE_SM2) && ((EfuseCtrl & EFUSE_AUTH_TYPE_MASK) == EFUSE_AUTH_TYPE_SM2)) {
		pSecureInfo->AuthType = OP_AUTH_SM2;
		pSecureInfo->HashType = OP_HASH_SM3;
		printf("Authentication ECC256\n");
	}
	else {
		printf("Auth type not match efuse set...\n");
		Status = ALFSBL_AUTHTYPE_NOT_MATCH_EFUSE;
		goto END;
	}

	/// check encryption type
	PtAttrEncType  = PartitionAttr & ALIH_PH_ATTRIB_ENC_TYPE_MASK;
	printf("Enc Type       : %x\n", PtAttrEncType);
	printf("Efuse Enc Type : %x\n", EfuseCtrl & EFUSE_ENC_TYPE_MASK);
	if((PtAttrEncType == ALIH_PH_ATTRIB_ENC_TYPE_NONE) && ((EfuseCtrl & EFUSE_ENC_TYPE_MASK) == EFUSE_ENC_TYPE_HEADER_SET)) {
		pSecureInfo->EncType = OP_ENCRYPT_NONE;
		printf("Encrypt NOT enabled.\n");
	}
	else if((PtAttrEncType == ALIH_PH_ATTRIB_ENC_TYPE_AES256) && ((EfuseCtrl & EFUSE_ENC_TYPE_MASK) == EFUSE_ENC_TYPE_AES256)) {
		pSecureInfo->EncType = OP_ENCRYPT_AES256;
		printf("Encrypte AES256.\n");
	}
	else if((PtAttrEncType == ALIH_PH_ATTRIB_ENC_TYPE_SM4) && ((EfuseCtrl & EFUSE_ENC_TYPE_MASK) == EFUSE_ENC_TYPE_SM4)) {
		pSecureInfo->EncType = OP_ENCRYPT_SM4;
		printf("Encrypte SM4.\n");
	}
	else {
		printf("Encrypt type not match efuse set...\n");
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
	printf("destination cpu: 0x%08x\n", DestCpu);
	if(DestCpu != ALIH_PH_ATTRIB_DEST_CPU_RPU &&
	   DestCpu != ALIH_PH_ATTRIB_DEST_CPU_APU0 &&
	   DestCpu != ALIH_PH_ATTRIB_DEST_CPU_APU1) {
		Status = ALFSBL_INVALID_DEST_CPU;
		goto END;
	}

	/// check destination device
	DestDev = PartitionAttr & ALIH_PH_ATTRIB_DEST_DEV_MASK;
	printf("destination dev: 0x%08x\n", DestDev);
	if(DestDev != ALIH_PH_ATTRIB_DEST_DEV_PS &&
	   DestDev != ALIH_PH_ATTRIB_DEST_DEV_PL) {
		Status = ALFSBL_INVALID_DEST_DEV;
		goto END;
	}

	/// check partion length
	printf("check partition length: \n");
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
	if(AL9000_DDR1_BASE_ADDR <= PtHdr->DestLoadAddr && PtHdr->DestLoadAddr < AL9000_DDR1_BASE_ADDR + AL9000_DDR1_BYTE_LENGTH) {
		// correct, do nothing
	}
	else if(AL9000_DDR2_BASE_ADDR <= PtHdr->DestLoadAddr && PtHdr->DestLoadAddr < AL9000_DDR2_BASE_ADDR + AL9000_DDR2_BYTE_LENGTH) {
		// correct, do nothing
	}
	else if(AL9000_RPU_ITCM_BASE_ADDR <= PtHdr->DestLoadAddr && PtHdr->DestLoadAddr < AL9000_RPU_ITCM_BASE_ADDR + AL9000_RPU_ITCM_BYTE_LENGTH) {
		// correct, do nothing
	}
	else if(AL9000_RPU_DTCM_BASE_ADDR <= PtHdr->DestLoadAddr && PtHdr->DestLoadAddr < AL9000_RPU_DTCM_BASE_ADDR + AL9000_RPU_ITCM_BYTE_LENGTH) {
		// correct, do nothing
	}
	else {
		Status = ALFSBL_INVALID_LOAD_ADDR;
	}

	if(AL9000_DDR1_BASE_ADDR <= PtHdr->DestExecAddr && PtHdr->DestExecAddr < AL9000_DDR1_BASE_ADDR + AL9000_DDR1_BYTE_LENGTH) {
		// correct, do nothing
	}
	else if(AL9000_DDR2_BASE_ADDR <= PtHdr->DestExecAddr && PtHdr->DestExecAddr < AL9000_DDR2_BASE_ADDR + AL9000_DDR2_BYTE_LENGTH) {
		// correct, do nothing
	}
	else if(AL9000_RPU_ITCM_BASE_ADDR <= PtHdr->DestExecAddr && PtHdr->DestExecAddr < AL9000_RPU_ITCM_BASE_ADDR + AL9000_RPU_ITCM_BYTE_LENGTH) {
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
	Length = PtHdr->TotalPartitionLen;

	printf("partition src address      : 0x%08x\n", SrcAddress);
	printf("partition load dest address: 0x%08x\n", LoadAddress);
	printf("partition length           : 0x%08x\n", Length);

	Status = FsblInstancePtr->DeviceOps.DeviceCopy(SrcAddress, LoadAddress, Length);
	if(ALFSBL_SUCCESS != Status) {
		goto END;
	}

	pSecureInfo->InputAddr   = LoadAddress;
	pSecureInfo->OutputAddr  = LoadAddress;
	pSecureInfo->DataLength  = PtHdr->PartitionLen;
	pSecureInfo->HashOutAddr = (uint32_t)HashBuffer;
	pSecureInfo->KeyMode     = OP_BHDR_KEY;
	pSecureInfo->EncMode     = SYM_ECB;
	pSecureInfo->EncDir      = SYM_DECRYPT;

	/// check secure info
	printf("Auth type :  %02x\n", pSecureInfo->AuthType);
	printf("Hash type :  %02x\n", pSecureInfo->HashType);
	printf("Enc type  :  %02x\n", pSecureInfo->EncType);
	printf("Enc mode  :  %02x\n", pSecureInfo->EncMode);
	printf("Key mode  :  %02x\n", pSecureInfo->KeyMode);


	if(pSecureInfo->EncType != OP_ENCRYPT_NONE) {
		printf("decryption\n");
		Status = AlFsbl_DecHash_1(pSecureInfo);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
	}
	else if(pSecureInfo->HashType != OP_HASH_NONE) {
		printf("hash\n");
		Status = AlFsbl_Hash_1(pSecureInfo);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
//		if(PtHdr->HashDataOffset != 0) {
//			HashByteLen = (pSecureInfo->HashType == OP_HASH_SHA256) ? 32 : 16;
//			Status = AlFsbl_CompareHash(HashBuffer, (uint8_t *)(LoadAddress + PtHdr->PartitionLen), HashByteLen);
//			if(Status != ALFSBL_SUCCESS) {
//				goto END;
//			}
//		}
	}
	if((pSecureInfo->HashType != OP_HASH_NONE) && (PtHdr->HashDataOffset != 0)) {
		HashByteLen = (pSecureInfo->HashType == OP_HASH_SHA256) ? 32 : 16;
		Status = AlFsbl_CompareHash(HashBuffer, (uint8_t *)(LoadAddress + PtHdr->PartitionLen), HashByteLen);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
		printf("Hash check passed...\n");
	}


	if(pSecureInfo->AuthType != OP_AUTH_NONE) {
		printf("auth\n");
		/// copy partition ac to local variate;
		PartitionAcOffset = PtHdr->AcOffset;
		printf("Partition AC offset: %08x\n", PartitionAcOffset);
		if(PartitionAcOffset == 0) {
			printf("ALFSBL_ERROR_PARTITION_HEADER_ACOFFSET\n");
			Status = ALFSBL_ERROR_PH_ACOFFSET;
			goto END;
		}
		else {
			printf("Copy Partition Header AC\n");
			Status = FsblInstancePtr->DeviceOps.DeviceCopy(
					ImageOffsetAddress+PartitionAcOffset,
					(PTRSIZE)PartitionAc,
					ALFSBL_AUTH_BUFFER_SIZE);
			if(Status != ALFSBL_SUCCESS) {
				goto END;
			}
		}
		/// initial auth parameters;
		pSecureInfo->PubKeyAddr = (uint32_t)(PartitionAc + ALAC_SPK_OFFSET);
		pSecureInfo->SignatureAddr = (uint32_t)(PartitionAc + ALAC_PART_SIGNATURE_OFFSET);
		Status = AlFsbl_Auth_1(pSecureInfo);
		if(Status != ALFSBL_SUCCESS) {
			goto END;
		}
	}

	/// check decrypt result:
	ocmptr = (uint32_t *)(LoadAddress);
	for(i = 0; i < 4; i++) {
		printf("%08x\n", *ocmptr);
		ocmptr++;
	}
	printf("\n\nUpdate handoff values\n");


	/// update handoff values
	if(LoadAddress != 0xFFFFFFFFU) {
		HandoffNum = FsblInstancePtr->HandoffCpuNum;
		FsblInstancePtr->HandoffValues[HandoffNum].CpuSettings = DestCpu;
		FsblInstancePtr->HandoffValues[HandoffNum].HandoffAddress = PtHdr->DestExecAddr;
		FsblInstancePtr->HandoffCpuNum += 1;
	}

END:
	return Status;
}




static uint32_t AlFsbl_LoadPlPartition(AlFsblInfo *FsblInstancePtr, uint32_t PartitionIdx)
{
	uint32_t Status = 0;

	AlFsbl_PartitionHeader *PtHdr;

	uint32_t SrcAddress;
	uint32_t LoadAddress;
	uint32_t Length;

	PtHdr = &(FsblInstancePtr->ImageHeader.PartitionHeader[PartitionIdx]);

	SrcAddress = FsblInstancePtr->ImageOffsetAddress + PtHdr->PartitionOffset;
	LoadAddress = PtHdr->DestLoadAddr;
	Length = PtHdr->TotalPartitionLen;

	printf("partition src address      : 0x%08x\n", SrcAddress);
	printf("partition load dest address: 0x%08x\n", LoadAddress);
	printf("partition length           : 0x%08x\n", Length);

/*
	/// check pl init done
	Status = AlFsbl_CheckPlInitDone();

	/// enable pcap, to remove pcap-pl isolation
	REG32(CSU_PCAP_ENABLE) = 1;
*/


END:
	return Status;
}

#define  RPU_MTIMER_COUNTER64     (0x68020000)
#define  RPU_MTIMER_COUNTER64_LOW (0x68020000)
#define  RPU_MTIMER_COUNTER64_HI  (0x68020004)

uint32_t AlFsbl_CheckPlInitDone(void)
{
	uint32_t Status = ALFSBL_SUCCESS;
	volatile uint32_t StartTime;
	uint32_t InitDone;

	StartTime = REG32(RPU_MTIMER_COUNTER64_LOW);

	do {
		InitDone = REG32(CRP_CFG_STATE) & CRP_CFG_STATE_MSK_PL2PS_INITN;

		/// if pl init done not asserted in 90 seconds, report an error
		if((REG32(RPU_MTIMER_COUNTER64_LOW) - StartTime ) > (90 * 10 * 1000 * 1000)) {
			Status = ALFSBL_ERROR_PL_INIT_TIMEOUT;
			goto END;
		}

	} while(InitDone != CRP_CFG_STATE_MSK_PL2PS_INITN);

END:
	return Status;

}


void AlFsbl_PrintPartitionHeaderInfo(AlFsbl_PartitionHeader *PtHdr)
{
	printf("\nPartition Header Infomation:\n");
	printf("Partition Length            : 0x%08x\n", PtHdr->PartitionLen);
	printf("Extracted Partition Length  : 0x%08x\n", PtHdr->ExtractedPartitionLen);
	printf("Total Partition Length      : 0x%08x\n", PtHdr->TotalPartitionLen);
	printf("Next Partition Header Offset: 0x%08x\n", PtHdr->NextPartHdrOffset);
	printf("Dest Execution Address      : 0x%08x\n", PtHdr->DestExecAddr);
	printf("Dest Load Address           : 0x%08x\n", PtHdr->DestLoadAddr);
	printf("Partition Offset            : 0x%08x\n", PtHdr->PartitionOffset);
	printf("Partition Attribute         : 0x%08x\n", PtHdr->PartitionAttribute);
	printf("Hash Data offset            : 0x%08x\n", PtHdr->HashDataOffset);
	printf("AC Offset                   : 0x%08x\n", PtHdr->AcOffset);
	printf("Partition Header Checksum   : 0x%08x\n", PtHdr->PartiHdrChecksum);
	putchar('\n');

	return;
}

