/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* See LICENSE of license details. */
#include <errno.h>
#include <sys/time.h>
#include <al_core.h>

__WEAK int _gettimeofday(struct timeval* tp, void* tzp)
{
    return -1;
}
