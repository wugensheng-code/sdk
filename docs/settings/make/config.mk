#########################################################################
AL_CHIP         ?= $(CHIP)

#########################################################################
## Available choices:
## ARMv8_STATE:     32,  64
## ARMv8_EL:        EL1, EL3
## ARMv8_SECURE:    SECURE, NONSECURE
## ARMv8_CORE:      MASTER, SLAVE

ARMv8_STATE     = #{{ARMv8_STATE}}
ARMv8_EL        = #{{ARMv8_EL}}
ARMv8_SECURE    = #{{ARMv8_SECURE}}
ARMv8_CORE      = #{{ARMv8_CORE}}

#########################################################################

#########################################################################
# options for enable mmu, available choices: 0/1
#########################################################################
ifeq ($(AL_CHIP),dr1v90)
ENABLE_MMU  ?= 0
else
ENABLE_MMU  ?= 1
DDR_2M_MAPPING ?= 1
endif


CODE_READONLY ?= #{{CODE_READONLY}}

#########################################################################
# options for compile command
#########################################################################
COMPILE_PREFIX  ?=

## Available choices:
## ocm: Program will be download into ocm and run directly in ocm
## ddr: Program will be download into ddr and run directly in ddr
## tcm: Program will be download into tcm and run directly in tcm

ifeq ($(AL_CHIP),dr1v90)
DOWNLOAD        ?= tcm
else
DOWNLOAD        ?= ddr
endif


## If V=1, it will display compiling message in verbose including compiling options
#Handling of V=1/VERBOSE=1 flag
# if V=1, $(DETAILS) will echo extra details

VERBOSE =  #{{VERBOSE}}
V ?= $(VERBOSE)

ifeq ($(V),1)
	Q = 
else
	Q = @
endif


## If SILENT=1, it will not display any compiling messsage
SILENT ?= #{{SILENT}}

ifeq ($(SILENT),1)
	MAKEFLAGS += -s 
else
	MAKEFLAGS +=
endif

OUTPUT_DEV ?= #{{OUTPUT_DEV}}

ifeq ($(OUTPUT_DEV),UART0)
	CFLAGS += -DLOG_DEV=AL_LOG_UART0
else ifeq ($(OUTPUT_DEV),UART1)
	CFLAGS += -DLOG_DEV=AL_LOG_UART1
else ifeq ($(OUTPUT_DEV),none)
	CFLAGS += -DLOG_DEV=NONE
endif


# Variables should be defined in Application Makefile
## Available choices:
## The name of sub directories in $(AL_SDK_ROOT)/OS/
RTOS ?=

## If PFLOAT=1, it will enable float point print when using nano newlib

PFLOAT ?= #{{PFLOAT}}

## If NEWLIB=nano, it will use nano newlib, otherwise it will use normal newlib
NEWLIB ?= #{{NEWLIB}}

## If NOGC=1, it will not gc any sections during compiling to save code size
NOGC ?= #{{NOGC}}


LIB_OUTPUT_DIR  ?= $(AL_SDK_ROOT)/output

AL_SDK_ROOT := $(abspath $(AL_SDK_ROOT))

AL_CUR_DIR ?= $(abspath ./)

ifeq ($(AL_PLAT_DIR),)
	AL_PLAT_DIR := $(patsubst %/soc_plat.h, %, $(wildcard $(AL_SDK_ROOT)/*/soc_plat.h))
endif

AL_PLAT_DIR := $(abspath $(AL_PLAT_DIR))

BSP_DIR  ?= $(patsubst %/chip, %, $(wildcard $(AL_SDK_ROOT)/*/chip))

CHIP_DIR ?= $(wildcard $(BSP_DIR)/*/$(AL_CHIP)) $(wildcard $(BSP_DIR)/chip/*/$(AL_CHIP))

#########################################################################

export AL_CHIP
export ARMv8_STATE
export ARMv8_EL
export ARMv8_SECURE
export ARMv8_CORE
export LIB_OUTPUT_DIR
export COMPILE_PREFIX
export V
export SILENT
export RTOS
export AL_SDK_ROOT
export AL_PLAT_DIR
export SDK_ROOT
export CFLAGS
export BSP_DIR
export CHIP_DIR
export ENABLE_MMU
export DDR_2M_MAPPING
export CODE_READONLY
export Q
export AL_CUR_DIR

# vim: syntax=make
