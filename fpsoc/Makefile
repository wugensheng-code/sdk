#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################

#########################################################################
# Make albsp & make -C 3rd party library, npu runtime library
#########################################################################

LIBNAME = albsp

#########################################################################
# arch sources & inc
#########################################################################

INC_DIR += inc \
           $(CHIP_DIR)/../common/inc \
           arch/common/inc


ifeq ($(CHIP_ARCH),armv8-a)

SRC_DIR += arch/arm/armv8/aarch64/cortex-a/src \
           arch/arm/common/gic_v3/src \
           arch/arm/common/sys_timer/src

INC_DIR += arch/arm/armv8/aarch64/cortex-a/inc \
           arch/arm/common/gic_v3/inc \
           arch/arm/common/sys_timer/inc

else

SRC_DIR += arch/riscv/rv64/src
INC_DIR += arch/riscv/rv64/inc


ifeq ($(ARCH_EXT),ext-nuclei)
SRC_DIR += arch/riscv/ext-nuclei/src
INC_DIR += arch/riscv/ext-nuclei/inc
endif

endif

#########################################################################
# chip sources dir
#########################################################################
SRC_DIR += $(CHIP_DIR)/../common/src \
           $(CHIP_DIR)/src

#########################################################################
# driver sources dir
#########################################################################
SRC_DIR += $(wildcard driver/pl_driver/*/src) \
           $(wildcard driver/ps_driver/*/src)

#########################################################################
# lib sources dir
#########################################################################
SRC_DIR += lib/newlib/gcc \
           $(wildcard lib/*/src)

#########################################################################
# platform sources dir
#########################################################################
SRC_DIR +=  $(AL_PLAT_DIR) \
            $(AL_PLAT_DIR)/src

#########################################################################

#########################################################################
# chip feature definition
#########################################################################

ifeq ($(AL_CHIP),dr1v90)
CPU_FEATURE += -D__ECLIC_PRESENT=1  -D__SYSTIMER_PRESENT=1
CPU_FEATURE += -D__PMP_PRESENT=1    -D__PMP_ENTRY_NUM=16
CPU_FEATURE += -D__ICACHE_PRESENT=0 -D__DCACHE_PRESENT=0
CPU_FEATURE += -D__Vendor_SysTickConfig=0 -DARCH_DMA_MINALIGN=64
else
ifeq ($(ARMv8_EL), EL1)
CFLAGS += -DSWITCH_TO_EL1_EL0_FROM_EL3
ifeq ($(ARMv8_SECURE), NONSECURE)
CFLAGS += -DSUPPORT_NONSECURE
endif
endif
ifeq ($(ARMv8_CORE), SLAVE)
CFLAGS += -DARM_CORE_SLAVE
endif
endif

CFLAGS += $(CPU_FEATURE)

#########################################################################
# include rules
#########################################################################
include $(AL_SDK_ROOT)/tools/make/rules.mk


# vim: syntax=make
