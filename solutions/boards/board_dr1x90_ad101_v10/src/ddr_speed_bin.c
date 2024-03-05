#include <string.h>
#include "fd_ddr_init.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define PARA_SEL_BY_WIDTH(para_x8, para_x16) ((FD_PARA_DRAM_WIDTH == DDR_COMP_WIDTH_x8) ? (para_x8) : (para_x16))

static float set_param_by_width(float para_x8, float para_x16, ddr_params_t ddr_params) {
    return ((ddr_params.dram_width == DDR_COMP_WIDTH_x8) ? (para_x8) : (para_x16));
}

static float set_trfc_by_density(u32 dram_density) {
    return  dram_density <=  512 ?  90.0 : \
            dram_density <= 1024 ? 110.0 : \
            dram_density <= 2048 ? 160.0 : \
            dram_density <= 4096 ? 260.0 : \
            dram_density <= 8196 ? 350.0 : \
            550.0; 
}

#define TREFI (7.8 * 1e3)    // 0 - 85C
// #define TREFI (3.9 * 1e3)    // 85 - 95C

static ddr_timing_t SPEED_BIN_DDR3_800D = {
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

static ddr_timing_t SPEED_BIN_DDR3_800E = {
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

static ddr_timing_t SPEED_BIN_DDR3_1066E = {
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

static ddr_timing_t SPEED_BIN_DDR3_1066F = {
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

static ddr_timing_t SPEED_BIN_DDR3_1066G = {
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

static ddr_timing_t SPEED_BIN_DDR3_1333F = {
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

static ddr_timing_t SPEED_BIN_DDR3_1333G = {
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

static ddr_timing_t SPEED_BIN_DDR3_1333H = {
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

static ddr_timing_t SPEED_BIN_DDR3_1333J = {
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

static ddr_timing_t SPEED_BIN_DDR4_1600J = {
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

static ddr_timing_t SPEED_BIN_DDR4_1600K = {
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

static ddr_timing_t SPEED_BIN_DDR4_1600L = {
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

static ddr_timing_t speed_bin_list[20] = {};

ddr_timing_t get_timing_para(double fck, ddr_params_t ddr_params)
{
    double tck = 1e3 / fck;
    ddr_timing_t para;
    speed_bin_list[0] = SPEED_BIN_DDR3_800D;
    speed_bin_list[1] = SPEED_BIN_DDR3_800E;
    speed_bin_list[2] = SPEED_BIN_DDR3_1066E;
    speed_bin_list[3] = SPEED_BIN_DDR3_1066F;
    speed_bin_list[4] = SPEED_BIN_DDR3_1066G;
    speed_bin_list[5] = SPEED_BIN_DDR3_1333F;
    speed_bin_list[6] = SPEED_BIN_DDR3_1333G;
    speed_bin_list[7] = SPEED_BIN_DDR3_1333H;
    speed_bin_list[8] = SPEED_BIN_DDR3_1333J;
    speed_bin_list[9] = SPEED_BIN_DDR4_1600J;
    speed_bin_list[10] = SPEED_BIN_DDR4_1600K;
    speed_bin_list[11] = SPEED_BIN_DDR4_1600L;
    // add new bin after this line
    speed_bin_list[12] = SPEED_BIN_DDR3_800D;

    // update params
    memcpy(&para, &speed_bin_list[ddr_params.speed_bin_index], sizeof(ddr_timing_t));
    // set param by ddr width
    if (ddr_params.speed_bin_index < 2) {
        // 800e and 800d
        para.tRRD_L = 10.0;
        para.tFAW   = set_param_by_width(40.0, 50.0, ddr_params);

    } else if (ddr_params.speed_bin_index < 5) {
        // 1066e 1066f 1066g
        para.tRRD_L = set_param_by_width(7.5,   10.0, ddr_params);
        para.tFAW   = set_param_by_width(37.5,  50.0, ddr_params);
    } else if (ddr_params.speed_bin_index < 9) {
        // 1333f 1333g 1333h 1333j
        para.tRRD_L = set_param_by_width(6.0,   7.5,  ddr_params);
        para.tFAW   = set_param_by_width(30.0,  45.0, ddr_params);
    }  else if (ddr_params.speed_bin_index < 12) {
        // 1333f 1333g 1333h 1333j
        para.tRRD_S = set_param_by_width(5.0,   6.0, ddr_params),
        para.tRRD_L = set_param_by_width(6.0,   7.5, ddr_params);
        para.tFAW   = set_param_by_width(25.0, 35.0, ddr_params);
    } else {
        /* doing nothing*/
    }
    // set param by 
    para.tRFC = set_trfc_by_density(ddr_params.dram_density);

    if (ddr_params.type == DDR4_TYPE) {
        para.tCCD_L = MAX(para.tCCD_L, tck * 5);
        para.tRRD_S = MAX(para.tRRD_S, tck * 4);
        para.tRRD_L = MAX(para.tRRD_L, tck * 4);
        para.tWTR_S = MAX(para.tWTR_S, tck * 2);
        para.tWTR_L = MAX(para.tWTR_L, tck * 4);
        para.tRTP   = MAX(para.tRTP, tck * 4);
        para.tFAW   = MAX(para.tFAW, tck * PARA_SEL_BY_WIDTH(20, 28));
    }
    return para;
}
