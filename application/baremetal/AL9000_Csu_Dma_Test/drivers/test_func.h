
#ifndef TEST_FUNC_H
#define TEST_FUNC_H

#include "al_types.h"

extern u32 CheckBuf(u32 *Buf0, u32 *Buf1, u32 Length);
extern u32 CheckAckValid(Ack_t *Ack);
extern void Get_Gold_Data(unsigned int const *ptr_const_data,
                          unsigned int *ptr_data, unsigned int len);

u32 CheckHash(u32 *HashBuf, u32 *GoldBuf);
#endif
