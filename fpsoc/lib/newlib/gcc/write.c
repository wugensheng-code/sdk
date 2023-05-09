#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <nuclei_sdk_hal.h>
#include "al_log.h"

__WEAK ssize_t _write(int fd, const void* ptr, size_t len)
{
    (void)fd;

    return AlLog_Write(ptr, len);
}
