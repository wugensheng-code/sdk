/*
 * Copyright (c) 2013-2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <cdefs.h>
#include <stdio.h>

 __attribute__((weak)) void __dead2 __assert(const char *file, unsigned int line,
		      const char *assertion)
{
	printf("ASSERT: %s:%d:%s\n", file, line, assertion);
	while (1)
		;
}
