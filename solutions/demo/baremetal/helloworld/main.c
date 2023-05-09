#include <stdio.h>
#include <math.h>
#include <time.h>
#include <al_type.h>
#include <stdlib.h>


AL_VOID main()
{
    AL_U8 *str = "Hello World From Anlogic!";
    AL_U8 *p;
    AL_U32 Size = 0x1;

    printf("str = %s \r\n", str);

    for (int i = 0; i < 20; i ++)
    {
        p = malloc(Size);
        if (p == NULL) {
            printf("malloc failed Size = %d \r\n", Size);
        } else {
            printf("malloc succeed %p size=%d \r\n", p, Size);

        }
        Size <<= 1;
        free(p);
    }
}
