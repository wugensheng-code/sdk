#include <config.h>
#include <linkage.h>
#include <macro.h>

WEAK(lowlevel_init)
	/* Save LR */
	stp	x29, x30, [sp, #-16]!

	// do cpu lowlevel init here
	bl _premain_init

	/* Restore LR */
	ldp	x29, x30, [sp],#16

	ret
ENDPROC(lowlevel_init)
