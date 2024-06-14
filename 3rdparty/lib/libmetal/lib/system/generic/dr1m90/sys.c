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
#if 1
    void *end = addr + len;
    if (!addr && !len)
        AlCache_FlushDcacheAll();
    else {
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

uint64_t metal_machine_io_read(struct metal_io_region *io,
				unsigned long offset,
				memory_order order,
				int width)
{
	void *ptr = metal_io_virt(io, offset);

	metal_machine_cache_invalidate(ptr, width);

	if (ptr && sizeof(atomic_uchar) == width)
		return atomic_load_explicit((atomic_uchar *)ptr, order);
	else if (ptr && sizeof(atomic_ushort) == width)
		return atomic_load_explicit((atomic_ushort *)ptr, order);
	else if (ptr && sizeof(atomic_uint) == width)
		return atomic_load_explicit((atomic_uint *)ptr, order);
	else if (ptr && sizeof(atomic_ulong) == width)
		return atomic_load_explicit((atomic_ulong *)ptr, order);
#ifndef NO_ATOMIC_64_SUPPORT
	else if (ptr && sizeof(atomic_ullong) == width)
		return atomic_load_explicit((atomic_ullong *)ptr, order);
#endif
	metal_assert(0);
	return 0;
}

void metal_machine_io_write(struct metal_io_region *io,
				 unsigned long offset,
				 uint64_t value,
				 memory_order order,
				 int width)
{
	void *ptr = metal_io_virt(io, offset);

	if (ptr && sizeof(atomic_uchar) == width)
		atomic_store_explicit((atomic_uchar *)ptr, (unsigned char)value,
				      order);
	else if (ptr && sizeof(atomic_ushort) == width)
		atomic_store_explicit((atomic_ushort *)ptr,
				      (unsigned short)value, order);
	else if (ptr && sizeof(atomic_uint) == width)
		atomic_store_explicit((atomic_uint *)ptr, (unsigned int)value,
				      order);
	else if (ptr && sizeof(atomic_ulong) == width)
		atomic_store_explicit((atomic_ulong *)ptr, (unsigned long)value,
				      order);
#ifndef NO_ATOMIC_64_SUPPORT
	else if (ptr && sizeof(atomic_ullong) == width)
		atomic_store_explicit((atomic_ullong *)ptr,
				      (unsigned long long)value, order);
#endif
	else
		metal_assert(0);

	metal_machine_cache_flush(ptr, width);
}

struct metal_io_ops io_ops = {
#ifdef ENABLE_MMU
	.read = metal_machine_io_read,
	.write = metal_machine_io_write,
#else
	.read = NULL,
	.write = NULL,
#endif
};
