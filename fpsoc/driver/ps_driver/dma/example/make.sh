make clean ELF=MEMCPY_BLOCK
make clean ELF=MEMCPY
make clean ELF=PERCPY_SINGLE
make clean ELF=PERCPY_CYCLE
make clean ELF=MEMCPY_PERFORMANCE
make clean ELF=MEMCPY_BLOCK ARMv8_STATE=32
make clean ELF=MEMCPY ARMv8_STATE=32
make clean ELF=PERCPY_SINGLE ARMv8_STATE=32
make clean ELF=PERCPY_CYCLE ARMv8_STATE=32
make clean ELF=MEMCPY_PERFORMANCE ARMv8_STATE=32
make clean CHIP=dr1v90 ELF=MEMCPY_BLOCK
make clean CHIP=dr1v90 ELF=MEMCPY
make clean CHIP=dr1v90 ELF=MEMCPY_PERFORMANCE
make clean CHIP=dr1v90 ELF=PERCPY_SINGLE
make clean CHIP=dr1v90 ELF=PERCPY_CYCLE
clear
make ELF=MEMCPY_BLOCK -j8 dasm
clear
make ELF=MEMCPY -j8 dasm
clear
make ELF=PERCPY_SINGLE -j8 dasm
clear
make ELF=PERCPY_CYCLE -j8 dasm
clear
make ELF=MEMCPY_PERFORMANCE -j8 dasm
clear
make ELF=MEMCPY_BLOCK ARMv8_STATE=32 -j8 dasm
clear
make ELF=MEMCPY ARMv8_STATE=32 -j8 dasm
clear
make ELF=PERCPY_SINGLE ARMv8_STATE=32 -j8 dasm
clear
make ELF=PERCPY_CYCLE ARMv8_STATE=32 -j8 dasm
clear
make ELF=MEMCPY_PERFORMANCE ARMv8_STATE=32 -j8 dasm
clear
make CHIP=dr1v90 ELF=MEMCPY_BLOCK -j8 dasm
clear
make CHIP=dr1v90 ELF=MEMCPY -j8 dasm
clear
make CHIP=dr1v90 ELF=MEMCPY_PERFORMANCE -j8 dasm
clear
make CHIP=dr1v90 ELF=PERCPY_SINGLE -j8 dasm
clear
make CHIP=dr1v90 ELF=PERCPY_CYCLE -j8 dasm
clear
