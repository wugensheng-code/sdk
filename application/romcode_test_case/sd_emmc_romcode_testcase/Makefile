TARGET = all

SOC ?= demosoc

PINMUX_MODE ?= PINMUX_MODE1

ifeq ($(SOC), demoapu)
COMPILE_PREFIX ?= /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

all:
	echo COMPILE_PREFIX = $(COMPILE_PREFIX)

ifeq ($(SOC), demoapu)
ifeq ($(PINMUX_MODE), PINMUX_MODE1)
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_sd_traverse
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_sd_other
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_sd_branch
else
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_emmc_traverse
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_emmc_other
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_emmc_branch
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 -f Makefile_emmc_downloadimage
endif
else
ifeq ($(PINMUX_MODE), PINMUX_MODE1)
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_sd_traverse
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_sd_other
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_sd_branch
else
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_emmc_traverse
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_emmc_other
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_emmc_branch
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=ilm CORE=ux600fd dasm -f Makefile_emmc_downloadimage
endif
endif
	make clean

clean:
	make clean -f Makefile_clean
cleanelf:
	rm -rf *.elf *.map *.dasm *.dump *.hex *.verilog
