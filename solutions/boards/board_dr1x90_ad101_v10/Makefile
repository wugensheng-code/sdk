# (0) include config file

# (1.1) add submodule resource file
SRC_DIR = ./src

# (1.2) add submodule include
INC_DIR += ./inc

LIB_DIR += $(AL_PLAT_DIR)/src/ddr_demo/

# (3) set specificity FLAGS
LIBNAME := plat

SUB_LD_LIBS = $(AL_PLAT_DIR)/src/ddr_demo/libddr.a

lib: ddr

.PHONY:
ddr: $(SUB_LD_LIBS)
	cp -r $(SUB_LD_LIBS) $(LIB_OUTPUT_DIR)/lib$(LIBNAME).a

.PHONY:
$(SUB_LD_LIBS): 
	$(MAKE) -C $(AL_PLAT_DIR)/src/ddr_demo/ ddr

.PHONY:
lib.do.clean: ddr.clean


.PHONY:
ddr.clean: 
	$(MAKE) -C $(AL_PLAT_DIR)/src/ddr_demo/ clean_ddr

include $(AL_SDK_ROOT)/tools/make/rules.mk
