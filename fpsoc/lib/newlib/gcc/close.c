/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* See LICENSE of license details. */
#include <al_core.h>
#include <errno.h>

#undef errno
extern int errno;

__WEAK int _close(int fd)
{
    errno = EBADF;
    return -1;
}
