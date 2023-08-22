
#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
SDK_ROOT   ?= ../../..

CHIP       ?= dr1v90

ifeq ($(CHIP), dr1m90)
ARMv8_EL   := EL3
COMPILE_PREFIX ?= /opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-
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
SRC_DIR  = ./ \
           ./ddr/
INC_DIR  = ./ \
           ./ddr/ \
           ./ddr/al9000_ddrc_regs


LD_LIBS  =
LIB_DIR  =

C_SRCS   :=
CXX_SRCS :=
ASM_SRCS :=

#########################################################################
# 3. CFLAGS
#########################################################################
CFLAGS := -O2 -g
CFLAGS += -DAL9000_CLK_CONFIG
CFLAGS += -DAL9000_OSC_33
CFLAGS += -DAL9000_BRINGUP
CFLAGS += -DBOARD_DR1X90_AD101_V10

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
# PLAT_DIR  = $(SDK_ROOT)/solutions/boards/board_dr1x90_emulation
PLAT_DIR  = $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v10

#########################################################################
# put these option to board_dr1x90_emulation Makefile
## Available choices:
## PINMUX_MODE1, PINMUX_MODE2, PINMUX_MODE3, PINMUX_MODE_SD1
#########################################################################
PINMUX_MODE ?= PINMUX_MODE1

#########################################################################
# 5. fsbl internal Macro
#########################################################################

## QSPI XIP
FSBL_QSPI_XIP              ?= no

## FSBL Simu flag:
FSBL_SIMU_SKIP_HEADERCHECK ?= no

## compile for al9000 dv simulation
SIMU_AL9000_DV             ?= no

ifeq ($(SIMU_AL9000_DV), yes)
CFLAGS  += -DSIMU_AL9000_DV
SRC_DIR  += ./simulation/
endif

ifeq ($(FSBL_QSPI_XIP), yes)
CFLAGS  += -DFSBL_QSPI_XIP
endif

ifeq ($(FSBL_SIMU_SKIP_HEADERCHECK), yes)
CFLAGS  += -DFSBL_SIMU_SKIP_HEADERCHECK
endif


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
