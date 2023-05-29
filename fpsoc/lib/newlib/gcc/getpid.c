/* See LICENSE of license details. */
#include <al_core.h>
#include <errno.h>

__WEAK int _getpid(void)
{
    return 1;
}
