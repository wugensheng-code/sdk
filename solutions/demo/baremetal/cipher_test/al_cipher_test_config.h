/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    al_cipher_test_config.h
 * @author  Anlogic esw team
 * @version V0.0.1
 * @date    2023-05-09
 * @brief   cipher test config file
 */

#ifndef __AL_CIPHER_TEST_CONFIG_H_
#define __AL_CIPHER_TEST_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "al_cipher_hal.h"

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/
/* Data size in words */
#define DATA_BUFF_SIZE 64 /* 256 bytes */
#define IV_BUFF_SIZE 8    /* 256 bits */
#define KEY_BUFF_SIZE 8   /* 256 bits */
#define HASH_DATA_SIZE 8  /* 256 bits */

/* Input data size in bytes */
#define INPUT_DATA_SIZE_128B    (0x80)
#define INPUT_DATA_SIZE_256B    (0x100)
#define INPUT_DATA_SIZE_512B    (0x200)
#define INPUT_DATA_SIZE_1K      (0x400)
#define INPUT_DATA_SIZE_2K      (0x800)
#define INPUT_DATA_SIZE_4K      (0x1000)
#define INPUT_DATA_SIZE_8K      (0x2000)
#define INPUT_DATA_SIZE_16K     (0x4000)
#define INPUT_DATA_SIZE_24K     (0x6000)
#define INPUT_DATA_SIZE_32K     (0x8000)
#define INPUT_DATA_SIZE_40K     (0xA000)
#define INPUT_DATA_SIZE_48K     (0xC000)
#define INPUT_DATA_SIZE_56K     (0xE000)
#define INPUT_DATA_SIZE_64K     (0x10000)
#define INPUT_DATA_SIZE_128K    (0x20000)

/**
 * Csu Address Mapping
 */
#define MSG_RAM_M_LENGTH          (0xFFF)
#define MSG_RAM_M_START           (0x00060000)
#define MSG_RAM_M_END             (MSG_RAM_M_START + MSG_RAM_M_LENGTH)
#define MSG_RAM_M_START_MAPPING   (0xF8080000)
#define MSG_RAM_M_END_MAPPING     (MSG_RAM_M_START_MAPPING + MSG_RAM_M_LENGTH)
#define DMA_CTRL_M_LENGTH         (0xFFF)
#define DMA_CTRL_M_START          (0x00061000)
#define DMA_CTRL_M_END            (DMA_CTRL_M_START + DMA_CTRL_M_LENGTH)
#define DMA_CTRL_M_START_MAPPING  (0xF8081000)
#define DMA_CTRL_M_END_MAPPING    (DMA_CTRL_M_START_MAPPING + DMA_CTRL_M_LENGTH)
#define PCAP_M_LENGTH             (0xFFF)
#define PCAP_M_START              (0x00062000)
#define PCAP_M_END                (PCAP_M_START + PCAP_M_LENGTH)
#define PCAP_M_START_MAPPING      (0xF8082000)
#define PCAP_M_END_MAPPING        (PCAP_M_START_MAPPING + PCAP_M_LENGTH)
#define SOC_M_START               (0x100000)

//Test buff mapping
#define TBM_DATA_LENTH    (0x20000)   //128k
#define TBM_INPUT_DATA_ADDR (0x40000000)
#define TBM_INPUT_DATA_END  (TBM_INPUT_DATA_ADDR + TBM_DATA_LENTH)
#define TBM_OUTPUT_DATA_ADDR (TBM_INPUT_DATA_END)
#define TBM_OUTPUT_DATA_END (TBM_OUTPUT_DATA_ADDR + TBM_DATA_LENTH)

#define IV_BUF_ADDR (0x40040000)
#define KEY_BUF_ADDR (0x40040100)
#define HASH_BUF_ADDR (0x40040200)
// #define ECC_PPK_ADDR (0x40040300)
// #define ECC_PPK_HASH_ADDR (0x40040400)
// #define ECC_SPK_ADDR (0x40040500)
// #define ECC_SPK_SIG_ADDR (0x40040600)
// #define SM2_PPK_ADDR (0x40040700)
// #define SM2_PPK_HASH_ADDR (0x40040800)
// #define SM2_SPK_ADDR (0x40040900)
// #define SM2_SPK_SIG_ADDR (0x40040a00)
#define SIGN_ECC_SIGN_ADDR (0x40040300)
#define SIGN_SM2_SIGN_ADDR (0x40040400)
#define GENKEY_ECC_PRI_ADDR (0x40040500)
#define GENKEY_ECC_PUB_ADDR (0x40040600)
#define GENKEY_SM2_PRI_ADDR (0x40040700)
#define GENKEY_SM2_PUB_ADDR (0x40040800)
#define GETZ_ZA_ADDR (0x40040900)

#define SRC_DATA_ADDR (0x40050000)      //src data
#define ENC_AES_DATA_ADDR (0x40060000)  //enc aes data
#define ENC_SM4_DATA_ADDR (0x40070000)  //enc sm4 data
#define SRC_AES_HASH (0x40080000)      //src aes hash
#define SRC_SM3_HASH (0x40080100)      //src sm3 hash
#define ENC_AES_DATA_HASH (0x40080200)  //enc aes hash
#define ENC_SM4_DATA_HASH (0x40080300)  //enc sm4 hash
#define ENC_AES_CBC_DATA_ADDR (0x40090000)  //enc aes cbc data
#define ENC_SM4_CBC_DATA_ADDR (0x400a0000)  //enc sm4 cbc data

#define IMAGE_1K_ADDR       (ENC_SM4_CBC_DATA_ADDR + INPUT_DATA_SIZE_64K)
#define IMAGE_4K_ADDR       (IMAGE_1K_ADDR + INPUT_DATA_SIZE_1K)
#define IMAGE_8K_ADDR       (IMAGE_4K_ADDR + INPUT_DATA_SIZE_4K)
#define IMAGE_16K_ADDR      (IMAGE_8K_ADDR + INPUT_DATA_SIZE_8K)
#define IMAGE_32K_ADDR      (IMAGE_16K_ADDR + INPUT_DATA_SIZE_16K)
#define IMAGE_64K_ADDR      (IMAGE_32K_ADDR + INPUT_DATA_SIZE_32K)
#define IMAGE_128K_ADDR     (IMAGE_64K_ADDR + INPUT_DATA_SIZE_64K)

#define HASH_LENGTH (0x20)
#define PPK_SPK_SIGN_LENGTH (0x40)
#define IDA_LENGTH (0x20)


#define REG32(addr)                  (*(volatile uint32_t *)(uint32_t)(addr))

#define SYSCTRL_S_BASEADDR            0XF8806000UL
#define SYSCTRL_S_GLOBAL_SRSTN        ((SYSCTRL_S_BASEADDR) + 0x00000330U)
#define SYSCTRL_S_GLOBAL_SRSTN_MSK_PSONLY              (0x1 << 9)
#define SYSCTRL_S_GLOBAL_SRSTN_MSK_GLB_PL_SRST         (0x1 << 8)
#define SYSCTRL_S_GLOBAL_SRSTN_MSK_FCLK_DOMAIN_SRST    (0xf << 4)
#define SYSCTRL_S_GLOBAL_SRSTN_MSK_GLB_SRST            (0x1 << 0)

#define CSU_PCAP_BASEADDR           0xF8082000UL
#define CSU_PCAP_RESET              ((CSU_PCAP_BASEADDR) + 0x00000000U)
#define CSU_PCAP_ENABLE             ((CSU_PCAP_BASEADDR) + 0x00000004U)
#define CSU_PCAP_WR_STREAM          ((CSU_PCAP_BASEADDR) + 0x00000008U)
#define CSU_PCAP_RD_STREAM          ((CSU_PCAP_BASEADDR) + 0x0000000CU)

#define CRP_BASEADDR               0xF8801000UL
#define CRP_CFG_STATE              ((CRP_BASEADDR) + 0x00000454U)
#define CRP_CFG_STATE_MSK_PL2PS_INITN      (1 << 0)
#define CRP_CFG_STATE_MSK_PL2PS_CFG_DONE   (1 << 1)
#define CRP_CFG_STATE_MSK_PL2PS_CFG_WAKEUP (1 << 2)

/**************************** Type Definitions *******************************/
typedef struct {
    uint8_t CryptMethod;
    uint8_t HashMode;
    uint32_t DmaSrcAddr;
    uint32_t DmaDstAddr;
    uint32_t HashOutAddr;
    uint8_t KeyMode;
    uint8_t DmaIncMode;
    uint8_t HashBlockMode;
    uint8_t EcbCbc;
    uint8_t EncDec;
}TestCfgBothInc_t;

/************************** Variable Definitions *****************************/
extern const TestCfgBothInc_t TestCfgBothIncData[];
extern const TestCfgBothInc_t TestCfgSrcIncData[];
extern const TestCfgBothInc_t TestCfgDmaErrorBranch[];
extern const TestCfgBothInc_t TestPerformanceData[];
extern uint32_t *InputDataBuff;
extern uint32_t *OutputDataBuff;
extern uint32_t *OutputDataBuffPcap;
extern uint32_t *IvDataBuff;
extern uint32_t *KeyDataBuff;
extern uint32_t *HashDataBuff;
extern uint32_t *SrcDataBuff;
extern uint32_t *EncAesBuff;
extern uint32_t *EncSm4Buff;
extern uint32_t *EncAesCbcBuff;
extern uint32_t *EncSm4CbcBuff;
extern uint32_t *SignEccSign;
extern uint32_t *SignSm2Sign;
extern uint32_t *GenKeyEccPri;
extern uint32_t *GenKeyEccPub;
extern uint32_t *GenKeySm2Pri;
extern uint32_t *GenKeySm2Pub;
extern uint32_t *GetZaZa;
extern uint32_t *SrcAesHashBuff;
extern uint32_t *SrcSm3HashBuff;
extern uint32_t *EncAesDataBuff;
extern uint32_t *EncSm4DataBuff;
extern uint32_t HashDataSize;
extern uint32_t UserSm4Key[];
extern uint32_t UserSm4IV[];
extern uint32_t UserAesKey[];
extern uint32_t UserAesIV[];
extern uint32_t EccPpk[];
extern uint32_t EccSpk[];
extern uint32_t EccSpkHash[];
extern uint32_t EccSpkSig[];
extern uint32_t Sm2Ppk[];
extern uint32_t Sm2Spk[];
extern uint32_t Sm2SpkHash[];
extern uint32_t Sm2SpkSig[];
extern uint32_t EncCbcAesDataBuff[];
extern uint32_t EncCbcSm4DataBuff[];
extern uint32_t PerformaceImageAddr[];
extern uint32_t PerformaceImageSize[];
extern uint32_t Ida[];

/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif
