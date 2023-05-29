/* See LICENSE of license details. */
#include <al_core.h>
#include <errno.h>

#undef errno
extern int errno;

__WEAK int _link(char* old, char* new)
{
    errno = EMLINK;
    return -1;
}
