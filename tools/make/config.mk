#########################################################################
## Available choices:
## The name of sub directories in $(AL_SDK_ROOT)/fpsoc/arch
CHIP            ?= dr1m90

#########################################################################
## Available choices:
## ARMv8_STATE: 32,  64
## ARMv8_EL:    EL1, EL3
## ARMv8_CORE:  0, 1
ARMv8_STATE     ?= 64
ARMv8_EL        ?= EL1
ARMv8_CORE		?= 0

#########################################################################

#########################################################################
# options for enable mmu, available choices: 0/1
#########################################################################
ENABLE_MMU  ?= 0
DDR_2M_MAPPING ?= 1
CODE_READONLY ?= 1

#########################################################################
# options for compile command
#########################################################################
COMPILE_PREFIX  ?=

## Available choices:
## ocm: Program will be download into ocm and run directly in ocm
## ddr: Program will be download into ddr and run directly in ddr
DOWNLOAD        ?= ocm

## If V=1, it will display compiling message in verbose including compiling options
V ?= 1

## If SILENT=1, it will not display any compiling messsage
SILENT ?= 1

# Variables should be defined in Application Makefile
## Available choices:
## The name of sub directories in $(AL_SDK_ROOT)/OS/
RTOS ?=

## If PFLOAT=1, it will enable float point print when using nano newlib
PFLOAT ?= 0

## If NEWLIB=nano, it will use nano newlib, otherwise it will use normal newlib
NEWLIB ?= nano

## If NOGC=1, it will not gc any sections during compiling to save code size
NOGC ?= 0


LIB_OUTPUT_DIR  ?= $(AL_SDK_ROOT)/output

AL_SDK_ROOT := $(abspath $(AL_SDK_ROOT))

ifeq ($(PLAT_DIR),)
PLAT_DIR := $(patsubst %/platform_init.h, %, $(wildcard $(AL_SDK_ROOT)/*/platform_init.h))
endif

PLAT_DIR := $(abspath $(PLAT_DIR))

BSP_DIR  ?= $(patsubst %/Makefile, %, $(wildcard $(AL_SDK_ROOT)/*/Makefile))

#########################################################################

export CHIP
export ARMv8_STATE
export ARMv8_EL
export ARMv8_CORE
export LIB_OUTPUT_DIR
export COMPILE_PREFIX
export V
export SILENT
export RTOS
export AL_SDK_ROOT
export SDK_ROOT
export CFLAGS
export BSP_DIR
export PLAT_DIR
export ENABLE_MMU
export DDR_2M_MAPPING
export CODE_READONLY

# vim: syntax=make
