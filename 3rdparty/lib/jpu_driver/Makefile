# (0) include config file
include $(core)/config.mk

# (1.1) add submodule resource file
src-$(CONFIG_JPU_DRIVER) += $(wildcard *.c)

# (1.2)add submodule include file path
inc-$(CONFIG_JPU_DRIVER) += ./

# (2) compile and return obj-y
include $(core)/build.mk

