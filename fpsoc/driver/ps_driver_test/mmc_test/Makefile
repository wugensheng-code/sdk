#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################


#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
SDK_ROOT   ?= ../../../..


CHIP        ?= dr1m90
ELF         ?= SD
ARMv8_STATE ?= 64

ifeq ($(CHIP), dr1m90)
ifeq ($(ARMv8_STATE), 32)
COMPILE_PREFIX ?= /opt/toolchain/13.2/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-
else
COMPILE_PREFIX ?= /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
endif
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif




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
CFLAGS := -O2 -DMMC_DEBUG -g
ifeq ($(ELF), SD)
CFLAGS += -DCONFIG_SD_TEST=1
else ($(ELF), EMMC)
CFLAGS += -DCONFIG_EMMC_TEST=1
endif

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_emulation
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v10
PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v20

#########################################################################
# put these option to board_dr1x90_emulation Makefile
## Available choices:
## PINMUX_MODE1, PINMUX_MODE2, PINMUX_MODE3, PINMUX_MODE_SD1
#########################################################################
ifeq ($(ELF), SD)
PINMUX_MODE := PINMUX_MODE1
else ($(ELF), EMMC)
PINMUX_MODE := PINMUX_MODE2
endif

PINMUX_MODE ?= PINMUX_MODE1


#########################################################################
# 5. define build target
#########################################################################
TARGET := mmc_test

ifneq ($(PINMUX_MODE),)
CFLAGS += -DENABLE_$(PINMUX_MODE)=1
TARGET:=$(TARGET)_$(ELF)_$(CHIP)_$(ARMv8_STATE)_$(PINMUX_MODE)
endif

all: $(TARGET)
	@$(ECHO) COMPILE_PREFIX = $(COMPILE_PREFIX)

# include rules.mk
include $(SDK_ROOT)/tools/make/rules.mk

# vim: syntax=make
