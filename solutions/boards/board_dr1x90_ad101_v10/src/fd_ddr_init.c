#include <stdio.h>
#include <string.h>
#include "fd_ddr_init.h"

ddr_timing_t  get_timing_para(double fck, const ddr_params_t* parm);
ddr_addrmap_t get_addrmap(const ddr_params_t* parm);
int ddr_init_by_params(const ddr_params_t* parm);
int ddr_init_by_tcl(const ddr_params_t* parm);

// called by socplat_init
int fd_ddr_init()
{   
    ddr_params_t param = {
        .osc_clk         = SYSTEM_CLOCK          ,
        .type            = FD_PARA_DDR_TYPE      ,
        .speed           = FD_PARA_DDR_SPEED     ,
        .dq_width        = FD_PARA_DQ_WIDTH      ,
        .io_vol          = FD_PARA_IO_VOLT       ,
        .verf            = FD_PARA_VREF          ,
        .pzq             = FD_PARA_PZQ_ENABLE    ,
        .dram_width      = FD_PARA_DRAM_WIDTH    ,
        .speed_bin_index = FD_PARA_SPEED_BIN     ,
        .wdbi            = FD_PARA_WDM           ,
        .rdbi            = FD_PARA_RDBI          ,
        .dram_density    = FD_PARA_DRAM_DENSITY  ,
        .ecc             = FD_PARA_ECC           ,
        .addr_map        = FD_PARA_ADDRMAP       ,
        .training        = FD_PARA_TRAINING      ,
        .byte0_ac_dely   = FD_PARA_BYTE0_AC_DLY  ,
        .byte0_dqs_dely  = FD_PARA_BYTE0_DQS_DLY ,
        .byte1_ac_dely   = FD_PARA_BYTE1_AC_DLY  ,
        .byte1_dqs_dely  = FD_PARA_BYTE1_DQS_DLY ,
        .byte2_ac_dely   = FD_PARA_BYTE2_AC_DLY  ,
        .byte2_dqs_dely  = FD_PARA_BYTE2_DQS_DLY ,
        .byte3_ac_dely   = FD_PARA_BYTE3_AC_DLY  ,
        .byte3_dqs_dely  = FD_PARA_BYTE3_DQS_DLY 
    };

    int err = ddr_init_by_params(&param);
    return err;
}

// called by ddr_init_tcl
int ddr_init_by_tcl(const ddr_params_t* parm)
{
    ddr_params_t param;
    memcpy(&param, (void*)parm, sizeof(ddr_params_t));
    int err = ddr_init_by_params(&param);
    return err;
}

int ddr_init_by_params(const ddr_params_t* parm)
{
    // Total Size in MB
    size_t total_size = parm->dram_density / 8 * parm->dq_width / parm->dram_width;
    uint32_t dram_depth = parm->dram_density / parm->dram_width;
    if (parm->ecc == DDR_ECC_INLINE)
        total_size = total_size * 7 / 8;
    printf("[DDR INFO] DRAM %dM x%d, Use DQ[%d:0] %s\r\n", dram_depth, parm->dram_width, 
        parm->dq_width - 1, parm->ecc == DDR_ECC_SIDEBAND ? "+ DQ[31:24]" : ""
    );
    printf("[DDR INFO] Available Capacity %ld MB\r\n", total_size);

    const double f_ck = (double)parm->speed / 2.0;
    const double fsys = (double)parm->osc_clk * 1e-6;
    ddr_width_t width = parm->dq_width / 8;
    ddr_basic_t basic_cfg = {
        .fck   = f_ck,
        .fsys  = fsys,
        .type  = parm->type,
        .width = width ,
        .ecc   = parm->ecc,
        .ecc_poison = ECC_POISON_NONE,
        .vref  = parm->verf,
        .size  = 1024UL * 1024UL * total_size
    };

    ddr_train_t train_cfg = {
        .pzq   = 0, // parm->pzq
        .dcc   = 0,
        .fast  = !parm->training,
        .wl    =  parm->training,
        .wladj =  parm->training,
        .gate  =  parm->training,
        .reye  =  parm->training,
        .weye  =  parm->training,
        .ac_dly = { (double)parm->byte0_ac_dely , (double)parm->byte1_ac_dely  , 
                    (double)parm->byte2_ac_dely , (double)parm->byte3_ac_dely  },
        .dx_dly = { (double)parm->byte0_dqs_dely, (double)parm->byte0_dqs_dely , 
                    (double)parm->byte0_dqs_dely, (double)parm->byte0_dqs_dely },
        .wlsl   = { -1,  -1,  -1,  -1},
        .wld    = { -1,  -1,  -1,  -1},
        .dgsl   = { -1,  -1,  -1,  -1},
        .dqsgd  = { -1,  -1,  -1,  -1},
        .wdqd   = { -1,  -1,  -1,  -1},
        .rdqsd  = { -1,  -1,  -1,  -1},
        .rdqsnd = { -1,  -1,  -1,  -1}
    };

    ddr_timing_t timpara = get_timing_para(f_ck, parm);

    ddr_addrmap_t addrmap = get_addrmap(parm);

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
    err = dr1x90_ddrc_init(FW_VERSION, &basic_cfg, &train_cfg, &timpara, &addrmap, &arbiter_cfg);

    return err;
}

static u8 clog2(uint64_t x)
{
    u8 i = 0;
    for (i = 0; x != 0; ++i)
        x >>= 1;
    return i;
}

#define MAP_INVALID 0
#define MAP_COL     1
#define MAP_ROW     2
#define MAP_BA      3
#define MAP_BG      4
#define MAP_MAXSIZE 8

typedef struct ddr_addrmap_config_t
{
    int len;
    int type[MAP_MAXSIZE];
    int size[MAP_MAXSIZE];
} ddr_addrmap_config_t;

static int map_append(ddr_addrmap_config_t* cfg, int type, int size)
{
    int idx = cfg->len;
    cfg->type[idx] = type;
    cfg->size[idx] = size;
    cfg->len = ++idx;
    return idx;
}

ddr_addrmap_t get_addrmap(const ddr_params_t* parm)
{
    ddr_addrmap_config_t config;
    memset(&config, 0, sizeof(ddr_addrmap_config_t));
    ddr_addrmap_t map;
    memset(&map, -1, sizeof(ddr_addrmap_t));

    const uint64_t depth = parm->dram_density / parm->dram_width;
    const int width = clog2(depth - 1) + 20;

    const int col_ec = (parm->ecc == DDR_ECC_INLINE) ? 3 : 0;
    const int col_lo = 
        parm->dq_width == 32 ? 2 :
        parm->dq_width == 16 ? 3 : 4;
    const int col_mi = 2;
    const int col_hi = 10 - col_mi - col_lo - col_ec;
    const int ba = (parm->type == DDR3_TYPE) ? 3 : 2;
    const int bg = (parm->type == DDR3_TYPE) ? 0 : 
                   (parm->dram_width == 16 ) ? 1 : 2;
    const int row = width - 10 - ba - bg;

    switch (parm->addr_map)
    {
    case ROW_COL_BANK:
        map_append(&config, MAP_COL, col_mi);
        map_append(&config, MAP_BG , bg    );
        map_append(&config, MAP_BA , ba    );
        map_append(&config, MAP_COL, col_hi);
        map_append(&config, MAP_ROW, row   );
        map_append(&config, MAP_COL, col_ec);
        break;
    case ROW_BANK_COL:
        map_append(&config, MAP_COL, col_mi);
        map_append(&config, MAP_COL, col_hi);
        map_append(&config, MAP_BG , bg    );
        map_append(&config, MAP_BA , ba    );
        map_append(&config, MAP_ROW, row   );
        map_append(&config, MAP_COL, col_ec);
        break;
    case BANK_ROW_COL:
        map_append(&config, MAP_COL, col_mi);
        map_append(&config, MAP_COL, col_hi);
        map_append(&config, MAP_ROW, row   );
        map_append(&config, MAP_BG , bg    );
        map_append(&config, MAP_BA , ba    );
        map_append(&config, MAP_COL, col_ec);
        break;
    }

    uint8_t axaddr = 4;
    uint8_t cdx = (uint8_t)col_lo;
    uint8_t rdx = 0;
    uint8_t bax = 0;
    uint8_t bgx = 0;

    for (int i = 0; i < config.len; ++i) {
        int type = config.type[i];
        int size = config.size[i];
        for (int j = 0; j < size; ++j) {
            switch (type)
            {
            case MAP_COL:
                map.col[cdx] = axaddr;
                ++cdx;
                break;
            case MAP_ROW:
                map.row[rdx] = axaddr;
                ++rdx;
                break;
            case MAP_BA:
                map.ba [bax] = axaddr;
                ++bax;
                break;
            case MAP_BG:
                map.bg [bgx] = axaddr;
                ++bgx;
                break;
            default:
                break;
            }
            ++axaddr;
        }
    }
    return map;
}
/*
static u8 find(const u8* arr, int n, u8 target)
{
    for (int i = 0; i < n; ++i) {
        if (arr[i] == target)
            return (u8)i;
    }
    return (u8)-1;
}

ddr_addrmap_t get_addrmap(const ddr_params_t* parm)
{
    ddr_addrmap_t map;
    memset(&map, -1, sizeof(ddr_addrmap_t));

    const size_t total_size = parm->dram_density / 8 * parm->dq_width / parm->dram_width;
    u8 width = clog2(total_size - 1) + 20;

    u8 ba[3] = {-1, -1, -1};
    u8 bg[2] = {-1, -1};

    u8 cdx = 4;
    cdx = parm->dq_width == 32 ? 2 :
          parm->dq_width == 16 ? 3 :
          4;
    u8 rdx = 0;

    switch (parm->addr_map)
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
    switch (parm->type)
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
*/
