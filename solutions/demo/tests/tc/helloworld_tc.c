/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <common.h>
#include <stdlib.h>


AL_S32 helloworld_Tc()
{
    const AL_CHAR *str = "Hello World From Anlogic!";
    AL_U32 Size = 0x1;

    printf("str = %s \r\n", str);
    printf("    __  __     ____         _       ______       __    __ \r\n");
    printf("   / / / /_   / / /___     | |     / / __ \\_____/ ____/ / \r\n");
    printf("  / /_/ / _ \\/ / / __ \\    | | /| / / / / / ___/ / __  /  \r\n");
    printf(" / __  /  __/ / / /_/ /    | |/ |/ / /_/ / /  / / /_/ /   \r\n");
    printf("/_/ /_/\\___/_/_/\\____/     |__/|__/\\____/_/  /_/\\__ _/    \r\n");

    return 0;
}  

CMOCKA_TC_DEFINE(helloworld_Tc, NULL, NULL, NULL);