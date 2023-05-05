#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"


int main(void)
{
    char *str = "Hello World From Nuclei RISC-V Processor!";
    char *p;

    for (int i = 0; i < 20; i ++) {
        printf("%d strlen=%d %s\r\n", i, strtol(str, &p, 10),  str);
    }

    return 0;
}
