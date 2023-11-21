make cleanelf
make clean ELF=NONDMA
make clean ELF=SDMA
make clean ELF=ADMA2
clear
make ELF=NONDMA -j8 dasm
clear
make ELF=SDMA -j8 dasm
clear
make ELF=ADMA2 -j8 dasm
clear