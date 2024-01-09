# (0) include config file

# (1.1) add submodule resource file
SRC_DIR = FreeRTOS-Kernel \

ifeq ($(CHIP_ARCH),armv8-a)
SRC_DIR += FreeRTOS-Kernel/portable/GCC/ARM_CA53_64_BIT_SRE
else
SRC_DIR += FreeRTOS-Kernel/portable/GCC/RISC-V
endif

C_SRCS	= FreeRTOS-Kernel/portable/MemMang/heap_4.c


# (1.2) add submodule include
INC_DIR += FreeRTOS-Kernel \
           FreeRTOS-Kernel/include

ifeq ($(CHIP_ARCH),armv8-a)
INC_DIR += $(AL_SDK_ROOT)/fpsoc/arch/arm/common/gic_v3/inc \
           $(AL_SDK_ROOT)/fpsoc/arch/arm/armv8/aarch64/cortex-a/inc \
           $(AL_SDK_ROOT)/fpsoc/arch \
           $(AL_SDK_ROOT)/fpsoc/arch/arm/common/gic_v3/inc \
           $(AL_SDK_ROOT)/fpsoc/arch/common/inc \
           FreeRTOS-Kernel/portable/GCC/ARM_CA53_64_BIT_SRE
else
INC_DIR += FreeRTOS-Kernel/portable/GCC/RISC-V
endif

ifeq ($(ARMv8_EL),EL1)
CFLAGS += -DGUEST
else
CFLAGS += -D__ECLIC_PRESENT=1
endif

# (3) set specificity FLAGS
LIBNAME := freertos


include ../../../tools/make/rules.mk
