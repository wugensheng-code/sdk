/*****************************************************************************/
/**
 * @file xxx.h
 * This is the header file which contains definitions for the
 * xxx.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date        Changes
 * ----- ---- -------- -------------------------------------------------------
 * 1.00  c.z  3/10/22  Initial release
 * </pre>
 *
 * @note
 ******************************************************************************/

#ifndef AL9000_REGISTERS_H
#define AL9000_REGISTERS_H

#include "al_types.h"

/* CSU local registers */
#define CSU_LOCAL_BASE_ADDR 0x00069000
#define CSU_LOCAL__CSU_IDCODE__ADDR (CSU_LOCAL_BASE_ADDR + 0x00)
#define CSU_LOCAL__CSU_VERSION__ADDR (CSU_LOCAL_BASE_ADDR + 0x04)
#define CSU_LOCAL__CSU_TAMPER_TRIG__ADDR (CSU_LOCAL_BASE_ADDR + 0x08)
#define CSU_LOCAL__TRNG_ENABLE__ADDR (CSU_LOCAL_BASE_ADDR + 0x0c)
#define CSU_LOCAL__CSU_RESET_VECTOR__ADDR (CSU_LOCAL_BASE_ADDR + 0x10)
#define CSU_LOCAL__NON_SECURE_LOCKDOWN__ADDR (CSU_LOCAL_BASE_ADDR + 0x14)
#define CSU_LOCAL__SECURE_LOCKDOWN__ADDR (CSU_LOCAL_BASE_ADDR + 0x18)
#define CSU_LOCAL__CSU_ROM_DIGEST_0__ADDR (CSU_LOCAL_BASE_ADDR + 0x40)
#define CSU_LOCAL__CSU_ROM_DIGEST_1__ADDR (CSU_LOCAL_BASE_ADDR + 0x44)
#define CSU_LOCAL__CSU_ROM_DIGEST_2__ADDR (CSU_LOCAL_BASE_ADDR + 0x48)
#define CSU_LOCAL__CSU_ROM_DIGEST_3__ADDR (CSU_LOCAL_BASE_ADDR + 0x4c)
#define CSU_LOCAL__CSU_ROM_DIGEST_4__ADDR (CSU_LOCAL_BASE_ADDR + 0x50)
#define CSU_LOCAL__CSU_ROM_DIGEST_5__ADDR (CSU_LOCAL_BASE_ADDR + 0x54)
#define CSU_LOCAL__CSU_ROM_DIGEST_6__ADDR (CSU_LOCAL_BASE_ADDR + 0x58)
#define CSU_LOCAL__CSU_ROM_DIGEST_7__ADDR (CSU_LOCAL_BASE_ADDR + 0x5c)
#define CSU_LOCAL__CSU_EFUSE_DATA_0__ADDR (CSU_LOCAL_BASE_ADDR + 0x80)
#define CSU_LOCAL__CSU_EFUSE_DATA_1__ADDR (CSU_LOCAL_BASE_ADDR + 0x84)
#define CSU_LOCAL__CSU_EFUSE_DATA_2__ADDR (CSU_LOCAL_BASE_ADDR + 0x88)
#define CSU_LOCAL__CSU_EFUSE_DATA_3__ADDR (CSU_LOCAL_BASE_ADDR + 0x8c)
#define CSU_LOCAL__CSU_EFUSE_DATA_4__ADDR (CSU_LOCAL_BASE_ADDR + 0x90)
#define CSU_LOCAL__CSU_EFUSE_DATA_5__ADDR (CSU_LOCAL_BASE_ADDR + 0x94)
#define CSU_LOCAL__CSU_EFUSE_DATA_6__ADDR (CSU_LOCAL_BASE_ADDR + 0x98)
#define CSU_LOCAL__CSU_EFUSE_DATA_7__ADDR (CSU_LOCAL_BASE_ADDR + 0x9c)
#define CSU_LOCAL__CSU_EFUSE_DATA_8__ADDR (CSU_LOCAL_BASE_ADDR + 0xA0)
#define CSU_LOCAL__CSU_EFUSE_DATA_9__ADDR (CSU_LOCAL_BASE_ADDR + 0xA4)
#define CSU_LOCAL__CSU_EFUSE_DATA_10__ADDR (CSU_LOCAL_BASE_ADDR + 0xA8)
#define CSU_LOCAL__CSU_EFUSE_DATA_11__ADDR (CSU_LOCAL_BASE_ADDR + 0xAc)
#define CSU_LOCAL__CSU_EFUSE_DATA_12__ADDR (CSU_LOCAL_BASE_ADDR + 0xB0)
#define CSU_LOCAL__CSU_EFUSE_DATA_13__ADDR (CSU_LOCAL_BASE_ADDR + 0xB4)
#define CSU_LOCAL__CSU_EFUSE_DATA_14__ADDR (CSU_LOCAL_BASE_ADDR + 0xB8)
#define CSU_LOCAL__CSU_EFUSE_DATA_15__ADDR (CSU_LOCAL_BASE_ADDR + 0xBc)
#define CSU_LOCAL__CSU_EFUSE_DATA_16__ADDR (CSU_LOCAL_BASE_ADDR + 0xC0)
#define CSU_LOCAL__CSU_EFUSE_DATA_17__ADDR (CSU_LOCAL_BASE_ADDR + 0xC4)
#define CSU_LOCAL__CSU_EFUSE_DATA_18__ADDR (CSU_LOCAL_BASE_ADDR + 0xC8)

typedef union {
  struct {
    u32 trng0_enable : 1;
    u32 trng1_enable : 1;
    u32 reserved : 30;
  };
  u32 reg;
} CSU_LOCAL__TRNG_ENABLE__ACC_T;

typedef union {
  struct {
    u32 non_secure_lockdown : 1;
    u32 reserved : 31;
  };
  u32 reg;
} CSU_LOCAL__NON_SECURE_LOCKDOWN__ACC_T;

typedef union {
  struct {
    u32 secure_lockdown : 1;
    u32 reserved : 31;
  };
  u32 reg;
} CSU_LOCAL__SECURE_LOCKDOWN__ACC_T;

/* TOP NS registers */
#define CSU_TOP_NS_BASE_ADDR 0xF8800000
#define TOP_NS__CFG_CTRL_OCM__ADDR (CSU_TOP_NS_BASE_ADDR + 0x1BC)
#define TOP_NS__BOOT_MOD__ADDR (CSU_TOP_NS_BASE_ADDR + 0x200)

typedef union {
  struct {
    u32 boot_mode : 3;
    u32 reserved : 29;
  };
  u32 reg;
} TOP_NS__BOOT_MOD__ACC_T;

/* TOP S registers */
#define TOP_S_BASE_ADDR 0xF8806000
#define TOP_S__ERR_HW_EN0_SET__ADDR (TOP_S_BASE_ADDR + 0x000)
#define TOP_S__ERR_HW_EN0_CLR__ADDR (TOP_S_BASE_ADDR + 0x004)
#define TOP_S__ERR_HW_EN1_SET__ADDR (TOP_S_BASE_ADDR + 0x008)
#define TOP_S__ERR_HW_EN1_CLR__ADDR (TOP_S_BASE_ADDR + 0x00C)
#define TOP_S__INT_EN0_SET__ADDR (TOP_S_BASE_ADDR + 0x010)
#define TOP_S__INT_EN0_CLR__ADDR (TOP_S_BASE_ADDR + 0x014)
#define TOP_S__INT_EN1_SET__ADDR (TOP_S_BASE_ADDR + 0x018)
#define TOP_S__INT_EN1_CLR__ADDR (TOP_S_BASE_ADDR + 0x01C)
#define TOP_S__INT_MSK0_SET__ADDR (TOP_S_BASE_ADDR + 0x020)
#define TOP_S__INT_MSK0_CLR__ADDR (TOP_S_BASE_ADDR + 0x024)
#define TOP_S__INT_MSK1_SET__ADDR (TOP_S_BASE_ADDR + 0x028)
#define TOP_S__INT_MSK1_CLR__ADDR (TOP_S_BASE_ADDR + 0x02C)
#define TOP_S__INT_CLR0__ADDR (TOP_S_BASE_ADDR + 0x030)
#define TOP_S__INT_CLR1__ADDR (TOP_S_BASE_ADDR + 0x034)
#define TOP_S__INT_STATE0__ADDR (TOP_S_BASE_ADDR + 0x040)
#define TOP_S__INT_STATE1__ADDR (TOP_S_BASE_ADDR + 0x044)
#define TOP_S__RAW_HIS0__ADDR (TOP_S_BASE_ADDR + 0x048)
#define TOP_S__RAW_HIS1__ADDR (TOP_S_BASE_ADDR + 0x04C)
#define TOP_S__SW_PMU_SHACK__ADDR (TOP_S_BASE_ADDR + 0x080)
#define TOP_S__PMU_STAT__ADDR (TOP_S_BASE_ADDR + 0x084)
#define TOP_S__CFG_CTRL2_DMAC_AXI__ADDR (TOP_S_BASE_ADDR + 0x0A0)
#define TOP_S__JTAG_CTRL__ADDR (TOP_S_BASE_ADDR + 0x0A8)
#define TOP_S__PROTEN_SET__ADDR (TOP_S_BASE_ADDR + 0x0B0)
#define TOP_S__PROTEN_CLR__ADDR (TOP_S_BASE_ADDR + 0x0B4)
#define TOP_S__GROUPID_CSU__ADDR (TOP_S_BASE_ADDR + 0x100)
#define TOP_S__GROUPID_RPU__ADDR (TOP_S_BASE_ADDR + 0x104)
#define TOP_S__GROUPID_APU__ADDR (TOP_S_BASE_ADDR + 0x108)
#define TOP_S__GROUPID_NPU__ADDR (TOP_S_BASE_ADDR + 0x110)
#define TOP_S__GROUPID_JPU__ADDR (TOP_S_BASE_ADDR + 0x114)
#define TOP_S__GROUPID_DMA_AXI__ADDR (TOP_S_BASE_ADDR + 0x118)
#define TOP_S__GROUPID_HP0__ADDR (TOP_S_BASE_ADDR + 0x120)
#define TOP_S__GROUPID_HP1__ADDR (TOP_S_BASE_ADDR + 0x124)
#define TOP_S__GROUPID_GP0__ADDR (TOP_S_BASE_ADDR + 0x128)
#define TOP_S__GROUPID_GP1__ADDR (TOP_S_BASE_ADDR + 0x12C)
#define TOP_S__GROUPID_HBUS__ADDR (TOP_S_BASE_ADDR + 0x130)
#define TOP_S__CSU_ERR_CODE__ADDR (TOP_S_BASE_ADDR + 0x180)
#define TOP_S__FSBL_ERR_CODE__ADDR (TOP_S_BASE_ADDR + 0x184)
#define TOP_S__MULTI_BOOT__ADDR (TOP_S_BASE_ADDR + 0x188)
#define TOP_S__PWR_SP_THD__ADDR (TOP_S_BASE_ADDR + 0x190)
#define TOP_S__PWR_LS_THD__ADDR (TOP_S_BASE_ADDR + 0x194)
#define TOP_S__PWR_REPEAT__ADDR (TOP_S_BASE_ADDR + 0x198)
#define TOP_S__APU_DBG_AUTH__ADDR (TOP_S_BASE_ADDR + 0x1A0)
#define TOP_S__PL_PORT_AUTH__ADDR (TOP_S_BASE_ADDR + 0x1B0)
#define TOP_S__EFUSE_SRST__ADDR (TOP_S_BASE_ADDR + 0x1F0)
#define TOP_S__EFUSE_ECC__ADDR (TOP_S_BASE_ADDR + 0x1F4)
#define TOP_S__EFUSE_GLB0__ADDR (TOP_S_BASE_ADDR + 0x200)
#define TOP_S__EFUSE_GLB1__ADDR (TOP_S_BASE_ADDR + 0x204)
#define TOP_S__EFUSE_GLB2__ADDR (TOP_S_BASE_ADDR + 0x208)
#define TOP_S__EFUSE_GLB3__ADDR (TOP_S_BASE_ADDR + 0x20C)
#define TOP_S__EFUSE_GLB4__ADDR (TOP_S_BASE_ADDR + 0x210)
#define TOP_S__EFUSE_GLB5__ADDR (TOP_S_BASE_ADDR + 0x214)
#define TOP_S__EFUSE_GLB6__ADDR (TOP_S_BASE_ADDR + 0x218)
#define TOP_S__EFUSE_GLB7__ADDR (TOP_S_BASE_ADDR + 0x21C)
#define TOP_S__EFUSE_GLB8__ADDR (TOP_S_BASE_ADDR + 0x220)
#define TOP_S__EFUSE_GLB9__ADDR (TOP_S_BASE_ADDR + 0x224)
#define TOP_S__EFUSE_GLB10__ADDR (TOP_S_BASE_ADDR + 0x228)
#define TOP_S__EFUSE_GLB11__ADDR (TOP_S_BASE_ADDR + 0x22C)
#define TOP_S__EFUSE_GLB12__ADDR (TOP_S_BASE_ADDR + 0x230)
#define TOP_S__EFUSE_GLB13__ADDR (TOP_S_BASE_ADDR + 0x234)
#define TOP_S__EFUSE_GLB14__ADDR (TOP_S_BASE_ADDR + 0x238)
#define TOP_S__EFUSE_GLB15__ADDR (TOP_S_BASE_ADDR + 0x23C)
#define TOP_S__EFUSE_GLB16__ADDR (TOP_S_BASE_ADDR + 0x240)
#define TOP_S__EFUSE_GLB17__ADDR (TOP_S_BASE_ADDR + 0x244)
#define TOP_S__EFUSE_GLB18__ADDR (TOP_S_BASE_ADDR + 0x248)
#define TOP_S__RWL0_RESERVED__ADDR (TOP_S_BASE_ADDR + 0x280)
#define TOP_S__RWL1_RESERVED__ADDR (TOP_S_BASE_ADDR + 0x284)
#define TOP_S__RWL2_RESERVED__ADDR (TOP_S_BASE_ADDR + 0x288)
#define TOP_S__RWL3_RESERVED__ADDR (TOP_S_BASE_ADDR + 0x28C)
#define TOP_S__RWL4_RESERVED__ADDR (TOP_S_BASE_ADDR + 0x290)
#define TOP_S__RWH0_RESERVED__ADDR (TOP_S_BASE_ADDR + 0x294)
#define TOP_S__RWH1_RESERVED__ADDR (TOP_S_BASE_ADDR + 0x298)
#define TOP_S__RO_RESERVED__ADDR (TOP_S_BASE_ADDR + 0x29C)
#define TOP_S__RWL0_RESERVED_E__ADDR (TOP_S_BASE_ADDR + 0x2A0)
#define TOP_S__RWL1_RESERVED_E__ADDR (TOP_S_BASE_ADDR + 0x2A4)
#define TOP_S__RWL2_RESERVED_E__ADDR (TOP_S_BASE_ADDR + 0x2A8)
#define TOP_S__RWL3_RESERVED_E__ADDR (TOP_S_BASE_ADDR + 0x2AC)
#define TOP_S__RWL4_RESERVED_E__ADDR (TOP_S_BASE_ADDR + 0x2B0)
#define TOP_S__RWH0_RESERVED_E__ADDR (TOP_S_BASE_ADDR + 0x2B4)
#define TOP_S__RWH1_RESERVED_E__ADDR (TOP_S_BASE_ADDR + 0x2B8)
#define TOP_S__RO_RESERVED_E__ADDR (TOP_S_BASE_ADDR + 0x2BC)
#define TOP_S__CFG_CTRL_APU0_RESET_VECTOR_H__ADDR (TOP_S_BASE_ADDR + 0x300)
#define TOP_S__CFG_CTRL_APU0_RESET_VECTOR_L__ADDR (TOP_S_BASE_ADDR + 0x304)
#define TOP_S__CFG_CTRL_APU1_RESET_VECTOR_H__ADDR (TOP_S_BASE_ADDR + 0x308)
#define TOP_S__CFG_CTRL_APU1_RESET_VECTOR_L__ADDR (TOP_S_BASE_ADDR + 0x30c)
#define TOP_S__CFG_CTRL_RPU_RESET_VECTOR_H__ADDR (TOP_S_BASE_ADDR + 0x320)
#define TOP_S__CFG_CTRL_RPU_RESET_VECTOR_L__ADDR (TOP_S_BASE_ADDR + 0x324)
#define TOP_S__GLOBAL_SRST_N__ADDR (TOP_S_BASE_ADDR + 0x330)
#define TOP_S__XPU_SRST__ADDR (TOP_S_BASE_ADDR + 0x334)

typedef union {
  struct {
    u32 apu_core_pulse_srst_n : 2;
    u32 reserved_3_2 : 2;
    u32 apu_core_srst_n : 2;
    u32 reserved_7_6 : 2;
    u32 rpu_pulse_srst_n : 1;
    u32 reserved_9 : 1;
    u32 rpu_srst_n : 1;
    u32 reserved_31_11 : 21;
  };
  u32 reg;
} TOP_S__XPU_SRST__ACC_T;

typedef union {
  struct {
    u32 glb_srst_n : 1;
    u32 reserved_3_1 : 3;
    u32 fclk_srst_n : 4;
    u32 glb_pl_srst_n : 1;
    u32 ps_rst_only : 1;
    u32 reserved_31_10 : 31;
  };
  u32 reg;
} TOP_S__GLOBAL_SRST_N__ACC_T;

typedef union {
  struct {
    u32 rpu_reset_vector_l : 32;
  };
  u32 reg;
} TOP_S__CFG_CTRL_RPU_RESET_VECTOR_L__ACC_T;

typedef union {
  struct {
    u32 rpu_reset_vector_h : 32;
  };
  u32 reg;
} TOP_S__CFG_CTRL_RPU_RESET_VECTOR_H__ACC_T;

typedef union {
  struct {
    u32 rvbaraddr1_l : 32;
  };
  u32 reg;
} TOP_S__CFG_CTRL_APU1_RESET_VECTOR_L__ACC_T;

typedef union {
  struct {
    u32 rvbaraddr1_h : 6;
    u32 reserved_31_6 : 26;
  };
  u32 reg;
} TOP_S__CFG_CTRL_APU1_RESET_VECTOR_H__ACC_T;

typedef union {
  struct {
    u32 rvbaraddr0_l : 32;
  };
  u32 reg;
} TOP_S__CFG_CTRL_APU0_RESET_VECTOR_L__ACC_T;

typedef union {
  struct {
    u32 rvbaraddr0_h : 6;
    u32 reserved_31_6 : 26;
  };
  u32 reg;
} TOP_S__CFG_CTRL_APU0_RESET_VECTOR_H__ACC_T;

typedef union {
  struct {
    u32 ro_reserved_e : 32;
  };
  u32 reg;
} TOP_S__RO_RESERVED_E__ACC_T;

typedef union {
  struct {
    u32 rwh1_reserved_e : 32;
  };
  u32 reg;
} TOP_S__RWH1_RESERVED_E__ACC_T;

typedef union {
  struct {
    u32 rwh0_reserved_e : 32;
  };
  u32 reg;
} TOP_S__RWH0_RESERVED_E__ACC_T;

typedef union {
  struct {
    u32 rwl4_reserved_e : 32;
  };
  u32 reg;
} TOP_S__RWL4_RESERVED_E__ACC_T;

typedef union {
  struct {
    u32 rwl3_reserved_e : 32;
  };
  u32 reg;
} TOP_S__RWL3_RESERVED_E__ACC_T;

typedef union {
  struct {
    u32 rwl2_reserved_e : 32;
  };
  u32 reg;
} TOP_S__RWL2_RESERVED_E__ACC_T;

typedef union {
  struct {
    u32 rwl1_reserved_e : 32;
  };
  u32 reg;
} TOP_S__RWL1_RESERVED_E__ACC_T;

typedef union {
  struct {
    u32 rwl0_reserved_e : 32;
  };
  u32 reg;
} TOP_S__RWL0_RESERVED_E__ACC_T;

typedef union {
  struct {
    u32 ro_reserved : 32;
  };
  u32 reg;
} TOP_S__RO_RESERVED__ACC_T;

typedef union {
  struct {
    u32 rwh1_reserved : 32;
  };
  u32 reg;
} TOP_S__RWH1_RESERVED__ACC_T;

typedef union {
  struct {
    u32 rwh0_reserved : 32;
  };
  u32 reg;
} TOP_S__RWH0_RESERVED__ACC_T;

typedef union {
  struct {
    u32 rwl4_reserved : 32;
  };
  u32 reg;
} TOP_S__RWL4_RESERVED__ACC_T;

typedef union {
  struct {
    u32 rwl3_reserved : 32;
  };
  u32 reg;
} TOP_S__RWL3_RESERVED__ACC_T;

typedef union {
  struct {
    u32 rwl2_reserved : 32;
  };
  u32 reg;
} TOP_S__RWL2_RESERVED__ACC_T;

typedef union {
  struct {
    u32 rwl1_reserved : 32;
  };
  u32 reg;
} TOP_S__RWL1_RESERVED__ACC_T;

typedef union {
  struct {
    u32 rwl0_reserved : 32;
  };
  u32 reg;
} TOP_S__RWL0_RESERVED__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_18 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB18__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_17 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB17__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_16 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB16__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_15 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB15__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_14 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB14__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_13 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB13__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_12 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB12__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_11 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB11__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_10 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB10__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_9 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB9__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_8 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB8__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_7 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB7__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_6 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB6__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_5 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB5__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_4 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB4__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_3 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB3__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_2 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB2__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_1 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB1__ACC_T;

typedef union {
  struct {
    u32 efuse_state__s_0 : 32;
  };
  u32 reg;
} TOP_S__EFUSE_GLB0__ACC_T;

typedef union {
  struct {
    u32 ecc_info_efuse_0 : 1;
    u32 ecc_info_efuse_1 : 1;
    u32 ecc_info_efuse_2 : 1;
    u32 ecc_info_efuse_3 : 1;
    u32 ecc_info_efuse_4 : 1;
    u32 ecc_info_efuse_5 : 1;
    u32 ecc_info_efuse_6 : 1;
    u32 ecc_info_efuse_7 : 1;
    u32 ecc_info_efuse_8 : 1;
    u32 ecc_info_efuse_9 : 1;
    u32 ecc_info_efuse_10 : 1;
    u32 ecc_info_efuse_11 : 1;
    u32 ecc_info_efuse_12 : 1;
    u32 ecc_info_efuse_13 : 1;
    u32 ecc_info_efuse_14 : 1;
    u32 ecc_info_efuse_15 : 1;
    u32 ecc_info_efuse_16 : 1;
    u32 ecc_info_efuse_17 : 1;
    u32 ecc_info_efuse_18 : 1;
    u32 ecc_info_efuse_19 : 1;
    u32 ecc_info_efuse_20 : 1;
    u32 ecc_info_efuse_21 : 1;
    u32 ecc_info_efuse_22 : 1;
    u32 ecc_info_efuse_23 : 1;
    u32 ecc_info_efuse_24 : 1;
    u32 ecc_info_efuse_25 : 1;
    u32 ecc_info_efuse_26 : 1;
    u32 ecc_info_efuse_27 : 1;
    u32 ecc_info_efuse_28 : 1;
    u32 ecc_info_efuse_29 : 1;
    u32 ecc_info_efuse_30 : 1;
    u32 ecc_info_efuse_31 : 1;
  };
  u32 reg;
} TOP_S__EFUSE_ECC__ACC_T;

typedef union {
  struct {
    u32 efuse_ctrl_srst_n : 1;
    u32 reserved_31_1 : 31;
  };
  u32 reg;
} TOP_S__EFUSE_SRST__ACC_T;

typedef union {
  struct {
    u32 pl2ps_gp0_prot : 1;
    u32 pl2ps_gp1_prot : 1;
    u32 reserved_3_2 : 2;
    u32 pl2ps_hp0_prot : 1;
    u32 pl2ps_hp1_prot : 1;
    u32 reserved_7_6 : 2;
    u32 pl2ps_acp_prot : 1;
    u32 reserved_31_9 : 23;
  };
  u32 reg;
} TOP_S__PL_PORT_AUTH__ACC_T;

typedef union {
  struct {
    u32 dbgen : 2;
    u32 reserved_3_2 : 2;
    u32 niden : 2;
    u32 reserved_7_6 : 2;
    u32 spiden : 2;
    u32 reserved_11_10 : 2;
    u32 spniden : 2;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__APU_DBG_AUTH__ACC_T;

typedef union {
  struct {
    u32 repeat_time : 4;
    u32 reserved_31_4 : 28;
  };
  u32 reg;
} TOP_S__PWR_REPEAT__ACC_T;

typedef union {
  struct {
    u32 short_thd : 16;
    u32 long_thd : 16;
  };
  u32 reg;
} TOP_S__PWR_LS_THD__ACC_T;

typedef union {
  struct {
    u32 pasue_thd : 16;
    u32 start_thd : 16;
  };
  u32 reg;
} TOP_S__PWR_SP_THD__ACC_T;

typedef union {
  struct {
    u32 multi_boot : 32;
  };
  u32 reg;
} TOP_S__MULTI_BOOT__ACC_T;

typedef union {
  struct {
    u32 fsbl_error_code : 32;
  };
  u32 reg;
} TOP_S__FSBL_ERR_CODE__ACC_T;

typedef union {
  struct {
    u32 csu_error_code : 32;
  };
  u32 reg;
} TOP_S__CSU_ERR_CODE__ACC_T;

typedef union {
  struct {
    u32 grpid_hbus : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_HBUS__ACC_T;

typedef union {
  struct {
    u32 grpid_gp1 : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_GP1__ACC_T;

typedef union {
  struct {
    u32 grpid_gp0 : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_GP0__ACC_T;

typedef union {
  struct {
    u32 grpid_hp1 : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_HP1__ACC_T;

typedef union {
  struct {
    u32 grpid_hp0 : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_HP0__ACC_T;

typedef union {
  struct {
    u32 grpid_dma_axi : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_DMA_AXI__ACC_T;

typedef union {
  struct {
    u32 grpid_jpu : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_JPU__ACC_T;

typedef union {
  struct {
    u32 grpid_npu : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_NPU__ACC_T;

typedef union {
  struct {
    u32 grpid_apu : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_APU__ACC_T;

typedef union {
  struct {
    u32 grpid_rpu : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_RPU__ACC_T;

typedef union {
  struct {
    u32 grpid_csu : 5;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} TOP_S__GROUPID_CSU__ACC_T;

typedef union {
  struct {
    u32 prot_en_mpu_ddrs0 : 1;
    u32 prot_en_mpu_ddrs1 : 1;
    u32 prot_en_mpu_hpm0 : 1;
    u32 prot_en_mpu_hpm1 : 1;
    u32 prot_en_mpu_ocms2 : 1;
    u32 prot_en_mpu_npu : 1;
    u32 prot_en_mpu_apu : 1;
    u32 prot_en_ddr : 1;
    u32 reserved_31_8 : 24;
  };
  u32 reg;
} TOP_S__PROTEN_CLR__ACC_T;

typedef union {
  struct {
    u32 prot_en_mpu_ddrs0 : 1;
    u32 prot_en_mpu_ddrs1 : 1;
    u32 prot_en_mpu_hpm0 : 1;
    u32 prot_en_mpu_hpm1 : 1;
    u32 prot_en_mpu_ocms2 : 1;
    u32 prot_en_mpu_npu : 1;
    u32 prot_en_mpu_apu : 1;
    u32 prot_en_ddr : 1;
    u32 reserved_31_8 : 24;
  };
  u32 reg;
} TOP_S__PROTEN_SET__ACC_T;

typedef union {
  struct {
    u32 ps_tap_off : 1;
    u32 pl_tap_off : 1;
    u32 rv_tap_off : 1;
    u32 arm_dap_off : 1;
    u32 pjtag_en : 1;
    u32 pjtag_cpu_sel : 1;
    u32 reserved_31_6 : 26;
  };
  u32 reg;
} TOP_S__JTAG_CTRL__ACC_T;

typedef union {
  struct {
    u32 ctrl2_dmac_axi : 32;
  };
  u32 reg;
} TOP_S__CFG_CTRL2_DMAC_AXI__ACC_T;

typedef union {
  struct {
    u32 pmu_fsm : 2;
    u32 reserved_31_14 : 30;
  };
  u32 reg;
} TOP_S__PMU_STAT__ACC_T;

typedef union {
  struct {
    u32 proc_done : 1;
    u32 fiq_ack : 1;
    u32 hdrstcnt_clr : 1;
    u32 reserved_31_3 : 29;
  };
  u32 reg;
} TOP_S__SW_PMU_SHACK__ACC_T;

typedef union {
  struct {
    u32 err_pll0_lock : 1;
    u32 err_pll1_lock : 1;
    u32 err_pll2_lock : 1;
    u32 reserved_7_3 : 5;
    u32 err_csu : 1;
    u32 reserved_12_9 : 4;
    u32 err_csu_boot : 1;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__RAW_HIS1__ACC_T;

typedef union {
  struct {
    u32 err_ddr_ecc : 1;
    u32 err_ocm_ecc : 1;
    u32 err_rpu_fatal : 1;
    u32 err_rpu_norm : 1;
    u32 err_temp_ov : 1;
    u32 reserved_7_5 : 3;
    u32 err_wdt0_ot : 1;
    u32 err_wdt1_ot : 1;
    u32 err_wdt2_ot : 1;
    u32 err_vcc : 1;
    u32 reserved_17_12 : 6;
    u32 err_clk_detec : 1;
    u32 reserved_19 : 1;
    u32 err_bus_timeout : 1;
    u32 reserved_23_21 : 3;
    u32 err_pl0 : 1;
    u32 err_pl1 : 1;
    u32 err_pl2 : 1;
    u32 err_pl3 : 1;
    u32 reserved_31_28 : 4;
  };
  u32 reg;
} TOP_S__RAW_HIS0__ACC_T;

typedef union {
  struct {
    u32 int_pll0_lock : 1;
    u32 int_pll1_lock : 1;
    u32 int_pll2_lock : 1;
    u32 reserved_7_3 : 5;
    u32 int_csu : 1;
    u32 reserved_12_9 : 4;
    u32 int_csu_boot : 1;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__INT_STATE1__ACC_T;

typedef union {
  struct {
    u32 int_ddr_ecc : 1;
    u32 int_ocm_ecc : 1;
    u32 int_rpu_fatal : 1;
    u32 int_rpu_norm : 1;
    u32 int_temp_ov : 1;
    u32 reserved_7_5 : 3;
    u32 int_wdt0_ot : 1;
    u32 int_wdt1_ot : 1;
    u32 int_wdt2_ot : 1;
    u32 int_vcc : 1;
    u32 reserved_17_12 : 6;
    u32 int_clk_detec : 1;
    u32 reserved_19 : 1;
    u32 int_bus_timeout : 1;
    u32 reserved_23_21 : 3;
    u32 int_pl0 : 1;
    u32 int_pl1 : 1;
    u32 int_pl2 : 1;
    u32 int_pl3 : 1;
    u32 reserved_31_28 : 4;
  };
  u32 reg;
} TOP_S__INT_STATE0__ACC_T;

typedef union {
  struct {
    u32 int_clr_pll0_lock : 1;
    u32 int_clr_pll1_lock : 1;
    u32 int_clr_pll2_lock : 1;
    u32 reserved_7_3 : 5;
    u32 int_clr_csu : 1;
    u32 reserved_12_9 : 4;
    u32 int_clr_csu_boot : 1;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__INT_CLR1__ACC_T;

typedef union {
  struct {
    u32 int_clr_ddr_ecc : 1;
    u32 int_clr_ocm_ecc : 1;
    u32 int_clr_rpu_fatal : 1;
    u32 int_clr_rpu_norm : 1;
    u32 int_clr_temp_ov : 1;
    u32 reserved_7_5 : 3;
    u32 int_clr_wdt0_ot : 1;
    u32 int_clr_wdt1_ot : 1;
    u32 int_clr_wdt2_ot : 1;
    u32 int_clr_vcc : 1;
    u32 reserved_17_12 : 6;
    u32 int_clr_clk_detec : 1;
    u32 reserved_19 : 1;
    u32 int_clr_bus_timeout : 1;
    u32 reserved_23_21 : 3;
    u32 int_clr_pl0 : 1;
    u32 int_clr_pl1 : 1;
    u32 int_clr_pl2 : 1;
    u32 int_clr_pl3 : 1;
    u32 reserved_31_28 : 4;
  };
  u32 reg;
} TOP_S__INT_CLR0__ACC_T;

typedef union {
  struct {
    u32 int_msk_pll0_lock : 1;
    u32 int_msk_pll1_lock : 1;
    u32 int_msk_pll2_lock : 1;
    u32 reserved_7_3 : 5;
    u32 int_msk_csu : 1;
    u32 reserved_12_9 : 4;
    u32 int_msk_csu_boot : 1;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__INT_MSK1_CLR__ACC_T;

typedef union {
  struct {
    u32 int_msk_pll0_lock : 1;
    u32 int_msk_pll1_lock : 1;
    u32 int_msk_pll2_lock : 1;
    u32 reserved_7_3 : 5;
    u32 int_msk_csu : 1;
    u32 reserved_12_9 : 4;
    u32 int_msk_csu_boot : 1;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__INT_MSK1_SET__ACC_T;

typedef union {
  struct {
    u32 int_msk_ddr_ecc : 1;
    u32 int_msk_ocm_ecc : 1;
    u32 int_msk_rpu_fatal : 1;
    u32 int_msk_rpu_norm : 1;
    u32 int_msk_temp_ov : 1;
    u32 reserved_7_5 : 3;
    u32 int_msk_wdt0_ot : 1;
    u32 int_msk_wdt1_ot : 1;
    u32 int_msk_wdt2_ot : 1;
    u32 int_msk_vcc : 1;
    u32 reserved_17_12 : 6;
    u32 int_msk_clk_detec : 1;
    u32 reserved_19 : 1;
    u32 int_msk_bus_timeout : 1;
    u32 reserved_23_21 : 3;
    u32 int_msk_pl0 : 1;
    u32 int_msk_pl1 : 1;
    u32 int_msk_pl2 : 1;
    u32 int_msk_pl3 : 1;
    u32 reserved_31_28 : 4;
  };
  u32 reg;
} TOP_S__INT_MSK0_CLR__ACC_T;

typedef union {
  struct {
    u32 int_msk_ddr_ecc : 1;
    u32 int_msk_ocm_ecc : 1;
    u32 int_msk_rpu_fatal : 1;
    u32 int_msk_rpu_norm : 1;
    u32 int_msk_temp_ov : 1;
    u32 reserved_7_5 : 3;
    u32 int_msk_wdt0_ot : 1;
    u32 int_msk_wdt1_ot : 1;
    u32 int_msk_wdt2_ot : 1;
    u32 int_msk_vcc : 1;
    u32 reserved_17_12 : 6;
    u32 int_msk_clk_detec : 1;
    u32 reserved_19 : 1;
    u32 int_msk_bus_timeout : 1;
    u32 reserved_23_21 : 3;
    u32 int_msk_pl0 : 1;
    u32 int_msk_pl1 : 1;
    u32 int_msk_pl2 : 1;
    u32 int_msk_pl3 : 1;
    u32 reserved_31_28 : 4;
  };
  u32 reg;
} TOP_S__INT_MSK0_SET__ACC_T;

typedef union {
  struct {
    u32 int_en_pll0_lock : 1;
    u32 int_en_pll1_lock : 1;
    u32 int_en_pll2_lock : 1;
    u32 reserved_7_3 : 5;
    u32 int_en_csu : 1;
    u32 reserved_12_9 : 4;
    u32 int_en_csu_boot : 1;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__INT_EN1_CLR__ACC_T;

typedef union {
  struct {
    u32 int_en_pll0_lock : 1;
    u32 int_en_pll1_lock : 1;
    u32 int_en_pll2_lock : 1;
    u32 reserved_7_3 : 5;
    u32 int_en_csu : 1;
    u32 reserved_12_9 : 4;
    u32 int_en_csu_boot : 1;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__INT_EN1_SET__ACC_T;

typedef union {
  struct {
    u32 int_en_ddr_ecc : 1;
    u32 int_en_ocm_ecc : 1;
    u32 int_en_rpu_fatal : 1;
    u32 int_en_rpu_norm : 1;
    u32 int_en_temp_ov : 1;
    u32 reserved_7_5 : 3;
    u32 int_en_wdt0_ot : 1;
    u32 int_en_wdt1_ot : 1;
    u32 int_en_wdt2_ot : 1;
    u32 int_en_vcc : 1;
    u32 reserved_17_12 : 6;
    u32 int_en_clk_detec : 1;
    u32 reserved_19 : 1;
    u32 int_en_bus_timeout : 1;
    u32 reserved_23_21 : 3;
    u32 int_en_pl0 : 1;
    u32 int_en_pl1 : 1;
    u32 int_en_pl2 : 1;
    u32 int_en_pl3 : 1;
    u32 reserved_31_28 : 4;
  };
  u32 reg;
} TOP_S__INT_EN0_CLR__ACC_T;

typedef union {
  struct {
    u32 int_en_ddr_ecc : 1;
    u32 int_en_ocm_ecc : 1;
    u32 int_en_rpu_fatal : 1;
    u32 int_en_rpu_norm : 1;
    u32 int_en_temp_ov : 1;
    u32 reserved_7_5 : 3;
    u32 int_en_wdt0_ot : 1;
    u32 int_en_wdt1_ot : 1;
    u32 int_en_wdt2_ot : 1;
    u32 int_en_vcc : 1;
    u32 reserved_17_12 : 6;
    u32 int_en_clk_detec : 1;
    u32 reserved_19 : 1;
    u32 int_en_bus_timeout : 1;
    u32 reserved_23_21 : 3;
    u32 int_en_pl0 : 1;
    u32 int_en_pl1 : 1;
    u32 int_en_pl2 : 1;
    u32 int_en_pl3 : 1;
    u32 reserved_31_28 : 4;
  };
  u32 reg;
} TOP_S__INT_EN0_SET__ACC_T;

typedef union {
  struct {
    u32 err_hw_en_pll0_lock : 1;
    u32 err_hw_en_pll1_lock : 1;
    u32 err_hw_en_pll2_lock : 1;
    u32 reserved_7_3 : 5;
    u32 err_hw_en_csu : 1;
    u32 reserved_12_9 : 4;
    u32 err_hw_en_csu_boot : 1;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__ERR_HW_EN1_CLR__ACC_T;

typedef union {
  struct {
    u32 err_hw_en_pll0_lock : 1;
    u32 err_hw_en_pll1_lock : 1;
    u32 err_hw_en_pll2_lock : 1;
    u32 reserved_7_3 : 5;
    u32 err_hw_en_csu : 1;
    u32 reserved_12_9 : 4;
    u32 err_hw_en_csu_boot : 1;
    u32 reserved_31_14 : 18;
  };
  u32 reg;
} TOP_S__ERR_HW_EN1_SET__ACC_T;

typedef union {
  struct {
    u32 err_hw_en_ddr_ecc : 1;
    u32 err_hw_en_ocm_ecc : 1;
    u32 err_hw_en_rpu_fatal : 1;
    u32 err_hw_en_rpu_norm : 1;
    u32 err_hw_en_temp_ov : 1;
    u32 reserved_7_5 : 3;
    u32 err_hw_en_wdt0_ot : 1;
    u32 err_hw_en_wdt1_ot : 1;
    u32 err_hw_en_wdt2_ot : 1;
    u32 err_hw_en_vcc : 1;
    u32 reserved_17_12 : 6;
    u32 err_hw_en_clk_detec : 1;
    u32 reserved_19 : 1;
    u32 err_hw_en_bus_timeout : 1;
    u32 reserved_23_21 : 3;
    u32 err_hw_en_pl0 : 1;
    u32 err_hw_en_pl1 : 1;
    u32 err_hw_en_pl2 : 1;
    u32 err_hw_en_pl3 : 1;
    u32 reserved_31_28 : 4;
  };
  u32 reg;
} TOP_S__ERR_HW_EN0_CLR__ACC_T;

typedef union {
  struct {
    u32 err_hw_en_ddr_ecc : 1;
    u32 err_hw_en_ocm_ecc : 1;
    u32 err_hw_en_rpu_fatal : 1;
    u32 err_hw_en_rpu_norm : 1;
    u32 err_hw_en_temp_ov : 1;
    u32 reserved_7_5 : 3;
    u32 err_hw_en_wdt0_ot : 1;
    u32 err_hw_en_wdt1_ot : 1;
    u32 err_hw_en_wdt2_ot : 1;
    u32 err_hw_en_vcc : 1;
    u32 reserved_17_12 : 6;
    u32 err_hw_en_clk_detec : 1;
    u32 reserved_19 : 1;
    u32 err_hw_en_bus_timeout : 1;
    u32 reserved_23_21 : 3;
    u32 err_hw_en_pl0 : 1;
    u32 err_hw_en_pl1 : 1;
    u32 err_hw_en_pl2 : 1;
    u32 err_hw_en_pl3 : 1;
    u32 reserved_31_28 : 4;
  };
  u32 reg;
} TOP_S__ERR_HW_EN0_SET__ACC_T;

/* CRP registers */

#define CRP__BASE_ADDR 0xF8801000
#define CRP__CLK_SEL__ADDR (CRP__BASE_ADDR + 0x040)
#define CRP__ANA_PVT__ADDR (CRP__BASE_ADDR + 0x404)
#define CRP__ANA_EFUSE__ADDR (CRP__BASE_ADDR + 0x410)
#define CRP__JTAG_MODE__ADDR (CRP__BASE_ADDR + 0x440)
#define CRP__RST_REASON__ADDR (CRP__BASE_ADDR + 0x4B0)
#define CRP__WDT_OUT_CTRL__ADDR (CRP__BASE_ADDR + 0x4B4)

typedef union {
  struct {
    u32 cpu6x4x_sel : 1;
    u32 swdtclk_sel : 1;
    u32 efuseclk_sel : 1;
    u32 canclk_sel : 1;
    u32 slow_sel : 1;
    u32 reserved_31_5 : 27;
  };
  u32 reg;
} CRP__CLK_SEL__ACC_T;

typedef union {
  struct {
    u32 wdt0_rst_outen : 1;
    u32 wdt1_rst_outen : 1;
    u32 wdt2_rst_outen : 1;
    u32 reserved_31_3 : 29;
  };
  u32 reg;
} CRP__WDT_OUT_CTRL__ACC_T;

typedef union {
  struct {
    u32 rreason_srst_b : 1;
    u32 rreason_srst_soft : 1;
    u32 rreason_jtag_apu : 1;
    u32 rreason_jtag_rpu : 1;
    u32 rreason_jtag_csu : 1;
    u32 rreason_pmu_err : 1;
    u32 rreason_swdt0_rst : 1;
    u32 rreason_swdt1_rst : 1;
    u32 rreason_swdt2_rst : 1;
    u32 reserved_31_9 : 23;
  };
  u32 reg;
} CRP__RST_REASON__ACC_T;

typedef union {
  struct {
    u32 jtag_chain_sel : 2;
    u32 reserved_3_2 : 2;
    u32 jtag_chain_mode : 1;
    u32 jtag_mio_sel : 1;
    u32 reserved_31_6 : 26;
  };
  u32 reg;
} CRP__JTAG__ACC_T;

typedef union {
  struct {
    u32 efuse_wraddr : 11;
    u32 reserved_15_11 : 5;
    u32 efuse_mr : 1;
    u32 efuse_wr_trig : 1;
    u32 reserved_19_18 : 2;
    u32 efuse_ready : 1;
    u32 reserved_31_21 : 11;
  };
  u32 reg;
} CRP__ANA_EFUSE__ACC_T;

typedef union {
  struct {
    u32 pvt_rdata : 12;
    u32 reserved_15_12 : 4;
    u32 pvt_xadc_ena : 1;
    u32 pvt_xadc_tvs_measure_sel : 1;
    u32 pvt_xadc_tvs_wakeup : 1;
    u32 reserved_19 : 1;
    u32 pvt_xadc_trim : 5;
    u32 reserved_27_25 : 3;
    u32 pvt_xadc_tvs_ovsamp_sel : 2;
    u32 reserved_31_30 : 2;
  };
  u32 reg;
} CRP__ANA_PVT__ACC_T;

typedef union {
  struct {
    unsigned reserved_7_0 : 5;
    unsigned dgbcs_srst_n : 1;
    unsigned l2c_srst_n : 1;
    unsigned presetdbg_srst_n : 1;
    unsigned apu_acp_srst_n : 1;
    unsigned gic_srst_n : 1;
    unsigned reserved_31_9 : 22;
  };
  unsigned reg;
} CRP__SRST_CTRL0__ACC_T;
#define CRP__SRST_CTRL0__ADDR (CRP__BASE_ADDR + 0x070ULL)

typedef union {
  struct {
    unsigned reserved_3_0 : 4;
    unsigned ocm_srst_n : 1;
    unsigned qspi_srst_n : 1;
    unsigned smc_srst_n : 1;
    unsigned wdt_srst_n : 1;
    unsigned dmac_axi_srst_n : 1;
    unsigned dmac_ahb_srst_n : 1;
    unsigned reserved_11_10 : 2;
    unsigned npu_srst_n : 1;
    unsigned jpu_srst_n : 1;
    unsigned ddrbus_srst_n : 1;
    unsigned reserved_31_15 : 17;
  };
  unsigned reg;
} CRP__SRST_CTRL1__ACC_T;
#define CRP__SRST_CTRL1__ADDR (CRP__BASE_ADDR + 0x074ULL)

typedef union {
  struct {
    unsigned nic_hp0_srst_n : 1;
    unsigned nic_hpl_srst_n : 1;
    unsigned reserved_3_2 : 2;
    unsigned nic_gp0m_srst_n : 1;
    unsigned nic_gplm_srst_n : 1;
    unsigned reserved_7_6 : 2;
    unsigned gpio_srst_n : 1;
    unsigned reserved_11_9 : 3;
    unsigned ipc_srst_n : 1;
    unsigned reserved_31_13 : 19;
  };
  unsigned reg;
} CRP__SRST_CTRL2__ACC_T;
#define CRP__SRST_CTRL2__ADDR (CRP__BASE_ADDR + 0x078ULL)

typedef union {
  struct {
    unsigned usb0_srst_n : 1;
    unsigned usbl_srst_n : 1;
    unsigned reserved_3_2 : 2;
    unsigned gbe0_srst_n : 1;
    unsigned gbel_srst_n : 1;
    unsigned reserved_7_6 : 2;
    unsigned sdio0_srst_n : 1;
    unsigned sdiol_srst_n : 1;
    unsigned reserved_11_10 : 2;
    unsigned uart0_srst_n : 1;
    unsigned uart1_srst_n : 1;
    unsigned reserved_15_14 : 2;
    unsigned spi0_srst_n : 1;
    unsigned spil_srst_n : 1;
    unsigned reserved_19_18 : 2;
    unsigned cano_srst_n : 1;
    unsigned canl_srst_n : 1;
    unsigned reserved_23_22 : 2;
    unsigned ttc0_srst_n : 1;
    unsigned ttcl_srst_n : 1;
    unsigned reserved_27_26 : 2;
    unsigned i2c0_srst_n : 1;
    unsigned i2c1_srst_n : 1;
    unsigned reserved_31_30 : 2;
  };
  unsigned reg;
} CRP__SRST_CTRL3__ACC_T;
#define CRP__SRST_CTRL3__ADDR (CRP__BASE_ADDR + 0x07CULL)

#endif /* AL9000_REGISTERS_H */
