/* See LICENSE of license details. */
#include <al_core.h>
#include <errno.h>
#undef errno
extern int errno;

__WEAK int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}
