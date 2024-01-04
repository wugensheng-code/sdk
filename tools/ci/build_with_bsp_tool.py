#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################


import argparse
import os
import subprocess
import json
from loguru import logger
from pathlib import Path


env_k = ['BSP_RESOURCE_PATH', 'AARCH64_TOOLCHAIN_PATH', 'RISCV_TOOLCHAIN_PATH']
AARCH64_TOOLCHAIN_PATH='/opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin'
RISCV_TOOLCHAIN_PATH='/opt/toolchain/riscv-gcc/bin'


class Bsp_tool(object):

    def __init__(self, bsp_tool_p, **kwargs):
        ''' The key contained in the kwargs is in envk'''

        super(Bsp_tool, self).__init__()
        self.env = dict.fromkeys(env_k)

        try:
            self.env['BSP_RESOURCE_PATH'] = os.path.abspath(kwargs['BSP_RESOURCE_PATH'])
            self.env['AARCH64_TOOLCHAIN_PATH'] = os.path.abspath(AARCH64_TOOLCHAIN_PATH)
            self.env['RISCV_TOOLCHAIN_PATH'] = os.path.abspath(RISCV_TOOLCHAIN_PATH)

            self.bsp_tool_p = Path(bsp_tool_p)
            if not self.bsp_tool_p.exists():
                raise FileNotFoundError(f'bsp_tool: {self.bsp_tool_p} not found')
        except Exception as e:
            logger.error(f'======> make failed {e}')
            exit(1)

    def setup_env(self):
        ''' set environment variables '''

        for k, v in self.env.items():
            os.putenv(k, v)
            PATH = os.getenv('PATH')
            PATH + f':{v}'
            os.putenv('PATH', PATH)

    def create_bsp(self, proj_name, location, proc_type, os_type, hpf_path):
        ''' create bsp '''

        self.location = location

        try:
            logger.info(f'======> Start creating bsp: project name: {proj_name} os type: {os_type}')
            subprocess.run(f'./anlogic_tool bsp_tool create_platform_project -projName {proj_name} -location {self.location} -proc {proc_type} -os {os_type} -hpf {hpf_path}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True)
            subprocess.run(f'./anlogic_tool bsp_tool create_default_mss -mssfile {self.location}/{proj_name}/system.mss -proc {proc_type} -os {os_type}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True)
            subprocess.run(f'./anlogic_tool bsp_tool generate_bsp_sources -hpf {hpf_path} -mssfile {self.location}/{proj_name}/system.mss -dir {self.location}/{proj_name}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True)
            logger.info(f'======> Platform created successfully: {proj_name}')
        except Exception as e:
            logger.error(f'======> create_bsp failed. {e}')
            exit(1)

        self.bsp_location = f'{os.getcwd()}/{proj_name}'

    def create_app_and_make(self, proj_name, app_name, bspLoc):
        ''' create bsp '''

        bspLoc = f'{os.getcwd()}/{bspLoc}'
        try:
            logger.info(f'======> Start creating app: {app_name}')
            subprocess.run(f'./anlogic_tool bsp_tool create_application_project -projName {proj_name} -location {os.getcwd()} -language C -bsp_loc {self.bsp_location}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True)
            subprocess.run(f'./anlogic_tool bsp_tool generate_app_sources -bsp_loc {bspLoc} -name {app_name} -dir {self.location}/{proj_name}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True)
            logger.info(f'======> App created successfully: {app_name}')
            logger.info(f'======> Start make project: {app_name}')

            subprocess.run(f'make -j8',
                shell=True, capture_output=True, cwd=f'{self.location}/{proj_name}', check=True)

        except Exception as e:
            logger.error(f'======> create app and run failed {e}')
            return e.cmd



def main():
    ''' main function '''

    parser = argparse.ArgumentParser(
        prog='build_with_bsp_tool',
        description='Using bsp_tool build master_sdk Engineering.',
        epilog='Copyright(r), 2023'
    )

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-A', '--all', help='build all project', action='store_true', dest='build_all')
    parser.add_argument('-t', '--bsp_tool', help='bsp_tool path')
    parser.add_argument('-b', '--bsp_resource_path', help='master_sdk path')
    parser.add_argument('-p', '--hpf', help='hpf path')

    args = parser.parse_args()

    os_type = 'standalone'

    bsp_tool = Bsp_tool(bsp_tool_p=args.bsp_tool, BSP_RESOURCE_PATH=args.bsp_resource_path, AARCH64_TOOLCHAIN_PATH=os.path.abspath(AARCH64_TOOLCHAIN_PATH), RISCV_TOOLCHAIN_PATH=os.path.abspath(RISCV_TOOLCHAIN_PATH))
    bsp_tool.setup_env()
    bsp_tool.create_bsp(proj_name=f'test_platform_standalone', location=os.getcwd(), proc_type='apu-0', os_type='standalone', hpf_path=args.hpf)
    bsp_tool.create_bsp(proj_name=f'test_platform_freertos', location=os.getcwd(), proc_type='apu-0', os_type='freertos', hpf_path=args.hpf)
    bsp_tool.create_bsp(proj_name=f'test_platform_rtthread', location=os.getcwd(), proc_type='apu-0', os_type='rtthread', hpf_path=args.hpf)

    statistics = list()
    with open(f'{args.bsp_resource_path}/docs/depend.json') as f:
        depend = json.load(f)
        for i in depend['app'].values():
            os_type = i['1.0']['supportedOS'][0]
            k = i['1.0']['name']
            ret = bsp_tool.create_app_and_make(proj_name=f'test_app_{k}', app_name=f'{k}', bspLoc=f'test_platform_{os_type}')
            statistics.append({k:ret})


        for i in depend['ps_driver_app'].values():
            for c in i['1.0'].values():
                os_type = c['supportedOS'][0]
                k = c['name']
                ret = bsp_tool.create_app_and_make(proj_name=f'test_app_{k}', app_name=f'{k}', bspLoc=f'test_platform_{os_type}')
                statistics.append({k:ret})


    logger.info(f'\r\n==============================> Statistics <===============================')

    ret = None
    for i in statistics:
        for k, v in i.items():
            if v is None:
                ret = 'Scucess'
                print(f"| {k:<40} |\033[32m{ret:^30}\033[0m |")
            else:
                ret = 'Fail: '+ str(v)
                print(f"| {k:<40} |\033[31m{ret:^30}\033[0m |")
                exit(1)


if __name__ == '__main__':
    main()
