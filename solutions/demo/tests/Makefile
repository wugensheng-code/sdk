#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################


#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
SDK_ROOT   ?= ../../..
RTOS = freertos

IP        	?= MMC0
ifeq ($(IP), MMC0)
CFLAGS += -DDEVICE_ID=0
else ifeq ($(IP), MMC1)
CFLAGS += -DDEVICE_ID=1
else
CFLAGS += -DDEVICE_ID=0
endif


CHIP       ?= dr1m90
ifeq ($(CHIP), dr1m90)
COMPILE_PREFIX ?= /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
LINKER_SCRIPT = ./gcc_dr1m90_ddr_test.ld
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

ifeq ($(RTOS),freertos)
AL_CFLAGS += -DUSE_RTOS
AL_CFLAGS += -DRTOS_FREERTOS
CFLAGS += -DUSE_RTOS
CFLAGS += -DRTOS_FREERTOS
else ifeq ($(RTOS), rtthread)
AL_CFLAGS += -DUSE_RTOS
AL_CFLAGS += -DRTOS_RTTHREAD
CFLAGS += -DUSE_RTOS
CFLAGS += -DRTOS_RTTHREAD
endif


#########################################################################
# 2. define SRC_DIR, INC_DIR, LD_LIBS, LIB_DIR
#    LD_LIBS: External library files that the app depends on
#    LIB_DIR: The path where the external library file resides
#    C_SRCS:   c sources for application
#    CXX_SRCS: cpp sources for application
#    ASM_SRCS: asm sources for application
#########################################################################
SRC_DIR  = ./components/cmocka/src
SRC_DIR += ./
ifeq ($(RTOS), freertos)
	SRC_DIR += ./components/shell/dr1x90
	SRC_DIR += ./components/shell/src
	SRC_DIR += ./components
	SRC_DIR += ./components/freertos
endif
INC_DIR += ./components/cmocka/include ./
ifeq ($(RTOS), freertos)
	INC_DIR += ./components/shell/dr1x90
	INC_DIR += ./components/shell/src
	INC_DIR += ./components
	INC_DIR += ./tc
endif


INC_DIR +=	./components/POSIX/FreeRTOS-Plus-POSIX/FreeRTOS-Plus-POSIX/include \
			./components/POSIX/include \
			./components/POSIX/FreeRTOS-Plus-POSIX/include \
			./components/POSIX/include/private \
			./components/POSIX/FreeRTOS-Plus-POSIX/include/portable \
			./components/POSIX/FreeRTOS-Plus-POSIX/include/portable/dr1x90


ifeq ($(CHIP), dr1m90)
INC_DIR += ./components/POSIX/FreeRTOS-Plus-POSIX/include/portable/dr1m90
endif

SRC_DIR += ./components/POSIX/FreeRTOS-Plus-POSIX/source

LD_LIBS  =
LIB_DIR  =

C_SRCS   += ./tc/uart_tc.c
C_SRCS   += ./tc/iic_tc.c
C_SRCS   += ./tc/mmc_tc.c
C_SRCS   += ./tc/ipc_tc.c
C_SRCS   += ./tc/xmon_tc.c
C_SRCS   += ./tc/qspi_tc.c
C_SRCS   += ./tc/can_tc.c
C_SRCS   += ./tc/dma_ahb_tc.c
C_SRCS   += ./tc/fatfs_tc.c
C_SRCS   += ./tc/tc_tc.c
C_SRCS   += ./tc/wdt_tc.c
C_SRCS   += ./tc/helloworld_tc.c
C_SRCS   += ./tc/gpio_tc.c
C_SRCS   += ./tc/mpu_tc.c
# C_SRCS   += ./tc/netio_tc.c
C_SRCS   += $(SDK_ROOT)/3rdparty/lwip-2.1.3/example/freertos_lwip_netio_example/netio.c
CXX_SRCS :=
ASM_SRCS :=

#########################################################################
# 3. CFLAGS
#########################################################################
CFLAGS := -O0 -g3
CFLAGS += -DCMAKE_BUILD_TYPE=Debug -DHAVE_CONFIG_H
CFLAGS += -DSHELL_CFG_USER
CFLAGS += -DconfigTOTAL_HEAP_SIZE=11*1024*1024
CFLAGS += -DconfigUSE_POSIX_ERRNO=1

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_emulation
PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v10

#########################################################################
# put these option to board_dr1x90_emulation Makefile
## Available choices:
## PINMUX_MODE1, PINMUX_MODE2, PINMUX_MODE3, PINMUX_MODE_SD1
#########################################################################
PINMUX_MODE ?= PINMUX_MODE1


#########################################################################
# 5. define build target
#########################################################################
TARGET := tests

ifneq ($(PINMUX_MODE),)
CFLAGS += -DENABLE_$(PINMUX_MODE)=1
TARGET:=$(TARGET)_$(CHIP)_$(PINMUX_MODE)
endif

all: $(TARGET)
	@$(ECHO) COMPILE_PREFIX = $(COMPILE_PREFIX)

# include rules.mk
include $(SDK_ROOT)/tools/make/rules.mk

# vim: syntax=make
