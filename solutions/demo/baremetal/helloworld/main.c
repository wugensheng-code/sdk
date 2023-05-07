#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "nuclei_sdk_soc.h"

extern void * stack_top; /* Defined by the linker */
extern void * _sp;   /* Defined by the linker */

int main(void)
{
    char *str = "Hello World From Nuclei RISC-V Processor!";
    char *p;

    for (int i = 0; i < 20; i ++) {
        printf("%s %p %p\r\n", str, stack_top, _sp);
    }

    return 0;
}
