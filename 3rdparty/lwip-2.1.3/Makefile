# (0) include config file

LWIP_ROOT_DIR = .

# (1.1) add submodule resource file
SRC_DIR += $(LWIP_ROOT_DIR)/src/core \
           $(LWIP_ROOT_DIR)/src/core/ipv4 \
           $(LWIP_ROOT_DIR)/src/core/ipv6 \
           $(LWIP_ROOT_DIR)/src/api \
           $(LWIP_ROOT_DIR)/ports/netif \
           $(LWIP_ROOT_DIR)/ports/OS

# (1.2) add submodule include
INC_DIR += $(LWIP_ROOT_DIR)/src/include/ \
           $(LWIP_ROOT_DIR)/ports \
           $(LWIP_ROOT_DIR)/arch \
           $(LWIP_ROOT_DIR)/ports/netif

C_SRCS += $(LWIP_ROOT_DIR)/src/netif/ethernet.c

# (3) set specificity FLAGS
LIBNAME := lwip


include ../../tools/make/rules.mk
