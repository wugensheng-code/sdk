.globl clear_mem
clear_mem:
    sw zero, (t0)
    addi t0, t0, 4
	addi t1, t1, -4
    bltu zero, t1, clear_mem
	ret

/* clear bss*/
.globl clear_bss
clear_bss:
    /* Save ra */
    mv t6, ra

    la t0, __bss_start
    la t1, _end
    sub t1, t1, t0
    call clear_mem

    /* Restore ra and return */
    mv ra, t6
    ret
