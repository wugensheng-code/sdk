
#ifndef TEST_DATA_H
#define TEST_DATA_H

#include "al_types.h"

/* Data size in words */
#define DATA_BUFF_SIZE 64 /* 256 bytes */
#define IV_BUFF_SIZE 8    /* 256 bits */
#define KEY_BUFF_SIZE 8   /* 256 bits */
#define HASH_DATA_SIZE 8  /* 256 bits */

#define MSG_RAM_BASEADDR 0xf8080000
#define MSG_RAM_CSU 0x60000
#define ROM_BASEADDR 0x00000000
#define RAM_BASEADDR 0x00020000
#define PKU_BASEADDR 0x00064000
#define AES_BASEADDR 0x00065000
#define HASH_BASEADDR 0x00066000
#define SM4_BASEADDR 0x0006A000
#define SOC_BASEADDR 0x00100000
#define SOC_ENDADDR 0x00104000



//#define MSG_RAM_BASEADDR 0x00060000

//extern unsigned int input_data_buff[DATA_BUFF_SIZE];
//extern unsigned int output_data_buff[DATA_BUFF_SIZE];
extern unsigned int * ibuff ;
extern unsigned int * obuff ;
extern unsigned int iv_data_buff[IV_BUFF_SIZE];
extern unsigned int key_data_buff[KEY_BUFF_SIZE];
extern unsigned int result_data_buff[DATA_BUFF_SIZE];
extern unsigned int hash_data_buff[HASH_DATA_SIZE];

extern unsigned int const aes256_en_gold_128byte[];
extern unsigned int const aes256_de_gold_128byte[];
extern unsigned int const SM4_en_gold_128byte[];
extern unsigned int const SM4_de_gold_128byte[];
extern unsigned int const aes256_en_cbc_gold_128byte[];
extern unsigned int const SM4_en_cbc_gold_128byte[];

void TestDataInit(void);
void AesDataInit(void);
void Sm4DataInit(void);

#endif
