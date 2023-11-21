make clean ELF=SINGLE_BLOCK
make clean ELF=SINGLE_NONBLOCK
make clean ELF=LLP_BLOCK
clear
make ELF=SINGLE_BLOCK -j8 dasm
clear
make ELF=SINGLE_NONBLOCK -j8 dasm
clear
make ELF=LLP_BLOCK -j8 dasm