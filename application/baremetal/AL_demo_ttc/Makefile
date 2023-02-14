TARGET = all

SOC ?= demosoc

ifeq ($(SOC), demoapu)
COMPILE_PREFIX := /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX := /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

all:
	echo COMPILE_PREFIX = $(COMPILE_PREFIX)
	make clean

ifeq ($(SOC), demoapu)
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_Iv_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_M1_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_M2_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_M3_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_Ov_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_Ev_intr
else
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_Iv_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_M1_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_M2_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_M3_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_Ov_intr
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_Ev_intr
endif
	make clean

clean:
	make clean -f Makefile_clean
cleanelf:
	rm -rf *.elf *.map *.dasm *.dump *.hex *.verilog
