/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_cipher_test_config.h"


//input data : ddr
uint32_t *InputDataBuff = (uint32_t *)(TBM_INPUT_DATA_ADDR);
//output data : pcap/ddr
uint32_t *OutputDataBuff = (uint32_t *)(TBM_OUTPUT_DATA_ADDR);
uint32_t *OutputDataBuffPcap = (uint32_t *)(AL_CIPHER_CSU_PCAP_WR_STREAM);
//
uint32_t *IvDataBuff = (uint32_t *)(IV_BUF_ADDR);
uint32_t *KeyDataBuff = (uint32_t *)(KEY_BUF_ADDR);
uint32_t *HashDataBuff = (uint32_t *)(HASH_BUF_ADDR);

uint32_t *SrcDataBuff = (uint32_t *)(SRC_DATA_ADDR);
uint32_t *EncAesBuff = (uint32_t *)(ENC_AES_DATA_ADDR);
uint32_t *EncSm4Buff = (uint32_t *)(ENC_SM4_DATA_ADDR);
uint32_t *SrcAesHashBuff = (uint32_t *)(SRC_AES_HASH);
uint32_t *SrcSm3HashBuff = (uint32_t *)(SRC_SM3_HASH);
uint32_t *EncAesDataBuff = (uint32_t *)(ENC_AES_DATA_HASH);
uint32_t *EncSm4DataBuff = (uint32_t *)(ENC_SM4_DATA_HASH);
uint32_t *EncAesCbcBuff = (uint32_t *)(ENC_AES_CBC_DATA_ADDR);
uint32_t *EncSm4CbcBuff = (uint32_t *)(ENC_SM4_CBC_DATA_ADDR);
//Sign data addr
uint32_t *SignEccSign = (uint32_t *)(SIGN_ECC_SIGN_ADDR);
uint32_t *SignSm2Sign = (uint32_t *)(SIGN_SM2_SIGN_ADDR);
//Gen key addr
uint32_t *GenKeyEccPri = (uint32_t *)(GENKEY_ECC_PRI_ADDR);
uint32_t *GenKeyEccPub = (uint32_t *)(GENKEY_ECC_PUB_ADDR);
uint32_t *GenKeySm2Pri = (uint32_t *)(GENKEY_SM2_PRI_ADDR);
uint32_t *GenKeySm2Pub = (uint32_t *)(GENKEY_SM2_PUB_ADDR);
//Get Za addr
uint32_t *GetZaZa = (uint32_t *)(GETZ_ZA_ADDR);

uint32_t CACHE_LINE_ALIGN UserSm4Key[] = {0xB82BB78D, 0x06928435, 0x6FFC32A6, 0x054A4C73};
uint32_t CACHE_LINE_ALIGN UserSm4IV[] = {0xC624E1DB, 0x141E678E, 0x91346E89, 0x1378FD4E};

uint32_t CACHE_LINE_ALIGN UserAesKey[] = {0xB82BB78D, 0x06928435, 0x6FFC32A6, 0x054A4C73, \
                                          0xBAF6AF50, 0xE5045340, 0x66A47271, 0xE72FF6A6};
uint32_t CACHE_LINE_ALIGN UserAesIV[] = {0xC624E1DB, 0x141E678E, 0x91346E89, 0x1378FD4E};

uint32_t CACHE_LINE_ALIGN EccPpk[] = {0x246394d6, 0x965bd1d9, 0x6cc6637a, 0x2abe94b1, \
                                      0x7202502d, 0x6320e934, 0x36dac62c, 0x26b601ec, \
                                      0xc076490f, 0x87485221, 0x370e6776, 0x5a0ec10a, \
                                      0x22ce1a7f, 0x216e1dd8, 0x467c1bc1, 0xaf5d4884};
uint32_t CACHE_LINE_ALIGN EccSpk[] = {0x353b1782, 0xd84265a7, 0x6284d0d1, 0xe7eb75b8, \
                                      0x7e8a1860, 0x5b178639, 0x9c3a7db0, 0x287afb63, \
                                      0xf15c23fd, 0x00566de2, 0xbdbea70c, 0x7410365a, \
                                      0x8707fe3a, 0x65c951f3, 0xd1290b98, 0xb5c37272};
uint32_t CACHE_LINE_ALIGN EccSpkHash[] = {0x27d91395, 0xa56c703d, 0x3e2d01b6, 0xb0f14b22, \
                                          0x8c1f761a, 0xb7a58e7e, 0x2117f2aa, 0x675894ec};
uint32_t CACHE_LINE_ALIGN EccSpkSig[] = {0xe3c97a36, 0x02d621e1, 0x27537ba6, 0x0e329852, \
                                         0x1f47cfbe, 0x248bac23, 0x9b6c9210, 0x03f4a646, \
                                         0xb9452241, 0x23000d9e, 0x45d424cf, 0x54c13de6, \
                                         0x3a147076, 0x7f14226a, 0x470d9853, 0xb20e9489};
uint32_t CACHE_LINE_ALIGN Sm2Ppk[] = {0xf9604902, 0x6cc22fa7, 0x67143dfa, 0x46d1bfed, \
                                      0xe6c76a64, 0xcf43fe4b, 0x0581d6a2, 0x977e745f, \
                                      0xa7686f34, 0xa2893ce0, 0x95ec3575, 0xf35fe8bf, \
                                      0xd25e1305, 0x67cb361c, 0x63d7fb9e, 0x527206a2};
uint32_t CACHE_LINE_ALIGN Sm2Spk[] = {0x856d0b0c, 0xba96854c, 0x47702880, 0x2c7d9e2a, \
                                      0x52265cd9, 0x28e628d7, 0x3ae862d4, 0xafc17053, \
                                      0xdfb34ca7, 0x9dde790e, 0xfb9142c8, 0xfab8cafe, \
                                      0xda12a484, 0x2a38b96e, 0xc8bc6f30, 0x5699606b};
uint32_t CACHE_LINE_ALIGN Sm2SpkHash[] = {0xb0fc8ccc, 0x1345a52c, 0xa6cda231, 0xf089bb25, \
                                          0xba071863, 0x9afe91fe, 0xd9f12c1a, 0xafc47d2c};
uint32_t CACHE_LINE_ALIGN Sm2SpkSig[] = {0xa5d3b779, 0x7272277f, 0x3b6b68b6, 0x170ed521, \
                                         0x134b4c45, 0x4c5b7064, 0xfc7caa4d, 0x700dbc03, \
                                         0x3e9f1e4a, 0xf2a7a7d4, 0x24079ffb, 0xc6132207, \
                                         0xbf6a97d8, 0xdf8d566d, 0x029d55e1, 0x74899200};

uint32_t CACHE_LINE_ALIGN EncCbcAesDataBuff[] = {0x486AEB42, 0xD121E0FB, 0xEE7681E8, 0xC84FA271, \
                                                 0x5947CC5C, 0x81E38E48, 0x09B6A262, 0xB3AD2AEB};
uint32_t CACHE_LINE_ALIGN EncCbcSm4DataBuff[] = {0x01B0BC04, 0x1EC5FABE, 0x6AE45260, 0xF5950598, \
                                                 0x176B25D8, 0xF88788BE, 0xE6179D17, 0x659F4368};

uint32_t CACHE_LINE_ALIGN PerformaceImageAddr[] = {IMAGE_1K_ADDR, IMAGE_4K_ADDR, \
                                                   IMAGE_8K_ADDR, IMAGE_16K_ADDR, \
                                                   IMAGE_32K_ADDR, IMAGE_64K_ADDR, \
                                                   IMAGE_128K_ADDR};
uint32_t CACHE_LINE_ALIGN PerformaceImageSize[] = {INPUT_DATA_SIZE_1K, INPUT_DATA_SIZE_4K, \
                                                   INPUT_DATA_SIZE_8K, INPUT_DATA_SIZE_16K, \
                                                   INPUT_DATA_SIZE_32K, INPUT_DATA_SIZE_64K, \
                                                   INPUT_DATA_SIZE_128K};

uint32_t CACHE_LINE_ALIGN Ida[] = {0x486AEB42, 0xD121E0FB, 0xEE7681E8, 0xC84FA271, \
                                   0x5947CC5C, 0x81E38E48, 0x09B6A262, 0xB3AD2AEB};

//CFG_MODE: ENCRYPT     HASH        DMA_SRC     DMA_DST     HASH_DST    KEY         DMA_MODE    HASH_BLOCK      ECBCBC      ENCDEC
//-----------------------------------------------------------------------------------------------------------------------------------
//case  0:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case  1:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case  2:  AES256      NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         ENC
//case  3:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         ENC
//case  4:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case  5:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case  6:  SM4         NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         ENC
//case  7:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         ENC
//case  8:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case  9:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 10:  AES256      NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         DEC
//case 11:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         DEC
//case 12:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 13:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case 14:  SM4         NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            ECB         DEC
//case 15:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         DEC
//case 16:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 17:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 18:  AES256      NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         ENC
//case 19:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         ENC
//case 20:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 21:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 22:  SM4         NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         ENC
//case 23:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         ENC
//case 24:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 25:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 26:  AES256      NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         DEC
//case 27:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         DEC
//case 28:  SM4         SM3         DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 29:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 30:  SM4         NONE        DDR         DDR         NONE        BHDR_KEY    BOTH_INCR   NONE            CBC         DEC
//case 31:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         DEC
//case 32:  NONE        NONE        DDR         DDR         NONE        NONE        BOTH_INCR   NONE            NONE        NONE



//WHOLE BLOCK and NONE HASH, if use for DST_INCR mode, change DmaIncMode to DST_INCR, if use BLOCK mode, change HashBlockMode
const TestCfgBothInc_t TestCfgBothIncData[] = \
            {{.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_NONE, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=(AL_U8)(AL_UINTPTR)AL_NULL, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL},\
        };


//CFG_MODE: ENCRYPT     HASH        DMA_SRC     DMA_DST     HASH_DST    KEY         DMA_MODE    HASH_BLOCK      ECBCBC      ENCDEC
//-----------------------------------------------------------------------------------------------------------------------------------
//case  0:  AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    SRC_INCR   WHOLE_BLOCK     ECB         DEC
//case  1:  AES256      SHA256      DDR         PCAP        DDR         USER_KEY    SRC_INCR   WHOLE_BLOCK     ECB         DEC
//case  2:  AES256      NONE        DDR         PCAP        NONE        BHDR_KEY    SRC_INCR   NONE            ECB         DEC
//case  3:  AES256      NONE        DDR         PCAP        NONE        USER_KEY    SRC_INCR   NONE            ECB         DEC
//case  4:  SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    SRC_INCR   WHOLE_BLOCK     ECB         DEC
//case  5:  SM4         SM3         DDR         PCAP        DDR         USER_KEY    SRC_INCR   WHOLE_BLOCK     ECB         DEC
//case  6:  SM4         NONE        DDR         PCAP        NONE        BHDR_KEY    SRC_INCR   NONE            ECB         DEC
//case  7:  SM4         NONE        DDR         PCAP        NONE        USER_KEY    SRC_INCR   NONE            ECB         DEC
//case  8:  AES256      SHA256      DDR         PCAP        DDR         BHDR_KEY    SRC_INCR   WHOLE_BLOCK     CBC         DEC
//case  9:  AES256      SHA256      DDR         PCAP        DDR         USER_KEY    SRC_INCR   WHOLE_BLOCK     CBC         DEC
//case 10:  AES256      NONE        DDR         PCAP        NONE        BHDR_KEY    SRC_INCR   NONE            CBC         DEC
//case 11:  AES256      NONE        DDR         PCAP        NONE        USER_KEY    SRC_INCR   NONE            CBC         DEC
//case 12:  SM4         SM3         DDR         PCAP        DDR         BHDR_KEY    SRC_INCR   WHOLE_BLOCK     CBC         DEC
//case 13:  SM4         SM3         DDR         PCAP        DDR         USER_KEY    SRC_INCR   WHOLE_BLOCK     CBC         DEC
//case 14:  SM4         NONE        DDR         PCAP        NONE        BHDR_KEY    SRC_INCR   NONE            CBC         DEC
//case 15:  SM4         NONE        DDR         PCAP        NONE        USER_KEY    SRC_INCR   NONE            CBC         DEC
//case 16:  NONE        NONE        DDR         PCAP        NONE        NONE        SRC_INCR   NONE            NONE        NONE

//WHOLE BLOCK and NONE HASH, if use for NONE_INCR mode, change DmaIncMode to NONE_INCR
const TestCfgBothInc_t TestCfgSrcIncData[] = \
            {{.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=AL_CIPHER_CSU_PCAP_WR_STREAM, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_NONE, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=(AL_U8)(AL_UINTPTR)AL_NULL, .DmaIncMode=AL_CIPHER_DMA_INCR_SRC, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL},\
            };



//CFG_MODE: ENCRYPT     HASH        DMA_SRC     DMA_DST     HASH_DST    KEY         DMA_MODE    HASH_BLOCK      ECBCBC      ENCDEC
//-----------------------------------------------------------------------------------------------------------------------------------
//case  0:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    BOTH_INCR   DEFAULT
//case  1:  AES256      SHA256      DDR         DDR         DDR         BHDR_KEY    DEFAULT
//case  2:  AES256      SHA256      DDR         DDR         DDR         DEFAULT
//case  3:  AES256      SHA256      DDR         DDR         DEFAULT
//case  4:  AES256      SHA256      DDR         DEFAULT
//case  5:  AES256      SHA256      DEFAULT
//case  6:  AES256      SM3
//case  7:  AES256      DEFAULT
//case  8:  SM4         SHA256
//case  9:  NONE        SHA256
//case 10:  NONE        SM3
//case 11:  DEFAULT

//TEST DMA ERROR BRANCH CFG
const TestCfgBothInc_t TestCfgDmaErrorBranch[] = \
            {{.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=0x4},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_BHDR, .DmaIncMode=0x4},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=0x0},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=0x0},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=0x0},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=0x0},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SM3},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=0x4},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SHA256},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_NONE, .HashMode=AL_CIPHER_OPS_HASH_SHA256},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_NONE, .HashMode=AL_CIPHER_OPS_HASH_SM3},\
            {.CryptMethod=0x0},\
        };

//CFG_MODE: ENCRYPT     HASH        DMA_SRC     DMA_DST     HASH_DST    KEY         DMA_MODE    HASH_BLOCK      ECBCBC      ENCDEC
//-----------------------------------------------------------------------------------------------------------------------------------
//case  1:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case  2:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         ENC
//case  3:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         ENC
//case  4:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         ENC
//case  5:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case  6:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         DEC
//case  7:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     ECB         DEC
//case  8:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            ECB         DEC
//case  9:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 10:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         ENC
//case 11:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         ENC
//case 12:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         ENC
//case 13:  AES256      SHA256      DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 14:  AES256      NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         DEC
//case 15:  SM4         SM3         DDR         DDR         DDR         USER_KEY    BOTH_INCR   WHOLE_BLOCK     CBC         DEC
//case 16:  SM4         NONE        DDR         DDR         NONE        USER_KEY    BOTH_INCR   NONE            CBC         DEC



//WHOLE BLOCK and NONE HASH, if use for DST_INCR mode, change DmaIncMode to DST_INCR, if use BLOCK mode, change HashBlockMode
const TestCfgBothInc_t TestPerformanceData[] = \
            {{.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_ECB, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_ENC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_SHA256, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_AES256, .HashMode=AL_CIPHER_OPS_HASH_NONE, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=AL_CIPHER_OPS_HASH_SM3, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=HASH_BUF_ADDR, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=AL_CIPHER_BLK_WHOLE, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
            {.CryptMethod=AL_CIPHER_OPS_CRYPT_SM4, .HashMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .DmaSrcAddr=TBM_INPUT_DATA_ADDR, .DmaDstAddr=TBM_OUTPUT_DATA_ADDR, .HashOutAddr=(AL_U32)(AL_UINTPTR)AL_NULL, \
            .KeyMode=AL_CIPHER_KEY_USER, .DmaIncMode=AL_CIPHER_DMA_INCR_BOTH, .HashBlockMode=(AL_U8)(AL_UINTPTR)AL_NULL, \
            .EcbCbc=AL_CIPHER_CRYPT_MODE_CBC, .EncDec=AL_CIPHER_CRYPT_DEC},\
        };

