set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

set(CMAKE_C_COMPILER riscv-nuclei-elf-gcc)
set(CMAKE_CXX_COMPILER riscv-nuclei-elf-g++)
set(CMAKE_ASM_COMPILER riscv-nuclei-elf-gcc)



ifeq ($(NOGC),1)
GC_CFLAGS =
GC_LDFLAGS =
else
GC_CFLAGS = -ffunction-sections -fdata-sections
GC_LDFLAGS = -Wl,--gc-sections -Wl,--check-sections
endif


set(CMAKE_C_FLAGS "-mlongcalls -Wno-frame-address" CACHE STRING "C Compiler Base Flags")
set(CMAKE_CXX_FLAGS "-mlongcalls -Wno-frame-address" CACHE STRING "C++ Compiler Base Flags")

set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -Wl,--gc-sections" CACHE STRING "Linker Base Flags")
