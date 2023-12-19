#ifndef __AL_DR1X90_DDRC_INIT_H_
#define __AL_DR1X90_DDRC_INIT_H_

#include "alc_ddr_env.h"
#include "dr1x90_ddrc.h"

#define DDR3_TYPE 3
#define DDR4_TYPE 4
typedef int ddr_type_t;

#define DDR_X8  1
#define DDR_X16 2
#define DDR_X32 4
typedef int ddr_width_t;

#define DDR_ECC_NONE     0
#define DDR_ECC_SIDEBAND 1
#define DDR_ECC_INLINE   2
typedef int ddr_ecc_t;

typedef struct ddr_timing_t
{
    u32 nCL;
    u32 nRCD;
    u32 nRP;
    u32 nCWL;
    double tRAS;
    double tRC;
    double tCCD_L;
    double tRRD_S;
    double tRRD_L;
    double tFAW;
    double tRFC;
    double tREFI;
} ddr_timing_t;

// AXI AxADDR to SDRAM Address Mapping
typedef struct ddr_addrmap_t
{
    u8  ba[3];
    u8  bg[2];
    u8 col[10];
    u8 row[18];
} ddr_addrmap_t;

typedef struct ddr_arbiter_t
{
    // TODO
} ddr_arbiter_t;

// DDRC init Proc
int dr1x90_ddrc_init(double fck, ddr_type_t type, ddr_width_t width, ddr_ecc_t ecc, const ddr_timing_t* timpara, const ddr_addrmap_t* addrmap, const ddr_arbiter_t* arbiter_cfg);

// GPLL & DPLL Config
void dr1x90_ddr_gpll_cfg(double fck);  // Frequency of CK (MHz)
void dr1x90_ddr_dpll_cfg();

// Bank Config
void dr1x90_ddr_iob_cfg();
void dr1x90_ddr_iol_cfg();
void dr1x90_ddr_busmatrix_cfg(ddr_type_t type);
void dr1x90_iomc_internal_loopback_cfg();

// PPC Config
void dr1x90_ddrppc_base_cfg(ddr_type_t type, ddr_width_t width, ddr_ecc_t ecc);
void dr1x90_ddrppc_set_ddrmr(double fck, ddr_type_t type, const ddr_timing_t* timpara);
void dr1x90_ddrppc_set_timing();
void dr1x90_ddrppc_fast_init();
void dr1x90_ddrppc_show_delay();
void dr1x90_zq_overwrite_cfg(ddr_type_t type);
void dr1x90_pub_training_cfg(ddr_type_t type);
void dr1x90_do_training(u8 wl_en, u8 wl2_en, u8 gt_en);

// MTEST
void dr1x90_mtest_set_addr(u8 bank, u16 row, u16 col);
int dr1x90_ddrppc_mtest(u8 bank, u16 row, u16 col, u8 byteNum);

// MC Config
void dr1x90_ddrmc_cfg(double fck, ddr_type_t type, ddr_width_t width, ddr_ecc_t ecc, const ddr_timing_t* timpara, const ddr_addrmap_t* addrmap, const ddr_arbiter_t* arbiter_cfg);
void dr1x90_ddrmc_post_cfg();
void dr1x90_ddrmc_init_cfg(double fck, ddr_type_t type, const ddr_timing_t* timpara);
void dr1x90_ddrmc_timing_cfg(double fck, ddr_type_t type, const ddr_timing_t* timpara);
void dr1x90_ddrmc_addrmap_cfg(ddr_type_t type, ddr_width_t width, const ddr_addrmap_t* addrmap);
void dr1x90_ddrmc_arbiter_cfg(const ddr_arbiter_t* arbiter_cfg);

// Execution
void dr1x90_ddrppc_mdl_cal();
void dr1x90_ddrppc_dram_init();

// MISC Functions
u32 get_nwr(double fck, ddr_type_t type);
void make_ddr_mr(double fck, ddr_type_t type, const ddr_timing_t* timpara, u32* MR);
double ns2cycle(double t, double tck);

#include "ddr_regs/dr1x90_ddrc_gpll_regs.h"
#include "ddr_regs/dr1x90_ddrc_dpll_regs.h"
#include "ddr_regs/dr1x90_ddrc_ppc_regs.h"
#include "ddr_regs/dr1x90_ddrc_bank_iomc1_regs.h"
#include "ddr_regs/dr1x90_ddrc_bank_glue_regs.h"
#include "ddr_regs/dr1x90_ddrc_umctl2_regs.h"
#include "ddr_regs/dr1x90_ddrc_bankref_regs.h"

#endif // end of AL_DR1X90_DDRC_INIT_H
