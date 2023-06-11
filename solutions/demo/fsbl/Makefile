
CHIP ?= dr1m90

ifeq ($(CHIP),dr1m90)
COMPILE_PREFIX=/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-
TARGET   = 1apu_fsbl
V=1
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
TARGET   = rpu_fsbl
NEWLIB   := nano
endif

SDK_ROOT = ../../..

SRC_DIR  = . simulation

INC_DIR  = . \
           $(SDK_ROOT)/3rdparty/lib/FATFS/lagecy/include \
           $(SDK_ROOT)/3rdparty/lib/FATFS/lagecy/qspi \
           $(SDK_ROOT)/3rdparty/lib/FATFS/lagecy/sd_emmc

## QSPI XIP
FSBL_QSPI_XIP ?= yes

## FSBL Simu flag:
FSBL_SIMU_SKIP_HEADERCHECK ?= no

## compile for al9000 dv simulation
SIMU_AL9000_DV ?=no

ifeq ($(SIMU_AL9000_DV), yes)
CFLAGS   += -DSIMU_AL9000_DV
endif

ifeq ($(FSBL_QSPI_XIP), yes)
CFLAGS   += -DFSBL_QSPI_XIP
endif

ifeq ($(FSBL_SIMU_SKIP_HEADERCHECK), yes)
CFLAGS   += -DFSBL_SIMU_SKIP_HEADERCHECK
endif

CFLAGS   += -O2

include $(SDK_ROOT)/tools/make/rules.mk

# vim: syntax=make
