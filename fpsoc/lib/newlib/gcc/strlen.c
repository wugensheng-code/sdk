/*
 * Copyright (c) 2018, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>

#if (defined _AARCH_64 || defined __aarch64__)

size_t strlen(const char *s)
{
	const char *cursor = s;

	while (*cursor)
		cursor++;

	return cursor - s;
}

#endif
