/* See LICENSE of license details. */
#include "nuclei_sdk_soc.h"
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

extern const char _heap_start; /* Defined by the linker */
extern const char _heap_end;   /* Defined by the linker */

const char * heap_start_ptr = &_heap_start;  /* pointer to head of heap */
const char * heap_end_ptr = &_heap_end;  /* pointer to head of heap */

static void * cur_heap = &_heap_start;

__WEAK void* _sbrk(ptrdiff_t incr)
{
    if (cur_heap + incr >  heap_end_ptr || cur_heap + incr <  heap_start_ptr) {
        return -1;
    } else {
        cur_heap += incr;
        return (cur_heap - incr);
    }
}