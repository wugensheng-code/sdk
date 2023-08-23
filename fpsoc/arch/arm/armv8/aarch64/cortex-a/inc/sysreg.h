#ifndef __SYSREG_H
#define __SYSREG_H

#ifdef __ASSEMBLY__
#define _AC(X,Y)	X
#else
#define _AC(X,Y)	(X##Y)
#endif

#define U(x)		(_AC(x, U))
#define UL(x)		(_AC(x, UL))
#define ULL(x)		(_AC(x, ULL))
#define L(_x)		(_AC(x, L))
#define LL(_x)		(_AC(x, LL))

#ifndef BIT
#define BIT(nr)     (UL(1) << (nr))
#endif

#ifndef BIT_32
#define BIT_32(nr)	(U(1) << (nr))
#endif

#ifndef BIT_64
#define BIT_64(nr)	(ULL(1) << (nr))
#endif

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

/* CPSR/SPSR definitions */
#define DAIF_FIQ_BIT		(U(1) << 0)
#define DAIF_IRQ_BIT		(U(1) << 1)
#define DAIF_ABT_BIT		(U(1) << 2)
#define DAIF_DBG_BIT		(U(1) << 3)
#define SPSR_DAIF_SHIFT		U(6)
#define SPSR_DAIF_MASK		U(0xf)

#define SPSR_AIF_SHIFT		U(6)
#define SPSR_AIF_MASK		U(0x7)
#define SPSR_AIF            (SPSR_AIF_MASK << SPSR_AIF_SHIFT)

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
#define SPSR_M_EL1H		U(0x5)
#define SPSR_M_EL0T		U(0x0)

#define SPSR_EL_SHIFT		U(2)
#define SPSR_EL_WIDTH		U(2)

#define SPSR_SSBS_SHIFT_AARCH64 U(12)
#define SPSR_SSBS_BIT_AARCH64	(ULL(1) << SPSR_SSBS_SHIFT_AARCH64)
#define SPSR_SSBS_SHIFT_AARCH32 U(23)
#define SPSR_SSBS_BIT_AARCH32	(ULL(1) << SPSR_SSBS_SHIFT_AARCH32)

#define SPSR_PAN_BIT		BIT_64(22)

#define SPSR_DIT_BIT		BIT(24)

#define SPSR_TCO_BIT_AARCH64	BIT_64(25)

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

#endif /* __SYSREG_H */
