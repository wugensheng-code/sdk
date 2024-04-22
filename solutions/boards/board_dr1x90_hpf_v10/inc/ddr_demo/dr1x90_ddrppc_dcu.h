#ifndef __AL_DR1X90_DDRPPC_DCU_H_
#define __AL_DR1X90_DDRPPC_DCU_H_

#include "alc_types.h"
#include "dr1x90_ddrc_init.h"

typedef uint64_t u64;

#define SCHD_REQ_DATA_OFFSET (0)
#define SCHD_REQ_MASK_OFFSET (SCHD_REQ_DATA_OFFSET + 5)
#define SCHD_REQ_ADDR_OFFSET (SCHD_REQ_MASK_OFFSET + 4)
#define SCHD_REQ_BANK_OFFSET (SCHD_REQ_ADDR_OFFSET + 18)
#define SCHD_REQ_RANK_OFFSET (SCHD_REQ_BANK_OFFSET + 4)
#define SCHD_REQ_CMD_OFFSET  (SCHD_REQ_RANK_OFFSET + 1)
#define SCHD_REQ_TAG_OFFSET  (SCHD_REQ_CMD_OFFSET + 4)
#define SCHD_REQ_DTP_OFFSET  (SCHD_REQ_TAG_OFFSET + 2)
#define SCHD_REQ_RPT_OFFSET  (SCHD_REQ_DTP_OFFSET + 5)

#define PUB_DATA_0000_0000    0
#define PUB_DATA_FFFF_FFFF    1
#define PUB_DATA_5555_5555    2
#define PUB_DATA_AAAA_AAAA    3
#define PUB_DATA_0000_5500    4
#define PUB_DATA_5555_0055    5
#define PUB_DATA_0000_AA00    6
#define PUB_DATA_AAAA_00AA    7
#define PUB_DATA_DTDR0        8
#define PUB_DATA_DTDR1        9
#define PUB_DATA_UDDR0        10
#define PUB_DATA_UDDR1        11
#define PUB_DATA_WALKING_1    12
#define PUB_DATA_WALKING_0    13
#define PUB_DATA_USER_PATTERN 14
#define PUB_DATA_LFSR         15
#define PUB_DATA_SCHCR0       16
#define PUB_DATA_FF00_FF00    17
#define PUB_DATA_FFFF_0000    18
#define PUB_DATA_0000_FF00    19
#define PUB_DATA_FFFF_00FF    20
#define PUB_DATA_00FF_00FF    21
#define PUB_DATA_F0F0_F0F0    22
#define PUB_DATA_0F0F_0F0F    23

#define DCU_CMD_NOP         0
#define DCU_CMD_MRS         1
#define DCU_CMD_SELF_REFR   2
#define DCU_CMD_REFR        3
#define DCU_CMD_PREC        4
#define DCU_CMD_PREC_ALL    5
#define DCU_CMD_ACTIVATE    6
#define DCU_CMD_SPECIAL     7
#define DCU_CMD_WRITE       8
#define DCU_CMD_WRITE_PREC  9
#define DCU_CMD_READ        10
#define DCU_CMD_READ_PREC   11
#define DCU_CMD_ZQCAL_SHORT 12
#define DCU_CMD_ZQCAL_LONG  13
#define DCU_CMD_POWER_DOWN  14
#define DCU_CMD_SDRAM_NOP   15

#define DCU_DTP_NONE     0
#define DCU_DTP_RP       1
#define DCU_DTP_RAS      2
#define DCU_DTP_RRD      3
#define DCU_DTP_RC       4
#define DCU_DTP_MRD      5
#define DCU_DTP_MOD      6
#define DCU_DTP_FAW      7
#define DCU_DTP_RFC      8
#define DCU_DTP_WLMRD    9
#define DCU_DTP_WLO      10
#define DCU_DTP_PPA      19
#define DCU_DTP_PRE2ACT  20
#define DCU_DTP_ACT2RW   21
#define DCU_DTP_RD2PRE   22
#define DCU_DTP_WR2PRE   23
#define DCU_DTP_RD2WR    24
#define DCU_DTP_WR2RD    25
#define DCU_DTP_RDAP2ACT 26
#define DCU_DTP_WRAP2ACT 27
#define DCU_DTP_DCUT0    28
#define DCU_DTP_DCUT1    29
#define DCU_DTP_DCUT2    30

#define DCU_PRT_1     0
#define DCU_PRT_2     1
#define DCU_PRT_4     2
#define DCU_PRT_8     3
#define DCU_PRT_BL    4
#define DCU_PRT_DCUT0 5
#define DCU_PRT_DCUT1 6
#define DCU_PRT_DCUT2 7

#define DINST_NOP       0x0
#define DINST_RUN       0x1
#define DINST_STOP      0x2
#define DINST_STOP_LOOP 0x3
#define DINST_RESET     0x4

u64 dcu_cmd_make(u64 data, u64 addr, u64 bank, u64 cmd, u64 dtp, u64 rpt);
int dcu_cmd_load(u64* cmd, int n);
int dcu_cmd_exec(int n, u32* data, u32 byte);

#define BL8 8
#define MAX_DGSL 16
#define MAX_WLSL 16

#define DCU_GATE_SAMPLE  32
#define DCU_GATE_DIFFDIV 8
#define DCU_GATE_THRD    8
#define DCU_GATE_SHIFT   3
#define DCU_GATE_STSLEN  (DCU_GATE_DIFFDIV * (MAX_DGSL + 1) + 8)

#define DCU_EYE_TABSIZE 256

#define DCU_REYE_MPR_SCAN   8
#define DCU_REYE_MPR_SAMPLE 8
#define DCU_REYE_MPR_WINDOW 5
#define DCU_REYE_MPR_THRD   16

#define DCU_REYE_SCAN     6
#define DCU_REYE_READ_RPT 4
#define DCU_REYE_PATT_RPT 1
#define DCU_REYE_WINDOW 5
#define DCU_REYE_THRD   64

#define DCU_WEYE_SCAN   2
#define DCU_WEYE_SAMPLE 4
#define DCU_WEYE_WINDOW 5
#define DCU_WEYE_THRD   64

#define DCU_WLADJ_SCAN   8
#define DCU_WLADJ_SAMPLE 16

int dcu_gate_train(u32 lane_mask, u32 mr3);
int dcu_reye_mpr_train(u32 lane_mask, u32 mr3);
int dcu_reye_mpr_train_v2(u32 lane_mask, u32 mr3);
int dcu_reye_train(u32 lane_mask);
int dcu_weye_train(u32 lane_mask);

int dcu_wl(u32 lane_mask, u32 mr1);
int dcu_wladj(u32 lane_mask);

#endif
