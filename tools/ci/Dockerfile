FROM ubuntu:20.04

ARG WORKSPACE=/workspace

ENV SHELL=/bin/bash

WORKDIR ${WORKSPACE}

#system
RUN apt-get update

RUN apt-get upgrade -y

RUN apt-get install -y vim git wget python3 python-is-python3 pip make sudo

RUN pip3 install -i https://pypi.tuna.tsinghua.edu.cn/simple loguru Commitizen scons

RUN mkdir /opt/toolchain/ \
    && wget -nv -c https://nucleisys.com/upload/files/toochain/gcc/nuclei_riscv_newlibc_prebuilt_linux64_2022.12.tar.bz2 \
    && tar -xjf nuclei_riscv_newlibc_prebuilt_linux64_2022.12.tar.bz2 -C /opt/toolchain \
    && rm -f nuclei_riscv_newlibc_prebuilt_linux64_2022.12.tar.bz2 \
    && mv /opt/toolchain/gcc /opt/toolchain/riscv-gcc \
    && chmod -R a+x /opt/toolchain/

COPY ./arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf.tar.xz /opt/toolchain

RUN xz -d /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf.tar.xz \
    && tar -xf /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf.tar -C /opt/toolchain\
    && rm -f /opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf.tar \
    && chmod -R a+x /opt/toolchain/

ENV APU_TOOLCHAIN_PATH=/opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin
ENV RPU_TOOLCHAIN_PATH=/opt/toolchain/riscv-gcc/bin
