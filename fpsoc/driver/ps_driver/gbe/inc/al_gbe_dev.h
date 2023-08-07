#ifndef AL_GBE_DEV_H
#define AL_GBE_DEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "al_gbe_ll.h"

/* Default descriptor count, at least 4 */
#ifndef AL_GBE_TX_DESC_CNT
#define AL_GBE_TX_DESC_CNT    16
#endif

#ifndef AL_GBE_RX_DESC_CNT
#define AL_GBE_RX_DESC_CNT    16
#endif

#define AL_GBE_SEGMENT_DEFAULT_SIZE              1460

typedef enum
{
    AL_GBE_GMII_MII_MODE      = 0x00,
    AL_GBE_RGMII_MODE         = 0x01,
    AL_GBE_RMII_MODE          = 0x04,
} AL_GBE_MediaInterfaceModeEnum;

typedef struct
{
    /* MAC configuration */
    AL_GBE_PreambleLenEnum      PreambleLength;
    AL_GBE_FunctionEnum         CarrierSenseBeforeTransmit;
    AL_GBE_FunctionEnum         LoopbackMode;
    AL_GBE_DuplexModeEnum       DuplexMode;
    AL_GBE_SpeedEnum            Speed;
    AL_GBE_FunctionEnum         JumboPacket;
    AL_GBE_FunctionEnum         JabberDisable;
    AL_GBE_FunctionEnum         WatchdogDisable;
    AL_GBE_FunctionEnum         AutoPadCrcStrip;
    AL_GBE_FunctionEnum         CrcStripTypePacket;
    AL_GBE_FunctionEnum         Support2KPacket;
    AL_GBE_FunctionEnum         GiantPacketSizeLimitControl;
    AL_GBE_InterPacketGapEnum   InterPacketGapVal;
    AL_GBE_FunctionEnum         ChecksumOffload;

    /* MAC extended configuration */
    AL_U16                      GiantPacketSizeLimit;
    AL_GBE_FunctionEnum         CrcCheckingRxPackets;
    AL_GBE_FunctionEnum         SlowProtocolDetect;
    AL_GBE_FunctionEnum         UnicastSlowProtocolPacketDetect;
    AL_GBE_FunctionEnum         ExtendedInterPacketGap;
    AL_U8                       ExtendedInterPacketGapVal;

    /* MAC packet filter */
    AL_GBE_FunctionEnum         PromiscuousMode;
    AL_GBE_FunctionEnum         HashUnicast;
    AL_GBE_FunctionEnum         HashMulticast;
    AL_GBE_FunctionEnum         DesteAddrInverseFilter;
    AL_GBE_FunctionEnum         PassAllMulticastPacket;
    AL_GBE_FunctionEnum         DisableBroadcastPackets;
    AL_GBE_PassCtlPacketsModeEnum PassControlPackets;
    AL_GBE_FunctionEnum         SourceAddrInverseFilter;
    AL_GBE_FunctionEnum         SourceAddrFilter;
    AL_GBE_FunctionEnum         HashOrPerfectFilter;
    AL_GBE_FunctionEnum         ReceiveAll;

    /* MAC watchdog timeout */
    AL_GBE_FunctionEnum           ProgrammableWatchdog;
    AL_GBE_WatchdogTimeoutEnum    WatchdogTimeout;

    /* MAC TX and RX flow control */
    AL_GBE_FunctionEnum           TransmitFlowControl;
    AL_GBE_PauseLowThresholdEnum  PauseLowThreshold;
    AL_GBE_FunctionEnum           ZeroQuantaPause;
    AL_U16                        PauseTime;
    AL_GBE_FunctionEnum           ReceiveFlowControl;
    AL_GBE_FunctionEnum           UnicastPausePacketDetect;

    /* MTL Q0 configuration */
    AL_GBE_FunctionEnum           TxQueueStoreForward;
    AL_GBE_FunctionEnum           RxQueueStoreForward;
    AL_GBE_FunctionEnum           DisableDropTCPIPChecksumErrorPacket;
    AL_GBE_FunctionEnum           ForwardRxErrorPacket;
    AL_GBE_FunctionEnum           ForwardRxUndersizedGoodPacket;
} AL_GBE_MacConfigStruct;

/* DMA configuration */
typedef struct
{
    AL_GBE_DmaTxRxArbSchemeEnum         DMAArbitration;
    AL_GBE_FunctionEnum                 DMATxPriority;
    AL_GBE_DmaTxRxPriorityRatioEnum     PriorityRatio;
    AL_GBE_FunctionEnum                 AddressAlignedBeats;
    AL_GBE_FunctionEnum                 FixedBurstLen;
    AL_GBE_FunctionEnum                 RebuildINCRxBurst;
    AL_GBE_FunctionEnum                 PBLx8Mode;
    AL_GBE_DmaTxProgramBurstLengthEnum  TxDMABurstLength;
    AL_GBE_FunctionEnum                 SecondPacketOperate;
    AL_GBE_DmaTxProgramBurstLengthEnum  RxDMABurstLength;
    AL_GBE_FunctionEnum                 FlushRxPacket;
    AL_GBE_FunctionEnum                 TCPSegmentation;
    AL_U32                              MaximumSegmentSize;
} AL_GBE_DmaConfigStruct;

typedef struct
{
    __IO AL_U32 DESC0;
    __IO AL_U32 DESC1;
    __IO AL_U32 DESC2;
    __IO AL_U32 DESC3;

    /* Just for RX descriptor, to store RX buffer1 and buffer2 address */
    AL_U32 BackupAddr0;
    AL_U32 BackupAddr1;
} AL_GBE_DMADescStruct;

/*
  The linked list structure of the sent buffer,
  according to which descriptors are filled when sent frame.
*/
typedef struct AL_GBE_FrameBufferStruct
{
    AL_U8  *Buffer;
    AL_U32 Len;
    struct AL_GBE_FrameBufferStruct *next;
} AL_GBE_BufferStruct;

/* Record sending descriptors and buffer usage */
typedef struct
{
    AL_U32 TxDesc[AL_GBE_TX_DESC_CNT];
    AL_U32 CurTxDesc;
    AL_U32 *PacketAddress[AL_GBE_TX_DESC_CNT];
    AL_U32 *CurrentPacketAddress;
    AL_U32 BuffersInUse;
    AL_U32 ReleaseIndex;
} AL_GBE_TxDescListStruct;

/* Record receive descriptors and buffer usage */
typedef struct
{
    AL_U32 RxDesc[AL_GBE_RX_DESC_CNT];
    AL_U32 ItMode;
    AL_U32 CurRxDesc;
    AL_U32 FirstAppDesc;
    AL_U32 AppDescNbr;
    AL_U32 AppContextDesc;
} AL_GBE_RxDescListStruct;

/* Just for user, Some feature options that may be used */
typedef struct
{
    AL_GBE_FunctionEnum   GbePhyAutoNegotiation;
    AL_GBE_DuplexModeEnum DuplexMode;
    AL_GBE_SpeedEnum      Speed;
} AL_GBE_MacDmaConfigStruct;

typedef struct
{
    AL_U8                           MacAddr[6];
    AL_GBE_MediaInterfaceModeEnum   MediaInterface;
    AL_GBE_DMADescStruct            *TxDescList;
    AL_GBE_DMADescStruct            *RxDescList;
    AL_U32                          RxBuffLen;
} AL_GBE_InitStruct;

typedef enum
{
  AL_GBE_EVENT_FATAL_BUS_ERROR    = 0x0,

} AL_GBE_EventIdEnum;

typedef struct
{
    AL_GBE_EventIdEnum      Event;
    AL_U32                  EventData;
} AL_GBE_EventStruct;

typedef AL_VOID (*AL_GBE_EventCallBack)(AL_GBE_EventStruct *GbetEvent, AL_VOID *CallbackRef);

typedef enum
{
    AL_GBE_STATE_NOT_INIT     = (0x0),
    AL_GBE_STATE_READY        = (0x01 << 0),
    AL_GBE_STATE_TX_BUSY      = (0x01 << 1),
    AL_GBE_STATE_RX_BUSY      = (0x01 << 2)
} AL_GBE_StateEnum;

typedef struct
{
    AL_GBE_HwConfigStruct       HwConfig;
    AL_GBE_InitStruct           InitConfig;
    AL_GBE_MacDmaConfigStruct   MacDmaConfig;

    AL_GBE_TxDescListStruct     TxDescList;
    AL_GBE_RxDescListStruct     RxDescList;

    AL_GBE_EventCallBack        EventCallBack;
    AL_VOID                     *EventCallBackRef;
    AL_U32                      ErrorCode;
    AL_GBE_StateEnum            State;
} AL_GBE_DevStruct;

/* TX descriptor configuration options, Configure the TX descriptor according to these options */
typedef struct
{
    AL_U32 Attributes;
    AL_U32 Length;
    AL_GBE_BufferStruct *TxBuffer;
    AL_U32 SrcAddrCtrl;
    AL_U32 CRCPadCtrl;
    AL_U32 ChecksumCtrl;
    AL_U32 MaxSegmentSize;
    AL_U32 PayloadLen;
    AL_U32 TCPHeaderLen;
    AL_U32 VlanTag;
    AL_U32 VlanCtrl;
    AL_U32 InnerVlanTag;
    AL_U32 InnerVlanCtrl;
    void *pData;
} AL_GBE_TxDescConfigStruct;

/* GBE error code define */
typedef enum
{
    AL_GBE_INVALID_DEVICE_ID    = 0x100,
    AL_GBE_ERROR_CONFIG         = 0x101,
    AL_GBE_NOT_READY            = 0x102,
    AL_GBE_BUSY                 = 0x103,

    AL_GBE_PHY_BUSY             = 0x104,
    AL_GBE_ERR_PHY_RESET        = 0x105,
    AL_GBE_ERR_PHY_LINK         = 0x106,
    AL_GBE_FATLA_BUS_ERROR      = 0x107,
    AL_GBE_DESC_ERROR           = 0x108,

} AL_GBE_ErrorCodeEnum;

#define AL_GBE_ERR_ILLEGAL_PARAM            (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_GBE_ERR_INVALID_DEVICE_ID        (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_GBE_INVALID_DEVICE_ID))
#define AL_GBE_ERR_CONFIG_ERROR             (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_GBE_ERROR_CONFIG))
#define AL_GBE_ERR_NOT_READY                (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_GBE_NOT_READY))
#define AL_GBE_ERR_BUSY                     (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_GBE_BUSY))
#define AL_GBE_ERR_TIMEOUT                  (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT))

#define AL_GBE_ERR_PHY_BUSY                 (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_GBE_PHY_BUSY))
#define AL_GBE_ERR_PHY_RESET_FAILED         (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_GBE_ERR_PHY_RESET))
#define AL_GBE_ERR_PHY_LINK_DOWN            (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_GBE_ERR_PHY_LINK))
#define AL_GBE_ERR_FATLA_BUS_ERROR          (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_GBE_FATLA_BUS_ERROR))
#define AL_GBE_ERR_DESC_STATE               (AL_DEF_ERR(AL_GBE, AL_LOG_LEVEL_ERROR, AL_GBE_DESC_ERROR))

/* Helper macros for TX descriptor handling */
#define INCR_TX_DESC_INDEX(Index, Offset) do {\
                                             (Index) += (Offset);\
                                             if ((Index) >= (AL_U32)AL_GBE_TX_DESC_CNT){\
                                             (Index) = ((Index) - (AL_U32)AL_GBE_TX_DESC_CNT);}\
                                           } while (0)

/* Helper macros for RX descriptor handling */
#define INCR_RX_DESC_INDEX(Index, Offset) do {\
                                             (Index) += (Offset);\
                                             if ((Index) >= (AL_U32)AL_GBE_RX_DESC_CNT){\
                                             (Index) = ((Index) - (AL_U32)AL_GBE_RX_DESC_CNT);}\
                                           } while (0)

/* Define phy register */

/* Basic mode control register */
#define PHY_BCR_ADDR                        0x0
/* Basic Mode Status Register */
#define PHY_BSR_ADDR                        0x1
/* Specific Mode Status Register */
#define PHY_SSR_ADDR                        0x1A

/* PHY_BCR register descriptions define */
#define PHY_BCR_SPEED1_SHIFT                6
#define PHY_BCR_DUPLEX_SHIFT                8
#define PHY_BCR_AUTO_NEGO_SHIFT             12
#define PHY_BCR_SPEED0_SHIFT                13

#define PHY_BCR_RESET                       ((AL_U16)0x8000)
#define PHY_BCR_LOOPBACK                    ((AL_U16)0x4000)
#define PHY_BCR_SPEED_1G                    ((AL_U16)0x0040)
#define PHY_BCR_SPEED_100M                  ((AL_U16)0x2000)
#define PHY_BCR_SPEED_10M                   ((AL_U16)0x0000)
#define PHY_BCR_AUTONEGOTIATION             ((AL_U16)0x1000)
#define PHY_BCR_PPWER_DOWN                  ((AL_U16)0x0800)
#define PHY_BCR_ISOLATE                     ((AL_U16)0x0400)
#define PHY_BCR_RESTART_AUTONEGOTIATION     ((AL_U16)0x0200)
#define PHY_BCR_FULL_DUPLEX_MODE            ((AL_U16)0x0100)

/* PHY_BSR register descriptions define */
#define PHY_BSR_AUTO_NEGOTIATION_COMPLETE   ((AL_U16)0x0020)
#define PHY_BSR_LINK_STATUS                 ((AL_U16)0x0004)
#define PHY_BSR_JABBER_DETECT               ((AL_U16)0x0002)

/* PHY_SSR register descriptions define */
#define PHY_SSR_LINK_STATUS                 ((AL_U16)0x0004)
#define PHY_SSR_FULL_DUPLEX                 ((AL_U16)0x0008)
#define PHY_SSR_SPEED_1G                    ((AL_U16)0x0020)
#define PHY_SSR_SPEED_100M                  ((AL_U16)0x0010)
#define PHY_SSR_SPEED_10M                   ((AL_U16)0x0000)


AL_GBE_HwConfigStruct *AlGbe_Dev_LookupConfig(AL_U32 DevId);

AL_S32 AlGbe_Dev_Init(AL_GBE_DevStruct *Gbe, AL_GBE_HwConfigStruct *HwConfig,
                      AL_GBE_InitStruct *InitConfig, AL_GBE_MacDmaConfigStruct *MacDmaConfig);

AL_S32 AlGbe_Dev_PhyInit(AL_GBE_DevStruct *Gbe, AL_U32 PHYAddress);

AL_S32 AlGbe_Dev_ConfigDuplexAndSpeed(AL_GBE_DevStruct *Gbe);

AL_U32 AlGbe_Dev_ConfigRxDescBuffer(AL_GBE_DevStruct *Gbe, AL_U32 Index, AL_U8 *pBuffer1, AL_U8 *pBuffer2);

AL_S32 AlGbe_Dev_Transmit(AL_GBE_DevStruct *Gbe, AL_GBE_TxDescConfigStruct *TxConfig);

AL_S32 AlGbe_Dev_TransmitPolling(AL_GBE_DevStruct *Gbe, AL_GBE_TxDescConfigStruct *TxConfig);

AL_S32 AlGbe_Dev_RegisterEventCallBack(AL_GBE_DevStruct *Gbe, AL_GBE_EventCallBack Callback, void *CallbackRef);

AL_VOID AlGbe_Dev_IntrHandler(AL_VOID *Instance);

AL_S32 AlGbe_Dev_StartMacDmaIntr(AL_GBE_DevStruct *Gbe);

AL_S32 AlGbe_Dev_StartMacDma(AL_GBE_DevStruct *Gbe);

AL_S32 AlGbe_Dev_GetRxDataBuffer(AL_GBE_DevStruct *Gbe, AL_GBE_BufferStruct *RxBuffer);

AL_S32 AlGbe_Dev_GetRxDataLength(AL_GBE_DevStruct *Gbe, AL_U32 *Length);

AL_S32 AlGbe_Dev_BuildRxDescriptors(AL_GBE_DevStruct *Gbe);

AL_S32 AlGbe_Dev_ReleaseTxPacket(AL_GBE_DevStruct *Gbe);

#ifdef __cplusplus
}
#endif

#endif /* AL_GBE_DEV_H */