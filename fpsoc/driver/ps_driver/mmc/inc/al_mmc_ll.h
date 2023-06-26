#ifndef AL_MMC_LL_H
#define AL_MMC_LL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_core.h"
#include "al_mmc_hw.h"

#define AL_MMC_INTR_CC_MASK     BITS_32(MMC_ERROR_INT_STAT_R_NORMAL_INT_STAT_R_CMD_COMPLETE_SHIFT, \
                                        MMC_ERROR_INT_STAT_R_NORMAL_INT_STAT_R_CMD_COMPLETE_SIZE)
#define AL_MMC_INTR_BRR_MASK    BITS_32(MMC_ERROR_INT_STAT_R_NORMAL_INT_STAT_R_BUF_RD_READY_SHIFT, \
                                        MMC_ERROR_INT_STAT_R_NORMAL_INT_STAT_R_BUF_RD_READY_SIZE)

#define AL_MMC_INTR_ALL_MASK    (0xFFFFFFFF)

#define AL_MMC_OCR_VOLT_27_36   (0x1FF)

#define AL_MMC_EXT_CSD_IDX_BUS_WIDTH    183

#define AL_MMC_FIXED_BLK_LEN    (0x200)
#define AL_MMC_EXT_CSD_LEN      (0x200)

#define AL_MMC_ADMA_TABLE_SIZE              (4)
#define AL_MMC_ADMA2_DESC_16BIT_LENGTH      (0xFFFF)
#define AL_MMC_ADMA2_DESC_26BIT_LENGTH      (0x3FFFFFF)
#define AL_MMC_ADMA2_16BIT_LENGTH_SIZE      (16)
#define AL_MMC_ADMA2_16BIT_LENGTH_SIZE_HI   (0)
#define AL_MMC_ADMA2_16BIT_LENGTH_SIZE_LO   (16)
#define AL_MMC_ADMA2_26BIT_LENGTH_SIZE      (26)
#define AL_MMC_ADMA2_26BIT_LENGTH_SIZE_HI   (10)
#define AL_MMC_ADMA2_26BIT_LENGTH_SIZE_LO   (16)


typedef enum
{
    AL_MMC_SIG_EMIO_MIO     = 0,
    AL_MMC_SIG_CONFIGURED   = 1
} AL_MMC_SwCardSigEnum;

typedef enum
{
    AL_MMC_BDARY_4K     = 0x0,
    AL_MMC_BDARY_8K,
    AL_MMC_BDARY_16K,
    AL_MMC_BDARY_32K,
    AL_MMC_BDARY_64K,
    AL_MMC_BDARY_128K,
    AL_MMC_BDARY_256K,
    AL_MMC_BDARY_512K
} AL_MMC_BuffBdaryEnum;

typedef enum

{
    AL_MMC_AUTO_CMD_DISABLE = 0x0,
    AL_MMC_AUTO_CMD_12,
    AL_MMC_AUTO_CMD_23,
    AL_MMC_AUTO_CMD_AUTO
} AL_MMC_AutoCmdEnum;

typedef enum
{
    AL_MMC_TRANS_DIR_WRITE,
    AL_MMC_TRANS_DIR_READ
} AL_MMC_TransDirEnum;

typedef enum
{
    AL_MMC_RESP_ERR_CHK_TYPE_1,
    AL_MMC_RESP_ERR_CHK_TYPE_5
} AL_MMC_RespErrChkTypeEnum;

typedef enum
{
    AL_MMC_RESP_TYPE_LEN_NONE,
    AL_MMC_RESP_TYPE_LEN_136,
    AL_MMC_RESP_TYPE_LEN_48,
    AL_MMC_RESP_TYPE_LEN_48B    /* Check busy after response */
} AL_MMC_RespTypeLenEnum;

typedef enum
{
    AL_MMC_CMD_TYPE_NORMAL,
    AL_MMC_CMD_TYPE_SUSPEND,
    AL_MMC_CMD_TYPE_RESUME,
    AL_MMC_CMD_TYPE_ABORT
} AL_MMC_CmdTypeEnum;

typedef enum
{
    AL_MMC_STATE_MASK_CMD_LINE          = BITS_32(0,1),
    AL_MMC_STATE_MASK_CMD_DAT_LINE      = BITS_32(1,1),
    AL_MMC_STATE_MASK_CMD_LINE_BOTH     = BITS_32(0,2),
    AL_MMC_STATE_MASK_DAT_LINE          = BITS_32(2,1),
    AL_MMC_STATE_MASK_LINE_BOTH         = BITS_32(0,3),
    AL_MMC_STATE_MASK_WR_ACTIVE         = BITS_32(8,1),
    AL_MMC_STATE_MASK_RD_ACTIVE         = BITS_32(9,1)
} AL_MMC_StateMaskEnum;

typedef enum
{
    AL_MMC_CARD_STAT_IDLE,
    AL_MMC_CARD_STAT_RDY,
    AL_MMC_CARD_STAT_IDENT,
    AL_MMC_CARD_STAT_STBY,
    AL_MMC_CARD_STAT_TRAN,
    AL_MMC_CARD_STAT_DATA,
    AL_MMC_CARD_STAT_RCV,
    AL_MMC_CARD_STAT_PRG,
    AL_MMC_CARD_STAT_DIS,
    AL_MMC_CARD_STAT_BTST,      /* eMMC only */
    AL_MMC_CARD_STAT_SLP        /* eMMC only */
} AL_MMC_CardStatEnum;

typedef enum
{
    AL_MMC_CSD_VER_SD10_EMMC10,
    AL_MMC_CSD_VER_SD20_EMMC11,
    AL_MMC_CSD_VER_EMMC12,
    AL_MMC_CSD_VER_EMMC_EXTCSD,
} AL_MMC_CsdVerEnum;

typedef enum
{
    AL_MMC_BLK_LEN_8B       = (8),
    AL_MMC_BLK_LEN_64B      = (64),
    AL_MMC_BLK_LEN_512B     = (512),
    AL_MMC_BLK_LEN_1024B    = (1024),
    AL_MMC_BLK_LEN_2048B    = (2048)
} AL_MMC_BlkLenEnum;

typedef enum
{
    AL_MMC_CMD_IDX_0    = 0x0,      /* [31: 0] 0x0       : Resets all cards to idle state(-) */
                                    /* [31: 0] 0xF0F0F0F0: Resets all cards to pre_idle state(eMMC only) */
                                    /* [31: 0] 0xFFFFFFFA: Initiate alternative boot operation(eMMC only) */
    AL_MMC_CMD_IDX_1    = 0x1,      /* [31: 0] OCR without busy: Ask dev in idle state to send its OCR on CMD line
                                                                 (eMMC only)(R3) */
    AL_MMC_CMD_IDX_2    = 0x2,      /* [31: 0] stuff bits: Ask dev to send its CID on CMD line(R2) */
    AL_MMC_CMD_IDX_3    = 0x3,      /* [31: 0] stuff bits: Assign relative address to the dev(SD only)(R6) */
                                    /* [31:16] RCA
                                       [15: 0] stuff bits: Assign relative address to the dev(eMMC only)(R3) */
    AL_MMC_CMD_IDX_4    = 0x4,      /* [31:16] Driver stage register(DSR)
                                       [15: 0] stuff bits: Programs the DSR of all cards(-) */
    AL_MMC_CMD_IDX_5    = 0x5,      /* [31:16] RCA
                                       [   15] sleep/awake
                                       [14: 0] stuff bits: Toggle dev between sleep and standby state(eMMC only)(R1B) */
    AL_MMC_CMD_IDX_6    = 0x6,      /* [31:26] set to 0
                                       [25:24] access
                                       [23:16] index
                                       [15: 8] value
                                       [ 7: 3] set to 0
                                       [ 2: 0] cmd set: switch the ops mode of selected device or modifies the EXT_CSD
                                                        reg(eMMC only)(R1B) */
    AL_MMC_CMD_IDX_7    = 0x7,      /* [31:16] relative device address(RCA)
                                       [15: 0] stuff bits: toggles a device between standby and transfer states(R1) or
                                                           programming and disconnect states(R1B)*/
    AL_MMC_CMD_IDX_8    = 0x8,      /* [31:12] reserved, set to 0
                                       [11: 8] supply voltage(VHS)
                                       [ 7: 0] check pattern: check whether card supports voltage int VHS(SD)(R7) */
                                    /* [31: 0] stuff bits: dev sends its EXT_CSD register as a block of data(eMMC)(R1) */
    AL_MMC_CMD_IDX_9    = 0x9,      /* [31:16] relative device address(RCA)
                                       [15: 0] stuff bits: dev sends its card specific data(CSD) on CMD line(R2) */
    AL_MMC_CMD_IDX_10   = 0xa,      /* [31:16] relative device address(RCA)
                                       [15: 0] stuff bits: dev sends its card identification(CID) on CMD line(R2) */
    AL_MMC_CMD_IDX_11   = 0xb,      /* [31: 0] reserved, set to 0: switch to 1.8V bus signaling level(SD only)(R1) */
    AL_MMC_CMD_IDX_12   = 0xc,      /* [31: 0] stuff bits: force the card to stop transmisson(SD)(R1) */
                                    /* [31:16] relative device address(RCA)
                                       [15: 1] stuff bits
                                       [    0] HPI: force the dev to stop transmission, if HPI set, will interrupt(eMMC)
                                                    (R1/read, R1B/write) */
    AL_MMC_CMD_IDX_13   = 0xd,      /* [31:16] relative device address(RCA)
                                       [   15] send task status register
                                       [14: 0] stuff bits: CQ enabled and [15]=1:task status; other: status(SD)(R1) */
                                    /* [31:16] relative device address(RCA)
                                       [   15] send task status register
                                       [14: 1] stuff bits
                                       [    0] HPI: [15]=1: queue status reg(QSR); other: status reg; HPI ctrl intr(eMMC)(R1) */
    AL_MMC_CMD_IDX_14   = 0xe,      /* [31: 0] stuff bits: read the reversed bus testing data pattern from dev(eMMC only)(R1) */
    AL_MMC_CMD_IDX_15   = 0xf,      /* [31:16] relative device address(RCA)
                                       [15: 0] stuff bits: set dev to inactive state(-) */
    AL_MMC_CMD_IDX_16   = 0x10,     /* [31: 0] block length: SDHC/SDXC: fixed 512B;
                                                             SDSC: avaliable when partial block read opreation allowed
                                                             eMMC: default block length is in CSD(R1) */
    AL_MMC_CMD_IDX_17   = 0x11,     /* [31: 0] data address: read single block(R1) */
    AL_MMC_CMD_IDX_18   = 0x12,     /* [31: 0] data address: read multiple block: until interrupted by a CMD12(R1) */
    AL_MMC_CMD_IDX_19   = 0x13,     /* [31: 0] reserved, set to 0: send 64 bytes tuning pattern for SDR50 OR SDR104(SD only)(R1) */
                                    /* [31: 0] stuff bits: send bus test data pattern to dev(eMMC only)(R1) */
    AL_MMC_CMD_IDX_20   = 0x14,     /* [31:28] speed class ctrl
                                       [27: 0] speed class ctrl cmd: speed class ctrl(SD only)(R1B) */
    AL_MMC_CMD_IDX_21   = 0x15,     /* [31: 0] stuff bits: send 64B/4-bit or 128B/8-bit for HS200 sampling point
                                                           detection(eMMC only)(R1) */
    AL_MMC_CMD_IDX_22   = 0x16,     /* Not support, for video speed calss */
    AL_MMC_CMD_IDX_23   = 0x17,     /* [31: 0] block count: for CMD18 and CMD25(SD) */
                                    /* [   31] reliable write request
                                       [   30] 0 to non-packed
                                       [   29] tag request(not together with [28:25])
                                       [28:25] context ID(not together with [29])
                                       [   24] forced programming
                                       [23:16] set to 0
                                       [15: 0] number of blocks: set block cnt(eMMC non-packed cmd version)(R1) */  /* TODO: learn detail with this cmd */
                                    /* [   31] set to 0
                                       [   30] 1 to packed
                                       [29:16] set to 0
                                       [15: 0] number of blocks: set block cnt(eMMC packed cmd version)(R1) */
    AL_MMC_CMD_IDX_24   = 0x18,     /* [31: 0] data address: write block cmd(R1) */
    AL_MMC_CMD_IDX_25   = 0x19,     /* [31: 0] data address: write multiple block: until interrupted by a CMD12(R1) */
    AL_MMC_CMD_IDX_26   = 0x1a,     /* Not support, program CID, issue only once by manufacturer(R1) */
    AL_MMC_CMD_IDX_27   = 0x1b,     /* [31: 0] stuff bits: program card specific data(CSD)(R1) */
    AL_MMC_CMD_IDX_28   = 0x1c,     /* [31: 0] data address: SDSC: protect the address group sized by WP_GRP_SIZE
                                                             SDHC/SDXC: not support this cmd
                                                             eMMC & CLASS_6_CTRL=0: protect the address group sized by
                                                                                    WP_GRP_SIZE or HC_WP_GRP_SIZE
                                                             eMMC & CLASS_6_CTRL=1: release the address group(R1B) */
    AL_MMC_CMD_IDX_29   = 0x1d,     /* [31: 0] data address: clear write protect of the address group
                                                             SDHC/SDXC: not support this cmd
                                                             eMMC & CLASS_6_CTRL=0: clear the address group
                                                             eMMC & CLASS_6_CTRL=1: ignored(R1B) */
    AL_MMC_CMD_IDX_30   = 0x1e,     /* [31: 0] data address: get address protection status
                                                             SDHC/SDXC: not support this cmd
                                                             eMMC & CLASS_6_CTRL=0: get the address group protect status
                                                             eMMC & CLASS_6_CTRL=1: get the status of release group
                                                                                    0: group valid and accessible
                                                                                    1: group released and cannot be used(R1) *//* TODO: learn detail with this cmd */
    AL_MMC_CMD_IDX_31   = 0x1f,     /* [31: 0] data address: eMMC & CLASS_6_CTRL=0: get the address group protect type
                                                             eMMC & CLASS_6_CTRL=1: return fixed 64 bits zeros(eMMC only)(R1) *//* TODO: learn detail with this cmd */
    AL_MMC_CMD_IDX_32   = 0x20,     /* [31: 0] data address: set the first block to be erased(SD only)(R1) */
    AL_MMC_CMD_IDX_33   = 0x21,     /* [31: 0] data address: set the last block to be erased(SD only)(R1) */
    AL_MMC_CMD_IDX_35   = 0x23,     /* [31: 0] data address: set the first block to be erased(eMMC only)(R1) */
    AL_MMC_CMD_IDX_36   = 0x24,     /* [31: 0] data address: set the last block to be erased(eMMC only)(R1) */
    AL_MMC_CMD_IDX_38   = 0x26,     /* [31: 0] erase function: start erase(SD only)(R1B)
                                                               0x1: Discard, de-allocate the discarded blocks partically or completely
                                                               0x2: full user area logical erase(FULE)
                                                               other: erase */
                                    /* [   31] secure request, only supported if SEC_GB_CL_EN (EXT_CSD[231] bit 4) is set
                                       [30:16] set to 0
                                       [   15] force garbage collect request, supported if SEC_ER_EN (EXT_CSD[231] bit 0) is set
                                       [14: 2] set to 0
                                       [    1] discard enable
                                       [    0] identify writeblocks for erase or TRIM enable
                                                               0x00000000: erase
                                                               0x00000003: discard
                                                               0x80000000: secure erase
                                                               0x80008000: secure trim step 2
                                                               0x80000001: secure trim step 1
                                                               0x00000001: trim(eMMC only)(R1B) */
    AL_MMC_CMD_IDX_39   = 0x27,     /* [31:16] RCA
                                       [   15] register write flag
                                       [14: 8] register address
                                       [ 7: 0] register data: write and read 8-bit register data field(eMMC only)(R4) */
    AL_MMC_CMD_IDX_40   = 0x28,     /* [31: 0] defined by DPS spec: single block read type, available even card locked(SD)(R1) */
                                    /* [31: 0] stuff bits: set system into intr mode(eMMC)(R5) */
    AL_MMC_CMD_IDX_42   = 0x2a,     /* [31: 0] set to 0: set/reset password or lock/unlock card(R1) */
    AL_MMC_CMD_IDX_44   = 0x2c,     /* [   31] reliable write request
                                       [   30] data direction(DD), 1: read, 0: write
                                       [   29] tag request
                                       [28:25] context ID
                                       [   24] forced programming
                                       [   23] priority, 1: high, 0: simple
                                       [22:21] reserved
                                       [20:16] task ID
                                       [15: 0] number of blocks: set queue task params(eMMC only)(R1) */
    AL_MMC_CMD_IDX_45   = 0x2d,     /* [31: 0] start block address: set queue task address(eMMC only)(R1) */
    AL_MMC_CMD_IDX_46   = 0x2e,     /* [31:21] reserved
                                       [20:16] task ID
                                       [15: 4] reserved
                                       [ 3: 0] TM op-code 2: this ID task, 1: ID field reserved
                                               : discard the specific ID task or entire queue(eMMC only)(R1) */
    AL_MMC_CMD_IDX_47   = 0x2f,     /* [31:21] reserved
                                       [20:16] task ID
                                       [15: 0] reserved: start the specific ID task write(eMMC only)(R1) */
    AL_MMC_CMD_IDX_48   = 0x30,     /* [31:21] reserved
                                       [20:16] task ID
                                       [15: 0] reserved: start the specific ID task write(eMMC only)(R1B) */
    AL_MMC_CMD_IDX_49   = 0x31,     /* [31: 0] stuff bits: set real time clk according to RTC info in 512B block
                                                           (eMMC only)(R1) */
    AL_MMC_CMD_IDX_53   = 0x35,     /* [31:16] security protocol specific
                                       [15: 0] security protocol
                                       [ 7: 0] reserved: continue read, block num defined by CMD23, stop by CMD12,
                                                         not support for packed CMD23(eMMC only)(R1) */
    AL_MMC_CMD_IDX_54   = 0x36,     /* [31:16] security protocol specific
                                       [15: 0] security protocol
                                       [ 7: 0] reserved: continue write, block num defined by CMD23, stop by CMD12,
                                                         not support for packed CMD23(eMMC only)(R1) */
    AL_MMC_CMD_IDX_55   = 0x37,     /* [31:16] RCA
                                       [15: 0] stuff bits: indicate next cmd is an ACMD(R1) */
    AL_MMC_CMD_IDX_56   = 0x38,     /* [31: 1] stuff bits
                                       [    0] RD/WR: next use ACMD to write(0) or read(1) a data block(R1) */
    AL_MMC_ACMD_OFFSET  = 0x100,    /* use to spearate CMD and ACMD */
    AL_MMC_ACMD_IDX_6   = 0X106,    /* [31: 2] stuff bits
                                       [ 1: 0] buf width, 0: 1-bit, 1: 4-bit. Set data bus width givin in SCR reg(SD only)(R1) */
    AL_MMC_ACMD_IDX_13  = 0X10d,    /* [31: 0] stuff bits: read SD status(SD only)(R1) */
    AL_MMC_ACMD_IDX_22  = 0X116,    /* [31: 0] stuff bits: get num of successfully writen blocks(SD only)(R1) */
    AL_MMC_ACMD_IDX_23  = 0X117,    /* [31:23] stuff bits
                                       [22: 0] number of blocks: pre-erase before writing, default 1(SD only)(R1) */
    AL_MMC_ACMD_IDX_41  = 0X129,    /* [   31] reserved
                                       [   30] high capacity support(HSC(OCR[30])), 1: SDHC/SDXC, 0: SDSC
                                       [   29] reserved for eSD
                                       [   28] SDXC power control(XPC), 1: max performance, 0: power saving
                                       [27:25] reserved
                                       [   24] switch to 1.8V request(S18R), 1: switch, 0: use current volt
                                       [23: 0] VDD volt window(OCR[23:0]): read OCR register(SD only)(R3) */
    AL_MMC_ACMD_IDX_42  = 0X12a,    /* [31: 1] stuff bits
                                       [    0] set_cd, 1: connect, 0: disconnect the 50KOhm pull-up resistor on DAT[3](SD only)(R1) */
    AL_MMC_ACMD_IDX_51  = 0X133,    /* [31: 0] stuff bits: read sd card configuration register(SCR)(SD only)(R1) */
} AL_MMC_CmdIdxEnum;

typedef enum
{
    AL_MMC_TCU_KHZ,
    AL_MMC_TCU_MHZ
} AL_MMC_ToutClkUnitEnum;

typedef enum
{
    AL_MMC_MAX_BLK_LEN_512B,
    AL_MMC_MAX_BLK_LEN_1024B,
    AL_MMC_MAX_BLK_LEN_2048B,
    AL_MMC_MAX_BLK_LEN_RSVD
} AL_MMC_MaxBlkLenEnum;

typedef enum
{
    AL_MMC_SLOT_REMOVABLE,      /* Removable card slot */
    AL_MMC_SLOT_EMBEDDED,       /* Embedded slot for one device */
    AL_MMC_SLOT_SHARED,         /* Shared bus slot(SD mode) */
    AL_MMC_SLOT_UHS2_EMBEDDED   /* UHS-II multiple embedded devices */
} AL_MMC_SlotTypeEnum;

typedef enum
{
    AL_MMC_RESP_TYPE_NONE,  /* No response */
    AL_MMC_RESP_TYPE_1,     /* Normal response command, 48-bit */
    AL_MMC_RESP_TYPE_1B,    /* R1 with optinal busy signal transmitted on DAT[0] */
    AL_MMC_RESP_TYPE_2,     /* CID, CSD, 136-bit */
    AL_MMC_RESP_TYPE_3,     /* OCR, 48-bit */
    AL_MMC_RESP_TYPE_4,     /* Fast I/O, 48-bit(eMMC only) */
    AL_MMC_RESP_TYPE_5,     /* Intr request, 48-bit(eMMC only) */
    AL_MMC_RESP_TYPE_6,     /* Published RCA response, 48-bit(SD only) */
    AL_MMC_RESP_TYPE_7      /* Card interface condition, 48-bit(SD only) */
} AL_MMC_RespTypeEnum;

typedef enum
{
    AL_MMC_SD_SECU_NONE     = 0x0,
    AL_MMC_SD_SECU_NOT_USE,
    AL_MMC_SD_SECU_SDSC,
    AL_MMC_SD_SECU_SDHC,
    AL_MMC_SD_SECU_SDXC,
    AL_MMC_SD_SECU_RSVD
} AL_MMC_SdSecurityEnum;

typedef enum
{
    AL_MMC_SD_SPEC_VER_101,     /* 0:0:0 Essential: Not support CMD6/CMD8, up to 2GB */
    AL_MMC_SD_SPEC_VER_110,     /* 1:0:0 Essential: Support CMD6/CMD8, up to 2GB */
    AL_MMC_SD_SPEC_VER_200,     /* 2:0:0 Essential: Support CMD6/CMD8/CMD42, up to 32GB(SDHC) */
    AL_MMC_SD_SPEC_VER_30X,     /* 2:1:0 Essential: Support CMD6/CMD8/CMD42, up to 2TGB(SDXC)
                                         Optional: UHS-I, CMD23*/
    AL_MMC_SD_SPEC_VER_4XX,     /* 2:1:1 Support CMD48/CMD49, UHS-II, DPS(4.00)
                                         Support CMD58/CMD59, Power Management, Grade 1 for UHS-II(4.10) */
    AL_MMC_SD_SPEC_VER_RSVD
} AL_MMC_SdSpecVersEnum;

typedef enum
{
    AL_MMC_VOLT_MODE_HI     = 0x0,
    AL_MMC_VOLT_MODE_DUAL
} AL_MMC_VoltModeEnum;

typedef enum
{
    AL_MMC_ACC_MODE_BYTE    = 0x0,
    AL_MMC_ACC_MODE_SECTOR  = 0x2
} AL_MMC_AccessModeEnum;

typedef enum
{
    AL_MMC_CCT_BC,          /* Broadcast command */
    AL_MMC_CCT_TYPE_BCR,    /* Broadcast command with response */
    AL_MMC_CCT_TYPE_AC,     /* Addressed(point-to-point) commands */
    AL_MMC_CCT_TYPE_ADTC    /* Addressed(point-to-point) data transfer commands , data transfer on DAT*/
} AL_MMC_CardCmdTypeEnum;

typedef enum
{
    AL_MMC_TOUT_CNT_13,
    AL_MMC_TOUT_CNT_14,
    AL_MMC_TOUT_CNT_15,
    AL_MMC_TOUT_CNT_16,
    AL_MMC_TOUT_CNT_17,
    AL_MMC_TOUT_CNT_18,
    AL_MMC_TOUT_CNT_19,
    AL_MMC_TOUT_CNT_20,
    AL_MMC_TOUT_CNT_21,
    AL_MMC_TOUT_CNT_22,
    AL_MMC_TOUT_CNT_23,
    AL_MMC_TOUT_CNT_24,
    AL_MMC_TOUT_CNT_25,
    AL_MMC_TOUT_CNT_26,
    AL_MMC_TOUT_CNT_27,
} AL_MMC_ToutCntEnum;

typedef enum
{
    AL_MMC_CLK_GEN_DIV,
    AL_MMC_CLK_GEN_PROGRAM
} AL_MMC_ClkGenSelEnum;

typedef enum
{
    AL_MMC_RST_HOST_ALL     = BITS_32(MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_SW_RST_ALL_SHIFT,1),
    AL_MMC_RST_HOST_CMD     = BITS_32(MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_SW_RST_CMD_SHIFT,1),
    AL_MMC_RST_HOST_DAT     = BITS_32(MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_SW_RST_DAT_SHIFT,1)
} AL_MMC_RstHostEnum;

typedef enum
{
    AL_MMC_HOST_VER_3,
    AL_MMC_HOST_VER_4
} AL_MMC_HostVerEnum;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 ClkPhase:3;
        AL_U32 ClkSoftRst:1;
        AL_U32 CfgCardDetectN:1;
        AL_U32 CfgCardProt:1;
        AL_U32 CardDetectSig:1;
        AL_U32 CardWrProtSig:1;
        AL_U32 Rsvd31_8:24;
    } Bit;
} AL_MMC_TopCfgUnion;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 XferBlkSize:12;
        AL_U32 SdmaBuffBdary:3; /* Reference to AL_MMC_BuffBdaryEnum */
        AL_U32 Rsvd15:1;
        AL_U32 BlkCnt:16;       /* Not use in ver 4, must set to 0 */
    } Bit;
} AL_MMC_BlockSetUnion;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 DmaEn:1;
        AL_U32 BlkCntEn:1;
        AL_U32 AutoCmdEn:2;         /* Reference to AL_MMC_AutoCmdEnum */
        AL_U32 DatXferDir:1;
        AL_U32 MultiBlkSel:1;
        AL_U32 RespErrChkType:1;    /* Reference to AL_MMC_RespErrChkTypeEnum */
        AL_U32 RespErrChkEn:1;
        AL_U32 RespIntDisable:1;
        AL_U32 Rsvd15_9:7;
        AL_U32 RespTypeSel:2;       /* Reference to AL_MMC_RespTypeLenEnum */
        AL_U32 SubCmdFlag:1;
        AL_U32 CmdCrcChkEn:1;
        AL_U32 CmdIdxChkEn:1;
        AL_U32 DatPresentSel:1;     /* Indicate the data is present and transfer using the DAT line*/
        AL_U32 CmdType:2;           /* Reference to AL_MMC_CmdTypeEnum */
        AL_U32 CmdIdx:6;
        AL_U32 Rsvd31_30:2;
    } Bit;
} AL_MMC_CmdXferModeUnion;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 CmdInhibit:1;
        AL_U32 CmdInhibitDat:1;
        AL_U32 DatLineActive:1; /* Indicate is read or write transaction is executing ont SD/eMMC bus */
        AL_U32 ReTuneReq:1;
        AL_U32 Dat7_4:4;        /* DAT[7:4] Line Signal Level, check DAT line level to recover from errors and for debuging */
        AL_U32 WrXferActive:1;  /* Indicates whether a write transfer is active for SD/eMMC mode */
        AL_U32 RdXferActive:1;
        AL_U32 BuffWrEn:1;      /* Indicate available for writing data for Non-DMA mode */
        AL_U32 BuffRdEn:1;
        AL_U32 Rsvd15_12:4;
        AL_U32 CardInserted:1;
        AL_U32 CardStable:1;
        AL_U32 CardDetectPinLvl:1;
        AL_U32 WrProtSwLvl:1;
        AL_U32 Dat3_0:1;
        AL_U32 CmdLineLvl:1;
        AL_U32 HostRegVol:1;
        AL_U32 Rsvd26:1;
        AL_U32 CmdIssueErr:1;
        AL_U32 SubCmdStat:1;
        AL_U32 InDormantStat:1;
        AL_U32 LaneSync:1;
        AL_U32 Uhs2IfDetect:1;
    } Bit;
} AL_MMC_CurStatUnion;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 LedCtrl:1;           /* Indicate user the card is being accessed */
        AL_U32 DatXferWidth:1;      /* 0: 1-bit, 1: 4-bit */
        AL_U32 HiSpdEn:1;
        AL_U32 DmaSel:2;
        AL_U32 ExtDatXfer:1;        /* 0: DatXferWidth, 1: 8-bit */
        AL_U32 CardDetectTestLvl:1; /* Test mode for card detect, 0: no card, 1: card inserted */
        AL_U32 CardDetectSigSel:1;  /* 0: normal use for card detect, 1: use test mode */
        AL_U32 SdBusPwrVdd1:1;      /* Not used */
        AL_U32 SdBusVolVdd1:3;      /* Not used */
        AL_U32 SdBusPwrVdd2:1;      /* Not used */
        AL_U32 SdBusVolVdd2:3;      /* Not used */
        AL_U32 StopBgReq:1;         /* Stop xfer at block gap */
        AL_U32 ContinueReq:1;       /* Restart xfer */
        AL_U32 RdWaitCtrl:1;        /* Use to enable read wait protocol using DAT[2] for card support read wait */
        AL_U32 IntrAtBgap:1;
        AL_U32 Rsvd23_20:4;
        AL_U32 CardIntrEn:1;
        AL_U32 CardInsertEn:1;
        AL_U32 CardRemovalEn:1;
        AL_U32 Rsvd31_27:5;
    } Bit;
} AL_MMC_CtrlWuBgPwHc1Union;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 IntlClkEn:1;
        AL_U32 IntlClkStable:1;
        AL_U32 SdClkEn:1;
        AL_U32 PllEn:1;
        AL_U32 Rsvd4:1;
        AL_U32 ClkGenSel:1;
        AL_U32 UpperFreqSel:2;
        AL_U32 FreqSel:8;
        AL_U32 ToutCnt:4;
        AL_U32 Rsvd23_20:4;
        AL_U32 SwRstAll:1;
        AL_U32 SwRstCmd:1;
        AL_U32 SwRstDat:1;
        AL_U32 Rsvd31_27:5;
    } Bit;
} AL_MMC_CtrlSrToClkUnion;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 CmdComp:1;
        AL_U32 XferComp:1;
        AL_U32 BgapEvent:1;
        AL_U32 DmaIntr:1;
        AL_U32 BuffWrRdy:1;
        AL_U32 BuffRdRdy:1;
        AL_U32 CardInsert:1;
        AL_U32 CardRemove:1;
        AL_U32 CardIntr:1;
        AL_U32 IntrA:1;
        AL_U32 IntrB:1;
        AL_U32 IntrC:1;
        AL_U32 ReTuneEvent:1;
        AL_U32 FxEvent:1;
        AL_U32 CqeEvent:1;
        AL_U32 ErrIntr:1;       /* Indicate any err occur */
        AL_U32 CmdToutErr:1;
        AL_U32 CmdCrcErr:1;
        AL_U32 CmdEndBitErr:1;
        AL_U32 CmdIdxErr:1;
        AL_U32 DatToutErr:1;
        AL_U32 DatCrcErr:1;
        AL_U32 DatEndBitErr:1;
        AL_U32 CurLmtErr:1;
        AL_U32 AutoCmdErr:1;
        AL_U32 AdmaErr:1;
        AL_U32 TuningErr:1;
        AL_U32 RespErr:1;
        AL_U32 BootAckErr:1;    /* Boot ack err in eMMC mode */
        AL_U32 RsvdVendor:3;
    } Bit;
} AL_MMC_IntrUnion;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 AutoCmd12NotExec:1;
        AL_U32 AutoCmdToutErr:1;
        AL_U32 AutoCmdCrcErr:1;
        AL_U32 AutoCmdEbitErr:1;
        AL_U32 AutoCmdIdxErr:1;
        AL_U32 AutoCmdRespErr:1;
        AL_U32 Rsvd6:1;
        AL_U32 CmdNotIssuedAutoCmd12:1; /* 0: auto cmd err generated by auto CMD23, 1: not exec due to auto CMD12 err */
        AL_U32 Rsvd15_8:8;
        AL_U32 UhsModeSel:3;
        AL_U32 SignalingEn:1;           /* Switch to 1.8V, set for all UHS-I mode */
        AL_U32 DrvStrengthSel:2;
        AL_U32 ExecTuning:1;
        AL_U32 SampleClkSel:1;
        AL_U32 Uhs2IfEn:1;              /* Rsvd */
        AL_U32 Rsvd21:1;
        AL_U32 Amda2LenMode:1;
        AL_U32 Cmd23En:1;
        AL_U32 HostVer4En:1;
        AL_U32 Addr:1;
        AL_U32 AsyncIntrEn:1;
        AL_U32 PresentValEn:1;
    } Bit;
} AL_MMC_Hc2AcUnion;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 ToutClkFreq:6;   /* 0 for info through another method, 0x1-0x3F for 1-63 K/MHz */
        AL_U32 Rsvd6:1;
        AL_U32 ToutClkUnit:1;   /* Reference to AL_MMC_ToutClkUnitEnum */
        AL_U32 BaseClkFreq:8;   /* 0 for info through another method, 0x1-0xFF for 1-255 MHz */
        AL_U32 MaxBlkLen:2;     /* Reference to AL_MMC_MaxBlkLenEnum */
        AL_U32 Embedded8Bit:1;
        AL_U32 AmdaSup:1;
        AL_U32 Rsvd20:1;
        AL_U32 HiSpdSup:1;
        AL_U32 SdmaSup:1;
        AL_U32 SusResSup:1;     /* Suspense/Resume Support */
        AL_U32 Volt33:1;
        AL_U32 Volt30:1;
        AL_U32 Volt18:1;
        AL_U32 SysAddr64V4:1;
        AL_U32 SysAddr64V3:1;
        AL_U32 AsyncIntrSup:1;  /* Asynchronous Interrupt Support (SD Mode only) */
        AL_U32 SlotType:2;      /* Reference to AL_MMC_SlotTypeEnum */
    } Bit;
} AL_MMC_Cap1Union;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 Sdr50Sup:1;          /* UHS-I only */
        AL_U32 Sdr104Sup:1;         /* UHS-I only */
        AL_U32 Ddr50Sup:1;          /* UHS-I only */
        AL_U32 Uhs2Sup:1;           /* UHS-II only */
        AL_U32 DrvTypeA:1;          /* UHS-I only */
        AL_U32 DrvTypeC:1;          /* UHS-I only */
        AL_U32 DrvTypeD:1;          /* UHS-I only */
        AL_U32 Rsvd7:1;
        AL_U32 RetuneCnt:4;         /* UHS-I only */
        AL_U32 Rsvd12:1;
        AL_U32 UseTuningSdr50:1;    /* UHS-I only */
        AL_U32 ReTuningModes:2;     /* UHS-I only */
        AL_U32 ClkMulti:8;          /* 0 for not support clk multiplier, 0x1-0xFF for 2-256 */
        AL_U32 Rsvd26_24:3;
        AL_U32 Amda3Sup:1;
        AL_U32 Vdd218VSup:1;
        AL_U32 Rsvd29:1;
        AL_U32 Rsvd31_30:2;
    } Bit;
} AL_MMC_Cap2Union;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 AutoCmd12NotExec:1;
        AL_U32 AutoCmdToutErr:1;
        AL_U32 AutoCmdCrcErr:1;
        AL_U32 AutoCmdEbitErr:1;
        AL_U32 AutoCmdIdxErr:1;
        AL_U32 AutoCmdRespErr:1;
        AL_U32 Rsvd6:1;
        AL_U32 CmdNotIssueAutoCmd12:1;
        AL_U32 Rsvd15_8:8;
        AL_U32 CmdToutErr:1;
        AL_U32 CmdCrcErr:1;
        AL_U32 CmdEbitErr:1;
        AL_U32 CmdIdxErr:1;
        AL_U32 DatToutErr:1;
        AL_U32 DatCrcErr:1;
        AL_U32 DatEbitErr:1;
        AL_U32 CurLmtErr:1;
        AL_U32 AutoCmdErr:1;
        AL_U32 AdmaErr:1;
        AL_U32 TuningErr:1;
        AL_U32 RespErr:1;
        AL_U32 BootAckErr:1;
        AL_U32 VendorErr1:1;
        AL_U32 VendorErr2:1;
        AL_U32 VendorErr3:1;
    } Bit;
} AL_MMC_ForceStateUnion;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 Rsvd14_0:15;
        AL_U32 Volt27_28:1;
        AL_U32 Volt28_29:1;
        AL_U32 Volt29_30:1;
        AL_U32 Volt30_31:1;
        AL_U32 Volt31_32:1;
        AL_U32 Volt32_33:1;
        AL_U32 Volt33_34:1;
        AL_U32 Volt34_35:1;
        AL_U32 Volt35_36:1;
        AL_U32 Switch18A:1;
        AL_U32 Rsvd28_25:4;
        AL_U32 Uhs2CardStat:1;
        AL_U32 HiCapSupport:1;
        AL_U32 CardPwrUpStatBit:1;
    } Sd;
    struct {
        AL_U32 Rsvd6_0:7;
        AL_U32 Volt170_195:1;       /* Reference to AL_MMC_VoltModeEnum */
        AL_U32 Volt20_26:7;
        AL_U32 Volt27_36:9;         /* Must set all bit to 1, define as AL_MMC_OCR_VOLT_27_36 */
        AL_U32 Rsvd28_24:5;
        AL_U32 AccessMode:2;        /* Reference to AL_MMC_AccessModeEnum */
        AL_U32 CardPwrUpStatBit:1;
    } Emmc;
} AL_MMC_RegOcrUnion;

typedef union
{
    AL_U32 Reg[4];
    struct {
        AL_U32 Rsvd0:1;
        AL_U32 Crc:7;
        AL_U32 Mdt:12;              /* Manufacturing date */
        AL_U32 Rsvd23_20:4;
        AL_U32 Psn31_24:8;          /* Product serial number */
        AL_U32 Psn55_32:24;         /* Product serial number */
        AL_U32 Prv:8;               /* Product revision */
        AL_U32 Pnm71_64:8;          /* Product name */
        AL_U32 Pnm79_72:8;          /* Product name */
        AL_U32 Pnm87_80:8;          /* Product name */
        AL_U32 Pnm95_88:8;          /* Product name */
        AL_U32 Pnm103_96:8;         /* Product name */
        AL_U32 Oid:16;              /* OEM/Application ID */
        AL_U32 Mid:8;               /* Manufacturer ID */
    } Sd;
    struct {
        AL_U32 Rsvd0:1;
        AL_U32 Crc:7;
        AL_U32 Mdt:8;
        AL_U32 Psn31_16:16;
        AL_U32 Psn47_32:16;
        AL_U32 Prv:8;
        AL_U32 Pnm63_56:8;          /* Product name */
        AL_U32 Pnm71_64:8;          /* Product name */
        AL_U32 Pnm79_72:8;          /* Product name */
        AL_U32 Pnm87_80:8;          /* Product name */
        AL_U32 Pnm95_88:8;          /* Product name */
        AL_U32 Pnm103_96:8;         /* Product name */
        AL_U32 Oid:8;
        AL_U32 Cbx:2;               /* Device/BGA */
        AL_U32 Rsvd119_114:6;
        AL_U32 Mid:8;
    } Emmc;
} AL_MMC_RegCidUnion;

typedef union
{
    AL_U32 Reg[4];
    struct {
        AL_U32 Rsvd0:1;
        AL_U32 Crc:7;
        AL_U32 Rsvd9_8:2;
        AL_U32 FileFormat:2;
        AL_U32 TmpWrProt:1;
        AL_U32 PermWrProt:1;
        AL_U32 CpyFlag:1;
        AL_U32 FileFormatGrp:1;
        AL_U32 Rsvd20_16:5;
        AL_U32 WrBlkPartial:1;
        AL_U32 WrBlkLen:4;
        AL_U32 WrSpdFactor:3;
        AL_U32 Rsvd30_29:2;
        AL_U32 WrProtGrpEn:1;
        AL_U32 WrProtGrpSize:7;
        AL_U32 EraseSectorSize:7;
        AL_U32 EraseBlkEn:1;
        AL_U32 DevSizeMulti:3;
        AL_U32 VddWrCurMax:3;
        AL_U32 VddWrCurMin:3;
        AL_U32 VddRdCurMax:3;
        AL_U32 VddRdCurMin:3;
        AL_U32 DevSize63_62:2;
        AL_U32 DevSize73_64:10;
        AL_U32 Rsvd75_74:2;
        AL_U32 DsrImp:1;
        AL_U32 RdBlkMisalign:1;
        AL_U32 WrBlkMisalign:1;
        AL_U32 RdBlkPartial:1;
        AL_U32 RdBlkLen:4;
        AL_U32 CardCmdClass:12;
        AL_U32 TransSpd:8;
        AL_U32 Nsac:8;              /* Data read access-time-2 in CLK cycles (NSAC*100) */
        AL_U32 Taac:8;              /* Data read access-time-1 */
        AL_U32 Rsvd125_120:6;
        AL_U32 CsdStruct:2;
    } SdV1;
    struct {
        AL_U32 Rsvd0:1;
        AL_U32 Crc:7;
        AL_U32 Rsvd9_8:2;
        AL_U32 FileFormat:2;
        AL_U32 TmpWrProt:1;
        AL_U32 PermWrProt:1;
        AL_U32 CpyFlag:1;
        AL_U32 FileFormatGrp:1;
        AL_U32 Rsvd20_16:5;
        AL_U32 WrBlkPartial:1;
        AL_U32 WrBlkLen:4;
        AL_U32 WrSpdFactor:3;
        AL_U32 Rsvd30_29:2;
        AL_U32 WrProtGrpEn:1;
        AL_U32 WrProtGrpSize:7;
        AL_U32 EraseSectorSize:7;
        AL_U32 EraseBlkEn:1;
        AL_U32 Rsvd47:1;
        AL_U32 DevSize63_48:16;
        AL_U32 DevSize69_64:6;          //
        AL_U32 Rsvd75_70:6;
        AL_U32 DsrImp:1;
        AL_U32 RdBlkMisalign:1;
        AL_U32 WrBlkMisalign:1;
        AL_U32 RdBlkPartial:1;
        AL_U32 RdBlkLen:4;
        AL_U32 CardCmdClass:12;
        AL_U32 TransSpd:8;
        AL_U32 Nsac:8;              /* Data read access-time-2 in CLK cycles (NSAC*100) */
        AL_U32 Taac:8;              /* Data read access-time-1 */
        AL_U32 Rsvd125_120:6;
        AL_U32 CsdStruct:2;
    } SdV2;
    struct {
        AL_U32 Rsvd0:1;
        AL_U32 Crc:7;
        AL_U32 Ecc:2;           //
        AL_U32 FileFormat:2;
        AL_U32 TmpWrProt:1;
        AL_U32 PermWrProt:1;
        AL_U32 CpyFlag:1;
        AL_U32 FileFormatGrp:1;
        AL_U32 ContentProtApp:1;    //
        AL_U32 Rsvd20_17:4;
        AL_U32 WrBlkPartial:1;
        AL_U32 WrBlkLen:4;
        AL_U32 WrSpdFactor:3;
        AL_U32 DefEcc:2;            //
        AL_U32 WrProtGrpEn:1;
        AL_U32 WrProtGrpSize:5;     //
        AL_U32 EraseGrpMulti:5;     //
        AL_U32 EraseGrpSize:5;      //
        AL_U32 DevSizeMulti:3;
        AL_U32 VddWrCurMax:3;
        AL_U32 VddWrCurMin:3;
        AL_U32 VddRdCurMax:3;
        AL_U32 VddRdCurMin:3;
        AL_U32 DevSize63_62:2;
        AL_U32 DevSize73_64:10;
        AL_U32 Rsvd75_74:2;
        AL_U32 DsrImp:1;
        AL_U32 RdBlkMisalign:1;
        AL_U32 WrBlkMisalign:1;
        AL_U32 RdBlkPartial:1;
        AL_U32 RdBlkLen:4;
        AL_U32 CardCmdClass:12;
        AL_U32 TransSpd:8;
        AL_U32 Nsac:8;              /* Data read access-time-2 in CLK cycles (NSAC*100) */
        AL_U32 Taac:8;              /* Data read access-time-1 */
        AL_U32 Rsvd121_120:2;       //
        AL_U32 SpecVers:4;          //
        AL_U32 CsdStruct:2;
    } Emmc;
} AL_MMC_RegCsdUnion;

typedef union
{
    AL_U8 Reg[AL_MMC_EXT_CSD_LEN];
    struct {
        AL_U8 Rsvd211_0[212];
        AL_U8 SecCount[4];          /*[215:212]*/
        AL_U8 Rsvd505_216[290];
        AL_U8 Rsvd511_506[6];
    } Emmc;
} AL_MMC_RegExtCsdUnion;

typedef union
{
    AL_U32 Reg[2];
    struct {
        AL_U32 Rsvd31_0:32;
        AL_U32 CmdSupport:4;        /* Reference to AL_MMC_SdSpecVersEnum */
        AL_U32 Rsvd41_36:6;
        AL_U32 SdSpec4:1;           /* Reference to AL_MMC_SdSpecVersEnum */
        AL_U32 ExSecurity:4;
        AL_U32 SdSpec3:1;           /* Reference to AL_MMC_SdSpecVersEnum */
        AL_U32 SdBusWidth:4;        /* Since the SD Memory Card shall support at least the two bus modes 1-bit or 4-bit
                                       width, then any SD Card shall set at least bits 0 and 2 (SD_BUS_WIDTH="0101") */
        AL_U32 SdSecurity:3;        /* Reference to AL_MMC_SdSecurityEnum */
        AL_U32 DatStatAfterErase:1;
        AL_U32 SdSpec:4;            /* Reference to AL_MMC_SdSpecVersEnum */
        AL_U32 ScrStructure:4;
    } Sd;
} AL_MMC_RegScrUnion;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 Rsvd2_0:3;
        AL_U32 AkeSeqErr:1;         /* SD only */
        AL_U32 Rsvd4:1;
        AL_U32 AppCmd:1;
        AL_U32 ExcEvent:1;          /*eMMC only, exception event */
        AL_U32 SwitchErr:1;         /*eMMC only */
        AL_U32 RdyForDat:1;         /* Buffer empty signaling on the bus */
        AL_U32 CurState:4;          /* Reference to AL_MMC_CardStatEnum */
        AL_U32 EraseReset:1;
        AL_U32 CardEccDis:1;        /* SD only, cmd has been executed without using internal ECC */
        AL_U32 WpEraseSkip:1;       /* Only partial address space was erased due to existing write protected blocks */
        AL_U32 CidCsdOverWr:1;      /* Write dat of rd-only field not match card content */
        AL_U32 Rsvd18_17:2;
        AL_U32 Err:1;               /* A general or an unknown error occurred during the operation. */
        AL_U32 CcErr:1;             /* Internal card controller error not releated to host controller */
        AL_U32 CardEccFail:1;       /* Card internal ECC was applied but failed to correct the data */
        AL_U32 IllegalCmd:1;        /* Command not legal for the card state */
        AL_U32 CmdCrcErr:1;         /* The CRC check of the previous command failed */
        AL_U32 LockUnlockFail:1;    /* Set when a sequence or password error has been detected in lock/unlock Device command */
        AL_U32 DevIsLocked:1;
        AL_U32 IsWrProtBlk:1;
        AL_U32 EraseParam:1;        /* An invalid selection of erase groups for erase occurred */
        AL_U32 EraseSeqErr:1;       /* An error in the sequence of erase commands occurred */
        AL_U32 BlkLenErr:1;         /* The transferred block length is not allowed for this card, or the number of
                                       transferred bytes does not match the block length. */
        AL_U32 AddrMisAlign:1;      /* A misaligned address which did not match the block length was used in the command */
        AL_U32 AddrOutRange:1;      /* The command’s address argument was out of the allowed range for this Device */
    } Bit;
} AL_MMC_CardStateUnion;

typedef enum
{
    AL_MMC_EMMC_CMD6_ACCESS_CMD_SET,
    AL_MMC_EMMC_CMD6_ACCESS_SET_BITS,
    AL_MMC_EMMC_CMD6_ACCESS_CLR_BITS,
    AL_MMC_EMMC_CMD6_ACCESS_WR_BYTE
} AL_MMC_EmmcCmd6AccessEnum;

typedef enum
{
    AL_MMC_SD_CMD6_ACCESS_DS_SDR12,
    AL_MMC_SD_CMD6_ACCESS_HS_SDR25,
    AL_MMC_SD_CMD6_ACCESS_SDR50,
    AL_MMC_SD_CMD6_ACCESS_SDR104,
    AL_MMC_SD_CMD6_ACCESS_DDR50
} AL_MMC_SdCmd6AccessEnum;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 Access:4;
        AL_U32 CmdSys:4;
        AL_U32 DrvStren:4;
        AL_U32 PwrLmt:4;
        AL_U32 Rsvd31_16:16;
    } Sd;
    struct {
        AL_U32 CmdSet:3;
        AL_U32 SetZero7_3:5;
        AL_U32 Value:8;
        AL_U32 Index:8;
        AL_U32 Access:2;
        AL_U32 SetZero31_26:6;
    } Emmc;
} AL_MMC_Cmd6ArgUnion;

/* --------------------Top Ctrl Register-------------------- */



/* Phase about cclk_rx clock signal */
static inline AL_U32 AlMmc_ll_GetClkPhase(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR :
                             MMC1_TOP_CFG_CTRL_ADDR, MMC_TOP_NS_CFG_CTRL_CLK_PHASE_SEL_SHIFT,
                             MMC_TOP_NS_CFG_CTRL_CLK_PHASE_SEL_SIZE);
}

static inline AL_VOID AlMmc_ll_SetClkPhase(AL_REG BaseAddr, AL_U32 Phase)
{
    AL_REG32_SET_BITS((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR : MMC1_TOP_CFG_CTRL_ADDR,
                      MMC_TOP_NS_CFG_CTRL_CLK_PHASE_SEL_SHIFT, MMC_TOP_NS_CFG_CTRL_CLK_PHASE_SEL_SIZE, Phase);
}

/* clock soft reset */
static inline AL_BOOL AlMmc_ll_IsClkSoftRst(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR :
                                     MMC1_TOP_CFG_CTRL_ADDR, MMC_TOP_NS_CFG_CTRL_CCLK_SOFT_RST_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetClkSoftRst(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR : MMC1_TOP_CFG_CTRL_ADDR,
                      MMC_TOP_NS_CFG_CTRL_CCLK_SOFT_RST_SHIFT, IsEnable);
}

/* Card detect signal, 0 valid, valid when switch_card_detect_n is 1 */
static inline AL_BOOL AlMmc_ll_IsCardNotDetect(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR :
                                     MMC1_TOP_CFG_CTRL_ADDR, MMC_TOP_NS_CFG_CTRL_CFG_CARD_DETECT_N_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetCardNotDetect(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR : MMC1_TOP_CFG_CTRL_ADDR,
                      MMC_TOP_NS_CFG_CTRL_CFG_CARD_DETECT_N_SHIFT, IsEnable);
}

/* Card write protect signal, valid when switch_card_write_prot is 1 */
static inline AL_BOOL AlMmc_ll_IsCardWrProt(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR :
                                     MMC1_TOP_CFG_CTRL_ADDR, MMC_TOP_NS_CFG_CTRL_CFG_CARD_WRITE_PROT_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetCardWrProt(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR : MMC1_TOP_CFG_CTRL_ADDR,
                      MMC_TOP_NS_CFG_CTRL_CFG_CARD_WRITE_PROT_SHIFT, IsEnable);
}

/* Switch card detect, 1 for configured card detect, 0 for signal from EMIO or MIO */
static inline AL_MMC_SwCardSigEnum AlMmc_ll_GetSwCardDetect(AL_REG BaseAddr)
{
    return (AL_MMC_SwCardSigEnum)AL_REG32_GET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR :
                                                  MMC1_TOP_CFG_CTRL_ADDR, MMC_TOP_NS_CFG_CTRL_SWITCH_CARD_DETECT_N_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetSwCardDetect(AL_REG BaseAddr, AL_MMC_SwCardSigEnum Signal)
{
    AL_REG32_SET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR : MMC1_TOP_CFG_CTRL_ADDR,
                      MMC_TOP_NS_CFG_CTRL_SWITCH_CARD_DETECT_N_SHIFT, Signal);
}

/* Switch card write protect, 1 for configured card detect, 0 for signal from EMIO or MIO */
static inline AL_MMC_SwCardSigEnum AlMmc_ll_GetSwCardWrPort(AL_REG BaseAddr)
{
    return (AL_MMC_SwCardSigEnum)AL_REG32_GET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR :
                                     MMC1_TOP_CFG_CTRL_ADDR, MMC_TOP_NS_CFG_CTRL_SWITCH_CARD_WRITE_PROT_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetSwCardWrPort(AL_REG BaseAddr, AL_MMC_SwCardSigEnum Signal)
{
    AL_REG32_SET_BIT((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR : MMC1_TOP_CFG_CTRL_ADDR,
                      MMC_TOP_NS_CFG_CTRL_SWITCH_CARD_WRITE_PROT_SHIFT, Signal);
}

/* Top config register */
static inline AL_U32 AlMmc_ll_ReadTopCfg(AL_REG BaseAddr)
{
    return AL_REG32_READ((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR : MMC1_TOP_CFG_CTRL_ADDR);
}

static inline AL_VOID AlMmc_ll_WriteTopCfg(AL_REG BaseAddr, AL_U32 Val)
{
    AL_REG32_WRITE((BaseAddr == MMC0__BASE_ADDR) ? MMC0_TOP_CFG_CTRL_ADDR : MMC1_TOP_CFG_CTRL_ADDR, Val);
}

/* --------------------MMC/Block Register-------------------- */



/**
 * 32-bit Block Count or an SDMA System Address
 * @note    ver4 = 0:
 *              SDMA: 32 bit SDMA tarans addr
 *              Auto CMD23 = 1 and Non-DMA/ADMA: 32-bit block count(must non-zero if 16-bit block count equal zero)
 *          ver4 = 1: 32-bit block count
*/
static inline AL_U32 AlMmc_ll_ReadBlkCnt_SdmaSysAddr(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_SDMASA_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteBlkCnt_SdmaSysAddr(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_SDMASA_R_OFFSET, Value);
}

/* Transfer Block Size */
static inline AL_U32 AlMmc_ll_GetTransBlkSize(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_BLOCKCOUNT_R_BLOCKSIZE_R_OFFSET,
                             MMC_BLOCKCOUNT_R_BLOCKSIZE_R_XFER_BLOCK_SIZE_SHIFT,
                             MMC_BLOCKCOUNT_R_BLOCKSIZE_R_XFER_BLOCK_SIZE_SIZE);
}

static inline AL_VOID AlMmc_ll_SetTransBlkSize(AL_REG BaseAddr, AL_U32 Size)
{
    AL_REG32_SET_BITS(BaseAddr + MMC_BLOCKCOUNT_R_BLOCKSIZE_R_OFFSET,
                      MMC_BLOCKCOUNT_R_BLOCKSIZE_R_XFER_BLOCK_SIZE_SHIFT,
                      MMC_BLOCKCOUNT_R_BLOCKSIZE_R_XFER_BLOCK_SIZE_SIZE, Size);
}

/* SDMA Buffer Boundary */
static inline AL_MMC_BuffBdaryEnum AlMmc_ll_GetSdmaBufBdary(AL_REG BaseAddr)
{
    return (AL_MMC_BuffBdaryEnum)AL_REG32_GET_BITS(BaseAddr + MMC_BLOCKCOUNT_R_BLOCKSIZE_R_OFFSET,
                                                   MMC_BLOCKCOUNT_R_BLOCKSIZE_R_SDMA_BUF_BDARY_SHIFT,
                                                   MMC_BLOCKCOUNT_R_BLOCKSIZE_R_SDMA_BUF_BDARY_SIZE);
}

static inline AL_VOID AlMmc_ll_SetSdmaBufBdary(AL_REG BaseAddr, AL_MMC_BuffBdaryEnum Bdary)
{
    AL_REG32_SET_BITS(BaseAddr + MMC_BLOCKCOUNT_R_BLOCKSIZE_R_OFFSET,
                      MMC_BLOCKCOUNT_R_BLOCKSIZE_R_SDMA_BUF_BDARY_SHIFT,
                      MMC_BLOCKCOUNT_R_BLOCKSIZE_R_SDMA_BUF_BDARY_SIZE, Bdary);
}

/**
 * 16-bit Block Count
 * @note    ver4 = 0:
 *              SDMA: 16-bit block count
 *              Auto CMD23 = 1 and Non-DMA/ADMA: must set 0 for 32-bit block count use
 *          ver4 = 1:
 *              set 0: 32-bit block count use
*/
static inline AL_U32 AlMmc_ll_GetBlkCnt(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_BLOCKCOUNT_R_BLOCKSIZE_R_OFFSET,
                                                   MMC_BLOCKCOUNT_R_BLOCKSIZE_R_BLOCK_CNT_SHIFT,
                                                   MMC_BLOCKCOUNT_R_BLOCKSIZE_R_BLOCK_CNT_SIZE);
}

static inline AL_VOID AlMmc_ll_SetBlkCnt(AL_REG BaseAddr, AL_U32 BlkCnt)
{
    AL_REG32_SET_BITS(BaseAddr + MMC_BLOCKCOUNT_R_BLOCKSIZE_R_OFFSET,
                      MMC_BLOCKCOUNT_R_BLOCKSIZE_R_BLOCK_CNT_SHIFT,
                      MMC_BLOCKCOUNT_R_BLOCKSIZE_R_BLOCK_CNT_SIZE, BlkCnt);
}

/**
 * BLOCKCOUNT_R_BLOCKSIZE_R
*/
static inline AL_U32 AlMmc_ll_ReadBlkCnt_BlkSize(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_BLOCKCOUNT_R_BLOCKSIZE_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteBlkCnt_BlkSize(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_BLOCKCOUNT_R_BLOCKSIZE_R_OFFSET, Value);
}

/**
 * Command Argument, specified in bits 39-8 of the Command format
*/
static inline AL_U32 AlMmc_ll_ReadArgument(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_ARGUMENT_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteArgument(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_ARGUMENT_R_OFFSET, Value);
}

/**
 * DMA Enable
*/
static inline AL_BOOL AlMmc_ll_IsDmaEn(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_DMA_ENABLE_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetDmaEn(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_DMA_ENABLE_SHIFT, IsEnable);
}

/**
 * Block Count Enable:
 * @note    0x0: ADMA mode or infinite transfer
 *          0x1: enable for block count register
*/
static inline AL_BOOL AlMmc_ll_IsBlkCntEn(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                     MMC_CMD_R_XFER_MODE_R_BLOCK_COUNT_ENABLE_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetBlkCntEn(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_BLOCK_COUNT_ENABLE_SHIFT, IsEnable);
}

/**
 * Auto Command Enable
 * @note    SDIO: must set as 00b
*/
static inline AL_MMC_AutoCmdEnum AlMmc_ll_GetAutoCmdEn(AL_REG BaseAddr)
{
    return (AL_MMC_AutoCmdEnum)AL_REG32_GET_BITS(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                                 MMC_CMD_R_XFER_MODE_R_AUTO_CMD_ENABLE_SHIFT,
                                                 MMC_CMD_R_XFER_MODE_R_AUTO_CMD_ENABLE_SIZE);
}

static inline AL_VOID AlMmc_ll_SetAutoCmdEn(AL_REG BaseAddr, AL_MMC_AutoCmdEnum IsEnable)
{
    AL_REG32_SET_BITS(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_AUTO_CMD_ENABLE_SHIFT,
                      MMC_CMD_R_XFER_MODE_R_AUTO_CMD_ENABLE_SIZE, IsEnable);
}

/**
 * Data Transfer Direction Select
 * @note    WRITE: SD/eMMC card to host driver
 *          READ: all other commands
*/
static inline AL_MMC_TransDirEnum AlMmc_ll_GetDatTransDir(AL_REG BaseAddr)
{
    return (AL_MMC_TransDirEnum)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                                 MMC_CMD_R_XFER_MODE_R_DATA_XFER_DIR_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetDatTransDir(AL_REG BaseAddr, AL_MMC_TransDirEnum Dir)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_DATA_XFER_DIR_SHIFT, Dir);
}

/**
 * Multi/Single Block Select
 * @note    0x0: single block, not necessary to set block count register
 *          0x1: multiple block transfer
*/
static inline AL_BOOL AlMmc_ll_IsMultiBlkSel(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                     MMC_CMD_R_XFER_MODE_R_MULTI_BLK_SEL_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetMultiBlkSel(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_MULTI_BLK_SEL_SHIFT, IsEnable);
}

/**
 * Response Type R1/R5 when response error check is selected
 * @note    R1: for memory, checked:
 *                                  OUT_OF_RANGE
 *                                  ADDRESS_ERROR
 *                                  BLOCK_LEN_ERROR
 *                                  WP_VIOLATION
 *                                  CARD_IS_LOCKED
 *                                  COM_CRC_ERROR
 *                                  CARD_ECC_FAILED
 *                                  CC_ERROR
 *                                  ERROR
 *          R5: for SDIO, checked:
 *                                  COM_CRC_ERROR
 *                                  ERROR
 *                                  FUNCTION_NUMBER
 *                                  OUT_OF_RANGE
*/
static inline AL_MMC_RespErrChkTypeEnum AlMmc_ll_GetRespErrChkType(AL_REG BaseAddr)
{
    return (AL_MMC_RespErrChkTypeEnum)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                                       MMC_CMD_R_XFER_MODE_R_RESP_TYPE_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetRespErrChkType(AL_REG BaseAddr, AL_MMC_RespErrChkTypeEnum Type)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_RESP_TYPE_SHIFT, Type);
}

/**
 * Response Error Check Enable
 * @note    0x0: disable check
 *          0x1: enable check, set resp intr disable to 1, will generate error in intr status reg without a intr
*/
static inline AL_BOOL AlMmc_ll_IsRespErrChkEn(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                                 MMC_CMD_R_XFER_MODE_R_RESP_ERR_CHK_ENABLE_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetRespErrChkEn(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_RESP_ERR_CHK_ENABLE_SHIFT, IsEnable);
}

/**
 * Response Interrupt Disable
 * @note    0x0: enable, host driver wait for cmd complete intr and check respnse register
 *          0x1: disable, set err check enable to 1, cmd complete intr disable and host controller check response error
*/
static inline AL_BOOL AlMmc_ll_IsRespIntrDisable(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                                 MMC_CMD_R_XFER_MODE_R_RESP_INT_DISABLE_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetRespIntrDisable(AL_REG BaseAddr, AL_BOOL IsDisable)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_RESP_INT_DISABLE_SHIFT, IsDisable);
}

/**
 * Response Type Select
*/
static inline AL_MMC_RespTypeLenEnum AlMmc_ll_GetRespTypeSel(AL_REG BaseAddr)
{
    return (AL_MMC_RespTypeLenEnum)AL_REG32_GET_BITS(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                                     MMC_CMD_R_XFER_MODE_R_RESP_TYPE_SELECT_SHIFT,
                                                     MMC_CMD_R_XFER_MODE_R_RESP_TYPE_SELECT_SIZE);
}

static inline AL_VOID AlMmc_ll_SetRespTypeSel(AL_REG BaseAddr, AL_MMC_RespTypeLenEnum Type)
{
    AL_REG32_SET_BITS(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_RESP_TYPE_SELECT_SHIFT,
                      MMC_CMD_R_XFER_MODE_R_RESP_TYPE_SELECT_SIZE, Type);
}

/**
 * Sub Command Flag
*/
static inline AL_BOOL AlMmc_ll_IsSubCmd(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                     MMC_CMD_R_XFER_MODE_R_SUB_CMD_FLAG_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetSubCmd(AL_REG BaseAddr, AL_BOOL IsSubCmd)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_SUB_CMD_FLAG_SHIFT, IsSubCmd);
}

/**
 * Command CRC Check Enable
 * @note    must set 0 for cmd with no response, R3 and R4 response
 *          must set 1 for tuning cmd
*/
static inline AL_BOOL AlMmc_ll_IsCmdCrcChkEn(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                     MMC_CMD_R_XFER_MODE_R_CMD_CRC_CHK_ENABLE_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetCmdCrcChkEn(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_CMD_CRC_CHK_ENABLE_SHIFT, IsEnable);
}

/**
 * Command Index Check Enable
 * @note    must set 0 for cmd with no response, R2, R3 and R4 response
 *          must set 1 for tuning cmd
*/
static inline AL_BOOL AlMmc_ll_IsCmdIdxChkEn(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                     MMC_CMD_R_XFER_MODE_R_CMD_IDX_CHK_ENABLE_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetCmdIdxChkEn(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_CMD_IDX_CHK_ENABLE_SHIFT, IsEnable);
}

/**
 * Data Present Select
 * @note    0x0: data present using DAT line
 *          0x1: cmd using CMD line or using busy signal on DAT[0] line
*/
static inline AL_BOOL AlMmc_ll_IsDatPresentSel(AL_REG BaseAddr)
{
    return (AL_BOOL)AL_REG32_GET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                     MMC_CMD_R_XFER_MODE_R_DATA_PRESENT_SEL_SHIFT);
}

static inline AL_VOID AlMmc_ll_SetDatPresentSel(AL_REG BaseAddr, AL_BOOL IsEnable)
{
    AL_REG32_SET_BIT(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_DATA_PRESENT_SEL_SHIFT, IsEnable);
}

/**
 * Command Type
 * @note    set to ABORT while issuing abort CMD using CMD12/CMD52 or reset CMD using CMD0/CMD52
*/
static inline AL_MMC_CmdTypeEnum AlMmc_ll_GetCmdType(AL_REG BaseAddr)
{
    return (AL_MMC_CmdTypeEnum)AL_REG32_GET_BITS(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                                  MMC_CMD_R_XFER_MODE_R_CMD_TYPE_SHIFT,
                                                  MMC_CMD_R_XFER_MODE_R_CMD_TYPE_SIZE);
}

static inline AL_VOID AlMmc_ll_SetCmdType(AL_REG BaseAddr, AL_MMC_CmdTypeEnum Type)
{
    AL_REG32_SET_BITS(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_CMD_TYPE_SHIFT,
                      MMC_CMD_R_XFER_MODE_R_CMD_TYPE_SIZE, Type);
}

/**
 * Command Index
 * @note    set to command number specified in bits 45-40 of the command format
*/
static inline AL_MMC_CmdIdxEnum AlMmc_ll_GetCmdIndex(AL_REG BaseAddr)
{
    return (AL_MMC_CmdIdxEnum)AL_REG32_GET_BITS(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET,
                                                  MMC_CMD_R_XFER_MODE_R_CMD_INDEX_SHIFT,
                                                  MMC_CMD_R_XFER_MODE_R_CMD_INDEX_SIZE);
}

static inline AL_VOID AlMmc_ll_SetCmdIndex(AL_REG BaseAddr, AL_MMC_CmdIdxEnum Type)
{
    AL_REG32_SET_BITS(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, MMC_CMD_R_XFER_MODE_R_CMD_INDEX_SHIFT,
                      MMC_CMD_R_XFER_MODE_R_CMD_INDEX_SIZE, Type);
}

/**
 * CMD_R_XFER_MODE_R
*/
static inline AL_U32 AlMmc_ll_ReadCmd_XferMode(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteCmd_XferMode(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_CMD_R_XFER_MODE_R_OFFSET, Value);
}

/**
 * RESP01_R
 * @note    reflect 39-8 bit of SD/eMMC response
 *          store TRANS_ABORT_CCMD in UHS-II mode
 *          for auto cmd, 39-8 bit field updated in RESP67_R register
*/
static inline AL_U32 AlMmc_ll_ReadResp01(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_RESP01_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteResp01(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_RESP01_R_OFFSET, Value);
}

/**
 * RESP23_R
 * @note    reflect 71-40 bit of SD/eMMC response
 *          reserved in UHS-II mode
*/
static inline AL_U32 AlMmc_ll_ReadResp23(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_RESP23_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteResp23(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_RESP23_R_OFFSET, Value);
}

/**
 * RESP45_R
 * @note    reflect 103-72 bit of SD/eMMC response
 *          store lower 4-byte CMD12 response in UHS-II mode
*/
static inline AL_U32 AlMmc_ll_ReadResp45(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_RESP45_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteResp45(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_RESP45_R_OFFSET, Value);
}

/**
 * RESP67_R
 * @note    reflect 135-104 bit of SD/eMMC response
 *          store upper 4-byte CMD12 response in UHS-II mode
 *          for auto cmd, 39-8 bit field updated here
*/
static inline AL_U32 AlMmc_ll_ReadResp67(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_RESP67_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteResp67(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_RESP67_R_OFFSET, Value);
}

/**
 * Buffer Data
*/
static inline AL_U32 AlMmc_ll_ReadBufferData(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_BUF_DATA_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteBufferData(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_BUF_DATA_R_OFFSET, Value);
}

/**
 * Present State Register
*/
static inline AL_U32 AlMmc_ll_ReadCurState(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_PSTATE_REG_OFFSET);
}

/**
 * WUP_CTRL_R_BGAP_CTRL_R_PWR_CTRL_R_HOST_CTRL1_R
*/
static inline AL_U32 AlMmc_ll_ReadCtrl_Wup_Bgap_Pwr_Host1(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_WUP_CTRL_R_BGAP_CTRL_R_PWR_CTRL_R_HOST_CTRL1_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteCtrl_Wup_Bgap_Pwr_Host1(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_WUP_CTRL_R_BGAP_CTRL_R_PWR_CTRL_R_HOST_CTRL1_R_OFFSET, Value);
}

static inline AL_MMC_ToutCntEnum AlMmc_ll_GetToutCnt(AL_REG BaseAddr)
{
    return (AL_MMC_ToutCntEnum)AL_REG32_GET_BITS(BaseAddr + MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_OFFSET,
                                                 MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_TOUT_CNT_SHIFT,
                                                 MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_TOUT_CNT_SIZE);
}

static inline AL_VOID AlMmc_ll_SetToutCnt(AL_REG BaseAddr, AL_MMC_ToutCntEnum Cnt)
{
    AL_REG32_SET_BITS(BaseAddr + MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_OFFSET,
                      MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_TOUT_CNT_SHIFT,
                      MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_TOUT_CNT_SIZE, Cnt);
}

/**
 * SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R
*/
static inline AL_U32 AlMmc_ll_ReadSwRst_Ctrl_Tout_Clk(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteSwRst_Ctrl_Tout_Clk(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_SW_RST_R_TOUT_CTRL_R_CLK_CTRL_R_OFFSET, Value);
}

/**
 * ERROR_INT_STAT_R_NORMAL_INT_STAT_R
*/
static inline AL_U32 AlMmc_ll_ReadIntrStat(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_ERROR_INT_STAT_R_NORMAL_INT_STAT_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteIntrStat(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_ERROR_INT_STAT_R_NORMAL_INT_STAT_R_OFFSET, Value);
}

/**
 * ERROR_INT_STAT_EN_R_NORMAL_INT_STAT_EN_R
*/
static inline AL_U32 AlMmc_ll_ReadIntrStatEn(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_ERROR_INT_STAT_EN_R_NORMAL_INT_STAT_EN_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteIntrStatEn(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_ERROR_INT_STAT_EN_R_NORMAL_INT_STAT_EN_R_OFFSET, Value);
}

/**
 * ERROR_INT_SIGNAL_EN_R_NORMAL_INT_SIGNAL_EN_R
*/
static inline AL_U32 AlMmc_ll_ReadIntrSigEn(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_ERROR_INT_SIGNAL_EN_R_NORMAL_INT_SIGNAL_EN_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteIntrSigEn(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_ERROR_INT_SIGNAL_EN_R_NORMAL_INT_SIGNAL_EN_R_OFFSET, Value);
}

/**
 * HOST_CTRL2_R_AUTO_CMD_STAT_R
*/
static inline AL_U32 AlMmc_ll_ReadAutoCmdStat_CtrlHost2(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_HOST_CTRL2_R_AUTO_CMD_STAT_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteAutoCmdStat_CtrlHost2(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_HOST_CTRL2_R_AUTO_CMD_STAT_R_OFFSET, Value);
}

/**
 * MMC_CAPABILITIES1_R_OFFSET
*/
static inline AL_U32 AlMmc_ll_ReadCapability1(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_CAPABILITIES1_R_OFFSET);
}

/**
 * MMC_CAPABILITIES2_R_OFFSET
*/
static inline AL_U32 AlMmc_ll_ReadCapability2(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_CAPABILITIES2_R_OFFSET);
}

/**
 * Get vdd1 max current 33V
*/
static inline AL_U32 AlMmc_ll_GetVdd1MaxCur33V(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_CURR_CAPABILITIES1_R_OFFSET, MMC_CURR_CAPABILITIES1_R_MAX_CUR_33V_SHIFT,
                             MMC_CURR_CAPABILITIES1_R_MAX_CUR_33V_SIZE);
}

/**
 * Get vdd1 max current 30V
*/
static inline AL_U32 AlMmc_ll_GetVdd1MaxCur30V(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_CURR_CAPABILITIES1_R_OFFSET, MMC_CURR_CAPABILITIES1_R_MAX_CUR_30V_SHIFT,
                             MMC_CURR_CAPABILITIES1_R_MAX_CUR_30V_SIZE);
}

/**
 * Get vdd1 max current 18V
*/
static inline AL_U32 AlMmc_ll_GetVdd1MaxCur18V(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_CURR_CAPABILITIES1_R_OFFSET, MMC_CURR_CAPABILITIES1_R_MAX_CUR_18V_SHIFT,
                             MMC_CURR_CAPABILITIES1_R_MAX_CUR_18V_SIZE);
}

/**
 * Get vdd2 max current 18V
*/
static inline AL_U32 AlMmc_ll_GetVdd2MaxCur18V(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_CURR_CAPABILITIES2_R_OFFSET, MMC_CURR_CAPABILITIES2_R_MAX_CUR_VDD2_18V_SHIFT,
                             MMC_CURR_CAPABILITIES2_R_MAX_CUR_VDD2_18V_SIZE);
}

/**
 * FORCE_ERROR_INT_STAT_R_FORCE_AUTO_CMD_STAT_R
*/
static inline AL_VOID AlMmc_ll_WriteForceStatErrIntr_AutoCmd(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_FORCE_ERROR_INT_STAT_R_FORCE_AUTO_CMD_STAT_R_OFFSET, Value);
}

/**
 * ADMA_ERR_STAT_R
*/
static inline AL_U32 AlMmc_ll_ReadAdmaErrStat(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_ADMA_ERR_STAT_R_OFFSET);
}

/**
 * ADMA_SA_LOW_R
*/
static inline AL_U32 AlMmc_ll_ReadAdmaSysAddrLow(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_ADMA_SA_LOW_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteAdmaSysAddrLow(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_ADMA_SA_LOW_R_OFFSET, Value);
}

/**
 * PRESET_DS_R_PRESET_INIT_R
*/
static inline AL_U32 AlMmc_ll_ReadPresetDs_Init(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_PRESET_DS_R_PRESET_INIT_R_OFFSET);
}


/**
 * PRESET_HS_R
*/
static inline AL_U32 AlMmc_ll_ReadPresetSdr12_Hs(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_PRESET_HS_R_OFFSET);
}

/**
 * PRESET_SDR50_R_PRESET_SDR25_R
*/
static inline AL_U32 AlMmc_ll_ReadPresetSdr50_Sdr25(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_PRESET_SDR50_R_PRESET_SDR25_R_OFFSET);
}

/**
 * PRESET_DDR50_R_PRESET_SDR104_R
*/
static inline AL_U32 AlMmc_ll_ReadPresetDdr50_Sdr104(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_PRESET_DDR50_R_PRESET_SDR104_R_OFFSET);
}

/**
 * PRESET_UHS2_R
*/
static inline AL_U32 AlMmc_ll_ReadPresetUhs2(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_PRESET_UHS2_R_OFFSET);
}

/**
 * ADMA_ID_LOW_R
*/
static inline AL_U32 AlMmc_ll_ReadAdmaIdLow(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_ADMA_ID_LOW_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteAdmaIdLow(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_ADMA_ID_LOW_R_OFFSET, Value);
}

/**
 * P_EMBEDDED_CNTRL
*/
static inline AL_U32 AlMmc_ll_GetPtrEmbeddedCtrl(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_P_EMBEDDED_CNTRL_OFFSET, MMC_P_EMBEDDED_CNTRL_REG_OFFSET_ADDR_SHIFT,
                             MMC_P_EMBEDDED_CNTRL_REG_OFFSET_ADDR_SIZE);
}

/**
 * P_VENDOR_SPECIFIC_AREA
*/
static inline AL_U32 AlMmc_ll_GetVendorSpecificArea(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_P_VENDOR2_SPECIFIC_AREA_P_VENDOR_SPECIFIC_AREA_OFFSET,
                             MMC_P_VENDOR2_SPECIFIC_AREA_P_VENDOR_SPECIFIC_AREA_REG_OFFSET_ADDR_0_SHIFT,
                             MMC_P_VENDOR2_SPECIFIC_AREA_P_VENDOR_SPECIFIC_AREA_REG_OFFSET_ADDR_0_SIZE);
}

/**
 * P_VENDOR2_SPECIFIC_AREA
*/
static inline AL_U32 AlMmc_ll_GetVendor2SpecificArea(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_P_VENDOR2_SPECIFIC_AREA_P_VENDOR_SPECIFIC_AREA_OFFSET,
                             MMC_P_VENDOR2_SPECIFIC_AREA_P_VENDOR_SPECIFIC_AREA_REG_OFFSET_ADDR_1_SHIFT,
                             MMC_P_VENDOR2_SPECIFIC_AREA_P_VENDOR_SPECIFIC_AREA_REG_OFFSET_ADDR_1_SIZE);
}

/**
 * SLOT_INTR_STATUS_R
*/
static inline AL_U32 AlMmc_ll_GetSlotIntrStatus(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_HOST_CNTRL_VERS_R_SLOT_INTR_STATUS_R_OFFSET,
                             MMC_HOST_CNTRL_VERS_R_SLOT_INTR_STATUS_R_INTR_SLOT_SHIFT,
                             MMC_HOST_CNTRL_VERS_R_SLOT_INTR_STATUS_R_INTR_SLOT_SIZE);
}

/**
 * Specification Version Number
*/
static inline AL_U32 AlMmc_ll_GetSpecificVerNum(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_HOST_CNTRL_VERS_R_SLOT_INTR_STATUS_R_OFFSET,
                             MMC_HOST_CNTRL_VERS_R_SLOT_INTR_STATUS_R_SPEC_VERSION_NUM_SHIFT,
                             MMC_HOST_CNTRL_VERS_R_SLOT_INTR_STATUS_R_SPEC_VERSION_NUM_SIZE);
}

/**
 * Vendor Version Number
*/
static inline AL_U32 AlMmc_ll_GetVendorVerNum(AL_REG BaseAddr)
{
    return AL_REG32_GET_BITS(BaseAddr + MMC_HOST_CNTRL_VERS_R_SLOT_INTR_STATUS_R_OFFSET,
                             MMC_HOST_CNTRL_VERS_R_SLOT_INTR_STATUS_R_VENDOR_VERSION_NUM_SHIFT,
                             MMC_HOST_CNTRL_VERS_R_SLOT_INTR_STATUS_R_VENDOR_VERSION_NUM_SIZE);
}

/* --------------------MMC/Embedded Control Block Register-------------------- */



/**
 * EMBEDDED_CTRL_R
*/
static inline AL_U32 AlMmc_ll_ReadEmbeddedCtrl(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_EMBEDDED_CTRL_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteEmbeddedCtrl(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_EMBEDDED_CTRL_R_OFFSET, Value);
}

/* --------------------MMC/Vendor 2 Block Register-------------------- */



/**
 * CQCAP
*/
static inline AL_U32 AlMmc_ll_ReadCmdQueueCap(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_CQCAP_OFFSET);
}



/* --------------------MMC/Vendor 1 Block Register-------------------- */



/**
 * MSHC_VER_ID_R
*/
static inline AL_U32 AlMmc_ll_ReadVerId(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_MSHC_VER_ID_R_OFFSET);
}

/**
 * MSHC_VER_TYPE_R
*/
static inline AL_U32 AlMmc_ll_ReadVerType(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_MSHC_VER_TYPE_R_OFFSET);
}

/**
 * MSHC_CTRL_R
 * TODO: describe detail
*/
static inline AL_U32 AlMmc_ll_ReadMshcCtrl(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_MSHC_CTRL_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteMshcCtrl(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_MSHC_CTRL_R_OFFSET, Value);
}

/**
 * MBIU_CTRL_R
*/
static inline AL_U32 AlMmc_ll_ReadMbiuCtrl(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_MBIU_CTRL_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteMbiuCtrl(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_MBIU_CTRL_R_OFFSET, Value);
}

/**
 * BOOT_CTRL_R_EMMC_CTRL_R
*/
static inline AL_U32 AlMmc_ll_ReadCtrlBoot_Emmc(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_BOOT_CTRL_R_EMMC_CTRL_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteCtrlBoot_Emmc(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_BOOT_CTRL_R_EMMC_CTRL_R_OFFSET, Value);
}

/**
 * AT_CTRL_R
*/
static inline AL_U32 AlMmc_ll_ReadAutoTuningCtrl(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_AT_CTRL_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_WriteAutoTuningCtrl(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + MMC_AT_CTRL_R_OFFSET, Value);
}

/**
 * AT_STAT_R
*/
static inline AL_U32 AlMmc_ll_ReadAutoTuningStat(AL_REG BaseAddr)
{
    return AL_REG32_READ(BaseAddr + MMC_AT_STAT_R_OFFSET);
}

static inline AL_VOID AlMmc_ll_SetCenterPhaseCode(AL_REG BaseAddr, AL_U32 Value)
{
    AL_REG32_SET_BITS(BaseAddr + MMC_AT_STAT_R_OFFSET, MMC_AT_STAT_R_CENTER_PH_CODE_SHIFT,
                      MMC_AT_STAT_R_CENTER_PH_CODE_SIZE, Value);
}

/* --------------------MMC/Common Read/Write Register-------------------- */



/**
 * Common register read
*/
static inline AL_U32 AlMmc_ll_ReadReg(AL_REG BaseAddr, AL_U32 Offset)
{
    return AL_REG32_READ(BaseAddr + Offset);
}

static inline AL_VOID AlMmc_ll_WriteReg(AL_REG BaseAddr, AL_U32 Offset, AL_U32 Value)
{
    AL_REG32_WRITE(BaseAddr + Offset, Value);
}

#ifdef __cplusplus
}
#endif

#endif
