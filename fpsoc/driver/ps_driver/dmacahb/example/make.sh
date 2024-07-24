make clean ELF=SINGLE_BLOCK
make clean ELF=SINGLE_NONBLOCK
make clean ELF=LLP_BLOCK
make clean ELF=SINGLE_BLOCK ARMv8_STATE=32
make clean ELF=SINGLE_NONBLOCK ARMv8_STATE=32
make clean ELF=LLP_BLOCK ARMv8_STATE=32
make clean CHIP=dr1v90 ELF=SINGLE_BLOCK
make clean CHIP=dr1v90 ELF=SINGLE_NONBLOCK
make clean CHIP=dr1v90 ELF=LLP_BLOCK
clear
make ELF=SINGLE_BLOCK -j8 dasm
clear
make ELF=SINGLE_NONBLOCK -j8 dasm
clear
make ELF=LLP_BLOCK -j8 dasm
clear
make ELF=SINGLE_BLOCK ARMv8_STATE=32 -j8 dasm
clear
make ELF=SINGLE_NONBLOCK ARMv8_STATE=32 -j8 dasm
clear
make ELF=LLP_BLOCK ARMv8_STATE=32 -j8 dasm
clear
make CHIP=dr1v90 ELF=SINGLE_BLOCK -j8 dasm
clear
make CHIP=dr1v90 ELF=SINGLE_NONBLOCK -j8 dasm
clear
make CHIP=dr1v90 ELF=LLP_BLOCK -j8 dasm
clear