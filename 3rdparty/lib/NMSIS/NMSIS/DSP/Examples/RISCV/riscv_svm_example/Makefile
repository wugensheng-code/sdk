#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################


#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
SDK_ROOT   ?= ../../../../../../../../

CHIP            ?= dr1v90
ARMv8_STATE     := 64

ifeq ($(CHIP), dr1m90)

ifeq ($(ARMv8_STATE), 32)
COMPILE_PREFIX ?= /opt/toolchain/13.2/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-
else
COMPILE_PREFIX ?= /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
endif

else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif


DOWNLOAD = tcm

#########################################################################
# 2. define SRC_DIR, INC_DIR, LD_LIBS, LIB_DIR
#    LD_LIBS: External library files that the app depends on
#    LIB_DIR: The path where the external library file resides
#    C_SRCS:   c sources for application
#    CXX_SRCS: cpp sources for application
#    ASM_SRCS: asm sources for application
#########################################################################
SRC_DIR  = ./

INC_DIR += ./


LD_LIBS  =
LIB_DIR  =

C_SRCS   :=

CXX_SRCS :=
ASM_SRCS :=

#########################################################################
# 3. CFLAGS
#########################################################################
CFLAGS := -O2 -g

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_emulation
PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v10
#PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v20
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_hpf_v20



#########################################################################
# 5. define build target
#########################################################################
TARGET := riscv_svm_example


all: $(TARGET)
	@$(ECHO) COMPILE_PREFIX = $(COMPILE_PREFIX)

# include rules.mk
include $(SDK_ROOT)/tools/make/rules.mk

# vim: syntax=make
