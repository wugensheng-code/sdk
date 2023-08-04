#ifndef AL_CIPHER_LL_H
#define AL_CIPHER_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_cipher_hw.h"

static inline AL_VOID AlCipher_ll_Req(AL_REG ReqIntrAddr)
{
    AL_REG32_WRITE(ReqIntrAddr, AL_TRUE);
    AL_REG32_WRITE(ReqIntrAddr, AL_FALSE);
}

static inline AL_VOID AlCipher_ll_SetMsg(AL_REG BaseAddr, AL_U32 Offset, AL_U32 Val)
{
    AL_REG32_WRITE(BaseAddr + CIPHER_MSG_OFFSET + (Offset << 2), Val);
}

static inline AL_U32 AlCipher_ll_GetAck(AL_REG BaseAddr, AL_U32 Offset)
{
    return AL_REG32_READ(BaseAddr + CIPHER_ACK_OFFSET + (Offset << 2));
}

#ifdef __cplusplus
}
#endif

#endif
