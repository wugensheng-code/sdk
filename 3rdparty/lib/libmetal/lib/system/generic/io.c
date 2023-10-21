/*
 * Copyright (c) 2017, Xilinx Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	generic/io.c
 * @brief	Generic libmetal io operations
 */

#include <metal/io.h>

static inline uint64_t page_sz_fix(int page_shift)
{
	int i;
	uint64_t ret = 1;

	if (page_shift == 64)
		return (ret << 63);

	for (i = 0; i < page_shift; i++)
		ret *= 2;

	return ret;
}

static inline int page_num_fix(uint64_t sz, int page_shift)
{
	return (sz / page_sz_fix(page_shift));
}

void metal_sys_io_mem_map(struct metal_io_region *io)
{
	unsigned long p;
	size_t psize;
	size_t *va;

	va = io->virt;
	psize = (size_t)io->size;
	if (psize) {
#if 0
		if (psize >> io->page_shift)
			psize = (size_t)1 << io->page_shift;
		for (p = 0; p <= (io->size >> io->page_shift); p++) {
			metal_machine_io_mem_map(va, io->physmap[p],
						 psize, io->mem_flags);
			va += psize;
		}
#else
		if (page_num_fix(psize, io->page_shift))
			psize = (size_t)page_sz_fix(io->page_shift);
		for (p = 0; p <= ((unsigned long)page_num_fix(io->size, io->page_shift)); p++) {
			metal_machine_io_mem_map(va, io->physmap[p],
										psize, io->mem_flags);
			va += psize;
		}
#endif
	}
}
