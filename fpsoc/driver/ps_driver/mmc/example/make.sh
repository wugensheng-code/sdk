make cleanelf
make clean ELF=NONDMA IP=MMC0
make clean ELF=SDMA IP=MMC0
make clean ELF=ADMA2 IP=MMC0
make clean ELF=NONDMA IP=MMC1
make clean ELF=SDMA IP=MMC1
make clean ELF=ADMA2 IP=MMC1
make clean ELF=NONDMA ARMv8_STATE=32 IP=MMC0
make clean ELF=SDMA ARMv8_STATE=32 IP=MMC0
make clean ELF=ADMA2 ARMv8_STATE=32 IP=MMC0
make clean ELF=NONDMA ARMv8_STATE=32 IP=MMC1
make clean ELF=SDMA ARMv8_STATE=32 IP=MMC1
make clean ELF=ADMA2 ARMv8_STATE=32 IP=MMC1
make clean CHIP=dr1v90 ELF=NONDMA IP=MMC0
make clean CHIP=dr1v90 ELF=SDMA IP=MMC0
make clean CHIP=dr1v90 ELF=ADMA2 IP=MMC0
make clean CHIP=dr1v90 ELF=NONDMA IP=MMC1
make clean CHIP=dr1v90 ELF=SDMA IP=MMC1
make clean CHIP=dr1v90 ELF=ADMA2 IP=MMC1
clear
make ELF=NONDMA -j8 dasm IP=MMC0
clear
make ELF=SDMA -j8 dasm IP=MMC0
clear
make ELF=ADMA2 -j8 dasm IP=MMC0
clear
make ELF=NONDMA -j8 dasm IP=MMC1
clear
make ELF=SDMA -j8 dasm IP=MMC1
clear
make ELF=ADMA2 -j8 dasm IP=MMC1
clear
make ELF=NONDMA ARMv8_STATE=32 IP=MMC0 -j8 dasm
clear
make ELF=SDMA ARMv8_STATE=32 IP=MMC0 -j8 dasm
clear
make ELF=ADMA2 ARMv8_STATE=32 IP=MMC0 -j8 dasm
clear
make ELF=NONDMA ARMv8_STATE=32 IP=MMC1 -j8 dasm
clear
make ELF=SDMA ARMv8_STATE=32 IP=MMC1 -j8 dasm
clear
make ELF=ADMA2 ARMv8_STATE=32 IP=MMC1 -j8 dasm
clear
make CHIP=dr1v90 ELF=NONDMA -j8 dasm IP=MMC0
clear
make CHIP=dr1v90 ELF=SDMA -j8 dasm IP=MMC0
clear
make CHIP=dr1v90 ELF=ADMA2 -j8 dasm IP=MMC0
clear
make CHIP=dr1v90 ELF=NONDMA -j8 dasm IP=MMC1
clear
make CHIP=dr1v90 ELF=SDMA -j8 dasm IP=MMC1
clear
make CHIP=dr1v90 ELF=ADMA2 -j8 dasm IP=MMC1
clear
