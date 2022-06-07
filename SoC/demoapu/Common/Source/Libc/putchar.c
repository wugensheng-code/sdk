/*
 * Copyright (c) 2013-2018, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "nuclei_sdk_hal.h"

int putchar(int c)
{
	return uart_write(SOC_DEBUG_UART, c);
}
