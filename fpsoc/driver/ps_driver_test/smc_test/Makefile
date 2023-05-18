TARGET = all

SOC ?= demosoc

ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif
#COMMON_FLAGS += -DSPI_DEBUG

all:
	echo COMPILE_PREFIX = $(COMPILE_PREFIX)

ifeq ($(SOC), demoapu)

	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_smc_el1

else
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_rpu
endif


clean:
	make clean -f Makefile_clean SOC=demoapu
	make clean -f Makefile_clean SOC=demosoc

cleanelf:
	rm -rf *.o *.d *.elf *.map *.dasm *.dump *.hex *.verilog
