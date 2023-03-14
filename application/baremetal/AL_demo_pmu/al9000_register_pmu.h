#ifndef _AL9000_REGISTER_PMU_H_
#define _AL9000_REGISTER_PMU_H_

//--------------------------------------------------------------------------------

#define TOP_NS__BASE_ADDR 0xF8800000ULL

///////////////////////////////////////////////////////
// Register: PLS_PROT
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned fahb_proten : 1;
        unsigned gp_proten : 1;
        unsigned reserved_31_2 : 30;
    };
    unsigned reg;
} TOP_NS__PLS_PROT__ACC_T;

#define TOP_NS__PLS_PROT__ADDR (TOP_NS__BASE_ADDR + 0x080ULL)

///////////////////////////////////////////////////////
// Register: CFG_CTRL_WDT
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned ctrl_wdt_0 : 1;
        unsigned ctrl_wdt_1 : 1;
        unsigned ctrl_wdt_2 : 1;
        unsigned ctrl_wdt_7_3 : 5;
        unsigned ctrl_smc : 8;
        unsigned reserved_31_16 : 16;
    };
    unsigned reg;
} TOP_NS__CFG_CTRL_WDT__ACC_T;

#define TOP_NS__CFG_CTRL_WDT__ADDR (TOP_NS__BASE_ADDR + 0x168ULL)

///////////////////////////////////////////////////////
// Register: STATE_RPU
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned state_rpu : 32;
    };
    unsigned reg;
} TOP_NS__STATE_RPU__ACC_T;

#define TOP_NS__STATE_RPU__ADDR (TOP_NS__BASE_ADDR + 0x17CULL)

///////////////////////////////////////////////////////
// Register: CFG_CTRL_OCM
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned ecc_en : 1;
        unsigned dbgm_sel : 3;
        unsigned sbit_intr_en : 1;
        unsigned dbit_intr_en : 1;
        unsigned sbit_intr_msk : 1;
        unsigned dbit_intr_msk : 1;
        unsigned reserved : 24;
    };
    unsigned reg;
} TOP_NS__CFG_CTRL_OCM__ACC_T;

#define TOP_NS__CFG_CTRL_OCM__ADDR (TOP_NS__BASE_ADDR + 0x1BCULL)

//--------------------------------------------------------------------------------

#define CRP__BASE_ADDR 0xF8801000ULL

///////////////////////////////////////////////////////
// Register: CLK_ERR_HIS
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned err_clkdet_cpupll : 1;
        unsigned err_clkdet_iopll : 1;
        unsigned err_clkdet_ddrpll : 1;
        unsigned reserved_31_3 : 29;
    };
    unsigned reg;
} CRP__CLK_ERR_HIS__ACC_T;

#define CRP__CLK_ERR_HIS__ADDR (CRP__BASE_ADDR + 0x480ULL)

///////////////////////////////////////////////////////
// Register: REF_THD
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned refclk_cnt_thd : 16;
        unsigned reserved_31_16 : 16;
    };
    unsigned reg;
} CRP__REF_THD__ACC_T;

#define CRP__REF_THD__ADDR (CRP__BASE_ADDR + 0x484ULL)

///////////////////////////////////////////////////////
// Register: TGT_HTHD_CPUPLL
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tgtclk_cnt_hthd_cpupll : 32;
    };
    unsigned reg;
} CRP__TGT_HTHD_CPUPLL__ACC_T;

#define CRP__TGT_HTHD_CPUPLL__ADDR (CRP__BASE_ADDR + 0x490ULL)

///////////////////////////////////////////////////////
// Register: TGT_LTHD_CPUPLL
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tgtclk_cnt_lthd_cpupll : 32;
    };
    unsigned reg;
} CRP__TGT_LTHD_CPUPLL__ACC_T;

#define CRP__TGT_LTHD_CPUPLL__ADDR (CRP__BASE_ADDR + 0x494ULL)

///////////////////////////////////////////////////////
// Register: TGT_HTHD_IOPLL
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tgtclk_cnt_hthd_iopll : 32;
    };
    unsigned reg;
} CRP__TGT_HTHD_IOPLL__ACC_T;

#define CRP__TGT_HTHD_IOPLL__ADDR (CRP__BASE_ADDR + 0x498ULL)

///////////////////////////////////////////////////////
// Register: TGT_LTHD_IOPLL
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned tgtclk_cnt_lthd_iopll : 32;
    };
    unsigned reg;
} CRP__TGT_LTHD_IOPLL__ACC_T;

#define CRP__TGT_LTHD_IOPLL__ADDR (CRP__BASE_ADDR + 0x49CULL)

//--------------------------------------------------------------------------------

#define TOP_S__BASE_ADDR 0xF8806000ULL

///////////////////////////////////////////////////////
// Register: ERR_HW_EN0_SET
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned err_hw_en_ddr_ecc : 1;
        unsigned err_hw_en_ocm_ecc : 1;
        unsigned err_hw_en_rpu_fatal : 1;
        unsigned err_hw_en_rpu_norm : 1;
        unsigned err_hw_en_apu_ecc : 1;
        unsigned reserved_7_5 : 3;
        unsigned err_hw_en_wdt0_ot : 1;
        unsigned err_hw_en_wdt1_ot : 1;
        unsigned err_hw_en_wdt2_ot : 1;
        unsigned err_hw_en_vcc : 1;
        unsigned reserved_17_12 : 6;
        unsigned err_hw_en_clk_detec : 1;
        unsigned reserved_19 : 1;
        unsigned err_hw_en_bus_timeout : 1;
        unsigned reserved_23_21 : 3;
        unsigned err_hw_en_pl0 : 1;
        unsigned err_hw_en_pl1 : 1;
        unsigned err_hw_en_pl2 : 1;
        unsigned err_hw_en_pl3 : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} TOP_S__ERR_HW_EN0_SET__ACC_T;

#define TOP_S__ERR_HW_EN0_SET__ADDR (TOP_S__BASE_ADDR + 0x000ULL)


///////////////////////////////////////////////////////
// Register: ERR_HW_EN0_CLR
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned err_hw_en_ddr_ecc : 1;
        unsigned err_hw_en_ocm_ecc : 1;
        unsigned err_hw_en_rpu_fatal : 1;
        unsigned err_hw_en_rpu_norm : 1;
        unsigned err_hw_en_apu_ecc : 1;
        unsigned reserved_7_5 : 3;
        unsigned err_hw_en_wdt0_ot : 1;
        unsigned err_hw_en_wdt1_ot : 1;
        unsigned err_hw_en_wdt2_ot : 1;
        unsigned err_hw_en_vcc : 1;
        unsigned reserved_17_12 : 6;
        unsigned err_hw_en_clk_detec : 1;
        unsigned reserved_19 : 1;
        unsigned err_hw_en_bus_timeout : 1;
        unsigned reserved_23_21 : 3;
        unsigned err_hw_en_pl0 : 1;
        unsigned err_hw_en_pl1 : 1;
        unsigned err_hw_en_pl2 : 1;
        unsigned err_hw_en_pl3 : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} TOP_S__ERR_HW_EN0_CLR__ACC_T;

#define TOP_S__ERR_HW_EN0_CLR__ADDR (TOP_S__BASE_ADDR + 0x004ULL)

///////////////////////////////////////////////////////
// Register: ERR_HW_EN1_SET
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned err_hw_en_pll0_lock : 1;
        unsigned err_hw_en_pll1_lock : 1;
        unsigned err_hw_en_pll2_lock : 1;
        unsigned reserved_7_3 : 5;
        unsigned err_hw_en_csu : 1;
        unsigned reserved_12_9 : 4;
        unsigned err_hw_en_csu_boot : 1;
        unsigned reserved_31_14 : 18;
    };
    unsigned reg;
} TOP_S__ERR_HW_EN1_SET__ACC_T;

#define TOP_S__ERR_HW_EN1_SET__ADDR (TOP_S__BASE_ADDR + 0x008ULL)


///////////////////////////////////////////////////////
// Register: ERR_HW_EN1_CLR
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned err_hw_en_pll0_lock : 1;
        unsigned err_hw_en_pll1_lock : 1;
        unsigned err_hw_en_pll2_lock : 1;
        unsigned reserved_7_3 : 5;
        unsigned err_hw_en_csu : 1;
        unsigned reserved_12_9 : 4;
        unsigned err_hw_en_csu_boot : 1;
        unsigned reserved_31_14 : 18;
    };
    unsigned reg;
} TOP_S__ERR_HW_EN1_CLR__ACC_T;

#define TOP_S__ERR_HW_EN1_CLR__ADDR (TOP_S__BASE_ADDR + 0x00CULL)


///////////////////////////////////////////////////////
// Register: INT_EN0_SET
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_en_ddr_ecc : 1;
        unsigned int_en_ocm_ecc : 1;
        unsigned int_en_rpu_fatal : 1;
        unsigned int_en_rpu_norm : 1;
        unsigned int_en_apu_ecc : 1;
        unsigned reserved_7_5 : 3;
        unsigned int_en_wdt0_ot : 1;
        unsigned int_en_wdt1_ot : 1;
        unsigned int_en_wdt2_ot : 1;
        unsigned int_en_vcc : 1;
        unsigned reserved_17_12 : 6;
        unsigned int_en_clk_detec : 1;
        unsigned reserved_19 : 1;
        unsigned int_en_bus_timeout : 1;
        unsigned reserved_23_21 : 3;
        unsigned int_en_pl0 : 1;
        unsigned int_en_pl1 : 1;
        unsigned int_en_pl2 : 1;
        unsigned int_en_pl3 : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} TOP_S__INT_EN0_SET__ACC_T;

#define TOP_S__INT_EN0_SET__ADDR (TOP_S__BASE_ADDR + 0x010ULL)


///////////////////////////////////////////////////////
// Register: INT_EN0_CLR
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_en_ddr_ecc : 1;
        unsigned int_en_ocm_ecc : 1;
        unsigned int_en_rpu_fatal : 1;
        unsigned int_en_rpu_norm : 1;
        unsigned int_en_apu_ecc : 1;
        unsigned reserved_7_5 : 3;
        unsigned int_en_wdt0_ot : 1;
        unsigned int_en_wdt1_ot : 1;
        unsigned int_en_wdt2_ot : 1;
        unsigned int_en_vcc : 1;
        unsigned reserved_17_12 : 6;
        unsigned int_en_clk_detec : 1;
        unsigned reserved_19 : 1;
        unsigned int_en_bus_timeout : 1;
        unsigned reserved_23_21 : 3;
        unsigned int_en_pl0 : 1;
        unsigned int_en_pl1 : 1;
        unsigned int_en_pl2 : 1;
        unsigned int_en_pl3 : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} TOP_S__INT_EN0_CLR__ACC_T;

#define TOP_S__INT_EN0_CLR__ADDR (TOP_S__BASE_ADDR + 0x014ULL)

///////////////////////////////////////////////////////
// Register: INT_EN1_SET
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_en_pll0_lock : 1;
        unsigned int_en_pll1_lock : 1;
        unsigned int_en_pll2_lock : 1;
        unsigned reserved_7_3 : 5;
        unsigned int_en_csu : 1;
        unsigned reserved_12_9 : 4;
        unsigned int_en_csu_boot : 1;
        unsigned reserved_31_14 : 18;
    };
    unsigned reg;
} TOP_S__INT_EN1_SET__ACC_T;

#define TOP_S__INT_EN1_SET__ADDR (TOP_S__BASE_ADDR + 0x018ULL)

///////////////////////////////////////////////////////
// Register: INT_EN1_CLR
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_en_pll0_lock : 1;
        unsigned int_en_pll1_lock : 1;
        unsigned int_en_pll2_lock : 1;
        unsigned reserved_7_3 : 5;
        unsigned int_en_csu : 1;
        unsigned reserved_12_9 : 4;
        unsigned int_en_csu_boot : 1;
        unsigned reserved_31_14 : 18;
    };
    unsigned reg;
} TOP_S__INT_EN1_CLR__ACC_T;

#define TOP_S__INT_EN1_CLR__ADDR (TOP_S__BASE_ADDR + 0x01CULL)

///////////////////////////////////////////////////////
// Register: INT_MSK0_SET
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_msk_ddr_ecc : 1;
        unsigned int_msk_ocm_ecc : 1;
        unsigned int_msk_rpu_fatal : 1;
        unsigned int_msk_rpu_norm : 1;
        unsigned int_msk_apu_ecc : 1;
        unsigned reserved_7_5 : 3;
        unsigned int_msk_wdt0_ot : 1;
        unsigned int_msk_wdt1_ot : 1;
        unsigned int_msk_wdt2_ot : 1;
        unsigned int_msk_vcc : 1;
        unsigned reserved_17_12 : 6;
        unsigned int_msk_clk_detec : 1;
        unsigned reserved_19 : 1;
        unsigned int_msk_bus_timeout : 1;
        unsigned reserved_23_21 : 3;
        unsigned int_msk_pl0 : 1;
        unsigned int_msk_pl1 : 1;
        unsigned int_msk_pl2 : 1;
        unsigned int_msk_pl3 : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} TOP_S__INT_MSK0_SET__ACC_T;

#define TOP_S__INT_MSK0_SET__ADDR (TOP_S__BASE_ADDR + 0x020ULL)

///////////////////////////////////////////////////////
// Register: INT_MSK0_CLR
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_msk_ddr_ecc : 1;
        unsigned int_msk_ocm_ecc : 1;
        unsigned int_msk_rpu_fatal : 1;
        unsigned int_msk_rpu_norm : 1;
        unsigned int_msk_apu_ecc : 1;
        unsigned reserved_7_5 : 3;
        unsigned int_msk_wdt0_ot : 1;
        unsigned int_msk_wdt1_ot : 1;
        unsigned int_msk_wdt2_ot : 1;
        unsigned int_msk_vcc : 1;
        unsigned reserved_17_12 : 6;
        unsigned int_msk_clk_detec : 1;
        unsigned reserved_19 : 1;
        unsigned int_msk_bus_timeout : 1;
        unsigned reserved_23_21 : 3;
        unsigned int_msk_pl0 : 1;
        unsigned int_msk_pl1 : 1;
        unsigned int_msk_pl2 : 1;
        unsigned int_msk_pl3 : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} TOP_S__INT_MSK0_CLR__ACC_T;

#define TOP_S__INT_MSK0_CLR__ADDR (TOP_S__BASE_ADDR + 0x024ULL)

///////////////////////////////////////////////////////
// Register: INT_MSK1_SET
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_msk_pll0_lock : 1;
        unsigned int_msk_pll1_lock : 1;
        unsigned int_msk_pll2_lock : 1;
        unsigned reserved_7_3 : 5;
        unsigned int_msk_csu : 1;
        unsigned reserved_12_9 : 4;
        unsigned int_msk_csu_boot : 1;
        unsigned reserved_31_14 : 18;
    };
    unsigned reg;
} TOP_S__INT_MSK1_SET__ACC_T;

#define TOP_S__INT_MSK1_SET__ADDR (TOP_S__BASE_ADDR + 0x028ULL)


///////////////////////////////////////////////////////
// Register: INT_MSK1_CLR
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_msk_pll0_lock : 1;
        unsigned int_msk_pll1_lock : 1;
        unsigned int_msk_pll2_lock : 1;
        unsigned reserved_7_3 : 5;
        unsigned int_msk_csu : 1;
        unsigned reserved_12_9 : 4;
        unsigned int_msk_csu_boot : 1;
        unsigned reserved_31_14 : 18;
    };
    unsigned reg;
} TOP_S__INT_MSK1_CLR__ACC_T;

#define TOP_S__INT_MSK1_CLR__ADDR (TOP_S__BASE_ADDR + 0x02CULL)

///////////////////////////////////////////////////////
// Register: INT_CLR0
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_clr_ddr_ecc : 1;
        unsigned int_clr_ocm_ecc : 1;
        unsigned int_clr_rpu_fatal : 1;
        unsigned int_clr_rpu_norm : 1;
        unsigned int_clr_apu_ecc : 1;
        unsigned reserved_7_5 : 3;
        unsigned int_clr_wdt0_ot : 1;
        unsigned int_clr_wdt1_ot : 1;
        unsigned int_clr_wdt2_ot : 1;
        unsigned int_clr_vcc : 1;
        unsigned reserved_17_12 : 6;
        unsigned int_clr_clk_detec : 1;
        unsigned reserved_19 : 1;
        unsigned int_clr_bus_timeout : 1;
        unsigned reserved_23_21 : 3;
        unsigned int_clr_pl0 : 1;
        unsigned int_clr_pl1 : 1;
        unsigned int_clr_pl2 : 1;
        unsigned int_clr_pl3 : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} TOP_S__INT_CLR0__ACC_T;

#define TOP_S__INT_CLR0__ADDR (TOP_S__BASE_ADDR + 0x030ULL)

///////////////////////////////////////////////////////
// Register: INT_CLR1
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_clr_pll0_lock : 1;
        unsigned int_clr_pll1_lock : 1;
        unsigned int_clr_pll2_lock : 1;
        unsigned reserved_7_3 : 5;
        unsigned int_clr_csu : 1;
        unsigned reserved_12_9 : 4;
        unsigned int_clr_csu_boot : 1;
        unsigned reserved_31_14 : 18;
    };
    unsigned reg;
} TOP_S__INT_CLR1__ACC_T;

#define TOP_S__INT_CLR1__ADDR (TOP_S__BASE_ADDR + 0x034ULL)

///////////////////////////////////////////////////////
// Register: INT_STATE0
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_ddr_ecc : 1;
        unsigned int_ocm_ecc : 1;
        unsigned int_rpu_fatal : 1;
        unsigned int_rpu_norm : 1;
        unsigned int_apu_ecc : 1;
        unsigned reserved_7_5 : 3;
        unsigned int_wdt0_ot : 1;
        unsigned int_wdt1_ot : 1;
        unsigned int_wdt2_ot : 1;
        unsigned int_vcc : 1;
        unsigned reserved_17_12 : 6;
        unsigned int_clk_detec : 1;
        unsigned reserved_19 : 1;
        unsigned int_bus_timeout : 1;
        unsigned reserved_23_21 : 3;
        unsigned int_pl0 : 1;
        unsigned int_pl1 : 1;
        unsigned int_pl2 : 1;
        unsigned int_pl3 : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} TOP_S__INT_STATE0__ACC_T;

#define TOP_S__INT_STATE0__ADDR (TOP_S__BASE_ADDR + 0x040ULL)

///////////////////////////////////////////////////////
// Register: INT_STATE1
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned int_pll0_lock : 1;
        unsigned int_pll1_lock : 1;
        unsigned int_pll2_lock : 1;
        unsigned reserved_7_3 : 5;
        unsigned int_csu : 1;
        unsigned reserved_12_9 : 4;
        unsigned int_csu_boot : 1;
        unsigned reserved_31_14 : 18;
    };
    unsigned reg;
} TOP_S__INT_STATE1__ACC_T;

#define TOP_S__INT_STATE1__ADDR (TOP_S__BASE_ADDR + 0x044ULL)

///////////////////////////////////////////////////////
// Register: RAW_HIS0
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned err_ddr_ecc : 1;
        unsigned err_ocm_ecc : 1;
        unsigned err_rpu_fatal : 1;
        unsigned err_rpu_norm : 1;
        unsigned err_apu_ecc : 1;
        unsigned reserved_7_5 : 3;
        unsigned err_wdt0_ot : 1;
        unsigned err_wdt1_ot : 1;
        unsigned err_wdt2_ot : 1;
        unsigned err_vcc : 1;
        unsigned reserved_17_12 : 6;
        unsigned err_clk_detec : 1;
        unsigned reserved_19 : 1;
        unsigned err_bus_timeout : 1;
        unsigned reserved_23_21 : 3;
        unsigned err_pl0 : 1;
        unsigned err_pl1 : 1;
        unsigned err_pl2 : 1;
        unsigned err_pl3 : 1;
        unsigned reserved_31_28 : 4;
    };
    unsigned reg;
} TOP_S__RAW_HIS0__ACC_T;

#define TOP_S__RAW_HIS0__ADDR (TOP_S__BASE_ADDR + 0x048ULL)

///////////////////////////////////////////////////////
// Register: RAW_HIS1
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned err_pll0_lock : 1;
        unsigned err_pll1_lock : 1;
        unsigned err_pll2_lock : 1;
        unsigned reserved_7_3 : 5;
        unsigned err_csu : 1;
        unsigned reserved_12_9 : 4;
        unsigned err_csu_boot : 1;
        unsigned reserved_31_14 : 18;
    };
    unsigned reg;
} TOP_S__RAW_HIS1__ACC_T;

#define TOP_S__RAW_HIS1__ADDR (TOP_S__BASE_ADDR + 0x04CULL)

///////////////////////////////////////////////////////
// Register: PMU_NMI_EN
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned pmu_nmi_en_csu : 1;
        unsigned pmu_nmi_en_rpu : 1;
        unsigned reserved_31_2 : 30;
    };
    unsigned reg;
} TOP_S__PMU_NMI_EN__ACC_T;

#define TOP_S__PMU_NMI_EN__ADDR (TOP_S__BASE_ADDR + 0x050ULL)

///////////////////////////////////////////////////////
// Register: SW_PMU_SHACK
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned proc_done : 1;
        unsigned fiq_ack : 1;
        unsigned hdrstcnt_clr : 1;
        unsigned reserved_31_3 : 29;
    };
    unsigned reg;
} TOP_S__SW_PMU_SHACK__ACC_T;

#define TOP_S__SW_PMU_SHACK__ADDR (TOP_S__BASE_ADDR + 0x080ULL)

///////////////////////////////////////////////////////
// Register: PMU_STAT
// 
///////////////////////////////////////////////////////

typedef union {
    struct {
        unsigned pmu_fsm : 2;
        unsigned reserved_31_14 : 30;
    };
    unsigned reg;
} TOP_S__PMU_STAT__ACC_T;

#define TOP_S__PMU_STAT__ADDR (TOP_S__BASE_ADDR + 0x084ULL)


#define AL_PMU_ERROR	0x1
#define AL_PMU_SUCCESS	0x0

#define CFG_CTRL_WDT_ADDR           TOP_NS__CFG_CTRL_WDT__ADDR
#define CFG_CTRL_OCM_ADDR           TOP_NS__CFG_CTRL_OCM__ADDR
#define TOP_NS_PLS_PROT_ADDR        TOP_NS__PLS_PROT__ADDR
#define CRP_TGT_HTHD_CPUPLL_ADDR    CRP__TGT_HTHD_CPUPLL__ADDR
#define STATE_RPU_ADDR              TOP_NS__STATE_RPU__ADDR

#define CFG_CTRL_WDT_T          TOP_NS__CFG_CTRL_WDT__ACC_T
#define CFG_CTRL_OCM_T          TOP_NS__CFG_CTRL_OCM__ACC_T
#define TOP_NS_PLS_PROT_T       TOP_NS__PLS_PROT__ACC_T
#define CRP_TGT_HTHD_CPUPLL_T   CRP__TGT_HTHD_CPUPLL__ACC_T

#define ERR_HW_EN0_SET_ADDR		TOP_S__ERR_HW_EN0_SET__ADDR
#define ERR_HW_EN0_CLR_ADDR		TOP_S__ERR_HW_EN0_CLR__ADDR
#define INT_EN0_SET_ADDR		TOP_S__INT_EN0_SET__ADDR
#define INT_EN0_CLR_ADDR		TOP_S__INT_EN0_CLR__ADDR
#define INT_MSK0_SET_ADDR		TOP_S__INT_MSK0_SET__ADDR
#define INT_MSK0_CLR_ADDR		TOP_S__INT_MSK0_CLR__ADDR
#define INT_CLR0_ADDR			TOP_S__INT_CLR0__ADDR
#define INT_STATE0_ADDR			TOP_S__INT_STATE0__ADDR
#define RAW_HIS0_ADDR			TOP_S__RAW_HIS0__ADDR
#define PMU_NMI_EN_ADDR			TOP_S__PMU_NMI_EN__ADDR
#define SW_PMU_SHACK_ADDR		TOP_S__SW_PMU_SHACK__ADDR
#define PMU_STAT_ADDR			TOP_S__PMU_STAT__ADDR

#define ERR_HW_EN0_SET_T				TOP_S__ERR_HW_EN0_SET__ACC_T	
#define ERR_HW_EN0_CLR_T				TOP_S__ERR_HW_EN0_CLR__ACC_T
#define INT_EN0_SET_T					TOP_S__INT_EN0_SET__ACC_T
#define INT_EN0_CLR_T					TOP_S__INT_EN0_CLR__ACC_T
#define INT_MSK0_SET_T					TOP_S__INT_MSK0_SET__ACC_T
#define INT_MSK0_CLR_T					TOP_S__INT_MSK0_CLR__ACC_T
#define INT_CLR0_T						TOP_S__INT_CLR0__ACC_T
#define INT_STATE0_T					TOP_S__INT_STATE0__ACC_T
#define RAW_HIS0_T						TOP_S__RAW_HIS0__ACC_T
#define PMU_NMI_EN_T					TOP_S__PMU_NMI_EN__ACC_T
#define SW_PMU_SHACK_T					TOP_S__SW_PMU_SHACK__ACC_T
#define PMU_STAT_T						TOP_S__PMU_STAT__ACC_T

#define ERR_OCM_ECC_MASK	    (0x1 << 1)
#define ERR_RPU_FATAL_MASK	    (0x1 << 2)
#define ERR_APU_ECC_MASK	    (0x1 << 4)
#define ERR_WDT0_OT_MASK	    (0x1 << 8)
#define ERR_WDT1_OT_MASK	    (0x1 << 9)
#define ERR_WDT2_OT_MASK	    (0x1 << 10)
#define ERR_CLK_DETECT_MASK	    (0x1 << 18)
#define ERR_BUS_TIMEOUT_MASK	(0x1 << 20)

#define PMU_NMI_EN_RPU_MASK     (0x1 << 1)

#define OCM_START_ADDR      (0x61000000)
#define OCM_END_ADDR        (OCM_START_ADDR + 0x40000)

#define ILM_START_ADDR      (0x60000000)
#define DLM_START_ADDR      (0x60040000)

//CTRL_OCM MASK
#define CO_CFG_ECC_EN         (0x1 << 0)
#define CO_CFG_DBIT_INTR_EN   (0x1 << 5)
#define CO_CFG_DBIT_INTR_MSK  (0x1 << 7)

//GP0 START ADDRESS
#define GP0_START_ADDR  0x80000000



#endif /* end of _AL9000_REGISTER_PMU_H_ */
