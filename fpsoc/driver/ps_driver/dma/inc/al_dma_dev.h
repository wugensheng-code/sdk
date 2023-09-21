/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef __AL_DMA_DEV_H
#define __AL_DMA_DEV_H


/** @defgroup DMA_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#include "al_dma_hw.h"
/***************************** MACRO Definition ******************************/
#define DMA_EVENT_START_BIT 6

typedef enum
{
    DMA_ERR_FAIL = AL_ERR_MAX,    /* receive message with error*/
    DMA_ERR_BUFF_NOT_ALIGN,
    DMA_ERR_ILLEGAL_LENGTH,
} AL_DMA_ErrCodeEnum;

#define AL_DMA_ERR_ILLEGAL_PARAM        AL_DEF_ERR(AL_DMA, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM)
#define AL_DMA_ERR_NULL_PTR             AL_DEF_ERR(AL_DMA, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR)
#define AL_DMA_ERR_NOT_SUPPORT          AL_DEF_ERR(AL_DMA, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT)
#define AL_DMA_ERR_TIMEOUT              AL_DEF_ERR(AL_DMA, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT)
#define AL_DMA_ERR_BUSY                 AL_DEF_ERR(AL_DMA, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY)
#define AL_DMA_ERR_FAIL                 AL_DEF_ERR(AL_DMA, AL_LOG_LEVEL_ERROR, DMA_ERR_FAIL)
#define AL_DMA_ERR_BUFF_NOT_ALIGN       AL_DEF_ERR(AL_DMA, AL_LOG_LEVEL_ERROR, DMA_ERR_BUFF_NOT_ALIGN)
#define AL_DMA_ERR_ILLEGAL_LENGTH       AL_DEF_ERR(AL_DMA, AL_LOG_LEVEL_ERROR, DMA_ERR_ILLEGAL_LENGTH)

#define AL_DMA_EVENTS_TO_ERRS(Events)   (AL_DEF_ERR(AL_DMA, AL_LOG_LEVEL_ERROR, \
                                                        (Events << DMA_EVENT_START_BIT)))

/** Dev status */
#define DMA_STATE_STOPPED           HAL_BIT(0)
#define DMA_STATE_EXECUTING         HAL_BIT(1)
#define DMA_STATE_WFE               HAL_BIT(2)
#define DMA_STATE_FAULTING          HAL_BIT(3)
#define DMA_STATE_COMPLETING        HAL_BIT(4)
#define DMA_STATE_WFP               HAL_BIT(5)
#define DMA_STATE_KILLING           HAL_BIT(6)
#define DMA_STATE_FAULT_COMPLETE    HAL_BIT(7)
#define DMA_STATE_CACHEMISS         HAL_BIT(8)
#define DMA_STATE_UPDTPC            HAL_BIT(9)
#define DMA_STATE_ATBARRIER         HAL_BIT(10)
#define DMA_STATE_QUEUEBUSY         HAL_BIT(11)
#define DMA_STATE_INVALID           HAL_BIT(15)

#define DMA_STABLE_STATES   (DMA_STATE_STOPPED | DMA_STATE_EXECUTING | DMA_STATE_WFE | DMA_STATE_FAULTING)

#define DMA_MAX_CHAN    8
#define DMA_MAX_IRQS    32
#define DMA_MAX_PERI    32
#define DMA_MAX_BURST   16

/*
 * With 256 bytes, we DMA do more than 2.5MB and 5MB xfers per req
 * at 1byte/burst for P<->M and M<->M respectively.
 * For typical scenario, at 1word/burst, 10MB and 20MB xfers per req
 * should be enough for P<->M and M<->M respectively.
 */
#define DMA_CHAN_BUF_LEN        128

/***************************** Structure Definition **************************/

/**
 * enum AL_DMA_TransEnum - dma transfer mode and direction indicator
 */
typedef enum {
    DMA_MEM_TO_MEM, /**< Async/Memcpy mode */
    DMA_MEM_TO_DEV, /**< Slave mode & From Memory to Device */
    DMA_DEV_TO_MEM, /**< Slave mode & From Device to Memory */
    DMA_DEV_TO_DEV, /**< Slave mode & From Device to Device */
    DMA_TRANS_NONE,
} AL_DMA_TransEnum;

/**
 * enum AL_DMA_ChBusWidthEnum - defines bus width of the DMA slave
 * device, source or target buses
 */
typedef enum {
    DMA_CH_BUSWIDTH_UNDEFINED = 0,
    DMA_CH_BUSWIDTH_1_BYTE    = 1,
    DMA_CH_BUSWIDTH_2_BYTES   = 2,
    DMA_CH_BUSWIDTH_3_BYTES   = 3,
    DMA_CH_BUSWIDTH_4_BYTES   = 4,
    DMA_CH_BUSWIDTH_8_BYTES   = 8,
    DMA_CH_BUSWIDTH_16_BYTES  = 16,
    DMA_CH_BUSWIDTH_32_BYTES  = 32,
    DMA_CH_BUSWIDTH_64_BYTES  = 64,
} AL_DMA_ChBusWidthEnum;

/** enum DMA_CACHECTRL - Dev cache control */
typedef enum {
    CCTRL0, /**< Noncacheable and nonbufferable */
    CCTRL1, /**< Bufferable only */
    CCTRL2, /**< Cacheable, but do not allocate */
    CCTRL3, /**< Cacheable and bufferable, but do not allocate */
    INVALID1, /**< AWCACHE = 0x1000 */
    INVALID2,
    CCTRL6, /**< Cacheable write-through, allocate on writes only */
    CCTRL7, /**< Cacheable write-back, allocate on writes only */
} AL_DMA_CacheCtrlEnum;

/** enum DMA_BYTESWAP - Dev byte Swap control */
typedef enum {
    SWAP_NO,
    SWAP_2,
    SWAP_4,
    SWAP_8,
    SWAP_16,
} AL_DMA_ByteSwapEnum;

/**
 * enum DMA_COND - dma transfer mode
 */
typedef enum {
    SINGLE,
    BURST,
    ALWAYS,
} AL_DMA_CondEnum;

/* DMA Channel Register Structure Define */
/* CSR: channel status */
/* CPC: channel program counter */
typedef  struct
{
    __I  AL_U32 CSR;
    __I  AL_U32 CPC;
} AL_DMA_ChRegStruct;

/* AXI status and loop counter register */
/* SAR: source address */
/* DAR: destination address */
/* CCR: channel control */
/* LC0: loop counter 0 */
/* LC1: loop counter 1 */
/* PADDING: reserved */
typedef struct
{
    __I  AL_U32 SAR;
    __I  AL_U32 DAR;
    __I  AL_U32 CCR;
    __I  AL_U32 LC0;
    __I  AL_U32 LC1;
         AL_U32 PADDING[3];
} AL_DMA_AxiRegStruct;

/* DMAC control register summary */
typedef struct
{
    __I  AL_U32         DSR;                        /* Address Offset: 0x0000 */
    __I  AL_U32         DPC;                        /* Address Offset: 0x0004 */
         AL_U32         RESERVED0008[6];            /* Address Offset: 0x0008 */
    __IO AL_U32         INTEN;                      /* Address Offset: 0x0020 */
    __I  AL_U32         EVENT_RIS;                  /* Address Offset: 0x0024 */
    __I  AL_U32         INTMIS;                     /* Address Offset: 0x0028 */
    __O  AL_U32         INTCLR;                     /* Address Offset: 0x002C */
    __I  AL_U32         FSRD;                       /* Address Offset: 0x0030 */
    __I  AL_U32         FSRC;                       /* Address Offset: 0x0034 */
    __IO AL_U32         FTRD;                       /* Address Offset: 0x0038 */
         AL_U32         RESERVED003C;               /* Address Offset: 0x003C */
    __I  AL_U32         FTR[8];                     /* Address Offset: 0x0040 */
         AL_U32         RESERVED0058[40];           /* Address Offset: 0x0060 */
    AL_DMA_ChRegStruct  CHAN_STS[8];                /* Address Offset: 0x0100 */
         AL_U32         RESERVED0130[176];          /* Address Offset: 0x0140 */
    AL_DMA_AxiRegStruct CHAN_CFG[8];                /* Address Offset: 0x0400 */
         AL_U32         RESERVED04C0[512];          /* Address Offset: 0x0500 */
    __I  AL_U32         DBGSTATUS;                  /* Address Offset: 0x0D00 */
    __O  AL_U32         DBGCMD;                     /* Address Offset: 0x0D04 */
    __O  AL_U32         DBGINST[2];                 /* Address Offset: 0x0D08 */
         AL_U32         RESERVED0D10[60];           /* Address Offset: 0x0D10 */
    __I  AL_U32         CR[5];                      /* Address Offset: 0x0E00 */
    __I  AL_U32         CRDN;                       /* Address Offset: 0x0E14 */
         AL_U32         RESERVED0E18[26];           /* Address Offset: 0x0E18 */
    __IO AL_U32         WD;                         /* Address Offset: 0x0E80 */
} AL_DMA_DmaCtrlRegStruct;

/** Dev soc configuration */
typedef struct
{
    AL_U32  SecureMode;
    AL_U32  DataBusWidth; /**< In number of bits */
    AL_U32  DataBufDep;
    AL_U32  NumChan;
    AL_U32  NumPeri;
    AL_U32  PeriNs;
    AL_U32  NumEvents;
    AL_U32  IrqNs;
} AL_DMA_IpCfgInfoStruct;

/** Dev request config */
typedef struct
{
    AL_U32                  SrcInc;
    AL_U32                  DstInc;
    AL_BOOL                 NonSecure;
    AL_BOOL                 Privileged;
    AL_BOOL                 InsnAccess;     /*
                                             * For now, the SRC & DST protection levels
                                             * and burst size/length are assumed same.
                                             */
    AL_U32                  BurstLen;
    AL_U32                  BurstSize;      /**< bytes */
    AL_DMA_CacheCtrlEnum    DstCacheCtl;
    AL_DMA_CacheCtrlEnum    SrcCacheCtl;
    AL_DMA_ByteSwapEnum     Swap;
} AL_DMA_ReqCfgStruct;

/** Dev block descriptor struct. */
typedef struct
{
    AL_U32  SrcAddr;    /**< Source starting address */
    AL_U32  DstAddr;    /**< Destination starting address */
    AL_U32  length;     /**< Number of bytes for the xfer */
} AL_DMA_XferStruct;

/**
 * A Dev Desc consisits of a request config struct, a xfer descriptor,
 * a pointer pointing to generated Dev program, and execution result.
 */
typedef struct {
    AL_U8               Peri;
    AL_DMA_TransEnum    Dir;
    AL_U32              BytesReq;
    AL_BOOL             Cyclic;
    AL_U32              NumPeriods;
    AL_DMA_ReqCfgStruct ReqCfg;
    AL_DMA_XferStruct   Xfer;
    AL_U16              SrcInterlaceSize;
    AL_U16              DstInterlaceSize;
    AL_VOID             *McBuf;
} AL_DMA_DescStruct;

typedef struct
{
    AL_U32              Ccr;
    AL_DMA_DescStruct   *Desc;
} AL_DMA_XferSpecStruct;

/**
 * @brief  Event id enum
 */
typedef enum
{
    AL_DMA_EVENT_TRANS_DONE     = BIT(0),
} AL_DMA_EventIdEnum;

/**
 * @brief  Event struct
 */
typedef struct
{
    AL_DMA_EventIdEnum  EventId;
    AL_U32              EventData;
} AL_DMA_EventStruct;

typedef AL_VOID (*AL_DMA_EventCallBack)(AL_DMA_EventStruct *Event, AL_VOID *CallBackRef);

struct DMA_DevStruct;
/**
 * The DMA_CHAN Data is a struct to book keep individual channel of
 * the DMAC.
 */
typedef struct
{
    AL_BOOL                 Used;
    AL_U16                  PeriId;
    AL_U16                  ChanId;
    AL_U32                  BurstSize;
    AL_U32                  BurstLen;
    AL_U16                  SrcInterlaceSize;
    AL_U16                  DstInterlaceSize;
    AL_DMA_DescStruct       Desc;
    AL_DMA_TransEnum        Dir;
    struct DMA_DevStruct    *Dev;
    AL_U8 CACHE_LINE_ALIGN  McBuf[DMA_CHAN_BUF_LEN];
    AL_U32 CACHE_LINE_ALIGN FifoAddr;
    AL_DMA_EventCallBack    EventCallBack;
    AL_VOID                 *EventCallBackRef;
} AL_DMA_ChStruct;

/**
 * The Dev driver instance data structure. A pointer to an instance data
 * structure is passed around by functions to refer to a specific driver
 * instance.
 */
typedef struct DMA_DevStruct
{
    AL_DMA_HwConfigStruct   *HwCfg;
    AL_DMA_DmaCtrlRegStruct *CtrlReg;
    AL_DMA_ChStruct         Chans[AL_DMA_CHAN_COUNT];
    AL_DMA_IpCfgInfoStruct  IpCfgInfo;
    AL_DMA_CondEnum         PeripReqType;   /* Current default 0 and not config */
    AL_BOOL                 Init;
} AL_DMA_DevStruct;

/**
 * AL_DMA_ChCfgStruct - dma slave channel runtime config
 */
typedef struct {
    AL_DMA_TransEnum        Direction; /**< Transfer direction. */
    AL_DMA_ChBusWidthEnum   SrcAddrWidth; /**< The width in bytes of the source,
                                        *  Legal values: 1, 2, 4, 8.
                                        */
    AL_DMA_ChBusWidthEnum   DstAddrWidth; /**< The same as SrcAddrWidth. */
    AL_U32                  SrcAddr; /**< The source physical address. */
    AL_U32                  DstAddr; /**< The destination physical address. */
    AL_U16                  SrcMaxBurst; /**< The maximum number of words (note: words, as in
                                            *  units of the SrcAddrWidth member, not bytes) that
                                            *  can be sent in one burst to the device, Typically
                                            *  something like half the FIFO depth on I/O Peri so
                                            *  you don't overflow it.
                                            */
    AL_U16                  DstMaxBurst; /**< The same as SrcMaxBurst for destination. */
    AL_U16                  SrcInterlaceSize; /**< The interlace size for src mem increase */
    AL_U16                  DstInterlaceSize; /**< The interlace size for dst mem increase */
} AL_DMA_ChCfgStruct ;

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup DMA_Public_Function_Declare Public Function Declare
 *  @{
 */

AL_DMA_HwConfigStruct *AlDma_Dev_LookupConfig(AL_U32 DevId);

AL_VOID AlDma_Dev_Reset(AL_VOID);

AL_S32 AlDma_Dev_Init(AL_DMA_DevStruct *Dev);

AL_S32 AlDma_Dev_DeInit(AL_DMA_DevStruct *Dev);

AL_S32 AlDma_Dev_Start(AL_DMA_ChStruct *Chan);

AL_S32 AlDma_Dev_Stop(AL_DMA_ChStruct *Chan);

AL_DMA_ChStruct *AlDma_Dev_RequestChannel(AL_DMA_DevStruct *Dev);

AL_S32 AlDma_Dev_ReleaseChannel(AL_DMA_ChStruct *Chan);

AL_S32 AlDma_Dev_Config(AL_DMA_ChStruct *Chan, AL_DMA_ChCfgStruct *Config);

AL_S32 AlDma_Dev_PrepDmaMemcpy(AL_DMA_ChStruct *Chan, AL_U32 Dst, AL_U32 Src, AL_U32 Len);

AL_S32 AlDma_Dev_PrepDmaCyclic(AL_DMA_ChStruct *Chan, AL_U32 MemAddr, AL_U32 Len, AL_U32 PeriodLen,
                               AL_DMA_TransEnum Direction);

AL_S32 AlDma_Dev_PrepDmaSingle(AL_DMA_ChStruct *Chan, AL_U32 MemAddr, AL_U32 Len, AL_DMA_TransEnum Direction);

AL_S32 AlDma_Dev_GetPosition(AL_DMA_ChStruct *Chan);

AL_S32 AlDma_Dev_RegisterEventCallBack(AL_DMA_ChStruct *Channel, AL_DMA_EventCallBack CallBack, AL_VOID *CallBackRef);

AL_S32 AlDma_Dev_UnRegisterEventCallBack(AL_DMA_ChStruct *Channel);

AL_U32 AlDma_Dev_IrqHandler(AL_VOID *Channel);

AL_U32 AlDma_Dev_GetRawIrqStatus(AL_DMA_DevStruct *Dev);

AL_S32 AlDma_Dev_ClearIrq(AL_DMA_DevStruct *Dev, AL_U32 Irq);

AL_S32 AlDma_Dev_SetMcBuf(AL_DMA_ChStruct *Chan, AL_VOID *Buf);

AL_VOID *AlDma_Dev_GetMcBuf(AL_DMA_ChStruct *Chan);

const AL_DMA_DescStruct *AlDma_Dev_GetDesc(AL_DMA_ChStruct *Chan);

AL_BOOL AlDma_Dev_FetchLock(AL_REG LockAddr);

AL_VOID AlDma_Dev_ReleaseLock(AL_REG LockAddr);

/** @} */

#endif

/** @} */

/** @} */

