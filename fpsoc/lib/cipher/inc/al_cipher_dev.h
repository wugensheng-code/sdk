/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_cipher_dev.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-07-10
 * @brief   cipher device driver
 */

#ifndef __AL_CIPHER_DEV_H_
#define __AL_CIPHER_DEV_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_cipher_ll.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

#define CIPHER_EVENT_START_BIT     6

/**
 * @brief  Error code enum
 */
typedef enum
{
    CIPHER_ERR_FAIL             = AL_ERR_MAX,
    CIPHER_ERR_CRYPT_ALGM,
    CIPHER_ERR_CRYPT_MODE,
    CIPHER_ERR_HASH_MODE,
    CIPHER_ERR_KEY_MODE,
    CIPHER_ERR_IV_VALUE,
    CIPHER_ERR_KEY_VALUE,
    CIPHER_ERR_ENCDEC_VALUE,
    CIPHER_ERR_DMA_INC_MODE,
    CIPHER_ERR_DATA_LENGTH_ALIGN,
    CIPHER_ERR_DATA_START_ALIGN,
    CIPHER_ERR_BLOCK_MODE,
    CIPHER_ERR_AUTH_MODE,
    CIPHER_ERR_SIGN_MODE,
    CIPHER_ERR_UNSUPPORT_CMD,
    CIPHER_ERR_DATA_OVERFLOW,
    CIPHER_ERR_STATE_NOT_READY,
    CIPHER_ERR_STATE_BUSY,
    CIPHER_ERR_AUTH_FAIL = 0x16A,
    CIPHER_ERR_CMD = 0x1E1,
    CIPHER_ERR_CSU = 0x1E2,
    CIPHER_ERR_CSU_TIMEOUT = 0x1E3,
    CIPHER_ERR_CSU_SEL_KEY_ERR = 0x1E4,
} AL_CIPHER_ErrCodeEnum;

#define AL_CIPHER_ERR_NULL_PTR              AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, AL_ERR_NULL_PTR)
#define AL_CIPHER_ERR_ILLEGAL_PARAM         AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM)
#define AL_CIPHER_ERR_NOT_SUPPORT           AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, AL_ERR_NOT_SUPPORT)
#define AL_CIPHER_ERR_TIMEOUT               AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, AL_ERR_TIMEOUT)
#define AL_CIPHER_ERR_BUSY                  AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, AL_ERR_BUSY)
#define AL_CIPHER_ERR_FAIL                  AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_FAIL)
#define AL_CIPHER_ERR_CRYPT_ALGM            AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_CRYPT_ALGM)
#define AL_CIPHER_ERR_CRYPT_MODE            AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_CRYPT_MODE)
#define AL_CIPHER_ERR_HASH_MODE             AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_HASH_MODE)
#define AL_CIPHER_ERR_KEY_MODE              AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_KEY_MODE)
#define AL_CIPHER_ERR_IV_VALUE              AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_IV_VALUE)
#define AL_CIPHER_ERR_KEY_VALUE             AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_KEY_VALUE)
#define AL_CIPHER_ERR_ENCDEC_VALUE          AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_ENCDEC_VALUE)
#define AL_CIPHER_ERR_DMA_INC_MODE          AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_DMA_INC_MODE)
#define AL_CIPHER_ERR_DATA_LENGTH_ALIGN     AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_DATA_LENGTH_ALIGN)
#define AL_CIPHER_ERR_DATA_START_ALIGN      AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_DATA_START_ALIGN)
#define AL_CIPHER_ERR_BLOCK_MODE            AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_BLOCK_MODE)
#define AL_CIPHER_ERR_AUTH_MODE             AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_INFO, CIPHER_ERR_AUTH_MODE)
#define AL_CIPHER_ERR_SIGN_MODE             AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_SIGN_MODE)
#define AL_CIPHER_ERR_UNSUPPORT_CMD         AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_UNSUPPORT_CMD)
#define AL_CIPHER_ERR_DATA_OVERFLOW         AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_DATA_OVERFLOW)
#define AL_CIPHER_ERR_STATE_NOT_READY       AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_STATE_NOT_READY)
#define AL_CIPHER_ERR_STATE_BUSY            AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_STATE_BUSY)
#define AL_CIPHER_ERR_AUTH_FAIL             AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_AUTH_FAIL)
#define AL_CIPHER_ERR_CMD                   AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_CMD)
#define AL_CIPHER_ERR_CSU                   AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_CSU)
#define AL_CIPHER_ERR_CSU_TIMEOUT           AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_CSU_TIMEOUT)
#define AL_CIPHER_ERR_CSU_SEL_KEY_ERR       AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, CIPHER_ERR_CSU_SEL_KEY_ERR)
#define AL_CIPHER_EVENTS_TO_ERRS(Events)    (AL_DEF_ERR(AL_CIPHER, AL_LOG_LEVEL_ERROR, \
                                                        (Events << CIPHER_EVENT_START_BIT)))

#define AL_CIPHER_DATA_ALIGN_SIZE       64

#define AL_CIPHER_HASH_SIZE             32
#define AL_CIPHER_AUTH_PUBKEY_SIZE      64
#define AL_CIPHER_AUTH_PRIKEY_SIZE      64
#define AL_CIPHER_AUTH_DIGEST_SIZE      32
#define AL_CIPHER_AUTH_SIGN_SIZE        64

#define AL_CIPHER_SYSCTRL_S_BASEADDR                            0xF8806000UL
#define AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN                        ((AL_CIPHER_SYSCTRL_S_BASEADDR) + 0x00000330U)
#define AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN_MSK_PSONLY             (0x1 << 9)
#define AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN_MSK_GLB_PL_SRST        (0x1 << 8)
#define AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN_MSK_FCLK_DOMAIN_SRST   (0xf << 4)
#define AL_CIPHER_SYSCTRL_S_GLOBAL_SRSTN_MSK_GLB_SRST           (0x1 << 0)

#define AL_CIPHER_CSU_PCAP_BASEADDR           0xF8082000UL
#define AL_CIPHER_CSU_PCAP_RESET              ((AL_CIPHER_CSU_PCAP_BASEADDR) + 0x00000000U)
#define AL_CIPHER_CSU_PCAP_ENABLE             ((AL_CIPHER_CSU_PCAP_BASEADDR) + 0x00000004U)
#define AL_CIPHER_CSU_PCAP_WR_STREAM          ((AL_CIPHER_CSU_PCAP_BASEADDR) + 0x00000008U)
#define AL_CIPHER_CSU_PCAP_RD_STREAM          ((AL_CIPHER_CSU_PCAP_BASEADDR) + 0x0000000CU)

#define AL_CIPHER_CRP_BASEADDR                          0xF8801000UL
#define AL_CIPHER_CRP_CFG_STATE                         ((CRP_BASEADDR) + 0x00000454U)
#define AL_CIPHER_CRP_CFG_STATE_MSK_PL2PS_INITN         (1 << 0)
#define AL_CIPHER_CRP_CFG_STATE_MSK_PL2PS_CFG_DONE      (1 << 1)
#define AL_CIPHER_CRP_CFG_STATE_MSK_PL2PS_CFG_WAKEUP    (1 << 2)

/**************************** Type Definitions *******************************/
/**
 * @brief  Encrypt Method enum
 */
typedef enum
{
    AL_CIPHER_CRYPT_MODE_ECB,
    AL_CIPHER_CRYPT_MODE_CBC
} AL_CIPHER_CryptModeEnum;

/**
 * @brief  Encrypt mode enum
 */
typedef enum
{
    AL_CIPHER_CRYPT_ENC  = 0x0,
    AL_CIPHER_CRYPT_DEC  = 0x8
} AL_CIPHER_CryptEnum;

/**
 * @brief  Frame id enum
 */
typedef enum
{
    AL_CIPHER_AES_128   = 0x10,
    AL_CIPHER_AES_192   = 0x20,
    AL_CIPHER_AES_256   = 0x40
} AL_CIPHER_AesEnum;

/**
 * @brief  Msg cmd enum
 */
typedef enum
{
    AL_CIPHER_CMD_AUTH      = 0xCD01,
    AL_CIPHER_CMD_DMA       = 0xCD02,
    AL_CIPHER_CMD_ENCRYPT   = 0xCD03,
    AL_CIPHER_CMD_HASH      = 0xCD04,
    AL_CIPHER_CMD_SIGN      = 0xCD05,
    AL_CIPHER_CMD_GENKEY    = 0xCD06,
    AL_CIPHER_CMD_ACK       = 0xAC01
} AL_CIPHER_CmdEnum;

/**
 * @brief  Msg options auth enum
 */
typedef enum
{
    AL_CIPHER_OPS_AUTH_ECC256   = 0x61,
    AL_CIPHER_OPS_AUTH_SM2      = 0x62
} AL_CIPHER_MsgOpsAuthEnum;

/**
 * @brief  Msg options crypt enum
 */
typedef enum
{
    AL_CIPHER_OPS_CRYPT_AES256    = 0x63,
    AL_CIPHER_OPS_CRYPT_SM4       = 0x64,
    AL_CIPHER_OPS_CRYPT_NONE      = 0x65
} AL_CIPHER_MsgOpsCryptEnum;
/**
 * @brief  Msg options hash enum
 */
typedef enum
{
    AL_CIPHER_OPS_HASH_NONE     = 0x0,
    AL_CIPHER_OPS_HASH_SHA256   = 0x2,
    AL_CIPHER_OPS_HASH_SM3      = 0x3,
} AL_CIPHER_MsgOpsHashEnum;

/**
 * @brief  Msg ack enum
 */
typedef enum
{
    AL_CIPHER_ACK_AUTH_PASS         = 0x69,
    AL_CIPHER_ACK_AUTH_FAIL         = 0x6A,
    AL_CIPHER_ACK_DMA_DONE          = 0x6B,
    AL_CIPHER_ACK_ENC_DONE          = 0x6C,
    AL_CIPHER_ACK_HASH_DONE         = 0x6D,
    AL_CIPHER_ACK_SIGN_DONE         = 0x6E,
    AL_CIPHER_ACK_GENKEY_DONE       = 0x6F,
    AL_CIPHER_ACK_CMD_ERR           = 0xE1,
    AL_CIPHER_ACK_CSU_ERR           = 0xE2,
    AL_CIPHER_ACK_CSU_TIMEOUT       = 0xE3,
    AL_CIPHER_ACK_CSU_SEL_KEY_ERR   = 0xE4
} AL_CIPHER_MsgAckEnum;

#define AL_CIPHER_IS_ACK_STATE(Ack)     ((Ack >= AL_CIPHER_ACK_AUTH_PASS) && (Ack <= AL_CIPHER_ACK_CSU_SEL_KEY_ERR))

/**
 * @brief  Key mode enum
 */
typedef enum
{
    AL_CIPHER_KEY_BHDR  = 0x6E,
    AL_CIPHER_KEY_USER  = 0x6F
} AL_CIPHER_KeyModeEnum;

/**
 * @brief  Frame trans mode enum
 */
typedef enum
{
    AL_CIPHER_DMA_INCR_BOTH,
    AL_CIPHER_DMA_INCR_DST,
    AL_CIPHER_DMA_INCR_SRC,
    AL_CIPHER_DMA_INCR_NONE,
} AL_CIPHER_DmaIncModeEnum;

/**
 * @brief  ACF mask mode enum
 */
typedef enum
{
    AL_CIPHER_BLK_WHOLE,
    AL_CIPHER_BLK_FIRST,
    AL_CIPHER_BLK_LAST,
    AL_CIPHER_BLK_MID,
} AL_CIPHER_HashBlkEnum;

/**
 * @brief  Event id enum
 */
typedef enum
{
    AL_CIPHER_EVENT_READY   = BIT(0),
    AL_CIPHER_EVENT_DONE    = BIT(1)
} AL_CIPHER_EventIdEnum;

/**
 * @brief  Module running state enum
 */
typedef enum {
    AL_CIPHER_STATE_NOT_INIT    = (0x0),
    AL_CIPHER_STATE_READY       = BIT(1),
    AL_CIPHER_STATE_BUSY        = BIT(2)
} AL_CIPHER_StateEnum;

/**
 * @brief  Io ctl cmd enum
 */
typedef enum
{
    AL_CIPHER_IOCTL_SET_BIT_RATE,
} AL_CIPHER_IoCtlCmdEnum;

/**
 * @brief  Event struct
 */
typedef struct
{
    AL_CIPHER_EventIdEnum   EventId;
    AL_U32                  EventData;
} AL_CIPHER_EventStruct;

typedef AL_S32 (*AL_CIPHER_EventCallBack)(AL_CIPHER_EventStruct *Event, AL_VOID *CallBackRef);

typedef struct
{
    AL_U32 A[8];
    AL_U32 B[8];
    AL_U32 X[8];
    AL_U32 Y[8];
}AL_CIPHER_EcCurveStruct;

typedef union
{
    AL_U32 Reg;
    struct {
        AL_U32 EcbCbc:1;
        AL_U32 Rsvd0:2;
        AL_U32 EncDec:1;
        AL_U32 AesLen:3;
        AL_U32 Rsvd1:25;
    } Bit;
} AL_CIPHER_OpModeUnion;

typedef struct
{
    AL_CIPHER_OpModeUnion   OpMode;
    AL_U32                  PbKey;
    AL_U32                  PbIv;
    AL_U32                  PbInput;
    AL_U32                  TotalLength;
    AL_U32                  PbOutput;
} AL_CIPHER_CipherStruct;

typedef struct
{
    AL_U32 Pubkey;
    AL_U32 Digest;
    AL_U32 Signature;
} AL_CIPHER_AuthStruct;

typedef struct
{
    AL_U32 PriKey;
    AL_U32 Digest;
    AL_U32 Signature;
} AL_CIPHER_SignStruct;

typedef struct
{
    AL_U32 PubKey;
    AL_U32 PriKey;
} AL_CIPHER_KeyStruct;

typedef struct {
    AL_CIPHER_CipherStruct  CipherParam;
    AL_U32                  HashOut;
} AL_CIPHER_DmaStruct;

typedef struct
{
    AL_U32 DataIn;
    AL_U32 DataLength;
    AL_U32 HashOut;
} AL_CIPHER_HashStruct;

typedef struct
{
    AL_U8 Low;
    AL_U8 High:2;
    AL_U8 Extend:2;
    AL_U8 Resved:4;
} AL_CIPHER_MsgOptStruct;

typedef struct
{
    AL_U32                  Cmd;
    AL_CIPHER_MsgOptStruct  Option0;
    AL_CIPHER_MsgOptStruct  Option1;
    union {
        AL_CIPHER_AuthStruct    AuthParam;
        AL_CIPHER_DmaStruct     DmaParam;
        AL_CIPHER_HashStruct    HashParam;
        AL_CIPHER_SignStruct    SignParam;
        AL_CIPHER_KeyStruct     GenKeyParam;
        AL_U32                  Data[7];
    } MsgData;
} AL_CIPHER_MsgStruct;

typedef struct
{
    AL_U32                  Cmd;
    AL_CIPHER_MsgOptStruct  Option0;
} AL_CIPHER_AckStruct;

typedef union
{
    struct {
        AL_CIPHER_MsgOpsCryptEnum   CryptAlgm;
        AL_CIPHER_CryptModeEnum     EcbCbc;
        AL_CIPHER_CryptEnum         EncDec;
        AL_CIPHER_MsgOpsHashEnum    HashMode;
        AL_CIPHER_KeyModeEnum       KeySel;
        AL_U32                      *Iv;
        AL_U32                      *Key;
        AL_CIPHER_HashBlkEnum       HashBlkMode;
        AL_CIPHER_DmaIncModeEnum    DmaIncMode;
        AL_U32                      *InputData;
        AL_U32                      *OutputData;
        AL_U32                      *HashOut;
        AL_U32                      DataLength;
    } Crypt;
    struct {
        AL_CIPHER_MsgOpsHashEnum    HashMode;
        AL_CIPHER_HashBlkEnum       HashBlkMode;
        AL_U32                      *InputData;
        AL_U32                      *HashOut;
        AL_U32                      DataLength;
    } Hash;
    struct {
        AL_CIPHER_MsgOpsAuthEnum    AuthMode;
        AL_U32                      *PubKey;
        AL_U32                      *Digest;
        AL_U32                      *Signature;
    } Auth;
    struct {
        AL_CIPHER_MsgOpsAuthEnum    KeyMode;
        AL_U32                      *PubKey;
        AL_U32                      *PriKey;
    } GenKey;
    struct {
        AL_CIPHER_MsgOpsAuthEnum    SignMode;
        AL_U32                      *PriKey;
        AL_U32                      *Digest;
        AL_U32                      *Signature;
    } Sign;
} AL_CIPHER_ConfigUnion;

/**
 * @brief  Module device struct
 */
typedef struct
{
    AL_CIPHER_HwConfigStruct    HwConfig;
    AL_CIPHER_MsgStruct         Msg;
    AL_CIPHER_AckStruct         Ack;
    AL_CIPHER_EventCallBack     EventCallBack;
    AL_VOID                     *EventCallBackRef;
    AL_CIPHER_StateEnum         State;
} AL_CIPHER_DevStruct;

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/
AL_CIPHER_HwConfigStruct *AlCipher_Dev_LookupConfig(AL_U32 DeviceId);

AL_S32 AlCipher_Dev_Init(AL_CIPHER_DevStruct *Dev, AL_CIPHER_HwConfigStruct *HwConfig);

AL_S32 AlCipher_Dev_RegisterEventCallBack(AL_CIPHER_DevStruct *Dev, AL_CIPHER_EventCallBack CallBack,
                                          AL_VOID *CallBackRef);

AL_S32 AlCipher_Dev_UnRegisterEventCallBack(AL_CIPHER_DevStruct *Dev);

AL_VOID AlCipher_Dev_IntrHandler(AL_VOID *Instance);

AL_S32 AlCipher_Dev_Start(AL_CIPHER_DevStruct *Dev, AL_CIPHER_CmdEnum Cmd, AL_CIPHER_ConfigUnion *Config);

AL_S32 AlCipher_Dev_CheckAck(AL_CIPHER_DevStruct *Dev, AL_CIPHER_MsgAckEnum *AckVal);

#ifdef __cplusplus
}
#endif

#endif
