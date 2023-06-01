/**
 * @file    al_mmc_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-05-19
 * @brief   mmc device driver
 */

#ifndef AL_MMC_DEV_H
#define AL_MMC_DEV_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_mmc_ll.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/
/**
 * @brief  Error code enum
 */
typedef enum
{
    MMC_ERR_INVALID_CARD            = 0x100,    /* receive message with error*/
    MMC_ERR_LINE_INHIBIT,
    MMC_ERR_CMD_TOUT,
    MMC_ERR_CMD_CRC,
    MMC_ERR_CMD_END_BIT,
    MMC_ERR_CMD_IDX,
    MMC_ERR_DATA_TOUT,
    MMC_ERR_DATA_CRC,
    MMC_ERR_DATA_END_BIT,
    MMC_ERR_CUR_LMT,
    MMC_ERR_AUTO_CMD,
    MMC_ERR_ADMA,
    MMC_ERR_TUNING,
    MMC_ERR_RESP,
    MMC_ERR_BOOT_ACK,
    MMC_ERR_RST_LINE_TIMEOUT,
    MMC_ERR_CARD_NOT_INSERTED,
    MMC_ERR_STATE_NOT_READY,
    MMC_ERR_TRANS_BUSY,
    MMC_ERR_HOST_RST,
    MMC_ERR_WR_RDY_TIMEOUT,
    MMC_ERR_RD_RDY_TIMEOUT,
    MMC_ERR_XFER_COMP_TIMEOUT,
} AL_MMC_ErrCodeEnum;

#define AL_MMC_ERR_INVALID_DEVID        AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_INVALID_DEVID)
#define AL_MMC_ERR_INVALID_CHNID        AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_INVALID_CHNID)
#define AL_MMC_ERR_ILLEGAL_PARAM        AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM)
#define AL_MMC_ERR_EXIST                AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_EXIST)
#define AL_MMC_ERR_UNEXIST              AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_UNEXIST)
#define AL_MMC_ERR_NULL_PTR             AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_NULL_PTR)
#define AL_MMC_ERR_NOT_CONFIG           AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_CONFIG)
#define AL_MMC_ERR_NOT_SUPPORT          AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_NOT_SUPPORT)
#define AL_MMC_ERR_TIMEOUT              AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_TIMEOUT)
#define AL_MMC_ERR_BUSY                 AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, AL_ERR_BUSY)
#define AL_MMC_ERR_INVALID_CARD         AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_INVALID_CARD)
#define AL_MMC_ERR_LINE_INHIBIT         AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_LINE_INHIBIT)
#define AL_MMC_ERR_CMD_TOUT             AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_CMD_TOUT)
#define AL_MMC_ERR_CMD_CRC              AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_CMD_CRC)
#define AL_MMC_ERR_CMD_END_BIT          AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_CMD_END_BIT)
#define AL_MMC_ERR_CMD_IDX              AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_CMD_IDX)
#define AL_MMC_ERR_DATA_TOUT            AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_DATA_TOUT)
#define AL_MMC_ERR_DATA_CRC             AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_DATA_CRC)
#define AL_MMC_ERR_DATA_END_BIT         AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_DATA_END_BIT)
#define AL_MMC_ERR_CUR_LMT              AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_CUR_LMT)
#define AL_MMC_ERR_AUTO_CMD             AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_AUTO_CMD)
#define AL_MMC_ERR_ADMA                 AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_ADMA)
#define AL_MMC_ERR_TUNING               AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_TUNING)
#define AL_MMC_ERR_RESP                 AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_RESP)
#define AL_MMC_ERR_BOOT_ACK             AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_BOOT_ACK)
#define AL_MMC_ERR_RST_LINE_TIMEOUT     AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_RST_LINE_TIMEOUT)
#define AL_MMC_ERR_CARD_NOT_INSERTED    AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_CARD_NOT_INSERTED)
#define AL_MMC_ERR_STATE_NOT_READY      AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_STATE_NOT_READY)
#define AL_MMC_ERR_TRANS_BUSY           AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_TRANS_BUSY)
#define AL_MMC_ERR_HOST_RST             AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_HOST_RST)
#define AL_MMC_ERR_WR_RDY_TIMEOUT       AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_WR_RDY_TIMEOUT)
#define AL_MMC_ERR_RD_RDY_TIMEOUT       AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_RD_RDY_TIMEOUT)
#define AL_MMC_ERR_XFER_COMP_TIMEOUT    AL_DEF_ERR(AL_MMC, AL_ERR_LEVEL_ERROR, MMC_ERR_XFER_COMP_TIMEOUT)


#define AL_MMC_CMD8_VOL_PATTERN     0x1AAU  /* CMD8 voltage pattern */

/**************************** Type Definitions *******************************/

typedef enum
{
    AL_MMC_CARD_TYPE_SD,
    AL_MMC_CARD_TYPE_EMMC,
    AL_MMC_CARD_TYPE_AUTO_DETECT,
    AL_MMC_CARD_TYPE_MAX
} AL_MMC_CardTypeEnum;

typedef enum
{
    AL_MMC_CARD_VER_101,
    AL_MMC_CARD_VER_110,
    AL_MMC_CARD_VER_200,
    AL_MMC_CARD_VER_30X,
    AL_MMC_CARD_VER_4XX,
} AL_MMC_CardVerEnum;

typedef enum
{
    AL_MMC_DMA_MODE_SDMA,
    AL_MMC_DMA_MODE_NONE,       /* Follow IP spec, not support ADMA1, so set to No-DMA */
    AL_MMC_DMA_MODE_ADMA2,
    AL_MMC_DMA_MODE_ADMA3
} AL_MMC_DmaModeEnum;

typedef enum
{
    AL_MMC_BUS_WIDTH_1BIT   = (0x1),
    AL_MMC_BUS_WIDTH_4BIT   = (0x4),
    AL_MMC_BUS_WIDTH_8BIT   = (0x8)
} AL_MMC_BusWidthEnum;

typedef enum
{
    AL_MMC_MAX_SPD_DS,      /* Max 25MHz, 12MB/s(4-bit), 26MB/s(eMMC 8-bit), 3.3V */
    AL_MMC_MAX_SPD_HS,      /* Max 50MHz, 25MB/s(4-bit), 52MB/s(eMMC 8-bit), 3.3V */
    AL_MMC_MAX_SPD_SDR12,   /* 25MHz, 12MB/s, 1.8V */
    AL_MMC_MAX_SPD_SDR25,   /* 50MHz, 25MB/s, 1.8V */
    AL_MMC_MAX_SPD_SDR50    /* 100MHz, 50MB/s, 1.8V */
} AL_MMC_SpdModeEnum;

/**
 * @brief   Bus frequency
 * @note    SD: <25Mhz, 3.3V: DS
 *              >25Mhz, <50Mhz, 3.3V: HS
 *              <25Mhz, 1.8V: SDR12
 *              >25Mhz, <50Mhz, 1.8V: SDR25
 *              >50Mhz, <100Mhz, 1.8V: SDR50
 *          eMMC: <26Mhz: Backwards Compatibility with legacy MMC card(DS)
 *                >26Mhz, <52Mhz: high speed SDR
 */
typedef enum
{
    AL_MMC_FREQ_KHZ_100     = 100,
    AL_MMC_FREQ_KHZ_DEF     = 400,
    AL_MMC_FREQ_KHZ_500     = 500,
    AL_MMC_FREQ_KHZ_1000    = 1000,
    AL_MMC_FREQ_KHZ_5000    = 5000,
    AL_MMC_FREQ_KHZ_10000   = 10000,
    AL_MMC_FREQ_KHZ_25000   = 25000,
    AL_MMC_FREQ_KHZ_26000   = 26000,
    AL_MMC_FREQ_KHZ_50000   = 50000,
    AL_MMC_FREQ_KHZ_52000   = 52000
} AL_MMC_FreqKhzEnum;

typedef enum
{
    AL_MMC_INTR_CMD_COMP_MASK           = BITS_32(0,1),
    AL_MMC_INTR_XFER_COMP_MASK          = BITS_32(1,1),
    AL_MMC_INTR_BGAP_MASK               = BITS_32(2,1),
    AL_MMC_INTR_DMA_MASK                = BITS_32(3,1),
    AL_MMC_INTR_BUF_WR_RDY_MASK         = BITS_32(4,1),
    AL_MMC_INTR_BUF_RD_RDY_MASK         = BITS_32(5,1),
    AL_MMC_INTR_CARD_INSERT_MASK        = BITS_32(6,1),
    AL_MMC_INTR_CARD_REMOVE_MASK        = BITS_32(7,1),
    AL_MMC_INTR_CARD_MASK               = BITS_32(8,1),
    AL_MMC_INTR_RETUNE_EVENT_MASK       = BITS_32(12,1),
    AL_MMC_INTR_FX_EVENT_MASK           = BITS_32(13,1),
    AL_MMC_INTR_CQE_EVENT_MASK          = BITS_32(14,1),
    AL_MMC_INTR_ERR_MASK                = BITS_32(15,1),
    AL_MMC_INTR_ERR_CMD_TOUT_MASK       = BITS_32(16,1),
    AL_MMC_INTR_ERR_CMD_CRC_MASK        = BITS_32(17,1),
    AL_MMC_INTR_ERR_CMD_END_BIT_MASK    = BITS_32(18,1),
    AL_MMC_INTR_ERR_CMD_IDX_MASK        = BITS_32(19,1),
    AL_MMC_INTR_ERR_DAT_TOUT_MASK       = BITS_32(20,1),
    AL_MMC_INTR_ERR_DAT_CRC_MASK        = BITS_32(21,1),
    AL_MMC_INTR_ERR_DAT_END_BIT_MASK    = BITS_32(22,1),
    AL_MMC_INTR_ERR_CUR_LMT_MASK        = BITS_32(23,1),
    AL_MMC_INTR_ERR_AUTO_CMD_MASK       = BITS_32(24,1),
    AL_MMC_INTR_ERR_ADMA_MASK           = BITS_32(25,1),
    AL_MMC_INTR_ERR_TUNING_MASK         = BITS_32(26,1),
    AL_MMC_INTR_ERR_RESP_MASK           = BITS_32(27,1),
    AL_MMC_INTR_ERR_BOOT_ACK_MASK       = BITS_32(28,1),
} AL_MMC_IntrEnum;

/**
 * @brief  Event id enum
 */
typedef enum
{
    AL_MMC_EVENT_WRITE_DONE     = BITS_32(0,1),
    AL_MMC_EVENT_READ_DONE      = BITS_32(1,1),
} AL_MMC_EventIdEnum;

/**
 * @brief  Module running state enum
 */
typedef enum {
    AL_MMC_STATE_NOT_INIT       = (0x0),
    AL_MMC_STATE_IDLE           = BITS_32(1,1),
    AL_MMC_STATE_READY          = BITS_32(2,1),
    AL_MMC_STATE_STBY           = BITS_32(3,1),
    AL_MMC_STATE_TRANS          = BITS_32(4,1),
    AL_MMC_STATE_WRITE_BUSY     = BITS_32(5,1),
    AL_MMC_STATE_READ_BUSY      = BITS_32(6,1),
    AL_MMC_STATE_ALL_MASK       = BITS_32(0,32),
} AL_MMC_StateEnum;

/**
 * @brief  Io ctl cmd enum
 */
typedef enum
{
    AL_MMC_IOCTL_SET_BIT_WIDTH,
} AL_MMC_IoCtlCmdEnum;

typedef enum
{
    AL_MMC_ADMA_ACT_NO_OPS      = 0x0,  /* Do not execute current line and go to next line */
    AL_MMC_ADMA_ACT_SD_CMD      = 0x1,  /* Fix set for ADMA3 cmd descriptor SD cmd */
    AL_MMC_ADMA_ACT_RSVD        = 0x2,  /* Same as Nop. Do not execute current line and go to next line */
    AL_MMC_ADMA_ACT_TRANS       = 0x4,  /* Transfer data of one descriptor line */
    AL_MMC_ADMA_ACT_LINK        = 0x6,  /* Link to another descriptor */
    AL_MMC_ADMA_ACT_ALL_MASK    = 0x7
} AL_MMC_AdmaActEnum;

/* TODO: support after osal interface done */
#ifdef USE_ROTS
typedef struct
{
    AL_U32 Sig;
} AL_MMC_IntrSigStruct;
#endif
/**
 * @brief  Event struct
 */
typedef struct
{
    AL_MMC_EventIdEnum  EventId;
    AL_U32              EventData;
} AL_MMC_EventStruct;

typedef AL_VOID (*AL_MMC_EventCallBack)(AL_MMC_EventStruct *Event, AL_VOID *CallBackRef);

/**
 * @brief  Event callback struct
 */
typedef struct
{
    AL_MMC_EventCallBack    Func;
    AL_VOID                 *Ref;
} AL_MMC_CallBackStruct;

typedef union
{
    AL_U32 Desc[2];
    struct {
        AL_U32 Valid:1;         /* Indicates validity of a descriptor line */
        AL_U32 End:1;           /* End of descriptor */
        AL_U32 Int:1;           /* Force to generate ADMA interrupt */
        AL_U32 Act:3;           /* Reference to AL_MMC_AdmaActEnum
                                   ADMA2: Set follow AL_MMC_AdmaActEnum
                                   ADMA3 Cmd Descriptor: must set AL_MMC_ADMA_ACT_SD_CMD
                                   ADMA3 Integrated Descriptor: must set AL_MMC_ADMA_ACT_ALL_MASK */
        AL_U32 UpperLen:10;     /* ADMA2: upper 10 bit of length
                                   ADMA3: reserved */
        AL_U32 LowerLen:16;     /* ADMA2: lower 16 bit of length
                                   ADMA3: reserved */
        AL_U32 AddrArgPtr:32;   /* ADMA2: address of data in system memory
                                   ADMA3 Cmd Descriptor: params are written to host control register from 00h - 0fh
                                   ADMA3 Integrated Descriptor: Pointer to command descriptor */
    } Bit;
} AL_MMC_AdmaDescUnion;

typedef struct
{
    AL_MMC_CardTypeEnum     CardType;       /* Card set */
    AL_MMC_FreqKhzEnum      FreqKhz;
    AL_MMC_DmaModeEnum      DmaMode;
    AL_MMC_BusWidthEnum     BusWidth;
    AL_U32                  Switch1v8;
    AL_U32                  AutoGenAdmaTblEn;   /* When dma mode is ADMA2/ADMA3, is enable auto generate describe table or not */
    AL_MMC_BuffBdaryEnum    DmaBdary;
} AL_MMC_InitStruct;

typedef struct
{
    AL_MMC_Cap1Union        Cap1;
    AL_MMC_Cap2Union        Cap2;
} AL_MMC_HostCapStruct;

typedef struct
{
    AL_MMC_CardTypeEnum     CardType;       /* Card identified */
    AL_MMC_CardVerEnum      CardVer;
    AL_MMC_FreqKhzEnum      FreqKhz;
    AL_MMC_BusWidthEnum     BusWidth;
    AL_BOOL                 HiCap;
    AL_MMC_RegCidUnion      CardId;
    AL_MMC_RegCsdUnion      CardSpecDat;
    AL_U32                  RealAddr;
    AL_U32                  CardCap;        /* KByte */
    AL_MMC_BlkLenEnum       BlkLen;
    AL_U32                  DevSize;
} AL_MMC_CardInfoStruct;

typedef struct
{
    AL_MMC_HwConfigStruct   HwConfig;
    AL_MMC_InitStruct       Config;
    AL_MMC_HostCapStruct    HostCap;
    AL_MMC_CardInfoStruct   CardInfo;
    AL_MMC_CallBackStruct   EventCallBack;
    AL_MMC_StateEnum        State;
    AL_MMC_CardStateUnion   CardState;
    AL_MMC_AdmaDescUnion    AdmaDesc[AL_MMC_ADMA_TABLE_SIZE];
#ifdef USE_ROTS
    AL_MMC_IntrSigStruct    IntrSig;
#endif
} AL_MMC_DevStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/

AL_MMC_HwConfigStruct *AlMmc_Dev_LookupConfig(AL_U32 DeviceId);
AL_S32 AlMmc_Dev_RegisterEventCallBack(AL_MMC_DevStruct *Dev, AL_MMC_CallBackStruct *CallBack);
AL_S32 AlMmc_Dev_UnRegisterEventCallBack(AL_MMC_DevStruct *Dev);
AL_S32 AlMmc_Dev_Init(AL_MMC_DevStruct *Dev, AL_MMC_HwConfigStruct *HwConfig, AL_MMC_InitStruct *InitConfig);
AL_S32 AlMmc_Dev_Write(AL_MMC_DevStruct *Dev, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt);
AL_S32 AlMmc_Dev_Read(AL_MMC_DevStruct *Dev, AL_U8 *Buf, AL_U32 BlkOffset, AL_U32 BlkCnt);
AL_S32 AlMmc_Dev_RstHostController(AL_MMC_DevStruct *Dev, AL_MMC_RstHostEnum Rst);
AL_VOID AlMmc_Dev_IntrHandler(void *Instance);
AL_S32 AlMmc_Dev_IoCtl(AL_MMC_DevStruct *Dev, AL_MMC_IoCtlCmdEnum Cmd, AL_VOID *Data);

#ifdef __cplusplus
}
#endif

#endif
