import subprocess
import sys
from pathlib import Path

from loguru import logger

def make_all(path, argv):
   
    makefiles_p = Path(path).rglob('Makefile')
    build_pass = True

    for makefile_p in makefiles_p:
        if makefile_p.is_file():
            logger.info(f'======> start make project: {str(makefile_p)}', colorize=True, format="<green>{time}</green> <level>{message}</level>")

            try:
                subprocess.run(f'make {argv}', shell=True, capture_output=True, cwd=makefile_p.parent, check=True)
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
    argv = sys.argv[2]
    make_all(path, argv)
