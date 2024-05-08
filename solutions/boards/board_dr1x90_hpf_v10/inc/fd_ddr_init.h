#ifndef AL_FD_DDR_INIT_H_
#define AL_FD_DDR_INIT_H_

#include "soc_plat.h"
#include "dr1x90_ddrc_init.h"

typedef struct ddr_params_t
{
    u32 osc_clk          ;
    u32 type             ;
    u32 speed            ; 
    u32 dq_width         ;
    float io_vol         ;
    u32 verf             ;
    u32 pzq              ;
    u32 dram_width       ;
    u32 speed_bin_index  ;
    u32 wdbi             ;
    u32 rdbi             ;
    u32 dram_density     ;
    u32 ecc              ;
    u32 addr_map         ;
    u32 training         ;
    float byte0_ac_dely  ;
    float byte0_dqs_dely ;
    float byte1_ac_dely  ;
    float byte1_dqs_dely ;
    float byte2_ac_dely  ;
    float byte2_dqs_dely ;
    float byte3_ac_dely  ;
    float byte3_dqs_dely ;

} ddr_params_t;

// Call dr1x90_ddrc_init(...)
// Used By FSBL
int fd_ddr_init();

#endif
