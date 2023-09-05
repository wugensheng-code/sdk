/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* See LICENSE of license details. */
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <al_core.h>

__WEAK ssize_t _read(int fd, void* ptr, size_t len)
{
    if (fd != STDIN_FILENO) {
        return -1;
    }

    return -1;
}
