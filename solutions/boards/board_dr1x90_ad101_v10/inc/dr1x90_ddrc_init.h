#ifndef __AL_DR1X90_DDRC_INIT_H_
#define __AL_DR1X90_DDRC_INIT_H_

#include "alc_types.h"

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

#define ECC_POISON_NONE 0
#define ECC_POISON_1BIT 1
#define ECC_POISON_2BIT 2

typedef struct ddr_basic_t
{
    double fck;
    double fsys;
    int type;
    int width;
    int ecc;
    int ecc_poison;
    size_t size;
} ddr_basic_t;

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
    double tWTR_S;
    double tWTR_L;
    double tRTP;
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

typedef struct ddr_train_t
{
    u8 pzq;
    u8 dcc;
    u8 fast;
    u8 wl;
    u8 wladj;
    u8 gate;
    u8 reye;
    u8 weye;
    double ac_dly[4];
    double dx_dly[4];
    u16 wlsl  [4];
    u16 wld   [4];
    u16 dgsl  [4];
    u16 dqsgd [4];
    u16 wdqd  [4];
    u16 rdqsd [4];
    u16 rdqsnd[4];
} ddr_train_t;

#define LPR 0
#define VPR 1
#define HPR 2
#define NPW 0
#define VPW 1

typedef struct ddr_port_t
{
    u16 rd_aging;       // 0 ~ 1023
    u16 wr_aging;       // 0 ~ 1023
    u8 rqos_level1;     // [0 ~ 14]
    u8 rqos_region0;    // ARQOS: [0 ~ level1]      -> region0
    u8 rqos_region1;    // ARQOS: [level1 + 1 ~ 15] -> region1
    u8 wqos_level1;     // [0 ~ 13]
    u8 wqos_level2;     // [level1 + 1 ~ 14]
    u8 wqos_region0;    // AWQOS: [0 ~ level1]          -> region0
    u8 wqos_region1;    // AWQOS: [level1 + 1 ~ level2] -> region1
    u8 wqos_region2;    // AWQOS: [level2 + 1 ~ 15]     -> region2
    u16 vpr_timeout;    // 0 ~ 2047
    u16 vpw_timeout1;   // 0 ~ 2047
    u16 vpw_timeout2;   // 0 ~ 2047
} ddr_port_t;

typedef struct ddr_arbiter_t
{
    u16 lpr_num;        // 0 ~ 31
    u16 hpr_xact_run;   // 0 ~ 255
    u16 hpr_max_starve; // 1 ~ 65535
    u16 lpr_xact_run;
    u16 lpr_max_starve;
    u16 w_xact_run;
    u16 w_max_starve;
    ddr_port_t port[4];
} ddr_arbiter_t;

// DDRC init Proc
int dr1x90_ddrc_init(
    const ddr_basic_t* basic_cfg,
    const ddr_train_t* train_cfg, const ddr_timing_t* timpara, 
    const ddr_addrmap_t* addrmap, const ddr_arbiter_t* arbiter_cfg
);
// Return 1 if DDRC init is done
int dr1x90_ddrc_is_init();

#endif // end of AL_DR1X90_DDRC_INIT_H
