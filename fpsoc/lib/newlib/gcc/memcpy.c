/*
 * Copyright (c) 2013-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stddef.h>
#include <string.h>

// void *memcpy(void *dst, const void *src, size_t len)
// {
// 	const char *s = src;
// 	char *d = dst;

// 	while (len--)
// 		*d++ = *s++;

// 	return dst;
// }

#include <config.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
 
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
    
    char *suffix = (void*)(((uint32_t)src) + length);
    char *prefix = (void*)(((uint32_t)src) & (~(sizeof(uint32_t)-1)));
    uint32_t *middle = (void*)(((uint32_t)suffix) & (~(sizeof(uint32_t)-1)));
 
    
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
 
void *memcpy(void *dst, const void *src, size_t length)
{
    if ((((uint32_t)src) & (~(sizeof(uint32_t)-1))) != (((uint32_t)dst) & (~(sizeof(uint32_t)-1))))
    {
        char *lpSrc = (void*)src;
        char *lpDst = (void*)dst;
        char *lpTail = lpSrc + length;
        
        while(lpSrc < lpTail) *lpDst ++ = *lpSrc ++;
        
        return dst;
    }
    
    return fast_memcpy(dst, src, length);

}