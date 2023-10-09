ifeq ($(AL_CHIP),dr1v90)
INC_DIR +=$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/applications \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/board \
	$(BSP_DIR)/inc \
	$(BSP_DIR)/arch/common/inc \
	$(BSP_DIR)/arch \
	$(BSP_DIR)/driver/ps_driver/uart/inc \
	$(BSP_DIR)/driver/ps_driver/can/inc \
	$(BSP_DIR)/driver/ps_driver/dmacahb/inc \
	$(BSP_DIR)/driver/ps_driver/gpio/inc \
	$(BSP_DIR)/driver/ps_driver/iic/inc \
	$(BSP_DIR)/driver/ps_driver/mmc/inc \
	$(BSP_DIR)/driver/ps_driver/mpu/inc \
	$(BSP_DIR)/driver/ps_driver/qspi/inc \
	$(BSP_DIR)/driver/ps_driver/smc/inc \
	$(BSP_DIR)/driver/ps_driver/spi/inc \
	$(BSP_DIR)/lib/cli/inc \
	$(BSP_DIR)/arch/riscv/rv64/inc \
	$(BSP_DIR)/arch/riscv/ext-nuclei/inc \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/board/riscv \
	$(BSP_DIR)/chip/dr1x90/dr1v90/inc \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/compilers/newlib \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/libcpu/risc-v/nuclei \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/finsh \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/include \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/posix/io/poll \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/posix/io/stdio \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/posix/ipc \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/utilities/utest \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/utest/testcases/kernel \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/compilers/common/include 

DEFINES += -DENABLE_PINMUX_MODE1 -DFPGA_STAGE_7 -DNUCLEI_BANNER=0 \
	-DRTOS_RTTHREAD -DRT_USING_LIBC -DRT_USING_NEWLIB -DUSE_RTOS -D_POSIX_C_SOURCE=1 -D_REENT_SMALL \
	-D__ECLIC_PRESENT=1 -D__RTTHREAD__ -D__SYSTIMER_PRESENT=1 -D__Vendor_SysTickConfig=0

else ifeq ($(AL_CHIP),dr1m90)

INC_DIR +=$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/applications \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/board \
	$(BSP_DIR)/inc \
	$(BSP_DIR)/arch/common/inc \
	$(BSP_DIR)/arch \
	$(BSP_DIR)/driver/ps_driver/uart/inc \
	$(BSP_DIR)/driver/ps_driver/can/inc \
	$(BSP_DIR)/driver/ps_driver/dmacahb/inc \
	$(BSP_DIR)/driver/ps_driver/gpio/inc \
	$(BSP_DIR)/driver/ps_driver/iic/inc \
	$(BSP_DIR)/driver/ps_driver/mmc/inc \
	$(BSP_DIR)/driver/ps_driver/mpu/inc \
	$(BSP_DIR)/driver/ps_driver/qspi/inc \
	$(BSP_DIR)/driver/ps_driver/smc/inc \
	$(BSP_DIR)/driver/ps_driver/spi/inc \
	$(BSP_DIR)/lib/cli/inc \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/board/arm \
	$(BSP_DIR)/arch/arm/common/gic_v3/inc \
	$(BSP_DIR)/arch/arm/fpsoc/arch/arm/common/gic_v3/inc \
	$(BSP_DIR)/arch/arm/armv8/aarch64/cortex-a/inc \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/libcpu/aarch64/common \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/libcpu/aarch64/cortex-a \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/compilers/common/include \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/compilers/newlib \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/finsh \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/include \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/posix/io/poll \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/posix/io/stdio \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/libc/posix/ipc \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/rt-thread/components/utilities/utest \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/utest/testcases/kernel 

DEFINES := -DFPGA_STAGE_7 -DRT_USING_LIBC -DRT_USING_NEWLIB -D_POSIX_C_SOURCE=1 -D__RTTHREAD__

endif
