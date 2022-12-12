#ifndef _ALSECURE_TEST_DATA_H_
#define _ALSECURE_TEST_DATA_H_

#include "al_types.h"

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
#define TBM_DATA_LENTH    (0x10000)   //64k
#define TBM_INPUT_DATA_ADDR (0x40000000)
#define TBM_INPUT_DATA_END  (TBM_INPUT_DATA_ADDR + TBM_DATA_LENTH)
#define TBM_OUTPUT_DATA_ADDR (0x40010000)
#define TBM_OUTPUT_DATA_END (TBM_OUTPUT_DATA_ADDR + TBM_DATA_LENTH)

#define IV_BUF_ADDR (0x40020000)
#define KEY_BUF_ADDR (0x40020100)
#define HASH_BUF_ADDR (0x40020200)

typedef struct {
    u8 EncryptMethod;
    u8 HashMode;
    u32 DmaSrcAddr;
    u32 DmaDstAddr;
    u32 HashOutAddr;
    u8 KeyMode;
    u8 DmaIncMode;
    u8 HashBlockMode;
    u8 EcbCbc;
    u8 EncDec;
}TestCfgBothInc_t;

extern const TestCfgBothInc_t TestCfgBothIncData[];
extern const TestCfgBothInc_t TestCfgBothIncBlock[];
extern uint32_t *InputDataBuff;
extern uint32_t *OutputDataBuff;
extern uint32_t *OutputDataBuffPcap;
extern uint32_t *IvDataBuff;
extern uint32_t *KeyDataBuff;
extern uint32_t *HashDataBuff;

#endif /*end of _ALSECURE_TEST_DATA_H_*/
