TARGET = all

SOC ?= demosoc
DOWNLOAD ?= ilm

ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-
CORE = a35
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
CORE = ux600fd
endif

all:
	echo COMPILE_PREFIX = $(COMPILE_PREFIX)

	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ V=1 -f Makefile_single_mode
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ V=1 -f Makefile_reload_mode
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ V=1 -f Makefile_llp_mode
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ V=1 -f Makefile_other_trans_type

clean:
	make clean -f Makefile_clean SOC=demoapu
	make clean -f Makefile_clean SOC=demosoc
	rm -rf *.o

cleanelf:
	rm -rf *.o *.d *.elf *.map *.dasm *.dump *.hex *.verilog
