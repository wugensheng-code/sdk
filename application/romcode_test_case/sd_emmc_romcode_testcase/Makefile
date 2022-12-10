TARGET = 1romcode_test_apu_sd

NUCLEI_SDK_ROOT = ../../..

SRCDIRS = . FATFS

INCDIRS = . FATFS

COMMON_FLAGS := -O2 -Wall -DAL_DEBUG_PRINT -D_USE_WRITE=1 -D_FS_READONLY=0 -D_USE_MKFS=1
#-Werror=uninitialized

#SOC := demoapu

#COMPILE_PREFIX := /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-

#PROGRAM := ./

#V := 1

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
