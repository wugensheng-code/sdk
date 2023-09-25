
#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
SDK_ROOT   ?= ../../../../../../

CHIP       = dr1v90
RTOS       := rtthread

ifeq ($(CHIP), dr1m90)
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
SRC_DIR  = ./
INC_DIR += ./ \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/common \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/port/dwc2 \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/core \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/class/cdc \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/class/hub \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/class/hid \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/class/msc \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/class/video \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/class/audio \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/config \
			$(SDK_ROOT)/3rdparty/lib/CherryUSB/osal

LD_LIBS  =
LIB_DIR  =

C_SRCS   :=
CXX_SRCS :=
ASM_SRCS :=

#########################################################################
# 3. CFLAGS
#########################################################################
CFLAGS := -O2 -g
CFLAGS += -DOS_RTTHREAD -DUSB_HOST

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
PLAT_DIR  = $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v10

#########################################################################
# put these option to board_dr1x90_emulation Makefile
## Available choices:
## PINMUX_MODE1, PINMUX_MODE2, PINMUX_MODE3, PINMUX_MODE_SD1
#########################################################################
PINMUX_MODE ?= PINMUX_MODE1


#########################################################################
# 5. define build target
#########################################################################
TARGET := msc_host_rtthread
DOWNLOAD = ocm

ifneq ($(PINMUX_MODE),)
CFLAGS += -DENABLE_$(PINMUX_MODE)=1
TARGET:=$(TARGET)_$(CHIP)_$(PINMUX_MODE)
endif

all: $(TARGET)
	@$(ECHO) COMPILE_PREFIX = $(COMPILE_PREFIX)

# include rules.mk
include $(SDK_ROOT)/tools/make/rules.mk

# vim: syntax=make
