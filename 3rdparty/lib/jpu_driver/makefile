# (0) include config file

# (1.1) add submodule resource file
C_SRCS += $(wildcard ./src/*.c) \
			$(wildcard ./jdi/src/*.c) \
			$(wildcard ./jdi/nonos/*.c) \
			$(wildcard ./jpuapi/src/*.c) \
			$(wildcard ./helper/src/*.c)

# (1.2) add submodule include file to /bsp/inc for calling
INC_DIR += inc \
			jdi/inc \
			jpuapi/inc \
			helper/inc 



# (3) set specificity FLAGS
LIBNAME := jpu_driver


# (2) compile and return obj-y
# include $(SDK_ROOT)/tools/make/rules.mk

include ../../../tools/make/rules.mk
