#ifndef __SDIO_TEST_H
#define __SDIO_TEST_H

#include <stdio.h>
#include "nuclei_sdk_soc.h"

#define XST_SUCCESS                     0L
#define XST_FAILURE                     1L
#define     __IO    volatile            

#define REG_READ(reg_address) reg_read(reg_address)
extern unsigned reg_read(unsigned long long reg_address);
#define REG_WRITE(reg_address, reg_wdata) reg_write(reg_address, reg_wdata)

#define SDRegWrite(reg_address, reg_wdata) REG_WRITE((SDIO_WRAP__SDIO1__BASE_ADDR+reg_address), reg_wdata)
#define EMMCRegWrite(reg_address, reg_wdata) REG_WRITE((SDIO_WRAP__SDIO0__BASE_ADDR+reg_address), reg_wdata)
extern void reg_write(unsigned long long reg_address, unsigned reg_wdata);

typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
#define  u32 uint32_t
extern u32 SD_Test(void);

typedef struct
{
	__IO uint32_t XFER_BLOCK_SIZE:12;
	__IO uint32_t SDMA_BUF_BDARY:3;
	__IO uint32_t RSVD_BLOCKSIZE15:1;
    __IO uint32_t BLOCKCOUNT_R:16;
}BLOCKCOUNT_R__BLOCKSIZE_R;

typedef union
{
    __IO uint32_t D32;
    struct BIT{
	    __IO uint32_t DMA_EN:1;
	    __IO uint32_t BLOCK_COUNT_ENABLE:1;
	    __IO uint32_t AUTO_CMD_ENABLE:2;
	    __IO uint32_t DATA_XFER_DIR:1;
	    __IO uint32_t MULTI_BLK_SEL:1;
	    __IO uint32_t RESP_TYPE:1;
	    __IO uint32_t RESP_ERR_CHK_ENABLE:1;
	    __IO uint32_t RESP_INT_DISABLE:1;
	    __IO uint32_t RSVD:7;
        __IO uint32_t RESP_TYPE_SELECT:2;
	    __IO uint32_t SUB_CMD_FLAG:1;
	    __IO uint32_t CMD_CRC_CHK_ENABLE:1;
	    __IO uint32_t CMD_IDX_CHK_ENABLE:1;
	    __IO uint32_t DATA_PRESENT_SEL:1;
	    __IO uint32_t CMD_TYPE:2;
	    __IO uint32_t CMD_INDEX:6;
	    __IO uint32_t RSVD_30_31:2;
    }BIT;
}CMD_R__XFER_MODE_R;

typedef struct
{
	__IO uint32_t CMD_INHIBIT:1;
	__IO uint32_t CMD_INHIBIT_DAT:1;
	__IO uint32_t DAT_LINE_ACTIVE:1;
	__IO uint32_t RE_TUNE_REQ:1;
	__IO uint32_t DAT_7_4_MASK:4;
	__IO uint32_t WR_XFER_ACTIVE:1;
	__IO uint32_t RD_XFER_ACTIVE:1;
	__IO uint32_t BUF_WR_ENABLE:1;
	__IO uint32_t BUF_RD_ENABLE:1;
	__IO uint32_t RSVD_15_12:4;
	__IO uint32_t CARD_INSERTED:1;
	__IO uint32_t CARD_STABLE:1;
	__IO uint32_t CARD_DETECT_PIN_LEVEL:1;
	__IO uint32_t WR_PROTECT_SW_LVL:1;
	__IO uint32_t DAT_3_0:4;
	__IO uint32_t CMD_LINE_LVL:1;
	__IO uint32_t HOST_REG_VOL:1;
	__IO uint32_t RSVD_26:1;
	__IO uint32_t CMD_ISSUE_ERR:1;
	__IO uint32_t SUB_CMD_STAT:1;
	__IO uint32_t IN_DORMANT_ST:1;
	__IO uint32_t LANE_SYNC:1;
	__IO uint32_t UHS2_IF_DETECT:1;
}PSTATE_REG_R;

typedef union
{
	__IO uint32_t D32;
	struct _BIT{
		__IO uint32_t LED_CTRL:1;
		__IO uint32_t DAT_XFER_WIDTH:1;
		__IO uint32_t HIGH_SPEED_EN:1;
		__IO uint32_t DMA_SEL:2;
		__IO uint32_t EXTDAT_XFER:1;
		__IO uint32_t CARD_DETECT_TEST_LVL:1;
		__IO uint32_t CARD_DETECT_SIG_SEL:1;
        __IO uint32_t SD_BUS_PWR_VDD1:1;
	    __IO uint32_t SD_BUS_VOL_VDD1:3;
	    __IO uint32_t SD_BUS_PWR_VDD2:1;
	    __IO uint32_t SD_BUS_VOL_VDD2:3;
        __IO uint32_t STOP_BG_REQ:1;
	    __IO uint32_t CONTINUE_REQ:1;
	    __IO uint32_t RD_WAIT_CTRL:1;
	    __IO uint32_t INT_AT_BGAP:1;
	    __IO uint32_t RSVD_23_20:4;
        __IO uint32_t CARD_INT:1;
	    __IO uint32_t CARD_INSERT:1;
	    __IO uint32_t CARD_REMOVAL:1;
	    __IO uint32_t RSVD_31_27:5;
	}BIT;
}WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R;

typedef union
{
	__IO uint32_t D32;
	struct{
	__IO uint32_t	INTERNAL_CLK_EN:1;
	__IO uint32_t	INTERNAL_CLK_STABLE:1;
	__IO uint32_t	SD_CLK_EN:1;
	__IO uint32_t	PLL_ENABLE:1;
	__IO uint32_t   RSVD_4:1;
	__IO uint32_t	CLK_GEN_SELECT:1;
	__IO uint32_t   UPPER_FREQ_SEL:2;
	__IO uint32_t   FREQ_SEL:8;
    __IO uint32_t   TOUT_CNT:4;
	__IO uint32_t   RSVD_23_20:4;
    __IO uint32_t	SW_RST_ALL:1;
	__IO uint32_t   SW_RST_CMD:1;
	__IO uint32_t	SW_RST_DAT:1;
	__IO uint32_t   RSVD_31_27:5;
    }BIT;
}SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R;


typedef union
{
    __IO uint32_t D32;
    struct{
	    __IO uint32_t	 CMD_COMPLETE:1;
	    __IO uint32_t	 XFER_COMPLETE:1;
	    __IO uint32_t	 BGAP_EVENT:1;
	    __IO uint32_t	 DMA_INTERRUPT:1;
	    __IO uint32_t    BUF_WR_READY:1;
	    __IO uint32_t	 BUF_RD_READY:1;
	    __IO uint32_t    CARD_INSERTION:1;
	    __IO uint32_t    CARD_REMOVAL:1;
	    __IO uint32_t    CARD_INTERRUPT:1;
	    __IO uint32_t    INT_A:1;
	    __IO uint32_t    INT_B:1;
	    __IO uint32_t    INT_C:1;
	    __IO uint32_t    RE_TUNE_EVENT:1;
	    __IO uint32_t    FX_EVENT:1;
	    __IO uint32_t    CQE_EVENT:1;
	    __IO uint32_t    ERR_INTERRUPT:1;
        __IO uint32_t	 CMD_TOUT_ERR:1;
	    __IO uint32_t	 CMD_CRC_ERR:1;
	    __IO uint32_t	 CMD_END_BIT_ERR:1;
	    __IO uint32_t	 CMD_IDX_ERR:1;
	    __IO uint32_t    DATA_TOUT_ERR:1;
	    __IO uint32_t	 DATA_CRC_ERR:1;
	    __IO uint32_t    DATA_END_BIT_ERR:1;
	    __IO uint32_t    CUR_LMT_ERR:1;
	    __IO uint32_t    AUTO_CMD_ERR:1;
	    __IO uint32_t    ADMA_ERR:1;
	    __IO uint32_t    TUNING_ERR:1;
	    __IO uint32_t    RESP_ERR:1;
	    __IO uint32_t    BOOT_ACK_ERR:1;
	    __IO uint32_t    VENDOR_ERR1:1;
	    __IO uint32_t    VENDOR_ERR2:1;
	    __IO uint32_t    VENDOR_ERR3:1;
    }BIT;
}ERROR_INT_STAT_R__NORMAL_INT_STAT_R;

typedef union
{
    __IO uint32_t D32;
    struct{
	    __IO uint32_t	CMD_COMPLETE_STAT_EN:1; /* intr when response received */
	    __IO uint32_t    XFER_COMPLETE_STAT_EN:1; /* intr when data read/write xfer completed */
	    __IO uint32_t    BGAP_EVENT_STAT_EN:1;
	    __IO uint32_t	DMA_INTERRUPT_STAT_EN:1;
	    __IO uint32_t	BUF_WR_READY_STAT_EN:1;
	    __IO uint32_t    BUF_RD_READY_STAT_EN:1;
	    __IO uint32_t	CARD_INSERTION_STAT_EN:1;
	    __IO uint32_t	CARD_REMOVAL_STAT_EN:1;
	    __IO uint32_t    CARD_INTERRUPT_STAT_EN:1;
	    __IO uint32_t	INT_A_STAT_EN:1;
	    __IO uint32_t	INT_B_STAT_EN:1;
	    __IO uint32_t	INT_C_STAT_EN:1;
	    __IO uint32_t	RE_TUNE_EVENT_STAT_EN:1;
	    __IO uint32_t	FX_EVENT_STAT_EN:1;
	    __IO uint32_t    CQE_EVENT_STAT_EN:1;
	    __IO uint32_t	CMD_TOUT_ERR_STAT_EN:1;
	    __IO uint32_t	CMD_CRC_ERR_STAT_EN:1;
	    __IO uint32_t	CMD_END_BIT_ERR_STAT_EN:1;
	    __IO uint32_t	CMD_IDX_ERR_STAT_EN:1;
	    __IO uint32_t    DATA_TOUT_ERR_STAT_EN:1;
	    __IO uint32_t    DATA_CRC_ERR_STAT_EN:1;
	    __IO uint32_t    DATA_END_BIT_ERR_STAT_EN:1;
	    __IO uint32_t	CUR_LMT_ERR_STAT_EN:1;
	    __IO uint32_t    AUTO_CMD_ERR_STAT_EN:1;
	    __IO uint32_t	ADMA_ERR_STAT_EN:1;
	    __IO uint32_t	TUNING_ERR_STAT_EN:1;
	    __IO uint32_t	RESP_ERR_STAT_EN:1;
	    __IO uint32_t    BOOT_ACK_ERR_STAT_EN:1;
	    __IO uint32_t    VENDOR_ERR_STAT_EN1:1;
	    __IO uint32_t	VENDOR_ERR_STAT_EN2:1;
	    __IO uint32_t	VENDOR_ERR_STAT_EN3:1;
     }BIT;
}ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R;

typedef union
{
    __IO uint32_t D32;
    struct{
	__IO uint32_t	CMD_COMPLETE_SIGNAL_EN:1; /* intr when response received */
	__IO uint32_t    XFER_COMPLETE_SIGNAL_EN:1; /* intr when data read/write xfer completed */
	__IO uint32_t    BGAP_EVENT_SIGNAL_EN:1;
	__IO uint32_t	DMA_INTERRUPT_SIGNAL_EN:1;
	__IO uint32_t	BUF_WR_READY_SIGNAL_EN:1;
	__IO uint32_t    BUF_RD_READY_SIGNAL_EN:1;
	__IO uint32_t	CARD_INSERTION_SIGNAL_EN:1;
	__IO uint32_t	CARD_REMOVAL_SIGNAL_EN:1;
	__IO uint32_t    CARD_INTERRUPT_SIGNAL_EN:1;
	__IO uint32_t	INT_A_SIGNAL_EN:1;
	__IO uint32_t	INT_B_SIGNAL_EN:1;
	__IO uint32_t	INT_C_SIGNAL_EN:1;
	__IO uint32_t	RE_TUNE_EVENT_SIGNAL_EN:1;
	__IO uint32_t	FX_EVENT_SIGNAL_EN:1;
	__IO uint32_t    CQE_EVENT_SIGNAL_EN:1;
	__IO uint32_t	CMD_TOUT_ERR_SIGNAL_EN:1;
	__IO uint32_t	CMD_CRC_ERR_SIGNAL_EN:1;
	__IO uint32_t	CMD_END_BIT_ERR_SIGNAL_EN:1;
	__IO uint32_t	CMD_IDX_ERR_SIGNAL_EN:1;
	__IO uint32_t    DATA_TOUT_ERR_SIGNAL_EN:1;
	__IO uint32_t    DATA_CRC_ERR_SIGNAL_EN:1;
	__IO uint32_t    DATA_END_BIT_ERR_SIGNAL_EN	:1;
	__IO uint32_t	CUR_LMT_ERR_SIGNAL_EN:1;
	__IO uint32_t    AUTO_CMD_ERR_SIGNAL_EN:1;
	__IO uint32_t	ADMA_ERR_SIGNAL_EN:1;
	__IO uint32_t	TUNING_ERR_SIGNAL_EN:1;
	__IO uint32_t	RESP_ERR_SIGNAL_EN:1;
	__IO uint32_t    BOOT_ACK_ERR_SIGNAL_EN:1;
	__IO uint32_t    VENDOR_ERR_SIGNAL_EN1:1;
	__IO uint32_t	VENDOR_ERR_SIGNAL_EN2:1;
	__IO uint32_t	VENDOR_ERR_SIGNAL_EN3:1;
    }BIT;
}ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R;

typedef union
{
    __IO uint32_t D32;
    struct{
	__IO uint32_t	AUTO_CMD12_NOT_EXEC:1;
	__IO uint32_t	AUTO_CMD_TOUT_ERR:1;
	__IO uint32_t	AUTO_CMD_CRC_ERR:1;
	__IO uint32_t	AUTO_CMD_EBIT_ERR:1;
	__IO uint32_t    AUTO_CMD_IDX_ERR:1;
	__IO uint32_t	AUTO_CMD_RESP_ERR:1;
	__IO uint32_t    RSVD_6:1;
	__IO uint32_t    CMD_NOT_ISSUED_AUTO_CMD12:1;
	__IO uint32_t    RSVD_15_8:8;
    __IO uint32_t	UHS_MODE_SEL:3;
	__IO uint32_t	SIGNALING_EN:1;
	__IO uint32_t	DRV_STRENGTH_SEL:2;
	__IO uint32_t	EXEC_TUNING:1;
	__IO uint32_t    SAMPLE_CLK_SEL:1;
	__IO uint32_t	UHS2_IF_ENABLE:1;
	__IO uint32_t    RSVD_9:1;
	__IO uint32_t    ADMA2_LEN_MODE:1;
	__IO uint32_t    CMD23_ENABLE:1;
	__IO uint32_t    HOST_VER4_ENABLE:1;
	__IO uint32_t    ADDRESSING:1;
	__IO uint32_t    ASYNC_INT_ENABLE:1;
	__IO uint32_t    PRESET_VAL_ENABLE:1;
    }BIT;
}HOST_CTRL2_R__AUTO_CMD_STAT_R;

typedef struct
{
	__IO uint32_t	SDR50_SUPPORT:1;
	__IO uint32_t 	SDR104_SUPPORT:1;
	__IO uint32_t 	DDR50_SUPPORT:1;
	__IO uint32_t	UHS2_SUPPORT:1;
	__IO uint32_t	DRV_TYPEA:1;
	__IO uint32_t 	DRV_TYPEC:1;
	__IO uint32_t	DRV_TYPED:1;
	__IO uint32_t	RSVD_39:1;
	__IO uint32_t 	RETUNE_CNT:4;
	__IO uint32_t	RSVD_44:1;
	__IO uint32_t	USE_TUNING_SDR50:1;
	__IO uint32_t	RE_TUNING_MODES:2;
	__IO uint32_t	CLK_MUL:8;
	__IO uint32_t    RSVD_56_58:3;
	__IO uint32_t	ADMA3_SUPPORT:1;
	__IO uint32_t	VDD2_18V_SUPPORT:1;
	__IO uint32_t	RSVD_61:1;
	__IO uint32_t	RSVD_62_63:2;
}CAPABILITIES2_R;

typedef struct
{
	__IO uint32_t TOUT_CLK_FREQ : 6;
	__IO uint32_t RSVD_6 : 1;
	__IO uint32_t TOUT_CLK_UNIT : 1;
	__IO uint32_t BASE_CLK_FREQ : 8;
	__IO uint32_t MAX_BLK_LEN : 2;
	__IO uint32_t Embedded_8_BIT : 1;
	__IO uint32_t ADMA2_SUPPORT : 1;
	__IO uint32_t RSVD_20 : 1;
	__IO uint32_t HIGH_SPEED_SUPPORT : 1;
	__IO uint32_t SDMA_SUPPORT : 1;
	__IO uint32_t SUS_RES_SUPPORT : 1;
	__IO uint32_t VOLT_33 : 1;
	__IO uint32_t VOLT_30 : 1;
	__IO uint32_t VOLT_18 : 1;
	__IO uint32_t SYS_ADDR_64_V4 : 1;
	__IO uint32_t SYS_ADDR_64_V3 : 1;
	__IO uint32_t ASYNC_INT_SUPPORT : 1;
	__IO uint32_t SLOT_TYPE_R : 1;
}CAPABILITIES1_R;

typedef struct
{
	__IO uint32_t MAX_CUR_33V : 8;
	__IO uint32_t MAX_CUR_30V : 8;
	__IO uint32_t MAX_CUR_18V : 8;
	__IO uint32_t RSVD_31_24 : 8;
}CURR_CAPABILITIES1_R;

typedef struct
{
	__IO uint32_t MAX_CUR_VDD2_18V : 8;
	__IO uint32_t RSVD_63_40 : 24;
}CURR_CAPABILITIES2_R;

typedef struct
{
	__IO uint32_t FORCE_AUTO_CMD12_NOT_EXEC : 1;
	__IO uint32_t FORCE_AUTO_CMD_TOUT_ERR : 1;
	__IO uint32_t FORCE_AUTO_CMD_CRC_ERR : 1;
	__IO uint32_t FORCE_AUTO_CMD_EBIT_ERR : 1;
	__IO uint32_t FORCE_AUTO_CMD_IDX_ERR : 1;
	__IO uint32_t FORCE_AUTO_CMD_RESP_ERR : 1;
	__IO uint32_t RSVD_6 : 1;
	__IO uint32_t FORCE_CMD_NOT_ISSUED_AUTO_CMD12 : 1;
	__IO uint32_t RSVD_15_8 : 8;
    __IO uint32_t FORCE_CMD_TOUT_ERR : 1;
	__IO uint32_t FORCE_CMD_CRC_ERR : 1;
	__IO uint32_t FORCE_CMD_END_BIT_ERR : 1;
	__IO uint32_t FORCE_CMD_IDX_ERR : 1;
	__IO uint32_t FORCE_DATA_TOUT_ERR : 1;
	__IO uint32_t FORCE_DATA_CRC_ERR : 1;
	__IO uint32_t FORCE_DATA_END_BIT_ERR : 1;
	__IO uint32_t FORCE_CUR_LMT_ERR : 1;
	__IO uint32_t FORCE_AUTO_CMD_ERR : 1;
	__IO uint32_t FORCE_ADMA_ERR : 1;
	__IO uint32_t FORCE_TUNING_ERR : 1;
	__IO uint32_t FORCE_RESP_ERR : 1;
	__IO uint32_t FORCE_BOOT_ACK_ERR : 1;
	__IO uint32_t FORCE_VENDOR_ERR1 : 1;
	__IO uint32_t FORCE_VENDOR_ERR2 : 1;
	__IO uint32_t FORCE_VENDOR_ERR3 : 1;
}FORCE_ERROR_INT_STAT_R__FORCE_AUTO_CMD_STAT_R;


typedef struct
{
	__IO uint32_t ADMA_ERR_STATES : 2;
	__IO uint32_t ADMA_LEN_ERR : 1;
	__IO uint32_t RSVD_31_3 : 29;
}ADMA_ERR_STAT_R;

typedef struct
{
	__IO uint32_t FREQ_SEL_VAL_INIT : 10;
	__IO uint32_t CLK_GEN_SEL_VAL_INIT : 1;
	__IO uint32_t RSVD_13_11 : 3;
	__IO uint32_t DRV_SEL_VAL_INIT : 2;
    __IO uint32_t FREQ_SEL_VAL_DS : 10;
	__IO uint32_t CLK_GEN_SEL_VAL_DS : 1;
	__IO uint32_t RSVD_29_27 : 3;
	__IO uint32_t DRV_SEL_VAL_DS : 2;
}PRESET_DS_R__PRESET_INIT_R;

typedef struct
{
	__IO uint32_t FREQ_SEL_VAL_0 : 10;
	__IO uint32_t CLK_GEN_SEL_VAL_0 : 1;
	__IO uint32_t RSVD_13_11 : 3;
	__IO uint32_t DRV_SEL_VAL_0 : 2;
    __IO uint32_t FREQ_SEL_VAL_1 : 10;
	__IO uint32_t CLK_GEN_SEL_VAL_1 : 1;
	__IO uint32_t RSVD_29_27 : 3;
	__IO uint32_t DRV_SEL_VAL_1 : 2;
}PRESET_HS_R;

typedef struct
{
	__IO uint32_t FREQ_SEL_VAL_0 : 10;
	__IO uint32_t CLK_GEN_SEL_VAL_0 : 1;
	__IO uint32_t RSVD_13_11 : 3;
	__IO uint32_t DRV_SEL_VAL_0 : 2;
    __IO uint32_t FREQ_SEL_VAL_1 : 10;
	__IO uint32_t CLK_GEN_SEL_VAL_1 : 1;
	__IO uint32_t RSVD_29_27 : 3;
	__IO uint32_t DRV_SEL_VAL_1 : 2;
}PRESET_SDR50_R__PRESET_SDR25_R;

typedef struct
{
	__IO uint32_t FREQ_SEL_VAL_0 : 10;
	__IO uint32_t CLK_GEN_SEL_VAL_0 : 1;
	__IO uint32_t RSVD_13_11 : 3;
	__IO uint32_t DRV_SEL_VAL_0 : 2;
    __IO uint32_t FREQ_SEL_VAL_1 : 10;
	__IO uint32_t CLK_GEN_SEL_VAL_1 : 1;
	__IO uint32_t RSVD_29_27 : 3;
	__IO uint32_t DRV_SEL_VAL_1 : 2;
}PRESET_DDR50_R__PRESET_SDR104_R;

typedef struct
{
	__IO uint32_t FREQ_SEL_VAL : 10;
	__IO uint32_t CLK_GEN_SEL_VAL : 1;
	__IO uint32_t RSVD_13_11 : 3;
	__IO uint32_t DRV_SEL_VAL : 2;
    __IO uint32_t RSVD_31_24:16;
}PRESET_UHS2;

/* dwc_mshc_map/DWC_mshc_block registers */
typedef struct
{
    __IO uint32_t SDMASA_R;
    __IO BLOCKCOUNT_R__BLOCKSIZE_R BLOCKCOUNT_R__BLOCKSIZE;
    __IO uint32_t ARGUMENT_R;
    __IO CMD_R__XFER_MODE_R CMD_R__XFER_MODE;
    __IO uint32_t RESP01;
	__IO uint32_t RESP23;
	__IO uint32_t RESP45;
	__IO uint32_t RESP67;
	__IO uint32_t BUF_DATA;
	__IO PSTATE_REG_R PSTATE_REG;
	__IO WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1_R WUP_CTRL_R__BGAP_CTRL_R__PWR_CTRL_R__HOST_CTRL1;
    __IO SW_RST_R__TOUT_CTRL_R__CLK_CTRL_R SW_RST_R__TOUT_CTRL_R__CLK_CTRL;
	__IO ERROR_INT_STAT_R__NORMAL_INT_STAT_R ERROR_INT_STAT_R__NORMAL_INT_STAT;
    __IO ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN_R ERROR_INT_STAT_EN_R__NORMAL_INT_STAT_EN;
    __IO ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN_R ERROR_INT_SIGNAL_EN_R__NORMAL_INT_SIGNAL_EN;
    __IO HOST_CTRL2_R__AUTO_CMD_STAT_R HOST_CTRL2_R__AUTO_CMD_STAT;
	__IO CAPABILITIES1_R CAPABILITIES1;
	__IO CAPABILITIES2_R CAPABILITIES2;
	__IO CURR_CAPABILITIES1_R CURR_CAPABILITIES1;
	__IO CURR_CAPABILITIES2_R CURR_CAPABILITIES2;
	__IO FORCE_ERROR_INT_STAT_R__FORCE_AUTO_CMD_STAT_R FORCE_AUTO_CMD_STAT_R;
    __IO ADMA_ERR_STAT_R ADMA_ERR_STAT_R;
	__IO uint32_t ADMA_SA_LOW_R;
	__IO uint32_t ADMA_SA_HIGH_R;
	__IO PRESET_DS_R__PRESET_INIT_R PRESET_DS_R__PRESET_INIT;
	__IO PRESET_HS_R PRESET_DS;
	__IO PRESET_SDR50_R__PRESET_SDR25_R PRESET_SDR50_R__PRESET_SDR25;
	__IO PRESET_DDR50_R__PRESET_SDR104_R PRESET_DDR50_R__PRESET_SDR104;
}DWC_mshc_block_registers;

#define AT_CTRL_R 0x540
#define MBIU_CTRL_R 0x510


#define     __IO    volatile             /*!< Defines 'read / write' permissions              */
/**
  * @brief  Card Specific Data: CSD Register
  */
typedef struct
{
  __IO uint8_t  CSDStruct;            /*!< CSD structure */
  __IO uint8_t  SysSpecVersion;       /*!< System specification version */
  __IO uint8_t  Reserved1;            /*!< Reserved */
  __IO uint8_t  TAAC;                 /*!< Data read access-time 1 */
  __IO uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  __IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  __IO uint16_t CardComdClasses;      /*!< Card command classes */
  __IO uint8_t  RdBlockLen;           /*!< Max. read data block length */
  __IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  __IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  __IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  __IO uint8_t  DSRImpl;              /*!< DSR implemented */
  __IO uint8_t  Reserved2;            /*!< Reserved */
  __IO uint32_t DeviceSize;           /*!< Device Size */
  __IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  __IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  __IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  __IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  __IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  __IO uint8_t  EraseGrSize;          /*!< Erase group size */
  __IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  __IO uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  __IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  __IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  __IO uint8_t  WrSpeedFact;          /*!< Write speed factor */
  __IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  __IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  __IO uint8_t  Reserved3;            /*!< Reserded */
  __IO uint8_t  ContentProtectAppli;  /*!< Content protection application */
  __IO uint8_t  FileFormatGrouop;     /*!< File format group */
  __IO uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  __IO uint8_t  PermWrProtect;        /*!< Permanent write protection */
  __IO uint8_t  TempWrProtect;        /*!< Temporary write protection */
  __IO uint8_t  FileFormat;           /*!< File Format */
  __IO uint8_t  ECC;                  /*!< ECC code */
  __IO uint8_t  CSD_CRC;              /*!< CSD CRC */
  __IO uint8_t  Reserved4;            /*!< always 1*/
} SD_CSD;

/**
  * @brief  Card Identification Data: CID Register
  */
typedef struct
{
  __IO uint8_t  ManufacturerID;       /*!< ManufacturerID */
  __IO uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  __IO uint32_t ProdName1;            /*!< Product Name part1 */
  __IO uint8_t  ProdName2;            /*!< Product Name part2*/
  __IO uint8_t  ProdRev;              /*!< Product Revision */
  __IO uint32_t ProdSN;               /*!< Product Serial Number */
  __IO uint8_t  Reserved1;            /*!< Reserved1 */
  __IO uint16_t ManufactDate;         /*!< Manufacturing Date */
  __IO uint8_t  CID_CRC;              /*!< CID CRC */
  __IO uint8_t  Reserved2;            /*!< always 1 */
} SD_CID;

/**
  * @brief SD Card information
  */
typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  uint64_t CardCapacity;  /*!< Card Capacity */
  uint32_t CardBlockSize; /*!< Card Block Size */
  uint16_t RCA;
  uint8_t CardType;
} SD_CardInfo;

extern u32 SD_Init(void);
extern u32 SD_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
extern u32 SD_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize, uint32_t NumberOfBlocks);
extern u32 SD_WaitReadOperation();
extern SD_CardInfo SDCardInfo;
extern u32 SD_GetCardInfo(SD_CardInfo *cardinfo);

/**
  * @brief Supported SD Memory Cards
  */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)

/** 
  * @brief SDIO Commands  Index 
  */
#define SD_CMD_GO_IDLE_STATE                       ((uint8_t)0)
#define SD_CMD_SEND_OP_COND                        ((uint8_t)1)
#define SD_CMD_ALL_SEND_CID                        ((uint8_t)2)
#define SD_CMD_SET_REL_ADDR                        ((uint8_t)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((uint8_t)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((uint8_t)5)
#define SD_CMD_HS_SWITCH                           ((uint8_t)6)
#define SD_CMD_SEL_DESEL_CARD                      ((uint8_t)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((uint8_t)8)
#define SD_CMD_SEND_CSD                            ((uint8_t)9)
#define SD_CMD_SEND_CID                            ((uint8_t)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((uint8_t)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((uint8_t)12)
#define SD_CMD_SEND_STATUS                         ((uint8_t)13)
#define SD_CMD_HS_BUSTEST_READ                     ((uint8_t)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((uint8_t)15)
#define SD_CMD_SET_BLOCKLEN                        ((uint8_t)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((uint8_t)17)
#define SD_CMD_READ_MULT_BLOCK                     ((uint8_t)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((uint8_t)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((uint8_t)20) /*!< SD Card doesn't support it */
#define SD_CMD_SET_BLOCK_COUNT                     ((uint8_t)23) /*!< SD Card doesn't support it */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((uint8_t)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((uint8_t)25)
#define SD_CMD_PROG_CID                            ((uint8_t)26) /*!< reserved for manufacturers */
#define SD_CMD_PROG_CSD                            ((uint8_t)27)
#define SD_CMD_SET_WRITE_PROT                      ((uint8_t)28)
#define SD_CMD_CLR_WRITE_PROT                      ((uint8_t)29)
#define SD_CMD_SEND_WRITE_PROT                     ((uint8_t)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((uint8_t)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((uint8_t)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((uint8_t)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((uint8_t)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((uint8_t)38)
#define SD_CMD_FAST_IO                             ((uint8_t)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((uint8_t)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((uint8_t)42)
#define SD_CMD_APP_CMD                             ((uint8_t)55)
#define SD_CMD_GEN_CMD                             ((uint8_t)56)
#define SD_CMD_NO_CMD                              ((uint8_t)64)

/** 
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD should be sent before sending these commands. 
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((uint8_t)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((uint8_t)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((uint8_t)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((uint8_t)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((uint8_t)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((uint8_t)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((uint8_t)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((uint8_t)53) /*!< For SD I/O Card only */

/** 
  * @brief Following commands are SD Card Specific security commands.
  *        SDIO_APP_CMD should be sent before sending these commands. 
  */
#define SD_CMD_SD_APP_GET_MKB                      ((uint8_t)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((uint8_t)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((uint8_t)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((uint8_t)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((uint8_t)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((uint8_t)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((uint8_t)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((uint8_t)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((uint8_t)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((uint8_t)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((uint8_t)48) /*!< For SD Card only */


#define DATA_READ 0x1
#define DATA_WRITE 0x0

#define SDIO_Response_No                    (0x0)
#define SDIO_Response_Long                  (0x1)
#define SDIO_Response_Short                 (0x2)
#define SDIO_Response_Short_48B             (0x3)

#define FREQ_400K                           (0)    
#define FREQ_10M                            (1)


u32 SD_WaitReadOperation(volatile DWC_mshc_block_registers* ptr);


/*****************************END OF FILE**************************/

#endif
