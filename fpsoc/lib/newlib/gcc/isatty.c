/* See LICENSE of license details. */
#include <al_core.h>
#include <unistd.h>

__WEAK int _isatty(int fd)
{
    return 1;
}
