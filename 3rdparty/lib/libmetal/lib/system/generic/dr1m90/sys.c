/*
 * Copyright (c) 2016, Xilinx Inc. and Contributors. All rights reserved.
 *
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/dr1m90/sys.c
 * @brief	machine specific system primitives implementation.
 */

#include <metal/compiler.h>
#include <metal/io.h>
#include <metal/sys.h>
#include <stdint.h>

#ifdef ENABLE_MMU
#include "al_cache.h"

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define CACHE_ALIGN(addr, len) \
    do { \
        uint64_t _addr = (uint64_t)(addr); \
        uint32_t cacheline_sz = dr1m90_cache_line_size(); \
        addr = (void *)((_addr / cacheline_sz) * cacheline_sz); \
        len = DIV_ROUND_UP(len, cacheline_sz) * cacheline_sz; \
    } while (0)

static uint32_t dr1m90_cache_line_size(void)
{
    uint64_t ctr_el0_val;

    __asm__ __volatile__ (
    "mrs %0, ctr_el0\n"
    : "=r" (ctr_el0_val)
    );

    ctr_el0_val >>= 16;
    ctr_el0_val &= 0xf;

    return (4 << ctr_el0_val); /* cache line size */
}
#endif /* ENABLE_MMU */

void sys_irq_restore_enable(unsigned int flags)
{
    AlIntr_RestoreLocalInterruptMask(flags);
}

unsigned int sys_irq_save_disable(void)
{
    return AlIntr_SaveLocalInterruptMask();
}

void metal_machine_cache_flush(void *addr, unsigned int len)
{
#ifdef ENABLE_MMU
#if 0 /* xxx FIXME hewb */
    void *end = addr + len;
    if (!addr && !len)
        AlCache_FlushDcacheAll();
    else {
        CACHE_ALIGN(addr, len);
        AlCache_FlushDcacheRange((AL_UINTPTR)addr, (AL_UINTPTR)end);
    }
#else
    AlCache_FlushDcacheAll();
#endif
#endif
}

void metal_machine_cache_invalidate(void *addr, unsigned int len)
{
#ifdef ENABLE_MMU
    void *end = addr + len;
    if (!addr && !len)
        AlCache_InvalidateDcacheAll();
    else {
        CACHE_ALIGN(addr, len);
        AlCache_InvalidateDcacheRange((AL_UINTPTR)addr, (AL_UINTPTR)end);
    }
#endif
}

/**
 * @brief poll function until some event happens
 */
void metal_weak metal_generic_default_poll(void)
{
    metal_asm volatile("wfi");
}

void *metal_machine_io_mem_map(void *va, metal_phys_addr_t pa,
			       size_t size, unsigned int flags)
{
    UNUSED(pa);
    UNUSED(size);
    UNUSED(flags);
    return va;
}
