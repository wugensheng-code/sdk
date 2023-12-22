#include "dr1x90_ddrc_init.h"

#define TRY_CNT 32

void dr1x90_ddrc_train_wl()
{
    u32 regData = 0;
    u32 wlprd = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0, WLPRD_offset, WL_mask);
    AL_DDR_LOG("[DDR WL] DX0GSR0.WLPRD = %d\r\n", wlprd);

    // 1'b0 = 8 DDL tap delays; 1'b1 = 0.52 * WLPRD DDL tap delays
    dr1x90_field_write(DDRC_ADDR_PPC + DTCR1, WLVLDPRD_offset, WLVLDPRD_mask, 0x0);
    // 1'b0 = 32 step sizes; 1'b1 = 1 step size
    dr1x90_field_write(DDRC_ADDR_PPC + PGCR1, WLSTEP_offset, WLSTEP_mask, 0x0);
    // Start WL
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = regData | WL_mask | INIT_mask;
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
    // Wait WL Done
    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, WLDONE_offset, WLDONE_mask, 0x1, -1);
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
    AL_DDR_LOG("[DDR WL] %s\r\n", (regData & PGSR0_WLERR_mask) ? "Error" : "Done");
}

void dr1x90_ddrc_train_gate()
{
    u32 regData = 0;
    u32 gdqsprd = dr1x90_field_read(DDRC_ADDR_PPC + DX0GSR0, GDQSPRD_offset, GDQSPRD_mask);
    AL_DDR_LOG("[DDR GATE] DX0GSR0.GDQSPRD = %d\r\n", gdqsprd);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + DTCR0);
    // Data Training Compare Data
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTCMPD_offset, DTCMPD_mask, 0x1, regData);
    // Data Training Using MPR
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTMPR_offset, DTMPR_mask, 0x1, regData);
    // Data Training Repeat Number
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR0, DTRPTN_offset, DTRPTN_mask, 15, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + DTCR0, regData);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + DTCR1);
    // Read Leveling Gate Sampling Difference, 0x2: GDQSPRD/8
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLGDIFF_offset, RDLVLGDIFF_mask, 0x2, regData);
    // Read Leveling Gate Shift, 0x3: GDQSPRD/16
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLGS_offset, RDLVLGS_mask, 0x3, regData);
    // Read Leveling Enable
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, RDLVLEN_offset, RDLVLEN_mask, 0x1, regData);
    // Basic Gate Training Enable
    regData = dr1x90_field_set(DDRC_ADDR_PPC + DTCR1, BSTEN_offset, BSTEN_mask, 0x1, regData);
    dr1x90_reg_write(DDRC_ADDR_PPC + DTCR1, regData);

    // Start QOS Gate Training
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = regData | QSGATE_mask | INIT_mask;
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
    // Wait Done
    dr1x90_field_wait(DDRC_ADDR_PPC + PGSR0, QSGDONE_offset, QSGDONE_mask, 0x1, -1);
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
    AL_DDR_LOG("[DDR GATE] %s\r\n", (regData & PGSR0_QSGERR_mask) ? "Error" : "Done");
}

void dr1x90_ddrc_train_wladj()
{
    u32 regData = 0;
    // Start WLADJ
    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
    regData = regData | WLADJ_mask | INIT_mask;
    dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
    // Wait Done
    dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, WLADONE_offset, WLADONE_mask, 0x1, -1);
    dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);

    regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
    AL_DDR_LOG("[DDR WLADJ] %s\r\n", (regData & PGSR0_WLAERR_mask) ? "Error" : "Done");
}

void dr1x90_ddrc_train_eye()
{
    u32 err = 1;
    u32 regData = 0;
    for (int i = 0; i < TRY_CNT && err != 0; ++i) {
        err = 0;
        // Read Data Bit Deskew
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
        regData = regData | RDDSKW_mask | INIT_mask;
        dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
        dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, RDDONE_offset, RDDONE_mask, 0x1, -1);
        dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        // AL_DDR_LOG("[DDR RD DSKEW] #%d %s\r\n", i, (regData & PGSR0_RDERR_mask) ? "Error" : "Done");
        err |= regData & PGSR0_RDERR_mask;

        // Write Data Bit Deskew
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
        regData = regData | WRDSKW_mask | INIT_mask;
        dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
        dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, WDDONE_offset, WDDONE_mask, 0x1, -1);
        dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        // AL_DDR_LOG("[DDR WR DSKEW] #%d %s\r\n", i, (regData & PGSR0_WDERR_mask) ? "Error" : "Done");
        err |= regData & PGSR0_WDERR_mask;

        // Read Data Eye Training
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
        regData = regData | RDEYE_mask | INIT_mask;
        dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
        dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, REDONE_offset, REDONE_mask, 0x1, -1);
        dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        // AL_DDR_LOG("[DDR RD CENTER] #%d %s\r\n", i, (regData & PGSR0_REERR_mask) ? "Error" : "Done");
        err |= regData & PGSR0_REERR_mask;

        // Write Data Eye Training
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PIR);
        regData = regData | WREYE_mask | INIT_mask;
        dr1x90_reg_write(DDRC_ADDR_PPC + PIR, regData);
        dr1x90_field_wait(DDRC_ADDR_PPC+ PGSR0, WEDONE_offset, WEDONE_mask, 0x1, -1);
        dr1x90_field_wait(DDRC_ADDR_PPC + PIR, INIT_offset, INIT_mask, 0x0, -1);
        regData = dr1x90_reg_read(DDRC_ADDR_PPC + PGSR0);
        // AL_DDR_LOG("[DDR WR CENTER] #%d %s\r\n", i, (regData & PGSR0_WEERR_mask) ? "Error" : "Done");
        err |= regData & PGSR0_WEERR_mask;

        AL_DDR_LOG("[DDR TRAIN #%d] RDDSKE:%c WRDSKW:%c RDEYE:%c WREYE:%c\r\n", i,
            (err & PGSR0_RDERR_mask) ? 'X' : 'O',
            (err & PGSR0_WDERR_mask) ? 'X' : 'O',
            (err & PGSR0_REERR_mask) ? 'X' : 'O',
            (err & PGSR0_WEERR_mask) ? 'X' : 'O'
        );
    }
}
