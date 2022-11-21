#include "qspi_test_driver.h"

QspiParams norParams;

void main(void)
{
    uint32_t status;

    status = Test_QspiInit(&norParams);
    if(status != QSPI_SUCCESS)
    {
        printf("Test_QspiInit error:%d\r\n",status);
        while(1);
    }

    status = Test_QspiInit(&norParams);
    if(status != QSPI_SUCCESS)
    {
        printf("Test_QspiInit error:%d\r\n",status);
        while(1);
    }

    return 0;

}