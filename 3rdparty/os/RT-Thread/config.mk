BSP_ROOT ?= /home/chenyu.wang/master_sdk/3rdparty/os/RT-Thread
RTT_ROOT ?= /home/chenyu.wang/master_sdk/3rdparty/os/RT-Thread/rt-thread

CROSS_COMPILE ?=/home/chenyu.wang/Nuclei/gcc/bin/riscv-nuclei-elf-

CFLAGS := -ffunction-sections -fdata-sections -fno-common  -O0 -ggdb
AFLAGS := -ffunction-sections -fdata-sections -fno-common  -ggdb
LFLAGS := --specs=nano.specs --specs=nosys.specs -nostartfiles -Wl,--gc-sections  -Wl,-cref,-Map=rtthread.map -u _isatty -u _write -u _sbrk -u _read -u _close -u _fstat -u _lseek 
CXXFLAGS := -ffunction-sections -fdata-sections -fno-common  -O0 -ggdb
EXTERN_LIB := -lstdc++ -lc -lm 

INC_DIR +=$(BSP_ROOT)/applications \
	$(BSP_ROOT)/board \
	/home/chenyu.wang/master_sdk/fpsoc/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/uart/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/can/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/dmacahb/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/gpio/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/iic/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/mmc/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/mpu/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/qspi/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/smc/inc \
	/home/chenyu.wang/master_sdk/fpsoc/driver/ps_driver/spi/inc \
	$(RTT_ROOT)/components/libc/compilers/common/include \
	$(RTT_ROOT)/components/libc/compilers/newlib \
	$(RTT_ROOT)/libcpu/risc-v/nuclei \
	/home/chenyu.wang/master_sdk/fpsoc/arch/riscv/ext-nuclei/inc \
	$(BSP_ROOT) \
	$(RTT_ROOT)/include \
	/home/chenyu.wang/master_sdk/fpsoc/arch/riscv/rv64/inc \
	/home/chenyu.wang/master_sdk/fpsoc/chip/dr1v90/inc \
	/home/chenyu.wang/master_sdk/solutions/boards/board_dr1x90_emulation/inc \
	/home/chenyu.wang/master_sdk/fpsoc/lib/log/inc \
	$(RTT_ROOT)/components/libc/posix/io/poll \
	$(RTT_ROOT)/components/libc/posix/io/stdio \
	$(RTT_ROOT)/components/libc/posix/ipc \
	$(RTT_ROOT)/components/utilities/ulog 

DEFINES := -DDOWNLOAD_MODE=DOWNLOAD_MODE_ILM -DDOWNLOAD_MODE_STRING="ILM" -DENABLE_PINMUX_MODE1 -DFPGA_STAGE_7 -DNUCLEI_BANNER=0 -DRTOS_RTTHREAD -DRT_USING_LIBC -DRT_USING_NEWLIB -DUSE_RTOS -D_POSIX_C_SOURCE=1 -D_REENT_SMALL -D__ECLIC_PRESENT=1 -D__RTTHREAD__ -D__SYSTIMER_PRESENT=1 -D__Vendor_SysTickConfig=0
