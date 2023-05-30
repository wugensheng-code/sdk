TARGET = npu_runtime_test

SOC ?= demoapu

# enable lib npu runtime, and runtime depends on FATFS & LAGECY driver
NPU_ENABLE = y

FATFS_ENABLE = y

# link using ddr address
DOWNLOAD = ddr

SDK_ROOT = ../../../..

SRCDIRS = .

INCDIRS = . $(SDK_ROOT)/fpsoc/arch/arm/armv8/aarch64/cortex-a/inc

COMMON_FLAGS := -O2


ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif


include $(SDK_ROOT)/tools/make/Makefile.base
