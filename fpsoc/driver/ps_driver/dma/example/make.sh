make clean ELF=MEMCPY_BLOCK
make clean ELF=MEMCPY
make clean ELF=PERCPY_SINGLE
make clean ELF=PERCPY_CYCLE
make clean ELF=MEMCPY_PERFORMANCE
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