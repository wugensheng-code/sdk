TARGET = all

SOC ?= demosoc
#apu toolchain使用这个
#COMPILE_PREFIX ?= /opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
#rpu 不同人的toolchain在不同位置这里需要更改。
COMPILE_PREFIX ?= D:\temp\ae_src\soft_tool\AnlogicStudio\toolchain\fpsoc\riscv-nuclei-elf\bin\riscv-nuclei-elf-

all:
	rm -rf *.o *.d *.map *.verilog
	echo COMPILE_PREFIX = $(COMPILE_PREFIX) .................
	#make SOC=demoapu COMPILE_PREFIX=/opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf- PROGRAM=./ V=1 all -f Makefile_1
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_1
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_2
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_3
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_4
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_5
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_6
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_7
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_8
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_9
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_10
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_11
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_12
	
	rm -rf *.o *.d *.map *.verilog
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ DOWNLOAD=$(DOWNLOAD) CORE=$(CORE) all -f Makefile_13
	
	
	

clean:
	make SOC=$(SOC) COMPILE_PREFIX=$(COMPILE_PREFIX) PROGRAM=./ V=1 clean -f Makefile_clean