make cleanelf
make clean
make SOC=demoapu PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_ocm_ecc_int_test 
/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-objdump -D -S pmu_ocm_ecc_int_test.elf > pmu_ocm_ecc_int_test.dump
make clean
make SOC=demoapu PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_apu_normal_init_test
/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-objdump -D -S pmu_apu_normal_init_test.elf > pmu_apu_normal_init_test.dump
make clean
make PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_rpu_normal_init_test DOWNLOAD=ilm CORE=ux600fd dasm
make clean
make SOC=demoapu PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_apu_fatal_bus_timeout_test 
/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-objdump -D -S pmu_apu_fatal_bus_timeout_test.elf > pmu_apu_fatal_bus_timeout_test.dump
make clean
make SOC=demoapu PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_apu_fatal_wdt0_test 
/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-objdump -D -S pmu_apu_fatal_wdt0_test.elf > pmu_apu_fatal_wdt0_test.dump
make clean
make SOC=demoapu PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_apu_fatal_wdt1_test 
/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-objdump -D -S pmu_apu_fatal_wdt1_test.elf > pmu_apu_fatal_wdt1_test.dump
make clean
make SOC=demoapu PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_apu_fatal_wdt2_test 
/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-objdump -D -S pmu_apu_fatal_wdt2_test.elf > pmu_apu_fatal_wdt2_test.dump
make clean
make SOC=demoapu PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_apu_fatal_apu_ecc_test 
/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-objdump -D -S pmu_apu_fatal_apu_ecc_test.elf > pmu_apu_fatal_apu_ecc_test.dump
make clean
make SOC=demoapu PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_apu_fatal_ocm_ecc_test 
/opt/toolchain/7.5.0/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-elf/bin/aarch64-elf-objdump -D -S pmu_apu_fatal_ocm_ecc_test.elf > pmu_apu_fatal_ocm_ecc_test.dump
make clean
make PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_rpu_fatal_multibit_ecc_test DOWNLOAD=ilm CORE=ux600fd dasm
make clean
make PINMUX_MODE=PINMUX_MODE1 TARGET=pmu_rpu_nmi_ocm_ecc_test DOWNLOAD=ilm CORE=ux600fd dasm
