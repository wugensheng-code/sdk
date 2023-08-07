import os
import subprocess
import sys
from pathlib import Path

from loguru import logger

APU_TOOLCHAIN_PATH = os.getenv('APU_TOOLCHAIN_PATH')
RPU_TOOLCHAIN_PATH = os.getenv('RPU_TOOLCHAIN_PATH')

def make_all(path, chip, download, sdk_root, debug):

    makefiles_p = Path(path).rglob('Makefile')
    build_pass = True
    CAPTURE_OUTPUT = True
    
    logger.info(f'======> Debug enable: {debug}', colorize=True, format="<green>{time}</green> <level>{message}</level>")

    if debug == 'true':
        CAPTURE_OUTPUT = False

    if chip == 'dr1m90':
        COMPILE_PREFIX = APU_TOOLCHAIN_PATH + '/aarch64-none-elf-'
    if chip == 'dr1v90':
        COMPILE_PREFIX = RPU_TOOLCHAIN_PATH + '/riscv-nuclei-elf-'

    for makefile_p in makefiles_p:
        if makefile_p.is_file():
            if 'rt-thread' in str(makefile_p) and chip == 'dr1m90':
                continue

            if 'NMSIS' in str(makefile_p) and chip == 'dr1m90':
                continue

            if not str(makefile_p.parent).endswith('baremetal'):
                continue
            
            if 'dr1v90_mmu' in str(makefile_p):
                chip = 'dr1v90'
                COMPILE_PREFIX = RPU_TOOLCHAIN_PATH + '/riscv-nuclei-elf-'

            logger.info(f'======> start make project: {str(makefile_p)}  -- chip: {chip} -- download: {download}', colorize=True, format="<green>{time}</green> <level>{message}</level>")

            try:
                subprocess.run(f'make clean', shell=True, capture_output=CAPTURE_OUTPUT, cwd=makefile_p.parent, check=True)
                subprocess.run(f'git clean -xfd', shell=True, capture_output=CAPTURE_OUTPUT, cwd=sdk_root, check=True)
                subprocess.run(f'make CHIP={chip} DOWNLOAD={download} COMPILE_PREFIX={COMPILE_PREFIX} -j8', shell=True, capture_output=CAPTURE_OUTPUT, cwd=makefile_p.parent, check=True)

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
    chip = sys.argv[2]
    download = sys.argv[3]
    sdk_root = sys.argv[4]
    debug = sys.argv[5]
    make_all(path, chip, download, sdk_root, debug)
