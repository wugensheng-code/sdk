#ifndef _AL_DEBUG_H_
#define _AL_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"

#define DEBUG_GENERAL       0x1     //only flow and success/fail
#define DEBUG_INFO          0x2     //GENERAL and cmd register info
#define DEBUG_BRANCHTEST    0x4     //branch test log

//#define DEBUG_MACRO
#ifdef DEBUG_MACRO
#define DebugCurType  ((DEBUG_GENERAL))
#else
uint32_t __attribute__((weak)) DebugCurType;
#endif

#define rom_printf(type, ...)\
        if (((type) & DebugCurType))  {printf (__VA_ARGS__); }

#ifdef __cplusplus
}
#endif

#endif /* _AL_DEBUG_H_ */
