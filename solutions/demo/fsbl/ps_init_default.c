#include <stdio.h>

int __attribute__((weak)) Soc_PlatInit(void)
{
	printf("Default ps init\r\n");
	return 0;
}
