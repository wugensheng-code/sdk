#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "fm_mem_alloc.h"
#include "npu_defines.h"
#include "npu_util.h"
#include "buddy_allocator.h"

#ifdef RUN_PLATFORM_LINUX
#include <sys/mman.h>
#endif

uint32_t fm_start_addr = 0 ; // filter data is stored before fm, after init, fm_start_addr is defined
uint32_t fm_current_addr = 0 ; // record the start addr of next alloc 
uint32_t cma_phy_start = 0; // record the phy_addr of the start of cma mem block
void* cma_virt_start = 0; // record the vir_addr of the start of cma mem block
struct buddy* buddy_allocator = NULL;

void alloc_mem (rt_tensor_t* ts, uint8_t align) {

    uint32_t aligned_addr = (fm_current_addr % align == 0) ? fm_current_addr : (fm_current_addr + align - (fm_current_addr % align));

    extern uint64_t* ts_vaddr_rec;
    ts_vaddr_rec[ts->index] = (uint64_t)(cma_virt_start + fm_start_addr + aligned_addr) ;

    ts->param->addr = cma_phy_start + fm_start_addr + aligned_addr ;
    ts->allocated = 1 ;

    fm_current_addr = aligned_addr + ts->param->size ;
}

// only for current test solution
void alloc_mem_reset () {
    fm_current_addr = fm_start_addr ;
}

void alloc_mem_buddy (rt_tensor_t* ts) {
    int res = buddy_alloc(buddy_allocator, ts->param->size) ;
    if (res < 0) {
        printf("alloc mem failed\n") ;
    } else {
        uint32_t true_offset = res << FM_MIN_SIZE_BIT_WIDTH ;

        extern uint64_t* ts_vaddr_rec;
        ts_vaddr_rec[ts->index] = (uint64_t)(cma_virt_start + fm_start_addr + true_offset) ;

        ts->param->addr = cma_phy_start + fm_start_addr + true_offset ;
        ts->allocated = 1 ;
    }

    return ;
}

void free_mem_buddy (rt_tensor_t* ts) {

    uint32_t addr = ts->param->addr ;
    int offset = (addr - fm_start_addr - cma_phy_start) >> FM_MIN_SIZE_BIT_WIDTH ;
    buddy_free(buddy_allocator, offset) ;
    ts->allocated = 0 ;

    return ;
}