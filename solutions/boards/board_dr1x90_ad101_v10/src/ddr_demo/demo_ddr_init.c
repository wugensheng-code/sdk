#include <stdio.h>

#include "alc_types.h"
#include "dr1x90_ddrc_init.h"
#include "dr1x90_misc.h"

/*************************************************************/

int ddrc_is_init()
{
    u32 dfi_done = dr1x90_field_read(DDRC_ADDR_UMCTL2 + DFISTAT, dfi_init_complete_offset, dfi_init_complete_mask);
    u32 opr_mode = dr1x90_field_read(DDRC_ADDR_UMCTL2 + STAT, operating_mode_offset, operating_mode_mask);
    // AL_DDR_LOG("DFI_Done == 0x%x, OP_Mode == 0x%x\r\n", dfi_done, opr_mode);
    return dfi_done == 0x1 && opr_mode != 0x0; // Normal or Power Down or Power Saving Mode
}

int demo_ddr_init()
{
    double f_ck = 1600.0 / 3;    // 533.3 MHz
    // double f_ck = 2000.0 / 3;    // 666.6 MHz
    ddr_type_t  type  = DDR3_TYPE;
    ddr_width_t width = DDR_X32;          // DDR_X32
    ddr_ecc_t   ecc   = DDR_ECC_NONE;     // DDR_ECC_NONE
    ddr_timing_t timpara = {
        .nCL  = 9,   // 9, 10
        .nCWL = 7,   // 7, 9
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
    /*
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
    */
    ddr_arbiter_t arbiter_cfg = {
        // TODO
    };

    int err = dr1x90_ddrc_init(f_ck, type, width, ecc, &timpara, &addrmap, &arbiter_cfg);

    return err;
}
