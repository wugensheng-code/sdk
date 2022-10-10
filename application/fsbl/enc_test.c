#include <stdio.h>

#include "demosoc.h"

#include "alfsbl_sec.h"
#include "test_data.h"
#include "enc_test.h"
extern SecureInfo FsblSecInfo;

extern unsigned int input_data_buff[DATA_BUFF_SIZE];
extern unsigned int output_data_buff[DATA_BUFF_SIZE];
extern unsigned int iv_data_buff[IV_BUFF_SIZE];
extern unsigned int key_data_buff[KEY_BUFF_SIZE];
extern unsigned int result_data_buff[DATA_BUFF_SIZE];
extern unsigned int hash_data_buff[HASH_DATA_SIZE];

uint32_t CsuDmaTest(SecureInfo *pSecInfo, uint8_t EncDir, uint32_t DataLen);

uint32_t CheckBuf(uint32_t *Buf0, uint32_t *Buf1, uint32_t Length);
void PrintBuf(uint32_t *Buf, uint32_t ByteLength);



int enc_test_aes(void)
{
	uint32_t Status = 0;
	printf("csu security test...\r\n");

	SecureIrqInit();
	AesDataInit();
//	PrintBuf(input_data_buff, 128);

	FsblSecInfo.InputAddr  = input_data_buff;
	FsblSecInfo.OutputAddr = output_data_buff;
	CsuDmaTest(&FsblSecInfo, SYM_ENCRYPT, 128);
	PrintBuf(output_data_buff, 128);


	FsblSecInfo.InputAddr  = output_data_buff;
	FsblSecInfo.OutputAddr = result_data_buff;
	CsuDmaTest(&FsblSecInfo, SYM_DECRYPT, 128);
	PrintBuf(result_data_buff, 128);

	Status = CheckBuf(result_data_buff, input_data_buff, 128);

	return 0;
}


uint32_t CsuDmaTest(SecureInfo *pSecInfo, uint8_t EncDir, uint32_t DataLen)
{
	uint32_t Status;

	pSecInfo->AuthType   = OP_AUTH_NONE;
	pSecInfo->HashType   = OP_HASH_NONE;
	pSecInfo->EncType    = OP_ENCRYPT_AES256;
	pSecInfo->EncMode    = SYM_ECB;
	pSecInfo->EncDir     = EncDir;
	pSecInfo->KeyMode    = OP_USER_KEY;
	pSecInfo->DataLength = DataLen;
	pSecInfo->KeyAddr    = key_data_buff;

	Status = AlFsbl_DecHash_1(pSecInfo);

	return Status;
}


uint32_t CheckBuf(uint32_t *Buf0, uint32_t *Buf1, uint32_t Length)
{
	uint32_t i;
    /* Check for DMA transfer result */
    for (i = 0; i < (Length / 4); i++) {
        if (Buf0[i] != Buf1[i]) {
            printf("Data not equal\r\n");
            return AL_FAILED;
        }
    }
    printf("Data equal\r\n");
    return AL_SUCCESS;
}


void PrintBuf(uint32_t *Buf, uint32_t ByteLength)
{
	uint32_t i;
	for(i = 0; i < (ByteLength / 4); i++) {
		printf("%08x ", Buf[i]);
		if((i + 1) % 4 == 0) {
			printf("\r\n");
		}
	}
	printf("\r\n");
}







