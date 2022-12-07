#include "stdint.h"
#include "al_debug.h"

uint32_t DebugCurType = 0;

void inline __attribute__((weak)) print(char const *format, ...)
{
    return;
}

void PrintSetDebugCurType(uint32_t level)
{
    DebugCurType = level;
}

