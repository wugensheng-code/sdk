#ifndef __AL_ALC_DDR_ENV_H_
#define __AL_ALC_DDR_ENV_H_

#include "alc_types.h"

#define DFI_1_2 0
#define DFI_1_4 1
#define DDR3 3
#define DDR4 4

#define env_cfg_dx_num    4
#define env_cfg_ddr_speed 1600
#define env_cfg_ddr_mode  DDR3

#define env_prfbyp_en          1
#define env_cfg_static_mode    0
#define env_cfg_rd_rr_mode     1
#define env_cfg_wr_rr_mode     1
#define env_cfg_wrpstex        1
#define env_cfg_tx_fifo_bypass 1
#define env_cfg_RDBI           0
#define env_cfg_WDBI           0
#define env_cfg_dfi_ratio      DFI_1_2

#define env_cfg_CL      9
#define env_cfg_CWL     7


// DRAM Settings
#define MR0_WR   0b110   // 12
//#define MR0_CL   0b1110  // 11
//#define MR2_CWL  0b011   // 8
#if (env_cfg_CL == 5) 
#define MR0_CL   0b0010 
#elif (env_cfg_CL == 6)   
#define MR0_CL   0b0100 
#elif (env_cfg_CL == 7)   
#define MR0_CL   0b0110 
#elif (env_cfg_CL == 8)   
#define MR0_CL   0b1000 
#elif (env_cfg_CL == 9)   
#define MR0_CL   0b1010 
#elif (env_cfg_CL == 10)   
#define MR0_CL   0b1100 
#elif (env_cfg_CL == 11)   
#define MR0_CL   0b1110 
#elif (env_cfg_CL == 12)   
#define MR0_CL   0b0001 
#elif (env_cfg_CL == 13)   
#define MR0_CL   0b0011 
#elif (env_cfg_CL == 14)   
#define MR0_CL   0b0101 
#endif
  
#define MR2_CWL  (env_cfg_CWL-5)   // 7

// MR Register
#define MR0 (0x0100 | (MR0_WR << 9) | (((MR0_CL >> 1) & 0b111) << 4) | ((MR0_CL & 0b0001) << 2))
#define MR1 0x0044
#define MR2 (0x0200 | (MR2_CWL << 3))
#define MR3 0x0000

#define MR4 0x0000
#define MR5 0x0000
#define MR6 0x0000

#endif // end of AL_ALC_DDR_ENV_H
