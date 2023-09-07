/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_aarch64_macro.h"

/* clean mem using zero
*/
.globl clear_mem
clear_mem:
	str  xzr, [x0], #8
	subs x1,  x1,   #8
	b.gt clear_mem
	ret

/* clear bss*/
.globl clear_bss
clear_bss:
	/* Save LR */
	mov x25, lr

	ldr	x0, =_bss_start
	ldr	x1, =_bss_end
	sub	x1, x1, x0
	bl 	clear_mem

	/* Restore LR and return */
	mov lr, x25
	ret
