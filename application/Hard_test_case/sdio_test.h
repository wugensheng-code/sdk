#ifndef __SDIO_TEST_H
#define __SDIO_TEST_H

extern void SD_Test(void);
#define REG_READ(reg_address) reg_read(reg_address)
extern unsigned reg_read(unsigned long long reg_address);
#define REG_WRITE(reg_address, reg_wdata) reg_write(reg_address, reg_wdata)
extern void reg_write(unsigned long long reg_address, unsigned reg_wdata);
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef struct
{
	uint16_t XFER_BLOCK_SIZE:12;
	uint16_t SDMA_BUF_BDARY:3;
	uint16_t RSVD_BLOCKSIZE15:1;
}BLOCKSIZE;

typedef struct
{
	uint16_t DMA_EN:1;
	uint16_t BLOCK_COUNT_ENABLE:1;
	uint16_t AUTO_CMD_ENABLE:2;
	uint16_t DATA_XFER_DIR:1;
	uint16_t MULTI_BLK_SEL:1;
	uint16_t RESP_TYPE:1;
	uint16_t RESP_ERR_CHK_ENABLE:1;
	uint16_t RESP_INT_DISABLE:1;
	uint16_t RSVD:7;
}XFER_MODE;

typedef struct
{
	uint16_t RESP_TYPE_SELECT:2;
	uint16_t SUB_CMD_FLAG:1;
	uint16_t CMD_CRC_CHK_ENABLE:1;
	uint16_t CMD_IDX_CHK_ENABLE:1;
	uint16_t DATA_PRESENT_SEL:1;
	uint16_t CMD_TYPE:2;
	uint16_t CMD_INDEX:6;
	uint16_t RSVD:2;
}CMD;
typedef struct
{
	uint32_t CMD_INHIBIT:1;
	uint32_t CMD_INHIBIT_DAT:1;
	uint32_t DAT_LINE_ACTIVE:1;
	uint32_t RE_TUNE_REQ:1;
	uint32_t DAT_7_4_MASK:4;
	uint32_t WR_XFER_ACTIVE:1;
	uint32_t RD_XFER_ACTIVE:1;
	uint32_t BUF_WR_ENABLE:1;
	uint32_t BUF_RD_ENABLE:1;
	uint32_t RSVD_15_12:4;
	uint32_t CARD_INSERTED:1;
	uint32_t CARD_STABLE:1;
	uint32_t CARD_DETECT_PIN_LEVEL:1;
	uint32_t WR_PROTECT_SW_LVL:1;
	uint32_t DAT_3_0:4;
	uint32_t CMD_LINE_LVL:1;
	uint32_t HOST_REG_VOL:1;
	uint32_t RSVD_26:1;
	uint32_t CMD_ISSUE_ERR:1;
	uint32_t SUB_CMD_STAT:1;
	uint32_t IN_DORMANT_ST:1;
	uint32_t LANE_SYNC:1;
	uint32_t UHS2_IF_DETECT:1;
}PSTATE_REG;

typedef struct
{
	uint8_t LED_CTRL:1;
	uint8_t DAT_XFER_WIDTH:1;
	uint8_t HIGH_SPEED_EN:1;
	uint8_t DMA_SEL:2;
	uint8_t EXTDAT_XFER:1;
	uint8_t CARD_DETECT_TEST_LVL:1;
	uint8_t CARD_DETECT_SIG_SEL:1;
}HOST_CTRL1;

typedef struct
{
	uint8_t SD_BUS_PWR_VDD1:1;
	uint8_t SD_BUS_VOL_VDD1:3;
	uint8_t SD_BUS_PWR_VDD2:1;
	uint8_t SD_BUS_VOL_VDD2:3;
}PWR_CTRL;

typedef struct
{
	uint8_t STOP_BG_REQ:1;
	uint8_t CONTINUE_REQ:1;
	uint8_t RD_WAIT_CTRL:1;
	uint8_t INT_AT_BGAP:1;
	uint8_t RSVD_7_4:4;
}BGAP_CTRL;

typedef struct
{
	uint8_t CARD_INT:1;
	uint8_t CARD_INSERT:1;
	uint8_t CARD_REMOVAL:1;
	uint8_t RSVD_7_3:5;
}WUP_CTRL;

typedef struct
{
	uint16_t	INTERNAL_CLK_EN:1;
	uint16_t	INTERNAL_CLK_STABLE:1;
	uint16_t	SD_CLK_EN:1;
	uint16_t	PLL_ENABLE:1;
	uint16_t    RSVD_4:1;
	uint16_t	CLK_GEN_SELECT:1;
	uint16_t    UPPER_FREQ_SEL:2;
	uint16_t    FREQ_SEL:8;
}CLK_CTRL;


typedef struct
{
	uint8_t TOUT_CNT:4;
	uint8_t RSVD_7_4:4;
}TOUT_CTRL;
typedef struct
{
	uint8_t	SW_RST_ALL:1;
	uint8_t SW_RST_CMD:1;
	uint8_t	SW_RST_DAT:1;
	uint8_t RSVD_7_3:5;
}SW_RST;

typedef struct
{
	uint16_t	CMD_COMPLETE:1;
	uint16_t	XFER_COMPLETE:1;
	uint16_t	BGAP_EVENT:1;
	uint16_t	DMA_INTERRUPT:1;
	uint16_t    BUF_WR_READY:1;
	uint16_t	BUF_RD_READY:1;
	uint16_t    CARD_INSERTION:1;
	uint16_t    CARD_REMOVAL:1;
	uint16_t    CARD_INTERRUPT:1;
	uint16_t    INT_A:1;
	uint16_t    INT_B:1;
	uint16_t    INT_C:1;
	uint16_t    RE_TUNE_EVENT:1;
	uint16_t    FX_EVENT:1;
	uint16_t    CQE_EVENT:1;
	uint16_t    ERR_INTERRUPT:1;
}NORMAL_INT_STAT;


typedef struct
{
	uint16_t	CMD_TOUT_ERR:1;
	uint16_t	CMD_CRC_ERR:1;
	uint16_t	CMD_END_BIT_ERR:1;
	uint16_t	CMD_IDX_ERR:1;
	uint16_t    DATA_TOUT_ERR:1;
	uint16_t	DATA_CRC_ERR:1;
	uint16_t    DATA_END_BIT_ERR:1;
	uint16_t    CUR_LMT_ERR:1;
	uint16_t    AUTO_CMD_ERR:1;
	uint16_t    ADMA_ERR:1;
	uint16_t    TUNING_ERR:1;
	uint16_t    RESP_ERR:1;
	uint16_t    BOOT_ACK_ERR:1;
	uint16_t    VENDOR_ERR1:1;
	uint16_t    VENDOR_ERR2:1;
	uint16_t    VENDOR_ERR3:1;
}ERROR_INT_STAT;

typedef struct
{
	uint32_t	CMD_COMPLETE_SIGNAL_EN:1; /* intr when response received */
	uint32_t XFER_COMPLETE_SIGNAL_EN:1; /* intr when data read/write xfer completed */
	uint32_t BGAP_EVENT_SIGNAL_EN:1;
	uint32_t	DMA_INTERRUPT_SIGNAL_EN:1;
	uint32_t	BUF_WR_READY_SIGNAL_EN:1;
	uint32_t BUF_RD_READY_SIGNAL_EN:1;
	uint32_t	CARD_INSERTION_SIGNAL_EN:1;
	uint32_t	CARD_REMOVAL_SIGNAL_EN:1;
	uint32_t CARD_INTERRUPT_SIGNAL_EN:1;
	uint32_t	INT_A_SIGNAL_EN:1;
	uint32_t	INT_B_SIGNAL_EN:1;
	uint32_t	INT_C_SIGNAL_EN:1;
	uint32_t	RE_TUNE_EVENT_SIGNAL_EN:1;
	uint32_t	FX_EVENT_SIGNAL_EN:1;
	uint32_t CQE_EVENT_SIGNAL_EN:1;
	uint32_t	CMD_TOUT_ERR_SIGNAL_EN:1;
	uint32_t	CMD_CRC_ERR_SIGNAL_EN:1;
	uint32_t	CMD_END_BIT_ERR_SIGNAL_EN:1;
	uint32_t	CMD_IDX_ERR_SIGNAL_EN:1;
	uint32_t DATA_TOUT_ERR_SIGNAL_EN:1;
	uint32_t DATA_CRC_ERR_SIGNAL_EN:1;
	uint32_t DATA_END_BIT_ERR_SIGNAL_EN	:1;
	uint32_t	CUR_LMT_ERR_SIGNAL_EN:1;
	uint32_t AUTO_CMD_ERR_SIGNAL_EN:1;
	uint32_t	ADMA_ERR_SIGNAL_EN:1;
	uint32_t	TUNING_ERR_SIGNAL_EN:1;
	uint32_t	RESP_ERR_SIGNAL_EN:1;
	uint32_t BOOT_ACK_ERR_SIGNAL_EN:1;
	uint32_t VENDOR_ERR_SIGNAL_EN1:1;
	uint32_t	VENDOR_ERR_SIGNAL_EN2:1;
	uint32_t	VENDOR_ERR_SIGNAL_EN3:1;
}ERROR_INT_SIGNAL_EN;

typedef struct
{
	uint16_t	AUTO_CMD12_NOT_EXEC:1;
	uint16_t	AUTO_CMD_TOUT_ERR:1;
	uint16_t	AUTO_CMD_CRC_ERR:1;
	uint16_t	AUTO_CMD_EBIT_ERR:1;
	uint16_t    AUTO_CMD_IDX_ERR:1;
	uint16_t	AUTO_CMD_RESP_ERR:1;
	uint16_t    RSVD_6:1;
	uint16_t    CMD_NOT_ISSUED_AUTO_CMD12:1;
	uint16_t    RSVD_15_8:8;
}AUTO_CMD_STAT;

typedef struct
{
	uint16_t	UHS_MODE_SEL:3;
	uint16_t	SIGNALING_EN:1;
	uint16_t	DRV_STRENGTH_SEL:2;
	uint16_t	EXEC_TUNING:1;
	uint16_t    SAMPLE_CLK_SEL:1;
	uint16_t	UHS2_IF_ENABLE:1;
	uint16_t    RSVD_9:1;
	uint16_t    ADMA2_LEN_MODE:1;
	uint16_t    CMD23_ENABLE:1;
	uint16_t    HOST_VER4_ENABLE:1;
	uint16_t    ADDRESSING:1;
	uint16_t    ASYNC_INT_ENABLE:1;
	uint16_t    PRESET_VAL_ENABLE:1;
}HOST_CTRL2;

typedef struct
{
	uint32_t	SDR50_SUPPORT:1;
	uint32_t 	SDR104_SUPPORT:1;
	uint32_t 	DDR50_SUPPORT:1;
	uint32_t	UHS2_SUPPORT:1;
	uint32_t	DRV_TYPEA:1;
	uint32_t 	DRV_TYPEC:1;
	uint32_t	DRV_TYPED:1;
	uint32_t	RSVD_39:1;
	uint32_t 	RETUNE_CNT:4;
	uint32_t	RSVD_44:1;
	uint32_t	USE_TUNING_SDR50:1;
	uint32_t	RE_TUNING_MODES:2;
	uint32_t	CLK_MUL:8;
	uint32_t    RSVD_56_58:3;
	uint32_t	ADMA3_SUPPORT:1;
	uint32_t	VDD2_18V_SUPPORT:1;
	uint32_t	RSVD_61:1;
	uint32_t	RSVD_62_63:2;
}CAPABILITIES2;

typedef struct
{
	uint32_t TOUT_CLK_FREQ : 6;
	uint32_t RSVD_6 : 1;
	uint32_t TOUT_CLK_UNIT : 1;
	uint32_t BASE_CLK_FREQ : 8;
	uint32_t MAX_BLK_LEN : 2;
	uint32_t Embedded_8_BIT : 1;
	uint32_t ADMA2_SUPPORT : 1;
	uint32_t RSVD_20 : 1;
	uint32_t HIGH_SPEED_SUPPORT : 1;
	uint32_t SDMA_SUPPORT : 1;
	uint32_t SUS_RES_SUPPORT : 1;
	uint32_t VOLT_33 : 1;
	uint32_t VOLT_30 : 1;
	uint32_t VOLT_18 : 1;
	uint32_t SYS_ADDR_64_V4 : 1;
	uint32_t SYS_ADDR_64_V3 : 1;
	uint32_t ASYNC_INT_SUPPORT : 1;
	uint32_t SLOT_TYPE_R : 1;
}CAPABILITIES1;

typedef struct
{
	uint32_t MAX_CUR_33V : 8;
	uint32_t MAX_CUR_30V : 8;
	uint32_t MAX_CUR_18V : 8;
	uint32_t RSVD_31_24 : 8;
}CURR_CAPABILITIES1;

typedef struct
{
	uint32_t MAX_CUR_VDD2_18V : 8;
	uint32_t RSVD_63_40 : 24;
}CURR_CAPABILITIES2;

typedef struct
{
	uint16_t FORCE_AUTO_CMD12_NOT_EXEC : 1;
	uint16_t FORCE_AUTO_CMD_TOUT_ERR : 1;
	uint16_t FORCE_AUTO_CMD_CRC_ERR : 1;
	uint16_t FORCE_AUTO_CMD_EBIT_ERR : 1;
	uint16_t FORCE_AUTO_CMD_IDX_ERR : 1;
	uint16_t FORCE_AUTO_CMD_RESP_ERR : 1;
	uint16_t RSVD_6 : 1;
	uint16_t FORCE_CMD_NOT_ISSUED_AUTO_CMD12 : 1;
	uint16_t RSVD_15_8 : 8;
}FORCE_AUTO_CMD_STAT;

typedef struct
{
	uint16_t FORCE_CMD_TOUT_ERR : 1;
	uint16_t FORCE_CMD_CRC_ERR : 1;
	uint16_t FORCE_CMD_END_BIT_ERR : 1;
	uint16_t FORCE_CMD_IDX_ERR : 1;
	uint16_t FORCE_DATA_TOUT_ERR : 1;
	uint16_t FORCE_DATA_CRC_ERR : 1;
	uint16_t FORCE_DATA_END_BIT_ERR : 1;
	uint16_t FORCE_CUR_LMT_ERR : 1;
	uint16_t FORCE_AUTO_CMD_ERR : 1;
	uint16_t FORCE_ADMA_ERR : 1;
	uint16_t FORCE_TUNING_ERR : 1;
	uint16_t FORCE_RESP_ERR : 1;
	uint16_t FORCE_BOOT_ACK_ERR : 1;
	uint16_t FORCE_VENDOR_ERR1 : 1;
	uint16_t FORCE_VENDOR_ERR2 : 1;
	uint16_t FORCE_VENDOR_ERR3 : 1;
}FORCE_ERROR_INT_STAT;

typedef struct
{
	uint8_t ADMA_ERR_STATES : 2;
	uint8_t ADMA_LEN_ERR : 1;
	uint8_t RSVD_7_3 : 5;
}ADMA_ERR_STAT;
typedef struct
{
	uint16_t FREQ_SEL_VAL : 10;
	uint16_t CLK_GEN_SEL_VAL : 1;
	uint16_t RSVD_13_11 : 3;
	uint16_t DRV_SEL_VAL : 2;
}PRESET_INIT;

typedef struct
{
	uint16_t FREQ_SEL_VAL : 10;
	uint16_t CLK_GEN_SEL_VAL : 1;
	uint16_t RSVD_13_11 : 3;
	uint16_t DRV_SEL_VAL : 2;
}PRESET_DS;

typedef struct
{
	uint16_t FREQ_SEL_VAL : 10;
	uint16_t CLK_GEN_SEL_VAL : 1;
	uint16_t RSVD_13_11 : 3;
	uint16_t DRV_SEL_VAL : 2;
}PRESET_HS;

typedef struct
{
	uint16_t FREQ_SEL_VAL : 10;
	uint16_t CLK_GEN_SEL_VAL : 1;
	uint16_t RSVD_13_11 : 3;
	uint16_t DRV_SEL_VAL : 2;
}PRESET_SDR12;

typedef struct
{
	uint16_t FREQ_SEL_VAL : 10;
	uint16_t CLK_GEN_SEL_VAL : 1;
	uint16_t RSVD_13_11 : 3;
	uint16_t DRV_SEL_VAL : 2;
}PRESET_SDR25;

typedef struct
{
	uint16_t FREQ_SEL_VAL : 10;
	uint16_t CLK_GEN_SEL_VAL : 1;
	uint16_t RSVD_13_11 : 3;
	uint16_t DRV_SEL_VAL : 2;
}PRESET_SDR50;

typedef struct
{
	uint16_t FREQ_SEL_VAL : 10;
	uint16_t CLK_GEN_SEL_VAL : 1;
	uint16_t RSVD_13_11 : 3;
	uint16_t DRV_SEL_VAL : 2;
}PRESET_SDR104;

typedef struct
{
	uint16_t FREQ_SEL_VAL : 10;
	uint16_t CLK_GEN_SEL_VAL : 1;
	uint16_t RSVD_13_11 : 3;
	uint16_t DRV_SEL_VAL : 2;
}PRESET_DDR50;

typedef struct
{
	uint16_t FREQ_SEL_VAL : 10;
	uint16_t CLK_GEN_SEL_VAL : 1;
	uint16_t RSVD_13_11 : 3;
	uint16_t DRV_SEL_VAL : 2;
}PRESET_UHS2;

typedef struct
{
	uint16_t UHS_II_BLK_SIZE : 12;
	uint16_t UHS_II_SDMA_BOUND : 3;
	uint16_t RSVD_15 : 1;
}UHS_II_BLOCK_SIZE;

typedef struct
{
	uint16_t DMA_EN : 1;
	uint16_t BLK_CNT_EN : 1;
	uint16_t RESERVED_3_2 : 2;
	uint16_t DATA_XFER_DIR : 1;
	uint16_t BLK_BYTE_MODE : 1;
	uint16_t RESP_TYP : 1;
	uint16_t RESP_ERR_CHK_EN : 1;
	uint16_t RESP_INTR_DISABLE : 1;
	uint16_t RESERVED_13_9 : 5;
	uint16_t EBSY_WAIT : 1;
	uint16_t HALF_FULL_SEL : 1;
}UHS_II_XFER_MODE;

typedef struct
{
	uint16_t RESERVED_1_0 : 2;
	uint16_t SUB_CMD_FLAG : 1;
	uint16_t RESERVED_4_3 : 2;
	uint16_t DATA_PRESENT : 1;
	uint16_t UHS_II_CMD_TYP : 2;
	uint16_t UHS_II_CMD_PKT_LEN : 5;
	uint16_t RESERVED_15_13 : 3;
}UHS_II_CMD;

typedef struct
{
	uint8_t UHS_II_MSG_SEL : 2;
	uint8_t RESERVED_7_2 : 6;
}UHS_II_MSG_SEL;


typedef struct
{
	uint16_t DEV_INTR_STATUS0 : 1;
	uint16_t DEV_INTR_STATUS1 : 1;
	uint16_t DEV_INTR_STATUS2 : 1;
	uint16_t DEV_INTR_STATUS3 : 1;
	uint16_t DEV_INTR_STATUS4 : 1;
	uint16_t DEV_INTR_STATUS5 : 1;
	uint16_t DEV_INTR_STATUS6 : 1;
	uint16_t DEV_INTR_STATUS7 : 1;
	uint16_t DEV_INTR_STATUS8 : 1;
	uint16_t DEV_INTR_STATUS9 : 1;
	uint16_t DEV_INTR_STATUS10 : 1;
	uint16_t DEV_INTR_STATUS11 : 1;
	uint16_t DEV_INTR_STATUS12 : 1;
	uint16_t DEV_INTR_STATUS13 : 1;
	uint16_t DEV_INTR_STATUS14 : 1;
	uint16_t DEV_INTR_STATUS15 : 1;
}UHS_II_DEV_INTR_STATUS;

typedef struct
{
	uint8_t DEV_SEL : 4;
	uint8_t RESERVED_6_4 : 3;
	uint8_t INT_MSG_EN : 1;
}UHS_II_DEV_SEL;

typedef struct
{
	uint16_t HOST_FULL_RST : 1;
	uint16_t HOST_SD_TRAN_RST : 1;
	uint16_t RESERVED_15_2 : 14;
}UHS_II_SOFT_RESET;

typedef struct
{
	uint16_t TIMEOUT_CNT_CMD_RES : 4;
	uint16_t TIMER_CNT_DEADLOCK : 4;
	uint16_t RESERVED_15_8 : 8;
}UHS_II_TIMER_CNTRL;

typedef struct
{
	uint32_t HEADER_ERR : 1;
	uint32_t RES_PKT_ERR : 1;
	uint32_t RETRY_EXPIRED : 1;
	uint32_t CRC_ERR : 1;
	uint32_t FRAMING_ERR : 1;
	uint32_t TID_ERR : 1;
	uint32_t RESERVED_6 : 1;
	uint32_t UNRECOVERABLE_ERR : 1;
	uint32_t EBSY_ERR : 1;
	uint32_t RESERVED_14_9 : 6;
	uint32_t ADMA_ERR : 1;
	uint32_t TIMEOUT_CMD_RES : 1;
	uint32_t TIMEOUT_DEADLOCK : 1;
	uint32_t RESERVED_26_18 : 9;
	uint32_t VENDOR_SPECIFIC_ERR0 : 1;
	uint32_t VENDOR_SPECIFIC_ERR1 : 1;
	uint32_t VENDOR_SPECIFIC_ERR2 : 1;
	uint32_t VENDOR_SPECIFIC_ERR3 : 1;
	uint32_t VENDOR_SPECIFIC_ERR4 : 1;

}UHS_II_ERR_INTR_STATUS;

typedef struct
{
	uint32_t HEADER_ERR : 1;
	uint32_t RES_PKT_ERR : 1;
	uint32_t RETRY_EXPIRED : 1;
	uint32_t CRC_ERR : 1;
	uint32_t FRAMING_ERR : 1;
	uint32_t TID_ERR : 1;
	uint32_t RESERVED_6 : 1;
	uint32_t UNRECOVERABLE_ERR : 1;
	uint32_t EBSY_ERR : 1;
	uint32_t RESERVED_14_9 : 6;
	uint32_t ADMA_ERR : 1;
	uint32_t TIMEOUT_CMD_RES : 1;
	uint32_t TIMEOUT_DEADLOCK : 1;
	uint32_t RESERVED_26_18 : 9;
	uint32_t VENDOR_SPECIFIC_ERR0 : 1;
	uint32_t VENDOR_SPECIFIC_ERR1 : 1;
	uint32_t VENDOR_SPECIFIC_ERR2 : 1;
	uint32_t VENDOR_SPECIFIC_ERR3 : 1;
	uint32_t VENDOR_SPECIFIC_ERR4 : 1;

}UHS_II_ERR_INTR_STATUS_EN;

typedef struct
{
	uint32_t HEADER_ERR : 1;
	uint32_t RES_PKT_ERR : 1;
	uint32_t RETRY_EXPIRED : 1;
	uint32_t CRC_ERR : 1;
	uint32_t FRAMING_ERR : 1;
	uint32_t TID_ERR : 1;
	uint32_t RESERVED_6 : 1;
	uint32_t UNRECOVERABLE_ERR : 1;
	uint32_t EBSY_ERR : 1;
	uint32_t RESERVED_14_9 : 6;
	uint32_t ADMA_ERR : 1;
	uint32_t TIMEOUT_CMD_RES : 1;
	uint32_t TIMEOUT_DEADLOCK : 1;
	uint32_t RESERVED_26_18 : 9;
	uint32_t VENDOR_SPECIFIC_ERR0 : 1;
	uint32_t VENDOR_SPECIFIC_ERR1 : 1;
	uint32_t VENDOR_SPECIFIC_ERR2 : 1;
	uint32_t VENDOR_SPECIFIC_ERR3 : 1;
	uint32_t VENDOR_SPECIFIC_ERR4 : 1;

}UHS_II_ERR_INTR_SIGNAL_EN;

typedef struct
{
	uint16_t REG_OFFSET_ADDR : 12;
	uint16_t RESERVED_15_12 : 4;
}P_UHS_II_SETTINGS;

typedef struct
{
	uint16_t REG_OFFSET_ADDR : 12;
	uint16_t RESERVED_15_12 : 4;
}P_UHS_II_HOST_CAPAB;

typedef struct
{
	uint16_t REG_OFFSET_ADDR : 12;
	uint16_t RESERVED_15_12 : 4;
}P_UHS_II_TEST;

typedef struct
{
	uint16_t REG_OFFSET_ADDR : 12;
	uint16_t RESERVED_15_12 : 4;
}P_VENDOR_SPECIFIC_AREA;


typedef struct
{
	uint16_t INTR_SLOT : 8;
	uint16_t RESERVED_15_12 : 8;
}SLOT_INTR_STATUS;

typedef struct
{
	uint16_t SPEC_VERSION_NUM : 8;
	uint16_t VENDOR_VERSION_NUM : 8;
}HOST_CNTRL_VERS;

typedef struct
{
	uint32_t PWR_MODE : 1;
	uint32_t RSVD_7_1 : 7;
	uint32_t NUM_LANE : 4;
	uint32_t RSVD_31_12 : 20;
}UHS2_GEN_SET;

typedef struct
{
	uint32_t RSVD_5_0 : 6;
	uint32_t SPD_RANGE : 2;
	uint32_t RSVD_14_8 : 7;
	uint32_t HBNATE_EN : 1;
	uint32_t N_LSS_SYN : 4;
	uint32_t N_LSS_DIR : 4;
	uint32_t RSVD_31_24 : 8;
}UHS2_PHY_SET;

typedef struct
{
	uint32_t RSVD_7_0 : 8;
	uint32_t N_FCU : 8;
	uint32_t RTRY_CNT : 2;
	uint32_t RSVD_31_18 : 14;
}UHS2_LNK_TRAN_SET_1;

typedef struct
{
	uint32_t N_DATA_GAP : 8;
	uint32_t RSVD_31_8 : 24;
}UHS2_LNK_TRAN_SET_2;

typedef struct
{
	uint32_t DAP : 4;
	uint32_t GAP : 4;
	uint32_t NUM_LANE : 6;
	uint32_t ADDR64 : 1;
	uint32_t BOOT_LOAD : 1;
	uint32_t RMV_EMBEDDED : 2;
	uint32_t NUM_DEV : 4;
	uint32_t BUS_TOPO : 2;
	uint32_t RSVD_31_24 : 8;
}UHS2_GEN_CAP;

typedef struct
{
	uint32_t DAP : 6;
	uint32_t GAP : 2;
	uint32_t NUM_LANE : 8;
	uint32_t ADDR64 : 4;
	uint32_t BOOT_LOAD : 4;
	uint32_t RMV_EMBEDDED : 8;
}UHS2_PHY_CAP;

typedef struct
{
	uint32_t LNK_REV : 6;
	uint32_t RSVD_7_6 : 2;
	uint32_t N_FCU : 8;
	uint32_t DEV_TYPE : 3;
	uint32_t RSVD_19 : 1;
	uint32_t MAX_BLK_LEN : 12;
}UHS2_LNK_TRAN_CAP_1;

typedef struct
{
	uint32_t N_DATA_GAP : 8;
	uint32_t RSVD_31_8 : 24;
}UHS2_LNK_TRAN_CAP_2;

typedef struct
{
	uint32_t HEADER_ERR : 1;
	uint32_t RES_PKT_ERR : 1;
	uint32_t RETRY_EXPIRED : 1;
	uint32_t CRC_ERR : 1;
	uint32_t FRAMING_ERR : 1;
	uint32_t TID_ERR : 1;
	uint32_t RESERVED_6 : 1;
	uint32_t UNRECOVERABLE_ERR : 1;
	uint32_t EBSY_ERR : 1;
	uint32_t RESERVED_14_9 : 6;
	uint32_t ADMA_ERR : 1;
	uint32_t TIMEOUT_CMD_RES : 1;
	uint32_t TIMEOUT_DEADLOCK : 1;
	uint32_t RESERVED_26_18 : 9;
	uint32_t VENDOR_SPECIFIC_ERR0 : 1;
	uint32_t VENDOR_SPECIFIC_ERR1 : 1;
	uint32_t VENDOR_SPECIFIC_ERR2 : 1;
	uint32_t VENDOR_SPECIFIC_ERR3 : 1;
	uint32_t VENDOR_SPECIFIC_ERR4 : 1;
}FORCE_UHS_II_ERR_INTR_STATUS;

typedef struct
{
	uint32_t HEADER_ERR : 3;
	uint32_t RES_PKT_ERR : 1;
	uint32_t RETRY_EXPIRED : 2;
	uint32_t CRC_ERR : 2;
	uint32_t FRAMING_ERR : 7;
	uint32_t TID_ERR : 1;
	uint32_t RESERVED_6 : 3;
	uint32_t UNRECOVERABLE_ERR : 1;
	uint32_t EBSY_ERR : 3;
	uint32_t RESERVED_14_9 : 1;
	uint32_t ADMA_ERR : 7;
	uint32_t TIMEOUT_CMD_RES : 1;
}EMBEDDED_CTRL;

typedef struct
{
	uint32_t EMMC_VER_SUFFIX : 4;
	uint32_t EMMC_VER_MINOR : 4;
	uint32_t EMMC_VER_MAJOR : 4;
	uint32_t EMMMC_VER_RSVD : 20;
}CQVER;

typedef struct
{
	uint32_t EMMC_VER_SUFFIX : 4;
	uint32_t EMMC_VER_MINOR : 4;
	uint32_t RES_PKT_ERR : 4;
	uint32_t RETRY_EXPIRED : 20;
}CQCAP;

typedef struct
{
	uint32_t CQ_EN : 1;
	uint32_t CR_GENERAL_EN : 1;
	uint32_t RES_PKT_ERR : 6;
	uint32_t RETRY_EXPIRED : 1;
	uint32_t CQCCFG_RSVD2 : 3;
	uint32_t DCMD_EN : 1;
	uint32_t CQCCFG_RSVD3 : 19;
}CQCFG;

typedef struct
{
	uint32_t HALT : 1;
	uint32_t CQCTL_RSVD1 : 7;
	uint32_t CLR_ALL_TASKS : 1;
	uint32_t CQCTL_RSVD2 : 23;
}CQCTL;

typedef struct
{
	uint32_t HAC : 1;
	uint32_t TCC : 1;
	uint32_t RED : 1;
	uint32_t TCL : 1;
	uint32_t GCE : 1;
	uint32_t ICCE : 1;
	uint32_t CQIS_RSVD1 : 26;
}CQIS;

typedef struct
{
	uint32_t HAC_STE : 1;
	uint32_t TCC_STE : 1;
	uint32_t RED_STE : 1;
	uint32_t TCL_STE : 1;
	uint32_t GCE_STE : 1;
	uint32_t ICCE_STE : 1;
	uint32_t CQISTE_RSVD1 : 26;
}CQISE;

typedef struct
{
	uint32_t HAC_SGE : 1;
	uint32_t TCC_SGE : 1;
	uint32_t RED_SGE : 1;
	uint32_t TCL_SGE : 1;
	uint32_t GCE_SGE : 1;
	uint32_t ICCE_SGE : 1;
	uint32_t CQISGE_RSVD1 : 26;
}CQISGE;

typedef struct
{
	uint32_t TOUT_VAL : 7;
	uint32_t TOUT_VAL_WEN : 1;
	uint32_t INTC_TH : 5;
	uint32_t CQIC_RSVD1 : 2;
	uint32_t INTC_TH_WEN : 1;
	uint32_t INTC_RST : 1;
	uint32_t CQIC_RSVD2 : 3;
	uint32_t INTC_STAT : 1;
	uint32_t CQIC_RSVD3 : 10;
	uint32_t INTC_EN : 1;
}CQIC;

typedef struct
{
	uint32_t SQSCMD_IDLE_TMR : 16;
	uint32_t SQSCMD_BLK_CNT : 4;
	uint32_t RSVD_20_31 : 12;
}CQSSC1;

typedef struct
{
	uint32_t SQSCMD_RCA : 16;
	uint32_t SQSCMD_BLK_CNT : 16;
}CQSSC2;

typedef struct
{
	uint32_t RESP_ERR_CMD_INDX : 6;
	uint32_t RSVD_6_7 : 2;
	uint32_t RESP_ERR_TASKID : 5;
	uint32_t RSVD_13_14 : 2;
	uint32_t RESP_ERR_FIELDS_VALID : 1;
	uint32_t TRANS_ERR_CMD_INDX : 6;
	uint32_t RSVD_23_22 : 2;
	uint32_t TRANS_ERR_TASKID : 5;
	uint32_t RSVD_30_29 : 2;
	uint32_t TRANS_ERR_FIELDS_VALID : 1;
}CQTERRI;

typedef struct
{
	uint32_t CMD_RESP_INDX : 6;
	uint32_t RSVD_31_6 : 26;
}CQCRI;

typedef struct
{
	uint8_t CMD_CONFLICT_CHECK : 1;
	uint8_t RSVD1 : 3;
	uint8_t SW_CG_DIS : 1;
	uint8_t Rsvd : 1;
	uint8_t PEDGE_DRV_EN : 1;
	uint8_t NEDGE_SMPL_EN : 1;

}MSHC_CTRL;

typedef struct
{
	uint8_t UNDEFL_INCR_EN : 1;
	uint8_t BURST_INCR4_EN : 1;
	uint8_t BURST_INCR8_EN : 1;
	uint8_t BURST_INCR16_EN : 1;
	uint8_t RSVD : 4;

}MBIU_CTRL;

typedef struct
{
	uint16_t CARD_IS_EMMC : 1;
	uint16_t DISABLE_DATA_CRC_CHK : 1;
	uint16_t EMMC_RST_N : 1;
	uint16_t EMMC_RST_N_OE : 1;
	uint16_t Rsvd : 4;
	uint16_t ENH_STROBE_ENABLE : 1;
	uint16_t CQE_ALGO_SEL : 1;
	uint16_t CQE_PREFETCH_DISABLE : 1;
	uint16_t RSVD : 5;
}EMMC_CTRL;

typedef struct
{
	uint16_t MAN_BOOT_EN : 1;
	uint16_t RSVD_6_1 : 6;
	uint16_t VALIDATE_BOOT : 1;
	uint16_t BOOT_ACK_ENABLE : 1;
	uint16_t RSVD_11_9 : 3;
	uint16_t BOOT_TOUT_CNT : 4;
}BOOT_CTRL;

typedef struct
{
	uint32_t AT_EN : 1;
	uint32_t CI_SEL : 1;
	uint32_t SWIN_TH_EN : 1;
	uint32_t RPT_TUNE_ERR : 1;
	uint32_t SW_TUNE_EN : 1;
	uint32_t RSDV2 : 3;
	uint32_t WIN_EDGE_SEL : 4;
	uint32_t RSDV3 : 4;
	uint32_t TUNE_CLK_STOP_EN : 1;
	uint32_t PRE_CHANGE_DLY : 2;
	uint32_t POST_CHANGE_DLY : 3;
	uint32_t Rsvd : 3;
	uint32_t SWIN_TH_VAL : 8;
}AT_CTRL;

typedef struct
{
	uint32_t CENTER_PH_CODE : 8;
	uint32_t R_EDGE_PH_CODE : 8;
	uint32_t L_EDGE_PH_CODE : 8;
	uint32_t RSDV1 : 8;
}AT_STAT;
/* dwc_mshc_map/DWC_mshc_block registers */
typedef struct
{
    uint32_t SDMASA_R;
    BLOCKSIZE BLOCKSIZE_R;
    uint16_t BLOCKCOUNT_R;
    uint32_t ARGUMENT_R;
    XFER_MODE XFER_MODE_R;
    CMD CMD_R;
    uint32_t RESP01_R;
	uint32_t RESP23_R;
	uint32_t RESP45_R;
	uint32_t RESP67_R;
	uint32_t BUF_DATA_R;
	PSTATE_REG PSTATE_REG_R;
	HOST_CTRL1 HOST_CTRL1_R;
	PWR_CTRL PWR_CTRL_R;
	BGAP_CTRL BGAP_CTRL_R;
	WUP_CTRL WUP_CTRL_R;
	CLK_CTRL CLK_CTRL_R;
	TOUT_CTRL TOUT_CTRL_R;
	SW_RST SW_RST_R;
	NORMAL_INT_STAT NORMAL_INT_STAT_R;
	ERROR_INT_STAT ERROR_INT_STAT_R;
	uint16_t NORMAL_INT_STAT_EN_R;
	uint16_t ERROR_INT_STAT_EN_R;
	uint16_t NORMAL_INT_SIGNAL_EN_R;
	uint16_t ERROR_INT_SIGNAL_EN_R;
	AUTO_CMD_STAT AUTO_CMD_STAT_R;
	HOST_CTRL2 HOST_CTRL2_R;
	CAPABILITIES1 CAPABILITIES1_R;
	CAPABILITIES2 CAPABILITIES2_R;
	CURR_CAPABILITIES1 CURR_CAPABILITIES1_R;
	CURR_CAPABILITIES2 CURR_CAPABILITIES2_R;
	FORCE_AUTO_CMD_STAT FORCE_AUTO_CMD_STAT_R;
	FORCE_ERROR_INT_STAT FORCE_ERROR_INT_STAT_R;
	ADMA_ERR_STAT ADMA_ERR_STAT_R;
	uint32_t ADMA_SA_LOW_R;
	uint32_t ADMA_SA_HIGH_R;
	PRESET_INIT PRESET_INiT_R;
	PRESET_DS PRESET_DS_R;
	PRESET_HS PRESET_HS_R;
	PRESET_SDR12 PRESET_SDR12_R;
	PRESET_SDR25 PRESET_SDR25_R;
	PRESET_SDR50 PRESET_SDR50_R;
	PRESET_SDR104 PRESET_SDR104_R;
	PRESET_DDR50 PRESET_DDR50_R;
	PRESET_UHS2 PRESET_UHS2_R;
	uint32_t ADMA_ID_LOW_R;
	uint32_t ADMA_ID_HIGH_R;
	UHS_II_BLOCK_SIZE UHS_II_BLOCK_SIZE_R;
	uint32_t UHS_II_BLOCK_COUNT_R;
	uint32_t UHS_II_CMD_PKT_0_3_R;
	uint32_t UHS_II_CMD_PKT_4_7_R;
	uint32_t UHS_II_CMD_PKT_8_11_R;
	uint32_t UHS_II_CMD_PKT_12_15_R;
	uint32_t UHS_II_CMD_PKT_16_19_R;
	UHS_II_XFER_MODE UHS_II_XFER_MODE_R;
	UHS_II_CMD UHS_II_CMD_R;
	uint32_t UHS_II_RESP_0_3_R;
	uint32_t UHS_II_RESP_4_7_R;
	uint32_t UHS_II_RESP_8_11_R;
	uint32_t UHS_II_RESP_12_15_R;
	uint32_t UHS_II_ESP_16_19_R;
	UHS_II_MSG_SEL UHS_II_MSG_SEL_R;
	uint8_t UHS_II_MSG_R;
	UHS_II_DEV_INTR_STATUS UHS_II_DEV_INTR_STATUS_R;
	UHS_II_DEV_SEL UHS_II_DEV_SEL_R;
	uint8_t UHS_II_DEV_INR_CODE_R;
	UHS_II_SOFT_RESET UHS_II_SOFT_RESET_R;
	UHS_II_TIMER_CNTRL UHS_II_TIMER_CNTRL_R;
	UHS_II_ERR_INTR_STATUS UHS_II_ERR_INTR_STATUS_R;
	UHS_II_ERR_INTR_STATUS_EN UHS_II_ERR_INTR_STATUS_EN_R;
	UHS_II_ERR_INTR_SIGNAL_EN UHS_II_ERR_INTR_SIGNAL_EN_R;
	P_UHS_II_SETTINGS P_UHS_II_SETTINGS_R;
	P_UHS_II_HOST_CAPAB P_UHS_II_HOST_CAPAB;
	P_UHS_II_TEST P_UHS_II_TEST;
	uint16_t P_EMBEDDED_CNTRL;
	P_VENDOR_SPECIFIC_AREA P_VENDOR_1_SPECIFIC_AREA;
	uint16_t P_VENDOR_2_SPECIFIC_AREA;
	uint16_t SLOT_INTR_STATUS_R;
	uint16_t HOST_CNTRL_VERS_R;
}DWC_mshc_block_registers;

/* DWC_mshc_map/DWC_mshc_UHS2_setting block register */
typedef struct
{
	UHS2_GEN_SET UHS2_GEN_SET_R;
	UHS2_PHY_SET UHS2_PHY_SET_R;
	uint32_t USH2_LNK_TRAN_SET_1_R;
    uint32_t UHS2_LNK_TRAN_SET_2_R;
} DWC_mshc_UHS2_setting_block_register;


/* DWC_mshc_map/DWC_mshc_UHS2_capability register */
typedef struct
{
	uint32_t UHS2_GEN_CAP_R;
	uint32_t UHS2_PHY_CAP_R;
	uint32_t UHS2_LNK_TRAN_CAP_1_R;
	uint32_t UHS2_LNK_TRAN_CAP_2_R;
}DWC_mshc_UHS2_capability_register;

typedef struct {
    uint32_t SDIO_Argument; /*!< Specifies the SDIO command argument which is sent
                            to a card as part of a command message. If a command
                            contains an argument, it must be loaded into this
                          register before writing the command to the command
                          register */

    uint32_t SDIO_CmdIndex; /*!< Specifies the SDIO command index. It must be
                               lower than 0x40. */

    uint32_t SDIO_Response; /*!< Specifies the SDIO response type.
                                 This parameter can be a value of @ref
                               SDIO_Response_Type */

    uint32_t SDIO_Wait; /*!< Specifies whether SDIO wait for interrupt request
                           is enabled or disabled.
                             This parameter can be a value of @ref
                           SDIO_Wait_Interrupt_State */

    uint32_t SDIO_CPSM; /*!< Specifies whether SDIO Command path state machine (CPSM)
                        is enabled or disabled.
                        This parameter can be a value of @ref SDIO_CPSM_State */
} SDIO_CmdInitTypeDef;
#endif


/*****************************END OF FILE**************************/
