make COMPILE_PREFIX=/opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf- PROGRAM=./ V=1 clean &&
make COMPILE_PREFIX=/opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf- PROGRAM=./ V=1 all

make COMPILE_PREFIX=/opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf- PROGRAM=./application/baremetal/AL9000_USB/AL9000_usb_case3/AL9000_usb/ DOWNLOAD=ilm CORE=ux600fd dasm
