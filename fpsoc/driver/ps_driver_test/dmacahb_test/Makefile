TARGET = all

SOC ?= demosoc

ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

all:
	echo COMPILE_PREFIX = $(COMPILE_PREFIX)

ifeq ($(SOC), demoapu)
	make clean
	rm -rf *.o
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ -f Makefile_single_mode
	make clean
	rm -rf *.o
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_auto_reload_mode
	make clean
	rm -rf *.o
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_llp_mode
else
endif


clean:
	make clean -f Makefile_clean SOC=demoapu
	make clean -f Makefile_clean SOC=demosoc

cleanelf:
	rm -rf *.o *.d *.elf *.map *.dasm *.dump *.hex *.verilog
