#ifndef _ALSECURE_MISC_H_
#define _ALSECURE_MISC_H_

#include "al_debug.h"
#include "al_types.h"

#define ALSECURE_PRINT(...)   rom_printf(DEBUG_INFO, __VA_ARGS__);

uint64_t AlSecure_GetCurTime(void);
uint64_t AlSecure_CalcTimeToUs(uint64_t StartTime, uint64_t EndTime);

#endif /*end of _ALSECURE_MISC_H_*/
