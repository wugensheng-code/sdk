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
    AlCli_Init(AL_CLI_CONSOLE_UART);
    AlIntr_SetLocalInterrupt(AL_FUNC_ENABLE);
    AlCli_Main(AL_NULL);
    __gcov_exit();

    return 0;
}
