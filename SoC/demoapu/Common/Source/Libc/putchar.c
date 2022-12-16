/*
 * Copyright (c) 2013-2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "nuclei_sdk_hal.h"

int __attribute__((weak)) putchar(int c)
{
	return uart_write(SOC_DEBUG_UART, c);
}
/*
char *logaddr = (char *)0x10000000;

int putchar(int c)
{
	//static char *logaddr = (char *)0x10000000;
	*logaddr++ = c;
	return 0;
}
*/