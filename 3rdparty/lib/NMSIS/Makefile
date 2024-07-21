#########################################################################
# 2. define SRC_DIR, INC_DIR, LD_LIBS, LIB_DIR
#    LD_LIBS: External library files that the app depends on
#    LIB_DIR: The path where the external library file resides
#    C_SRCS:   c sources for application
#    CXX_SRCS: cpp sources for application
#    ASM_SRCS: asm sources for application
#########################################################################

ifeq ($(AL_CHIP), dr1v90)

SRC_DIR  =
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
            ./NMSIS/DSP/Source/CommonTables/CommonTables.c \
            ./NMSIS/DSP/Source/DistanceFunctions/DistanceFunctions.c \
            ./NMSIS/DSP/Source/InterpolationFunctions/InterpolationFunctions.c \
            ./NMSIS/DSP/Source/StatisticsFunctions/StatisticsFunctions.c \
            ./NMSIS/DSP/Source/TransformFunctions/TransformFunctions.c

CXX_SRCS :=
ASM_SRCS :=

#########################################################################
# 3. CFLAGS
#########################################################################
CFLAGS := -O2 -g -DNUCLEI_DSP_N1 -DNUCLEI_DSP_N2 -DNUCLEI_DSP_N3 -DRISCV_MATH_LOOPUNROLL


# (2) compile and return obj-y
# include $(SDK_ROOT)/tools/make/rules.mk

endif

LIBNAME := NMSIS

include ../../../tools/make/rules.mk


# vim: syntax=make
