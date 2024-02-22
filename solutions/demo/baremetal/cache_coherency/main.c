/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include "al_core.h"
#include "al_ipc_hal.h"
#include <stdlib.h>

#define CORE0_READY  0xFFFE
#define CORE1_READY  0xEEEF
#define ARRAY_SIZE   100


static AL_U32 *Ptr = 0x100000;

AL_S32 main()
{
    AL_U32 Value = 0xFCC0;
    AL_U32 Flag = 0;
    AL_U32 Times = 0;

    AL_IpcMailBox_HalStruct *Notify;
    AL_IpcMailBox_HalStruct *Check;
    AL_IpcMailBox_HalStruct *Result;

    AlIPC_Hal_MboxInit(&Notify, 0);
    AlIPC_Hal_MboxInit(&Check, 1);
    AlIPC_Hal_MboxInit(&Result, 2);

#ifdef CORE0

    AL_U32 Passed = 0;

    AL_LOG(AL_LOG_LEVEL_INFO, "Waiting for data validation\r\n");
    AlIpc_Hal_MboxWrite(Result, 0xFFFF);

    while (Times < 100) {
        AlIpc_Hal_MboxRead(Notify, &Flag);

        while (Flag == CORE1_READY) {
            AlIpc_Hal_MboxRead(Check, &Passed);
            AL_LOG(AL_LOG_LEVEL_INFO, "The Value: %d , Addr: 0x%X verification passed\r\n", Value, Ptr);
            Value++;
            AlIpc_Hal_MboxWrite(Result, 0);

            for (AL_U32 Index; Index < ARRAY_SIZE; Index++) {
                Ptr[Index] = Value;
            }
            AlIpc_Hal_MboxWrite(Check, Value);
            AlIpc_Hal_MboxWrite(Notify, CORE0_READY);
            Times++;
            Flag = 0;
            Ptr++;
            break;
        }
    }


#endif
#ifdef CORE1
    AlIpc_Hal_MboxWrite(Notify, CORE1_READY);

    while (Times < 100) {
        AlIpc_Hal_MboxRead(Notify, &Flag);

        while (Flag == CORE0_READY) {
            AlIpc_Hal_MboxRead(Check, &Value);

            AL_BOOL Pass = AL_TRUE;

            for (AL_U32 Index; Index < ARRAY_SIZE; Index++) {
                if (Ptr[Index] != Value) {
                    Pass = AL_FALSE;
                }
            }

            if (Pass == AL_TRUE) {
                AlIpc_Hal_MboxWrite(Result, 1);
            }

            Pass = AL_TRUE;

            AlIpc_Hal_MboxWrite(Notify, CORE1_READY);
            Times++;
            Flag = 0;
            Ptr++;
            break;
        }
    }
    
    

#endif
    return 0;
}
