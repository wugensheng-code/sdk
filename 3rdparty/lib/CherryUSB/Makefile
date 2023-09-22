# (0) include config file

# (1.1) add submodule resource file
C_SRCS += $(wildcard ./port/dwc2/*.c) \
			$(wildcard ./core/*.c) \
			$(wildcard ./class/cdc/*.c) \
			$(wildcard ./class/hub/*.c) \
			$(wildcard ./class/hid/*.c) \
			$(wildcard ./class/msc/*.c) \
			$(wildcard ./class/video/*.c) \
			$(wildcard ./class/audio/*.c) \
			$(wildcard ./osal/al_usb_osal.c)

# (1.2) add submodule include file to /bsp/inc for calling
INC_DIR += common \
			port/dwc2 \
			core \
			class/cdc \
			class/hub \
			class/hid \
			class/msc \
			class/video \
			class/audio \
			config \
			osal

# (3) set specificity FLAGS
LIBNAME := cherryusb_driver

# (2) compile and return obj-y
# include $(SDK_ROOT)/tools/make/rules.mk

include ../../../tools/make/rules.mk
