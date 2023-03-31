#ifndef _AL_DEBUG_H_
#define _AL_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "stdint.h"

#define DEBUG_GENERAL       0x1     //only flow and success/fail
#define DEBUG_INFO          0x2     //GENERAL and cmd register info
#define DEBUG_BRANCHTEST    0x4     //branch test log

extern uint32_t DebugCurType;

#define rom_printf(type, ...)\
        if (((type) & DebugCurType))  {printf (__VA_ARGS__); }

extern void PrintSetDebugCurType(uint32_t level);

#ifdef __cplusplus
}
#endif

#endif /* _AL_DEBUG_H_ */
