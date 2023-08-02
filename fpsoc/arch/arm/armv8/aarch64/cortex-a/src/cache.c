#include <al_type.h>

void __asm_invalidate_icache_all(void);
void __asm_flush_dcache_range(AL_UINTPTR, AL_UINTPTR);
void __asm_invalidate_dcache_range(AL_UINTPTR, AL_UINTPTR);
void __asm_invalidate_dcache_all(void);
void __asm_flush_dcache_all(void);


AL_VOID AlCache_InvalidateIcacheAll(AL_VOID)
{
    __asm_invalidate_icache_all();
}

AL_VOID AlCache_FlushDcacheRange(AL_UINTPTR Start, AL_UINTPTR Stop)
{
    __asm_flush_dcache_range(Start, Stop);
}

AL_VOID AlCache_InvalidateDcacheRange(AL_UINTPTR Start, AL_UINTPTR Stop)
{
    __asm_invalidate_dcache_range(Start, Stop);
}

AL_VOID AlCache_InvalidateDcacheAll(AL_VOID)
{
    __asm_invalidate_dcache_all();
}

AL_VOID AlCache_FlushDcacheAll(AL_VOID)
{
    __asm_flush_dcache_all();
}
