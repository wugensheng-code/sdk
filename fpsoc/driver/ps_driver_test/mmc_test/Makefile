#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
CHIP ?= dr1v90
SDK_ROOT   ?= ../../../..

ifeq ($(CHIP), dr1m90)
COMPILE_PREFIX ?= /opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

BOARD_DIR  ?= $(SDK_ROOT)/solutions/boards/
BOARD_NAME ?= board_dr1x90_emulation

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
SRC_DIR  = ./
INC_DIR  += ./

LD_LIBS  :=
LIB_DIR  :=

C_SRCS   :=
CXX_SRCS :=
ASM_SRCS :=

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
ifneq ($(BOARD_NAME),)
SRC_DIR  += $(BOARD_DIR)/$(BOARD_NAME)/src
INC_DIR  += $(BOARD_DIR)/$(BOARD_NAME)/inc
endif


#########################################################################
# put these option to board_dr1x90_emulation Makefile
## Available choices:
## PINMUX_MODE1, PINMUX_MODE2, PINMUX_MODE3, PINMUX_MODE_SD1
#########################################################################.
PINMUX_MODE ?= PINMUX_MODE1

export SOC
export SDK_ROOT
export CHIP
export BOARD_DIR
export BOARD_NAME
export SRC_DIR
export INC_DIR

export LD_LIBS
export LIB_DIR

export CXX_SRCS
export ASM_SRCS

export PINMUX_MODE
export COMPILE_PREFIX


all:
	echo COMPILE_PREFIX = $(COMPILE_PREFIX)
	make cleanapp
	make -f Makefile_sd dasm
	make cleanapp
	make -f Makefile_emmc dasm

cleanapp:
	$(RM) $(foreach subdir, $(SRC_DIR), $(wildcard $(subdir)/*.d $(subdir)/*.o))

clean:
	make clean -f Makefile_clean CHIP=dr1m90
	make clean -f Makefile_clean CHIP=dr1v90

cleanelf:
	rm -rf *.bin *.dasm *.dump *.elf *.hex *.map *.verilog

