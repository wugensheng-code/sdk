#ifndef __SYSREG_H
#define __SYSREG_H

// #include "type.h"
#define __stringify_1(x...)     #x
#define __stringify(x...)       __stringify_1(x)

/*
 * ARMv8 ARM reserves the following encoding for system registers:
 * (Ref: ARMv8 ARM, Section: "System instruction class encoding overview",
 *  C5.2, version:ARM DDI 0487A.f)
 *  [20-19] : Op0
 *  [18-16] : Op1
 *  [15-12] : CRn
 *  [11-8]  : CRm
 *  [7-5]   : Op2
 */
#define Op0_shift   19
#define Op0_mask    0x3
#define Op1_shift   16
#define Op1_mask    0x7
#define CRn_shift   12
#define CRn_mask    0xf
#define CRm_shift   8
#define CRm_mask    0xf
#define Op2_shift   5
#define Op2_mask    0x7

#define sys_reg(op0, op1, crn, crm, op2) \
    (((op0) << Op0_shift) | ((op1) << Op1_shift) | \
     ((crn) << CRn_shift) | ((crm) << CRm_shift) | \
     ((op2) << Op2_shift))

#define sys_insn    sys_reg

#define sys_reg_Op0(id)     (((id) >> Op0_shift) & Op0_mask)
#define sys_reg_Op1(id)     (((id) >> Op1_shift) & Op1_mask)
#define sys_reg_CRn(id)     (((id) >> CRn_shift) & CRn_mask)
#define sys_reg_CRm(id)     (((id) >> CRm_shift) & CRm_mask)
#define sys_reg_Op2(id)     (((id) >> Op2_shift) & Op2_mask)

#ifndef CONFIG_BROKEN_GAS_INST

#ifdef __ASSEMBLY__
// The space separator is omitted so that __emit_inst(x) can be parsed as
// either an assembler directive or an assembler macro argument.
#define __emit_inst(x)      .inst(x)
#else
#define __emit_inst(x)      ".inst " __stringify((x)) "\n\t"
#endif

#else  /* CONFIG_BROKEN_GAS_INST */

#ifndef CONFIG_CPU_BIG_ENDIAN
#define __INSTR_BSWAP(x)        (x)
#else  /* CONFIG_CPU_BIG_ENDIAN */
#define __INSTR_BSWAP(x)        ((((x) << 24) & 0xff000000)    | \
                                 (((x) <<  8) & 0x00ff0000)    | \
                                 (((x) >>  8) & 0x0000ff00)    | \
                                 (((x) >> 24) & 0x000000ff))
#endif    /* CONFIG_CPU_BIG_ENDIAN */

#ifdef __ASSEMBLY__
#define __emit_inst(x)      .long __INSTR_BSWAP(x)
#else  /* __ASSEMBLY__ */
#define __emit_inst(x)      ".long " __stringify(__INSTR_BSWAP(x)) "\n\t"
#endif    /* __ASSEMBLY__ */

#endif    /* CONFIG_BROKEN_GAS_INST */

/*
 * Instructions for modifying PSTATE fields.
 * As per Arm ARM for v8-A, Section "C.5.1.3 op0 == 0b00, architectural hints,
 * barriers and CLREX, and PSTATE access", ARM DDI 0487 C.a, system instructions
 * for accessing PSTATE fields have the following encoding:
 *  Op0 = 0, CRn = 4
 *  Op1, Op2 encodes the PSTATE field modified and defines the constraints.
 *  CRm = Imm4 for the instruction.
 *  Rt = 0x1f
 */
#define pstate_field(op1, op2)  ((op1) << Op1_shift | (op2) << Op2_shift)
#define PSTATE_Imm_shift        CRm_shift

#define PSTATE_PAN      pstate_field(0, 4)
#define PSTATE_UAO      pstate_field(0, 3)
#define PSTATE_SSBS     pstate_field(3, 1)
#define PSTATE_TCO      pstate_field(3, 4)

#define SET_PSTATE_PAN(x)       __emit_inst(0xd500401f | PSTATE_PAN | ((!!x) << PSTATE_Imm_shift))
#define SET_PSTATE_UAO(x)       __emit_inst(0xd500401f | PSTATE_UAO | ((!!x) << PSTATE_Imm_shift))
#define SET_PSTATE_SSBS(x)      __emit_inst(0xd500401f | PSTATE_SSBS | ((!!x) << PSTATE_Imm_shift))
#define SET_PSTATE_TCO(x)       __emit_inst(0xd500401f | PSTATE_TCO | ((!!x) << PSTATE_Imm_shift))

#define __SYS_BARRIER_INSN(CRm, op2, Rt) \
    __emit_inst(0xd5000000 | sys_insn(0, 3, 3, (CRm), (op2)) | ((Rt) & 0x1f))

#define SB_BARRIER_INSN     __SYS_BARRIER_INSN(0, 7, 31)

#define SYS_DC_ISW      sys_insn(1, 0, 7, 6, 2)
#define SYS_DC_CSW      sys_insn(1, 0, 7, 10, 2)
#define SYS_DC_CISW     sys_insn(1, 0, 7, 14, 2)

/*
 * System registers, organised loosely by encoding but grouped together
 * where the architected name contains an index. e.g. ID_MMFR<n>_EL1.
 */
#define SYS_OSDTRRX_EL1         sys_reg(2, 0, 0, 0, 2)
#define SYS_MDCCINT_EL1         sys_reg(2, 0, 0, 2, 0)
#define SYS_MDSCR_EL1           sys_reg(2, 0, 0, 2, 2)
#define SYS_OSDTRTX_EL1         sys_reg(2, 0, 0, 3, 2)
#define SYS_OSECCR_EL1          sys_reg(2, 0, 0, 6, 2)
#define SYS_DBGBVRn_EL1(n)      sys_reg(2, 0, 0, n, 4)
#define SYS_DBGBCRn_EL1(n)      sys_reg(2, 0, 0, n, 5)
#define SYS_DBGWVRn_EL1(n)      sys_reg(2, 0, 0, n, 6)
#define SYS_DBGWCRn_EL1(n)      sys_reg(2, 0, 0, n, 7)
#define SYS_MDRAR_EL1           sys_reg(2, 0, 1, 0, 0)
#define SYS_OSLAR_EL1           sys_reg(2, 0, 1, 0, 4)
#define SYS_OSLSR_EL1           sys_reg(2, 0, 1, 1, 4)
#define SYS_OSDLR_EL1           sys_reg(2, 0, 1, 3, 4)
#define SYS_DBGPRCR_EL1         sys_reg(2, 0, 1, 4, 4)
#define SYS_DBGCLAIMSET_EL1     sys_reg(2, 0, 7, 8, 6)
#define SYS_DBGCLAIMCLR_EL1     sys_reg(2, 0, 7, 9, 6)
#define SYS_DBGAUTHSTATUS_EL1   sys_reg(2, 0, 7, 14, 6)
#define SYS_MDCCSR_EL0          sys_reg(2, 3, 0, 1, 0)
#define SYS_DBGDTR_EL0          sys_reg(2, 3, 0, 4, 0)
#define SYS_DBGDTRRX_EL0        sys_reg(2, 3, 0, 5, 0)
#define SYS_DBGDTRTX_EL0        sys_reg(2, 3, 0, 5, 0)
#define SYS_DBGVCR32_EL2        sys_reg(2, 4, 0, 7, 0)

#define SYS_MIDR_EL1            sys_reg(3, 0, 0, 0, 0)
#define SYS_MPIDR_EL1           sys_reg(3, 0, 0, 0, 5)
#define SYS_REVIDR_EL1          sys_reg(3, 0, 0, 0, 6)

#define SYS_ID_PFR0_EL1         sys_reg(3, 0, 0, 1, 0)
#define SYS_ID_PFR1_EL1         sys_reg(3, 0, 0, 1, 1)
#define SYS_ID_PFR2_EL1         sys_reg(3, 0, 0, 3, 4)
#define SYS_ID_DFR0_EL1         sys_reg(3, 0, 0, 1, 2)
#define SYS_ID_DFR1_EL1         sys_reg(3, 0, 0, 3, 5)
#define SYS_ID_AFR0_EL1         sys_reg(3, 0, 0, 1, 3)
#define SYS_ID_MMFR0_EL1        sys_reg(3, 0, 0, 1, 4)
#define SYS_ID_MMFR1_EL1        sys_reg(3, 0, 0, 1, 5)
#define SYS_ID_MMFR2_EL1        sys_reg(3, 0, 0, 1, 6)
#define SYS_ID_MMFR3_EL1        sys_reg(3, 0, 0, 1, 7)
#define SYS_ID_MMFR4_EL1        sys_reg(3, 0, 0, 2, 6)
#define SYS_ID_MMFR5_EL1        sys_reg(3, 0, 0, 3, 6)

#define SYS_ID_ISAR0_EL1        sys_reg(3, 0, 0, 2, 0)
#define SYS_ID_ISAR1_EL1        sys_reg(3, 0, 0, 2, 1)
#define SYS_ID_ISAR2_EL1        sys_reg(3, 0, 0, 2, 2)
#define SYS_ID_ISAR3_EL1        sys_reg(3, 0, 0, 2, 3)
#define SYS_ID_ISAR4_EL1        sys_reg(3, 0, 0, 2, 4)
#define SYS_ID_ISAR5_EL1        sys_reg(3, 0, 0, 2, 5)
#define SYS_ID_ISAR6_EL1        sys_reg(3, 0, 0, 2, 7)

#define SYS_MVFR0_EL1           sys_reg(3, 0, 0, 3, 0)
#define SYS_MVFR1_EL1           sys_reg(3, 0, 0, 3, 1)
#define SYS_MVFR2_EL1           sys_reg(3, 0, 0, 3, 2)

#define SYS_ID_AA64PFR0_EL1     sys_reg(3, 0, 0, 4, 0)
#define SYS_ID_AA64PFR1_EL1     sys_reg(3, 0, 0, 4, 1)
#define SYS_ID_AA64ZFR0_EL1     sys_reg(3, 0, 0, 4, 4)

#define SYS_ID_AA64DFR0_EL1     sys_reg(3, 0, 0, 5, 0)
#define SYS_ID_AA64DFR1_EL1     sys_reg(3, 0, 0, 5, 1)

#define SYS_ID_AA64AFR0_EL1     sys_reg(3, 0, 0, 5, 4)
#define SYS_ID_AA64AFR1_EL1     sys_reg(3, 0, 0, 5, 5)

#define SYS_ID_AA64ISAR0_EL1    sys_reg(3, 0, 0, 6, 0)
#define SYS_ID_AA64ISAR1_EL1    sys_reg(3, 0, 0, 6, 1)
#define SYS_ID_AA64ISAR2_EL1    sys_reg(3, 0, 0, 6, 2)

#define SYS_ID_AA64MMFR0_EL1    sys_reg(3, 0, 0, 7, 0)
#define SYS_ID_AA64MMFR1_EL1    sys_reg(3, 0, 0, 7, 1)
#define SYS_ID_AA64MMFR2_EL1    sys_reg(3, 0, 0, 7, 2)
#define SYS_ID_AA64MMFR3_EL1    sys_reg(3, 0, 0, 7, 3)

#define SYS_SCTLR_EL1           sys_reg(3, 0, 1, 0, 0)
#define SYS_ACTLR_EL1           sys_reg(3, 0, 1, 0, 1)
#define SYS_CPACR_EL1           sys_reg(3, 0, 1, 0, 2)
#define SYS_RGSR_EL1            sys_reg(3, 0, 1, 0, 5)
#define SYS_GCR_EL1             sys_reg(3, 0, 1, 0, 6)

#define SYS_ZCR_EL1             sys_reg(3, 0, 1, 2, 0)

#define SYS_TTBR0_EL1           sys_reg(3, 0, 2, 0, 0)
#define SYS_TTBR1_EL1           sys_reg(3, 0, 2, 0, 1)
#define SYS_TCR_EL1             sys_reg(3, 0, 2, 0, 2)

#define SYS_APIAKEYLO_EL1       sys_reg(3, 0, 2, 1, 0)
#define SYS_APIAKEYHI_EL1       sys_reg(3, 0, 2, 1, 1)
#define SYS_APIBKEYLO_EL1       sys_reg(3, 0, 2, 1, 2)
#define SYS_APIBKEYHI_EL1       sys_reg(3, 0, 2, 1, 3)

#define SYS_APDAKEYLO_EL1       sys_reg(3, 0, 2, 2, 0)
#define SYS_APDAKEYHI_EL1       sys_reg(3, 0, 2, 2, 1)
#define SYS_APDBKEYLO_EL1       sys_reg(3, 0, 2, 2, 2)
#define SYS_APDBKEYHI_EL1       sys_reg(3, 0, 2, 2, 3)

#define SYS_APGAKEYLO_EL1       sys_reg(3, 0, 2, 3, 0)
#define SYS_APGAKEYHI_EL1       sys_reg(3, 0, 2, 3, 1)

#define SYS_SPSR_EL1            sys_reg(3, 0, 4, 0, 0)
#define SYS_ELR_EL1             sys_reg(3, 0, 4, 0, 1)

#define SYS_ICC_PMR_EL1         sys_reg(3, 0, 4, 6, 0)

#define SYS_AFSR0_EL1           sys_reg(3, 0, 5, 1, 0)
#define SYS_AFSR1_EL1           sys_reg(3, 0, 5, 1, 1)
#define SYS_ESR_EL1             sys_reg(3, 0, 5, 2, 0)

#define SYS_ERRIDR_EL1          sys_reg(3, 0, 5, 3, 0)
#define SYS_ERRSELR_EL1         sys_reg(3, 0, 5, 3, 1)
#define SYS_ERXFR_EL1           sys_reg(3, 0, 5, 4, 0)
#define SYS_ERXCTLR_EL1         sys_reg(3, 0, 5, 4, 1)
#define SYS_ERXSTATUS_EL1       sys_reg(3, 0, 5, 4, 2)
#define SYS_ERXADDR_EL1         sys_reg(3, 0, 5, 4, 3)
#define SYS_ERXMISC0_EL1        sys_reg(3, 0, 5, 5, 0)
#define SYS_ERXMISC1_EL1        sys_reg(3, 0, 5, 5, 1)
#define SYS_TFSR_EL1            sys_reg(3, 0, 5, 6, 0)
#define SYS_TFSRE0_EL1          sys_reg(3, 0, 5, 6, 1)

#define SYS_FAR_EL1             sys_reg(3, 0, 6, 0, 0)
#define SYS_PAR_EL1             sys_reg(3, 0, 7, 4, 0)

/*** Statistical Profiling Extension ***/
/* ID registers */
#define SYS_PMSIDR_EL1                  sys_reg(3, 0, 9, 9, 7)
#define SYS_PMSIDR_EL1_FE_SHIFT         0
#define SYS_PMSIDR_EL1_FT_SHIFT         1
#define SYS_PMSIDR_EL1_FL_SHIFT         2
#define SYS_PMSIDR_EL1_ARCHINST_SHIFT   3
#define SYS_PMSIDR_EL1_LDS_SHIFT        4
#define SYS_PMSIDR_EL1_ERND_SHIFT       5
#define SYS_PMSIDR_EL1_INTERVAL_SHIFT   8
#define SYS_PMSIDR_EL1_INTERVAL_MASK    UL(0xf)
#define SYS_PMSIDR_EL1_MAXSIZE_SHIFT    12
#define SYS_PMSIDR_EL1_MAXSIZE_MASK     UL(0xf)
#define SYS_PMSIDR_EL1_COUNTSIZE_SHIFT  16
#define SYS_PMSIDR_EL1_COUNTSIZE_MASK   UL(0xf)

#define SYS_PMBIDR_EL1                  sys_reg(3, 0, 9, 10, 7)
#define SYS_PMBIDR_EL1_ALIGN_SHIFT      0
#define SYS_PMBIDR_EL1_ALIGN_MASK       U(0xf)
#define SYS_PMBIDR_EL1_P_SHIFT          4
#define SYS_PMBIDR_EL1_F_SHIFT          5

/* Sampling controls */
#define SYS_PMSCR_EL1                   sys_reg(3, 0, 9, 9, 0)
#define SYS_PMSCR_EL1_E0SPE_SHIFT       0
#define SYS_PMSCR_EL1_E1SPE_SHIFT       1
#define SYS_PMSCR_EL1_CX_SHIFT          3
#define SYS_PMSCR_EL1_PA_SHIFT          4
#define SYS_PMSCR_EL1_TS_SHIFT          5
#define SYS_PMSCR_EL1_PCT_SHIFT         6

#define SYS_PMSCR_EL2                   sys_reg(3, 4, 9, 9, 0)
#define SYS_PMSCR_EL2_E0HSPE_SHIFT      0
#define SYS_PMSCR_EL2_E2SPE_SHIFT       1
#define SYS_PMSCR_EL2_CX_SHIFT          3
#define SYS_PMSCR_EL2_PA_SHIFT          4
#define SYS_PMSCR_EL2_TS_SHIFT          5
#define SYS_PMSCR_EL2_PCT_SHIFT         6

#define SYS_PMSICR_EL1                  sys_reg(3, 0, 9, 9, 2)

#define SYS_PMSIRR_EL1                  sys_reg(3, 0, 9, 9, 3)
#define SYS_PMSIRR_EL1_RND_SHIFT        0
#define SYS_PMSIRR_EL1_INTERVAL_SHIFT   8
#define SYS_PMSIRR_EL1_INTERVAL_MASK    UL(0xffffff)

/* Filtering controls */
#define SYS_PMSFCR_EL1                  sys_reg(3, 0, 9, 9, 4)
#define SYS_PMSFCR_EL1_FE_SHIFT         0
#define SYS_PMSFCR_EL1_FT_SHIFT         1
#define SYS_PMSFCR_EL1_FL_SHIFT         2
#define SYS_PMSFCR_EL1_B_SHIFT          16
#define SYS_PMSFCR_EL1_LD_SHIFT         17
#define SYS_PMSFCR_EL1_ST_SHIFT         18

#define SYS_PMSEVFR_EL1                 sys_reg(3, 0, 9, 9, 5)
#define SYS_PMSEVFR_EL1_RES0            UL(0x0000ffff00ff0f55)

#define SYS_PMSLATFR_EL1                sys_reg(3, 0, 9, 9, 6)
#define SYS_PMSLATFR_EL1_MINLAT_SHIFT   0

/* Buffer controls */
#define SYS_PMBLIMITR_EL1               sys_reg(3, 0, 9, 10, 0)
#define SYS_PMBLIMITR_EL1_E_SHIFT       0
#define SYS_PMBLIMITR_EL1_FM_SHIFT      1
#define SYS_PMBLIMITR_EL1_FM_MASK       UL(0x3)
#define SYS_PMBLIMITR_EL1_FM_STOP_IRQ   (0 << SYS_PMBLIMITR_EL1_FM_SHIFT)

#define SYS_PMBPTR_EL1                  sys_reg(3, 0, 9, 10, 1)

/* Buffer error reporting */
#define SYS_PMBSR_EL1                   sys_reg(3, 0, 9, 10, 3)
#define SYS_PMBSR_EL1_COLL_SHIFT        16
#define SYS_PMBSR_EL1_S_SHIFT           17
#define SYS_PMBSR_EL1_EA_SHIFT          18
#define SYS_PMBSR_EL1_DL_SHIFT          19
#define SYS_PMBSR_EL1_EC_SHIFT          26
#define SYS_PMBSR_EL1_EC_MASK           UL(0x3f)

#define SYS_PMBSR_EL1_EC_BUF            (UL(0x0) << SYS_PMBSR_EL1_EC_SHIFT)
#define SYS_PMBSR_EL1_EC_FAULT_S1       (UL(0x24) << SYS_PMBSR_EL1_EC_SHIFT)
#define SYS_PMBSR_EL1_EC_FAULT_S2       (UL(0x25) << SYS_PMBSR_EL1_EC_SHIFT)

#define SYS_PMBSR_EL1_FAULT_FSC_SHIFT   0
#define SYS_PMBSR_EL1_FAULT_FSC_MASK    UL(0x3f)

#define SYS_PMBSR_EL1_BUF_BSC_SHIFT     0
#define SYS_PMBSR_EL1_BUF_BSC_MASK      UL(0x3f)

#define SYS_PMBSR_EL1_BUF_BSC_FULL      (UL(0x1) << SYS_PMBSR_EL1_BUF_BSC_SHIFT)

/*** End of Statistical Profiling Extension ***/

#define SYS_PMINTENSET_EL1          sys_reg(3, 0, 9, 14, 1)
#define SYS_PMINTENCLR_EL1          sys_reg(3, 0, 9, 14, 2)

#define SYS_PMMIR_EL1               sys_reg(3, 0, 9, 14, 6)

#define SYS_MAIR_EL1                sys_reg(3, 0, 10, 2, 0)
#define SYS_AMAIR_EL1               sys_reg(3, 0, 10, 3, 0)

#define SYS_LORSA_EL1               sys_reg(3, 0, 10, 4, 0)
#define SYS_LOREA_EL1               sys_reg(3, 0, 10, 4, 1)
#define SYS_LORN_EL1                sys_reg(3, 0, 10, 4, 2)
#define SYS_LORC_EL1                sys_reg(3, 0, 10, 4, 3)
#define SYS_LORID_EL1               sys_reg(3, 0, 10, 4, 7)

#define SYS_VBAR_EL1                sys_reg(3, 0, 12, 0, 0)
#define SYS_DISR_EL1                sys_reg(3, 0, 12, 1, 1)

#define SYS_ICC_IAR0_EL1            sys_reg(3, 0, 12, 8, 0)
#define SYS_ICC_EOIR0_EL1           sys_reg(3, 0, 12, 8, 1)
#define SYS_ICC_HPPIR0_EL1          sys_reg(3, 0, 12, 8, 2)
#define SYS_ICC_BPR0_EL1            sys_reg(3, 0, 12, 8, 3)
#define SYS_ICC_AP0Rn_EL1(n)        sys_reg(3, 0, 12, 8, 4 | n)
#define SYS_ICC_AP0R0_EL1           SYS_ICC_AP0Rn_EL1(0)
#define SYS_ICC_AP0R1_EL1           SYS_ICC_AP0Rn_EL1(1)
#define SYS_ICC_AP0R2_EL1           SYS_ICC_AP0Rn_EL1(2)
#define SYS_ICC_AP0R3_EL1           SYS_ICC_AP0Rn_EL1(3)
#define SYS_ICC_AP1Rn_EL1(n)        sys_reg(3, 0, 12, 9, n)
#define SYS_ICC_AP1R0_EL1           SYS_ICC_AP1Rn_EL1(0)
#define SYS_ICC_AP1R1_EL1           SYS_ICC_AP1Rn_EL1(1)
#define SYS_ICC_AP1R2_EL1           SYS_ICC_AP1Rn_EL1(2)
#define SYS_ICC_AP1R3_EL1           SYS_ICC_AP1Rn_EL1(3)
#define SYS_ICC_DIR_EL1             sys_reg(3, 0, 12, 11, 1)
#define SYS_ICC_RPR_EL1             sys_reg(3, 0, 12, 11, 3)
#define SYS_ICC_SGI1R_EL1           sys_reg(3, 0, 12, 11, 5)
#define SYS_ICC_ASGI1R_EL1          sys_reg(3, 0, 12, 11, 6)
#define SYS_ICC_SGI0R_EL1           sys_reg(3, 0, 12, 11, 7)
#define SYS_ICC_IAR0_EL1            sys_reg(3, 0, 12, 8,  0)
#define SYS_ICC_IAR1_EL1            sys_reg(3, 0, 12, 12, 0)
#define SYS_ICC_EOIR1_EL1           sys_reg(3, 0, 12, 12, 1)
#define SYS_ICC_HPPIR1_EL1          sys_reg(3, 0, 12, 12, 2)
#define SYS_ICC_BPR1_EL1            sys_reg(3, 0, 12, 12, 3)
#define SYS_ICC_CTLR_EL1            sys_reg(3, 0, 12, 12, 4)
#define SYS_ICC_SRE_EL1             sys_reg(3, 0, 12, 12, 5)
#define SYS_ICC_IGRPEN0_EL1         sys_reg(3, 0, 12, 12, 6)
#define SYS_ICC_IGRPEN1_EL1         sys_reg(3, 0, 12, 12, 7)

#define SYS_CONTEXTIDR_EL1          sys_reg(3, 0, 13, 0, 1)
#define SYS_TPIDR_EL1               sys_reg(3, 0, 13, 0, 4)

#define SYS_SCXTNUM_EL1             sys_reg(3, 0, 13, 0, 7)

#define SYS_CNTKCTL_EL1             sys_reg(3, 0, 14, 1, 0)

#define SYS_CCSIDR_EL1              sys_reg(3, 1, 0, 0, 0)
#define SYS_CLIDR_EL1               sys_reg(3, 1, 0, 0, 1)
#define SYS_GMID_EL1                sys_reg(3, 1, 0, 0, 4)
#define SYS_AIDR_EL1                sys_reg(3, 1, 0, 0, 7)

#define SYS_CSSELR_EL1              sys_reg(3, 2, 0, 0, 0)

#define SYS_CTR_EL0                 sys_reg(3, 3, 0, 0, 1)
#define SYS_DCZID_EL0               sys_reg(3, 3, 0, 0, 7)

#define SYS_RNDR_EL0                sys_reg(3, 3, 2, 4, 0)
#define SYS_RNDRRS_EL0              sys_reg(3, 3, 2, 4, 1)

#define SYS_PMCR_EL0                sys_reg(3, 3, 9, 12, 0)
#define SYS_PMCNTENSET_EL0          sys_reg(3, 3, 9, 12, 1)
#define SYS_PMCNTENCLR_EL0          sys_reg(3, 3, 9, 12, 2)
#define SYS_PMOVSCLR_EL0            sys_reg(3, 3, 9, 12, 3)
#define SYS_PMSWINC_EL0             sys_reg(3, 3, 9, 12, 4)
#define SYS_PMSELR_EL0              sys_reg(3, 3, 9, 12, 5)
#define SYS_PMCEID0_EL0             sys_reg(3, 3, 9, 12, 6)
#define SYS_PMCEID1_EL0             sys_reg(3, 3, 9, 12, 7)
#define SYS_PMCCNTR_EL0             sys_reg(3, 3, 9, 13, 0)
#define SYS_PMXEVTYPER_EL0          sys_reg(3, 3, 9, 13, 1)
#define SYS_PMXEVCNTR_EL0           sys_reg(3, 3, 9, 13, 2)
#define SYS_PMUSERENR_EL0           sys_reg(3, 3, 9, 14, 0)
#define SYS_PMOVSSET_EL0            sys_reg(3, 3, 9, 14, 3)

#define SYS_TPIDR_EL0               sys_reg(3, 3, 13, 0, 2)
#define SYS_TPIDRRO_EL0             sys_reg(3, 3, 13, 0, 3)

#define SYS_SCXTNUM_EL0             sys_reg(3, 3, 13, 0, 7)

/* Definitions for system register interface to AMU for ARMv8.4 onwards */
#define SYS_AM_EL0(crm, op2)        sys_reg(3, 3, 13, (crm), (op2))
#define SYS_AMCR_EL0                SYS_AM_EL0(2, 0)
#define SYS_AMCFGR_EL0              SYS_AM_EL0(2, 1)
#define SYS_AMCGCR_EL0              SYS_AM_EL0(2, 2)
#define SYS_AMUSERENR_EL0           SYS_AM_EL0(2, 3)
#define SYS_AMCNTENCLR0_EL0         SYS_AM_EL0(2, 4)
#define SYS_AMCNTENSET0_EL0         SYS_AM_EL0(2, 5)
#define SYS_AMCNTENCLR1_EL0         SYS_AM_EL0(3, 0)
#define SYS_AMCNTENSET1_EL0         SYS_AM_EL0(3, 1)

/*
 * Group 0 of activity monitors (architected):
 *                op0  op1  CRn   CRm       op2
 * Counter:       11   011  1101  010:n<3>  n<2:0>
 * Type:          11   011  1101  011:n<3>  n<2:0>
 * n: 0-15
 *
 * Group 1 of activity monitors (auxiliary):
 *                op0  op1  CRn   CRm       op2
 * Counter:       11   011  1101  110:n<3>  n<2:0>
 * Type:          11   011  1101  111:n<3>  n<2:0>
 * n: 0-15
 */

#define SYS_AMEVCNTR0_EL0(n)        SYS_AM_EL0(4 + ((n) >> 3), (n) & 7)
#define SYS_AMEVTYPER0_EL0(n)       SYS_AM_EL0(6 + ((n) >> 3), (n) & 7)
#define SYS_AMEVCNTR1_EL0(n)        SYS_AM_EL0(12 + ((n) >> 3), (n) & 7)
#define SYS_AMEVTYPER1_EL0(n)       SYS_AM_EL0(14 + ((n) >> 3), (n) & 7)

/* AMU v1: Fixed (architecturally defined) activity monitors */
#define SYS_AMEVCNTR0_CORE_EL0      SYS_AMEVCNTR0_EL0(0)
#define SYS_AMEVCNTR0_CONST_EL0     SYS_AMEVCNTR0_EL0(1)
#define SYS_AMEVCNTR0_INST_RET_EL0  SYS_AMEVCNTR0_EL0(2)
#define SYS_AMEVCNTR0_MEM_STALL     SYS_AMEVCNTR0_EL0(3)

/* AMCNTENSET0_EL0 definitions */
#define AMCNTENSET0_EL0_Pn_SHIFT	U(0)
#define AMCNTENSET0_EL0_Pn_MASK		ULL(0xffff)

/* AMCNTENSET1_EL0 definitions */
#define AMCNTENSET1_EL0_Pn_SHIFT	U(0)
#define AMCNTENSET1_EL0_Pn_MASK		ULL(0xffff)

/* AMCNTENCLR0_EL0 definitions */
#define AMCNTENCLR0_EL0_Pn_SHIFT	U(0)
#define AMCNTENCLR0_EL0_Pn_MASK		ULL(0xffff)

/* AMCNTENCLR1_EL0 definitions */
#define AMCNTENCLR1_EL0_Pn_SHIFT	U(0)
#define AMCNTENCLR1_EL0_Pn_MASK		ULL(0xffff)

/* AMCFGR_EL0 definitions */
#define AMCFGR_EL0_NCG_SHIFT	U(28)
#define AMCFGR_EL0_NCG_MASK	U(0xf)
#define AMCFGR_EL0_N_SHIFT	U(0)
#define AMCFGR_EL0_N_MASK	U(0xff)

/* AMCGCR_EL0 definitions */
#define AMCGCR_EL0_CG0NC_SHIFT	U(0)
#define AMCGCR_EL0_CG0NC_MASK	U(0xff)
#define AMCGCR_EL0_CG1NC_SHIFT	U(8)
#define AMCGCR_EL0_CG1NC_MASK	U(0xff)

#define SYS_CNTFRQ_EL0              sys_reg(3, 3, 14, 0, 0)

#define SYS_CNTP_TVAL_EL0           sys_reg(3, 3, 14, 2, 0)
#define SYS_CNTP_CTL_EL0            sys_reg(3, 3, 14, 2, 1)
#define SYS_CNTP_CVAL_EL0           sys_reg(3, 3, 14, 2, 2)

#define SYS_CNTV_CTL_EL0            sys_reg(3, 3, 14, 3, 1)
#define SYS_CNTV_CVAL_EL0           sys_reg(3, 3, 14, 3, 2)

#define SYS_AARCH32_CNTP_TVAL       sys_reg(0, 0, 14, 2, 0)
#define SYS_AARCH32_CNTP_CTL        sys_reg(0, 0, 14, 2, 1)
#define SYS_AARCH32_CNTP_CVAL       sys_reg(0, 2, 0, 14, 0)

#define __PMEV_op2(n)               ((n) & 0x7)
#define __CNTR_CRm(n)               (0x8 | (((n) >> 3) & 0x3))
#define SYS_PMEVCNTRn_EL0(n)        sys_reg(3, 3, 14, __CNTR_CRm(n), __PMEV_op2(n))
#define __TYPER_CRm(n)              (0xc | (((n) >> 3) & 0x3))
#define SYS_PMEVTYPERn_EL0(n)       sys_reg(3, 3, 14, __TYPER_CRm(n), __PMEV_op2(n))

#define SYS_PMCCFILTR_EL0           sys_reg(3, 3, 14, 15, 7)

#define SYS_ZCR_EL2                 sys_reg(3, 4, 1, 2, 0)
#define SYS_DACR32_EL2              sys_reg(3, 4, 3, 0, 0)
#define SYS_SPSR_EL2                sys_reg(3, 4, 4, 0, 0)
#define SYS_ELR_EL2                 sys_reg(3, 4, 4, 0, 1)
#define SYS_IFSR32_EL2              sys_reg(3, 4, 5, 0, 1)
#define SYS_ESR_EL2                 sys_reg(3, 4, 5, 2, 0)
#define SYS_VSESR_EL2               sys_reg(3, 4, 5, 2, 3)
#define SYS_FPEXC32_EL2             sys_reg(3, 4, 5, 3, 0)
#define SYS_TFSR_EL2                sys_reg(3, 4, 5, 6, 0)
#define SYS_FAR_EL2                 sys_reg(3, 4, 6, 0, 0)

#define SYS_VDISR_EL2               sys_reg(3, 4, 12, 1,  1)
#define __SYS__AP0Rx_EL2(x)         sys_reg(3, 4, 12, 8, x)
#define SYS_ICH_AP0R0_EL2           __SYS__AP0Rx_EL2(0)
#define SYS_ICH_AP0R1_EL2           __SYS__AP0Rx_EL2(1)
#define SYS_ICH_AP0R2_EL2           __SYS__AP0Rx_EL2(2)
#define SYS_ICH_AP0R3_EL2           __SYS__AP0Rx_EL2(3)

#define __SYS__AP1Rx_EL2(x)         sys_reg(3, 4, 12, 9, x)
#define SYS_ICH_AP1R0_EL2           __SYS__AP1Rx_EL2(0)
#define SYS_ICH_AP1R1_EL2           __SYS__AP1Rx_EL2(1)
#define SYS_ICH_AP1R2_EL2           __SYS__AP1Rx_EL2(2)
#define SYS_ICH_AP1R3_EL2           __SYS__AP1Rx_EL2(3)

#define SYS_ICH_VSEIR_EL2           sys_reg(3, 4, 12, 9, 4)
#define SYS_ICC_SRE_EL2             sys_reg(3, 4, 12, 9, 5)
#define SYS_ICH_HCR_EL2             sys_reg(3, 4, 12, 11, 0)
#define SYS_ICH_VTR_EL2             sys_reg(3, 4, 12, 11, 1)
#define SYS_ICH_MISR_EL2            sys_reg(3, 4, 12, 11, 2)
#define SYS_ICH_EISR_EL2            sys_reg(3, 4, 12, 11, 3)
#define SYS_ICH_ELRSR_EL2           sys_reg(3, 4, 12, 11, 5)
#define SYS_ICH_VMCR_EL2            sys_reg(3, 4, 12, 11, 7)

#define __SYS__LR0_EL2(x)           sys_reg(3, 4, 12, 12, x)
#define SYS_ICH_LR0_EL2             __SYS__LR0_EL2(0)
#define SYS_ICH_LR1_EL2             __SYS__LR0_EL2(1)
#define SYS_ICH_LR2_EL2             __SYS__LR0_EL2(2)
#define SYS_ICH_LR3_EL2             __SYS__LR0_EL2(3)
#define SYS_ICH_LR4_EL2             __SYS__LR0_EL2(4)
#define SYS_ICH_LR5_EL2             __SYS__LR0_EL2(5)
#define SYS_ICH_LR6_EL2             __SYS__LR0_EL2(6)
#define SYS_ICH_LR7_EL2             __SYS__LR0_EL2(7)

#define __SYS__LR8_EL2(x)           sys_reg(3, 4, 12, 13, x)
#define SYS_ICH_LR8_EL2             __SYS__LR8_EL2(0)
#define SYS_ICH_LR9_EL2             __SYS__LR8_EL2(1)
#define SYS_ICH_LR10_EL2            __SYS__LR8_EL2(2)
#define SYS_ICH_LR11_EL2            __SYS__LR8_EL2(3)
#define SYS_ICH_LR12_EL2            __SYS__LR8_EL2(4)
#define SYS_ICH_LR13_EL2            __SYS__LR8_EL2(5)
#define SYS_ICH_LR14_EL2            __SYS__LR8_EL2(6)
#define SYS_ICH_LR15_EL2            __SYS__LR8_EL2(7)

/* VHE encodings for architectural EL0/1 system registers */
#define SYS_SCTLR_EL12              sys_reg(3, 5, 1, 0, 0)
#define SYS_CPACR_EL12              sys_reg(3, 5, 1, 0, 2)
#define SYS_ZCR_EL12                sys_reg(3, 5, 1, 2, 0)
#define SYS_TTBR0_EL12              sys_reg(3, 5, 2, 0, 0)
#define SYS_TTBR1_EL12              sys_reg(3, 5, 2, 0, 1)
#define SYS_TCR_EL12                sys_reg(3, 5, 2, 0, 2)
#define SYS_SPSR_EL12               sys_reg(3, 5, 4, 0, 0)
#define SYS_ELR_EL12                sys_reg(3, 5, 4, 0, 1)
#define SYS_AFSR0_EL12              sys_reg(3, 5, 5, 1, 0)
#define SYS_AFSR1_EL12              sys_reg(3, 5, 5, 1, 1)
#define SYS_ESR_EL12                sys_reg(3, 5, 5, 2, 0)
#define SYS_TFSR_EL12               sys_reg(3, 5, 5, 6, 0)
#define SYS_FAR_EL12                sys_reg(3, 5, 6, 0, 0)
#define SYS_MAIR_EL12               sys_reg(3, 5, 10, 2, 0)
#define SYS_AMAIR_EL12              sys_reg(3, 5, 10, 3, 0)
#define SYS_VBAR_EL12               sys_reg(3, 5, 12, 0, 0)
#define SYS_CONTEXTIDR_EL12         sys_reg(3, 5, 13, 0, 1)
#define SYS_CNTKCTL_EL12            sys_reg(3, 5, 14, 1, 0)
#define SYS_CNTP_TVAL_EL02          sys_reg(3, 5, 14, 2, 0)
#define SYS_CNTP_CTL_EL02           sys_reg(3, 5, 14, 2, 1)
#define SYS_CNTP_CVAL_EL02          sys_reg(3, 5, 14, 2, 2)
#define SYS_CNTV_TVAL_EL02          sys_reg(3, 5, 14, 3, 0)
#define SYS_CNTV_CTL_EL02           sys_reg(3, 5, 14, 3, 1)
#define SYS_CNTV_CVAL_EL02          sys_reg(3, 5, 14, 3, 2)

/* MIDR_EL1 bit definitions */
#define MIDR_VAR_SHIFT      U(20)
#define MIDR_VAR_BITS       U(4)
#define MIDR_VAR_MASK       U(0xf)

#define MIDR_IMPL_SHIFT     U(0x18)
#define MIDR_IMPL_MASK      U(0xff)

#define MIDR_PN_SHIFT       U(0x4)
#define MIDR_PN_MASK        U(0xfff)

#define MIDR_REV_SHIFT      U(0)
#define MIDR_REV_BITS       U(4)
#define MIDR_REV_MASK       U(0xf)

/* MPIDR_EL1 bit definitions */
#define MPIDR_AFF0_SHIFT        U(0)
#define MPIDR_AFF1_SHIFT        U(8)
#define MPIDR_AFF2_SHIFT        U(16)
#define MPIDR_AFF3_SHIFT        U(32)
#define MPIDR_AFFINITY_BITS     U(8)
#define MPIDR_AFFINITY_MASK     ULL(0xff00ffffff)
#define MPIDR_AFF_SHIFT(_n)     MPIDR_AFF##_n##_SHIFT


#define MPIDR_AFFLVL_SHIFT      U(3)
#define MPIDR_AFFLVL_MASK       ULL(0xff)
#define MPIDR_AFFLVL0           ULL(0x0)
#define MPIDR_AFFLVL1           ULL(0x1)
#define MPIDR_AFFLVL2           ULL(0x2)
#define MPIDR_AFFLVL3           ULL(0x3)
#define MPIDR_AFFLVL(_n)        MPIDR_AFFLVL##_n

#define MPIDR_MT_MASK           (ULL(1) << 24)
#define MPIDR_CPU_MASK          MPIDR_AFFLVL_MASK
#define MPIDR_CLUSTER_MASK      (MPIDR_AFFLVL_MASK << MPIDR_AFFINITY_BITS)

#define MPIDR_AFFLVL0_VAL(mpidr)    (((mpidr) >> MPIDR_AFF0_SHIFT) & MPIDR_AFFLVL_MASK)
#define MPIDR_AFFLVL1_VAL(mpidr)    (((mpidr) >> MPIDR_AFF1_SHIFT) & MPIDR_AFFLVL_MASK)
#define MPIDR_AFFLVL2_VAL(mpidr)    (((mpidr) >> MPIDR_AFF2_SHIFT) & MPIDR_AFFLVL_MASK)
#define MPIDR_AFFLVL3_VAL(mpidr)    (((mpidr) >> MPIDR_AFF3_SHIFT) & MPIDR_AFFLVL_MASK)

/*
 * The MPIDR_MAX_AFFLVL count starts from 0. Take care to
 * add one while using this macro to define array sizes.
 * TODO: Support only the first 3 affinity levels for now.
 */
#define MPIDR_MAX_AFFLVL    U(2)

#define MPID_MASK           (MPIDR_MT_MASK                 | \
                             (MPIDR_AFFLVL_MASK << MPIDR_AFF3_SHIFT) | \
                             (MPIDR_AFFLVL_MASK << MPIDR_AFF2_SHIFT) | \
                             (MPIDR_AFFLVL_MASK << MPIDR_AFF1_SHIFT) | \
                             (MPIDR_AFFLVL_MASK << MPIDR_AFF0_SHIFT))

#define MPIDR_AFF_ID(mpid, n)   (((mpid) >> MPIDR_AFF_SHIFT(n)) & MPIDR_AFFLVL_MASK)

/*
 * An invalid MPID. This value can be used by functions that return an MPID to
 * indicate an error.
 */
#define INVALID_MPID        U(0xFFFFFFFF)

/* Common SCTLR_ELx flags. */
#define SCTLR_ELx_DSSBS     (BIT(44))
#define SCTLR_ELx_ATA       (BIT(43))

#define SCTLR_ELx_TCF_SHIFT 40
#define SCTLR_ELx_TCF_NONE  (UL(0x0) << SCTLR_ELx_TCF_SHIFT)
#define SCTLR_ELx_TCF_SYNC  (UL(0x1) << SCTLR_ELx_TCF_SHIFT)
#define SCTLR_ELx_TCF_ASYNC (UL(0x2) << SCTLR_ELx_TCF_SHIFT)
#define SCTLR_ELx_TCF_MASK  (UL(0x3) << SCTLR_ELx_TCF_SHIFT)

#define SCTLR_ELx_ITFSB     (BIT(37))
#define SCTLR_ELx_ENIA      (BIT(31))
#define SCTLR_ELx_ENIB      (BIT(30))
#define SCTLR_ELx_ENDA      (BIT(27))
#define SCTLR_ELx_EE        (BIT(25))
#define SCTLR_ELx_IESB      (BIT(21))
#define SCTLR_ELx_WXN       (BIT(19))
#define SCTLR_ELx_ENDB      (BIT(13))
#define SCTLR_ELx_I         (BIT(12))
#define SCTLR_ELx_SA        (BIT(3))
#define SCTLR_ELx_C         (BIT(2))
#define SCTLR_ELx_A         (BIT(1))
#define SCTLR_ELx_M         (BIT(0))

#define SCTLR_ELx_FLAGS     (SCTLR_ELx_M  | SCTLR_ELx_A | SCTLR_ELx_C | \
                             SCTLR_ELx_SA | SCTLR_ELx_I | SCTLR_ELx_IESB)

/* SCTLR definitions */
#define SCTLR_EL2_RES1      ((BIT(4))  | (BIT(5))  | (BIT(11)) | (BIT(16)) | \
                             (BIT(18)) | (BIT(22)) | (BIT(23)) | (BIT(28)) | \
                             (BIT(29)))

#ifdef CONFIG_CPU_BIG_ENDIAN
#define ENDIAN_SET_EL2      SCTLR_ELx_EE
#else
#define ENDIAN_SET_EL2      0
#endif

/* SCTLR_EL1 specific flags. */
#define SCTLR_EL1_ATA0          (BIT(42))

#define SCTLR_EL1_TCF0_SHIFT    38
#define SCTLR_EL1_TCF0_NONE     (UL(0x0) << SCTLR_EL1_TCF0_SHIFT)
#define SCTLR_EL1_TCF0_SYNC     (UL(0x1) << SCTLR_EL1_TCF0_SHIFT)
#define SCTLR_EL1_TCF0_ASYNC    (UL(0x2) << SCTLR_EL1_TCF0_SHIFT)
#define SCTLR_EL1_TCF0_MASK     (UL(0x3) << SCTLR_EL1_TCF0_SHIFT)

#define SCTLR_EL1_BT1           (BIT(36))
#define SCTLR_EL1_BT0           (BIT(35))
#define SCTLR_EL1_UCI           (BIT(26))
#define SCTLR_EL1_E0E           (BIT(24))
#define SCTLR_EL1_SPAN          (BIT(23))
#define SCTLR_EL1_NTWE          (BIT(18))
#define SCTLR_EL1_NTWI          (BIT(16))
#define SCTLR_EL1_UCT           (BIT(15))
#define SCTLR_EL1_DZE           (BIT(14))
#define SCTLR_EL1_UMA           (BIT(9))
#define SCTLR_EL1_SED           (BIT(8))
#define SCTLR_EL1_ITD           (BIT(7))
#define SCTLR_EL1_CP15BEN       (BIT(5))
#define SCTLR_EL1_SA0           (BIT(4))

#define SCTLR_EL1_RES1    ((BIT(11)) | (BIT(20)) | (BIT(22)) | (BIT(28)) | (BIT(29)))

#ifdef CONFIG_CPU_BIG_ENDIAN
#define ENDIAN_SET_EL1        (SCTLR_EL1_E0E | SCTLR_ELx_EE)
#else
#define ENDIAN_SET_EL1        0
#endif

#define SCTLR_EL1_SET   (SCTLR_ELx_M    | SCTLR_ELx_C    | SCTLR_ELx_SA   |\
                         SCTLR_EL1_SA0  | SCTLR_EL1_SED  | SCTLR_ELx_I    |\
                         SCTLR_EL1_DZE  | SCTLR_EL1_UCT                   |\
                         SCTLR_EL1_NTWE | SCTLR_ELx_IESB | SCTLR_EL1_SPAN |\
                         SCTLR_ELx_ITFSB| SCTLR_ELx_ATA  | SCTLR_EL1_ATA0 |\
                         ENDIAN_SET_EL1 | SCTLR_EL1_UCI  | SCTLR_EL1_RES1)

/* MAIR_ELx memory attributes (used by Linux) */
#define MAIR_ATTR_DEVICE_nGnRnE     UL(0x00)
#define MAIR_ATTR_DEVICE_nGnRE      UL(0x04)
#define MAIR_ATTR_DEVICE_GRE        UL(0x0c)
#define MAIR_ATTR_NORMAL_NC         UL(0x44)
#define MAIR_ATTR_NORMAL_WT         UL(0xbb)
#define MAIR_ATTR_NORMAL_TAGGED     UL(0xf0)
#define MAIR_ATTR_NORMAL            UL(0xff)
#define MAIR_ATTR_MASK              UL(0xff)

/* Position the attr at the correct index */
#define MAIR_ATTRIDX(attr, idx)        ((attr) << ((idx) * 8))

/* ID_AA64ISAR0_EL1 definitions */
#define ID_AA64ISAR0_RNDR_SHIFT     60
#define ID_AA64ISAR0_RNDR_MASK      UL(0xf)

#define ID_AA64ISAR0_TLB_SHIFT      56
#define ID_AA64ISAR0_TLB_RANGE_NI   0x0
#define ID_AA64ISAR0_TLB_RANGE      0x2

#define ID_AA64ISAR0_TS_SHIFT       52
#define ID_AA64ISAR0_FHM_SHIFT      48
#define ID_AA64ISAR0_DP_SHIFT       44
#define ID_AA64ISAR0_SM4_SHIFT      40
#define ID_AA64ISAR0_SM3_SHIFT      36
#define ID_AA64ISAR0_SHA3_SHIFT     32
#define ID_AA64ISAR0_RDM_SHIFT      28
#define ID_AA64ISAR0_ATOMICS_SHIFT  20
#define ID_AA64ISAR0_CRC32_SHIFT    16
#define ID_AA64ISAR0_SHA2_SHIFT     12
#define ID_AA64ISAR0_SHA1_SHIFT     8
#define ID_AA64ISAR0_AES_SHIFT      4

/* ID_AA64ISAR1_EL1 definitions */
#define ID_AA64ISAR1_I8MM_SHIFT                 52
#define ID_AA64ISAR1_DGH_SHIFT                  48
#define ID_AA64ISAR1_BF16_SHIFT                 44
#define ID_AA64ISAR1_SPECRES_SHIFT              40

#define ID_AA64ISAR1_SB_SHIFT                   U(36)
#define ID_AA64ISAR1_SB_MASK                    ULL(0xf)
#define ID_AA64ISAR1_SB_SUPPORTED               ULL(0x1)
#define ID_AA64ISAR1_SB_NOT_SUPPORTED           ULL(0x0)

#define ID_AA64ISAR1_FRINTTS_SHIFT              32

#define ID_AA64ISAR1_GPI_SHIFT                  U(28)
#define ID_AA64ISAR1_GPI_MASK                   ULL(0xf)
#define ID_AA64ISAR1_GPI_NI                     0x0
#define ID_AA64ISAR1_GPI_IMP_DEF                0x1

#define ID_AA64ISAR1_GPA_SHIFT                  U(24)
#define ID_AA64ISAR1_GPA_MASK                   ULL(0xf)
#define ID_AA64ISAR1_GPA_NI                     0x0
#define ID_AA64ISAR1_GPA_ARCHITECTED            0x1

#define ID_AA64ISAR1_LRCPC_SHIFT                20
#define ID_AA64ISAR1_FCMA_SHIFT                 16
#define ID_AA64ISAR1_JSCVT_SHIFT                12

#define ID_AA64ISAR1_API_SHIFT                  U(8)
#define ID_AA64ISAR1_API_MASK                   ULL(0xf)
#define ID_AA64ISAR1_API_NI                     0x0
#define ID_AA64ISAR1_API_IMP_DEF                0x1
#define ID_AA64ISAR1_API_IMP_DEF_EPAC           0x2
#define ID_AA64ISAR1_API_IMP_DEF_EPAC2          0x3
#define ID_AA64ISAR1_API_IMP_DEF_EPAC2_FPAC     0x4
#define ID_AA64ISAR1_API_IMP_DEF_EPAC2_FPAC_CMB 0x5

#define ID_AA64ISAR1_APA_SHIFT                  U(4)
#define ID_AA64ISAR1_APA_MASK                   ULL(0xf)
#define ID_AA64ISAR1_APA_NI                     0x0
#define ID_AA64ISAR1_APA_ARCHITECTED            0x1
#define ID_AA64ISAR1_APA_ARCH_EPAC              0x2
#define ID_AA64ISAR1_APA_ARCH_EPAC2             0x3
#define ID_AA64ISAR1_APA_ARCH_EPAC2_FPAC        0x4
#define ID_AA64ISAR1_APA_ARCH_EPAC2_FPAC_CMB    0x5

#define ID_AA64ISAR1_DPB_SHIFT                  0

/* ID_AA64ISAR2_EL1 definitions */
#define ID_AA64ISAR2_APA3_SHIFT         U(12)
#define ID_AA64ISAR2_APA3_MASK          ULL(0xf)

#define ID_AA64ISAR2_GPA3_SHIFT         U(8)
#define ID_AA64ISAR2_GPA3_MASK          ULL(0xf)

/* ID_AA64PFR0_EL1 definitions */
#define ID_AA64PFR0_CSV3_SHIFT              60

#define ID_AA64PFR0_CSV2_SHIFT              U(56)
#define ID_AA64PFR0_CSV2_MASK               ULL(0xf)
#define ID_AA64PFR0_CSV2_LENGTH             U(4)
#define ID_AA64PFR0_CSV2_2_SUPPORTED        ULL(0x2)

#define ID_AA64PFR0_FEAT_RME_SHIFT          U(52)
#define ID_AA64PFR0_FEAT_RME_MASK           ULL(0xf)
#define ID_AA64PFR0_FEAT_RME_LENGTH         U(4)
#define ID_AA64PFR0_FEAT_RME_NOT_SUPPORTED  U(0)
#define ID_AA64PFR0_FEAT_RME_V1             U(1)

#define ID_AA64PFR0_DIT_SHIFT               U(48)
#define ID_AA64PFR0_DIT_MASK                ULL(0xf)
#define ID_AA64PFR0_DIT_LENGTH              U(4)
#define ID_AA64PFR0_DIT_SUPPORTED           U(1)

#define ID_AA64PFR0_AMU_SHIFT               U(44)
#define ID_AA64PFR0_AMU_MASK                ULL(0xf)
#define ID_AA64PFR0_AMU_NOT_SUPPORTED       U(0x0)
#define ID_AA64PFR0_AMU_V1                  U(0x1)
#define ID_AA64PFR0_AMU_V1P1                U(0x2)

#define ID_AA64PFR0_MPAM_SHIFT              U(40)
#define ID_AA64PFR0_MPAM_MASK               ULL(0xf)

#define ID_AA64PFR0_SEL2_SHIFT              U(36)
#define ID_AA64PFR0_SEL2_MASK               ULL(0xf)

#define ID_AA64PFR0_SVE_SHIFT               U(32)
#define ID_AA64PFR0_SVE_MASK                ULL(0xf)
#define ID_AA64PFR0_SVE_SUPPORTED           ULL(0x1)
#define ID_AA64PFR0_SVE_LENGTH              U(4)

#define ID_AA64PFR0_RAS_SHIFT               U(28)
#define ID_AA64PFR0_RAS_MASK                ULL(0xf)
#define ID_AA64PFR0_RAS_NOT_SUPPORTED       ULL(0x0)
#define ID_AA64PFR0_RAS_V1                  0x1
#define ID_AA64PFR0_RAS_LENGTH              U(4)

#define ID_AA64PFR0_GIC_SHIFT               U(24)
#define ID_AA64PFR0_GIC_WIDTH               U(4)
#define ID_AA64PFR0_GIC_MASK                ULL(0xf)

#define ID_AA64PFR0_ASIMD_SHIFT             20
#define ID_AA64PFR0_ASIMD_SUPPORTED         0x0
#define ID_AA64PFR0_ASIMD_NI                0xf

#define ID_AA64PFR0_FP_SHIFT                16
#define ID_AA64PFR0_FP_SUPPORTED            0x0
#define ID_AA64PFR0_FP_NI                   0xf

#define ID_AA64PFR0_EL3_SHIFT               U(12)
#define ID_AA64PFR0_EL2_SHIFT               U(8)
#define ID_AA64PFR0_EL1_SHIFT               U(4)
#define ID_AA64PFR0_EL0_SHIFT               U(0)
#define ID_AA64PFR0_ELX_MASK                ULL(0xf)
#define ID_AA64PFR0_EL_IMPL_NONE            ULL(0)
#define ID_AA64PFR0_EL_IMPL_A64ONLY         ULL(1)
#define ID_AA64PFR0_EL_IMPL_A64_A32         ULL(2)

/* ID_AA64PFR1_EL1 definitions */
#define ID_AA64PFR1_EL1_RNDR_TRAP_SHIFT         U(28)
#define ID_AA64PFR1_EL1_RNDR_TRAP_MASK          U(0xf)
#define ID_AA64PFR1_EL1_RNG_TRAP_SUPPORTED      ULL(0x1)
#define ID_AA64PFR1_EL1_RNG_TRAP_NOT_SUPPORTED  ULL(0x0)

#define ID_AA64PFR1_EL1_SME_SHIFT               U(24)
#define ID_AA64PFR1_EL1_SME_MASK                ULL(0xf)

#define ID_AA64PFR1_MPAM_FRAC_SHIFT             ULL(16)
#define ID_AA64PFR1_MPAM_FRAC_MASK              ULL(0xf)

#define ID_AA64PFR1_RASFRAC_SHIFT               12

#define ID_AA64PFR1_EL1_MTE_SHIFT               U(8)
#define ID_AA64PFR1_EL1_MTE_MASK                ULL(0xf)
#define ID_AA64PFR1_MTE_NI                      0x0 /* Memory Tagging Extension is not implemented */
#define ID_AA64PFR1_MTE_EL0                     0x1 /* FEAT_MTE: MTE instructions accessible at EL0 are implemented */
#define ID_AA64PFR1_MTE_ELX                     0x2 /* FEAT_MTE2: Full MTE is implemented */
#define ID_AA64PFR1_MTE_ASY                     0x3 /* FEAT_MTE3: MTE is implemented with support for asymmetric Tag Check Fault handling */

#define ID_AA64PFR1_EL1_SSBS_SHIFT              U(4)
#define ID_AA64PFR1_EL1_SSBS_MASK               ULL(0xf)
#define ID_AA64PFR1_EL1_SSBS_PSTATE_NI          0
#define ID_AA64PFR1_EL1_SSBS_PSTATE_ONLY        1
#define ID_AA64PFR1_EL1_SSBS_PSTATE_INSNS       2

#define ID_AA64PFR1_EL1_BT_SHIFT                U(0)
#define ID_AA64PFR1_EL1_BT_MASK                 ULL(0xf)
#define ID_AA64PFR1_EL1_BT_BTI                  0x1

/* id_aa64zfr0 */
#define ID_AA64ZFR0_F64MM_SHIFT         56
#define ID_AA64ZFR0_F64MM               0x1

#define ID_AA64ZFR0_F32MM_SHIFT         52
#define ID_AA64ZFR0_F32MM               0x1

#define ID_AA64ZFR0_I8MM_SHIFT          44
#define ID_AA64ZFR0_I8MM                0x1

#define ID_AA64ZFR0_SM4_SHIFT           40
#define ID_AA64ZFR0_SM4                 0x1

#define ID_AA64ZFR0_SHA3_SHIFT          32
#define ID_AA64ZFR0_SHA3                0x1

#define ID_AA64ZFR0_BF16_SHIFT          20
#define ID_AA64ZFR0_BF16                0x1

#define ID_AA64ZFR0_BITPERM_SHIFT       16
#define ID_AA64ZFR0_BITPERM             0x1

#define ID_AA64ZFR0_AES_SHIFT           4
#define ID_AA64ZFR0_AES                 0x1
#define ID_AA64ZFR0_AES_PMULL           0x2

#define ID_AA64ZFR0_SVEVER_SHIFT        0
#define ID_AA64ZFR0_SVEVER_SVE2         0x1

/* ID_AA64MMFR0_EL1 definitions */
#define ID_AA64MMFR0_EL1_ECV_SHIFT              U(60)
#define ID_AA64MMFR0_EL1_ECV_MASK               ULL(0xf)
#define ID_AA64MMFR0_EL1_ECV_NOT_SUPPORTED      ULL(0x0)
#define ID_AA64MMFR0_EL1_ECV_SUPPORTED          ULL(0x1)
#define ID_AA64MMFR0_EL1_ECV_SELF_SYNCH         ULL(0x2)

#define ID_AA64MMFR0_EL1_FGT_SHIFT              U(56)
#define ID_AA64MMFR0_EL1_FGT_MASK               ULL(0xf)
#define ID_AA64MMFR0_EL1_FGT_SUPPORTED          ULL(0x1)
#define ID_AA64MMFR0_EL1_FGT_NOT_SUPPORTED      ULL(0x0)

#define ID_AA64MMFR0_EL1_EXS_SHIFT              44
#define ID_AA64MMFR0_EL1_TGRAN4_2_SHIFT         40
#define ID_AA64MMFR0_EL1_TGRAN64_2_SHIFT        36
#define ID_AA64MMFR0_EL1_TGRAN16_2_SHIFT        32

#define ID_AA64MMFR0_EL1_TGRAN4_SHIFT           U(28)
#define ID_AA64MMFR0_EL1_TGRAN4_MASK            ULL(0xf)
#define ID_AA64MMFR0_EL1_TGRAN4_SUPPORTED       ULL(0x0)
#define ID_AA64MMFR0_EL1_TGRAN4_NOT_SUPPORTED   ULL(0xf)

#define ID_AA64MMFR0_EL1_TGRAN64_SHIFT          U(24)
#define ID_AA64MMFR0_EL1_TGRAN64_MASK           ULL(0xf)
#define ID_AA64MMFR0_EL1_TGRAN64_SUPPORTED      ULL(0x0)
#define ID_AA64MMFR0_EL1_TGRAN64_NOT_SUPPORTED  ULL(0xf)

#define ID_AA64MMFR0_EL1_TGRAN16_SHIFT          U(20)
#define ID_AA64MMFR0_EL1_TGRAN16_MASK           ULL(0xf)
#define ID_AA64MMFR0_EL1_TGRAN16_SUPPORTED      ULL(0x1)
#define ID_AA64MMFR0_EL1_TGRAN16_NOT_SUPPORTED  ULL(0x0)

#define ID_AA64MMFR0_EL1_BIGENDEL0_SHIFT        16
#define ID_AA64MMFR0_EL1_SNSMEM_SHIFT           12
#define ID_AA64MMFR0_EL1_BIGENDEL_SHIFT         8
#define ID_AA64MMFR0_EL1_ASID_SHIFT             4

#define ID_AA64MMFR0_EL1_PARANGE_SHIFT          U(0)
#define ID_AA64MMFR0_EL1_PARANGE_MASK           ULL(0xf)
#define ID_AA64MMFR0_EL1_PARANGE_0000           U(32)
#define ID_AA64MMFR0_EL1_PARANGE_0001           U(36)
#define ID_AA64MMFR0_EL1_PARANGE_0010           U(40)
#define ID_AA64MMFR0_EL1_PARANGE_0011           U(42)
#define ID_AA64MMFR0_EL1_PARANGE_0100           U(44)
#define ID_AA64MMFR0_EL1_PARANGE_0101           U(48)
#define ID_AA64MMFR0_EL1_PARANGE_0110           U(52)
#define ID_AA64MMFR0_EL1_PARANGE_48             0x5
#define ID_AA64MMFR0_EL1_PARANGE_52             0x6

#ifdef CONFIG_ARM64_PA_BITS_52
#define ID_AA64MMFR0_EL1_PARANGE_MAX    ID_AA64MMFR0_EL1_PARANGE_52
#else
#define ID_AA64MMFR0_EL1_PARANGE_MAX    ID_AA64MMFR0_EL1_PARANGE_48
#endif

#if defined(CONFIG_ARM64_4K_PAGES)
#define ID_AA64MMFR0_EL1_TGRAN_SHIFT        ID_AA64MMFR0_EL1_TGRAN4_SHIFT
#define ID_AA64MMFR0_EL1_TGRAN_SUPPORTED    ID_AA64MMFR0_EL1_TGRAN4_SUPPORTED
#elif defined(CONFIG_ARM64_16K_PAGES)
#define ID_AA64MMFR0_EL1_TGRAN_SHIFT        ID_AA64MMFR0_EL1_TGRAN16_SHIFT
#define ID_AA64MMFR0_EL1_TGRAN_SUPPORTED    ID_AA64MMFR0_EL1_TGRAN16_SUPPORTED
#elif defined(CONFIG_ARM64_64K_PAGES)
#define ID_AA64MMFR0_EL1_TGRAN_SHIFT        ID_AA64MMFR0_EL1_TGRAN64_SHIFT
#define ID_AA64MMFR0_EL1_TGRAN_SUPPORTED    ID_AA64MMFR0_EL1_TGRAN64_SUPPORTED
#endif

/* ID_AA64MMFR1_EL1 definitions */
#define ID_AA64MMFR1_EL1_HCX_SHIFT          U(40)
#define ID_AA64MMFR1_EL1_HCX_MASK           ULL(0xf)
#define ID_AA64MMFR1_EL1_HCX_SUPPORTED      ULL(0x1)
#define ID_AA64MMFR1_EL1_HCX_NOT_SUPPORTED  ULL(0x0)

#define ID_AA64MMFR1_EL1_ETS_SHIFT          36

#define ID_AA64MMFR1_EL1_TWED_SHIFT         U(32)
#define ID_AA64MMFR1_EL1_TWED_MASK          ULL(0xf)
#define ID_AA64MMFR1_EL1_TWED_SUPPORTED     ULL(0x1)
#define ID_AA64MMFR1_EL1_TWED_NOT_SUPPORTED ULL(0x0)

#define ID_AA64MMFR1_EL1_XNX_SHIFT          28
#define ID_AA64MMFR1_EL1_SPECSEI_SHIFT      24

#define ID_AA64MMFR1_EL1_PAN_SHIFT          U(20)
#define ID_AA64MMFR1_EL1_PAN_MASK           ULL(0xf)
#define ID_AA64MMFR1_EL1_PAN_NOT_SUPPORTED  ULL(0x0)
#define ID_AA64MMFR1_EL1_PAN_SUPPORTED      ULL(0x1)
#define ID_AA64MMFR1_EL1_PAN2_SUPPORTED     ULL(0x2)
#define ID_AA64MMFR1_EL1_PAN3_SUPPORTED     ULL(0x3)

#define ID_AA64MMFR1_LOR_SHIFT              16
#define ID_AA64MMFR1_HPD_SHIFT              12

#define ID_AA64MMFR1_EL1_VHE_SHIFT          U(8)
#define ID_AA64MMFR1_EL1_VHE_MASK           ULL(0xf)

#define ID_AA64MMFR1_EL1_VMIDBITS_SHIFT     4
#define ID_AA64MMFR1_EL1_VMIDBITS_8         0
#define ID_AA64MMFR1_EL1_VMIDBITS_16        2

#define ID_AA64MMFR1_EL1_HADBS_SHIFT        0

/* ID_AA64MMFR2_EL1 definitions */
#define ID_AA64MMFR2_EL1_E0PD_SHIFT         60
#define ID_AA64MMFR2_EL1_EVT_SHIFT          56
#define ID_AA64MMFR2_EL1_BBM_SHIFT          52
#define ID_AA64MMFR2_EL1_TTL_SHIFT          48
#define ID_AA64MMFR2_EL1_FWB_SHIFT          40
#define ID_AA64MMFR2_EL1_IDS_SHIFT          36
#define ID_AA64MMFR2_EL1_AT_SHIFT           32

#define ID_AA64MMFR2_EL1_ST_SHIFT           U(28)
#define ID_AA64MMFR2_EL1_ST_MASK            ULL(0xf)

#define ID_AA64MMFR2_EL1_NV_SHIFT           U(24)
#define ID_AA64MMFR2_EL1_NV_MASK            ULL(0xf)
#define ID_AA64MMFR2_EL1_NV_NOT_SUPPORTED   ULL(0x0)
#define ID_AA64MMFR2_EL1_NV_SUPPORTED       ULL(0x1)
#define ID_AA64MMFR2_EL1_NV2_SUPPORTED      ULL(0x2)

#define ID_AA64MMFR2_EL1_CCIDX_SHIFT        U(20)
#define ID_AA64MMFR2_EL1_CCIDX_MASK         ULL(0xf)
#define ID_AA64MMFR2_EL1_CCIDX_LENGTH       U(4)

#define ID_AA64MMFR2_EL1_LVA_SHIFT          16
#define ID_AA64MMFR2_EL1_IESB_SHIFT         12
#define ID_AA64MMFR2_EL1_LSM_SHIFT          8
#define ID_AA64MMFR2_EL1_UAO_SHIFT          4

#define ID_AA64MMFR2_EL1_CNP_SHIFT          U(0)
#define ID_AA64MMFR2_EL1_CNP_MASK           ULL(0xf)

/* ID_AA64MMFR3_EL1 definitions */
#define ID_AA64MMFR3_EL1_TCRX_SHIFT         U(0)
#define ID_AA64MMFR3_EL1_TCRX_MASK          ULL(0xf)

/* ID_AA64DFR0_EL1 definitions */
#define ID_AA64DFR0_EL1_BRBE_SHIFT              U(52)
#define ID_AA64DFR0_EL1_BRBE_MASK               ULL(0xf)
#define ID_AA64DFR0_EL1_BRBE_SUPPORTED          ULL(1)

#define ID_AA64DFR0_EL1_MTPMU_SHIFT             U(48)
#define ID_AA64DFR0_EL1_MTPMU_MASK              ULL(0xf)
#define ID_AA64DFR0_EL1_MTPMU_SUPPORTED         ULL(1)

#define ID_AA64DFR0_EL1_TRACEBUFFER_SHIFT       U(44)
#define ID_AA64DFR0_EL1_TRACEBUFFER_MASK        ULL(0xf)
#define ID_AA64DFR0_EL1_TRACEBUFFER_SUPPORTED   ULL(1)

#define ID_AA64DFR0_EL1_TRACEFILT_SHIFT         U(40)
#define ID_AA64DFR0_EL1_TRACEFILT_MASK          U(0xf)
#define ID_AA64DFR0_EL1_TRACEFILT_SUPPORTED     U(1)
#define ID_AA64DFR0_EL1_TRACEFILT_LENGTH        U(4)

#define ID_AA64DFR0_EL1_DOUBLELOCK_SHIFT        36

#define ID_AA64DFR0_EL1_PMS_SHIFT               U(32)
#define ID_AA64DFR0_EL1_PMS_MASK                ULL(0xf)
#define ID_AA64DFR0_EL1_SPE_SUPPORTED           ULL(0x1)
#define ID_AA64DFR0_EL1_SPE_NOT_SUPPORTED       ULL(0x0)

#define ID_AA64DFR0_EL1_CTX_CMPS_SHIFT          28
#define ID_AA64DFR0_EL1_WRPS_SHIFT              20
#define ID_AA64DFR0_EL1_BRPS_SHIFT              12

#define ID_AA64DFR0_EL1_PMUVER_SHIFT            8
#define ID_AA64DFR0_PMUVER_8_0                  0x1
#define ID_AA64DFR0_PMUVER_8_1                  0x4
#define ID_AA64DFR0_PMUVER_8_4                  0x5
#define ID_AA64DFR0_PMUVER_8_5                  0x6
#define ID_AA64DFR0_PMUVER_IMP_DEF              0xf

#define ID_AA64DFR0_EL1_TRACEVER_SHIFT          U(4)
#define ID_AA64DFR0_EL1_TRACEVER_MASK           ULL(0xf)
#define ID_AA64DFR0_EL1_TRACEVER_SUPPORTED      ULL(1)
#define ID_AA64DFR0_EL1_TRACEVER_LENGTH         U(4)

#define ID_AA64DFR0_EL1_DEBUGVER_SHIFT          0

/* ID_DFR0_EL1 definitions */
#define ID_DFR0_EL1_PERFMON_SHIFT       24
#define ID_DFR0_EL1_PERFMON_8_1         0x4

/* ID_ISAR4_EL1 definitions */
#define ID_ISAR4_SWP_FRAC_SHIFT         28
#define ID_ISAR4_PSR_M_SHIFT            24
#define ID_ISAR4_SYNCH_PRIM_FRAC_SHIFT  20
#define ID_ISAR4_BARRIER_SHIFT          16
#define ID_ISAR4_SMC_SHIFT              12
#define ID_ISAR4_WRITEBACK_SHIFT        8
#define ID_ISAR4_WITHSHIFTS_SHIFT       4
#define ID_ISAR4_UNPRIV_SHIFT           0

/* ID_DFR1_EL1 definitions */
#define ID_DFR1_EL1_MTPMU_SHIFT        0

/* ID_ISAR0_EL1 definitions */
#define ID_ISAR0_EL1_DIVIDE_SHIFT       24
#define ID_ISAR0_EL1_DEBUG_SHIFT        20
#define ID_ISAR0_EL1_COPROC_SHIFT       16
#define ID_ISAR0_EL1_CMPBRANCH_SHIFT    12
#define ID_ISAR0_EL1_BITFIELD_SHIFT     8
#define ID_ISAR0_EL1_BITCOUNT_SHIFT     4
#define ID_ISAR0_EL1_SWAP_SHIFT         0

/* ID_ISAR5_EL1 definitions */
#define ID_ISAR5_EL1_RDM_SHIFT          24
#define ID_ISAR5_EL1_CRC32_SHIFT        16
#define ID_ISAR5_EL1_SHA2_SHIFT         12
#define ID_ISAR5_EL1_SHA1_SHIFT         8
#define ID_ISAR5_EL1_AES_SHIFT          4
#define ID_ISAR5_EL1_SEVL_SHIFT         0

/* ID_ISAR6_EL1 definitions */
#define ID_ISAR6_EL1_I8MM_SHIFT         24
#define ID_ISAR6_EL1_BF16_SHIFT         20
#define ID_ISAR6_EL1_SPECRES_SHIFT      16
#define ID_ISAR6_EL1_SB_SHIFT           12
#define ID_ISAR6_EL1_FHM_SHIFT          8
#define ID_ISAR6_EL1_DP_SHIFT           4
#define ID_ISAR6_EL1_JSCVT_SHIFT        0

/* ID_MMFR0_EL1 definitions */
#define ID_MMFR0_EL1_INNERSHR_SHIFT     28
#define ID_MMFR0_EL1_FCSE_SHIFT         24
#define ID_MMFR0_EL1_AUXREG_SHIFT       20
#define ID_MMFR0_EL1_TCM_SHIFT          16
#define ID_MMFR0_EL1_SHARELVL_SHIFT     12
#define ID_MMFR0_EL1_OUTERSHR_SHIFT     8
#define ID_MMFR0_EL1_PMSA_SHIFT         4
#define ID_MMFR0_EL1_VMSA_SHIFT         0

/* ID_MMFR4_EL1 definitions */
#define ID_MMFR4_EL1_EVT_SHIFT          28
#define ID_MMFR4_EL1_CCIDX_SHIFT        24
#define ID_MMFR4_EL1_LSM_SHIFT          20
#define ID_MMFR4_EL1_HPDS_SHIFT         16
#define ID_MMFR4_EL1_CNP_SHIFT          12
#define ID_MMFR4_EL1_XNX_SHIFT          8
#define ID_MMFR4_EL1_AC2_SHIFT          4
#define ID_MMFR4_EL1_SPECSEI_SHIFT      0

/* ID_MMFR5_EL1 definitions */
#define ID_MMFR5_EL1_ETS_SHIFT          0

/* ID_PFR0_EL1 definitions */
#define ID_PFR0_EL1_DIT_SHIFT           24
#define ID_PFR0_EL1_CSV2_SHIFT          16
#define ID_PFR0_EL1_STATE3_SHIFT        12
#define ID_PFR0_EL1_STATE2_SHIFT        8
#define ID_PFR0_EL1_STATE1_SHIFT        4
#define ID_PFR0_EL1_STATE0_SHIFT        0

/* ID_DFR0_EL1 definitions */
#define ID_DFR0_EL1_PERFMON_SHIFT       24
#define ID_DFR0_EL1_MPROFDBG_SHIFT      20
#define ID_DFR0_EL1_MMAPTRC_SHIFT       16
#define ID_DFR0_EL1_COPTRC_SHIFT        12
#define ID_DFR0_EL1_MMAPDBG_SHIFT       8
#define ID_DFR0_EL1_COPSDBG_SHIFT       4
#define ID_DFR0_EL1_COPDBG_SHIFT        0

/* ID_PFR2_EL1 definitions */
#define ID_PFR2_EL1_SSBS_SHIFT          4
#define ID_PFR2_EL1_CSV3_SHIFT          0

/* MVFR0_EL1 definitions */
#define MVFR0_EL1_FPROUND_SHIFT         28
#define MVFR0_EL1_FPSHVEC_SHIFT         24
#define MVFR0_EL1_FPSQRT_SHIFT          20
#define MVFR0_EL1_FPDIVIDE_SHIFT        16
#define MVFR0_EL1_FPTRAP_SHIFT          12
#define MVFR0_EL1_FPDP_SHIFT            8
#define MVFR0_EL1_FPSP_SHIFT            4
#define MVFR0_EL1_SIMD_SHIFT            0

/* MVFR1_EL1 definitions */
#define MVFR1_EL1_SIMDFMAC_SHIFT        28
#define MVFR1_EL1_FPHP_SHIFT            24
#define MVFR1_EL1_SIMDHP_SHIFT          20
#define MVFR1_EL1_SIMDSP_SHIFT          16
#define MVFR1_EL1_SIMDINT_SHIFT         12
#define MVFR1_EL1_SIMDLS_SHIFT          8
#define MVFR1_EL1_FPDNAN_SHIFT          4
#define MVFR1_EL1_FPFTZ_SHIFT           0

/* ID_PFR1_EL1 definitions */
#define ID_PFR1_EL1_GIC_SHIFT               28
#define ID_PFR1_EL1_VIRT_FRAC_SHIFT         24
#define ID_PFR1_EL1_SEC_FRAC_SHIFT          20
#define ID_PFR1_EL1_GENTIMER_SHIFT          16

#define ID_PFR1_EL1_VIRTUALIZATION_SHIFT    12
#define ID_PFR1_VIRTEXT_MASK                U(0xf)
#define GET_VIRT_EXT(id)                    (((id) >> ID_PFR1_VIRTEXT_SHIFT) \
                                             & ID_PFR1_VIRTEXT_MASK)

#define ID_PFR1_EL1_MPROGMOD_SHIFT          8
#define ID_PFR1_EL1_SECURITY_SHIFT          4
#define ID_PFR1_EL1_PROGMOD_SHIFT           0

/* MVFR2_EL1 definitions */
#define MVFR2_EL1_FPMISC_SHIFT      4
#define MVFR2_EL1_SIMDMISC_SHIFT    0

/* DCZID_EL0 definitions */
#define DCZID_EL0_DZP_SHIFT     4
#define DCZID_EL0_BS_SHIFT      0

/* ZCR_ELx definitions */
/*
 * The ZCR_ELx_LEN_* definitions intentionally include bits [8:4] which
 * are reserved by the SVE architecture for future expansion of the LEN
 * field, with compatible semantics.
 */
#define ZCR_ELx_LEN_SHIFT       0
#define ZCR_ELx_LEN_SIZE        9
#define ZCR_ELx_LEN_MASK        0x1ff

/* CPACR_EL1 definitions */
#define CPACR_EL1_ZEN_EL1EN     (BIT(16)) /* enable EL1 access */
#define CPACR_EL1_ZEN_EL0EN     (BIT(17)) /* enable EL0 access, if EL1EN set */
#define CPACR_EL1_ZEN           (CPACR_EL1_ZEN_EL1EN | CPACR_EL1_ZEN_EL0EN)

#define CPACR_EL1_FPEN(x)       ((x) << 20)
#define CPACR_EL1_FP_TRAP_EL0   UL(0x1)
#define CPACR_EL1_FP_TRAP_ALL   UL(0x2)
#define CPACR_EL1_FP_TRAP_NONE  UL(0x3)

/* SCR definitions */
#define SCR_RES1_BITS       ((U(1) << 4) | (U(1) << 5))
#define SCR_NSE_SHIFT       U(62)
#define SCR_NSE_BIT         (ULL(1) << SCR_NSE_SHIFT)
#define SCR_GPF_BIT         (UL(1) << 48)
#define SCR_TWEDEL_SHIFT    U(30)
#define SCR_TWEDEL_MASK     ULL(0xf)
#define SCR_TCR2EN_BIT      (UL(1) << 43)
#define SCR_TRNDR_BIT       (UL(1) << 40)
#define SCR_HXEn_BIT        (UL(1) << 38)
#define SCR_ENTP2_SHIFT     U(41)
#define SCR_ENTP2_BIT       (UL(1) << SCR_ENTP2_SHIFT)
#define SCR_AMVOFFEN_SHIFT  U(35)
#define SCR_AMVOFFEN_BIT    (UL(1) << SCR_AMVOFFEN_SHIFT)
#define SCR_TWEDEn_BIT      (UL(1) << 29)
#define SCR_ECVEN_BIT       (UL(1) << 28)
#define SCR_FGTEN_BIT       (UL(1) << 27)
#define SCR_ATA_BIT         (UL(1) << 26)
#define SCR_EnSCXT_BIT      (UL(1) << 25)
#define SCR_FIEN_BIT        (UL(1) << 21)
#define SCR_EEL2_BIT        (UL(1) << 18)
#define SCR_API_BIT         (UL(1) << 17)
#define SCR_APK_BIT         (UL(1) << 16)
#define SCR_TERR_BIT        (UL(1) << 15)
#define SCR_TWE_BIT         (UL(1) << 13)
#define SCR_TWI_BIT         (UL(1) << 12)
#define SCR_ST_BIT          (UL(1) << 11)
#define SCR_RW_BIT          (UL(1) << 10)
#define SCR_SIF_BIT         (UL(1) << 9)
#define SCR_HCE_BIT         (UL(1) << 8)
#define SCR_SMD_BIT         (UL(1) << 7)
#define SCR_EA_BIT          (UL(1) << 3)
#define SCR_FIQ_BIT         (UL(1) << 2)
#define SCR_IRQ_BIT         (UL(1) << 1)
#define SCR_NS_BIT          (UL(1) << 0)
#define SCR_VALID_BIT_MASK  U(0x24000002F8F)
#define SCR_RESET_VAL       SCR_RES1_BITS

/* GCR_EL1 Definitions */
#define GCR_EL1_RRND        (BIT(16))
#define GCR_EL1_EXCL_MASK   0xffffUL

/* RGSR_EL1 Definitions */
#define RGSR_EL1_TAG_MASK       0xfUL
#define RGSR_EL1_SEED_SHIFT     8
#define RGSR_EL1_SEED_MASK      0xffffUL

/* CLIDR_EL1 field definitions */
#define LOC_SHIFT           U(24)
#define LOUIS_SHIFT         U(21)
#define CTYPE_SHIFT(n)      U(3 * (n - 1))
#define CLIDR_FIELD_WIDTH   U(3)

/* GMID_EL1 field definitions */
#define GMID_EL1_BS_SHIFT       0
#define GMID_EL1_BS_SIZE        4

/* CSSELR_EL1 field definitions */
#define LEVEL_SHIFT         U(1)

/* TFSR{,E0}_EL1 bit definitions */
#define TFSR_EL1_TF0_SHIFT      0
#define TFSR_EL1_TF1_SHIFT      1
#define TFSR_EL1_TF0            BIT(TFSR_EL1_TF0_SHIFT)
#define TFSR_EL1_TF1            BIT(TFSR_EL1_TF1_SHIFT)

/* MDCR_EL3 definitions */
#define MDCR_EnPMSN_BIT     (ULL(1) << 36)
#define MDCR_MPMX_BIT       (ULL(1) << 35)
#define MDCR_MCCD_BIT       (ULL(1) << 34)

#define MDCR_SBRBE_SHIFT    U(32)
#define MDCR_SBRBE_MASK     ULL(0x3)

#define MDCR_MTPME_BIT      (ULL(1) << 28)
#define MDCR_TDCC_BIT       (ULL(1) << 27)
#define MDCR_NSTBE          (ULL(1) << 26)

#define MDCR_NSTB(x)        ((x) << 24)
#define MDCR_NSTB_EL1       ULL(0x3)

#define MDCR_SCCD_BIT       (ULL(1) << 23)
#define MDCR_EPMAD_BIT      (ULL(1) << 21)
#define MDCR_EDAD_BIT       (ULL(1) << 20)
#define MDCR_TTRF_BIT       (ULL(1) << 19)
#define MDCR_STE_BIT        (ULL(1) << 18)
#define MDCR_SPME_BIT       (ULL(1) << 17)
#define MDCR_SDD_BIT        (ULL(1) << 16)

#define MDCR_SPD32(x)       ((x) << 14)
#define MDCR_SPD32_LEGACY   ULL(0x0)
#define MDCR_SPD32_DISABLE  ULL(0x2)
#define MDCR_SPD32_ENABLE   ULL(0x3)

#define MDCR_NSPB(x)        ((x) << 12)
#define MDCR_NSPB_EL1       ULL(0x3)

#define MDCR_TDOSA_BIT      (ULL(1) << 10)
#define MDCR_TDA_BIT        (ULL(1) << 9)
#define MDCR_TPM_BIT        (ULL(1) << 6)
#define MDCR_EL3_RESET_VAL  ULL(0x0)

/* MDCR_EL2 definitions */
#define MDCR_EL2_MTPME      (U(1) << 28)
#define MDCR_EL2_HLP        (U(1) << 26)

#define MDCR_EL2_E2TB(x)    ((x) << 24)
#define MDCR_EL2_E2TB_EL1   U(0x3)

#define MDCR_EL2_HCCD       (U(1) << 23)
#define MDCR_EL2_TTRF       (U(1) << 19)
#define MDCR_EL2_HPMD       (U(1) << 17)
#define MDCR_EL2_TPMS       (U(1) << 14)

#define MDCR_EL2_E2PB(x)    ((x) << 12)
#define MDCR_EL2_E2PB_EL1   U(0x3)

#define MDCR_EL2_TDRA_BIT   (U(1) << 11)
#define MDCR_EL2_TDOSA_BIT  (U(1) << 10)
#define MDCR_EL2_TDA_BIT    (U(1) << 9)
#define MDCR_EL2_TDE_BIT    (U(1) << 8)
#define MDCR_EL2_HPME_BIT   (U(1) << 7)
#define MDCR_EL2_TPM_BIT    (U(1) << 6)
#define MDCR_EL2_TPMCR_BIT  (U(1) << 5)
#define MDCR_EL2_RESET_VAL  U(0x0)

/* HSTR_EL2 definitions */
#define HSTR_EL2_RESET_VAL	U(0x0)
#define HSTR_EL2_T_MASK		U(0xff)

/* CNTHP_CTL_EL2 definitions */
#define CNTHP_CTL_ENABLE_BIT	(U(1) << 0)
#define CNTHP_CTL_RESET_VAL	U(0x0)

/* VTTBR_EL2 definitions */
#define VTTBR_RESET_VAL		ULL(0x0)
#define VTTBR_VMID_MASK		ULL(0xff)
#define VTTBR_VMID_SHIFT	U(48)
#define VTTBR_BADDR_MASK	ULL(0xffffffffffff)
#define VTTBR_BADDR_SHIFT	U(0)

/* HCR definitions */
#define HCR_RESET_VAL		ULL(0x0)
#define HCR_AMVOFFEN_SHIFT	U(51)
#define HCR_AMVOFFEN_BIT	(ULL(1) << HCR_AMVOFFEN_SHIFT)
#define HCR_TEA_BIT		(ULL(1) << 47)
#define HCR_API_BIT		(ULL(1) << 41)
#define HCR_APK_BIT		(ULL(1) << 40)
#define HCR_E2H_BIT		(ULL(1) << 34)
#define HCR_HCD_BIT		(ULL(1) << 29)
#define HCR_TGE_BIT		(ULL(1) << 27)
#define HCR_RW_SHIFT		U(31)
#define HCR_RW_BIT		(ULL(1) << HCR_RW_SHIFT)
#define HCR_TWE_BIT		(ULL(1) << 14)
#define HCR_TWI_BIT		(ULL(1) << 13)
#define HCR_AMO_BIT		(ULL(1) << 5)
#define HCR_IMO_BIT		(ULL(1) << 4)
#define HCR_FMO_BIT		(ULL(1) << 3)

/* ISR definitions */
#define ISR_A_SHIFT		U(8)
#define ISR_I_SHIFT		U(7)
#define ISR_F_SHIFT		U(6)

/* CNTHCTL_EL2 definitions */
#define CNTHCTL_RESET_VAL	U(0x0)
#define EVNTEN_BIT		(U(1) << 2)
#define EL1PCEN_BIT		(U(1) << 1)
#define EL1PCTEN_BIT		(U(1) << 0)

/* CNTKCTL_EL1 definitions */
#define EL0PTEN_BIT		(U(1) << 9)
#define EL0VTEN_BIT		(U(1) << 8)
#define EL0PCTEN_BIT		(U(1) << 0)
#define EL0VCTEN_BIT		(U(1) << 1)
#define EVNTEN_BIT		(U(1) << 2)
#define EVNTDIR_BIT		(U(1) << 3)
#define EVNTI_SHIFT		U(4)
#define EVNTI_MASK		U(0xf)

/* CPTR_EL3 definitions */
#define TCPAC_BIT		(U(1) << 31)
#define TAM_SHIFT		U(30)
#define TAM_BIT			(U(1) << TAM_SHIFT)
#define TTA_BIT			(U(1) << 20)
#define ESM_BIT			(U(1) << 12)
#define TFP_BIT			(U(1) << 10)
#define CPTR_EZ_BIT		(U(1) << 8)
#define CPTR_EL3_RESET_VAL	((TCPAC_BIT | TAM_BIT | TTA_BIT | TFP_BIT) & \
                ~(CPTR_EZ_BIT | ESM_BIT))

/* CPTR_EL2 definitions */
#define CPTR_EL2_RES1		((U(1) << 13) | (U(1) << 12) | (U(0x3ff)))
#define CPTR_EL2_TCPAC_BIT	(U(1) << 31)
#define CPTR_EL2_TAM_SHIFT	U(30)
#define CPTR_EL2_TAM_BIT	(U(1) << CPTR_EL2_TAM_SHIFT)
#define CPTR_EL2_SMEN_MASK	ULL(0x3)
#define CPTR_EL2_SMEN_SHIFT	U(24)
#define CPTR_EL2_TTA_BIT	(U(1) << 20)
#define CPTR_EL2_TSM_BIT	(U(1) << 12)
#define CPTR_EL2_TFP_BIT	(U(1) << 10)
#define CPTR_EL2_TZ_BIT		(U(1) << 8)
#define CPTR_EL2_RESET_VAL	CPTR_EL2_RES1

/* VTCR_EL2 definitions */
#define VTCR_RESET_VAL		U(0x0)
#define VTCR_EL2_MSA		(U(1) << 31)

/* CPSR/SPSR definitions */
#define DAIF_FIQ_BIT		(U(1) << 0)
#define DAIF_IRQ_BIT		(U(1) << 1)
#define DAIF_ABT_BIT		(U(1) << 2)
#define DAIF_DBG_BIT		(U(1) << 3)
#define SPSR_DAIF_SHIFT		U(6)
#define SPSR_DAIF_MASK		U(0xf)

#define SPSR_AIF_SHIFT		U(6)
#define SPSR_AIF_MASK		U(0x7)

#define SPSR_E_SHIFT		U(9)
#define SPSR_E_MASK		U(0x1)
#define SPSR_E_LITTLE		U(0x0)
#define SPSR_E_BIG		U(0x1)

#define SPSR_T_SHIFT		U(5)
#define SPSR_T_MASK		U(0x1)
#define SPSR_T_ARM		U(0x0)
#define SPSR_T_THUMB		U(0x1)

#define SPSR_M_SHIFT		U(4)
#define SPSR_M_MASK		U(0x1)
#define SPSR_M_AARCH64		U(0x0)
#define SPSR_M_AARCH32		U(0x1)
#define SPSR_M_EL2H		U(0x9)

#define SPSR_EL_SHIFT		U(2)
#define SPSR_EL_WIDTH		U(2)

#define SPSR_SSBS_SHIFT_AARCH64 U(12)
#define SPSR_SSBS_BIT_AARCH64	(ULL(1) << SPSR_SSBS_SHIFT_AARCH64)
#define SPSR_SSBS_SHIFT_AARCH32 U(23)
#define SPSR_SSBS_BIT_AARCH32	(ULL(1) << SPSR_SSBS_SHIFT_AARCH32)

#define SPSR_PAN_BIT		BIT_64(22)

#define SPSR_DIT_BIT		BIT(24)

#define SPSR_TCO_BIT_AARCH64	BIT_64(25)

#define DISABLE_ALL_EXCEPTIONS \
        (DAIF_FIQ_BIT | DAIF_IRQ_BIT | DAIF_ABT_BIT | DAIF_DBG_BIT)

#define DISABLE_INTERRUPTS	(DAIF_FIQ_BIT | DAIF_IRQ_BIT)

/* RMR_EL3 definitions */
#define RMR_EL3_RR_BIT		(U(1) << 1)
#define RMR_EL3_AA64_BIT	(U(1) << 0)

/* HI-VECTOR address for AArch32 state */
#define HI_VECTOR_BASE		U(0xFFFF0000)

/* TCR Definitions */
#define TCR_EL1_TCMA1    (BIT(58))
#define TCR_EL1_TCMA0    (BIT(57))

#define TCR_EL3_RES1		((ULL(1) << 31) | (ULL(1) << 23))
#define TCR_EL2_RES1		((ULL(1) << 31) | (ULL(1) << 23))

#define TCR_TxSZ_MIN		ULL(16)
#define TCR_TxSZ_MAX		ULL(39)
#define TCR_TxSZ_MAX_TTST	ULL(48)

#define TCR_T0SZ_SHIFT		U(0)
#define TCR_T1SZ_SHIFT		U(16)

/* (internal) physical address size bits in EL3/EL1 */
#define TCR_EL1_IPS_SHIFT	U(32)
#define TCR_EL2_PS_SHIFT	U(16)
#define TCR_EL3_PS_SHIFT	U(16)

#define TCR_PS_BITS_4GB		ULL(0x0)
#define TCR_PS_BITS_64GB	ULL(0x1)
#define TCR_PS_BITS_1TB		ULL(0x2)
#define TCR_PS_BITS_4TB		ULL(0x3)
#define TCR_PS_BITS_16TB	ULL(0x4)
#define TCR_PS_BITS_256TB	ULL(0x5)

#define ADDR_MASK_48_TO_63	ULL(0xFFFF000000000000)
#define ADDR_MASK_44_TO_47	ULL(0x0000F00000000000)
#define ADDR_MASK_42_TO_43	ULL(0x00000C0000000000)
#define ADDR_MASK_40_TO_41	ULL(0x0000030000000000)
#define ADDR_MASK_36_TO_39	ULL(0x000000F000000000)
#define ADDR_MASK_32_TO_35	ULL(0x0000000F00000000)

#define TCR_RGN_INNER_NC	(ULL(0x0) << 8)
#define TCR_RGN_INNER_WBA	(ULL(0x1) << 8)
#define TCR_RGN_INNER_WT	(ULL(0x2) << 8)
#define TCR_RGN_INNER_WBNA	(ULL(0x3) << 8)

#define TCR_RGN_OUTER_NC	(ULL(0x0) << 10)
#define TCR_RGN_OUTER_WBA	(ULL(0x1) << 10)
#define TCR_RGN_OUTER_WT	(ULL(0x2) << 10)
#define TCR_RGN_OUTER_WBNA	(ULL(0x3) << 10)

#define TCR_SH_NON_SHAREABLE	(ULL(0x0) << 12)
#define TCR_SH_OUTER_SHAREABLE	(ULL(0x2) << 12)
#define TCR_SH_INNER_SHAREABLE	(ULL(0x3) << 12)

#define TCR_RGN1_INNER_NC	(ULL(0x0) << 24)
#define TCR_RGN1_INNER_WBA	(ULL(0x1) << 24)
#define TCR_RGN1_INNER_WT	(ULL(0x2) << 24)
#define TCR_RGN1_INNER_WBNA	(ULL(0x3) << 24)

#define TCR_RGN1_OUTER_NC	(ULL(0x0) << 26)
#define TCR_RGN1_OUTER_WBA	(ULL(0x1) << 26)
#define TCR_RGN1_OUTER_WT	(ULL(0x2) << 26)
#define TCR_RGN1_OUTER_WBNA	(ULL(0x3) << 26)

#define TCR_SH1_NON_SHAREABLE	(ULL(0x0) << 28)
#define TCR_SH1_OUTER_SHAREABLE	(ULL(0x2) << 28)
#define TCR_SH1_INNER_SHAREABLE	(ULL(0x3) << 28)

#define TCR_TG0_SHIFT		U(14)
#define TCR_TG0_MASK		ULL(3)
#define TCR_TG0_4K		(ULL(0) << TCR_TG0_SHIFT)
#define TCR_TG0_64K		(ULL(1) << TCR_TG0_SHIFT)
#define TCR_TG0_16K		(ULL(2) << TCR_TG0_SHIFT)

#define TCR_TG1_SHIFT		U(30)
#define TCR_TG1_MASK		ULL(3)
#define TCR_TG1_16K		(ULL(1) << TCR_TG1_SHIFT)
#define TCR_TG1_4K		(ULL(2) << TCR_TG1_SHIFT)
#define TCR_TG1_64K		(ULL(3) << TCR_TG1_SHIFT)

#define TCR_EPD0_BIT		(ULL(1) << 7)
#define TCR_EPD1_BIT		(ULL(1) << 23)

#define MODE_SP_SHIFT		U(0x0)
#define MODE_SP_MASK		U(0x1)
#define MODE_SP_EL0		U(0x0)
#define MODE_SP_ELX		U(0x1)

#define MODE_RW_SHIFT		U(0x4)
#define MODE_RW_MASK		U(0x1)
#define MODE_RW_64		U(0x0)
#define MODE_RW_32		U(0x1)

#define MODE_EL_SHIFT		U(0x2)
#define MODE_EL_MASK		U(0x3)
#define MODE_EL_WIDTH		U(0x2)
#define MODE_EL3		U(0x3)
#define MODE_EL2		U(0x2)
#define MODE_EL1		U(0x1)
#define MODE_EL0		U(0x0)

#define MODE32_SHIFT		U(0)
#define MODE32_MASK		U(0xf)
#define MODE32_usr		U(0x0)
#define MODE32_fiq		U(0x1)
#define MODE32_irq		U(0x2)
#define MODE32_svc		U(0x3)
#define MODE32_mon		U(0x6)
#define MODE32_abt		U(0x7)
#define MODE32_hyp		U(0xa)
#define MODE32_und		U(0xb)
#define MODE32_sys		U(0xf)

#define GET_RW(mode)		(((mode) >> MODE_RW_SHIFT) & MODE_RW_MASK)
#define GET_EL(mode)		(((mode) >> MODE_EL_SHIFT) & MODE_EL_MASK)
#define GET_SP(mode)		(((mode) >> MODE_SP_SHIFT) & MODE_SP_MASK)
#define GET_M32(mode)		(((mode) >> MODE32_SHIFT) & MODE32_MASK)

#define SPSR_64(el, sp, daif)					\
    (((MODE_RW_64 << MODE_RW_SHIFT) |			\
    (((el) & MODE_EL_MASK) << MODE_EL_SHIFT) |		\
    (((sp) & MODE_SP_MASK) << MODE_SP_SHIFT) |		\
    (((daif) & SPSR_DAIF_MASK) << SPSR_DAIF_SHIFT)) &	\
    (~(SPSR_SSBS_BIT_AARCH64)))

#define SPSR_MODE32(mode, isa, endian, aif)		\
    (((MODE_RW_32 << MODE_RW_SHIFT) |		\
    (((mode) & MODE32_MASK) << MODE32_SHIFT) |	\
    (((isa) & SPSR_T_MASK) << SPSR_T_SHIFT) |	\
    (((endian) & SPSR_E_MASK) << SPSR_E_SHIFT) |	\
    (((aif) & SPSR_AIF_MASK) << SPSR_AIF_SHIFT)) &	\
    (~(SPSR_SSBS_BIT_AARCH32)))

/* TTBR Definitions */
#define TTBR_CNP_BIT		ULL(0x1)

/* CTR_EL0 definitions */
#define CTR_CWG_SHIFT		U(24)
#define CTR_CWG_MASK		U(0xf)
#define CTR_ERG_SHIFT		U(20)
#define CTR_ERG_MASK		U(0xf)
#define CTR_DMINLINE_SHIFT	U(16)
#define CTR_DMINLINE_MASK	U(0xf)
#define CTR_L1IP_SHIFT		U(14)
#define CTR_L1IP_MASK		U(0x3)
#define CTR_IMINLINE_SHIFT	U(0)
#define CTR_IMINLINE_MASK	U(0xf)

#define MAX_CACHE_LINE_SIZE	U(0x800) /* 2KB */

/* Physical timer control register bit fields shifts and masks */
#define CNTP_CTL_ENABLE_SHIFT	U(0)
#define CNTP_CTL_IMASK_SHIFT	U(1)
#define CNTP_CTL_ISTATUS_SHIFT	U(2)

#define CNTP_CTL_ENABLE_MASK	U(1)
#define CNTP_CTL_IMASK_MASK	U(1)
#define CNTP_CTL_ISTATUS_MASK	U(1)

/* Physical timer control macros */
#define CNTP_CTL_ENABLE_BIT	(U(1) << CNTP_CTL_ENABLE_SHIFT)
#define CNTP_CTL_IMASK_BIT	(U(1) << CNTP_CTL_IMASK_SHIFT)

/* Exception Syndrome register bits and bobs */
#define ESR_EC_SHIFT			U(26)
#define ESR_EC_MASK			U(0x3f)
#define ESR_EC_LENGTH			U(6)
#define ESR_ISS_SHIFT			U(0)
#define ESR_ISS_LENGTH			U(25)
#define EC_UNKNOWN			U(0x0)
#define EC_WFE_WFI			U(0x1)
#define EC_AARCH32_CP15_MRC_MCR		U(0x3)
#define EC_AARCH32_CP15_MRRC_MCRR	U(0x4)
#define EC_AARCH32_CP14_MRC_MCR		U(0x5)
#define EC_AARCH32_CP14_LDC_STC		U(0x6)
#define EC_FP_SIMD			U(0x7)
#define EC_AARCH32_CP10_MRC		U(0x8)
#define EC_AARCH32_CP14_MRRC_MCRR	U(0xc)
#define EC_ILLEGAL			U(0xe)
#define EC_AARCH32_SVC			U(0x11)
#define EC_AARCH32_HVC			U(0x12)
#define EC_AARCH32_SMC			U(0x13)
#define EC_AARCH64_SVC			U(0x15)
#define EC_AARCH64_HVC			U(0x16)
#define EC_AARCH64_SMC			U(0x17)
#define EC_AARCH64_SYS			U(0x18)
#define EC_IABORT_LOWER_EL		U(0x20)
#define EC_IABORT_CUR_EL		U(0x21)
#define EC_PC_ALIGN			U(0x22)
#define EC_DABORT_LOWER_EL		U(0x24)
#define EC_DABORT_CUR_EL		U(0x25)
#define EC_SP_ALIGN			U(0x26)
#define EC_AARCH32_FP			U(0x28)
#define EC_AARCH64_FP			U(0x2c)
#define EC_SERROR			U(0x2f)
#define EC_BRK				U(0x3c)

/*
 * External Abort bit in Instruction and Data Aborts synchronous exception
 * syndromes.
 */
#define ESR_ISS_EABORT_EA_BIT		U(9)

#define EC_BITS(x)			(((x) >> ESR_EC_SHIFT) & ESR_EC_MASK)

/* Reset bit inside the Reset management register for EL3 (RMR_EL3) */
#define RMR_RESET_REQUEST_SHIFT 	U(0x1)
#define RMR_WARM_RESET_CPU		(U(1) << RMR_RESET_REQUEST_SHIFT)

/*******************************************************************************
 * Definitions of register offsets, fields and macros for CPU system
 * instructions.
 ******************************************************************************/

#define TLBI_ADDR_SHIFT		U(12)
#define TLBI_ADDR_MASK		ULL(0x00000FFFFFFFFFFF)
#define TLBI_ADDR(x)		(((x) >> TLBI_ADDR_SHIFT) & TLBI_ADDR_MASK)

/*******************************************************************************
 * Definitions of register offsets and fields in the CNTCTLBase Frame of the
 * system level implementation of the Generic Timer.
 ******************************************************************************/
#define CNTCTLBASE_CNTFRQ	U(0x0)
#define CNTNSAR			U(0x4)
#define CNTNSAR_NS_SHIFT(x)	(x)

#define CNTACR_BASE(x)		(U(0x40) + ((x) << 2))
#define CNTACR_RPCT_SHIFT	U(0x0)
#define CNTACR_RVCT_SHIFT	U(0x1)
#define CNTACR_RFRQ_SHIFT	U(0x2)
#define CNTACR_RVOFF_SHIFT	U(0x3)
#define CNTACR_RWVT_SHIFT	U(0x4)
#define CNTACR_RWPT_SHIFT	U(0x5)

/*******************************************************************************
 * Definitions of register offsets and fields in the CNTBaseN Frame of the
 * system level implementation of the Generic Timer.
 ******************************************************************************/
/* Physical Count register. */
#define CNTPCT_LO		U(0x0)
/* Counter Frequency register. */
#define CNTBASEN_CNTFRQ		U(0x10)
/* Physical Timer CompareValue register. */
#define CNTP_CVAL_LO		U(0x20)
/* Physical Timer Control register. */
#define CNTP_CTL		U(0x2c)

/* PMCR_EL0 definitions */
#define PMCR_EL0_RESET_VAL	U(0x0)
#define PMCR_EL0_N_SHIFT	U(11)
#define PMCR_EL0_N_MASK		U(0x1f)
#define PMCR_EL0_N_BITS		(PMCR_EL0_N_MASK << PMCR_EL0_N_SHIFT)
#define PMCR_EL0_LP_BIT		(U(1) << 7)
#define PMCR_EL0_LC_BIT		(U(1) << 6)
#define PMCR_EL0_DP_BIT		(U(1) << 5)
#define PMCR_EL0_X_BIT		(U(1) << 4)
#define PMCR_EL0_D_BIT		(U(1) << 3)
#define PMCR_EL0_C_BIT		(U(1) << 2)
#define PMCR_EL0_P_BIT		(U(1) << 1)
#define PMCR_EL0_E_BIT		(U(1) << 0)

/*******************************************************************************
 * Definitions for system register interface to SVE
 ******************************************************************************/
#define ZCR_EL3			S3_6_C1_C2_0
#define ZCR_EL2			S3_4_C1_C2_0

/* ZCR_EL3 definitions */
#define ZCR_EL3_LEN_MASK	U(0xf)

/* ZCR_EL2 definitions */
#define ZCR_EL2_LEN_MASK	U(0xf)

/*******************************************************************************
 * Definitions for system register interface to SME as needed in EL3
 ******************************************************************************/
#define ID_AA64SMFR0_EL1		S3_0_C0_C4_5
#define SMCR_EL3			S3_6_C1_C2_6

/* ID_AA64SMFR0_EL1 definitions */
#define ID_AA64SMFR0_EL1_FA64_BIT	(UL(1) << 63)

/* SMCR_ELx definitions */
#define SMCR_ELX_LEN_SHIFT		U(0)
#define SMCR_ELX_LEN_MASK		U(0x1ff)
#define SMCR_ELX_FA64_BIT		(U(1) << 31)

/*******************************************************************************
 * Definitions of MAIR encodings for device and normal memory
 ******************************************************************************/
/*
 * MAIR encodings for device memory attributes.
 */
#define MAIR_DEV_nGnRnE		ULL(0x0)
#define MAIR_DEV_nGnRE		ULL(0x4)
#define MAIR_DEV_nGRE		ULL(0x8)
#define MAIR_DEV_GRE		ULL(0xc)

/*
 * MAIR encodings for normal memory attributes.
 *
 * Cache Policy
 *  WT:	 Write Through
 *  WB:	 Write Back
 *  NC:	 Non-Cacheable
 *
 * Transient Hint
 *  NTR: Non-Transient
 *  TR:	 Transient
 *
 * Allocation Policy
 *  RA:	 Read Allocate
 *  WA:	 Write Allocate
 *  RWA: Read and Write Allocate
 *  NA:	 No Allocation
 */
#define MAIR_NORM_WT_TR_WA	ULL(0x1)
#define MAIR_NORM_WT_TR_RA	ULL(0x2)
#define MAIR_NORM_WT_TR_RWA	ULL(0x3)
#define MAIR_NORM_NC		ULL(0x4)
#define MAIR_NORM_WB_TR_WA	ULL(0x5)
#define MAIR_NORM_WB_TR_RA	ULL(0x6)
#define MAIR_NORM_WB_TR_RWA	ULL(0x7)
#define MAIR_NORM_WT_NTR_NA	ULL(0x8)
#define MAIR_NORM_WT_NTR_WA	ULL(0x9)
#define MAIR_NORM_WT_NTR_RA	ULL(0xa)
#define MAIR_NORM_WT_NTR_RWA	ULL(0xb)
#define MAIR_NORM_WB_NTR_NA	ULL(0xc)
#define MAIR_NORM_WB_NTR_WA	ULL(0xd)
#define MAIR_NORM_WB_NTR_RA	ULL(0xe)
#define MAIR_NORM_WB_NTR_RWA	ULL(0xf)

#define MAIR_NORM_OUTER_SHIFT	U(4)

#define MAKE_MAIR_NORMAL_MEMORY(inner, outer)	\
        ((inner) | ((outer) << MAIR_NORM_OUTER_SHIFT))

/* PAR_EL1 fields */
#define PAR_F_SHIFT	U(0)
#define PAR_F_MASK	ULL(0x1)
#define PAR_ADDR_SHIFT	U(12)
#define PAR_ADDR_MASK	(BIT(40) - ULL(1)) /* 40-bits-wide page address */

/*******************************************************************************
 * RAS system registers definitions
 ******************************************************************************/
#define DISR_A_BIT		U(31)

#define ERRIDR_MASK		U(0xffff)

#define ERXCTLR_ED_SHIFT	U(0)
#define ERXCTLR_ED_BIT		(U(1) << ERXCTLR_ED_SHIFT)
#define ERXCTLR_UE_BIT		(U(1) << 4)

#define ERXPFGCTL_UC_BIT	(U(1) << 1)
#define ERXPFGCTL_UEU_BIT	(U(1) << 2)
#define ERXPFGCTL_CDEN_BIT	(U(1) << 31)

/*******************************************************************************
 * Armv8.4 Data Independent Timing Registers
 ******************************************************************************/
#define DIT			S3_3_C4_C2_5
#define DIT_BIT			BIT(24)

/*******************************************************************************
 * Armv8.5 - new MSR encoding to directly access PSTATE.SSBS field
 ******************************************************************************/
#define SSBS			S3_3_C4_C2_6

/*******************************************************************************
 * Armv8.5 - Memory Tagging Extension Registers
 ******************************************************************************/
#define TFSRE0_EL1		S3_0_C5_C6_1
#define TFSR_EL1		S3_0_C5_C6_0
#define RGSR_EL1		S3_0_C1_C0_5
#define GCR_EL1			S3_0_C1_C0_6

/*******************************************************************************
 * Armv8.5 - Random Number Generator Registers
 ******************************************************************************/
#define RNDR			S3_3_C2_C4_0
#define RNDRRS			S3_3_C2_C4_1

/*******************************************************************************
 * FEAT_HCX - Extended Hypervisor Configuration Register
 ******************************************************************************/
#define HCRX_EL2		S3_4_C1_C2_2
#define HCRX_EL2_FGTnXS_BIT	(UL(1) << 4)
#define HCRX_EL2_FnXS_BIT	(UL(1) << 3)
#define HCRX_EL2_EnASR_BIT	(UL(1) << 2)
#define HCRX_EL2_EnALS_BIT	(UL(1) << 1)
#define HCRX_EL2_EnAS0_BIT	(UL(1) << 0)

/*******************************************************************************
 * FEAT_TCR2 - Extended Translation Control Register
 ******************************************************************************/
#define TCR2_EL2		S3_4_C2_C0_3

/*******************************************************************************
 * Definitions for DynamicIQ Shared Unit registers
 ******************************************************************************/
#define CLUSTERPWRDN_EL1	S3_0_c15_c3_6

/* CLUSTERPWRDN_EL1 register definitions */
#define DSU_CLUSTER_PWR_OFF	0
#define DSU_CLUSTER_PWR_ON	1
#define DSU_CLUSTER_PWR_MASK	U(1)

/*******************************************************************************
 * Definitions for CPU Power/Performance Management registers
 ******************************************************************************/

#define CPUPPMCR_EL3			S3_6_C15_C2_0
#define CPUPPMCR_EL3_MPMMPINCTL_SHIFT	UINT64_C(0)
#define CPUPPMCR_EL3_MPMMPINCTL_MASK	UINT64_C(0x1)

#define CPUMPMMCR_EL3			S3_6_C15_C2_1
#define CPUMPMMCR_EL3_MPMM_EN_SHIFT	UINT64_C(0)
#define CPUMPMMCR_EL3_MPMM_EN_MASK	UINT64_C(0x1)

#define __DEFINE_MRS_MSR_S_REGNUM                \
"    .irp    num,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30\n" \
"    .equ    .L__reg_num_x\\num, \\num\n"            \
"    .endr\n"                        \
"    .equ    .L__reg_num_xzr, 31\n"

#define DEFINE_MRS_S                        \
    __DEFINE_MRS_MSR_S_REGNUM                \
"    .macro    mrs_s, rt, sreg\n"                \
    __emit_inst(0xd5200000|(\\sreg)|(.L__reg_num_\\rt))    \
"    .endm\n"

#define DEFINE_MSR_S                        \
    __DEFINE_MRS_MSR_S_REGNUM                \
"    .macro    msr_s, sreg, rt\n"                \
    __emit_inst(0xd5000000|(\\sreg)|(.L__reg_num_\\rt))    \
"    .endm\n"

#define UNDEFINE_MRS_S                        \
"    .purgem    mrs_s\n"

#define UNDEFINE_MSR_S                        \
"    .purgem    msr_s\n"

#define __mrs_s(v, r)                        \
    DEFINE_MRS_S                        \
"    mrs_s " v ", " __stringify(r) "\n"            \
    UNDEFINE_MRS_S

#define __msr_s(r, v)                        \
    DEFINE_MSR_S                        \
"    msr_s " __stringify(r) ", " v "\n"            \
    UNDEFINE_MSR_S

/*
 * Unlike read_cpuid, calls to read_sysreg are never expected to be
 * optimized away or replaced with synthetic values.
 */
#define read_sysreg(r) ({					\
	AL_U64 __val;						\
	asm volatile("mrs %0, " __stringify(r) : "=r" (__val));	\
	__val;							\
})

/*
 * The "Z" constraint normally means a zero immediate, but when combined with
 * the "%x0" template means XZR.
 */
#define write_sysreg(v, r) do {					\
	AL_U64 __val = (AL_U64)(v);					\
	asm volatile("msr " __stringify(r) ", %x0"		\
		     : : "rZ" (__val));				\
} while (0)

/*
 * For registers without architectural names, or simply unsupported by
 * GAS.
 */
#define read_sysreg_s(r) ({						\
	AL_U64 __val;							\
	asm volatile(__mrs_s("%0", r) : "=r" (__val));			\
	__val;								\
})

#define write_sysreg_s(v, r) do {					\
	AL_U64 __val = (AL_U64)(v);						\
	asm volatile(__msr_s(r, "%x0") : : "rZ" (__val));		\
} while (0)

/*
 * Modify bits in a sysreg. Bits in the clear mask are zeroed, then bits in the
 * set mask are set. Other bits are left as-is.
 */
#define sysreg_clear_set(sysreg, clear, set) do {			\
	AL_U64 __scs_val = read_sysreg(sysreg);				\
	AL_U64 __scs_new = (__scs_val & ~(AL_U64)(clear)) | (set);		\
	if (__scs_new != __scs_val)					\
		write_sysreg(__scs_new, sysreg);			\
} while (0)

#define sysreg_clear_set_s(sysreg, clear, set) do {			\
	AL_U64 __scs_val = read_sysreg_s(sysreg);				\
	AL_U64 __scs_new = (__scs_val & ~(AL_U64)(clear)) | (set);		\
	if (__scs_new != __scs_val)					\
		write_sysreg_s(__scs_new, sysreg);			\
} while (0)

#define read_sysreg_par() ({						\
	AL_U64 par;							\
	asm("dmb sy");	\
	par = read_sysreg(par_el1);					\
	asm("dmb sy");	\
	par;								\
})

#endif /* __SYSREG_H */
