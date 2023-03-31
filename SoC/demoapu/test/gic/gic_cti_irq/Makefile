#
# Add PPI_EDGE_TRIGGER to CFLAGS, need clean project first
#
all: clean $(TARGET)


TARGET = gic_cti_irq

NUCLEI_SDK_ROOT = ../../../../../

SRCDIRS = .

INCDIRS = .

COMMON_FLAGS := -DPPI_EDGE_TRIGGER

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
