#ifndef __AL_DR1X90_DDRC_FUNC_H_
#define __AL_DR1X90_DDRC_FUNC_H_

#include "dr1x90_ddrc.h"
#include "dr1x90_ddrc_init.h"

// GPLL & DPLL Config
void dr1x90_ddr_gpll_cfg(double fck, double fsys);  // Frequency of CK (MHz)
void dr1x90_ddr_dpll_cfg();

// Bank Config
void dr1x90_ddr_iob_cfg(ddr_type_t type);
void dr1x90_ddr_iol_cfg(ddr_type_t type);
void dr1x90_ddr_iob_vref_cfg(ddr_vref_t vref_sel, u32 vref);
void dr1x90_ddr_busmatrix_cfg(ddr_type_t type);
void dr1x90_iomc_internal_loopback_cfg();
void dr1x90_zq_overwrite_v2(ddr_type_t type, const u32* zq_code, u32 vref);

// PPC Config
void dr1x90_ddrppc_base_cfg(ddr_type_t type, u32 lane_mask);
void dr1x90_ddrppc_set_ddrmr(double fck, ddr_type_t type, const ddr_timing_t* timpara);
void dr1x90_ddrppc_set_timing();
void dr1x90_ddrppc_fast_init(double fck, u32 lane_mask, const ddr_train_t* train_cfg);
void dr1x90_ddrppc_show_delay();
void dr1x90_ddrppc_show_bdl();
void dr1x90_zq_overwrite_cfg(ddr_type_t type);
void dr1x90_pub_training_cfg(ddr_type_t type);
void dr1x90_do_training(u8 wl_en, u8 wl2_en, u8 gt_en);

// MTEST
void dr1x90_mtest_set_addr(u8 bank, u16 row, u16 col);
int dr1x90_ddrppc_mtest(u8 bank, u16 row, u16 col, u8 byteNum);

// MC Config
void dr1x90_ddrmc_cfg(const ddr_basic_t* basic_cfg, const ddr_timing_t* timpara, const ddr_addrmap_t* addrmap, const ddr_arbiter_t* arbiter_cfg);
void dr1x90_ddrmc_post_cfg();
void dr1x90_ddrmc_init_cfg(double fck, ddr_type_t type, const ddr_timing_t* timpara);
void dr1x90_ddrmc_timing_cfg(double fck, ddr_type_t type, const ddr_timing_t* timpara);
void dr1x90_ddrmc_addrmap_cfg(ddr_type_t type, ddr_width_t width, const ddr_addrmap_t* addrmap);
void dr1x90_ddrmc_arbiter_cfg(ddr_type_t type, const ddr_arbiter_t* arbiter_cfg);

// Execution
void dr1x90_ddrppc_mdl_cal();
void dr1x90_ddrppc_show_mdl(double fck, u32 lane_mask);
void dr1x90_ddrppc_zq_cal(ddr_type_t type);
void dr1x90_ddrppc_dram_init();

// Training
void dr1x90_ddrc_train_dcc();
void dr1x90_ddrc_train_wl();
void dr1x90_ddrc_train_gate();
void dr1x90_ddrc_train_wladj();
void dr1x90_ddrc_train_eye();

// MISC Functions
void dr1x90_release_ddr_bus();
u32 get_nwr(double fck, ddr_type_t type);
void make_ddr_mr(double fck, ddr_type_t type, const ddr_timing_t* timpara, u32* MR);

#include "ddr_regs/dr1x90_ddrc_gpll_regs.h"
#include "ddr_regs/dr1x90_ddrc_dpll_regs.h"
#include "ddr_regs/dr1x90_ddrc_ppc_regs.h"
#include "ddr_regs/dr1x90_ddrc_bank_iomc1_regs.h"
#include "ddr_regs/dr1x90_ddrc_bank_glue_regs.h"
#include "ddr_regs/dr1x90_ddrc_umctl2_regs.h"
#include "ddr_regs/dr1x90_ddrc_bankref_regs.h"

#endif
