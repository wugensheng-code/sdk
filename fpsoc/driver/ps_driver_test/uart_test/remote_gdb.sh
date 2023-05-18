make
/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-gdb -tui \
target remote 10.8.9.4:3333 \
load fpsoc_uart_el1_apu_mode1.elf \
add-symbol-file fpsoc_uart_el1_apu_mode1.elf \
