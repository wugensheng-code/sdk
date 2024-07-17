/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#if defined __aarch64__ || defined __arm__

void *fast_memcpy(void *dst, const void *src, size_t length)
{
    union{
        char *lpstr;
        uint32_t *lpint;
    }s;
    union{
        char *lpstr;
        uint32_t *lpint;
    }d;

    char *suffix = (void*)(((uint64_t)src) + length);
    char *prefix = (void*)(((uint64_t)src) & (~(sizeof(uint32_t)-1)));
    uint32_t *middle = (void*)(((uint64_t)suffix) & (~(sizeof(uint32_t)-1)));


    s.lpstr = (void*)src;
    d.lpstr = (void*)dst;

    if (prefix != src)
    {
        while(s.lpstr < prefix + sizeof(uint32_t))
        {
            *d.lpstr++ = *s.lpstr ++;
        }
    }

    while(s.lpint < middle - (sizeof(uint32_t) * 8))
    {
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
    }

    while(s.lpint < middle - (sizeof(uint32_t) * 4))
    {
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
    }

    while(s.lpint < middle - (sizeof(uint32_t) * 2))
    {
        *d.lpint++ = *s.lpint ++;
        *d.lpint++ = *s.lpint ++;
    }

    while(s.lpint < middle) *d.lpint++ = *s.lpint ++;
    while(s.lpstr < suffix) *d.lpstr++ = *s.lpstr ++;

    return dst;
}

void *al_memcpy(void *dst, const void *src, size_t length)
{
    if ((((uint32_t)(uint64_t)src) & (~(sizeof(uint32_t)-1))) != (((uint32_t)(uint64_t)dst) & (~(sizeof(uint32_t)-1))))
    {
        char *lpSrc = (void*)src;
        char *lpDst = (void*)dst;
        char *lpTail = lpSrc + length;

        while(lpSrc < lpTail) *lpDst ++ = *lpSrc ++;

        return dst;
    }

    return fast_memcpy(dst, src, length);

}

#ifndef ENABLE_MMU
/*
 * MMU is disabled, so only al_memcpy can be used.
 * Therefore, memcpy is implemented as al_memcpy.
 */
void *memcpy(void *dst, const void *src, size_t length){
    return al_memcpy(dst, src, length);
}
#else
/*
 * MMU is enabled, so we need to distinguish between the following cases:
 *
 * 1. If the variable is noncacheable, the C library's memcpy cannot be used.
 *    Therefore, we should manually use al_memcpy.
 *
 * 2. If the variable is cacheable, the C library's memcpy can be used due to
 *    its higher efficiency. Use the C library's memcpy.
 */
#endif

#endif
