#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "fm_mem_alloc.h"
#include "npu_defines.h"
#include "npu_util.h"
#include "buddy_allocator.h"

#define LOG_TAG "FM_MEM_ALLOC"
#include "elog.h"

AL_U32 fm_start_addr = 0 ; // filter data is stored before fm, after init, fm_start_addr is defined
AL_U32 fm_current_addr = 0 ; // record the start addr of next alloc 
AL_U32 cma_phy_start = 0; // record the phy_addr of the start of cma mem block
AL_VOID* cma_virt_start = 0; // record the vir_addr of the start of cma mem block
struct buddy* buddy_allocator = NULL;

AL_VOID alloc_mem (rt_tensor_t* ts, AL_U8 align) {

    AL_U32 aligned_addr = (fm_current_addr % align == 0) ? fm_current_addr : (fm_current_addr + align - (fm_current_addr % align));

    extern AL_U64* ts_vaddr_rec;
    ts_vaddr_rec[ts->index] = (AL_U64)(cma_virt_start + fm_start_addr + aligned_addr) ;

    ts->param->addr = cma_phy_start + fm_start_addr + aligned_addr ;
    ts->allocated = 1 ;

    fm_current_addr = aligned_addr + ts->param->size ;
}

// only for current test solution
AL_VOID alloc_mem_reset () {
    fm_current_addr = fm_start_addr ;
}

AL_VOID alloc_mem_buddy (rt_tensor_t* ts) {
    int res = buddy_alloc(buddy_allocator, ts->param->size) ;
    if (res < 0) {
        log_e("alloc mem failed\n") ;
    } else {
        AL_U32 true_offset = res << FM_MIN_SIZE_BIT_WIDTH ;

        extern AL_U64* ts_vaddr_rec;
        ts_vaddr_rec[ts->index] = (AL_U64)(cma_virt_start + fm_start_addr + true_offset) ;

        ts->param->addr = cma_phy_start + fm_start_addr + true_offset ;
        ts->allocated = 1 ;
    }

    return ;
}

AL_VOID free_mem_buddy (rt_tensor_t* ts) {

    AL_U32 addr = ts->param->addr ;
    int offset = (addr - fm_start_addr - cma_phy_start) >> FM_MIN_SIZE_BIT_WIDTH ;
    buddy_free(buddy_allocator, offset) ;
    ts->allocated = 0 ;

    return ;
}