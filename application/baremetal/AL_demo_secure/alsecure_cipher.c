#include "alsecure_cipher.h"

extern Ack_t *Ack;

/**
 * @brief 
 * 
 * @param InstancePtr 
 * @param HashBlockMode 
 * @param DmaIncMode 
 * @param InputData 
 * @param OutputData 
 * @param HashOut 
 * @param DataLength 
 * @return u32 
 */
u32 AlSecure_EncryptBlock(Msg_t *InstancePtr, u8 HashBlockMode, u8 DmaIncMode, \
                        u32 *InputData, u32 *OutputData, u32 *HashOut, u32 DataLength)
{
    u32 Status = AL_SECURE_SUCCESS;

    //Is hash block mode valid
    if ((HashBlockMode != OP_FIRST_BLOCK) && (HashBlockMode != OP_MID_BLOCK) \
        && (HashBlockMode != OP_LAST_BLOCK)) {
        Status = AL_SECURE_ERROR_BLOCK_MODE;
        goto END;
    }
    //Is dma increase mode valid
    if ((DmaIncMode != OP_NONE_INCR) && (DmaIncMode != OP_BOTH_INCR) &&\
        (DmaIncMode != OP_SRC_INCR) && (DmaIncMode != OP_DST_INCR)) {
        Status = AL_SECURE_ERROR_DMA_INC_MODE;
        goto END;
    }
    //Is data length align
    if (DataLength % 64 != 0) {
        Status = AL_SECURE_ERROR_DATA_ALIGN;
        goto END;
    }

    InstancePtr->Option1.High                               = DmaIncMode;
    InstancePtr->Option1.Extend                             = HashBlockMode;
    InstancePtr->MsgData.DmaParam.CipherParam.pbInput       = InputData;
    InstancePtr->MsgData.DmaParam.CipherParam.pbOutput      = OutputData;
    InstancePtr->MsgData.DmaParam.CipherParam.TotalLength   = DataLength;
    InstancePtr->MsgData.DmaParam.HashOut                   = HashOut;

    Status = Msg_Send(InstancePtr);
    Status = Msg_CheckAckValid(Ack);
    if (Status != AL_SECURE_SUCCESS) {
        goto END;
    }

END:
    return Status;
}

/**
 * @brief 
 * 
 * @param InstancePtr 
 * @param DmaIncMode 
 * @param InputData 
 * @param OutputData 
 * @param HashOut 
 * @param DataLength 
 * @return u32 
 */
u32 AlSecure_EncryptData(Msg_t *InstancePtr, u8 DmaIncMode, u32 *InputData, 
                        u32 *OutputData, u32 *HashOut, u32 DataLength)
{
    u32 Status = AL_SECURE_SUCCESS;

    //Is dma increase mode valid
    if ((DmaIncMode != OP_NONE_INCR) && (DmaIncMode != OP_BOTH_INCR) &&\
        (DmaIncMode != OP_SRC_INCR) && (DmaIncMode != OP_DST_INCR)) {
        Status = AL_SECURE_ERROR_DMA_INC_MODE;
        goto END;
    }
    //Is data length align
    if (DataLength % 64 != 0) {
        Status = AL_SECURE_ERROR_DATA_ALIGN;
        goto END;
    }

    InstancePtr->Option1.High                               = DmaIncMode;
    InstancePtr->Option1.Extend                             = OP_WHOLE_BLOCK;
    InstancePtr->MsgData.DmaParam.CipherParam.pbInput       = InputData;
    InstancePtr->MsgData.DmaParam.CipherParam.pbOutput      = OutputData;
    InstancePtr->MsgData.DmaParam.CipherParam.TotalLength   = DataLength;
    InstancePtr->MsgData.DmaParam.HashOut                   = HashOut;

    Status = Msg_Send(InstancePtr);
    Status = Msg_CheckAckValid(Ack);
    if (Status != AL_SECURE_SUCCESS) {
        goto END;
    }
    
END:
    return Status;
}

/**
 * @brief encrypt or decrypt func init
 * 
 * @param InstancePtr   Msg_t structure
 * @param EncryptMethod OP_ENCRYPT_AES256/OP_ENCRYPT_SM4/OP_ENCRYPT_NONE
 * @param EcbCbc SYM_ECB/SYM_CBC
 * @param EncDec SYM_ENCRYPT/SYM_DECRYPT
 * @param HashMode OP_HASH_SHA256/OP_HASH_SM3/OP_HASH_NONE
 * @param KeySel OP_BHDR_KEY/OP_USER_KEY
 * @param IvPtr valid for SYM_CBC
 * @param KeyPtr valid for OP_USER_KEY
 * @return u32 init status
 */
u32 AlSecure_EncryptInitialize(Msg_t *InstancePtr, u8 EncryptMethod, u32 EcbCbc, u32 EncDec, \
                                u8 HashMode, u8 KeySel, u32 *IvPtr, u32 *KeyPtr)
{
    u32 Status = AL_SECURE_SUCCESS;

    //Is encrypt method error
    if ((EncryptMethod != OP_ENCRYPT_AES256) && (EncryptMethod != OP_ENCRYPT_SM4)\
        && (EncryptMethod != OP_ENCRYPT_NONE)) {
        Status = AL_SECURE_ERROR_ENCRYPT_METHOD;
        goto END;
    }
    //Is encrypt mode error
    if ((EcbCbc != SYM_ECB) && (EcbCbc != SYM_CBC)) {
        Status = AL_SECURE_ERROR_ENCRYPT_MODE;
        goto END;
    }
    //Is encrypt or decrypt
    if ((EncryptMethod != OP_ENCRYPT_NONE) && (EncDec != SYM_ENCRYPT) && (EncDec != SYM_DECRYPT)) {
        Status = AL_SECURE_ERROR_ENCDEC_VALUE;
        goto END;
    }
    //Is hash mode not suit with encrypt method or error
    if (((EncryptMethod == OP_ENCRYPT_AES256) && (HashMode != OP_HASH_SHA256)) || \
        ((EncryptMethod == OP_ENCRYPT_SM4) && (HashMode != OP_HASH_SM3)) || \
        ((EncryptMethod == OP_ENCRYPT_NONE) && (HashMode != OP_HASH_NONE)) || \
        ((HashMode != OP_HASH_SHA256) && (HashMode != OP_HASH_SM3) && \
        (HashMode != OP_HASH_NONE))) {
        Status = AL_SECURE_ERROR_HASH_MODE;
        goto END;
    }
    //Is key sel error
    if ((KeySel != OP_BHDR_KEY) && (KeySel != OP_USER_KEY)) {
        Status = AL_SECURE_ERROR_KEY_MODE;
        goto END;
    }
    //Is IV valid
    if ((EcbCbc == SYM_CBC) && (IvPtr == NULL)) {
        Status = AL_SECURE_ERROR_IV_VALUE;
        goto END;
    }
    //Is key valid
    if ((KeySel == OP_USER_KEY) && (KeyPtr == NULL)) {
        Status = AL_SECURE_ERROR_KEY_VALUE;
        goto END;
    }
    //Clear Instance
    memset(InstancePtr, 0, sizeof(Msg_t));

    InstancePtr->Cmd                                        = CMD_DMA;
    InstancePtr->Option0.Low                                = EncryptMethod;
    InstancePtr->Option0.High                               = HashMode;
    InstancePtr->Option1.Low                                = KeySel;
    InstancePtr->MsgData.DmaParam.CipherParam.OpMode.All    = SYM_AES256 | EncDec | EcbCbc;
    if (EcbCbc == SYM_CBC) {
        InstancePtr->MsgData.DmaParam.CipherParam.pbIV      = IvPtr;
    } else {
        InstancePtr->MsgData.DmaParam.CipherParam.pbIV      = NULL;
    }
    if (KeySel == OP_USER_KEY) {
        InstancePtr->MsgData.DmaParam.CipherParam.pbKey     = KeyPtr;
    } else {
        InstancePtr->MsgData.DmaParam.CipherParam.pbKey     = NULL;
    }

END:
    return Status;
}
