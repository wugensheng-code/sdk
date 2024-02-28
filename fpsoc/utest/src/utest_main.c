/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "al_cli.h"
#include "gcov_public.h"
#include "gcov_gcc.h"


AL_U32 main(AL_VOID)
{
    __gcov_call_constructors();

    AL_U32 Size = 0x1;
    for (int i = 0; i < 20; i ++)
    {
        AL_U8 *p;

        p = malloc(Size);
        if (p == NULL) {
            printf("malloc failed Size = %d \r\n", Size);
        } else {
            printf("malloc succeed %p size=%d \r\n", p, Size);
        }
        Size <<= 1;
        free(p);
    }

    AlCli_Init(AL_CLI_CONSOLE_UART);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    AlCli_Main(AL_NULL);
    __gcov_exit();

    return 0;
}
