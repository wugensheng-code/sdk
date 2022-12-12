#ifndef _ALSECURE_CIPHER_H_
#define _ALSECURE_CIPHER_H_

#include "alsecure_msg.h"



u32 AlSecure_EncryptBlock(Msg_t *InstancePtr, u8 HashBlockMode, u8 DmaIncMode, \
                        u32 *InputData, u32 *OutputData, u32 *HashOut, u32 DataLength);
u32 AlSecure_EncryptData(Msg_t *InstancePtr, u8 DmaIncMode, u32 *InputData, 
                        u32 *OutputData, u32 *HashOut, u32 DataLength);
u32 AlSecure_EncryptInitialize(Msg_t *InstancePtr, u8 EncryptMethod, u32 EncryptMode, u32 EncDec, \
                                u8 HashMode, u8 KeySel, u32 *IvPtr, u32 *KeyPtr);
#endif /*end of _ALSECURE_CIPHER_H_*/
