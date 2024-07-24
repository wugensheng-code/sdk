make cleanelf
make clean ELF=IN_LOOPBACK
make clean ELF=EX_LOOPBACK
make clean ELF=FD_DMA
make clean ELF=FD_STB_FIFO
make clean ELF=FD_STB_PRIO
make clean ELF=STD_PTB
make clean ELF=FD_DMA_LLP
make clean ELF=IN_LOOPBACK ARMv8_STATE=32
make clean ELF=EX_LOOPBACK ARMv8_STATE=32
make clean ELF=FD_DMA ARMv8_STATE=32
make clean ELF=FD_STB_FIFO ARMv8_STATE=32
make clean ELF=FD_STB_PRIO ARMv8_STATE=32
make clean ELF=STD_PTB ARMv8_STATE=32
make clean ELF=FD_DMA_LLP ARMv8_STATE=32
make clean CHIP=dr1v90 ELF=IN_LOOPBACK
make clean CHIP=dr1v90 ELF=EX_LOOPBACK
make clean CHIP=dr1v90 ELF=FD_DMA
make clean CHIP=dr1v90 ELF=FD_STB_FIFO
make clean CHIP=dr1v90 ELF=FD_STB_PRIO
make clean CHIP=dr1v90 ELF=STD_PTB
make clean CHIP=dr1v90 ELF=FD_DMA_LLP
clear
make ELF=IN_LOOPBACK -j8 dasm
clear
make ELF=EX_LOOPBACK -j8 dasm
clear
make ELF=FD_DMA -j8 dasm
clear
make ELF=FD_DMA_LLP -j8 dasm
clear
make ELF=FD_STB_FIFO -j8 dasm
clear
make ELF=FD_STB_PRIO -j8 dasm
clear
make ELF=STD_PTB -j8 dasm
clear
make ELF=IN_LOOPBACK ARMv8_STATE=32 -j8 dasm
clear
make ELF=EX_LOOPBACK ARMv8_STATE=32 -j8 dasm
clear
make ELF=FD_DMA ARMv8_STATE=32 -j8 dasm
clear
make ELF=FD_DMA_LLP ARMv8_STATE=32 -j8 dasm
clear
make ELF=FD_STB_FIFO ARMv8_STATE=32 -j8 dasm
clear
make ELF=FD_STB_PRIO ARMv8_STATE=32 -j8 dasm
clear
make ELF=STD_PTB ARMv8_STATE=32 -j8 dasm
clear
make CHIP=dr1v90 ELF=IN_LOOPBACK -j8 dasm
clear
make CHIP=dr1v90 ELF=EX_LOOPBACK -j8 dasm
clear
make CHIP=dr1v90 ELF=FD_DMA -j8 dasm
clear
make CHIP=dr1v90 ELF=FD_DMA_LLP -j8 dasm
clear
make CHIP=dr1v90 ELF=FD_STB_FIFO -j8 dasm
clear
make CHIP=dr1v90 ELF=FD_STB_PRIO -j8 dasm
clear
make CHIP=dr1v90 ELF=STD_PTB -j8 dasm
clear
