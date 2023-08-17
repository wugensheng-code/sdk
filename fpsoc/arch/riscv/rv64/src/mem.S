/* qspi xip mode need load data section to ilm*/
.globl load_code
load_code:
    /* Save ra */
	mv t6, ra

    /* Load code section from FLASH to ILM when code LMA is different with VMA */
    la t0, _ilm_lma
    la t1, _ilm
    beq t0, t1, end_load_code
    /* If the ILM phy-address same as the logic-address, then quit */
    la t2, _eilm
    bgeu t1, t2, end_load_code
loop_load_code:
    lw t5, (t0)
    sw t5, (t1)
    addi t0, t0, 4
    addi t1, t1, 4
    bltu t1, t2, loop_load_code
end_load_code:
    /* Restore ra and return */
	mv ra, t6
	ret

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

.globl load_data
load_data:
    /* Save ra */
	mv t6, ra

    /* Load data section */
    la t0, _data_lma
    la t1, _data
    la t2, _edata
    bgeu t1, t2, end_load_data
loop_load_data:
    lw t5, (t0)
    sw t5, (t1)
    addi t0, t0, 4
    addi t1, t1, 4
    bltu t1, t2, loop_load_data
end_load_data:
    /* Restore ra and return */
    mv ra, t6
    ret
