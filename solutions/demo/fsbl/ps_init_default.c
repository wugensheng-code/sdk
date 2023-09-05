/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

int __attribute__((weak)) Soc_PlatInit(void)
{
	printf("Default ps init\r\n");
	return 0;
}
