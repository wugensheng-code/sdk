# (0) include config file

# (1.1) add submodule resource file
SRC_DIR += ./

# (1.2) add submodule include
INC_DIR += ../../inc/ddr_demo/ \
			../../inc/ddr_demo/ddr_regs

# (3) set specificity FLAGS

include $(AL_SDK_ROOT)/tools/make/rules.mk

.PHONY:
ddr: libddr.a

libddr.a: $(ALL_OBJS)
	$(AR) $(ARFLAGS) $@ $(C_OBJS) $(ASM_OBJS)

