/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __AL_UTEST_H_
#define __AL_UTEST_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif

#endif

#include "al_type.h"
#include <stdio.h>

#define AL_UTEST_ERR_ILLEGAL_PARAM       (AL_DEF_ERR(AL_UTEST, AL_LOG_LEVEL_ERROR, AL_ERR_ILLEGAL_PARAM))
#define AL_UTEST_ERR_NOMEM               (AL_DEF_ERR(AL_UTEST, AL_LOG_LEVEL_ERROR, AL_ERR_NOMEM))


#define AL_UTEST_PRINT_START(cmdid, fmt, arg...)                \
do {                                                            \
    printf("UTest %d: "fmt" ==>", cmdid, ##arg);                \
} while(0)

#define AL_UTEST_PASS_END(cmdid, fmt, arg...)                   \
do {                                                            \
    printf("UTest %d: "fmt" pass <==", cmdid, ##arg);           \
} while(0)

#define AL_UTEST_FAIL_END(cmdid, fmt, arg...)                   \
do {                                                            \
    printf("UTest %d: "fmt" fail <==", cmdid, ##arg);           \
} while(0)


typedef enum {
    UTEST_MALLOC            = 0x0,
    UTEST_FREE              = 0x1,
    UTEST_MEMSET            = 0x2,
    UTEST_FILL_PATTERN_0    = 0x3,

    UTEST_UART_IOCTL        = 0x20,
    UTEST_UART_SENDBLOCK    = 0x21,
} AL_UTEST_CMDENUM;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif  /* AL_UTEST_H */