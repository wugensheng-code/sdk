#include <string.h>
#include "dr1x90_ddrc_func.h"
#include "dr1x90_ddrppc_dcu.h"

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

u64 dcu_cmd_make(u64 data, u64 addr, u64 bank, u64 cmd, u64 dtp, u64 rpt)
{
    return 
        (data << SCHD_REQ_DATA_OFFSET) | 
        ( 0x0 << SCHD_REQ_MASK_OFFSET) | 
        (addr << SCHD_REQ_ADDR_OFFSET) | 
        (bank << SCHD_REQ_BANK_OFFSET) | 
        ( 0x0 << SCHD_REQ_RANK_OFFSET) | 
        ( cmd << SCHD_REQ_CMD_OFFSET ) | 
        ( 0x0 << SCHD_REQ_TAG_OFFSET ) | 
        ( dtp << SCHD_REQ_DTP_OFFSET ) | 
        ( rpt << SCHD_REQ_RPT_OFFSET ) ;
}

static u32 lsfr_seed = 0x1234ABCD;

u32 pub_lsfr_set(u32 seed)
{
    lsfr_seed = seed;
    dr1x90_reg_write(DDRC_ADDR_PPC + BISTLSR, seed);
    return seed;
}

u32 pub_lsfr()
{
    const u32 lsfr_poly = 0x80000CEC;
    u32 val = lsfr_seed;
    u32 fbk = lsfr_seed & 0x1;
    for (int i = 1; i < 32; ++i) {
        fbk ^= (lsfr_seed & lsfr_poly & (0x1 << i)) ? 0x1 : 0x0;
    }
    lsfr_seed >>= 1;
    lsfr_seed |= fbk << 31;
    return val;
}

void pub_data_set(u32 pattern, u32 data)
{
    // TODO
}

void pub_data_get(u32 pattern, u32* buf)
{
    const u32 tab[] = {
        0x00000000, 0xFFFFFFFF, 0x55555555, 0xAAAAAAAA, 0x00005500, 0x55550055, 0x0000AA00, 0xAAAA00AA,
        0x20021001, 0x80084004, 0x02200110, 0x08800440, 0xdffdeffe, 0x7ff7bffb, 0xfddffeef, 0xf77ffbbf,
        0x1234ABCD, 0xFF00FF00, 0xFFFF0000, 0x0000FF00, 0xFFFF00FF, 0x00FF00FF, 0xF0F0F0F0, 0x0F0F0F0F
    };
    u32 data = tab[pattern];
    switch (pattern)
    {
    case PUB_DATA_DTDR0:
        data = dr1x90_reg_read(DDRC_ADDR_PPC + DTDR0);
        break;
    case PUB_DATA_DTDR1:
        data = dr1x90_reg_read(DDRC_ADDR_PPC + DTDR1);
        break;
    case PUB_DATA_UDDR0:
        data = dr1x90_reg_read(DDRC_ADDR_PPC + UDDR0);
        break;
    case PUB_DATA_UDDR1:
        data = dr1x90_reg_read(DDRC_ADDR_PPC + UDDR1);
        break;
    case PUB_DATA_USER_PATTERN:
        data = dr1x90_reg_read(DDRC_ADDR_PPC + BISTUDPR);
        break;
    case PUB_DATA_SCHCR0:
        // data = dr1x90_field_read(DDRC_ADDR_PPC + SCHCR0, SCHDQV_offset, SCHDQV_mask);
        data = 0;
        break;
    }
    for (int i = 0; i < 4; ++i) {
        switch (pattern)
        {
        case PUB_DATA_WALKING_1:
            buf[i] = tab[ 8 + i];
            break;
        case PUB_DATA_WALKING_0:
            buf[i] = tab[12 + i];
            break;
        case PUB_DATA_USER_PATTERN:
            buf[i] = 0;
            break;
        case PUB_DATA_LFSR:
            buf[i] = pub_lsfr();
            break;
        default:
            buf[i] = data;
            break;
        }
    }
    if (pattern == PUB_DATA_USER_PATTERN) {
        u8* buf8 = buf;
        for (int i = 0; i < 16; ++i) {
            buf8[i] |= (data & (0x1 << (i + 0 ))) ? 0x55 : 0x0;
            buf8[i] |= (data & (0x1 << (i + 16))) ? 0xAA : 0x0;
        }
    }
}

int dcu_cmd_load(u64* cmd, int n)
{
    u32 dcuar = (0x0 << CSEL_offset) | (0x0 << INCA_offset) | (0x0 << ATYPE_offset);
    for (int i = 0; i < n; ++i) {
        dcuar = dr1x90_field_set(DDRC_ADDR_PPC + DCUAR, CWADDR_W_offset, CWADDR_W_mask, (u32)i, dcuar);
        dcuar = dr1x90_field_set(DDRC_ADDR_PPC + DCUAR, CSADDR_W_offset, CSADDR_W_mask, 0x0, dcuar);
        dr1x90_reg_write(DDRC_ADDR_PPC + DCUAR, dcuar);
        dr1x90_reg_write(DDRC_ADDR_PPC + DCUDR, (u32)(cmd[i] >> 0));

        dcuar = dr1x90_field_set(DDRC_ADDR_PPC + DCUAR, CSADDR_W_offset, CSADDR_W_mask, 0x1, dcuar);
        dr1x90_reg_write(DDRC_ADDR_PPC + DCUAR, dcuar);
        dr1x90_reg_write(DDRC_ADDR_PPC + DCUDR, (u32)(cmd[i] >> 32));
    }
    return n;
}

int dcu_cmd_exec(int n, u32* data, u32 byte)
{
    u32 dcurr = (DINST_RESET << DINST_offset) | (0 << SADDR_offset) |
                ((n - 1) << EADDR_offset);
    if (data) {
        dcurr |= 0x1 << RCEN_offset;
        dr1x90_field_write(DDRC_ADDR_PPC + BISTRR, BDXSEL_offset, BDXSEL_mask, byte);
    }
    dr1x90_reg_write(DDRC_ADDR_PPC + DCURR, dcurr);
    dcurr = dr1x90_field_set(DDRC_ADDR_PPC + DCURR, DINST_offset, DINST_mask, DINST_RUN, dcurr);
    dr1x90_reg_write(DDRC_ADDR_PPC + DCURR, dcurr);
    dr1x90_field_wait(DDRC_ADDR_PPC + DCUSR0, RDONE_offset, RDONE_mask, 0x1, -1);

    u32 recv = 0;
    if (data) {
        recv = dr1x90_reg_read(DDRC_ADDR_PPC + DCUSR1);
        recv &= RDCNT_mask;
        u32 dcuar = (0x2 << CSEL_offset) | (0x1 << INCA_offset) | (0x1 << ATYPE_offset) | (0x0 << CWADDR_R_offset) | (0x0 << CSADDR_R_offset);
        dr1x90_reg_write(DDRC_ADDR_PPC + DCUAR, dcuar);
        for (u32 i = 0; i < 4; ++i)
            data[i] = dr1x90_reg_read(DDRC_ADDR_PPC + DCUDR);
        dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 0);
        dr1x90_field_write(DDRC_ADDR_PPC + PGCR0, PHYFRST_offset, PHYFRST_mask, 1);
    }

    dr1x90_reg_write(DDRC_ADDR_PPC + DCURR, DINST_RESET);
    return recv;
}

int dcu_gate_edge_dect(int rept, int byte, u32* cnt)
{
    // cnt[0] :  low level
    // cnt[1] : fail edge
    // cnt[2] : rise edge
    // cnt[3] : high level
    memset(cnt, 0x0, 4 * sizeof(u32));

    // DX3 -> T0
    // DX2 -> T1
    // DX1 -> T2
    // DX0 -> T3
    u32 base = DDRC_ADDR_BK1_T3 - 0x1000 * byte;

    for (int r = 0; r < rept; ++r) {
        // Issue read command
        dcu_cmd_exec(1, NULL, 0);

        u32 idx = dr1x90_field_read(base + DQSGRP_QS_GATE_SR, QS_GATE_SR_STATUS_offset, QS_GATE_SR_STATUS_mask);
        ++cnt[idx];
    }
    return 0;
}

int dcu_gate_train(u32 lane_mask, const u32* MR, ddr_type_t type)
{
    u64 dcu_cmd[3];
    u32 prd[4];
    u32 cnt[4];
    short status[DCU_GATE_STSLEN];
    short correl[DCU_GATE_STSLEN];

    for (int dx = 0; dx < 4; ++dx) {
        prd[dx] = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0  + 0x100 * dx, GDQSPRD_offset, GDQSPRD_mask);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR5 + 0x100 * dx, DQSGSD_offset, DQSGSD_mask, prd[dx] / 8);
    }
    AL_DDR_LOG("[DCU GATE] GDQSPRD = %d %d %d %d\r\n", prd[0], prd[1], prd[2], prd[3]);

    int cmd_len = 2;
    // Enter MPR Mode
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000,         0x0, 0x0, DCU_CMD_PREC_ALL, DCU_DTP_PPA, DCU_PRT_1);
    dcu_cmd[1] = dcu_cmd_make(PUB_DATA_0000_0000, MR[3] | 0x4, 0x3, DCU_CMD_MRS     , DCU_DTP_MOD, DCU_PRT_1);
    if (type == DDR4_TYPE) {
        dcu_cmd[2] = dcu_cmd_make(PUB_DATA_0000_0000, MR[4] | 0x400, 0x4, DCU_CMD_MRS, DCU_DTP_MOD, DCU_PRT_1);
        cmd_len = 3;
    }
    dcu_cmd_load(dcu_cmd, cmd_len);
    dcu_cmd_exec(cmd_len, NULL, 0);

    // setup READ command
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, 0x0, 0x0, DCU_CMD_READ, DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd_load(dcu_cmd, 1);

    // for (int dx = 0; dx < 4; ++dx) {
    //     u32 step = prd[dx] / 4;
    //     u32  dgsl = 0;
    //     u32 dqsgd = 0;
    //     u32   dly = 0;
    //     for (dly = 0; dly <= prd[dx] * 16; dly+=step) {
    //         dgsl  = dly / prd[dx];
    //         dqsgd = dly % prd[dx];
    //         dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,  DGSL_mask , dgsl);
    //         dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset, DQSGD_mask, dqsgd + 1);

    //         dcu_gate_edge_dect(DCU_GATE_SAMPLE, dx, cnt);
    //         AL_DDR_LOG("[DCU GATE] DX%d DGSL %2d DQSGD %3d: %2d %2d %2d %2d\r\n", dx, dgsl, dqsgd, cnt[0], cnt[1], cnt[2], cnt[3]);
    //     }
    //     AL_DDR_LOG("[DCU GATE] ============================================\r\n");
    // }

    for (int dx = 0; dx < 4; ++dx) {
        u32 lane_en = lane_mask & (0x1 << dx);
        if (lane_en == 0)
            continue;
        
        // u32 step = prd[dx] / DCU_GATE_DIFFDIV;
        u32 dgsl  = 0;
        u32 dqsgd = 0;
        u32   dly = 0;
        u32 l_dly = 0;
        u32 r_dly = 0;

        int sts_len = 0;

        while (dgsl < MAX_DGSL && sts_len < DCU_GATE_STSLEN) {
            dly   = sts_len * prd[dx] / DCU_GATE_DIFFDIV;
            dgsl  = dly / prd[dx];
            dqsgd = dly % prd[dx];
            dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,  DGSL_mask , dgsl);
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset, DQSGD_mask, dqsgd + 1);

            dcu_gate_edge_dect(DCU_GATE_SAMPLE, dx, cnt);
            // AL_DDR_LOG("[DCU GATE] DX%d DGSL %2d DQSGD %3d: %2d %2d %2d %2d\r\n", dx, dgsl, dqsgd, cnt[0], cnt[1], cnt[2], cnt[3]);
            status[sts_len] = (short)(cnt[1] + cnt[3]) - (short)(cnt[0] + cnt[2]);
            ++sts_len;
        }

        for (int i = 0; i < sts_len; ++i) {
            correl[i] = 0;
            int idx = i;
            for (int j = 0; j < 9; ++j) {
                for (int k = 0; k < DCU_GATE_DIFFDIV; ++k) { 
                    short x = idx < sts_len ? status[idx] : 0;
                    short y = j % 2 ? 1 : -1;
                    correl[i] += x * y;
                    ++idx;
                }
            }
        }

        short max_correl = -32767;
        int idx_max = 0;
        for (int i = 0; i < sts_len; ++i) {
            if (max_correl < correl[i]) {
                max_correl = correl[i];
                idx_max = i;
            }
        }

        l_dly = idx_max * prd[dx] / DCU_GATE_DIFFDIV + prd[dx] / 2;
        r_dly = idx_max * prd[dx] / DCU_GATE_DIFFDIV + prd[dx] / 2 + prd[dx];

        // for (int i = 0; i < sts_len; ++i) {
        //     dly   = i * step;
        //     dgsl  = dly / prd[dx];
        //     dqsgd = dly % prd[dx];
        //     AL_DDR_LOG("[DCU GATE] DX%d %3d# DLY=%3d DGSL=%2d DQSGD=%3d: %3d %d\r\n", dx, i, dly, dgsl, dqsgd, status[i], correl[i]);
        // }
        // AL_DDR_LOG("[DCU GATE] =============================\r\n");
        AL_DDR_LOG("[DCU GATE] DX%d MAX @ %d# DLY=%3d: %d\r\n", dx, idx_max, idx_max * prd[dx] / DCU_GATE_DIFFDIV, max_correl);

        // Find Last Low Level
        for (dly = l_dly; dly < r_dly; ++dly) {
            dgsl  = dly / prd[dx];
            dqsgd = dly % prd[dx];
            dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,  DGSL_mask , dgsl);
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset, DQSGD_mask, dqsgd + 1);

            dcu_gate_edge_dect(DCU_GATE_SAMPLE, dx, cnt);

            if (cnt[0] == DCU_GATE_SAMPLE) {
                l_dly = dly;
            }
        }
        // Find First High Level
        for (dly = r_dly; dly > l_dly; --dly) {
            dgsl  = dly / prd[dx];
            dqsgd = dly % prd[dx];
            dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,  DGSL_mask , dgsl);
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset, DQSGD_mask, dqsgd + 1);

            dcu_gate_edge_dect(DCU_GATE_SAMPLE, dx, cnt);

            if (cnt[3] == DCU_GATE_SAMPLE) {
                r_dly = dly;
            }
        }
        AL_DDR_LOG("[DCU GATE] DX%d L_DLY:%d R_DLY:%d\r\n", dx, l_dly, r_dly);

        dly = (l_dly + r_dly) / 2 - prd[dx] / DCU_GATE_SHIFT;
        dgsl  = dly / prd[dx];
        dqsgd = dly % prd[dx];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,  DGSL_mask , dgsl);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset, DQSGD_mask, dqsgd + 1);
        AL_DDR_LOG("[DCU GATE] DX%d DGSL:%d DQSGD:%d\r\n", dx, dgsl, dqsgd);

        // if (dgsl != 3 && dgsl != 4)
        // {
        //     for (int i = 0; i < sts_len; ++i) {
        //         dly   =   i * prd[dx] / DCU_GATE_DIFFDIV;
        //         dgsl  = dly / prd[dx];
        //         dqsgd = dly % prd[dx];
        //         AL_DDR_LOG("[GATE BUG] DX%d %3d# DLY=%3d DGSL=%2d DQSGD=%3d: %3d %d\r\n", dx, i, dly, dgsl, dqsgd, status[i], correl[i]);
        //     }
        //     AL_DDR_LOG("[GATE BUG] =============================\r\n");
        // }
    }

    cmd_len = 1;
    // Exit MPR Mode
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, MR[3], 0x3, DCU_CMD_MRS, DCU_DTP_MOD, DCU_PRT_1);
    if (type == DDR4_TYPE) {
        dcu_cmd[1] = dcu_cmd_make(PUB_DATA_0000_0000, MR[4], 0x4, DCU_CMD_MRS, DCU_DTP_MOD, DCU_PRT_1);
        cmd_len = 2;
    }
    dcu_cmd_load(dcu_cmd, cmd_len);
    dcu_cmd_exec(cmd_len, NULL, 0);

    return 0;
}

int dcu_mtest_log(u32 dx_mask, u32 bank, u32 row, u32 col, int rpt)
{
    int err = 0;
    
    u64 dcu_cmd[6];
    u32 dcu_data[4];
    u32 dcu_expc[4];
    int recv = 0;

    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, row    , bank, DCU_CMD_ACTIVATE, DCU_DTP_ACT2RW, DCU_PRT_1);
    dcu_cmd[1] = 0;
    dcu_cmd[2] = 0;
    dcu_cmd[3] = dcu_cmd_make(PUB_DATA_0000_0000, col + 0, bank, DCU_CMD_READ    , DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd[4] = dcu_cmd_make(PUB_DATA_0000_0000, col + 8, bank, DCU_CMD_READ    , DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd[5] = dcu_cmd_make(PUB_DATA_0000_0000, row    , bank, DCU_CMD_PREC    , DCU_DTP_RP    , DCU_PRT_1);

    for (int dx = 0; dx < 4; ++dx) {
        for (int idx = 0; idx < 24; ++idx) {
            dcu_cmd[1] = dcu_cmd_make(idx, col + 0, bank, DCU_CMD_WRITE, DCU_DTP_WR2RD, DCU_PRT_BL);
            dcu_cmd[2] = dcu_cmd_make(idx, col + 8, bank, DCU_CMD_WRITE, DCU_DTP_WR2RD, DCU_PRT_BL);
            dcu_cmd_load(dcu_cmd, 6);
            recv = dcu_cmd_exec(6, dcu_data, dx);

            pub_data_get(idx, dcu_expc);

            if (recv != 4 || 
                dcu_data[0] != dcu_expc[0] || 
                dcu_data[1] != dcu_expc[1] || 
                dcu_data[2] != dcu_expc[2] || 
                dcu_data[3] != dcu_expc[3]) {
                AL_DDR_LOG("[DCU MTEST] DX%d %2d# Recv %d: 0x%08x 0x%08x 0x%08x 0x%08x == 0x%08x 0x%08x 0x%08x 0x%08x\r\n", dx, idx, recv,
                    dcu_data[0], dcu_data[1], dcu_data[2], dcu_data[3], dcu_expc[0], dcu_expc[1], dcu_expc[2], dcu_expc[3]
                );
                ++err;
            }
            // AL_DDR_LOG("[DCU MTEST] DX%d %2d# Recv %d: 0x%08x 0x%08x 0x%08x 0x%08x == 0x%08x 0x%08x 0x%08x 0x%08x\r\n", dx, idx, recv,
                // dcu_data[0], dcu_data[1], dcu_data[2], dcu_data[3], dcu_expc[0], dcu_expc[1], dcu_expc[2], dcu_expc[3]
            // );
        }
    }
    if (err == 0)
        AL_DDR_LOG("[DCU MTEST] PASS\r\n");
    else
        AL_DDR_LOG("[DCU MTEST] FAIL: %d in %d\r\n", err, 4 * 24);
    return err;
}

u32 sum_tab(const u16* tab, int n, int step)
{
    u32 s = 0;
    for (int i = 0; i < n; ++i)
        s += (u32)tab[i * step];
    return s;
}

void sort_u32(u32* data, int n)
{
    int j = 0;
    u32 t = 0;
    for (int i = 1; i < n; ++i) {
        t = data[i];
        for (j = i - 1; j >= 0 && t < data[j]; --j) {
            data[j + 1] = data[j];
        }
        data[j + 1] = t;
    }
}

// data: u32[4] or u8[16]
int dcu_data_error(const u8* data, const u8* gold, u16* bit_err)
{
    int err = 0;

    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 8; ++j) {
            u8 mask = 0x1 << j;
            if ((data[i] & mask) != (gold[i] & mask)) {
                ++err;
                ++bit_err[j];
            }
        }
    }

    return err;
}

// data: u32[4] or u8[16]
int dcu_data_error_edge(const u8* data, const u8* gold, u16* rise_err, u16* fall_err)
{
    int err = 0;

    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 8; ++j) {
            u8 mask = 0x1 << j;
            if ((data[i] & mask) != (gold[i] & mask)) {
                ++err;
                if (i % 2 == 0)
                    ++rise_err[j];
                else
                    ++fall_err[j];
            }
        }
    }

    return err;
}

int dcu_mtest_weye(u32 dx, int rpt, u16* bit_err)
{
    int err = 0;
    
    u64 dcu_cmd[6];
    u32 dcu_data[4];
    u32 dcu_expc[4];

    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, 0, 0, DCU_CMD_ACTIVATE, DCU_DTP_ACT2RW, DCU_PRT_1);
    dcu_cmd[1] = 0;
    dcu_cmd[2] = 0;
    dcu_cmd[3] = dcu_cmd_make(PUB_DATA_0000_0000, 0, 0, DCU_CMD_READ    , DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd[4] = dcu_cmd_make(PUB_DATA_0000_0000, 8, 0, DCU_CMD_READ    , DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd[5] = dcu_cmd_make(PUB_DATA_0000_0000, 0, 0, DCU_CMD_PREC    , DCU_DTP_RP    , DCU_PRT_1);

    for (int i = 0; i < rpt; ++i) {
        for (int idx = 8; idx < 16; ++idx) {
            dcu_cmd[1] = dcu_cmd_make(idx, 0, 0, DCU_CMD_WRITE, DCU_DTP_WR2RD, DCU_PRT_BL);
            dcu_cmd[2] = dcu_cmd_make(idx, 8, 0, DCU_CMD_WRITE, DCU_DTP_WR2RD, DCU_PRT_BL);
            dcu_cmd_load(dcu_cmd, 6);

            int recv = 0;
            for (int retry = 0; retry < 2 && recv != 4; ++retry) {
                recv = dcu_cmd_exec(6, dcu_data, dx);
                pub_data_get(idx, dcu_expc);
            }

            err += dcu_data_error(dcu_data, dcu_expc, bit_err);
        }
    }

    return err;
}

int dcu_mtest_reye(u32 dx, int rpt, int rd_rpt, u16* rise_err, u16* fall_err)
{
    int err = 0;
    
    u64 dcu_cmd[4];
    u32 dcu_data[4];
    u32 dcu_expc[4];
    int recv = 0;

    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, 0, 0, DCU_CMD_ACTIVATE, DCU_DTP_ACT2RW, DCU_PRT_1);
    dcu_cmd[1] = 0;
    dcu_cmd[2] = 0;
    dcu_cmd[3] = dcu_cmd_make(PUB_DATA_0000_0000, 0, 0, DCU_CMD_PREC    , DCU_DTP_RP    , DCU_PRT_1);

    for (int i = 0; i < rpt; ++i) {
        for (int idx = 8; idx < 16; ++idx) {
            dcu_cmd[1] = dcu_cmd_make(idx, 0, 0, DCU_CMD_WRITE, DCU_DTP_WR2RD, DCU_PRT_BL);
            dcu_cmd[2] = dcu_cmd_make(idx, 8, 0, DCU_CMD_WRITE, DCU_DTP_WR2RD, DCU_PRT_BL);
            dcu_cmd_load(dcu_cmd, 4);
            dcu_cmd_exec(4, NULL, dx);
            pub_data_get(idx, dcu_expc);

            dcu_cmd[1] = dcu_cmd_make(PUB_DATA_0000_0000, 0, 0, DCU_CMD_READ, DCU_DTP_RD2PRE, DCU_PRT_BL);
            dcu_cmd[2] = dcu_cmd_make(PUB_DATA_0000_0000, 8, 0, DCU_CMD_READ, DCU_DTP_RD2PRE, DCU_PRT_BL);
            dcu_cmd_load(dcu_cmd, 4);

            for (int i = 0; i < rd_rpt; ++i) {
                int recv = 0;
                for (int retry = 0; retry < 2 && recv != 4; ++retry) {
                    recv = dcu_cmd_exec(4, dcu_data, dx);
                }
                err += dcu_data_error_edge(dcu_data, dcu_expc, rise_err, fall_err);
            }
        }
    }
    
    return err;
}

int dcu_reye_mpr_train(u32 lane_mask, u32 mr3)
{
    const u32 golden[4] = {0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00};

    u32 gtprd[4];
    u32 gtdly[4];

    u64 dcu_cmd[2];
    u32 dcu_data[4];
    int recv = 0;

    int err = 0;
    u16 err_tab[DCU_EYE_TABSIZE][8];

    u32 eye_l[8];
    u32 eye_r[8];
    u32 eye_m[8];

    for (int dx = 0; dx < 4; ++dx) {
        u32  dgsl = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset   , DGSL_mask   );
        u32 dqsgd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset  , DQSGD_mask  );
        u32 rdqsd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset  , RDQSD_mask  );
        gtprd[dx] = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0   + 0x100 * dx, GDQSPRD_offset, GDQSPRD_mask);
        gtdly[dx] = dgsl * gtprd[dx] + dqsgd - rdqsd;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR3 + 0x100 * dx, 0);
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR4 + 0x100 * dx, 0);
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR5 + 0x100 * dx, 0);
    }

    // Enter MPR Mode
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000,       0x0, 0x0, DCU_CMD_PREC_ALL, DCU_DTP_PPA, DCU_PRT_1);
    dcu_cmd[1] = dcu_cmd_make(PUB_DATA_0000_0000, mr3 | 0x4, 0x3, DCU_CMD_MRS     , DCU_DTP_MOD, DCU_PRT_1);
    dcu_cmd_load(dcu_cmd, 2);
    dcu_cmd_exec(2, NULL, 0);

    // setup READ command
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, 0x0, 0x0, DCU_CMD_READ, DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd[1] = dcu_cmd_make(PUB_DATA_0000_0000, 0x8, 0x0, DCU_CMD_READ, DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd_load(dcu_cmd, 2);
/*
    for (int dx = 0; dx < 1; ++dx) {
        u32 rdqsd = 0;
        for (rdqsd = 0; rdqsd <= gtprd[dx]; rdqsd += 4) {
            u32 dgsl  = (gtdly[dx] + rdqsd) / gtprd[dx];
            u32 dqsgd = (gtdly[dx] + rdqsd) % gtprd[dx];
            dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  rdqsd);
            dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, rdqsd);

            recv = dcu_cmd_exec(1, dcu_data, dx);

            AL_DDR_LOG("[DCU REYE] DX%d DGSL:%d DQSGD:%3d RDQSD:%3d Recv:%d 0x%08x 0x%08x\r\n", dx, dgsl, dqsgd, rdqsd, recv, dcu_data[0], dcu_data[1]);
        }
    }
*/
    for (int dx = 0; dx < 4; ++dx) {
        u32 lane_en = lane_mask & (0x1 << dx);
        if (lane_en == 0)
            continue;
        
        memset(eye_l, 0, sizeof(eye_l));
        memset(eye_r, 0, sizeof(eye_r));
        memset(err_tab, 0, sizeof(err_tab));

        u32 max_rdqsd = gtprd[dx] + gtprd[dx] / 5;
        max_rdqsd = min(DCU_EYE_TABSIZE, max_rdqsd);

        for (int i = 0; i < DCU_REYE_MPR_SCAN; ++i) {
            for (u32 rdqsd = 0; rdqsd < max_rdqsd; rdqsd += 1) {
                u32 dgsl  = (gtdly[dx] + rdqsd) / gtprd[dx];
                u32 dqsgd = (gtdly[dx] + rdqsd) % gtprd[dx];
                dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  rdqsd + 1);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, rdqsd + 1);

                for (int rpt = 0; rpt < DCU_REYE_MPR_SAMPLE; ++rpt) {
                    recv = dcu_cmd_exec(2, dcu_data, dx);
                    err += dcu_data_error(dcu_data, golden, err_tab[rdqsd]);
                }
            }
        }

        // AL_DDR_LOG("[REYE DX%d] DLY DQ0 DQ1 DQ2 DQ3 DQ4 DQ5 DQ6 DQ7\r\n", dx);
        // for (int i = 0; i < max_rdqsd; ++i) {
        //     AL_DDR_LOG(
        //            "           %3d %3d %3d %3d %3d %3d %3d %3d %3d\r\n", i,
        //            err_tab[i][0], err_tab[i][1], err_tab[i][2], err_tab[i][3], 
        //            err_tab[i][4], err_tab[i][5], err_tab[i][6], err_tab[i][7]
        //     );
        // }

        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < max_rdqsd - DCU_REYE_MPR_WINDOW; ++i) {
                if (err_tab[i][j] == 0 && 
                    sum_tab(&err_tab[i + 1][j], DCU_REYE_MPR_WINDOW, 8) < DCU_REYE_MPR_THRD
                ) {
                    eye_l[j] = i;
                    break;
                }
            }
            for (int i = max_rdqsd; i > DCU_REYE_MPR_WINDOW; --i) {
                if (err_tab[i][j] == 0 && 
                    sum_tab(&err_tab[i - DCU_REYE_MPR_WINDOW][j], DCU_REYE_MPR_WINDOW, 8) < DCU_REYE_MPR_THRD
                ) {
                    eye_r[j] = i;
                    break;
                }
            }
            eye_m[j] = (eye_l[j] + eye_r[j]) / 2;
        }
        AL_DDR_LOG("[REYE MPR] DX BIT L_DLY M_DLY R_DLY WIDTH\r\n", dx);
        for (int i = 0; i < 8; ++i) {
            AL_DDR_LOG("[REYE MPR] %2d %2d %5d %5d %5d %5d\r\n", dx, i, eye_l[i], eye_m[i], eye_r[i], eye_r[i] - eye_l[i]);
        }
/*
        u32 cfg_eye = 0;
        for (int j = 0; j < 8; ++j) {
            cfg_eye = max(cfg_eye, eye_m[j]);
        }
        u32 dgsl  = (gtdly[dx] + cfg_eye) / gtprd[dx];
        u32 dqsgd = (gtdly[dx] + cfg_eye) % gtprd[dx];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  cfg_eye + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, cfg_eye + 1);

        u32 bdlr = 0;
        bdlr |= min((cfg_eye - eye_m[7]), 63) << DQ0RBD_offset;
        bdlr |= min((cfg_eye - eye_m[6]), 63) << DQ1RBD_offset;
        bdlr |= min((cfg_eye - eye_m[5]), 63) << DQ2RBD_offset;
        bdlr |= min((cfg_eye - eye_m[4]), 63) << DQ3RBD_offset;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR3 + 0x100 * dx, bdlr);
        bdlr = 0;
        bdlr |= min((cfg_eye - eye_m[3]), 63) << DQ4RBD_offset;
        bdlr |= min((cfg_eye - eye_m[2]), 63) << DQ5RBD_offset;
        bdlr |= min((cfg_eye - eye_m[1]), 63) << DQ6RBD_offset;
        bdlr |= min((cfg_eye - eye_m[0]), 63) << DQ7RBD_offset;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR4 + 0x100 * dx, bdlr);
*/
        sort_u32(eye_m, 8);
        u32 rdqsd = (eye_m[3] + eye_m[4]) / 2;
        u32 dgsl  = (gtdly[dx] + rdqsd) / gtprd[dx];
        u32 dqsgd = (gtdly[dx] + rdqsd) % gtprd[dx];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  rdqsd + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, rdqsd + 1);
    }

    // Exit MPR Mode
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, mr3, 0x3, DCU_CMD_MRS, DCU_DTP_MOD, DCU_PRT_1);
    dcu_cmd_load(dcu_cmd, 1);
    dcu_cmd_exec(1, NULL, 0);
    return 0;
}

int dcu_reye_mpr_train_v2(u32 lane_mask, u32 mr3)
{
    const u32 golden[4] = {0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00};

    u32 gtprd[4];
    u32 gtdly[4];

    u64 dcu_cmd[2];
    u32 dcu_data[4];
    int recv = 0;

    int err = 0;
    u16 err_tab[DCU_EYE_TABSIZE][16];

    u32 eyep_l[8];
    u32 eyep_r[8];
    u32 eyep_m[8];
    u32 eyen_l[8];
    u32 eyen_r[8];
    u32 eyen_m[8];

    for (int dx = 0; dx < 4; ++dx) {
        u32  dgsl = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset   , DGSL_mask   );
        u32 dqsgd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset  , DQSGD_mask  );
        u32 rdqsd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset  , RDQSD_mask  );
        gtprd[dx] = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0   + 0x100 * dx, GDQSPRD_offset, GDQSPRD_mask);
        gtdly[dx] = dgsl * gtprd[dx] + dqsgd - rdqsd;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR3 + 0x100 * dx, 0);
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR4 + 0x100 * dx, 0);
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR5 + 0x100 * dx, 0);
    }

    // Enter MPR Mode
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000,       0x0, 0x0, DCU_CMD_PREC_ALL, DCU_DTP_PPA, DCU_PRT_1);
    dcu_cmd[1] = dcu_cmd_make(PUB_DATA_0000_0000, mr3 | 0x4, 0x3, DCU_CMD_MRS     , DCU_DTP_MOD, DCU_PRT_1);
    dcu_cmd_load(dcu_cmd, 2);
    dcu_cmd_exec(2, NULL, 0);

    // setup READ command
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, 0x0, 0x0, DCU_CMD_READ, DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd[1] = dcu_cmd_make(PUB_DATA_0000_0000, 0x8, 0x0, DCU_CMD_READ, DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd_load(dcu_cmd, 2);

    for (int dx = 0; dx < 4; ++dx) {
        u32 lane_en = lane_mask & (0x1 << dx);
        if (lane_en == 0)
            continue;
        
        memset(eyep_l, 0, sizeof(eyep_l));
        memset(eyep_r, 0, sizeof(eyep_r));
        memset(eyep_m, 0, sizeof(eyep_m));
        memset(eyen_l, 0, sizeof(eyen_l));
        memset(eyen_r, 0, sizeof(eyen_r));
        memset(eyen_m, 0, sizeof(eyen_m));
        memset(err_tab, 0, sizeof(err_tab));

        u32 max_rdqsd = gtprd[dx] + gtprd[dx] / 5;
        max_rdqsd = min(DCU_EYE_TABSIZE, max_rdqsd);

        for (int i = 0; i < DCU_REYE_MPR_SCAN; ++i) {
            for (u32 rdqsd = 0; rdqsd < max_rdqsd; rdqsd += 1) {
                u32 dgsl  = (gtdly[dx] + rdqsd) / gtprd[dx];
                u32 dqsgd = (gtdly[dx] + rdqsd) % gtprd[dx];
                dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  rdqsd + 1);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, rdqsd + 1);

                for (int rpt = 0; rpt < DCU_REYE_MPR_SAMPLE; ++rpt) {
                    recv = dcu_cmd_exec(2, dcu_data, dx);
                    err += dcu_data_error_edge(dcu_data, golden, &err_tab[rdqsd][0], &err_tab[rdqsd][8]);
                }
            }
        }
        // AL_DDR_LOG("[REYE DX%d] DLY DQ0 DQ1 DQ2 DQ3 DQ4 DQ5 DQ6 DQ7 | DQ0 DQ1 DQ2 DQ3 DQ4 DQ5 DQ6 DQ7\r\n", dx);
        // for (int i = 0; i < max_rdqsd; ++i) {
        //     AL_DDR_LOG(
        //            "           %3d %3d %3d %3d %3d %3d %3d %3d %3d", i,
        //            err_tab[i][0], err_tab[i][1], err_tab[i][2], err_tab[i][3], 
        //            err_tab[i][4], err_tab[i][5], err_tab[i][6], err_tab[i][7]
        //     );
        //     AL_DDR_LOG(
        //            " | %3d %3d %3d %3d %3d %3d %3d %3d\r\n",
        //            err_tab[i][ 8], err_tab[i][ 9], err_tab[i][10], err_tab[i][11], 
        //            err_tab[i][12], err_tab[i][13], err_tab[i][14], err_tab[i][15]
        //     );
        // }

        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < max_rdqsd - DCU_REYE_WINDOW; ++i) {
                if (err_tab[i][j + 0] == 0 && 
                    sum_tab(&err_tab[i + 1][j + 0], DCU_REYE_WINDOW, 16) < DCU_REYE_THRD
                ) {
                    eyep_l[j] = i;
                    break;
                }
            }
            for (int i = max_rdqsd; i > DCU_REYE_WINDOW; --i) {
                if (err_tab[i][j + 0] == 0 && 
                    sum_tab(&err_tab[i - DCU_REYE_WINDOW][j + 0], DCU_REYE_WINDOW, 16) < DCU_REYE_THRD
                ) {
                    eyep_r[j] = i;
                    break;
                }
            }
            for (int i = 0; i < max_rdqsd - DCU_REYE_WINDOW; ++i) {
                if (err_tab[i][j + 8] == 0 && 
                    sum_tab(&err_tab[i + 1][j + 8], DCU_REYE_WINDOW, 16) < DCU_REYE_THRD
                ) {
                    eyen_l[j] = i;
                    break;
                }
            }
            for (int i = max_rdqsd; i > DCU_REYE_WINDOW; --i) {
                if (err_tab[i][j + 8] == 0 && 
                    sum_tab(&err_tab[i - DCU_REYE_WINDOW][j + 8], DCU_REYE_WINDOW, 16) < DCU_REYE_THRD
                ) {
                    eyen_r[j] = i;
                    break;
                }
            }
            eyep_m[j] = (eyep_l[j] + eyep_r[j]) / 2;
            eyen_m[j] = (eyen_l[j] + eyen_r[j]) / 2;
        }
        AL_DDR_LOG("[REYE MPR] DX BIT L_DLY M_DLY R_DLY WIDTH | L_DLY M_DLY R_DLY WIDTH\r\n", dx); 
        for (int i = 0; i < 8; ++i) {
            AL_DDR_LOG("[REYE MPR] %2d %2d %5d %5d %5d %5d  | %4d %5d %5d %5d\r\n", dx, i, 
                eyep_l[i], eyep_m[i], eyep_r[i], eyep_r[i] - eyep_l[i],
                eyen_l[i], eyen_m[i], eyen_r[i], eyen_r[i] - eyen_l[i]
            );
        }
        sort_u32(eyep_m, 8);
        sort_u32(eyen_m, 8);
        u32 rqdsd  = (eyep_m[3] + eyep_m[4]) / 2;
        u32 rqdsnd = (eyen_m[3] + eyen_m[4]) / 2;
        u32 dgsl  = (gtdly[dx] + rqdsnd) / gtprd[dx];
        u32 dqsgd = (gtdly[dx] + rqdsnd) % gtprd[dx];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  rqdsd  + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, rqdsnd + 1);
    }

    // Exit MPR Mode
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, mr3, 0x3, DCU_CMD_MRS, DCU_DTP_MOD, DCU_PRT_1);
    dcu_cmd_load(dcu_cmd, 1);
    dcu_cmd_exec(1, NULL, 0);
    return 0;
}

int dcu_weye_train(u32 lane_mask)
{
    u32 gtprd[4];

    int err = 0;
    u16 err_tab[DCU_EYE_TABSIZE][8];

    u32 eye_l[8];
    u32 eye_r[8];
    u32 eye_m[8];
    u32 max_wdqd;

    for (int dx = 0; dx < 4; ++dx) {
        gtprd[dx] = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0   + 0x100 * dx, GDQSPRD_offset, GDQSPRD_mask);
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR1 + 0x100 * dx, 0);
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR2 + 0x100 * dx, 0);
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR3 + 0x100 * dx, 0);
    }

    for (int dx = 0; dx < 4; ++dx) {
        u32 lane_en = lane_mask & (0x1 << dx);
        if (lane_en == 0)
            continue;
        
        memset(eye_l, 0, sizeof(eye_l));
        memset(eye_r, 0, sizeof(eye_r));
        memset(err_tab, 0, sizeof(err_tab));

        max_wdqd = gtprd[dx] + gtprd[dx] / 5;
        max_wdqd = min(DCU_EYE_TABSIZE, max_wdqd);

        for (int i = 0; i < DCU_WEYE_SCAN; ++i) {
            for (u32 wdqd = 0; wdqd < max_wdqd; wdqd += 1) {
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * dx, WDQD_offset, WDQD_mask, wdqd);
                err += dcu_mtest_weye(dx, DCU_WEYE_SAMPLE, err_tab[wdqd]);
            }
        }

        // AL_DDR_LOG("[WEYE DX%d] DLY DQ0 DQ1 DQ2 DQ3 DQ4 DQ5 DQ6 DQ7\r\n", dx);
        // for (int i = 0; i < max_wdqd; ++i) {
        //     AL_DDR_LOG(
        //            "           %3d %3d %3d %3d %3d %3d %3d %3d %3d\r\n", i,
        //            err_tab[i][0], err_tab[i][1], err_tab[i][2], err_tab[i][3], 
        //            err_tab[i][4], err_tab[i][5], err_tab[i][6], err_tab[i][7]
        //     );
        // }

        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < max_wdqd - DCU_WEYE_WINDOW; ++i) {
                if (err_tab[i][j] == 0 && 
                    sum_tab(&err_tab[i + 1][j], DCU_WEYE_WINDOW, 8) < DCU_WEYE_THRD
                ) {
                    eye_l[j] = i;
                    break;
                }
            }
            for (int i = max_wdqd; i > DCU_WEYE_WINDOW; --i) {
                if (err_tab[i][j] == 0 && 
                    sum_tab(&err_tab[i - DCU_WEYE_WINDOW][j], DCU_WEYE_WINDOW, 8) < DCU_WEYE_THRD
                ) {
                    eye_r[j] = i;
                    break;
                }
            }
            eye_m[j] = (eye_l[j] + eye_r[j]) / 2;
        }
        AL_DDR_LOG("[WEYE] DX BIT L_DLY M_DLY R_DLY WIDTH\r\n", dx);
        for (int i = 0; i < 8; ++i) {
            AL_DDR_LOG("[WEYE] %2d %2d %5d %5d %5d %5d\r\n", dx, i, eye_l[i], eye_m[i], eye_r[i], eye_r[i] - eye_l[i]);
        }

        u32 cfg_eye = 0;
/*
        for (int j = 0; j < 8; ++j) {
            cfg_eye = max(cfg_eye, eye_m[j]);
        }
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * dx, WDQD_offset, WDQD_mask, cfg_eye + 1);
        u32 bdlr = 0;
        bdlr |= min((cfg_eye - eye_m[7]), 63) << DQ0WBD_offset;
        bdlr |= min((cfg_eye - eye_m[6]), 63) << DQ1WBD_offset;
        bdlr |= min((cfg_eye - eye_m[5]), 63) << DQ2WBD_offset;
        bdlr |= min((cfg_eye - eye_m[4]), 63) << DQ3WBD_offset;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR1 + 0x100 * dx, bdlr);
        bdlr = 0;
        bdlr |= min((cfg_eye - eye_m[3]), 63) << DQ4WBD_offset;
        bdlr |= min((cfg_eye - eye_m[2]), 63) << DQ5WBD_offset;
        bdlr |= min((cfg_eye - eye_m[1]), 63) << DQ6WBD_offset;
        bdlr |= min((cfg_eye - eye_m[0]), 63) << DQ7WBD_offset;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR1 + 0x100 * dx, bdlr);
        bdlr = xxx << DMWBD_offset;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR2 + 0x100 * dx, bdlr);
*/
        sort_u32(eye_m, 8);
        cfg_eye = (eye_m[3] + eye_m[4]) / 2;
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * dx, WDQD_offset, WDQD_mask, cfg_eye + 1);
    }

    return 0;
}

int dcu_reye_train(u32 lane_mask)
{
    u32 gtprd[4];
    u32 gtdly[4];

    int err = 0;
    u16 err_tab[DCU_EYE_TABSIZE][16];

    u32 eyep_l[8];
    u32 eyep_r[8];
    u32 eyep_m[8];
    u32 eyen_l[8];
    u32 eyen_r[8];
    u32 eyen_m[8];

    for (int dx = 0; dx < 4; ++dx) {
        u32  dgsl = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset   , DGSL_mask   );
        u32 dqsgd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset  , DQSGD_mask  );
        u32 rdqsd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset  , RDQSD_mask  );
        gtprd[dx] = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0   + 0x100 * dx, GDQSPRD_offset, GDQSPRD_mask);
        gtdly[dx] = dgsl * gtprd[dx] + dqsgd - rdqsd;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR3 + 0x100 * dx, 0);
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR4 + 0x100 * dx, 0);
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR5 + 0x100 * dx, 0);
    }

    for (int dx = 0; dx < 4; ++dx) {
        u32 lane_en = lane_mask & (0x1 << dx);
        if (lane_en == 0)
            continue;
        
        memset(eyep_l, 0, sizeof(eyep_l));
        memset(eyep_r, 0, sizeof(eyep_r));
        memset(eyep_m, 0, sizeof(eyep_m));
        memset(eyen_l, 0, sizeof(eyen_l));
        memset(eyen_r, 0, sizeof(eyen_r));
        memset(eyen_m, 0, sizeof(eyen_m));
        memset(err_tab, 0, sizeof(err_tab));

        u32 max_rdqsd = gtprd[dx] + gtprd[dx] / 5;
        max_rdqsd = min(DCU_EYE_TABSIZE, max_rdqsd);

        for (int i = 0; i < DCU_REYE_SCAN; ++i) {
            for (u32 rdqsd = 0; rdqsd < max_rdqsd; rdqsd += 1) {
                u32 dgsl  = (gtdly[dx] + rdqsd) / gtprd[dx];
                u32 dqsgd = (gtdly[dx] + rdqsd) % gtprd[dx];
                dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  rdqsd + 1);
                dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, rdqsd + 1);

                err += dcu_mtest_reye(dx, DCU_REYE_PATT_RPT, DCU_REYE_READ_RPT, &err_tab[rdqsd][0], &err_tab[rdqsd][8]);
            }
        }

        // AL_DDR_LOG("[REYE DX%d] DLY DQ0 DQ1 DQ2 DQ3 DQ4 DQ5 DQ6 DQ7 | DQ0 DQ1 DQ2 DQ3 DQ4 DQ5 DQ6 DQ7\r\n", dx);
        // for (int i = 0; i < max_rdqsd; ++i) {
        //     AL_DDR_LOG(
        //            "           %3d %3d %3d %3d %3d %3d %3d %3d %3d", i,
        //            err_tab[i][0], err_tab[i][1], err_tab[i][2], err_tab[i][3], 
        //            err_tab[i][4], err_tab[i][5], err_tab[i][6], err_tab[i][7]
        //     );
        //     AL_DDR_LOG(
        //            " | %3d %3d %3d %3d %3d %3d %3d %3d\r\n",
        //            err_tab[i][ 8], err_tab[i][ 9], err_tab[i][10], err_tab[i][11], 
        //            err_tab[i][12], err_tab[i][13], err_tab[i][14], err_tab[i][15]
        //     );
        // }

        for (int j = 0; j < 8; ++j) {
            for (int i = 0; i < max_rdqsd - DCU_REYE_WINDOW; ++i) {
                if (err_tab[i][j + 0] == 0 && 
                    sum_tab(&err_tab[i + 1][j + 0], DCU_REYE_WINDOW, 16) < DCU_REYE_THRD
                ) {
                    eyep_l[j] = i;
                    break;
                }
            }
            for (int i = max_rdqsd; i > DCU_REYE_WINDOW; --i) {
                if (err_tab[i][j + 0] == 0 && 
                    sum_tab(&err_tab[i - DCU_REYE_WINDOW][j + 0], DCU_REYE_WINDOW, 16) < DCU_REYE_THRD
                ) {
                    eyep_r[j] = i;
                    break;
                }
            }
            for (int i = 0; i < max_rdqsd - DCU_REYE_WINDOW; ++i) {
                if (err_tab[i][j + 8] == 0 && 
                    sum_tab(&err_tab[i + 1][j + 8], DCU_REYE_WINDOW, 16) < DCU_REYE_THRD
                ) {
                    eyen_l[j] = i;
                    break;
                }
            }
            for (int i = max_rdqsd; i > DCU_REYE_WINDOW; --i) {
                if (err_tab[i][j + 8] == 0 && 
                    sum_tab(&err_tab[i - DCU_REYE_WINDOW][j + 8], DCU_REYE_WINDOW, 16) < DCU_REYE_THRD
                ) {
                    eyen_r[j] = i;
                    break;
                }
            }
            eyep_m[j] = (eyep_l[j] + eyep_r[j]) / 2;
            eyen_m[j] = (eyen_l[j] + eyen_r[j]) / 2;
        }
        AL_DDR_LOG("[REYE] DX BIT L_DLY M_DLY R_DLY WIDTH | L_DLY M_DLY R_DLY WIDTH\r\n", dx); 
        for (int i = 0; i < 8; ++i) {
            AL_DDR_LOG("[REYE] %2d %2d %5d %5d %5d %5d  | %4d %5d %5d %5d\r\n", dx, i, 
                eyep_l[i], eyep_m[i], eyep_r[i], eyep_r[i] - eyep_l[i],
                eyen_l[i], eyen_m[i], eyen_r[i], eyen_r[i] - eyen_l[i]
            );
        }
/*
        u32 cfg_eye = 0;
        for (int j = 0; j < 8; ++j) {
            cfg_eye = max(cfg_eye, eye_m[j]);
        }
        u32 dgsl  = (gtdly[dx] + cfg_eye) / gtprd[dx];
        u32 dqsgd = (gtdly[dx] + cfg_eye) % gtprd[dx];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  cfg_eye + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, cfg_eye + 1);

        u32 bdlr = 0;
        bdlr |= min((cfg_eye - eye_m[7]), 63) << DQ0RBD_offset;
        bdlr |= min((cfg_eye - eye_m[6]), 63) << DQ1RBD_offset;
        bdlr |= min((cfg_eye - eye_m[5]), 63) << DQ2RBD_offset;
        bdlr |= min((cfg_eye - eye_m[4]), 63) << DQ3RBD_offset;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR3 + 0x100 * dx, bdlr);
        bdlr = 0;
        bdlr |= min((cfg_eye - eye_m[3]), 63) << DQ4RBD_offset;
        bdlr |= min((cfg_eye - eye_m[2]), 63) << DQ5RBD_offset;
        bdlr |= min((cfg_eye - eye_m[1]), 63) << DQ6RBD_offset;
        bdlr |= min((cfg_eye - eye_m[0]), 63) << DQ7RBD_offset;
        dr1x90_reg_write(DDRC_ADDR_PPC + DX0BDLR4 + 0x100 * dx, bdlr);
*/
        sort_u32(eyep_m, 8);
        sort_u32(eyen_m, 8);
        u32 rqdsd  = (eyep_m[3] + eyep_m[4]) / 2;
        u32 rqdsnd = (eyen_m[3] + eyen_m[4]) / 2;
        u32 dgsl  = (gtdly[dx] + rqdsnd) / gtprd[dx];
        u32 dqsgd = (gtdly[dx] + rqdsnd) % gtprd[dx];
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  rqdsd  + 1);
        dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, rqdsnd + 1);
    }

    return 0;
}

int dcu_wl(u32 lane_mask, u32 mr1)
{
    // TODO
    return 0;
}

int dcu_wladj(u32 lane_mask)
{
    int err = 0;
    u16 err_tab[MAX_WLSL][8];

    for (int dx = 0; dx < 4; ++dx) {
        u32 lane_en = lane_mask & (0x1 << dx);
        if (lane_en == 0)
            continue;
        
        u32 wlsl = 0;
        memset(err_tab, 0, sizeof(err_tab));

        for (int i = 0; i < DCU_WLADJ_SCAN; ++i) {
            for (wlsl = 0; wlsl < MAX_WLSL; wlsl += 1) {
                do {
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * dx, WLSL_offset, WLSL_mask, wlsl);
                } while (dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * dx, WLSL_offset, WLSL_mask) != wlsl);
                err += dcu_mtest_weye(dx, DCU_WLADJ_SAMPLE, err_tab[wlsl]);
            }
        }
        AL_DDR_LOG("[WDA DX%d] WLSL DQ0 DQ1 DQ2 DQ3 DQ4 DQ5 DQ6 DQ7\r\n", dx);
        for (int i = 0; i < MAX_WLSL; ++i) {
            AL_DDR_LOG(
                   "           %3d %3d %3d %3d %3d %3d %3d %3d %3d\r\n", i,
                   err_tab[i][0], err_tab[i][1], err_tab[i][2], err_tab[i][3], 
                   err_tab[i][4], err_tab[i][5], err_tab[i][6], err_tab[i][7]
            );
        }
        u32 err_min = 32768;
        wlsl = 0;
        for (int i = 0; i < MAX_WLSL; ++i) {
            u32 s = sum_tab(err_tab[i], 8, 1);
            if (s < err_min) {
                err_min = s;
                wlsl = i;
            }
        }
        dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0 + 0x100 * dx, WLSL_offset, WLSL_mask, wlsl);
    }

    return 0;
}

int dcu_vref_reye_mpr_scan(u32 lane_mask, u32 mr3)
{
    const u32 golden[4] = {0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00};
    const u32 vref_step = 10;
    const u32 rdqsd_step = 1;

    u32 gtprd[4];
    u32 gtdly[4];

    u64 dcu_cmd[2];
    u32 dcu_data[4];
    int recv = 0;

    int err = 0;
    u16 err_tab[DCU_EYE_TABSIZE][8];


    for (int dx = 0; dx < 4; ++dx) {
        u32  dgsl = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset   , DGSL_mask   );
        u32 dqsgd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset  , DQSGD_mask  );
        u32 rdqsd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset  , RDQSD_mask  );
        gtprd[dx] = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0   + 0x100 * dx, GDQSPRD_offset, GDQSPRD_mask);
        gtdly[dx] = dgsl * gtprd[dx] + dqsgd - rdqsd;
    }

    // Enter MPR Mode
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000,       0x0, 0x0, DCU_CMD_PREC_ALL, DCU_DTP_PPA, DCU_PRT_1);
    dcu_cmd[1] = dcu_cmd_make(PUB_DATA_0000_0000, mr3 | 0x4, 0x3, DCU_CMD_MRS     , DCU_DTP_MOD, DCU_PRT_1);
    dcu_cmd_load(dcu_cmd, 2);
    dcu_cmd_exec(2, NULL, 0);

    // setup READ command
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, 0x0, 0x0, DCU_CMD_READ, DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd[1] = dcu_cmd_make(PUB_DATA_0000_0000, 0x8, 0x0, DCU_CMD_READ, DCU_DTP_RD2PRE, DCU_PRT_BL);
    dcu_cmd_load(dcu_cmd, 2);

    for (int dx = 0; dx < 4; ++dx) {
        u32 lane_en = lane_mask & (0x1 << dx);
        if (lane_en == 0)
            continue;

        u32 max_rdqsd = gtprd[dx] + gtprd[dx] / 5;
        max_rdqsd = min(DCU_EYE_TABSIZE, max_rdqsd);

        AL_DDR_LOG("DX%d_Vref ", dx);
        for (u32 rdqsd = 0; rdqsd < max_rdqsd; rdqsd += rdqsd_step) {
            AL_DDR_LOG("%4d ", rdqsd);
        }
        AL_DDR_LOG("\r\n");

        for (uint32_t vref = 0; vref <= 0xFF; vref += vref_step) {
            uint32_t bref_cfg = (vref << 0) | (vref << 8) | (vref << 16) | (vref << 24);
            dr1x90_reg_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg1  , bref_cfg);
            dr1x90_reg_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg1_1, bref_cfg);
            dr1x90_reg_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg1  , bref_cfg);
            dr1x90_reg_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg1_1, bref_cfg);

            AL_DDR_LOG("0x%02x ", vref);

            memset(err_tab, 0, sizeof(err_tab));

            for (int i = 0; i < DCU_REYE_MPR_SCAN; ++i) {
                for (u32 rdqsd = 0; rdqsd < max_rdqsd; rdqsd += rdqsd_step) {
                    u32 dgsl  = (gtdly[dx] + rdqsd) / gtprd[dx];
                    u32 dqsgd = (gtdly[dx] + rdqsd) % gtprd[dx];
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  rdqsd + 1);
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, rdqsd + 1);

                    for (int rpt = 0; rpt < DCU_REYE_MPR_SAMPLE; ++rpt) {
                        recv = 0;
                        for (int retry = 0; retry < 3 && recv != 4; ++retry) {
                            recv = dcu_cmd_exec(2, dcu_data, dx);
                        }
                        err += dcu_data_error(dcu_data, golden, err_tab[rdqsd]);
                    }
                }
            }

            for (u32 rdqsd = 0; rdqsd < max_rdqsd; rdqsd += rdqsd_step) {
                u32 val = sum_tab(err_tab[rdqsd], 8, 1);
                AL_DDR_LOG("%4d ", val);
            }
            AL_DDR_LOG("\r\n");

        }
    }

    // Exit MPR Mode
    dcu_cmd[0] = dcu_cmd_make(PUB_DATA_0000_0000, mr3, 0x3, DCU_CMD_MRS, DCU_DTP_MOD, DCU_PRT_1);
    dcu_cmd_load(dcu_cmd, 1);
    dcu_cmd_exec(1, NULL, 0);
    return 0;
}

int dcu_vref_rweye_scan(u32 lane_mask)
{
    const u32 vref_step = 10;
    const u32  dly_step = 1;

    u32 gtprd[4];
    u32 gtdly[4];

    u64 dcu_cmd[2];
    u32 dcu_data[4];
    int recv = 0;

    int err = 0;
    u16 err_tab[DCU_EYE_TABSIZE][8];


    for (int dx = 0; dx < 4; ++dx) {
        u32  dgsl = dr1x90_field_read(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset   , DGSL_mask   );
        u32 dqsgd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset  , DQSGD_mask  );
        u32 rdqsd = dr1x90_field_read(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset  , RDQSD_mask  );
        gtprd[dx] = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0   + 0x100 * dx, GDQSPRD_offset, GDQSPRD_mask);
        gtdly[dx] = dgsl * gtprd[dx] + dqsgd - rdqsd;
    }

    for (int dx = 0; dx < 4; ++dx) {
        u32 lane_en = lane_mask & (0x1 << dx);
        if (lane_en == 0)
            continue;

        u32 max_dly = gtprd[dx] + gtprd[dx] / 5;
        max_dly = min(DCU_EYE_TABSIZE, max_dly);

        AL_DDR_LOG("DX%d_Vref ", dx);
        for (u32 dly = 0; dly < max_dly; dly += dly_step) {
            AL_DDR_LOG("%4d ", dly);
        }
        AL_DDR_LOG("\r\n");

        for (uint32_t vref = 0; vref <= 0xFF; vref += vref_step) {
            uint32_t bref_cfg = (vref << 0) | (vref << 8) | (vref << 16) | (vref << 24);
            dr1x90_reg_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg1  , bref_cfg);
            dr1x90_reg_write(DDRC_ADDR_BK0_IOMC1 + brefhp_cfg1_1, bref_cfg);
            dr1x90_reg_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg1  , bref_cfg);
            dr1x90_reg_write(DDRC_ADDR_BK1_IOMC1 + brefhp_cfg1_1, bref_cfg);

            AL_DDR_LOG("0x%02x ", vref);

            memset(err_tab, 0, sizeof(err_tab));
/*
            for (int i = 0; i < DCU_REYE_SCAN; ++i) {
                for (u32 dly = 0; dly < max_dly; dly += dly_step) {
                    u32 dgsl  = (gtdly[dx] + dly) / gtprd[dx];
                    u32 dqsgd = (gtdly[dx] + dly) % gtprd[dx];
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0GTR0   + 0x100 * dx, DGSL_offset,   DGSL_mask ,  dgsl);
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR2 + 0x100 * dx, DQSGD_offset,  DQSGD_mask,  dqsgd + 1);
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR3 + 0x100 * dx, RDQSD_offset,  RDQSD_mask,  dly + 1);
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR4 + 0x100 * dx, RDQSND_offset, RDQSND_mask, dly + 1);

                    err += dcu_mtest_reye(dx, DCU_REYE_PATT_RPT, DCU_REYE_READ_RPT, &err_tab[dly][0], &err_tab[dly][0]);
                }
            }
*/
            for (int i = 0; i < DCU_WEYE_SCAN; ++i) {
                for (u32 dly = 0; dly < max_dly; dly += 1) {
                    dr1x90_field_write(DDRC_ADDR_PPC + DX0LCDLR1 + 0x100 * dx, WDQD_offset, WDQD_mask, dly);
                    err += dcu_mtest_weye(dx, DCU_WEYE_SAMPLE, &err_tab[dly][0]);
                }
            }

            for (u32 dly = 0; dly < max_dly; dly += dly_step) {
                u32 val = sum_tab(err_tab[dly], 8, 1);
                AL_DDR_LOG("%4d ", val);
            }
            AL_DDR_LOG("\r\n");

        }
    }

    return 0;
}
