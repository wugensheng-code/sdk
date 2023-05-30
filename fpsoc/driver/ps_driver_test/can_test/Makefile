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
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ -f Makefile_can_fd_intr dasm
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ -f Makefile_can_2_0B_intr dasm
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ -f Makefile_can_2_0B_in_loopback dasm
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ -f Makefile_can_2_0B_ex_loopback dasm
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ -f Makefile_can_fd_listen_only dasm
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ -f Makefile_can_fd_stb_fifo dasm
	make clean
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) PROGRAM=./ -f Makefile_can_fd_dma dasm


clean:
	make clean -f Makefile_clean SOC=demoapu
	make clean -f Makefile_clean SOC=demosoc
	rm -rf *.o

cleanelf:
	rm -rf *.o *.d *.elf *.map *.dasm *.dump *.hex *.verilog *.bin
