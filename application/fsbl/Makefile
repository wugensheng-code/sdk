ifeq ($(SOC),demoapu)
COMPILE_PREFIX=/opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
TARGET = 1apu_fsbl
V=1
else
TARGET = rpu_fsbl
endif

#apu compile: make SOC=demoapu all
#rpu compile: make COMPILE_PREFIX=/opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf- CORE=ux600fd dasm

NUCLEI_SDK_ROOT = ../..

SRCDIRS = . ./driver/qspi/ driver/sd_emmc/ driver/sd_emmc/FATFS/

INCDIRS = . ./driver/qspi/ driver/sd_emmc/ driver/sd_emmc/FATFS/

COMMON_FLAGS := -O2 -fno-builtin-printf -fno-builtin-sprintf -fno-builtin-snprintf	#-msave-restore -msmall-data-limit=8

#LDFLAGS += -Xlinker --gc-sections

PROGRAM=./

NEWLIB := nano

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base

