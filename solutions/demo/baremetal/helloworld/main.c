#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"


int main(void)
{
    for (int i = 0; i < 20; i ++) {
        printf("%d: Hello World From Nuclei RISC-V Processor!\r\n", i);
    }

    return 0;
}
