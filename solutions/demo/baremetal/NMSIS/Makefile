
#########################################################################
# 1. define SDK_ROOT, BOARD info, CHIP, COMPILE_PREFIX
#########################################################################
SDK_ROOT   ?= ../../../..

CHIP       ?= dr1v90

ifeq ($(CHIP), dr1m90)
COMPILE_PREFIX ?= /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-
else
COMPILE_PREFIX ?= /opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf-
endif

DOWNLOAD        ?= ddr

#########################################################################
# 2. define SRC_DIR, INC_DIR, LD_LIBS, LIB_DIR
#    LD_LIBS: External library files that the app depends on
#    LIB_DIR: The path where the external library file resides
#    C_SRCS:   c sources for application
#    CXX_SRCS: cpp sources for application
#    ASM_SRCS: asm sources for application
#########################################################################
SRC_DIR  = ./
INC_DIR += ./NMSIS/Core/Include \
		   ./NMSIS/DSP/Include \
		   ./NMSIS/DSP/PrivateInclude

LD_LIBS  =
LIB_DIR  =

C_SRCS   := ./NMSIS/DSP/Source/BasicMathFunctions/BasicMathFunctions.c \
            ./NMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.c \
            ./NMSIS/DSP/Source/FastMathFunctions/FastMathFunctions.c \
            ./NMSIS/DSP/Source/MatrixFunctions/MatrixFunctions.c  \
            ./NMSIS/DSP/Source/SupportFunctions/SupportFunctions.c  \
            ./NMSIS/DSP/Source/WindowFunctions/WindowFunctions.c \
            ./NMSIS/DSP/Source/BayesFunctions/BayesFunctions.c   \
            ./NMSIS/DSP/Source/ControllerFunctions/ControllerFunctions.c   \
            ./NMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.c   \
            ./NMSIS/DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.c \
            ./NMSIS/DSP/Source/SVMFunctions/SVMFunctions.c \
            ./NMSIS/DSP/Source/CommonTables/CommonTables.c    \
            ./NMSIS/DSP/Source/DistanceFunctions/DistanceFunctions.c   \
            ./NMSIS/DSP/Source/InterpolationFunctions/InterpolationFunctions.c  \
            ./NMSIS/DSP/Source/StatisticsFunctions/StatisticsFunctions.c    \
            ./NMSIS/DSP/Source/TransformFunctions/TransformFunctions.c \
            ./NMSIS/DSP/Examples/RISCV/riscv_convolution_example/math_helper.c \
            ./NMSIS/DSP/Examples/RISCV/riscv_convolution_example/riscv_convolution_example_f32.c
			
CXX_SRCS :=
ASM_SRCS :=

#########################################################################
# 3. CFLAGS
#########################################################################
CFLAGS := -O0 -g -DNUCLEI_DSP_N1 -DNUCLEI_DSP_N2 -DNUCLEI_DSP_N3 -DRISCV_MATH_LOOPUNROLL

#########################################################################
# 4. PINMUX_MODE: sdk internal define, pinmux setting need to remove;
#    Fixme: where to add board init source files and headers files
#########################################################################
PLAT_DIR  = $(SDK_ROOT)/solutions/boards/board_dr1x90_emulation

#########################################################################
# put these option to board_dr1x90_emulation Makefile
## Available choices:
## PINMUX_MODE1, PINMUX_MODE2, PINMUX_MODE3, PINMUX_MODE_SD1
#########################################################################
PINMUX_MODE ?= PINMUX_MODE1


#########################################################################
# 5. define build target
#########################################################################
TARGET := riscv_convolution_example_f32

ifneq ($(PINMUX_MODE),)
CFLAGS += -DENABLE_$(PINMUX_MODE)=1
TARGET:=$(TARGET)_$(CHIP)_$(PINMUX_MODE)
endif

all: $(TARGET)
	@$(ECHO) COMPILE_PREFIX = $(COMPILE_PREFIX)

# include rules.mk
include $(SDK_ROOT)/tools/make/rules.mk

# vim: syntax=make
