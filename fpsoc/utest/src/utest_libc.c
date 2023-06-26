#include "al_cli.h"
#include "al_type.h"
#include "al_utest.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static AL_S32 AL_Libc_UnitTest(AL_U32 Argc, AL_S8 *Argv[])
{
    AL_UCHAR *P   = AL_NULL;
    AL_S32   Ret  = AL_OK;
    AL_UTEST_CMDENUM Id = (AL_UTEST_CMDENUM)atoi(Argv[0]);

    if (Argc < 1) {
        return AL_UTEST_ERR_ILLEGAL_PARAM;
    }

    switch (Id) {

    case UTEST_MALLOC: {

        AL_U32 Size = (AL_U32)atoi(Argv[1]);
        AL_UTEST_PRINT_START(UTEST_MALLOC, "%d", Size);
        P = malloc(Size);
        if (P == AL_NULL) {
            Ret = AL_UTEST_ERR_NOMEM;
            AL_UTEST_FAIL_END(UTEST_MALLOC, "");
        } else {
            AL_UTEST_PASS_END(UTEST_MALLOC, "0x%8x", P);
        }

        break;
    }

    case UTEST_FREE: {
        AL_UTEST_PRINT_START(UTEST_FREE, "0x%8x", P);
        P = (AL_UCHAR *)atoi(Argv[1]);
        free(P);
        AL_UTEST_PASS_END(UTEST_FREE, "");

        break;
    }

    case UTEST_MEMSET: {
        P = (AL_UCHAR *)atoi(Argv[1]);
        AL_UTEST_PRINT_START(UTEST_MEMSET, "0x%8x %d %d", P, atoi(Argv[2]), atoi(Argv[3]));
        memset(P, atoi(Argv[2]), atoi(Argv[3]));
        AL_UTEST_PASS_END(UTEST_MEMSET, "");

        break;
    }

    case UTEST_FILL_PATTERN_0: {
        AL_U32 Size = atoi(Argv[2]);
        P = atoi(Argv[1]);

        AL_UTEST_PRINT_START(UTEST_FILL_PATTERN_0, "0x%8x %d", P);

        for (; Size > 0; Size--) {
            *P ++ = (Size & 0xFF);
        }

        AL_UTEST_PASS_END(UTEST_FILL_PATTERN_0, "");

        break;
    }

    default:
        AL_UTEST_PRINT_START(Id, "");
        AL_UTEST_FAIL_END(Id, "");
        Ret = AL_UTEST_ERR_ILLEGAL_PARAM;
        break;
    }

    return Ret;
}

AL_CLI_CMD_REGISTER(AL_Libc_UnitTest, unitylibc, unity-libc);
