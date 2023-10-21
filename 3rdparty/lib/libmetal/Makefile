# (0) include config file

# (1.1) add submodule resource file
C_SRCS += $(wildcard ./lib/*.c) \
		    $(wildcard ./lib/system/generic/*.c) \
			$(wildcard ./lib/system/generic/anlogic_common/*.c) \
			$(wildcard ./lib/system/generic/dr1m90/*.c)

# (1.2) add submodule include file to /bsp/inc for calling
INC_DIR += include/lib/include

CFLAGS += -DMACHINE="dr1m90" \
			-DDEFAULT_LOGGER_ON \
			-DMETAL_INTERNAL

# (3) set specificity FLAGS
LIBNAME := metal

# (2) compile and return obj-y
# include $(SDK_ROOT)/tools/make/rules.mk

include ../../../tools/make/rules.mk
