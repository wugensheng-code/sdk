#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################

#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################

SDK_ROOT    ?= ../../../../..


CHIP        ?= dr1m90
ELF         ?= IN_LOOPBACK

ifeq ($(CHIP), dr1m90)
COMPILE_PREFIX ?= /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

BOARD_DIR  ?= $(SDK_ROOT)/solutions/boards/
BOARD_NAME ?= board_dr1x90_ad101_v10

#########################################################################
# 2. define SRC_DIR, INC_DIR, LD_LIBS, LIB_DIR
#    SRC_DIR: application source path
#    INC_DIR  application header path
#    LD_LIBS: External library files that the app depends on
#    LIB_DIR: The path where the external library file resides
#    C_SRCS:   c sources for application
#    CXX_SRCS: cpp sources for application
#    ASM_SRCS: asm sources for application
#########################################################################
ifeq ($(ELF), EX_LOOPBACK)
C_SRCS += al_can_ex_loopback_blocked.c
else ifeq ($(ELF), FD_DMA)
C_SRCS += al_can_fd_dma_nonblock.c
else ifeq ($(ELF), FD_DMA_LLP)
C_SRCS += al_can_fd_dma_llp_nonblock.c
else ifeq ($(ELF), FD_STB_FIFO)
C_SRCS += al_can_fd_stb_fifo_blocked.c
else ifeq ($(ELF), FD_STB_PRIO)
C_SRCS += al_can_fd_stb_priority_blocked.c
else ifeq ($(ELF), STD_PTB)
C_SRCS += al_can_std_ptb_blocked.c
else
C_SRCS += al_can_in_loopback_blocked.c
endif

INC_DIR     += ./

LD_LIBS     :=
LIB_DIR     :=

CXX_SRCS    :=
ASM_SRCS    :=

#########################################################################
# 3. CFLAGS
#########################################################################
CFLAGS := -O2 -g
CFLAGS += -DCAN_DEBUG
# CFLAGS += -DUSE_RTOS
# CFLAGS += -DRTOS_RTTHREAD

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
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
ifeq ($(ELF), EX_LOOPBACK)
TARGET := fpsoc_can_example_ex_loopback_block
else ifeq ($(ELF), FD_DMA)
TARGET := fpsoc_can_example_fd_dma_nonblock
else ifeq ($(ELF), FD_DMA_LLP)
TARGET := fpsoc_can_example_fd_dma_llp_nonblock
else ifeq ($(ELF), FD_STB_FIFO)
TARGET := fpsoc_can_example_fd_stb_fifo_block
else ifeq ($(ELF), FD_STB_PRIO)
TARGET := fpsoc_can_example_fd_stb_priority_block
else ifeq ($(ELF), STD_PTB)
TARGET := fpsoc_can_example_std_ptb_block
else
TARGET := fpsoc_can_example_in_loopback_block
endif

ifneq ($(PINMUX_MODE),)
CFLAGS += -DENABLE_$(PINMUX_MODE)=1
TARGET:=$(TARGET)_$(CHIP)_$(PINMUX_MODE)
endif

all: $(TARGET)
	@$(ECHO) COMPILE_PREFIX = $(COMPILE_PREFIX)

cleanapp:
	$(RM) $(foreach subdir, $(SRC_DIR), $(wildcard $(subdir)/*.d $(subdir)/*.o))

clean:
	make clean CHIP=dr1m90
	make clean CHIP=dr1v90

cleanelf:
	rm -rf *.bin *.dasm *.dump *.elf *.hex *.map *.verilog

include $(SDK_ROOT)/tools/make/rules.mk
