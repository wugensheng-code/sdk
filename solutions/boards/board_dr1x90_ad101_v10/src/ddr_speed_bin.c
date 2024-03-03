#include <string.h>
#include "fd_ddr_init.h"

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define PARA_SEL_BY_WIDTH(para_x8, para_x16) ((FD_PARA_DRAM_WIDTH == DDR_COMP_WIDTH_X8) ? (para_x8) : (para_x16))

#define TRFC_SEL_BY_DENSITY (   \
    FD_PARA_DRAM_DENSITY <=  512 ?  90.0 : \
    FD_PARA_DRAM_DENSITY <= 1024 ? 110.0 : \
    FD_PARA_DRAM_DENSITY <= 2048 ? 160.0 : \
    FD_PARA_DRAM_DENSITY <= 4096 ? 260.0 : \
    FD_PARA_DRAM_DENSITY <= 8196 ? 350.0 : \
    550.0   \
)

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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
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
    .tRFC   = TRFC_SEL_BY_DENSITY,
    .tREFI  = TREFI
};

ddr_timing_t get_timing_para(double fck, ddr_type_t type)
{
    double tck = 1e3 / fck;
    ddr_timing_t para;

    memcpy(&para, &FD_PARA_SPEED_BIN, sizeof(ddr_timing_t));
    // memcpy(&para, &SPEED_BIN_DDR3_1333H, sizeof(ddr_timing_t));

    if (type == DDR4_TYPE) {
        para.tCCD_L = MAX(para.tCCD_L, tck * 5);
        para.tRRD_S = MAX(para.tRRD_S, tck * 4);
        para.tRRD_L = MAX(para.tRRD_L, tck * 4);
        para.tWTR_S = MAX(para.tWTR_S, tck * 2);
        para.tWTR_L = MAX(para.tWTR_L, tck * 4);
        para.tRTP = MAX(para.tRTP, tck * 4);
        para.tFAW = MAX(para.tFAW, tck * PARA_SEL_BY_WIDTH(20, 28));
    }
    return para;
}
