/* See LICENSE of license details. */
#include <errno.h>
#include <sys/time.h>
#include "nuclei_sdk_soc.h"

__WEAK int _gettimeofday(struct timeval* tp, void* tzp)
{
    return -1;
}
