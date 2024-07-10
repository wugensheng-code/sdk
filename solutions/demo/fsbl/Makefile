#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################


#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
SDK_ROOT   ?= ../../..


CHIP       ?= dr1v90

DOWNLOAD      = ocm
# ENABLE_MMU    := 1

ifeq ($(CHIP), dr1m90)
ARMv8_EL         := EL3
COMPILE_PREFIX ?= /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
DDR_2M_MAPPING   := 0
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
SRC_DIR  = .

INC_DIR  = .

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
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v20
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/platform
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_hpf_v10
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_dc_v1d1

#########################################################################
# put these option to board_dr1x90_emulation Makefile
## Available choices:
## PINMUX_MODE1, PINMUX_MODE2, PINMUX_MODE3, PINMUX_MODE_SD1
#########################################################################
PINMUX_MODE ?= PINMUX_MODE1
# OUTPUT_DEV  := NONE

#########################################################################
# 5. fsbl internal Macro
#########################################################################
#CFLAGS += -DQSPI_USE_AHB_DMA
#CFLAGS += -DQSPI_XIP_THROUTH_CSU_DMA


#########################################################################
# 6. define build target
#########################################################################
TARGET := fsbl

ifneq ($(PINMUX_MODE),)
CFLAGS += -DENABLE_$(PINMUX_MODE)=1
TARGET:=$(TARGET)_$(CHIP)_$(PINMUX_MODE)
endif

all: $(TARGET)
	@$(ECHO) COMPILE_PREFIX = $(COMPILE_PREFIX)

# include rules.mk
include $(SDK_ROOT)/tools/make/rules.mk


# vim: syntax=make
