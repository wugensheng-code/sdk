#include <stdio.h>
#include <string.h>
#include "dr1x90_ddrc_init.h"

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

#define DDR_TYPE  DDR3_TYPE
#define DDR_WIDTH DDR_X32
#define DDR_ECC   DDR_ECC_NONE

int demo_ddr_init()
{
    const double f_ck = 1600.0 / 3;    // 533.3 MHz
    // const double f_ck = 2000.0 / 3;    // 666.6 MHz
    // const double f_ck = 800.0;         // 800.0 MHz

    #if DDR_TYPE == DDR3_TYPE
    // For Demo Board DDR3
    ddr_timing_t timpara = {
        .nCL  = 9,  // 9
        .nCWL = 7,  // 7
        .nRCD = 9,
        .nRP  = 9,
        .tRAS = 36.0,
        .tRC  = 49.5,
        .tCCD_L = 0.0,
        .tRRD_S = 10.0,
        .tRRD_L = 10.0,
        .tFAW  = 50.0,
        .tRFC  = 260.0,
        .tREFI = 7.8 * 1e3
    };
    #if DDR_WIDTH == DDR_X32
    // For DDR3 X32
    ddr_addrmap_t addrmap = {
        .ba = {
        //  ba0 ba1 ba2
            6,  7,  8
        },
        .bg = {
        //  bg0 bg1
            -1, -1
        },
        .col = {
        //  c0  c1  c2  c3  c4  c5  c6  c7  c8  c9
            -1, -1,  4,  5,  9, 10, 11, 12, 13, 14
        },
        .row = {
        //  r0  r1  r2  r3  r4  r5  r6  r7  r8  r9
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        //  r10 r11 r12 r13 r14 r15 r16 r17
            25, 26, 27, 28, 29, -1, -1, -1
        }
    };
    #elif DDR_WIDTH == DDR_X16
    // For DDR3 X16
    ddr_addrmap_t addrmap = {
        .ba = {
        //  ba0 ba1 ba2
            6,  7,  8
        },
        .bg = {
        //  bg0 bg1
            -1, -1
        },
        .col = {
        //  c0  c1  c2  c3  c4  c5  c6  c7  c8  c9
            -1, -1, -1,  4,  5,  9, 10, 11, 12, 13,
        },
        .row = {
        //  r0  r1  r2  r3  r4  r5  r6  r7  r8  r9
            14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
        //  r10 r11 r12 r13 r14 r15 r16 r17
            24, 25, 26, 27, 28, -1, -1, -1
        }
    };
    #endif
    #elif DDR_TYPE == DDR4_TYPE
    const double t_ck = 1e3 / f_ck;
    // For DC Borad DDR4
    ddr_timing_t timpara = {
        .nCL  = 10,
        .nCWL = 9,
        .nRCD = 10,
        .nRP  = 10,
        .tRAS = 35.0,
        .tRC  = 35.0 + 10 * t_ck,
        .tCCD_L = max(4 * t_ck, 6.25),
        .tRRD_S = max(4 * t_ck, 6.0),
        .tRRD_L = max(4 * t_ck, 7.5),
        .tFAW  = max(28 * t_ck, 35.0),
        .tRFC  = 350.0,
        .tREFI = 7.8 * 1e3
    };
    // For DDR4 X32
    ddr_addrmap_t addrmap = {
        .ba = {
        //  ba0 ba1 ba2
            7,  8,  -1
        },
        .bg = {
        //  bg0 bg1
            6, -1
        },
        .col = {
        //  c0  c1  c2  c3  c4  c5  c6  c7  c8  c9
            -1, -1,  4,  5,  9, 10, 11, 12, 13, 14
        },
        .row = {
        //  r0  r1  r2  r3  r4  r5  r6  r7  r8  r9
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        //  r10 r11 r12 r13 r14 r15 r16 r17
            25, 26, 27, 28, 29, -1, -1, -1
        }
    };
    #endif

    ddr_arbiter_t arbiter_cfg = {
        // TODO
    };

    int err = dr1x90_ddrc_init(f_ck, DDR_TYPE, DDR_WIDTH, DDR_ECC, &timpara, &addrmap, &arbiter_cfg);
    
    return err;
}
