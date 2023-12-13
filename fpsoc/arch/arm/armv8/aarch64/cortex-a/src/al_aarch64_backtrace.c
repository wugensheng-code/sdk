/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "al_type.h"
#include "al_utils_def.h"

#define AL_CALL_STACK_DEPTH 32

extern AL_UINTPTR _heap_end;
extern AL_UINTPTR _sp;

AL_VOID AlBackTrace_Fault(AL_UINTPTR *Sp)
{
    AL_UINTPTR *EndofStack = (AL_UINTPTR *)(&_heap_end);
    AL_UINTPTR *StartofStack = (AL_UINTPTR *)(&_sp);
    AL_UINTPTR *CurSp = Sp;
    AL_UINTPTR *Lr, *Fp, *Pc, *Arg;
    AL_UINTPTR FuncAddr, SpOffset, FuncArgOffset;   /* Offset in byte */
    AL_UINTPTR CallDepth[AL_CALL_STACK_DEPTH];
    AL_U32 CurDepth = 0;
    AL_U32 FuncArgNum;

    // AL_ASSERT(((Sp >= EndofStack) && (Sp <= StartofStack)), AL_ERR_ILLEGAL_PARAM);

    /* skip restore registers */
    CurSp += 32;
    Fp = CurSp;
    Lr = Fp + 1;
    Pc = (AL_UINTPTR *)((*Lr) - 4);

    do {
        printf("%d: FP: 0x%016lx PC: 0x%016lx\r\n", CurDepth, (AL_UINTPTR)Fp, (AL_UINTPTR)Pc);
        /* Print frame */
        if (*Fp > (AL_UINTPTR)(Fp + 2)) {
            /* Decode instruction of str general register operation for income parameters */
            if (((*Pc >> 24) & 0xff) == 0x94) {
                FuncAddr = (AL_UINTPTR)Pc + ((*Pc & 0xffffff) << 2);
                SpOffset = (~(((*(AL_UINTPTR *)FuncAddr) >> 15) & 0x3f) + 1) & 0x3f;
                /* Jump stp x29, x30 and mov sp to x29 */
                FuncAddr += 8;
                if (((((*(AL_UINTPTR *)FuncAddr) >> 22) & 0x3ff) == 0x3e4) ||
                    ((((*(AL_UINTPTR *)FuncAddr) >> 22) & 0x3ff) == 0x2e4)) {
                    FuncArgOffset = ((*(AL_UINTPTR *)FuncAddr) >> 10) & 0xfff;
                } else {
                    continue;
                }
                /* str Xn, [sp] */
                FuncArgNum = 0;
                Arg = (AL_UINTPTR *)*Fp - SpOffset + FuncArgOffset;
                while ((((((*(AL_UINTPTR *)FuncAddr) >> 22) & 0x3ff) == 0x3e4) ||
                    ((((*(AL_UINTPTR *)FuncAddr) >> 22) & 0x3ff) == 0x2e4)) &&
                    (((*(AL_UINTPTR *)FuncAddr) & 0x1f) == FuncArgNum)) {
                    printf("arg %d: ptr - 0x%016lx, val - 0x%016lx\r\n",FuncArgNum, (AL_UINTPTR)Arg, *Arg);
                    FuncArgNum += 1;
                    Arg -= 1;
                    FuncAddr += 4;
                }
            }
            /* TODO: Print remain local variable */
        }
        if (CurDepth < AL_CALL_STACK_DEPTH) {
            CallDepth[CurDepth++] = (AL_UINTPTR)Pc;
        }
        CurSp = Fp;
        Fp = (AL_UINTPTR *)*Fp;
        Lr = Fp + 1;
        Pc = (AL_UINTPTR *)((*Lr) - 4);
    } while((Fp < StartofStack) && (Fp > EndofStack));

    printf("Show more call stack info by run: addr2line -e <elfname> -afpiC ");
    for (AL_U32 i = 0; i < CurDepth; i++) {
        printf("%08lx ", CallDepth[i]);
    }
    printf("\r\n");
}