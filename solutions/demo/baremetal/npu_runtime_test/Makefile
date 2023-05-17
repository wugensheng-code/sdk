TARGET = npu_runtime_test

SOC ?= demoapu

# enable lib npu runtime
NPU_ENABLE = y

# link using ddr address
DOWNLOAD = ddr

NUCLEI_SDK_ROOT = ../../../..

SRCDIRS = .

INCDIRS = .

COMMON_FLAGS := -O2


ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif


include $(NUCLEI_SDK_ROOT)/tools/make/Makefile.base
