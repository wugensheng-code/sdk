#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################

#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################

SDK_ROOT    ?= ../../../../..

DOWNLOAD    := ocm

CHIP        ?= dr1m90
ELF         ?= X1_INTR_EXAMPLE
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

BOARD_DIR  ?= $(SDK_ROOT)/solutions/boards/
BOARD_NAME ?= board_dr1x90_ad101_v20

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
ifeq ($(ELF), X1_INTR_EXAMPLE)
C_SRCS += al_axi_qspi_x1_intr_example.c
else ifeq ($(ELF), X1_POLLED_EXAMPLE)
C_SRCS += al_axi_qspi_x1_polled_example.c
else ifeq ($(ELF), X1_3BYTE_INTR_EXAMPLE)
C_SRCS += al_axi_qspi_x1_3byte_unit_intr_example.c
else ifeq ($(ELF), X1_3BYTE_POLLED_EXAMPLE)
C_SRCS += al_axi_qspi_x1_3byte_unit_polled_example.c
else ifeq ($(ELF), X1_4BYTE_INTR_EXAMPLE)
C_SRCS += al_axi_qspi_x1_4byte_unit_intr_example.c
else ifeq ($(ELF), X1_4BYTE_POLLED_EXAMPLE)
C_SRCS += al_axi_qspi_x1_4byte_unit_polled_example.c
else ifeq ($(ELF), X2_3BYTE_POLLED_EXAMPLE)
C_SRCS += al_axi_qspi_x2_3byte_unit_polled_example.c
else ifeq ($(ELF), X4_3BYTE_POLLED_EXAMPLE)
C_SRCS += al_axi_qspi_x4_3byte_unit_polled_example.c
else
C_SRCS += al_axi_qspi_x1_intr_example.c
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

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
# PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v10
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
TARGET := al_axi_qspi_example

ifneq ($(PINMUX_MODE),)
CFLAGS += -DENABLE_$(PINMUX_MODE)=1
TARGET:=$(TARGET)_$(CHIP)_$(ARMv8_STATE)_$(PINMUX_MODE)
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
