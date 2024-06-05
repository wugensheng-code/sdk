# (0) include config file

# (1.1) add submodule resource file
C_SRCS += $(wildcard ./port/dwc2/usb_hc_dwc2.c) \
			$(wildcard ./port/dwc2/usb_dc_dwc2.c) \
			$(wildcard ./core/usbh_core.c) \
			$(wildcard ./core/usbd_core.c) \
			$(wildcard ./class/cdc/usbd_cdc.c) \
			$(wildcard ./class/hub/usbh_hub.c) \
			$(wildcard ./third_party/fatfs/fatfs_usbh.c) \
			$(wildcard ./class/msc/usbh_msc.c) \
			$(wildcard ./class/msc/usbd_msc.c) \
			$(wildcard ./osal/al_usb_osal.c)

# (1.2) add submodule include file to /bsp/inc for calling
INC_DIR += common \
			port/dwc2 \
			core \
			class/cdc \
			class/hub \
			../FATFS \
			class/msc \
			config \
			osal

# (3) set specificity FLAGS
LIBNAME := cherryusb_driver

# (2) compile and return obj-y
# include $(SDK_ROOT)/tools/make/rules.mk

include ../../../tools/make/rules.mk
