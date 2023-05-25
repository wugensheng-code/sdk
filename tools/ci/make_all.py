import subprocess
import sys
from pathlib import Path

from loguru import logger

def make_all(path, cpu):
   
    makefiles_p = Path(path).rglob('Makefile')
    build_pass = True

    for makefile_p in makefiles_p:
        if makefile_p.is_file():
            logger.info(f'======> start make project: {str(makefile_p)}', colorize=True, format="<green>{time}</green> <level>{message}</level>")

            try:
                if cpu == 'apu':
                    subprocess.run(f'make \
                        SOC=demoapu \
                        COMPILE_PREFIX=/opt/toolchain/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf/bin/aarch64-none-elf- \
                        DOWNLOAD=ilm', shell=True, capture_output=True, cwd=makefile_p.parent, check=True)
                else:
                    subprocess.run(f'make \
                        SOC=demosoc \
                        COMPILE_PREFIX=/opt/toolchain/riscv-gcc/bin/riscv-nuclei-elf- \
                        CORE=ux600fd \
                        DOWNLOAD=ilm', shell=True, capture_output=True, cwd=makefile_p.parent, check=True)

                logger.info(f'======> make successful\n', colorize=True, format="<green>{time}</green> <level>{message}</level>")
            except subprocess.CalledProcessError as e:
                build_pass = False               
                logger.error(f'======> make filed {str(makefile_p)}\r', colorize=True, format="<red>{time}</red> <level>{message}</level>")
                logger.error(f'\n\n{bytes.decode(e.stderr)}')
                bytearray
    
    if build_pass is not True:
        exit(1)


if __name__ == '__main__':
    path = sys.argv[1]
    cpu = sys.argv[2]
    make_all(path, cpu)
