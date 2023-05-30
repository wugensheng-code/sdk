TARGET = all

SOC ?= demosoc

ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif
#COMMON_FLAGS += -DSPI_DEBUG

all:
	echo COMPILE_PREFIX = $(COMPILE_PREFIX)

ifeq ($(SOC), demoapu)
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ V=1 -f Makefile_smc_apu_el1
else
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ V=1 -f Makefile_smc_rpu_el1
endif


clean:
	make clean -f Makefile_clean SOC=demoapu
	make clean -f Makefile_clean SOC=demosoc

cleanelf:
	rm -rf *.o *.d *.elf *.map *.dasm *.dump *.hex *.verilog
