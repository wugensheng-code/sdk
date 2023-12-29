#########################################################################
AL_SDK_ROOT ?= $(SDK_ROOT)
AL_PLAT_DIR ?= $(PLAT_DIR)
LIB_PREBUILD_DIR  ?= $(AL_SDK_ROOT)/prebuild

include $(AL_SDK_ROOT)/tools/make/config.mk
sinclude $(AL_PLAT_DIR)/board_cfg.mk

CC      = ${Q}$(COMPILE_PREFIX)gcc
CXX     = ${Q}$(COMPILE_PREFIX)g++
AR      = ${Q}$(COMPILE_PREFIX)ar
LD      = ${Q}$(COMPILE_PREFIX)ld
OBJCOPY = ${Q}$(COMPILE_PREFIX)objcopy
OBJDUMP = ${Q}$(COMPILE_PREFIX)objdump
NM      = ${Q}$(COMPILE_PREFIX)nm
AS      = ${Q}$(COMPILE_PREFIX)as
GDB     = ${Q}$(COMPILE_PREFIX)gdb
SIZE    = ${Q}$(COMPILE_PREFIX)size
ECHO  	= echo
MAKE    = make

ARFLAGS = crs

#########################################################################
ifeq ($(AL_CHIP),dr1v90)
CORE            := riscv
CHIP_ARCH       := rv64imafdc
ARCH_ABI        := lp64d
ARCH_EXT        := ext-nuclei
else ifeq ($(AL_CHIP),dr1m90)
CORE            := arm
CHIP_ARCH       := armv8-a
MTUNE           := cortex-a35
endif

export CHIP_ARCH
export ARCH_EXT

#########################################################################
LINKER_SCRIPT ?= $(CHIP_DIR)/lds/gcc_$(AL_CHIP)_$(DOWNLOAD).ld

ifeq ($(ENABLE_MMU),1)
AL_CFLAGS   += -DENABLE_MMU=1
ifeq ($(DDR_2M_MAPPING),1)
AL_CFLAGS   += -DDDR_2M_MAPPING=1
ifeq ($(CODE_READONLY),1)
AL_CFLAGS   += -DCODE_READONLY=1
endif
endif
endif


#########################################################################
# gcc arm option:   https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
# gcc riscv option: https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Options.html
#

ifeq ($(CORE), arm)
AL_CFLAGS   += -mtune=$(MTUNE) -march=$(CHIP_ARCH) -fno-builtin \
               $(GC_CFLAGS) -fno-common -DDOWNLOAD_MODE=$(DOWNLOAD)
else ifeq ($(CORE), riscv)
AL_CFLAGS   += -march=$(CHIP_ARCH) -mabi=$(ARCH_ABI) -mcmodel=medany \
               $(GC_CFLAGS) -fno-common -DDOWNLOAD_MODE=$(DOWNLOAD)

ifeq ($(NEWLIB),nano)
NEWLIB_LDFLAGS = --specs=nano.specs
endif

endif

MKDEP_OPT   = -MMD -MT $@ -MF $@.d

ifeq ($(RTOS), freertos)
AL_CFLAGS += -DUSE_RTOS
AL_CFLAGS += -DRTOS_FREERTOS
CFLAGS += -DUSE_RTOS
CFLAGS += -DRTOS_FREERTOS
else ifeq ($(RTOS), rtthread)
AL_CFLAGS += -DUSE_RTOS
AL_CFLAGS += -DRTOS_RTTHREAD
CFLAGS += -DUSE_RTOS
CFLAGS += -DRTOS_RTTHREAD
endif

#########################################################################
# all public inc
PUBLIC_INC_DIR :=  $(BSP_DIR)/inc \
                   $(CHIP_DIR)/inc \
                   $(CHIP_DIR)/../common/inc \
                   $(wildcard $(BSP_DIR)/driver/pl_driver/*/inc) \
                   $(wildcard $(BSP_DIR)/driver/ps_driver/*/inc) \
                   $(patsubst %/Makefile, %, $(wildcard $(AL_SDK_ROOT)/3rdparty/lib/*/Makefile)) \
                   $(wildcard $(AL_SDK_ROOT)/3rdparty/lib/*/inc) \
                   $(wildcard $(AL_SDK_ROOT)/3rdparty/lib/*/*/inc) \
                   $(wildcard $(BSP_DIR)/lib/*/inc) \
                   $(wildcard $(BSP_DIR)/lib/*/api/inc) \
                   $(AL_SDK_ROOT)/3rdparty/lib/FATFS/lagecy/include \
                   $(AL_SDK_ROOT)/3rdparty/lib/FATFS/lagecy/qspi \
                   $(AL_SDK_ROOT)/3rdparty/lib/FATFS/lagecy/sd_emmc


ifeq ($(RTOS), rtthread)
PUBLIC_INC_DIR +=   $(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/include \
                    $(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/finsh \
                    $(AL_SDK_ROOT)/3rdparty/os/RT-Thread
endif

ifeq ($(RTOS), freertos)
PUBLIC_INC_DIR +=   $(AL_SDK_ROOT)/3rdparty/os/FreeRTOS/FreeRTOS-Kernel/include \
                    $(AL_SDK_ROOT)/3rdparty/os/FreeRTOS/FreeRTOS-Kernel

ifeq ($(AL_CHIP),dr1m90)
PUBLIC_INC_DIR +=   $(CHIP_DIR)/../../../arch/arm/armv8/aarch64/cortex-a/inc \
					$(AL_SDK_ROOT)/3rdparty/os/FreeRTOS/FreeRTOS-Kernel/portable/GCC/ARM_CA53_64_BIT_SRE \
                    $(CHIP_DIR)/../../../arch/common/inc \
                    $(CHIP_DIR)/../../../arch/arm/common/gic_v3/inc

else ifeq ($(AL_CHIP), dr1v90)
PUBLIC_INC_DIR +=   $(AL_SDK_ROOT)/fpsoc/arch/riscv/rv64/inc \
					$(AL_SDK_ROOT)/fpsoc/arch/riscv/ext-nuclei/inc \
					$(CHIP_DIR)/../../../arch/common/inc \
					$(AL_SDK_ROOT)/3rdparty/os/FreeRTOS/FreeRTOS-Kernel/portable/GCC/RISC-V

CFLAGS += -D__SYSTIMER_PRESENT=1 -D__Vendor_SysTickConfig=0 -D__ECLIC_PRESENT=1

endif

endif

PUBLIC_INC_DIR +=  $(AL_SDK_ROOT)/3rdparty/lwip-2.1.3/src/include \
                   $(AL_SDK_ROOT)/3rdparty/lwip-2.1.3/src/include/compat/posix \
                   $(AL_SDK_ROOT)/3rdparty/lwip-2.1.3/ports   \
                   $(AL_SDK_ROOT)/3rdparty/lwip-2.1.3/ports/netif

PUBLIC_INC_DIR +=  $(SDK_ROOT)/3rdparty/lib/CherryUSB/common \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/port/dwc2 \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/core \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/class/cdc \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/class/hub \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/class/hid \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/class/msc \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/class/video \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/class/audio \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/config \
                   $(SDK_ROOT)/3rdparty/lib/CherryUSB/osal


#libmetal
PUBLIC_INC_DIR +=   $(SDK_ROOT)/3rdparty/lib/libmetal/include/lib/include

#open-amp
PUBLIC_INC_DIR +=   $(SDK_ROOT)/3rdparty/lib/open-amp/lib/include \
				   	$(SDK_ROOT)/3rdparty/lib/open-amp/lib/rpmsg \
					$(SDK_ROOT)/3rdparty/lib/open-amp/include/include/generated





PUBLIC_INC  :=  $(foreach subdir,$(sort $(PUBLIC_INC_DIR)), -I$(subdir))

## module inc
MODULE_INC  :=  $(foreach subdir,$(sort $(INC_DIR)), -I$(subdir)) -I$(AL_PLAT_DIR)/inc -I$(AL_PLAT_DIR)

#########################################################################

ifeq ($(CORE),arm)
CFLAGS += -mstrict-align -ffreestanding -fno-omit-frame-pointer -fno-stack-protector -mcpu=cortex-a35 -gdwarf-2
endif

AL_CFLAGS  += $(CFLAGS) $(PUBLIC_INC) $(MODULE_INC) $(MKDEP_OPT) -w

#########################################################################
# ldflags
ifneq ($(NOGC),1)
GC_CFLAGS  =   -ffunction-sections -fdata-sections
GC_LDFLAGS +=  -Wl,--gc-sections -Wl,--check-sections
else
GC_LDFLAGS += -flto
endif

ifeq ($(PFLOAT),1)
NEWLIB_LDFLAGS += -u _printf_float
endif

LIB_OPT  = $(addprefix -L, $(sort $(LIB_DIR)))
LDFLAGS += -T$(LINKER_SCRIPT) -Wl,--start-group -Wl,--whole-archive $(ld_libs) -Wl,--no-whole-archive -lgcc -lc -lm -L$(LIB_OUTPUT_DIR) -L$(LIB_PREBUILD_DIR) -Wl,--end-group \
           $(LIB_OPT) -nostartfiles -Wl,-M,-Map=$(TARGET).map \
           $(GC_LDFLAGS) $(NEWLIB_LDFLAGS) --specs=nosys.specs -Wl,--build-id=none \
           -u _isatty -u _write -u _sbrk -u _read -u _close -u _fstat -u _lseek -u memset -u memcpy

ifeq ($(CORE),arm)
LDFLAGS += -Wl,--no-warn-rwx-segments 
endif
#########################################################################
# source

C_SRCS     += $(foreach subdir, $(SRC_DIR), $(wildcard $(subdir)/*.c $(subdir)/*.C))
CXX_SRCS   += $(foreach subdir, $(SRC_DIR), $(wildcard $(subdir)/*.cpp $(subdir)/*.CPP))
ASM_SRCS   += $(foreach subdir, $(SRC_DIR), $(wildcard $(subdir)/*.s $(subdir)/*.S))

C_OBJS     := $(C_SRCS:=.o)
ASM_OBJS   := $(ASM_SRCS:=.o)

CXX_OBJS   := $(CXX_SRCS:=.o)

ALL_OBJS   =  $(ASM_OBJS) $(C_OBJS) $(CXX_OBJS)
ALL_DEPS   := $(ALL_OBJS:=.d)

CLEAN_OBJS += $(TARGET).elf $(TARGET).map $(TARGET).bin $(TARGET).dump $(TARGET).dasm \
              $(TARGET).hex $(TARGET).verilog $(ALL_OBJS) $(ALL_DEPS)

REAL_CLEAN_OBJS = $(subst /,$(PS), $(CLEAN_OBJS))

#########################################################################
# Prerequesties
COMMON_PREREQS +=	$(AL_SDK_ROOT)/tools/make/rules.mk
COMMON_PREREQS +=	$(AL_SDK_ROOT)/tools/make/config.mk
COMMON_PREREQS +=	$(AL_CUR_DIR)/Makefile
COMMON_PREREQS +=   Makefile




#########################################################################
# target: build elf, or build libs
#
ifneq ($(TARGET),)

TARGET_ELF = $(TARGET).elf
$(TARGET): $(TARGET_ELF)

endif
#########################################################################
# Default goal, placed before dependency includes
#
all: info $(TARGET_ELF)

#########################################################################
# include dependency files of application
#
ifneq ($(MAKECMDGOALS),clean)
-include $(ALL_DEPS)
endif

.PHONY: all info help clean

info:
	@$(ECHO) AL_CHIP=$(AL_CHIP) CORE=$(CORE) BOARD=$(BOARD) V=$(V) RTOS=$(RTOS) PFLOAT=$(PFLOAT) NOGC:$(NOGC) DOWNLOAD: $(DOWNLOAD)

help:
	@$(ECHO) "Anlogic FPSoc Software Development Kit "
	@$(ECHO) "== Make variables used in FPSoc SDK =="
	@$(ECHO) "SOC:         Select SoC built in FPSoc SDK, will select board_dr1x90_emulation by default"
	@$(ECHO) "BOARD:       Select SoC's Board built in FPSoc SDK, will select nuclei_fpga_eval by default"
	@$(ECHO) "DOWNLOAD:    Select SoC's download mode, use ocm by default, optional ocm/ddr"
	@$(ECHO) "V:           V=1 verbose make, will print more information, by default V=0"
	@$(ECHO) "== Example Usage =="
	@$(ECHO) "cd $(AL_SDK_ROOT)/solutions/demo/baremetal/helloworld make DOWNLOAD=ocm"

#########################################################################
# Convenience function for verifying option has a boolean value
# $(eval $(call assert_boolean,FOO)) will assert FOO is 0 or 1
define assert_boolean
    $(if $(filter-out 0 1,$($1)),$(error $1 must be boolean))
endef

# Convenience function for verifying options have boolean values
# $(eval $(call assert_booleans,FOO BOO)) will assert FOO and BOO for 0 or 1 values
define assert_booleans
    $(foreach bool,$1,$(eval $(call assert_boolean,$(bool))))
endef

# Convenience function for verifying option has a right string
# $(eval $(call assert_option,option1 option2, FOO)) will assert FOO is string1 or string2
define assert_two_option
    $(if $(filter-out $1 $2, $3),$(error $4 must be $1 or $2))
endef

define assert_three_option
    $(if $(filter-out $1$2$3,$4),$(error$5must be $1 or $2 or $3))
endef

.PHONY: check

check:
	$(call assert_booleans, DDR_2M_MAPPING ENABLE_MMU CODE_READONLY VERBOSE SILENT PFLOAT NOGC)
	$(call assert_option_two_option, 32, 64, $(ARMv8_STATE), ARMv8_STATE)
	$(call assert_two_option,EL1, EL3, $(ARMv8_EL), ARMv8_EL)
	$(call assert_two_option,SECURE, NONSECURE, $(ARMv8_SECURE), ARMv8_SECURE)
	$(call assert_two_option,MASTER, SLAVE, $(ARMv8_CORE), ARMv8_CORE)
	$(call assert_two_option,dr1v90, dr1m90, $(AL_CHIP), AL_CHIP)
	$(call assert_two_option,freertos, rtthread, $(RTOS), RTOS)
	$(call assert_three_option,ocm,ddr,tcm $(DOWNLOAD), DOWNLOAD)
	@$(ECHO) "all parameters have been checked"

#########################################################################
$(ASM_OBJS): %.o: % $(COMMON_PREREQS)
	@$(ECHO) "Compling: " $(notdir $@)
	$(CC) $(AL_CFLAGS) -c -o $@ $<

#########################################################################
$(C_OBJS): %.o: % $(COMMON_PREREQS)
	@$(ECHO) "Compling: " $(notdir $@)
	$(CC) $(AL_CFLAGS) -c -o $@ $<

#########################################################################
$(CXX_OBJS): %.o: % $(COMMON_PREREQS)
	@$(ECHO) "Compling: " $(notdir $@)
	$(CC) $(AL_CFLAGS) -c -o $@ $<

#########################################################################
#### if target is elf
####

ifeq ($(RTOS), freertos)
	filterout_lib = %librtthread
else ifeq ($(RTOS), rtthread)
	filterout_lib = %libfreertos
else
	filterout_lib = %libfreertos %librtthread
endif


$(TARGET_ELF): bsp make_all_libs $(ALL_OBJS)
	$(eval ld_libs := $(patsubst lib%.a,-l%,$(filter-out $(filterout_lib), $(notdir $(wildcard $(LIB_OUTPUT_DIR)/*.a $(LIB_PREBUILD)/*.a)))) $(LD_LIBS))
	$(CC) $(ALL_OBJS) -o $@ $(AL_CFLAGS) $(LDFLAGS)
	$(OBJCOPY) $@ -O binary $(TARGET).bin
	$(SIZE) $@


########################################################################
# get bsp library path: bps folder name is different between
# sdk workspace and embedded workspace

.PHONY: bsp
bsp:
ifneq ($(BSP_DIR),)
	@$(MAKE) -C $(BSP_DIR) lib
endif

.PHONY: bsp_clean
bsp_clean:
ifneq ($(BSP_DIR),)
	@$(MAKE) -C $(BSP_DIR) lib.do.clean
endif

#########################################################################
# 3rdparty, libnpuruntime may not include in sdk workspace
#########################################################################
LIBS_DIR = $(patsubst %/Makefile, %, $(wildcard $(AL_SDK_ROOT)/3rdparty/lib/*/Makefile $(BSP_DIR)/lib/*/Makefile))

LIBS_DIR += $(patsubst %/Makefile, %, $(wildcard $(AL_SDK_ROOT)/3rdparty/lwip-2.1.3/Makefile))

ifneq ($(PLAT_DIR),)
LIBS_DIR += $(PLAT_DIR)
LIBS_DIR += $(patsubst %/Makefile, %, $(wildcard $(PLAT_DIR)/src/ddr_demo/Makefile))

endif


ifeq ($(RTOS), freertos)
    LIBS_DIR += $(patsubst %/Makefile, %, $(wildcard $(AL_SDK_ROOT)/3rdparty/os/FreeRTOS/Makefile))
else ifeq ($(RTOS), rtthread)
	LIBS_DIR += $(patsubst %/Makefile, %, $(wildcard $(AL_SDK_ROOT)/3rdparty/os/RT-Thread/Makefile))
endif


.PHONY: make_all_libs
make_all_libs: $(addsuffix /make.lib, $(LIBS_DIR))

.PHONY:
%/make.lib:
	$(MAKE) -C $(patsubst %/make.lib,%,$@) lib

.PHONY: all_libs_clean
all_libs_clean: $(addsuffix /make.clean, $(LIBS_DIR))

.PHONY:
%/make.clean:
	$(MAKE) -C $(patsubst %/make.clean,%,$@) lib.do.clean

.PHONY:
lib.do.clean:
	$(RM) -rf $(ALL_OBJS) $(ALL_DEPS) $(LIB_OUTPUT_DIR)/lib$(LIBNAME).a

#########################################################################
#### if target is lib
####

lib: $(LIB_OUTPUT_DIR)/lib$(LIBNAME).a
$(LIB_OUTPUT_DIR)/lib$(LIBNAME).a: $(ALL_OBJS) $(SUB_LD_LIBS)
	@mkdir -p $(LIB_OUTPUT_DIR)
	$(AR) $(ARFLAGS) $@ $(C_OBJS) $(ASM_OBJS)

#########################################################################
dasm: $(TARGET_ELF)
	$(OBJDUMP) -S -d --all-headers --demangle --line-numbers --wide $< > $(TARGET).dump
	$(OBJDUMP) -d $< > $(TARGET).dasm
	$(OBJCOPY) $< -O ihex $(TARGET).hex
	$(OBJCOPY) $< -O verilog $(TARGET).verilog
	$(OBJCOPY) $< -O binary $(TARGET).bin

#########################################################################
clean: all_libs_clean bsp_clean
	@$(ECHO) "Clean all build objects"
	$(RM) $(CLEAN_OBJS)

# vim: syntax=make
