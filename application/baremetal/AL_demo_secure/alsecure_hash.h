#ifndef _ALSECURE_HASH_H_
#define _ALSECURE_HASH_H_

#include "alsecure_msg.h"


u32 AlSecure_HashBlock(Msg_t *InstancePtr, u8 HashBlockMode, u32 *InputData, u32 *HashOut, u32 DataLength);
u32 AlSecure_HashData(Msg_t *InstancePtr, u32 *InputData, u32 *HashOut, u32 DataLength);
u32 AlSecure_HashInitialize(Msg_t *InstancePtr, u8 HashMode);


#endif /*end of _ALSECURE_HASH_H_*/
