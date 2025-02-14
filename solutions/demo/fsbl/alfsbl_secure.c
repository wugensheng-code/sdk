/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "alfsbl_secure.h"
#include "al_intr.h"
#include "al_utils_def.h"
#include "al_cache.h"

//#include "alfsbl_err_code.h"
//#include "alfsbl_data.h"

//#include "alfsbl_hw.h"
//#include "alfsbl_misc.h"



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
	uint32_t Status = 0;
	uint32_t Cal_Checksum = 0;

	/// CRC32
	Cal_Checksum = AlFsbl_CalcCrc32(pBuffer, Length);
	AL_LOG(AL_LOG_LEVEL_INFO,  "Calculated Checksum: 0x%08x\r\n", Cal_Checksum);

	if(Cal_Checksum != Checksum) {
		Status = SEC_ERROR_CHECKSUM_ERROR;
	}
	else {
		AL_LOG(AL_LOG_LEVEL_INFO,  "checksum check pass...\r\n");
		Status = 0;
	}


	return Status;
}


void TriggerSecInterrupt(void)
{
#ifdef ENABLE_MMU
    AlCache_FlushDcacheAll();
#endif

	/// trigger rpu2csu interrupt
	*(volatile uint32_t *)(RPU2CSU_REQ_ADDR) = 1;
	*(volatile uint32_t *)(RPU2CSU_REQ_ADDR) = 0;
	return;
}


void MsgReceive(void)
{
	MsgFlag = 1;
	return;
}


void RpuCsuAckHandler(void *Param)
{
	(void)Param;
	MsgReceive();
}


uint32_t SecureIrqInit(void)
{
	uint32_t ret;
	AL_INTR_AttrStrct Attr = {
		.TrigMode   =  POSTIVE_EDGE_TRIGGER,
		.Priority   =  0,
#if (defined __riscv || defined __riscv__)
		.VectorMode =  NON_VECTOR_INTERRUPT,
#endif
	};

#if (defined __riscv || defined __riscv__)
	ret = AlIntr_RegHandler(RPU2CSU_ACK_IRQN, &Attr, RpuCsuAckHandler, NULL);
#else
	ret = AlIntr_RegHandler(APU2CSU_ACK_IRQN, &Attr, RpuCsuAckHandler, NULL);
#endif
	AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);

	return ret;
}


uint32_t CheckAckValid(AckDef *pAck)
{
	uint32_t Status = 0;
	while(MsgFlag == 0) {
		/// wait for ack interrupt
	}
	MsgFlag = 0;  /// clear flag

	if(pAck->Cmd != CMD_ACK) {
		AL_LOG(AL_LOG_LEVEL_ERROR,  "Invalid ACK Command: %08x\r\n", pAck->Cmd);
		Status = SEC_ERROR_INVALID_CSU_ACK;
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
		Status = SEC_ERROR_AUTH_FAIL;
	}
	else if(pAck->Option0.Low == OP_HASH_DONE) {
		goto END;
	}
	else if(pAck->Option0.Low == OP_SIGN_DONE) {
		goto END;
	}
	else if(pAck->Option0.Low == OP_GENKEY_DONE) {
		goto END;
	}
	else if(pAck->Option0.Low == OP_GETZ_DONE) {
		goto END;
	}
	else {
		AL_LOG(AL_LOG_LEVEL_ERROR,  "Invalid ACK Command : %02x\r\n", pAck->Option0.Low);
		Status = SEC_ERROR_INVALID_CSU_ACK;
		goto END;
	}

END:
	return Status;
}


uint32_t AlFsbl_Hash(SecureInfo *pSecureInfo)
{
	uint32_t Status = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM_BASEADDR);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM_BASEADDR + 64);

	if((pSecureInfo->HashType != OP_HASH_SHA256) &&
	   (pSecureInfo->HashType != OP_HASH_SM3) &&
	   (pSecureInfo->HashType != OP_HASH_NONE)) {
		Status = SEC_ERROR_SEC_PARAM_INVALID;
	}
	if(pSecureInfo->HashType == OP_HASH_NONE) {
		goto END;
	}

	pMsg->Cmd = CMD_HASH;
//	pMsg->HashParam.DataInAddr  = pSecureInfo->InputAddr;
	pMsg->HashParam.DataInAddr  = pSecureInfo->HashDataAddr;
	pMsg->HashParam.DataLen     = pSecureInfo->DataLength;
	pMsg->HashParam.HashOutAddr = pSecureInfo->HashOutAddr;

	pMsg->Option0.Low           = pSecureInfo->HashType;
	pMsg->Option1.High          = pSecureInfo->BlockMode;

	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);
	if(Status != 0) {
		goto END;
	}
END:
	return Status;
}


uint32_t AlFsbl_CompareHash(uint8_t *pHash1, uint8_t *pHash2, uint32_t HashLen)
{
	uint32_t Idx;
	uint32_t *pHash1_word = (uint32_t *)(pHash1);
	uint32_t *pHash2_word = (uint32_t *)(pHash2);

	for(Idx = 0; Idx < HashLen / 4; Idx++) {
		if(pHash1_word[Idx] != pHash2_word[Idx]) {
			AL_LOG(AL_LOG_LEVEL_ERROR,  "hash different: %08x, %08x\r\n", pHash1_word[Idx], pHash2_word[Idx]);
			return SEC_ERROR_HASH_FAIL;
		}
	}
	return 0;
}


uint32_t AlFsbl_Auth(SecureInfo *pSecureInfo)
{
	uint32_t Status = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM_BASEADDR);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM_BASEADDR + 64);

	if((pSecureInfo->AuthType != OP_AUTH_ECC256) &&
	   (pSecureInfo->AuthType != OP_AUTH_SM2)) {
		Status = SEC_ERROR_SEC_PARAM_INVALID;
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


uint32_t AlFsbl_DecHash(SecureInfo *pSecureInfo)
{
	uint32_t Status = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM_BASEADDR);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM_BASEADDR + 64);

	if((pSecureInfo->EncType != OP_ENCRYPT_AES256) &&
	   (pSecureInfo->EncType != OP_ENCRYPT_SM4) &&
	   (pSecureInfo->EncType != OP_ENCRYPT_NONE)) {
		Status = SEC_ERROR_SEC_PARAM_INVALID;
		goto END;
	}
	if((pSecureInfo->HashType != OP_HASH_SHA256) &&
	   (pSecureInfo->HashType != OP_HASH_SM3) &&
	   (pSecureInfo->HashType != OP_HASH_NONE)) {
		Status = SEC_ERROR_SEC_PARAM_INVALID;
		goto END;
	}
	if((pSecureInfo->KeyMode != OP_BHDR_KEY) &&
	   (pSecureInfo->KeyMode != OP_USER_KEY)) {
		Status = SEC_ERROR_SEC_PARAM_INVALID;
		goto END;
	}

	pMsg->Cmd = CMD_DMA;
	pMsg->DmaParam.KeyAddr     = pSecureInfo->KeyAddr;
	pMsg->DmaParam.IvAddr      = pSecureInfo->IvAddr;
	pMsg->DmaParam.InputAddr   = pSecureInfo->InputAddr;
	pMsg->DmaParam.TotalLength = pSecureInfo->DataLength;
	pMsg->DmaParam.OutputAddr  = pSecureInfo->OutputAddr;
	pMsg->DmaParam.HashOutAddr = pSecureInfo->HashOutAddr;

	pMsg->DmaParam.OpMode.Bits = SYM_256BIT | (pSecureInfo->EncDir) | SYM_ECB;

	pMsg->Option0.Low  = pSecureInfo->EncType;
	pMsg->Option1.High = (pSecureInfo->CsuAddrIncMode) | (pSecureInfo->BlockMode);
	pMsg->Option1.Low  = pSecureInfo->KeyMode;

	/// if encryption type is none, and hash calculation required, only do data copy through csu dma,
	/// calculate hash by hash module after data copy.
	if(pSecureInfo->EncType != OP_ENCRYPT_NONE) {
		pMsg->Option0.High = pSecureInfo->HashType;
	}
	else {
		pMsg->Option0.High =  OP_HASH_NONE;
	}

	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);

END:
	return Status;
}





uint32_t AlFsbl_CsuDmaCopy(uint32_t SrcAddr, uint32_t DestAddr, uint32_t DataByteLen, uint8_t AddrMode)
{
	uint32_t Status = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM_BASEADDR);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM_BASEADDR + 64);

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

	return Status;

}


uint32_t AlFsbl_Signature(uint8_t AuthType, uint32_t PriKeyAddr, uint32_t DigestAddr, uint32_t SigAddr)
{
	uint32_t Status = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM_BASEADDR);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM_BASEADDR + 64);

	pMsg->Cmd = CMD_SIGN;
	pMsg->Option0.Low = AuthType;

	pMsg->SignatureParam.PriKeyAddr    = PriKeyAddr;
	pMsg->SignatureParam.DigestAddr    = DigestAddr;
	pMsg->SignatureParam.SignatureAddr = SigAddr;


	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);

	return Status;

}


uint32_t AlFsbl_KeyPairGen(uint8_t AuthType, uint32_t PubKeyAddr, uint32_t PriKeyAddr)
{
	uint32_t Status = 0;
	SecMsgDef *pMsg = (SecMsgDef *)(CSU_MSG_RAM_BASEADDR);
	AckDef    *pAck = (AckDef *)(CSU_MSG_RAM_BASEADDR + 64);

	pMsg->Cmd = CMD_GENKEY;
	pMsg->Option0.Low = AuthType;

	pMsg->KeyPairGenParam.PubKeyAddr = PubKeyAddr;
	pMsg->KeyPairGenParam.PriKeyAddr = PriKeyAddr;

	TriggerSecInterrupt();
	Status = CheckAckValid(pAck);

	return Status;
}

