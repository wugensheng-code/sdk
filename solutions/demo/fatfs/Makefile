TARGET = all

SOC ?= demosoc

PINMUX ?= PINMUX_MODE1

ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

all:
	echo COMPILE_PREFIX = $(COMPILE_PREFIX)

ifeq ($(SOC), demoapu)
	make clean
	make SOC=$(SOC) PINMUX_MODE=$(PINMUX) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_fatfs_demo_read_write
else
endif


clean:
	make clean -f Makefile_clean SOC=demoapu
	make clean -f Makefile_clean SOC=demosoc
	rm -rf *.o

cleanelf:
	rm -rf *.o *.d *.elf *.map *.dasm *.dump *.hex *.verilog
