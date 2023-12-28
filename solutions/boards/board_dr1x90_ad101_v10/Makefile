# (0) include config file

# (1.1) add submodule resource file
SRC_DIR = ./src

# (1.2) add submodule include
INC_DIR += ./inc

LIB_DIR += $(AL_PLAT_DIR)/src/ddr_demo/

# (3) set specificity FLAGS
LIBNAME := plat

SUB_LD_LIBS = $(AL_PLAT_DIR)/src/ddr_demo/libddr.a

lib: $(AL_PLAT_DIR)/src/ddr_demo/libddr.a

$(AL_PLAT_DIR)/src/ddr_demo/libddr.a: 
	$(MAKE) -C $(AL_PLAT_DIR)/src/ddr_demo/ ddr
	@cp -i $(SUB_LD_LIBS) $(LIB_OUTPUT_DIR)/lib$(LIBNAME).a

.PHONY:
lib.do.clean: ddr.clean


.PHONY:
ddr.clean: 
	$(MAKE) -C $(AL_PLAT_DIR)/src/ddr_demo/ clean

include $(AL_SDK_ROOT)/tools/make/rules.mk
