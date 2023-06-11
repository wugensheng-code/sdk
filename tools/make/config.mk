#########################################################################
## Available choices:
## The name of sub directories in $(SDK_ROOT)/fpsoc/arch
CHIP            ?= dr1m90

#########################################################################
## Available choices:
## ARMv8_STATE: 32,  64
## ARMv8_EL:    EL1, EL3
ARMv8_STATE     ?= 64
ARMv8_EL        ?= EL1

#########################################################################
## Available choices:
## The name of sub directories in $(SDK_ROOT)/solutions/boards
BOARD_DIR  ?= $(SDK_ROOT)/solutions/boards/
BOARD_NAME ?=

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
## The name of sub directories in $(SDK_ROOT)/OS/
RTOS ?=

## If PFLOAT=1, it will enable float point print when using nano newlib
PFLOAT ?= 0

## If NEWLIB=nano, it will use nano newlib, otherwise it will use normal newlib
NEWLIB ?= nano

## If NOGC=1, it will not gc any sections during compiling to save code size
NOGC ?= 0


LIB_OUTPUT_DIR  ?= $(SDK_ROOT)/output

SDK_ROOT := $(abspath $(SDK_ROOT))
#########################################################################

export CHIP
export ARMv8_STATE
export ARMv8_EL
export LIB_OUTPUT_DIR
export COMPILE_PREFIX
export V
export SILENT
export RTOS
export SDK_ROOT
export CFLAGS

# vim: syntax=make
