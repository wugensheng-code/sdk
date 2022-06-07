#include <linkage.h>
#include <macro.h>

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

	adrp	x0, _bss_start
	adrp	x1, _bss_end
	sub	x1, x1, x0
	bl 	clear_mem
	
	/* Restore LR and return */
	mov lr, x25 
	ret
