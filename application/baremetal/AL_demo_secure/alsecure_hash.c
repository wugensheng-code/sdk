#include "alsecure_hash.h"

extern Ack_t *Ack;

/**
 * @brief 
 * 
 * @param InstancePtr 
 * @param HashBlockMode 
 * @param InputData 
 * @param HashOut 
 * @param DataLength 
 * @return u32 
 */
u32 AlSecure_HashBlock(Msg_t *InstancePtr, u8 HashBlockMode, u32 *InputData, u32 *HashOut, u32 DataLength)
{
    u32 Status = AL_SECURE_SUCCESS;

    //Is hash block mode valid
    if ((HashBlockMode != OP_FIRST_BLOCK) && (HashBlockMode != OP_MID_BLOCK) \
        && (HashBlockMode != OP_LAST_BLOCK)) {
        Status = AL_SECURE_ERROR_BLOCK_MODE;
        goto END;
    }
    //Is data length align
    if (DataLength % 64 != 0) {
        Status = AL_SECURE_ERROR_DATA_ALIGN;
        goto END;
    }

    InstancePtr->Option1.Extend                 = HashBlockMode;
    InstancePtr->MsgData.HashParam.DataIn       = InputData;
    InstancePtr->MsgData.HashParam.DataLength   = DataLength;
    InstancePtr->MsgData.HashParam.HashOut      = HashOut;

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
 * @param InputData 
 * @param HashOut 
 * @param DataLength 
 * @return u32 
 */
u32 AlSecure_HashData(Msg_t *InstancePtr, u32 *InputData, u32 *HashOut, u32 DataLength)
{
    u32 Status = AL_SECURE_SUCCESS;

    //Is data length align
    if (DataLength % 64 != 0) {
        Status = AL_SECURE_ERROR_DATA_ALIGN;
        goto END;
    }

    InstancePtr->Option1.Extend                 = OP_WHOLE_BLOCK;
    InstancePtr->MsgData.HashParam.DataIn       = InputData;
    InstancePtr->MsgData.HashParam.DataLength   = DataLength;
    InstancePtr->MsgData.HashParam.HashOut      = HashOut;

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
 * @param HashMode 
 * @return u32 
 */
u32 AlSecure_HashInitialize(Msg_t *InstancePtr, u8 HashMode)
{
    u32 Status = AL_SECURE_SUCCESS;

    if ((HashMode != OP_HASH_SHA256) && (HashMode != OP_HASH_SM3)) {
        Status = AL_SECURE_ERROR_HASH_MODE;
        goto END;
    }

    //Clear Instance
    memset(InstancePtr, 0, sizeof(Msg_t));

    InstancePtr->Option0.Low = HashMode;

END:
    return Status;
}