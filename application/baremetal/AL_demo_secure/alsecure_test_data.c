#include "alsecure_msg.h"
#include "alsecure_test_data.h"


//input data : ddr
uint32_t *InputDataBuff = (uint32_t *)(TBM_INPUT_DATA_ADDR);
//output data : pcap/ddr
uint32_t *OutputDataBuff = (uint32_t *)(TBM_OUTPUT_DATA_ADDR);
uint32_t *OutputDataBuffPcap = (uint32_t *)(PCAP_M_START_MAPPING);
//
uint32_t *IvDataBuff = (uint32_t *)(IV_BUF_ADDR);
uint32_t *KeyDataBuff = (uint32_t *)(KEY_BUF_ADDR);
uint32_t *HashDataBuff = (uint32_t *)(HASH_BUF_ADDR);

//CFG_MODE: ENCRYPT     HASH        DMA_SRC     DMA_DST     HASH_DST    KEY         DMA_MODE    HASH_BLOCK      ECBCBC      ENCDEC
//-----------------------------------------------------------------------------------------------------------------------------------
//case 0:   AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case 1:   AES256      SHA256      DDR         PCAP        DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case 2:   AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case 3:   AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case 4:   AES256      NONE        DDR         PCAP        NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         ENC
//case 5:   AES256      NONE        DDR         PCAP        NONE        USER_KEY    BOTH_INCR   NONE            ECB         ENC
//case 6:   AES256      NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         ENC
//case 7:   AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         ENC
//case 8:   SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case 9:   SM4         SM3         DDR         PCAP        DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case 10:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case 11:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case 12:  SM4         NONE        DDR         PCAP        NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         ENC
//case 13:  SM4         NONE        DDR         PCAP        NONE        USER_KEY    BOTH_INCR   NONE            ECB         ENC
//case 14:  SM4         NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         ENC
//case 15:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         ENC
//case 16:  AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 17:  AES256      SHA256      DDR         PCAP        DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 18:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 19:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 20:  AES256      NONE        DDR         PCAP        NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         DEC
//case 21:  AES256      NONE        DDR         PCAP        NONE        USER_KEY    BOTH_INCR   NONE            ECB         DEC
//case 22:  AES256      NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         DEC
//case 23:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         DEC
//case 24:  SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 25:  SM4         SM3         DDR         PCAP        DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 26:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 27:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 28:  SM4         NONE        DDR         PCAP        NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         DEC
//case 29:  SM4         NONE        DDR         PCAP        NONE        USER_KEY    BOTH_INCR   NONE            ECB         DEC
//case 30:  SM4         NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         DEC
//case 31:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         DEC
//case 32:  AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 33:  AES256      SHA256      DDR         PCAP        DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 34:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 35:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 36:  AES256      NONE        DDR         PCAP        NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         ENC
//case 37:  AES256      NONE        DDR         PCAP        NONE        USER_KEY    BOTH_INCR   NONE            CBC         ENC
//case 38:  AES256      NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         ENC
//case 39:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         ENC
//case 40:  SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 41:  SM4         SM3         DDR         PCAP        DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 42:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 43:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 44:  SM4         NONE        DDR         PCAP        NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         ENC
//case 45:  SM4         NONE        DDR         PCAP        NONE        USER_KEY    BOTH_INCR   NONE            CBC         ENC
//case 46:  SM4         NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         ENC
//case 47:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         ENC
//case 48:  AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 49:  AES256      SHA256      DDR         PCAP        DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 50:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 51:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 52:  AES256      NONE        DDR         PCAP        NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         DEC
//case 53:  AES256      NONE        DDR         PCAP        NONE        USER_KEY    BOTH_INCR   NONE            CBC         DEC
//case 54:  AES256      NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         DEC
//case 55:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         DEC
//case 56:  SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 57:  SM4         SM3         DDR         PCAP        DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 58:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 59:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 60:  SM4         NONE        DDR         PCAP        NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         DEC
//case 61:  SM4         NONE        DDR         PCAP        NONE        USER_KEY    BOTH_INCR   NONE            CBC         DEC
//case 62:  SM4         NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         DEC
//case 63:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         DEC
//case 64:  NONE        NONE        DDR         PCAP        NONE        NONE        BOTH_INCR   NONE            NONE        NONE
//case 65:  NONE        NONE        DDR         DDR         NONE        NONE        BOTH_INCR   NONE            NONE        NONE

//WHOLE BLOCK and NONE HASH
const TestCfgBothInc_t TestCfgBothIncData[] = \
            {{.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_WHOLE_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_NONE, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=NULL, \
            .KeyMode=NULL, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL},\
            {.EncryptMethod=OP_ENCRYPT_NONE, .HashMode=OP_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=NULL, \
            .KeyMode=NULL, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=NULL},\
        };

//CFG_MODE: ENCRYPT     HASH        DMA_SRC     DMA_DST     HASH_DST    KEY         DMA_MODE    HASH_BLOCK              ECBCBC      ENCDEC
//------------------------------------------------------------------------------------------------------------------------------------------
//case 0:   AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         ENC
//case 1:   AES256      SHA256      DDR         PCAP        DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         ENC
//case 2:   AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         ENC
//case 3:   AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         ENC
//case 4:   SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         ENC
//case 5:   SM4         SM3         DDR         PCAP        DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         ENC
//case 6:   SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         ENC
//case 7:   SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         ENC
//case 8:   AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         DEC
//case 9:   AES256      SHA256      DDR         PCAP        DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         DEC
//case 10:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         DEC
//case 11:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         DEC
//case 12:  SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         DEC
//case 13:  SM4         SM3         DDR         PCAP        DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         DEC
//case 14:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         DEC
//case 15:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    ECB         DEC
//case 16:  AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         ENC
//case 17:  AES256      SHA256      DDR         PCAP        DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         ENC
//case 18:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         ENC
//case 19:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         ENC
//case 20:  SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         ENC
//case 21:  SM4         SM3         DDR         PCAP        DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         ENC
//case 22:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         ENC
//case 23:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         ENC
//case 24:  AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         DEC
//case 25:  AES256      SHA256      DDR         PCAP        DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         DEC
//case 26:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         DEC
//case 27:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         DEC
//case 28:  SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         DEC
//case 29:  SM4         SM3         DDR         PCAP        DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         DEC
//case 30:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         DEC
//case 31:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   FIRST/MID/LAST_BLOCK    CBC         DEC

//FIRST/MID/LAST_BLOCK
const TestCfgBothInc_t TestCfgBothIncBlock[] = \
            {{.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_ECB, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_ENCRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_AES256, .HashMode=OP_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=PCAP_M_START_MAPPING, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_BHDR_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
            {.EncryptMethod=OP_ENCRYPT_SM4, .HashMode=OP_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=OP_USER_KEY, .DmaIncMode=OP_BOTH_INCR, .HashBlockMode=OP_FIRST_BLOCK, .EcbCbc=SYM_CBC, .EncDec=SYM_DECRYPT},\
        };