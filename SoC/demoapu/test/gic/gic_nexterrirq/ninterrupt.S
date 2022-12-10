#include <linkage.h>
#include <macro.h>

/*
*/
.globl ninterrupt_trigger
ninterrupt_trigger:
// L2 Error injection example (AArch64)
    MRS x0, S3_1_C15_C0_0 // read L2ACTLR_EL1
    ORR x0, x0, #(0x1 << 29) // set L2DEIEN (or setting bit24 L2TEIEN for L2 tag RAM error injection)
    MSR S3_1_C15_C0_0, x0 // write L2ACTLR_EL1
    ISB

    LDR x0, =0x38300240
    LDR x1, =0x12345678
    STR x1, [x0] // MMU and Dcache is required to be enabled and x0 is a cacheable address with address bits[4:2] = 3'b000
    MOV x1, #0 // x1 = way counter way_loop
    MOV x2, #9 // x2 = set number, for 0x38300240, set number=9

way_loop: // iterate all ways to make sure the dirty line is cleaned to L2 so that the error is injected to L2 data RAM
    LSL x3, x1, #30
    ORR x3, x3, x2, LSL #6
    DC CISW, x3 // DC CISW by x3
    ADD x1, x1, #1 // next way
    CMP x1, #4 // last way reached yet?
    BNE way_loop // if not, iterate way_loop

    DSB sy // ensure cache maintenance is complete

    // disable error injection
    MRS x0, S3_1_C15_C0_0 // read L2ACTLR_EL1
    BIC x0, x0, #(0x1 << 29) // clear L2DEIEN (or clear bit24 L2TEIEN for L2 tag RAM error injection)
    MSR S3_1_C15_C0_0, x0 // write L2ACTLR_EL1
    ISB
    LDR x0, =0x38300240 //
    DC CIVAC, x0 // DC CIVAC: clean and invalidate the dirty cacheline from L2 to external memory to trigger the error (update L2MERRSR)
    DSB sy // ensure cache maintenance is complete
    MRS x0, S3_1_C15_C2_3 // read L2MERRSR_EL1 for error information

	ret


.globl mmu_at_test
mmu_at_test:
	at s1e1r, x0
	mrs x0, par_el1
	ret