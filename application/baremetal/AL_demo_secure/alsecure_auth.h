#ifndef _ALSECURE_AUTH_H_
#define _ALSECURE_AUTH_H_

#include "alsecure_msg.h"

u32 AlSecure_AuthVerification(Msg_t *InstancePtr, u32 *PubKey, u32 *Digest, u32 *Signature);
u32 AlSecure_AuthInitialize(Msg_t *InstancePtr, u8 AuthMode);


#endif /*end of _ALSECURE_AUTH_H_*/
