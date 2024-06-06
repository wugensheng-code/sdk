#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################


import argparse
import os
import subprocess
import json
import logging
from pathlib import Path
from shutil import copytree


env_k = ['BSP_RESOURCE_PATH', 'AARCH64_TOOLCHAIN_PATH', 'RISCV_TOOLCHAIN_PATH']
AARCH64_TOOLCHAIN_PATH='/opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin'
RISCV_TOOLCHAIN_PATH='/opt/toolchain/riscv-gcc/bin'


logger = logging.getLogger('logger')
logger.setLevel(logging.DEBUG)
stream_handler = logging.StreamHandler()
stream_handler.setLevel(logging.INFO)
logger.addHandler(stream_handler)
filter_ = logging.Filter('logger')
stream_handler.addFilter(filter_)
formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
stream_handler.setFormatter(formatter)


class Bsp_tool(object):

    def __init__(self, bsp_tool_p, **kwargs):
        ''' The key contained in the kwargs is in envk'''

        super(Bsp_tool, self).__init__()
        self.env = dict.fromkeys(env_k)

        try:
            self.env['BSP_RESOURCE_PATH'] = os.path.abspath(kwargs['BSP_RESOURCE_PATH']) + '/'
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

        self.environ = os.environ.copy()

        for k, v in self.env.items():
            os.environ[k] = v
            PATH = os.getenv('PATH')
            PATH + f':{v}'
            os.environ['PATH'] = PATH
            self.environ[k] = v
            self.environ['PATH'] = PATH
        

    def create_bsp(self, proj_name, location, chip,  proc_type, os_type, hpf_path):
        ''' create bsp '''

        self.location = location
        self.proc_type = proc_type
        self.os_type = os_type

        try:
            logger.info(f'======> Start creating bsp: project name: {proj_name} os type: {os_type}')
            ret = subprocess.run(f'{self.bsp_tool_p}/asct dr1x90_tool create_platform_project -projName {proj_name} -location {self.location} -chip {chip} -proc {proc_type} -os {os_type} -hpf {hpf_path}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True, text=True)

            ret = subprocess.run(f'{self.bsp_tool_p}/asct dr1x90_tool create_default_mss -mssfile {self.location}/{proj_name}/system.mss -chip {chip} -proc {proc_type} -os {os_type}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True, text=True)

            ret = subprocess.run(f'{self.bsp_tool_p}/asct dr1x90_tool generate_bsp_sources -hpf {hpf_path} -mssfile {self.location}/{proj_name}/system.mss -bsp_loc {self.location}/{proj_name}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True, text=True)

            logger.info(f'======> Platform created successfully: {proj_name}')
        except Exception as e:
            logger.error(f'======> create_bsp failed. {e}')
            logger.error(e.stderr)
            logger.error(e.stdout)
            exit(1)

        self.bsp_location = f'{os.getcwd()}/{proj_name}'

    def create_app_and_make(self, proj_name, app_name, bspLoc):
        ''' create bsp '''

        bspLoc = f'{os.getcwd()}/{bspLoc}'
        try:
            logger.info(f'======> Start creating app: {app_name}')
            subprocess.run(f'{self.bsp_tool_p}/asct dr1x90_tool create_application_project -projName {proj_name} -location {os.getcwd()} -language C -bsp_loc {self.bsp_location}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True, text=True, env=self.environ)
            subprocess.run(f'{self.bsp_tool_p}/asct dr1x90_tool generate_app_sources -bsp_loc {bspLoc} -name {app_name} -app_loc {self.location}/{proj_name}',
                shell=True, capture_output=True, cwd=self.bsp_tool_p, check=True, text=True, env=self.environ)
            logger.info(f'======> App created successfully: {app_name}')
            logger.info(f'======> Start make project: {app_name}')

            if self.proc_type == 'rpu':
                COMPILE_PREFIX = RISCV_TOOLCHAIN_PATH + '/riscv-nuclei-elf-'
            else:
                COMPILE_PREFIX = AARCH64_TOOLCHAIN_PATH + '/aarch64-none-elf-'

            if 'rpc' in app_name and self.proc_type != 'rpu':
                subprocess.run(f'make -j8 COMPILE_PREFIX={COMPILE_PREFIX} WITH_PROXY=1 SDK_ROOT={bspLoc}',
                    shell=True, capture_output=True, cwd=f'{self.location}/{proj_name}', check=True, text=True)
            elif 'rpc' in app_name and self.proc_type == 'rpu':
                return

            subprocess.run(f'make -j8 COMPILE_PREFIX={COMPILE_PREFIX} WITH_PROXY=1 SDK_ROOT={bspLoc}',
                shell=True, capture_output=True, cwd=f'{self.location}/{proj_name}', check=True, text=True)
        except subprocess.CalledProcessError as e:
            logger.error(e.stderr)
            faild_prj_app = Path(self.env['BSP_RESOURCE_PATH']).joinpath('log').joinpath(self.proc_type).joinpath(f'{proj_name}_{app_name}')
            faild_prj_app.mkdir(parents=True)
            faild_prj_bsp = Path(self.env['BSP_RESOURCE_PATH']).joinpath('log').joinpath(self.proc_type).joinpath(f'{self.os_type}')
            copytree(src=f'{self.location}/{proj_name}', dst=faild_prj_app, ignore=None, dirs_exist_ok=True)
            copytree(src=f'{bspLoc}', dst=faild_prj_bsp, ignore=None, dirs_exist_ok=True)
            return e.cmd
        except Exception as e:
            logger.error(f'======> create app and run failed {e}')




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

    args = parser.parse_args()

    # proc_types = {'rpu': 'demo_board_hpf_rpu.hpf', 'apu-0': 'demo_board_hpf_apu.hpf'}
    proc_types = {'apu-0': 'dr1x90.hpf', 'rpu': 'dr1x90.hpf'}

    Path(args.bsp_resource_path).joinpath('log').mkdir()

    bsp_tool = Bsp_tool(bsp_tool_p=args.bsp_tool, BSP_RESOURCE_PATH=args.bsp_resource_path, AARCH64_TOOLCHAIN_PATH=os.path.abspath(AARCH64_TOOLCHAIN_PATH), RISCV_TOOLCHAIN_PATH=os.path.abspath(RISCV_TOOLCHAIN_PATH))
    bsp_tool.setup_env()

    statistics = {'rpu': None, 'apu-0': None}

    for proc_type, hpf in proc_types.items():
        bsp_tool.create_bsp(proj_name=f'test_platform_standalone_{proc_type}', location=os.getcwd(), chip=('dr1v90' if proc_type == 'rpu' else 'dr1m90'), proc_type=proc_type, os_type='standalone', hpf_path=hpf)
        bsp_tool.create_bsp(proj_name=f'test_platform_freertos_{proc_type}', location=os.getcwd(), chip=('dr1v90' if proc_type == 'rpu' else 'dr1m90'), proc_type=proc_type, os_type='freertos', hpf_path=hpf)
        bsp_tool.create_bsp(proj_name=f'test_platform_rtthread_{proc_type}', location=os.getcwd(), chip=('dr1v90' if proc_type == 'rpu' else 'dr1m90'), proc_type=proc_type, os_type='rtthread', hpf_path=hpf)

        proc_statistics = list()
        with open(f'{args.bsp_resource_path}/docs/depend.json') as f:
            depend = json.load(f)
            for i in depend['app'].values():
                os_type = i['1.0']['supportedOS'][0]
                k = i['1.0']['name']
                if ('DEMO_OPENAMP' in k and 'rpu' in proc_type):
                    pass
                else:
                    ret = bsp_tool.create_app_and_make(proj_name=f'test_app_{proc_type}_{k}', app_name=f'{k}', bspLoc=f'test_platform_{os_type}_{proc_type}')
                    proc_statistics.append({k:ret})

            for i in depend['ps_driver_app'].values():
                for c in i['1.0'].values():
                    os_type = c['supportedOS'][0]
                    k = c['name']
                    ret = bsp_tool.create_app_and_make(proj_name=f'test_app_{proc_type}_{k}', app_name=f'{k}', bspLoc=f'test_platform_{os_type}_{proc_type}')
                    proc_statistics.append({k:ret})

        statistics[proc_type] = proc_statistics

    ret = None

    Fail = False

    try:
        for proc, proc_statistics in statistics.items():
            logger.info(f'\r\n============================> Statistics for {proc} <==========================')
            for i in proc_statistics:
                for k, v in i.items():
                    if v is None:
                        ret = 'Scucess'
                        logger.info(f"| {k:<40} |\033[32m{ret:^30}\033[0m |")
                    else:
                        ret = 'Fail: '+ str(v)
                        logger.info(f"| {k:<40} |\033[31m{ret:^30}\033[0m |")
                        Fail = True
    except Exception as e:
        pass


    if Fail:
        exit(1)


if __name__ == '__main__':
    main()
