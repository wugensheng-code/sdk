ifeq ($(AL_CHIP),dr1v90)
INC_DIR +=$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/applications \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/board \
	$(AL_SDK_ROOT)/fpsoc/inc \
	$(AL_SDK_ROOT)/fpsoc/arch/common/inc \
	$(AL_SDK_ROOT)/fpsoc/arch \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/uart/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/can/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/dmacahb/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/gpio/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/iic/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/mmc/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/mpu/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/qspi/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/smc/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/spi/inc \
	$(AL_SDK_ROOT)/fpsoc/lib/cli/inc \
	$(AL_SDK_ROOT)/fpsoc/arch/riscv/rv64/inc \
	$(AL_SDK_ROOT)/fpsoc/arch/riscv/ext-nuclei/inc \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/board/riscv \
	$(AL_SDK_ROOT)/fpsoc/chip/dr1x90/dr1v90/inc \
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
	$(AL_SDK_ROOT)/fpsoc/inc \
	$(AL_SDK_ROOT)/fpsoc/arch/common/inc \
	$(AL_SDK_ROOT)/fpsoc/arch \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/uart/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/can/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/dmacahb/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/gpio/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/iic/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/mmc/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/mpu/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/qspi/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/smc/inc \
	$(AL_SDK_ROOT)/fpsoc/driver/ps_driver/spi/inc \
	$(AL_SDK_ROOT)/fpsoc/lib/cli/inc \
	$(AL_SDK_ROOT)/3rdparty/os/RT-Thread/board/arm \
	$(AL_SDK_ROOT)/fpsoc/arch/arm/common/gic_v3/inc \
	$(AL_SDK_ROOT)/fpsoc/arch/arm/fpsoc/arch/arm/common/gic_v3/inc \
	$(AL_SDK_ROOT)/fpsoc/arch/arm/armv8/aarch64/cortex-a/inc \
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
