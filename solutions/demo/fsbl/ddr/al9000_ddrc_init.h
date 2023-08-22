#ifndef AL9000_DDRC_INIT_H
#define AL9000_DDRC_INIT_H

#include "alc_ddr_env.h"
#include "al9000_ddrc.h"
#include "./al9000_ddrc_regs/al9000_ddrc_gpll_regs.h"
#include "./al9000_ddrc_regs/al9000_ddrc_dpll_regs.h"
#include "./al9000_ddrc_regs/al9000_ddrc_ppc_regs.h"
#include "./al9000_ddrc_regs/al9000_ddrc_bank_iomc1_regs.h"
#include "./al9000_ddrc_regs/al9000_ddrc_bank_glue_regs.h"
#include "./al9000_ddrc_regs/al9000_ddrc_umctl2_regs.h"
#include "./al9000_ddrc_regs/al9000_ddrc_bankref_regs.h"

// Configuration
void al9000_ddrppc_base_cfg();
void al9000_ddrppc_set_ddrmr();
void al9000_ddrppc_set_timing();
void al9000_mtest_set_addr(u8 bank, u16 row, u16 col);

void al9000_ddrppc_fast_init();
void al9000_ddrmc_init(u8 sd_ecc_en);
void al9000_ddrmc_post_cfg();

void al9000_iomc_internal_loopback_cfg();
void al9000_zq_overwrite_cfg();
void al9000_pub_training_cfg();
void al9000_do_training(u8 wl_en, u8 wl2_en, u8 gt_en);
// Execution
void al9000_ddrppc_mdl_cal ();
void al9000_ddrppc_dram_init();

// MTEST
int al9000_ddrppc_mtest(u8 bank, u16 row, u16 col, u8 byteNum);

#endif // end of AL9000_DDRC_INIT_H

