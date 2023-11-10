#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################

import os
import subprocess
import sys
from pathlib import Path

from loguru import logger

APU_TOOLCHAIN_PATH = os.getenv('APU_TOOLCHAIN_PATH')
RPU_TOOLCHAIN_PATH = os.getenv('RPU_TOOLCHAIN_PATH')

def check_string(string, sdk_root):
    ''' find out string "string" '''

    logger.info(f'======> Start checking the strings: {str(string)}', colorize=True, format="<green>{time}</green> <level>{message}</level>")

    try:
        ret = subprocess.run(f'grep -ri {string} --exclude-dir=.git --exclude=.gitlab-ci.yml', shell=True, capture_output=True, cwd=sdk_root, check=True)

        result = bytes.decode(ret.stdout)
        if string in result:
            count = result.count(result)
            if count != 1:
                logger.error(f'======> Character found. \r\r {result}\r', colorize=True, format="<red>{time}</red> <level>{message}</level>")
                exit(1)
            else:
                logger.info(f'======> Character not found\n', colorize=True, format="<green>{time}</green> <level>{message}</level>")
        else:
            logger.info(f'======> Character not found\n', colorize=True, format="<green>{time}</green> <level>{message}</level>")
    except subprocess.CalledProcessError as e:
        logger.error(f'\n\n{bytes.decode(e.stderr)}')

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

            if 'freertos' in str(makefile_p) and chip == 'dr1v90':
                continue

            if 'msc_rtthread' in str(makefile_p) and chip == 'dr1m90':
                continue

            if 'openamp_rpmsg_echo' in str(makefile_p) and chip == 'dr1v90':
                continue

            if 'NMSIS' in str(makefile_p) and chip == 'dr1m90':
                continue

            if not str(makefile_p.parent).endswith('baremetal') and 'NMSIS' in str(makefile_p):
                continue

            if 'dr1v90_mmu' in str(makefile_p):
                chip = 'dr1v90'
                COMPILE_PREFIX = RPU_TOOLCHAIN_PATH + '/riscv-nuclei-elf-'

            logger.info(f'======> start make project: {str(makefile_p)}  -- chip: {chip} -- download: {download}', colorize=True, format="<green>{time}</green> <level>{message}</level>")

            try:
                subprocess.run(f'make clean', shell=True, capture_output=CAPTURE_OUTPUT, cwd=makefile_p.parent, check=True)
                subprocess.run(f'git clean -xfd', shell=True, capture_output=CAPTURE_OUTPUT, cwd=sdk_root, check=True)
                subprocess.run(f'make CHIP={chip} DOWNLOAD={download} COMPILE_PREFIX={COMPILE_PREFIX}', shell=True, capture_output=CAPTURE_OUTPUT, cwd=makefile_p.parent, check=True)

                logger.info(f'======> make successful\n', colorize=True, format="<green>{time}</green> <level>{message}</level>")
            except subprocess.CalledProcessError as e:
                build_pass = False
                logger.error(f'======> make filed {str(makefile_p)}\r', colorize=True, format="<red>{time}</red> <level>{message}</level>")
                logger.error(f'\n\n{bytes.decode(e.stderr)}')

    if build_pass is not True:
        exit(1)

def check_file_permissions(check_path):

    paths = Path(check_path).rglob('*')
    for path in paths:
        if path.is_file():
            path.chmod(644)
        if path.is_dir():
            path.chmod(755)

def main(path, chip, download, sdk_root, debug, check_s):
    if check_s == 'true':
        check_string(string='AL9000', sdk_root=sdk_root)

    else:
        check_file_permissions(check_path=sdk_root)

        make_all(path, chip, download, sdk_root, debug)


if __name__ == '__main__':
    path = sys.argv[1]
    chip = sys.argv[2]
    download = sys.argv[3]
    sdk_root = sys.argv[4]
    debug = sys.argv[5]
    check_s = sys.argv[6]
    main(path, chip, download, sdk_root, debug, check_s)
