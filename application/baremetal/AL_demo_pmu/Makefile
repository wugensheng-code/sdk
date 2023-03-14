TARGET ?= pmu_demo_test

ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
SRCDIRS = . ./apu
INCDIRS = . ./apu
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
SRCDIRS = .
INCDIRS = .
endif

NUCLEI_SDK_ROOT = ../../..



COMMON_FLAGS := -O0 -Wall 

ifeq ($(TARGET), pmu_ocm_ecc_int_test)
COMMON_FLAGS += -D_OCM_ECC_INT_TEST
else ifeq ($(TARGET), pmu_apu_normal_init_test)
COMMON_FLAGS += -D_APU_NORMAL_INT_TEST
else ifeq ($(TARGET), pmu_rpu_normal_init_test)
COMMON_FLAGS += -D_RPU_NORMAL_INT_TEST
else ifeq ($(TARGET), pmu_apu_fatal_bus_timeout_test)
COMMON_FLAGS += -D_APU_FATAL_BUS_TIMEOUT_TEST
else ifeq ($(TARGET), pmu_apu_fatal_clk_detect_test)
COMMON_FLAGS += -D_APU_FATAL_CLK_DETECT_TEST
else ifeq ($(TARGET), pmu_apu_fatal_wdt0_test)
COMMON_FLAGS += -D_APU_FATAL_WDT0_TEST
else ifeq ($(TARGET), pmu_apu_fatal_wdt1_test)
COMMON_FLAGS += -D_APU_FATAL_WDT1_TEST
else ifeq ($(TARGET), pmu_apu_fatal_wdt2_test)
COMMON_FLAGS += -D_APU_FATAL_WDT2_TEST
else ifeq ($(TARGET), pmu_apu_fatal_apu_ecc_test)
COMMON_FLAGS += -D_APU_FATAL_APU_ECC_TEST
else ifeq ($(TARGET), pmu_apu_fatal_ocm_ecc_test)
COMMON_FLAGS += -D_APU_FATAL_OCM_ECC_TEST
else ifeq ($(TARGET), pmu_rpu_fatal_multibit_ecc_test)
COMMON_FLAGS += -D_RPU_FATAL_RPU_MULTIBIT_ECC_TEST
else ifeq ($(TARGET), pmu_rpu_nmi_ocm_ecc_test)
COMMON_FLAGS += -D_RPU_NMI_OCM_ECC_TEST
else
endif

#SOC := demoapu

#COMPILE_PREFIX := /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-

PROGRAM := ./

#V := 1

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base

cleanelf:
	rm -rf *.elf *.map *.dasm *.dump *.hex *.verilog
