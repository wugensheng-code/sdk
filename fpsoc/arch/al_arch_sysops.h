/*
 * Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */


#ifndef __AL_ARCH_H_
#define __AL_ARCH_H_

#ifndef STRINGIFY
#define STRINGIFY(s)            #s
#endif

#define ARCH_SYSREG_READ(Name)                                   \
    ({                                                           \
        AL_REGISTER __V;                                         \
        __ASM volatile(ISA_SYSREG_READ" %[__V], " STRINGIFY(Name)       \
                     : [__V]"=r"(__V)                            \
                     :                                           \
                     : "memory");                                \
        __V;                                                     \
    })

#define ARCH_SYSREG_WRITE(Name, Val)                             \
    ({                                                           \
        AL_REGISTER volatile __V = (AL_REGISTER)(Val);           \
        __ASM volatile(ISA_SYSREG_WRITE" "STRINGIFY(Name) ", %[__V]"    \
                     :                                           \
                     : [__V]"rK"(__V)                            \
                     : "memory");                                \
    })

#ifdef ISA_SYSREG_WRITE_CONST
#define ARCH_SYSREG_WRITE_CONST(Name, Val)                       \
    ({                                                           \
        __asm__ volatile (ISA_SYSREG_WRITE_CONST" "#Name ", %0"         \
        :                                                        \
        : "i" (Val)                                              \
        : "memory");                                             \
    })
#endif

#ifdef ISA_SWAP

#define ARCH_SYSREG_SWAP(Name, Val)                                 \
    ({                                                              \
        AL_REGISTER __V = (AL_REGISTER)(Val);                       \
        __ASM volatile(ISA_SWAP" %[Out], " STRINGIFY(Name) ", %[In]"       \
                     : [Out]"=r"(__V)                                \
                     : [In]"rK"(__V)                                \
                     : "memory");                                    \
        __V;                                                         \
    })
#endif


#ifdef ISA_READ_SET

#define ARCH_SYSREG_READ_SET(Name, Val)                          \
    ({                                                          \
        AL_REGISTER __V = (AL_REGISTER)(Val);                \
        __ASM volatile(ISA_READ_SET" %[Out], " STRINGIFY(Name) ", %[In]"       \
                     : [Out]"=r"(__V)                                \
                     : [In]"rK"(__V)                                \
                     : "memory");                               \
        __V;                                                    \
    })

#endif


#ifdef ISA_SET

#define ARCH_SYSREG_SET(Name, Val)                                  \
    ({                                                          \
        AL_REGISTER __V = (AL_REGISTER)(Val);                   \
        __ASM volatile(ISA_SET" "STRINGIFY(Name) ", %[__V]"      \
                     :                                          \
                     : [__V]"rK"(__V)                           \
                     : "memory");                               \
    })

#endif

#ifdef ISA_READ_CLEAR

#define ARCH_SYSREG_READ_CLEAR(Name, Val)                           \
    ({                                                          \
        AL_REGISTER __V = (AL_REGISTER)(Val);                \
        __ASM volatile(ISA_READ_CLEAR" %[OUT], " STRINGIFY(Name) ", %[IN]"       \
                     : [OUT]"=r"(__V)                                \
                     : [IN]"rK"(__V)                                \
                     : "memory");                               \
        __V;                                                    \
    })
#endif

#ifdef ISA_CLEAR

#define ARCH_SYSREG_CLEAR(Name, Val)                                \
    ({                                                          \
        AL_REGISTER __V = (AL_REGISTER)(Val);                \
        __ASM volatile(ISA_CLEAR" "STRINGIFY(Name) ", %[__V]"            \
                     :                                          \
                     : [__V]"rK"(__V)                                \
                     : "memory");                               \
    })
#endif





#endif /* AL_AARCH64_ARCH */
