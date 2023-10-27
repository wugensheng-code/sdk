
# Set compilation tool chain
SDK_ROOT   ?= ../../../..
CUR_DIR		?= ./

CHIP       ?= dr1m90

ifeq ($(CHIP), dr1m90)
COMPILE_PREFIX ?= /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif


# BOARD_DIR  ?= $(SDK_ROOT)/solutions/boards/
# BOARD_NAME ?= board_dr1x90_ad101_v10
DOWNLOAD ?= ddr

SRC_DIR  = ./
INC_DIR += ./

LD_LIBS     :=
LIB_DIR     :=

CXX_SRCS    :=
ASM_SRCS    :=

CFLAGS := -O0 -g

PLAT_DIR  ?= $(SDK_ROOT)/solutions/boards/board_dr1x90_ad101_v10

PINMUX_MODE ?= PINMUX_MODE1

ifneq ($(PINMUX_MODE),)
CFLAGS += -DENABLE_$(PINMUX_MODE)=1
TARGET:=$(TARGET)_$(CHIP)_$(PINMUX_MODE)
endif


TARGET := dry

all: $(TARGET)
	@$(ECHO) COMPILE_PREFIX = $(COMPILE_PREFIX)

include $(SDK_ROOT)/tools/make/rules.mk

# .PHONY: run clean

# run: dry2 dry2nr dry2o
# 	./dry2 ${1-50000} 2>/dev/null
# 	./dry2nr ${1-50000} 2>/dev/null
# 	./dry2o ${1-50000} 2>/dev/null

# dry2: dry.h dry1.c dry2.c
# 	${CC} -c ${CFLAGS} dry1.c -o dry1.o
# 	${CC} ${CFLAGS} dry2.c dry1.o ${LFLAGS} -o dry2 

# dry2nr: dry.h dry1.c dry2.c
# 	${CC} -c -DREG ${CFLAGS} dry1.c -o dry1.o
# 	${CC} -DREG ${CFLAGS} dry2.c dry1.o ${LFLAGS} -o dry2nr

# dry2o: dry.h dry1.c dry2.c
# 	${CC} -c -O ${CFLAGS} dry1.c -o dry1.o
# 	${CC} -O ${CFLAGS} dry2.c dry1.o ${LFLAGS} -o dry2o

# clean:
# 	rm -f dry1.o dry2 dry2nr dry2o



