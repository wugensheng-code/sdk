#include <math.h>
#include <string.h>
#include "dr1x90_ddrc_init.h"

u32 get_mr0_cl_code(ddr_type_t type, u32 ncl)
{
    if (type == DDR3_TYPE) {
        switch (ncl)
        {
        case 5 : return 0b0010;
        case 6 : return 0b0100;
        case 7 : return 0b0110;
        case 8 : return 0b1000;
        case 9 : return 0b1010;
        case 10: return 0b1100;
        case 11: return 0b1110;
        case 12: return 0b0001;
        case 13: return 0b0011;
        case 14: return 0b0101;
        }
    }
    else if (type == DDR4_TYPE) {
        switch (ncl)
        {
        case 9 : return 0b00000;
        case 10: return 0b00001;
        case 11: return 0b00010;
        case 12: return 0b00011;
        case 13: return 0b00100;
        case 14: return 0b00101;
        case 15: return 0b00110;
        case 16: return 0b00111;
        case 18: return 0b01000;
        case 20: return 0b01001;
        case 22: return 0b01010;
        case 24: return 0b01011;
        case 23: return 0b01100;
        case 17: return 0b01101;
        case 19: return 0b01110;
        case 21: return 0b01111;
        case 25: return 0b10000;
        case 26: return 0b10001;
        case 27: return 0b10010;
        case 28: return 0b10011;
        case 29: return 0b10100;
        case 30: return 0b10101;
        case 31: return 0b10110;
        case 32: return 0b10111;
        }
    }
    return (u32)-1;
}

u32 get_mr0_wr_code(ddr_type_t type, u32 nwr)
{
    if (type == DDR3_TYPE) {
        switch (nwr)
        {
        case 16: return 0b000;
        case  5: return 0b001;
        case  6: return 0b010;
        case  7: return 0b011;
        case  8: return 0b100;
        case 10: return 0b101;
        case 12: return 0b110;
        case 14: return 0b111;
        }
    }
    else if (type == DDR4_TYPE) {
        switch (nwr)
        {
        case 10: return 0b0000;
        case 12: return 0b0001;
        case 14: return 0b0010;
        case 16: return 0b0011;
        case 18: return 0b0100;
        case 20: return 0b0101;
        case 24: return 0b0110;
        case 22: return 0b0111;
        case 26: return 0b1000;
        case 28: return 0b1001;
        }
    }
    return (u32)-1;
}

u32 get_mr2_cwl_code(ddr_type_t type, u32 ncwl)
{
    if (type == DDR3_TYPE) {
        switch (ncwl)
        {
        case 5 : return 0b000;
        case 6 : return 0b001;
        case 7 : return 0b010;
        case 8 : return 0b011;
        case 9 : return 0b100;
        case 10: return 0b101;
        }
    }
    else if (type == DDR4_TYPE) {
        switch (ncwl)
        {
        case 9 : return 0b000;
        case 10: return 0b001;
        case 11: return 0b010;
        case 12: return 0b011;
        case 14: return 0b100;
        case 16: return 0b101;
        case 18: return 0b110;
        case 20: return 0b111;
        }
    }
    return (u32)-1;
}

u32 get_nwr(double fck, ddr_type_t type)
{
    // return (type == DDR3_TYPE) ? 12 : 22;

    u32 nWR = (u32)ceil(15.0 / 1e3 * fck);   // Write Recover Time = 15ns For DDR3 & 4
    if (type == DDR3_TYPE) {
        if (nWR < 5)
            nWR = 5;
        else if (nWR <= 8)
            (void) nWR;
        else if (nWR <= 16)
            nWR = (nWR + 1) & (~0x1);
        else
            nWR = 16;
    }
    else if (type == DDR4_TYPE) {
        if (nWR < 10)
            nWR = 10;
        else if (nWR <= 28)
            nWR = (nWR + 1) & (~0x1);
        else
            nWR = 28;
    }
    return nWR;
}

void make_ddr_mr(double fck, ddr_type_t type, const ddr_timing_t* timpara, u32* MR)
{
    u32  cl_code = get_mr0_cl_code(type, timpara->nCL);
    u32 cwl_code = get_mr2_cwl_code(type, timpara->nCWL);
    u32 nWR      = get_nwr(fck, type);
    u32  wr_code = get_mr0_wr_code(type, nWR);
    if (type == DDR3_TYPE) {
        MR[0] = 0x0100 | (wr_code << 9) | ((cl_code & 0b0001) << 2) | ((cl_code & 0b1110) << 3);
        MR[1] = 0x0044;
        MR[2] = 0x0200 | (cwl_code << 3);
        MR[3] = 0x0;
    }
    else if (type == DDR4_TYPE) {
        MR[0] = 0x0100 | (wr_code << 9) | ((cl_code & 0b00001) << 2) | ((cl_code & 0b01110) << (4 - 1)) | ((cl_code & 0b10000) << (12 - 4));
        MR[1] = 0x0001 | (0x0 << 8) | (0x0 << 1);
        MR[2] = 0x0080 | (cwl_code << 3);
        MR[3] = 0x0000;
        MR[4] = 0x0000;
        MR[5] = 0x0000 | (0x3 << 6);    // RTT_park = RZQ / 6
        MR[6] = 0x0419;
    }
}

void dr1x90_release_ddr_bus()
{
    u32 val = 0;
    // DDR Port 0 MPU
    val = dr1x90_dram_read(0xf840e004UL);
    val &= ~0x1;
    dr1x90_dram_write(0xf840e004UL, val);

    // DDR Port 1 MPU
    val = dr1x90_dram_read(0xf840f004UL);
    val &= ~0x1;
    dr1x90_dram_write(0xf840f004UL, val);

    // DDR BUS Reset
    val = dr1x90_dram_read(0xF8801074UL);
    val |= 0x1 << 14;
    dr1x90_dram_write(0xF8801074UL, val);
}
