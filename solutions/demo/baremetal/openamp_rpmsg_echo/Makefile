#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################


#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
SDK_ROOT   ?= ../../../..

CHIP       ?= dr1m90
ifeq ($(CHIP), dr1m90)
COMPILE_PREFIX ?= /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

ENABLE_MMU := 1

#ARMv8_STATE=64 
#ARMv8_SECURE=SECURE 
ARMv8_CORE=SLAVE
ARMv8_EL=EL1

#########################################################################
# 2. define SRC_DIR, INC_DIR, LD_LIBS, LIB_DIR
#    LD_LIBS: External library files that the app depends on
#    LIB_DIR: The path where the external library file resides
#    C_SRCS:   c sources for application
#    CXX_SRCS: cpp sources for application
#    ASM_SRCS: asm sources for application
#########################################################################
SRC_DIR  = ./
INC_DIR += ./src/system/generic \
		   ./src/machine/dr1m90
#libmetal
INC_DIR += ../../../../3rdparty/lib/libmetal/include/lib/include

#open-amp
INC_DIR += ../../../../3rdparty/lib/open-amp/lib/include \
			../../../../3rdparty/lib/open-amp/lib/rpmsg \
			../../../../3rdparty/lib/open-amp/include/include/generated

#raw
INC_DIR += ./ ${SDK_ROOT}/fpsoc/arch/arm/armv8/aarch64/cortex-a/inc \
		    ${SDK_ROOT}/fpsoc/arch/common/inc \
			${SDK_ROOT}/fpsoc/arch/arm/common/gic_v3/inc \
			${SDK_ROOT}/fpsoc/arch/arm/common/sys_timer/inc \
			${SDK_ROOT}/fpsoc/inc/


LD_LIBS  =
LIB_DIR  =

C_SRCS   :=./src/system/generic/machine/dr1m90/helper.c \
			./src/system/generic/rpmsg-echo.c \
			./src/machine/dr1m90/dr1m90_rproc.c \
			./src/machine/dr1m90/platform_info.c \
			./src/machine/dr1m90/rsc_table.c 

CXX_SRCS :=
ASM_SRCS :=

#########################################################################
# 3. CFLAGS
#########################################################################
CFLAGS := -O0 -g -Wall -DLOG_DEV=AL_LOG_UART0 -DRPMSG_DEBUG=1 -DBOARD_SEL=1 -DOPENAMP_EXAMPLE_APP

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
#PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_emulation
PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v10

LINKER_SCRIPT := ./src/system/generic/machine/dr1m90/gcc_dr1m90_ddr_board_dr1x90_ad101_v10.ld

#########################################################################
# put these option to board_dr1x90_emulation Makefile
## Available choices:
## PINMUX_MODE1, PINMUX_MODE2, PINMUX_MODE3, PINMUX_MODE_SD1
#########################################################################
PINMUX_MODE ?= PINMUX_MODE1


#########################################################################
# 5. define build target
#########################################################################
TARGET := rpmsg-echo

ifneq ($(PINMUX_MODE),)
CFLAGS += -DENABLE_$(PINMUX_MODE)=1
TARGET:=$(TARGET)_$(CHIP)_$(PINMUX_MODE)
endif

all: $(TARGET)
	@$(ECHO) COMPILE_PREFIX = $(COMPILE_PREFIX)

# include rules.mk
include $(SDK_ROOT)/tools/make/rules.mk

# vim: syntax=make
