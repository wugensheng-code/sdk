/* See LICENSE of license details. */
#include "nuclei_sdk_soc.h"
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

extern void * _heap_start; /* Defined by the linker */
extern void * _heap_end;   /* Defined by the linker */
static void * cur_heap_end;

__WEAK void* _sbrk(ptrdiff_t incr)
{
    void * next_end;

    if (cur_heap_end == 0)
        cur_heap_end = _heap_start;

    next_end = cur_heap_end + incr;

    if ((next_end < _heap_start) || (next_end > _heap_end)) {
        return (void*)(-1);
    }

    cur_heap_end = next_end;
    return (void*)(next_end - incr);
}