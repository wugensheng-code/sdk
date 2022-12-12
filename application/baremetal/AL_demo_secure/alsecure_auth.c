#include "alsecure_auth.h"

extern Ack_t *Ack;

/**
 * @brief 
 * 
 * @param InstancePtr 
 * @param PubKey 
 * @param Digest 
 * @param Signature 
 * @return u32 
 */
u32 AlSecure_AuthVerification(Msg_t *InstancePtr, u32 *PubKey, u32 *Digest, u32 *Signature)
{
    u32 Status = AL_SECURE_SUCCESS;

    InstancePtr->MsgData.AuthParam.Pubkey       = PubKey;
    InstancePtr->MsgData.AuthParam.Digest       = Digest;
    InstancePtr->MsgData.AuthParam.Signature    = Signature;

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
 * @param AuthMode 
 * @return u32 
 */
u32 AlSecure_AuthInitialize(Msg_t *InstancePtr, u8 AuthMode)
{
    u32 Status = AL_SECURE_SUCCESS;

    if ((AuthMode != OP_AUTH_ECC256) && (AuthMode != OP_AUTH_SM2)) {
        Status = AL_SECURE_ERROR_HASH_MODE;
        goto END;
    }

    //Clear Instance
    memset(InstancePtr, 0, sizeof(Msg_t));

    InstancePtr->Option0.Low = AuthMode;

END:
    return Status;
}
