

#include <string.h>
#include "fd_ddr_init.h"

// Total Size in MB
static unsigned long long TOTAL_SZIE = 0;
ddr_timing_t get_timing_para(double fck, ddr_params_t ddr_params);
ddr_addrmap_t get_addrmap();
int ddr_init_by_params();
int ddr_init_by_tcl(ddr_params_t* params_addr);

// default params config
ddr_params_t ddr_params = {
    .osc_clk         = 33330000,
    .type            = 3       ,
    .speed           = 1066    ,
    .dq_width        = 32      ,

    .io_vol          = 1.5     ,
    .verf            = 0       ,
    .pzq             = 0       ,
    .dram_width      = 8       ,

    .speed_bin_index = 4       ,
    .wdbi            = 1       ,
    .rdbi            = 0       ,
    .dram_density    = 4096    ,

    .ecc             = 0       ,
    .addr_map        = 0       ,
    .training        = 1       ,
    .byte0_ac_dely   = 0.0     ,

    .byte0_dqs_dely  = 0.0     ,
    .byte1_ac_dely   = 0.0     ,
    .byte1_dqs_dely  = 0.0     ,
    .byte2_ac_dely   = 0.0     ,

    .byte2_dqs_dely  = 0.0     ,
    .byte3_ac_dely   = 0.0     ,
    .byte3_dqs_dely  = 0.0  
};

// called by socplat_init
int fd_ddr_init()
{   
    int err = 0;
    // update params
   ddr_params.osc_clk         = SYSTEM_CLOCK          ;
   ddr_params.type            = FD_PARA_DDR_TYPE      ;
   ddr_params.speed           = FD_PARA_DDR_SPEED     ;
   ddr_params.dq_width        = FD_PARA_DQ_WIDTH      ;
   ddr_params.io_vol          = FD_PARA_IO_VOLT       ;
   ddr_params.verf            = FD_PARA_VREF          ;
   ddr_params.pzq             = FD_PARA_PZQ_ENABLE    ;
   ddr_params.dram_width      = FD_PARA_DRAM_WIDTH    ;
   ddr_params.speed_bin_index = FD_PARA_SPEED_BIN     ;
   ddr_params.wdbi            = FD_PARA_WDM           ;
   ddr_params.rdbi            = FD_PARA_RDBI          ;
   ddr_params.dram_density    = FD_PARA_DRAM_DENSITY  ;
   ddr_params.ecc             = FD_PARA_ECC           ;
   ddr_params.addr_map        = FD_PARA_ADDRMAP       ;
   ddr_params.training        = FD_PARA_TRAINING      ;
   ddr_params.byte0_ac_dely   = FD_PARA_BYTE0_AC_DLY  ;
   ddr_params.byte0_dqs_dely  = FD_PARA_BYTE0_DQS_DLY ;
   ddr_params.byte1_ac_dely   = FD_PARA_BYTE1_AC_DLY  ;
   ddr_params.byte1_dqs_dely  = FD_PARA_BYTE1_DQS_DLY ;
   ddr_params.byte2_ac_dely   = FD_PARA_BYTE2_AC_DLY  ;
   ddr_params.byte2_dqs_dely  = FD_PARA_BYTE2_DQS_DLY ;
   ddr_params.byte3_ac_dely   = FD_PARA_BYTE3_AC_DLY  ;
   ddr_params.byte3_dqs_dely  = FD_PARA_BYTE3_DQS_DLY ;

    err = ddr_init_by_params();
    return err;
}

// called by ddr_init_tcl
int ddr_init_by_tcl(ddr_params_t* params_addr)
{
     int err = 0;
     memcpy(&ddr_params, (void*)params_addr, sizeof(ddr_params_t));
     err = ddr_init_by_params();
     return err;
}

int ddr_init_by_params()
{
    // update params
    TOTAL_SZIE = ddr_params.dram_density / 8 * ddr_params.dq_width / ddr_params.dram_width;
    const double f_ck = (double)(ddr_params.speed) / 2;
    const double fsys = (double)ddr_params.osc_clk * 1e-6;
    ddr_width_t width = ddr_params.dq_width  / 8;
    ddr_basic_t basic_cfg = {
        .fck   = f_ck,
        .fsys  = fsys,
        // gxrao
        .type  = ddr_params.type,
        .width = width ,
        .ecc   = ddr_params.ecc,
        .ecc_poison = ECC_POISON_NONE,
        .size  = 1024UL * 1024UL * TOTAL_SZIE
    };

    ddr_train_t train_cfg = {
        .pzq   = ddr_params.pzq,
        .dcc   = 0,
        .fast  = !ddr_params.training,
        .wl    = ddr_params.training,
        .wladj = ddr_params.training,
        .gate  = ddr_params.training,
        .reye  = ddr_params.training,
        .weye  = ddr_params.training,
        .ac_dly = { (double)ddr_params.byte0_ac_dely , (double)ddr_params.byte1_ac_dely , 
                    (double)ddr_params.byte2_ac_dely , (double)ddr_params.byte3_ac_dely },
        
        .dx_dly = {(double)ddr_params.byte0_dqs_dely, (double)ddr_params.byte0_dqs_dely, 
                    (double)ddr_params.byte0_dqs_dely, (double)ddr_params.byte0_dqs_dely},
        .wlsl   = { -1,  -1,  -1,  -1},
        .wld    = { -1,  -1,  -1,  -1},
        .dgsl   = { -1,  -1,  -1,  -1},
        .dqsgd  = { -1,  -1,  -1,  -1},
        .wdqd   = { -1,  -1,  -1,  -1},
        .rdqsd  = { -1,  -1,  -1,  -1},
        .rdqsnd = { -1,  -1,  -1,  -1}
    };

    ddr_timing_t timpara = get_timing_para(f_ck, ddr_params);

    ddr_addrmap_t addrmap = get_addrmap();

    ddr_arbiter_t arbiter_cfg = {
        .lpr_num        = 31  ,
        .hpr_xact_run   = 8   ,
        .hpr_max_starve = 64  ,
        .lpr_xact_run   = 16  ,
        .lpr_max_starve = 64  ,
        .w_xact_run     = 16  ,
        .w_max_starve   = 64  ,
        .port[0].rd_aging     = 200 ,   .port[1].rd_aging     = -1  ,   .port[2].rd_aging     = 500 ,   .port[3].rd_aging     = 500 ,
        .port[0].wr_aging     = 200 ,   .port[1].wr_aging     = -1  ,   .port[2].wr_aging     = 500 ,   .port[3].wr_aging     = 500 ,
        .port[0].rqos_level1  = 7   ,   .port[1].rqos_level1  = 7   ,   .port[2].rqos_level1  = 7   ,   .port[3].rqos_level1  = 7   ,
        .port[0].rqos_region0 = LPR ,   .port[1].rqos_region0 = LPR ,   .port[2].rqos_region0 = LPR ,   .port[3].rqos_region0 = LPR ,
        .port[0].rqos_region1 = VPR ,   .port[1].rqos_region1 = VPR ,   .port[2].rqos_region1 = VPR ,   .port[3].rqos_region1 = VPR ,
        .port[0].wqos_level1  = 5   ,   .port[1].wqos_level1  = 5   ,   .port[2].wqos_level1  = 5   ,   .port[3].wqos_level1  = 5   ,
        .port[0].wqos_level2  = 10  ,   .port[1].wqos_level2  = 10  ,   .port[2].wqos_level2  = 10  ,   .port[3].wqos_level2  = 10  ,
        .port[0].wqos_region0 = NPW ,   .port[1].wqos_region0 = NPW ,   .port[2].wqos_region0 = NPW ,   .port[3].wqos_region0 = NPW ,
        .port[0].wqos_region1 = VPW ,   .port[1].wqos_region1 = VPW ,   .port[2].wqos_region1 = VPW ,   .port[3].wqos_region1 = VPW ,
        .port[0].wqos_region2 = VPW ,   .port[1].wqos_region2 = VPW ,   .port[2].wqos_region2 = VPW ,   .port[3].wqos_region2 = VPW ,
        .port[0].vpr_timeout  = 100 ,   .port[1].vpr_timeout  = 100 ,   .port[2].vpr_timeout  = 100 ,   .port[3].vpr_timeout  = 100 ,
        .port[0].vpw_timeout1 = 100 ,   .port[1].vpw_timeout1 = 100 ,   .port[2].vpw_timeout1 = 100 ,   .port[3].vpw_timeout1 = 100 ,
        .port[0].vpw_timeout2 = 100 ,   .port[1].vpw_timeout2 = 100 ,   .port[2].vpw_timeout2 = 100 ,   .port[3].vpw_timeout2 = 100
    };

    int err = 0;
    err = dr1x90_ddrc_init(&basic_cfg, &train_cfg, &timpara, &addrmap, &arbiter_cfg);

    return err;
}

static u8 clog2(uint64_t x)
{
    u8 i = 0;
    for (i = 0; x != 0; ++i)
        x >>= 1;
    return i;
}

static u8 find(const u8* arr, int n, u8 target)
{
    for (int i = 0; i < n; ++i) {
        if (arr[i] == target)
            return (u8)i;
    }
    return (u8)-1;
}

ddr_addrmap_t get_addrmap()
{
    ddr_addrmap_t map;
    memset(&map, -1, sizeof(ddr_addrmap_t));

    u8 width = clog2(TOTAL_SZIE - 1) + 20;

    u8 ba[3] = {-1, -1, -1};
    u8 bg[2] = {-1, -1};

    u8 cdx = 4;
    cdx = ddr_params.dq_width == 32 ? 2 :
          ddr_params.dq_width == 16 ? 3 :
          4;
    u8 rdx = 0;

    switch (ddr_params.addr_map)
    {
        case ROW_COL_BANK:
            ba[0] = 6;
            break;
        case ROW_BANK_COL:
            ba[0] = 4 + 10 - cdx;
            break;
        case BANK_ROW_COL:
            ba[0] = width - 2;
            break;
    }
    switch (ddr_params.type)
    {
    case DDR3_TYPE:
        ba[1] = ba[0] + 1;
        ba[2] = ba[0] + 2;
        break;
    case DDR4_TYPE:
        bg[0] = ba[0];
        ba[0] = ba[0] + 1;
        ba[1] = ba[0] + 1;
        break;
    }

    u8 idx = (u8)-1;

    for (u8 axaddr = 4; axaddr < width; ++axaddr) {
        idx = find(ba, 3, axaddr);
        if (idx != (u8)-1) {
            map.ba[idx] = axaddr;
            continue;
        }
        idx = find(bg, 2, axaddr);
        if (idx != (u8)-1) {
            map.bg[idx] = axaddr;
            continue;
        }
        if (cdx < 10) {
            map.col[cdx] = axaddr;
            ++cdx;
            continue;
        }
        map.row[rdx] = axaddr;
        ++rdx;
    }

    return map;
}
