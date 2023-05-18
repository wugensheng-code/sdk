FROM ubuntu:20.04

ARG WORKSPACE=/workspace

ENV SHELL=/bin/bash

WORKDIR ${WORKSPACE}

#system
RUN apt-get update

RUN apt-get upgrade -y

RUN apt-get install -y vim git wget python3 python-is-python3 pip make sudo

RUN pip3 install loguru Commitizen

RUN mkdir /opt/toolchain/ \
    && wget -nv -c https://armkeil.blob.core.windows.net/developer/Files/downloads/gnu/11.2-2022.02/binrel/gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf.tar.xz \
    && tar -xf gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf.tar.xz -C /opt/toolchain \
    && rm -f gcc-arm-11.2-2022.02-x86_64-aarch64-none-elf.tar.xz \
    && wget -nv -c https://nucleisys.com/upload/files/toochain/gcc/nuclei_riscv_newlibc_prebuilt_linux64_2022.12.tar.bz2 \
    && tar -xjf nuclei_riscv_newlibc_prebuilt_linux64_2022.12.tar.bz2 -C /opt/toolchain \
    && rm -f nuclei_riscv_newlibc_prebuilt_linux64_2022.12.tar.bz2 \
    && mv /opt/toolchain/gcc /opt/toolchain/riscv-gcc \
    && chmod -R a+x /opt/toolchain/
