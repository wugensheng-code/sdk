#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################
#{{OTHERS}}

#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
#{{SDK_ROOT}}

#{{CHIP}}



COMPILE_PREFIX ?= 


#########################################################################
# 2. define SRC_DIR, INC_DIR, LD_LIBS, LIB_DIR
#    LD_LIBS: External library files that the app depends on
#    LIB_DIR: The path where the external library file resides
#    C_SRCS:   c sources for application
#    CXX_SRCS: cpp sources for application
#    ASM_SRCS: asm sources for application
#########################################################################

SRC_DIR += $(sort $(dir $(foreach dir, $(shell find "./" -type d), $(wildcard $(dir)/*.c))))

INC_DIR += $(sort $(dir $(foreach dir, $(shell find "./" -type d), $(wildcard $(dir)/*.h))))
#{{SRC_DIR}}
#{{INC_DIR}}

LD_LIBS  =
LIB_DIR  =

CXX_SRCS :=
ASM_SRCS :=


#########################################################################
# 3. CFLAGS
#########################################################################
CFLAGS := -O2 -g
#{{CFLAGS}}

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
PLAT_DIR ?= $(SDK_ROOT)/platform


#########################################################################
# 5. define build target
#########################################################################
#{{TARGET}}


#########################################################################
# 6. define ARMv8_CORE for apu
#########################################################################
#{{ARMv8_CORE}}

#########################################################################
# 7. linker scripts
#########################################################################
ARMv8_STATE ?= 64
ifeq ($(CHIP), dr1v90)
ARCH_ABI := lp64d
else ifeq ($(ARMv8_STATE),64)
ARCH_ABI := aarch64
else ifeq ($(ARMv8_STATE),32)
ARCH_ABI := aarch32
endif

#{{LINKER_SCRIPT}}

# include rules.mk
include $(SDK_ROOT)/tools/make/rules.mk

# vim: syntax=make
