# (0) include config file

# (1.1) add submodule resource file
SRC_DIR = FreeRTOS-Kernel \
           FreeRTOS-Kernel/portable/GCC/ARM_CA53_64_BIT_SRE \

C_SRCS	= FreeRTOS-Kernel/portable/MemMang/heap_4.c


CFLAGS += -DGUEST

# (1.2) add submodule include
INC_DIR += FreeRTOS-Kernel \
           FreeRTOS-Kernel/include \
           FreeRTOS-Kernel/portable/GCC/ARM_CA53_64_BIT_SRE

ifeq ($(CHIP_ARCH),armv8-a)
INC_DIR += $(AL_SDK_ROOT)/fpsoc/arch/arm/common/gic_v3/inc \
           $(AL_SDK_ROOT)/fpsoc/arch/arm/armv8/aarch64/cortex-a/inc \
           $(AL_SDK_ROOT)/fpsoc/arch \
           $(AL_SDK_ROOT)/fpsoc/arch/arm/common/gic_v3/inc \
           $(AL_SDK_ROOT)/fpsoc/arch/common/inc \
           FreeRTOS-Kernel/portable/GCC/ARM_CA53_64_BIT_SRE
else

endif


# (3) set specificity FLAGS
LIBNAME := freertos


include ../../../tools/make/rules.mk
