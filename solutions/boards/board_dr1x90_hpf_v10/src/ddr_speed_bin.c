#include <string.h>
#include "fd_ddr_init.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define PARA_SEL_BY_WIDTH(para_x8, para_x16) ((FD_PARA_DRAM_WIDTH == DDR_COMP_WIDTH_x8) ? (para_x8) : (para_x16))

static double set_param_by_width(double para_x8, double para_x16, u32 dram_width)
{
    return (dram_width == DDR_COMP_WIDTH_x8) ? para_x8 : para_x16;
}

static double set_trfc_by_density(u32 dram_density)
{
    return  dram_density <=  512 ?  90.0 : \
            dram_density <= 1024 ? 110.0 : \
            dram_density <= 2048 ? 160.0 : \
            dram_density <= 4096 ? 260.0 : \
            dram_density <= 8196 ? 350.0 : \
            550.0; 
}

#define TREFI (7.8 * 1e3)    // 0 - 85C
// #define TREFI (3.9 * 1e3)    // 85 - 95C

static const ddr_timing_t SPEED_BIN_DDR3_800D = {
    .nCL  = 5,
    .nCWL = 5,
    .nRCD = 5,
    .nRP  = 5,
    .tRAS = 37.5,
    .tRC  = 50.0,
    .tCCD_L = 0.0,
    .tRRD_S = 10.0,
    .tRRD_L = PARA_SEL_BY_WIDTH(10.0, 10.0),
    .tWTR_S = 0.0,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(40.0, 50.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR3_800E = {
    .nCL  = 6,
    .nCWL = 5,
    .nRCD = 6,
    .nRP  = 6,
    .tRAS = 37.5,
    .tRC  = 52.5,
    .tCCD_L = 0.0,
    .tRRD_S = 10.0,
    .tRRD_L = PARA_SEL_BY_WIDTH(10.0, 10.0),
    .tWTR_S = 0.0,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(40.0, 50.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR3_1066E = {
    .nCL  = 6,
    .nCWL = 6,
    .nRCD = 6,
    .nRP  = 6,
    .tRAS = 37.5,
    .tRC  = 48.75,
    .tCCD_L = 0.0,
    .tRRD_S = 10.0,
    .tRRD_L = PARA_SEL_BY_WIDTH(7.5, 10.0),
    .tWTR_S = 0.0,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(37.5, 50.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR3_1066F = {
    .nCL  = 7,
    .nCWL = 6,
    .nRCD = 7,
    .nRP  = 7,
    .tRAS = 37.5,
    .tRC  = 50.625,
    .tCCD_L = 0.0,
    .tRRD_S = 10.0,
    .tRRD_L = PARA_SEL_BY_WIDTH(7.5, 10.0),
    .tWTR_S = 0.0,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(37.5, 50.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR3_1066G = {
    .nCL  = 8,
    .nCWL = 6,
    .nRCD = 8,
    .nRP  = 8,
    .tRAS = 37.5,
    .tRC  = 52.5,
    .tCCD_L = 0.0,
    .tRRD_S = 10.0,
    .tRRD_L = PARA_SEL_BY_WIDTH(7.5, 10.0),
    .tWTR_S = 0.0,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(37.5, 50.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR3_1333F = {
    .nCL  = 7,
    .nCWL = 7,
    .nRCD = 7,
    .nRP  = 7,
    .tRAS = 36.0,
    .tRC  = 46.5,
    .tCCD_L = 0.0,
    .tRRD_S = 10.0,
    .tRRD_L = PARA_SEL_BY_WIDTH(6.0, 7.5),
    .tWTR_S = 0.0,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(30.0, 45.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR3_1333G = {
    .nCL  = 8,
    .nCWL = 7,
    .nRCD = 8,
    .nRP  = 8,
    .tRAS = 36.0,
    .tRC  = 48.0,
    .tCCD_L = 0.0,
    .tRRD_S = 10.0,
    .tRRD_L = PARA_SEL_BY_WIDTH(6.0, 7.5),
    .tWTR_S = 0.0,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(30.0, 45.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR3_1333H = {
    .nCL  = 9,
    .nCWL = 7,
    .nRCD = 9,
    .nRP  = 9,
    .tRAS = 36.0,
    .tRC  = 49.5,
    .tCCD_L = 0.0,
    .tRRD_S = 10.0,
    .tRRD_L = PARA_SEL_BY_WIDTH(6.0, 7.5),
    .tWTR_S = 0.0,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(30.0, 45.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR3_1333J = {
    .nCL  = 10,
    .nCWL = 7,
    .nRCD = 10,
    .nRP  = 10,
    .tRAS = 36.0,
    .tRC  = 51.0,
    .tCCD_L = 0.0,
    .tRRD_S = 10.0,
    .tRRD_L = PARA_SEL_BY_WIDTH(6.0, 7.5),
    .tWTR_S = 0.0,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(30.0, 45.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR4_1600J = {
    .nCL  = 10,
    .nCWL = 9,
    .nRCD = 10,
    .nRP  = 10,
    .tRAS = 35.0,
    .tRC  = 47.5,
    .tCCD_L = 6.250,
    .tRRD_S = PARA_SEL_BY_WIDTH(5.0, 6.0),
    .tRRD_L = PARA_SEL_BY_WIDTH(6.0, 7.5),
    .tWTR_S = 2.5,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(25.0, 35.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR4_1600K = {
    .nCL  = 11,
    .nCWL = 9,
    .nRCD = 11,
    .nRP  = 11,
    .tRAS = 35.0,
    .tRC  = 48.75,
    .tCCD_L = 6.250,
    .tRRD_S = PARA_SEL_BY_WIDTH(5.0, 6.0),
    .tRRD_L = PARA_SEL_BY_WIDTH(6.0, 7.5),
    .tWTR_S = 2.5,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(25.0, 35.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t SPEED_BIN_DDR4_1600L = {
    .nCL  = 12,
    .nCWL = 9,
    .nRCD = 12,
    .nRP  = 12,
    .tRAS = 35.0,
    .tRC  = 50.0,
    .tCCD_L = 6.250,
    .tRRD_S = PARA_SEL_BY_WIDTH(5.0, 6.0),
    .tRRD_L = PARA_SEL_BY_WIDTH(6.0, 7.5),
    .tWTR_S = 2.5,
    .tWTR_L = 7.5,
    .tRTP   = 7.5,
    .tFAW   = PARA_SEL_BY_WIDTH(25.0, 35.0),
    .tRFC   = 260.0,
    .tREFI  = TREFI
};

static const ddr_timing_t* speed_bin_list[] = {
    &SPEED_BIN_DDR3_800D , &SPEED_BIN_DDR3_800E , 
    &SPEED_BIN_DDR3_1066E, &SPEED_BIN_DDR3_1066F, &SPEED_BIN_DDR3_1066G, 
    &SPEED_BIN_DDR3_1333F, &SPEED_BIN_DDR3_1333G, &SPEED_BIN_DDR3_1333H, &SPEED_BIN_DDR3_1333J, 
    &SPEED_BIN_DDR4_1600J, &SPEED_BIN_DDR4_1600K, &SPEED_BIN_DDR4_1600L
};
static const speed_bin_size = sizeof(speed_bin_list) / sizeof(const ddr_timing_t*);

static u32 autoSel_speed_bin(double fck, u32 ddr_type)
{
    if (ddr_type == DDR3_TYPE) {
        if (fck < 400.0)
            return 1;   // SPEED_BIN_DDR3_800E
        else if (fck < 533.4)
            return 4;   // SPEED_BIN_DDR3_1066G
        else
            return 8;   // SPEED_BIN_DDR3_1333J
    }
    else {
        return 9;   // SPEED_BIN_DDR4_1600J;
    }
    return (u32)-1;
}

ddr_timing_t get_timing_para(double fck, const ddr_params_t* parm)
{
    const double tck = 1e3 / fck;
    ddr_timing_t para;
    
    u32 bin_idx = parm->speed_bin_index;
    if (bin_idx >= speed_bin_size)
        bin_idx = autoSel_speed_bin(fck, parm->type);
    memcpy(&para, speed_bin_list[bin_idx], sizeof(ddr_timing_t));

    // set param by ddr width
    if (bin_idx < 2) {
        // DDR3 800E and 800D
        para.tRRD_L = 10.0;
        para.tFAW   = set_param_by_width(40.0, 50.0, parm->dram_width);
    } else if (bin_idx < 5) {
        // DDR3 1066E 1066F 1066G
        para.tRRD_L = set_param_by_width(7.5,  10.0, parm->dram_width);
        para.tFAW   = set_param_by_width(37.5, 50.0, parm->dram_width);
    } else if (bin_idx < 9) {
        // DDR3 1333F 1333G 1333H 1333J
        para.tRRD_L = set_param_by_width(6.0,  7.5,  parm->dram_width);
        para.tFAW   = set_param_by_width(30.0, 45.0, parm->dram_width);
    } else if (bin_idx < 12) {
        // DDR4 1600J 1600K 1600L
        para.tRRD_S = set_param_by_width(5.0,   6.0, parm->dram_width),
        para.tRRD_L = set_param_by_width(6.0,   7.5, parm->dram_width);
        para.tFAW   = set_param_by_width(25.0, 35.0, parm->dram_width);
    } else {
        /* do nothing*/
    }
    // set param by 
    para.tRFC = set_trfc_by_density(parm->dram_density);

    if (parm->type == DDR4_TYPE) {
        para.tCCD_L = MAX(para.tCCD_L, tck * 5);
        para.tRRD_S = MAX(para.tRRD_S, tck * 4);
        para.tRRD_L = MAX(para.tRRD_L, tck * 4);
        para.tWTR_S = MAX(para.tWTR_S, tck * 2);
        para.tWTR_L = MAX(para.tWTR_L, tck * 4);
        para.tRTP   = MAX(para.tRTP, tck * 4);
        para.tFAW   = MAX(para.tFAW, tck * set_param_by_width(20, 28, parm->dram_width));
    }
    return para;
}
