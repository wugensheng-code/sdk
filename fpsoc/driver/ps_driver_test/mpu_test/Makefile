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

CFLAGS := -O0 -g3

include $(SDK_ROOT)/tools/make/Makefile.base

ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

ifeq ($(SECURE), 1)
CFLAGS +=
else
CFLAGS += -DSUPPORT_NONSECURE

# For APU privilege test, disable by default
#CFLAGS += -DAPU_MPU_PRIVILEGE_TEST
#CFLAGS += -DSWITCH_TO_EL0_FROM_EL3
#CFLAGS += -DENABLE_APU_ABORT_AT_ERET

endif

clean:
	make clean -f Makefile_clean SOC=demoapu
	make clean -f Makefile_clean SOC=demosoc
	rm -rf *.o *.d *.elf *.map *.dasm *.dump *.hex *.verilog
