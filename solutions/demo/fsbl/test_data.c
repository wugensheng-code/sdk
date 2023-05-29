#include "test_data.h"
#include "stdint.h"

unsigned int input_data_buff[DATA_BUFF_SIZE];
unsigned int output_data_buff[DATA_BUFF_SIZE];
unsigned int iv_data_buff[IV_BUFF_SIZE];
unsigned int key_data_buff[KEY_BUFF_SIZE];
unsigned int result_data_buff[DATA_BUFF_SIZE];
unsigned int hash_data_buff[HASH_DATA_SIZE];

/**
 * Hash result golden data
 */
unsigned int const aes256_en_gold_128byte[] = {
    0x729671F5, 0xC1588D43, 0x413331B4, 0xF5675027,
    0xF7B4B3DD, 0xB0162296, 0x402843E4, 0x173C9063,
};

unsigned int const aes256_en_cbc_gold_128byte[] = {
    0x87B55778, 0x9387FE21, 0xC9384F98, 0xF44B53C4,
    0x39C72830, 0xC4D9E763, 0xCB8D4917, 0x983F7871,
};

unsigned int const aes256_de_gold_128byte[] = {
    0xE765CC54, 0xD52685ED, 0xDADC2C9E, 0x3247D152,
    0x1C7F3EAE, 0x3273FCFF, 0x74321FB6, 0x3FB69DE1,
};

unsigned int const SM4_en_gold_128byte[] = {
    0x91682438, 0x6F989FE6, 0xB80C51A2, 0x899A1412,
    0xF9ECEC9A, 0xD8662183, 0xF42CAB67, 0x24C6CD9A,
};

unsigned int const SM4_en_cbc_gold_128byte[] = {
    0xDCFD596F, 0xD9A28CE5, 0x03338B19, 0x8DC4B94D,
    0xF82F6F8C, 0x11CD4463, 0xDDF69054, 0x4AB4CEF1,
};

unsigned int const SM4_de_gold_128byte[] = {
    0x9F97424A, 0x99CAEC20, 0x7DCC9E93, 0x2735E57D,
    0x6EB3AED7, 0x27F0D439, 0x67CF7955, 0xEEF3F4AB,
};

/**
 * Init data for DMA test
 */
void TestDataInit()
{
    unsigned int i;
    for (i = 0; i < DATA_BUFF_SIZE; i++) {
        input_data_buff[i] = 0x12345678 + i;
    }
}

/**
 * Init data for AES test
 */
void AesDataInit(void)
{
    unsigned int i;

    for (i = 0; i < KEY_BUFF_SIZE; i++) {
        key_data_buff[i] = 0x11111111 * (i + 1);
//        key_data_buff[i] = 0xffffffff;
    }

    for (i = 0; i < DATA_BUFF_SIZE; i += 4) {
        input_data_buff[i]     = 0x11111111 * (i / 4);
        input_data_buff[i + 1] = 0x11111111 * (i / 4);
        input_data_buff[i + 2] = 0x11111111 * (i / 4);
        input_data_buff[i + 3] = 0x11111111 * (i / 4);
    }

    for (i = 0; i < IV_BUFF_SIZE; i++) {
        iv_data_buff[i] = 0x00000001 + i;
    }
}

/**
 * Init data for SM4 test
 */
void Sm4DataInit(void)
{
    unsigned int i;

    key_data_buff[0] = 0x01234567;
    key_data_buff[1] = 0x89abcdef;
    key_data_buff[2] = 0xfedcba98;
    key_data_buff[3] = 0x76543210;

    for (i = 0; i < DATA_BUFF_SIZE; i += 4) {
        input_data_buff[i]     = 0x11111111 * (i / 4);
        input_data_buff[i + 1] = 0x11111111 * (i / 4);
        input_data_buff[i + 2] = 0x11111111 * (i / 4);
        input_data_buff[i + 3] = 0x11111111 * (i / 4);
    }

    for (i = 0; i < IV_BUFF_SIZE; i++) {
        iv_data_buff[i] = 0x00000001 + i;
    }
}
