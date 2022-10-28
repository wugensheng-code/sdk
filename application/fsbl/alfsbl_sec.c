/*
 * alfsbl_sec.c
 *
 *  Created on: May 25, 2022
 *      Author: qsxu
 */

#include <stdio.h>

#include "alfsbl_err_code.h"
#include "alfsbl_data.h"
#include "alfsbl_sec.h"
#include "alfsbl_hw.h"
#include "alfsbl_misc.h"
#include "demosoc.h"
#ifndef __riscv
#include "gic_v3_addr.h"
#endif

//extern uint8_t  ReadBuffer[READ_BUFFER_SIZE];
extern uint8_t  AuthBuffer[ALFSBL_AUTH_BUFFER_SIZE];

/// for temporary test only, will remove later, xuqingsong

//uint8_t aes256_key0[] = {
//		0x7B,0xCD,0xD8,0xD1,
//		0x91,0x6E,0xA8,0x38,
//		0xBE,0x2F,0x41,0x0D,
//		0x7E,0xC4,0x59,0xC6,
//		0xF9,0x52,0x8F,0xCF,
//		0xD9,0x1B,0x22,0x10,
//		0x03,0x5D,0xB4,0xE3,
//		0x78,0xD6,0x09,0x55};

uint8_t aes256_key0[] = {
		0x78, 0xd6, 0x09, 0x55,
		0x03, 0x5d, 0xb4, 0xe3,
		0xd9, 0x1b, 0x22, 0x10,
		0xf9, 0x52, 0x8f, 0xcf,
		0x7e, 0xc4, 0x59, 0xc6,
		0xbe, 0x2f, 0x41, 0x0d,
		0x91, 0x6e, 0xa8, 0x38,
		0x7b, 0xcd, 0xd8, 0xd1};



//uint8_t aes256_key0[] = {0xD1,0xD8,0xCD,0x7B, 0x38,0xA8,0x6E,0x91,
//		                 0x0D,0x41,0x2F,0xBE, 0xC6,0x59,0xC4,0x7E,
//						 0xCF,0x8F,0x52,0xF9, 0x10,0x22,0x1B,0xD9,
//						 0xE3,0xB4,0x5D,0x03, 0x55,0x09,0xD6,0x78};

uint8_t aes256_key1[] = {0x51,0x37,0xDC,0x62, 0x09,0xF9,0xBA,0x00,
		                 0xD7,0x03,0x98,0xBC, 0xEF,0xB9,0x1D,0x58,
						 0xDD,0x9A,0xE1,0xE7, 0x5F,0x1A,0x0A,0xCB,
						 0x23,0x51,0xCF,0xD5, 0xC0,0x03,0x40,0x54};

uint8_t sm4_key0[]    = {0x55,0x09,0xD6,0x78, 0xE3,0xB4,0x5D,0x03,
		                 0x10,0x22,0x1B,0xD9, 0xCF,0x8F,0x52,0xF9};

uint8_t sm4_key1[]    = {0x51,0x37,0xDC,0x62, 0x09,0xF9,0xBA,0x00,
		                 0xD7,0x03,0x98,0xBC, 0xEF,0xB9,0x1D,0x58};

//uint8_t aes256_key0[] = {0x55,0x09,0xD6,0x78, 0xE3,0xB4,0x5D,0x03,
//		                 0x10,0x22,0x1B,0xD9, 0xCF,0x8F,0x52,0xF9,
//						 0xC6,0x59,0xC4,0x7E, 0x0D,0x41,0x2F,0xBE,
//						 0x38,0xA8,0x6E,0x91, 0xD1,0xD8,0xCD,0x7B};
//
//uint8_t aes256_key1[] = {0x51,0x37,0xDC,0x62, 0x09,0xF9,0xBA,0x00,
//		                 0xD7,0x03,0x98,0xBC, 0xEF,0xB9,0x1D,0x58,
//						 0xDD,0x9A,0xE1,0xE7, 0x5F,0x1A,0x0A,0xCB,
//						 0x23,0x51,0xCF,0xD5, 0xC0,0x03,0x40,0x54};
//
//uint8_t sm4_key0[]    = {0x55,0x09,0xD6,0x78, 0xE3,0xB4,0x5D,0x03,
//		                 0x10,0x22,0x1B,0xD9, 0xCF,0x8F,0x52,0xF9};
//
//uint8_t sm4_key1[]    = {0x51,0x37,0xDC,0x62, 0x09,0xF9,0xBA,0x00,
//		                 0xD7,0x03,0x98,0xBC, 0xEF,0xB9,0x1D,0x58};
//SecMsgDef SecCmd;
volatile uint32_t MsgFlag = 0;

uint32_t AlFsbl_CalcCrc32(uint8_t *pBuffer, uint32_t Length)
{
	uint8_t i;
	uint32_t crc = 0xffffffff; // initial value

	while(Length--) {
		crc ^= *pBuffer++;
		for(i = 0; i < 8; i++) {
			if(crc & 1) {
				crc = (crc >> 1) ^ 0xEDB88320;
			}
			else {
				crc = (crc >> 1);
			}
		}
	}

	return (~crc);
}


uint32_t AlFsbl_ChecksumCheck(uint8_t *pBuffer, uint32_t Length, uint32_t Checksum)
{
	uint32_t Status = ALFSBL_SUCCESS;
	uint32_t Cal_Checksum = 0;
	uint32_t Count;

	/// CRC32
	Cal_Checksum = AlFsbl_CalcCrc32(pBuffer, Length);
	printf("Calculated Checksum: 0x%08x\r\n", Cal_Checksum);

	if(Cal_Checksum != Checksum) {
		Status = ALFSBL_ERROR_CHECKSUM_ERROR;
	}
	else {
		printf("checksum check pass...\r\n");
		Status = ALFSBL_SUCCESS;
	}


	return Status;
}



void TriggerSecInterrupt(void)
{
	/// trigger rpu2csu interrupt
	*(volatile uint32_t *)(RPU2CSU_REQ_ADDR) = 1;
	*(volatile uint32_t *)(RPU2CSU_REQ_ADDR) = 0;
	return;
}



void MsgReceive(void)
{
	MsgFlag = 1;
	printf("ack\r\n");
	*(volatile uint32_t *)(RPU2CSU_ACK_ADDR) = 0;
	return;
}



void RpuCsuAckHandler()
{
	MsgReceive();
}



uint32_t SecureIrqInit(void)
{
	uint32_t ret;
#ifndef __riscv
	REG32(GICD_ISENABLER+8) = 0x80000000;
	REG32(GICD_ISENABLER+12) = 0x1;
#endif
	ret = ECLIC_Register_IRQ(
			RPU2CSU_ACK_IRQN,
			ECLIC_NON_VECTOR_INTERRUPT,
			ECLIC_POSTIVE_EDGE_TRIGGER,
			1,
			0,
			RpuCsuAckHandler);
	__enable_irq();
	return ret;
}



uint32_t CheckAckValid(AckDef *pAck)
{
	uint32_t Status = ALFSBL_SUCCESS;
	printf("Check CSU ACK\r\n");
	while(MsgFlag == 0) {
		/// wait for ack interrupt
	}
	MsgFlag = 0;  /// clear flag

	if(pAck->Cmd != CMD_ACK) {
		printf("Invalid ACK Command!\r\n");
		Status = ALFSBL_ERROR_INVALID_CSU_ACK;
		goto END;
	}
	else {
		pAck->Cmd = 0;   /// clear ack
	}

	if(pAck->Option0.Low == OP_DMA_DONE) {
		goto END;
	}
	else if(pAck->Option0.Low == OP_AUTH_PASS) {
		goto END;
	}
	else if(pAck->Option0.Low == OP_AUTH_FAIL) {
		Status = ALFSBL_ERROR_AUTH_FAIL;
	}
	else if(pAck->Option0.Low == OP_HASH_DONE) {
		goto END;
	}
	else {
		printf("Invalid ACK Command!\r\n");
		Status = ALFSBL_ERROR_INVALID_CSU_ACK;
		goto END;
	}


END:
	return Status;
}



uint32_t AlFsbl_Hash_1(SecureInfo *pSecureInfo)
{
	uint32_t Status = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM + 64);
	uint32_t HashByteLen = 0;

	if((pSecureInfo->HashType != OP_HASH_SHA256) &&
	   (pSecureInfo->HashType != OP_HASH_SM3) &&
	   (pSecureInfo->HashType != OP_HASH_NONE)) {
		Status = ALFSBL_ERROR_SEC_PARAM_INVALID;
	}
	if(pSecureInfo->HashType == OP_HASH_NONE) {
		goto END;
	}

	pMsg->Cmd = CMD_HASH;
	pMsg->Option0.Low           = pSecureInfo->HashType;
	pMsg->HashParam.DataInAddr  = pSecureInfo->InputAddr;
	pMsg->HashParam.DataLen     = pSecureInfo->DataLength;
	pMsg->HashParam.HashOutAddr = pSecureInfo->HashOutAddr;
	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
END:
	return Status;
}



uint32_t AlFsbl_Hash(uint32_t DataInAddr, uint32_t DataByteLen, uint32_t HashOutAddr, uint8_t HashType)
{
	uint32_t Status;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM + 64);

	pMsg->Cmd = CMD_HASH;

	//if(EfuseCtrl & EFUSE_PPK_HASH_TYPE_MASK == EFUSE_PPK_HASH_TYPE_SM3) {
	if(HashType == HASH_TYPE_SM3) {
		pMsg->Option0.Low = OP_HASH_SM3;
	}
	//else if(EfuseCtrl & EFUSE_PPK_HASH_TYPE_MASK == EFUSE_PPK_HASH_TYPE_SHA256) {
	else if(HashType == HASH_TYPE_SHA256) {
		pMsg->Option0.Low = OP_HASH_SHA256;
	}

	pMsg->HashParam.DataInAddr  = DataInAddr;
	pMsg->HashParam.DataLen     = DataByteLen;
	pMsg->HashParam.HashOutAddr = HashOutAddr;

	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);

	return Status;
}



uint32_t AlFsbl_CompareHash(uint8_t *pHash1, uint8_t *pHash2, uint32_t HashLen)
{
	uint32_t Idx;
	uint32_t *pHash1_word = (uint32_t *)(pHash1);
	uint32_t *pHash2_word = (uint32_t *)(pHash2);

	for(Idx = 0; Idx < HashLen / 4; Idx++) {
//		printf("%08x %08x\r\n", pHash1_word[Idx], pHash2_word[Idx]);

		if(pHash1_word[Idx] != pHash2_word[Idx]) {
			printf("hash different: %08x, %08x\r\n", pHash1_word[Idx], pHash2_word[Idx]);
			return ALFSBL_HASH_FAIL;
		}
	}
	return ALFSBL_SUCCESS;
}



uint32_t AlFsbl_Auth_1(SecureInfo *pSecureInfo)
{
	uint32_t Status = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM + 64);

	if((pSecureInfo->AuthType != OP_AUTH_ECC256) &&
	   (pSecureInfo->AuthType != OP_AUTH_SM2)) {
		Status = ALFSBL_ERROR_SEC_PARAM_INVALID;
		goto END;
	}

	pMsg->Cmd = CMD_AUTH;
	pMsg->Option0.Low             = pSecureInfo->AuthType;
	pMsg->AuthParam.PubkeyAddr    = pSecureInfo->PubKeyAddr;
	pMsg->AuthParam.SignatureAddr = pSecureInfo->SignatureAddr;
	pMsg->AuthParam.DigestAddr    = pSecureInfo->HashOutAddr;

	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);
END:
	return Status;
}



uint32_t AlFsbl_Auth(uint32_t PubKeyAddr, uint32_t SignatureAddr, uint32_t DigestAddr, uint8_t AuthType)
{
	uint32_t Status;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM + 64);

	pMsg->Cmd = CMD_AUTH;
	if(AuthType == AUTH_TYPE_ECC256) {
		pMsg->Option0.Low = OP_AUTH_ECC256;
	}
	else if(AuthType == AUTH_TYPE_SM2) {
		pMsg->Option0.Low = OP_AUTH_SM2;
	}
	pMsg->AuthParam.PubkeyAddr    = PubKeyAddr;
	pMsg->AuthParam.SignatureAddr = SignatureAddr;
	pMsg->AuthParam.DigestAddr    = DigestAddr;

	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);

	return Status;
}



uint32_t AlFsbl_DecHash_1(SecureInfo *pSecureInfo)
{
	uint32_t Status = ALFSBL_SUCCESS;
	uint32_t HashByteLen = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM + 64);

	if((pSecureInfo->EncType != OP_ENCRYPT_AES256) &&
	   (pSecureInfo->EncType != OP_ENCRYPT_SM4) &&
	   (pSecureInfo->EncType != OP_ENCRYPT_NONE)) {
		Status = ALFSBL_ERROR_SEC_PARAM_INVALID;
		goto END;
	}
	if((pSecureInfo->HashType != OP_HASH_SHA256) &&
	   (pSecureInfo->HashType != OP_HASH_SM3) &&
	   (pSecureInfo->HashType != OP_HASH_NONE)) {
		Status = ALFSBL_ERROR_SEC_PARAM_INVALID;
		goto END;
	}
	if((pSecureInfo->KeyMode != OP_BHDR_KEY) &&
	   (pSecureInfo->KeyMode != OP_USER_KEY)) {
		Status = ALFSBL_ERROR_SEC_PARAM_INVALID;
		goto END;
	}

	pMsg->Cmd = CMD_DMA;
	/// bootrom and bootgen not support this, use userkey instead for temporary test, xuqingsong
	pMsg->DmaParam.KeyAddr     = pSecureInfo->KeyAddr;
	pMsg->DmaParam.IvAddr      = pSecureInfo->IvAddr;
	pMsg->DmaParam.InputAddr   = pSecureInfo->InputAddr;
	pMsg->DmaParam.TotalLength = pSecureInfo->DataLength;
	pMsg->DmaParam.OutputAddr  = pSecureInfo->OutputAddr;
	pMsg->DmaParam.HashOutAddr = pSecureInfo->HashOutAddr;

	pMsg->DmaParam.OpMode.Bits = SYM_256BIT | (pSecureInfo->EncDir) | SYM_ECB;

	pMsg->Option0.Low  = pSecureInfo->EncType;
	pMsg->Option0.High = pSecureInfo->HashType;
	pMsg->Option1.Low  = pSecureInfo->KeyMode;

	if(pSecureInfo->EncType == OP_ENCRYPT_AES256) {
		pMsg->DmaParam.KeyAddr = aes256_key0;
	}
	else if(pSecureInfo->EncType == OP_ENCRYPT_SM4) {
		pMsg->DmaParam.KeyAddr = sm4_key0;
	}


	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);

END:
	return Status;
}



uint32_t AlFsbl_EncHash(SecEncHashIODef *pSecEncHashIOParam)
{
	uint32_t Status = ALFSBL_SUCCESS;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM + 64);

	pMsg->Cmd = CMD_DMA;
	pMsg->DmaParam.KeyAddr     = pSecEncHashIOParam->KeyAddr;
	pMsg->DmaParam.IvAddr      = pSecEncHashIOParam->IvAddr;
	pMsg->DmaParam.InputAddr   = pSecEncHashIOParam->InputAddr;
	pMsg->DmaParam.TotalLength = pSecEncHashIOParam->TotalLength;
	pMsg->DmaParam.OutputAddr  = pSecEncHashIOParam->OutputAddr;
	pMsg->DmaParam.HashOutAddr = pSecEncHashIOParam->HashOutAddr;

	if(pSecEncHashIOParam->EncMethod == OP_ENCRYPT_AES256) {
		pMsg->DmaParam.OpMode.Bits = SYM_256BIT |  SYM_DECRYPT | SYM_ECB;
		pMsg->Option0.Low = OP_ENCRYPT_AES256;
		if(pSecEncHashIOParam->HashEnable) {
			pMsg->Option0.High = OP_HASH_SHA256;
		}
		else {
			pMsg->Option0.High = OP_HASH_NONE;
		}
	}
	else if(pSecEncHashIOParam->EncMethod == OP_ENCRYPT_SM4){
		pMsg->DmaParam.OpMode.Bits = SYM_256BIT |  SYM_DECRYPT | SYM_ECB;
		pMsg->Option0.Low = OP_ENCRYPT_SM4;
		if(pSecEncHashIOParam->HashEnable) {
			pMsg->Option0.High = OP_HASH_SM3;
		}
		else {
			pMsg->Option0.High = OP_HASH_NONE;
		}	}
	pMsg->Option1.Low = pSecEncHashIOParam->KeyMode;

	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}

	Status = AlFsbl_CompareHash(0, 0, 0);
	if(Status != ALFSBL_SUCCESS) {
		goto END;
	}
END:
	return Status;
}

uint32_t AlFsbl_CsuDmaCopy(uint32_t SrcAddr, uint32_t DestAddr, uint32_t DataByteLen, uint8_t AddrMode)
{
	uint32_t Status = ALFSBL_SUCCESS;
	uint32_t HashByteLen = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM + 64);

	pMsg->Cmd = CMD_DMA;

	pMsg->DmaParam.KeyAddr     = 0;
	pMsg->DmaParam.IvAddr      = 0;
	pMsg->DmaParam.InputAddr   = SrcAddr;
	pMsg->DmaParam.TotalLength = DataByteLen;
	pMsg->DmaParam.OutputAddr  = DestAddr;
	pMsg->DmaParam.HashOutAddr = 0;

	pMsg->Option0.Low  = OP_ENCRYPT_NONE;
	pMsg->Option0.High = OP_HASH_NONE;
	pMsg->Option1.Low  = OP_BHDR_KEY;
	pMsg->Option1.High = AddrMode;

	pMsg->DmaParam.OpMode.Bits = 0;


	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);

END:
	return Status;

}











/*
static const unsigned int crc32tab[] = {
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL,
	0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L,
	0x0edb8832L, 0x79dcb8a4L, 0xe0d5e91eL, 0x97d2d988L,
	0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L,
	0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L,
	0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL,
	0x14015c4fL, 0x63066cd9L, 0xfa0f3d63L, 0x8d080df5L,
	0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L,
	0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L,
	0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L,
	0x26d930acL, 0x51de003aL, 0xc8d75180L, 0xbfd06116L,
	0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL,
	0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL,
	0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL,
	0x71b18589L, 0x06b6b51fL, 0x9fbfe4a5L, 0xe8b8d433L,
	0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L,
	0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL,
	0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L,
	0x65b0d9c6L, 0x12b7e950L, 0x8bbeb8eaL, 0xfcb9887cL,
	0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L,
	0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL,
	0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L,
	0x44042d73L, 0x33031de5L, 0xaa0a4c5fL, 0xdd0d7cc9L,
	0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L,
	0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L,
	0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL,
	0xedb88320L, 0x9abfb3b6L, 0x03b6e20cL, 0x74b1d29aL,
	0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L,
	0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L,
	0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL,
	0xf762575dL, 0x806567cbL, 0x196c3671L, 0x6e6b06e7L,
	0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL,
	0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L,
	0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL,
	0xd80d2bdaL, 0xaf0a1b4cL, 0x36034af6L, 0x41047a60L,
	0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L,
	0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL,
	0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L,
	0xc2d7ffa7L, 0xb5d0cf31L, 0x2cd99e8bL, 0x5bdeae1dL,
	0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL,
	0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L,
	0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L,
	0x86d3d2d4L, 0xf1d4e242L, 0x68ddb3f8L, 0x1fda836eL,
	0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L,
	0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L,
	0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L,
	0xa7672661L, 0xd06016f7L, 0x4969474dL, 0x3e6e77dbL,
	0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L,
	0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L,
	0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL,
	0xb3667a2eL, 0xc4614ab8L, 0x5d681b02L, 0x2a6f2b94L,
	0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL
};


uint32_t cal_crc32(uint8_t *buf, uint32_t size)
{
	uint32_t i, crc;
	crc = 0xFFFFFFFF;

	for (i = 0; i < size; i++) {
		crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
	}

	return crc ^ 0xFFFFFFFF;
}
*/


