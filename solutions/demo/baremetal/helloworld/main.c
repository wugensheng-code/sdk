/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include "al_core.h"
#include <stdlib.h>

AL_S32 main()
{
    const AL_CHAR *str = "Hello World From Anlogic!";

    al_printf("str = %s \r\n", str);
    al_printf("    __  __     ____         _       ______       __    __ \r\n");
    al_printf("   / / / /_   / / /___     | |     / / __ \\_____/ ____/ / \r\n");
    al_printf("  / /_/ / _ \\/ / / __ \\    | | /| / / / / / ___/ / __  /  \r\n");
    al_printf(" / __  /  __/ / / /_/ /    | |/ |/ / /_/ / /  / / /_/ /   \r\n");
    al_printf("/_/ /_/\\___/_/_/\\____/     |__/|__/\\____/_/  /_/\\__ _/    \r\n");

    return 0;
}
