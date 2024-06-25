/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


.globl _start
.globl _start_arm32

#define ES_TO_AARCH64           1
#define ES_TO_AARCH32           0

#define SCTLR_EL2_EE_LE         (0 << 25) /* Exception Little-endian          */
#define SCTLR_EL2_WXN_DIS       (0 << 19) /* Write permission is not XN       */
#define SCTLR_EL2_ICACHE_DIS    (0 << 12) /* Instruction cache disabled       */
#define SCTLR_EL2_SA_DIS        (0 << 3)  /* Stack Alignment Check disabled   */
#define SCTLR_EL2_DCACHE_DIS    (0 << 2)  /* Data cache disabled              */
#define SCTLR_EL2_ALIGN_DIS     (0 << 1)  /* Alignment check disabled         */
#define SCTLR_EL2_MMU_DIS       (0)       /* MMU disabled                     */

/*
 * SCR_EL3 bits definitions
 */
#define SCR_EL3_RW_AARCH64      (1 << 10) /* Next lower level is AArch64     */
#define SCR_EL3_RW_AARCH32      (0 << 10) /* Lower lowers level are AArch32  */
#define SCR_EL3_HCE_EN          (1 << 8)  /* Hypervisor Call enable          */
#define SCR_EL3_SMD_DIS         (1 << 7)  /* Secure Monitor Call disable     */
#define SCR_EL3_RES1            (3 << 4)  /* Reserved, RES1                  */
#define SCR_EL3_EA_EN           (1 << 3)  /* External aborts taken to EL3    */
#define SCR_EL3_NS_EN           (1 << 0)  /* EL0 and EL1 in Non-scure state  */


/*
 * SPSR_EL3/SPSR_EL2 bits definitions
 */
#define SPSR_EL_END_LE        (0 << 9)  /* Exception Little-endian          */
#define SPSR_EL_DEBUG_MASK    (1 << 9)  /* Debug exception masked           */
#define SPSR_EL_ASYN_MASK    (1 << 8)  /* Asynchronous data abort masked   */
#define SPSR_EL_SERR_MASK    (1 << 8)  /* System Error exception masked    */
#define SPSR_EL_IRQ_MASK    (1 << 7)  /* IRQ exception masked             */
#define SPSR_EL_FIQ_MASK    (1 << 6)  /* FIQ exception masked             */
#define SPSR_EL_T_A32        (0 << 5)  /* AArch32 instruction set A32      */
#define SPSR_EL_M_AARCH64    (0 << 4)  /* Exception taken from AArch64     */
#define SPSR_EL_M_AARCH32    (1 << 4)  /* Exception taken from AArch32     */
#define SPSR_EL_M_SVC        (0x3)     /* Exception taken from SVC mode    */

#define SPSR_EL_M_HYP        (0xa)     /* Exception taken from HYP mode    */
#define SPSR_EL_M_EL1H        (5)       /* Exception taken from EL1h mode   */
#define SPSR_EL_M_EL2H        (9)       /* Exception taken from EL2h mode   */


/*
 * CNTHCTL_EL2 bits definitions
 */
#define CNTHCTL_EL2_EL1PCEN_EN    (1 << 1)  /* Physical timer regs accessible   */
#define CNTHCTL_EL2_EL1PCTEN_EN    (1 << 0)  /* Physical counter accessible      */

/*
 * CPTR_EL2 bits definitions
 */
#define CPTR_EL2_RES1        (3 << 12 | 0x3ff)           /* Reserved, RES1 */


/*
 * SCTLR_EL1 bits definitions
 */
#define SCTLR_EL1_RES1        (3 << 28 | 3 << 22 | 1 << 20 |\
                 1 << 11) /* Reserved, RES1                   */
#define SCTLR_EL1_UCI_DIS    (0 << 26) /* Cache instruction disabled       */
#define SCTLR_EL1_EE_LE        (0 << 25) /* Exception Little-endian          */
#define SCTLR_EL1_WXN_DIS    (0 << 19) /* Write permission is not XN       */
#define SCTLR_EL1_NTWE_DIS    (0 << 18) /* WFE instruction disabled         */
#define SCTLR_EL1_NTWI_DIS    (0 << 16) /* WFI instruction disabled         */
#define SCTLR_EL1_UCT_DIS    (0 << 15) /* CTR_EL0 access disabled          */
#define SCTLR_EL1_DZE_DIS    (0 << 14) /* DC ZVA instruction disabled      */
#define SCTLR_EL1_ICACHE_DIS    (0 << 12) /* Instruction cache disabled       */
#define SCTLR_EL1_UMA_DIS    (0 << 9)  /* User Mask Access disabled        */
#define SCTLR_EL1_SED_EN    (0 << 8)  /* SETEND instruction enabled       */
#define SCTLR_EL1_ITD_EN    (0 << 7)  /* IT instruction enabled           */
#define SCTLR_EL1_CP15BEN_DIS    (0 << 5)  /* CP15 barrier operation disabled  */
#define SCTLR_EL1_SA0_DIS    (0 << 4)  /* Stack Alignment EL0 disabled     */
#define SCTLR_EL1_SA_DIS    (0 << 3)  /* Stack Alignment EL1 disabled     */
#define SCTLR_EL1_DCACHE_DIS    (0 << 2)  /* Data cache disabled              */
#define SCTLR_EL1_ALIGN_DIS    (0 << 1)  /* Alignment check disabled         */
#define SCTLR_EL1_MMU_DIS    (0)       /* MMU disabled                     */


/*
 * HCR_EL2 bits definitions
 */
#define HCR_EL2_RW_AARCH64    (1 << 31) /* EL1 is AArch64                   */
#define HCR_EL2_RW_AARCH32    (0 << 31) /* Lower levels are AArch32         */
#define HCR_EL2_HCD_DIS        (1 << 29) /* Hypervisor Call disabled         */


/*
 * CPACR_EL1 bits definitions
 */
#define CPACR_EL1_FPEN_EN    (3 << 20) /* SIMD and FP instruction enabled  */


_start:
    msr daifset, 0xF
    isb
    /* disable mmu */
    tlbi alle3
    ic iallu
    dsb sy
    isb
    mrs x0, sctlr_el3
    mov x1, #0x1005
    bic x0, x0, x1
    msr sctlr_el3, x0
    isb

    mov x0, #0
    mov x1, #0
    mov x2, #0
    mov x3, #0
    mov x4, #0
    mov x5, #0
    mov x6, #0
    mov x7, #0
    mov x8, #0
    mov x9, #0
    mov x10, #0
    mov x11, #0
    mov x12, #0
    mov x13, #0
    mov x14, #0
    mov x15, #0
    mov x16, #0
    mov x17, #0
    mov x18, #0
    mov x19, #0
    mov x20, #0
    mov x21, #0
    mov x22, #0
    mov x23, #0
    mov x24, #0
    mov x25, #0
    mov x26, #0
    mov x27, #0
    mov x28, #0
    mov x29, #0
    mov x30, #0

/* execute el3 */
el3_entry:

    /* Enable FP/SIMD */
    msr cptr_el3, xzr

    /* SMP EN */
    mrs x0, S3_1_c15_c2_1
    orr x0, x0, #0x40
    msr S3_1_c15_c2_1, x0
    isb

    /* enable hardware coherency between cores */
    mov x0, #(0x01 << 6)
    msr actlr_el3, x0    //Enable L2ACTLR write access from EL2
    msr actlr_el2, x0    //Enable L2ACTLR write access from Non-secure EL1

    /* set icc_sre_el3 */
    mov x0, #(0x1 << 0)         //SRE, Enable system register
    orr x0, x0, #(0x1 << 1)     //DFB, Disable FIQ bypass
    orr x0, x0, #(0x1 << 2)     //DIB, Disable IRQ bypass
    orr x0, x0, #(0x1 << 3)     //Enable, Enable lower EL access SRE_EL1 and SRE_EL2
    msr S3_6_c12_c12_5, x0
    isb

    /* set icc_igrpen1_el3 */
    mov x0, #(0x1 << 0)         //Enable group 1 interrupts for Non-secure state
    orr x0, x0, #(0x1 << 1)     //Enable group 1 interrupts for Secure state
    msr S3_6_c12_c12_7, x0

    /* set icc_igrpen0_el1 */
    mov x0, #(0x1 << 0)         //Enable group 0 interrupts
    msr S3_0_c12_c12_6, x0

    mrs    x0, scr_el3
    orr    x0, x0, #(0x1 << 1)      /* Enable IRQ */
    orr    x0, x0, #(0x1 << 2)      /* Enable FIQ */
    orr    x0, x0, #(0x1 << 3)      /* Enable EA */
    bic    x0, x0, #0x1             /* clear scr_el3.NS */
    msr    scr_el3, x0

    msr SCTLR_EL2, xzr
    msr SCTLR_EL1, xzr

#if 1
    // Bits [5:4] Reserved, RES1
    mov x0, #0x30

    /*
    NS, bit [0]
    Non-secure bit.
    0 Indicates that EL0 and EL1 are in Secure state, and so memory accesses from those
    Exception levels can access Secure memory.
    When executing at EL3:
    • The AT S1E2R, AT S1E2W, TLBI VAE2, TLBI VALE2, TLBI VAE2IS, TLBI
    VALE2IS, TLBI ALLE2, and TLBI ALLE2IS System instructions are
    UNDEFINED.
    • Each AT S12E** System instruction executes as the corresponding AT S1E**
    instruction. For example, AT S12E0R executes as AT S1E0R.
    • Each of the TLBI IPAS2E1, TLBI IPAS2E1IS, TLBI IPAS2LE1, and TLBI
    IPAS2LE1IS System instructions executes as a NOP.
    • A TLBI VMALLS12E1 System instruction executes as TLBI VMALLE1, and a
    TLBI VMALLS12E1IS System instruction executes as TLBI VMALLE1IS.
    1 Indicates that EL0 and EL1 are in Non-secure state, and so memory accesses from those
    Exception levels cannot access Secure memory.
    Note
    EL2 is not supported in the Secure state. When SCR_EL3.NS==0, it is not possible to enter EL2,
    and the EL2 state has no effect on execution. See Virtualization on page D1-1782.

    SMD, bit [7]
    Secure Monitor Call disable. Disables SMC instructions at EL1 and above, from both Security states
    and both Execution states.
    0 SMC instructions are enabled at EL1 and above.
    1 SMC instructions are UNDEFINED at EL1 and above.
    */
    orr x0, x0, #(1 << 7)

    /*
    HCE, bit [8]
    Hypervisor Call instruction enable. Enables HVC instructions at EL3, EL2, and Non-secure EL1,
    in both Execution states.
    0 HVC instructions are UNDEFINED at EL3, EL2, and Non-secure EL1, and any resulting
    exception is taken from the current Exception level to the current Exception level.
    1 HVC instructions are enabled at EL1 and above.
    Note
    HVC instructions are always UNDEFINED at EL0.
    If EL2 is not implemented, this bit is RES0.
    */

    orr x0, x0, #(1 << 8)

    /*
    SIF, bit [9]
    Secure instruction fetch. When the PE is in Secure state, this bit disables instruction fetch from
    Non-secure memory. The possible values for this bit are:
    0 Secure state instruction fetches from Non-secure memory are permitted.
    1 Secure state instruction fetches from Non-secure memory are not permitted.
    This bit is permitted to be cached in a TLB.
    */
    orr x0, x0, #(1 << 9)

    /*
    RW, bit [10]
    Execution state control for lower Exception levels.
    0 Lower levels are all AArch32.
    1 The next lower level is AArch64.
    If EL2 is present:
    • EL2 is AArch64.
    • EL2 controls EL1 and EL0 behaviors.
    If EL2 is not present:
    • EL1 is AArch64.
    • EL0 is determined by the Execution state described in the current process state
        when executing at EL0.
    If all lower Exception levels cannot use AArch32 then this bit is RAO/WI.
    This bit is permitted to be cached in a TLB.
    */
    //orr x0, x0, #(1 << 10)         //AArch32? AArch64
    msr scr_el3, x0

    ldr x0, =_start_arm32
    add x0, x0, #0x400
    bic x0, x0, #(0x400 - 1)
    msr elr_el3, x0

    /*
    M[4], bit [4]
    Execution state that the exception was taken from. Possible values of this bit are:
    1 Exception taken from AArch32.

    T, bit [5]
    T32 Instruction set state bit. Determines the AArch32 instruction set state that the exception was
    taken from. Possible values of this bit are:
    0 Taken from A32 state.
    1 Taken from T32 state.

    F, bit [6]
    FIQ mask bit. The possible values of this bit are:
    0 Exception not masked.
    1 Exception masked.

    I, bit [7]
    IRQ mask bit. The possible values of this bit are:
    0 Exception not masked.
    1 Exception masked.

    A, bit [8]
    SError interrupt mask bit. The possible values of this bit are:
    0 Exception not masked.
    1 Exception masked.

    E, bit [9]
    Endianness state bit. Controls the load and store endianness for data accesses:
    0 Little-endian operation
    1 Big-endian operation.

    Instruction fetches ignore this bit.
    When the reset value of the SCTLR.EE bit is defined by a configuration input signal, that value also
    applies to the CPSR.E bit on reset, and therefore applies to software execution from reset.
    If an implementation does not provide Big-endian support, this bit is RES0. If it does not provide
    Little-endian support, this bit is RES1.
    If an implementation provides Big-endian support but only at EL0, this bit is RES0 for an exception
    return to any Exception level other than EL0.
    Likewise, if it provides Little-endian support only at EL0, this bit is RES1 for an exception return to
    any Exception level other than EL0.

    */

    // 0x1c3: 0x1 1101 0011
    mov x1, #((0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 4) |(0x3 << 0))
    msr spsr_el3, x1

    eret
#endif

_start_arm32:
    .word 0xdeadbeef
