#include <config.h>
#include <linkage.h>
#include <macro.h>

WEAK(lowlevel_init)
	/* Save LR */
	mov	x29, lr

	// do cpu lowlevel init here
	bl _premain_init

	/* Restore LR */
	mov	lr, x29
	ret
ENDPROC(lowlevel_init)
