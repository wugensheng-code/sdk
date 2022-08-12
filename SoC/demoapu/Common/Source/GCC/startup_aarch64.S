/*
 * @Author: Guangxiang Rao
 * @Date: 2022-01-14 14:05:47
 * @LastEditors: Guangxiang Rao
 * @LastEditTime: 2022-05-02 17:13:03
 * @Description: boot barematal for al9000
 * @FilePath: /alsoc-embedded-DE/bsp/boot/apu/src/startup64.S
 */

#include <config.h>
#include <linkage.h>
#include <macro.h>
#include <sysregs.h>

/* boot flow:
 * setup_vector --> clear bss --> lowlevel_init --> set sp --> main
*/
.align 3
.globl	_start

_start:
#ifdef DEBUG
#undef DEBUG
#endif

#define icc_igrpen1_el3 s3_6_c12_c12_7
#define icc_igrpen1_el1 s3_0_c12_c12_7
#define icc_igrpen0_el1 s3_0_c12_c12_6

#ifdef DEBUG
	bl uart0_init_simple
	bl print_boot_el
	isb
#endif
	b setup_vector

/* Initial State: MMU Disabled, i/dCache Disabled */
/* set up vector table */
setup_vector:
	/* Gets the current exception level, and jumps to the corresponding label to execute
	 * x1 = CurrentEL，
	 * 0xc --> el3_entry
	 * 0x8 --> el2_entry
	 * 0x4 --> el1_entry
	**/
	switch_el x1, el3_entry, el2_entry, el1_entry
	/* execute el3 */
el3_entry:
	mrs x0, mpidr_el1
	ldr	x1, =0x00ff
	and	x0, x0, x1
	cmp x0, #0

	/*if primary cpu, continue boot */
	b.eq 0f

	/*if secondary cpu, loop here*/
	b .

0:
	/*init el3 gic system registers*/
	ldr x0, =0x0f
	msr icc_sre_el3, x0

	//enable group1 secure,non-secure
	ldr x0, =0x03
	msr icc_igrpen1_el3, x0
	ldr x0, =0x01
	msr icc_igrpen1_el1, x0
	msr icc_igrpen0_el1, x0

	//start CNTHP_CTL_EL2, so EL1 can get the interrupt
	ldr x0, =0x01
	msr cnthp_ctl_el2, x0 //start

	//set CNTHP_CVAL_EL2 value
	ldr x0, =0x10000
	msr cnthp_cval_el2, x0

	adr	x0, vectors
	msr vbar_el3, x0
	mrs	x0, scr_el3
	/* x0 = x0 | 0xf */
	orr	x0, x0, #0xf			/* SCR_EL3.NS|IRQ|FIQ|EA */
	msr	scr_el3, x0
	msr	cptr_el3, xzr			/* Enable FP/SIMD */
	/* set up stack point */
	ldr x28, =_sp
	mov sp, x28
#ifdef COUNTER_FREQUENCY
	ldr	x0, =COUNTER_FREQUENCY
	msr	cntfrq_el0, x0			/* Initialize CNTFRQ */
#endif
/* switch to el1 */
#ifdef SWITCH_TO_EL1_FROM_EL3
	switch_el3_to_el1
#endif
	b   main_entry

/* el2 */
.align 3
el2_entry:
	adr	x0, vectors
	msr	vbar_el2, x0
	mov	x0, #0x33ff
	msr	cptr_el2, x0			/* Enable FP/SIMD */
	/* set up stack point */
	ldr x28, =_sp
	mov sp, x28
	b   main_entry

/* el1 */
.align 3
el1_entry:
	/* set up stack point */
	ldr x28, =_sp
	mov sp, x28
	adr	x0, vectors
	msr	vbar_el1, x0
	mov	x0, #3 << 20
	msr	cpacr_el1, x0			/* Enable FP/SIMD */
	//enable icache & MMU
	mrs x0, sctlr_el1
	orr x0, x0, #(1 << 12)
	msr sctlr_el1, x0
	b   main_entry

main_entry:
	isb
#ifdef DEBUG
	bl uart0_init_simple
	bl print_el
#endif
	/* clear bss */
	bl clear_bss

	/* Processor specific initialization */
	bl	lowlevel_init

	//123

	/* jump to main */
	bl	main

	/* hang */
	b .

