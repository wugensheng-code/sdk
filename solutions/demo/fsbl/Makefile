ifeq ($(SOC),demoapu)
COMPILE_PREFIX=/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-
TARGET = 1apu_fsbl
V=1
else
TARGET = rpu_fsbl
endif

FATFS_ENABLE = y

SDK_ROOT = ../../..

SRCDIRS = . simulation

INCDIRS = .

COMMON_FLAGS := -O2 -fno-builtin-printf -fno-builtin-sprintf -fno-builtin-snprintf	#-msave-restore -msmall-data-limit=8


#LDFLAGS += -Xlinker --gc-sections

PROGRAM=./

NEWLIB := nano

include $(SDK_ROOT)/tools/make/Makefile.base

