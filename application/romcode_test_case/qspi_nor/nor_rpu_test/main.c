#include "qspi_test_driver.h"

QspiParams norParams;


uint8_t ReadBuf[100] = {0};
uint8_t WriteBuf[100] = {0};

#define READ_SIZE sizeof(ReadBuf)
#define WRITE_SIZE sizeof(WriteBuf)

void main(void)
{
    uint32_t status;

    Qspi_PinmuxInit();

    status = Csu_QspiInit(&norParams);
    if(status != AL_SUCCESS)
    {
        printf("Csu_QspiInit error:%d\r\n",status);
        while(1);
    }

    printf("flashID:%x\r\n", norParams.flashID);
    printf("flashSize:%x\r\n", norParams.flashSize);

    Qspi_ChipErase();

    Test_QspiProgram(0, WriteBuf, WRITE_SIZE);

    Csu_QspiRead(0, ReadBuf, READ_SIZE);
    return 0;

}