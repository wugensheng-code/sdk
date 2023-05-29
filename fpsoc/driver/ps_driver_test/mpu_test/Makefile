SOC ?= demosoc
SECURE ?=

ifeq ($(SECURE), 1)
TARGET = fpsoc_mpu_secure_el1
else
TARGET = fpsoc_mpu_nonsecure_el1
endif

SDK_ROOT = ../../../..

SRCDIRS = .

INCDIRS = .

COMMON_FLAGS := -O0 -g3

include $(SDK_ROOT)/tools/make/Makefile.base

ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

ifeq ($(SECURE), 1)
COMMON_FLAGS +=
else
COMMON_FLAGS += -DSUPPORT_NONSECURE

# For APU privilege test, disable by default
#COMMON_FLAGS += -DAPU_MPU_PRIVILEGE_TEST
#COMMON_FLAGS += -DSWITCH_TO_EL0_FROM_EL3
#COMMON_FLAGS += -DENABLE_APU_ABORT_AT_ERET

endif

clean:
	make clean -f Makefile_clean SOC=demoapu
	make clean -f Makefile_clean SOC=demosoc
	rm -rf *.o *.d *.elf *.map *.dasm *.dump *.hex *.verilog
