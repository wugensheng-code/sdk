#########################################################################
# Copyright (c) 2023, Anlogic Inc. and Contributors. All rights reserved.

# SPDX-License-Identifier: BSD-3-Clause
#########################################################################


import argparse
import os
import subprocess
import json
import logging
from copy import deepcopy
from contextlib import AbstractContextManager, contextmanager, ExitStack
from uuid import uuid1
from pathlib import Path
from shutil import copytree, rmtree


env_k = ['BSP_RESOURCE_PATH', 'AARCH64_TOOLCHAIN_PATH', 'RISCV_TOOLCHAIN_PATH']
AARCH64_TOOLCHAIN_PATH='/opt/toolchain/arm-gnu-toolchain-12.3.rel1-x86_64-aarch64-none-elf/bin'
ARM_TOOLCHAIN_PATH='/opt/toolchain/13.2/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin'
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

'''       key = ['uuid',    'bsp_proj_name',    'app_proj_name',    'location',    'chip',    'proc_type',    'os_type',    'hpf_path',               'ext',    'result']        '''
bsp_template = [
                [None,       None,               None,               None,         'dr1m90',  'apu-0',       'standalone',   'ad101_v10.hpf',          64,       None],
                [None,       None,               None,               None,         'dr1m90',  'apu-0',       'standalone',   'ad101_v10.hpf',          32,       None],
                [None,       None,               None,               None,         'dr1m90',  'apu-0',       'freertos',     'ad101_v10.hpf',          None,     None],
                [None,       None,               None,               None,         'dr1m90',  'apu-0',       'rtthread',     'ad101_v10.hpf',          None,     None],
                [None,       None,               None,               None,         'dr1v90',  'rpu',         'standalone',   'ad102_v10.hpf',          None,     None],
                [None,       None,               None,               None,         'dr1v90',  'rpu',         'freertos',     'ad102_v10.hpf',          None,     None],
                [None,       None,               None,               None,         'dr1v90',  'rpu',         'rtthread',     'ad102_v10.hpf',          None,     None]
]


class Bsp_tool(AbstractContextManager):

    def __init__(self, bsp_tool_p, **kwargs):
        ''' The key contained in the kwargs is in envk'''

        super().__init__()
        self.env = dict.fromkeys(env_k)

        try:
            self.env['BSP_RESOURCE_PATH'] = Path(kwargs['BSP_RESOURCE_PATH']).absolute()
            self.env['AARCH64_TOOLCHAIN_PATH'] = Path(AARCH64_TOOLCHAIN_PATH).absolute()
            self.env['RISCV_TOOLCHAIN_PATH'] = Path(RISCV_TOOLCHAIN_PATH).absolute()
            self.bsp_tool_p = Path(bsp_tool_p)
            self.log_path = self.env['BSP_RESOURCE_PATH'].joinpath('log')

        except Exception as e:
            logger.error(f'======> make failed {e}')
            exit(1)

    @contextmanager
    def _cleanup_on_error(self):
        with ExitStack() as stack:
            stack.push(self)
            yield
            # The validation check passed and didn't raise an exception
            # Accordingly, we want to keep the resource, and pass it
            # back to our caller
            stack.pop_all()

    def __enter__(self):

        with self._cleanup_on_error():
            if not self.bsp_tool_p.exists():
                raise RuntimeError(f'bsp_tool: {self.bsp_tool_p} not found')
            self._setup_env()
            self.log_path.mkdir()
        self._create_table()
        return self

    def __exit__(self, *exc_details):

        if exc_details is not None:
            for i in exc_details:
                if i is not None:
                    logger.error(f'{i}')

    def _setup_env(self):
        ''' set environment variables '''

        self.environ = os.environ.copy()

        for k, v in self.env.items():
            os.environ[k] = str(v) + '/'
            PATH = os.getenv('PATH')
            PATH + f':{v}'
            os.environ['PATH'] = PATH
            self.environ[k] = v
            self.environ['PATH'] = PATH

    def _create_table(self):
        ''' Create project information table '''

        self.table_key = ['uuid', 'bsp_proj_name', 'app_proj_name', 'location', 'chip', 'proc_type', 'os_type', 'hpf_path', 'ext', 'result']
        self.table = list()
        self.template = bsp_template

    def create_bsp(self, location):
        """create bsp"""

        bsp_names = list()

        for i in self.template:
            bsp_name = "bsp"
            for j in i:
                if j is not None:
                    bsp_name = f"{bsp_name}_{j}"

            bsp_names.append(bsp_name)

        for index, name in enumerate(bsp_names):
            self.template[index][1] = name

        try:

            for entry in self.template:
                logger.info(
                    f"======> Start creating bsp: project name: {entry[1]} chip: {entry[4]} proc_type: {entry[5]} os_type: {entry[6]}"
                )

                ret = subprocess.run(
                    f"{self.bsp_tool_p} dr1x90_tool create_platform_project -projName {entry[1]} -location {location} -chip {entry[4]} -proc {entry[5]} -os {entry[6]} -hpf {entry[7]}",
                    shell=True,
                    capture_output=True,
                    cwd=self.bsp_tool_p.parent,
                    check=True,
                    text=True,
                )

                ret = subprocess.run(
                    f"{self.bsp_tool_p} dr1x90_tool create_default_mss -mssfile {location}/{entry[1]}/system.mss -chip {entry[4]} -proc {entry[5]} -os {entry[6]}",
                    shell=True,
                    capture_output=True,
                    cwd=self.bsp_tool_p.parent,
                    check=True,
                    text=True,
                )

                ret = subprocess.run(
                    f"{self.bsp_tool_p} dr1x90_tool generate_bsp_sources -hpf {entry[7]} -mssfile {location}/{entry[1]}/system.mss -bsp_loc {location}/{entry[1]}",
                    shell=True,
                    capture_output=True,
                    cwd=self.bsp_tool_p.parent,
                    check=True,
                    text=True,
                )

                entry[3] = location
                logger.info(f"======> Platform created successfully: {entry[1]}")

        except Exception as e:
            logger.error(f"======> create_bsp failed. {e}")
            logger.error(e.stderr)
            logger.error(e.stdout)
            exit(1)

    def create_app_and_make(self, app_name, os_type, proc_type):
        ''' create app '''

        entry = None
        bsp_type = None

        try:
            for index, bsp_content in enumerate(self.template):
                if bsp_content[5] == proc_type and bsp_content[6] == os_type:
                    bsp_type = index

            entry = dict(zip(self.table_key, self.template[bsp_type]))
            entry['uuid'] = uuid1()
            ext = ''
            if entry['ext'] is not None:
                ext = '_{ext}'.format(ext=entry['ext'])

            entry['app_proj_name'] = 'app_{app_name}_{proc_type}_{os_type}{ext_info}'.format(app_name=app_name, proc_type=entry['proc_type'], os_type=entry['os_type'], ext_info=ext)

            subprocess.run(f"{self.bsp_tool_p} dr1x90_tool create_application_project -projName {entry['app_proj_name']} -location {os.getcwd()} -language C -bsp_loc {entry['location']}/{entry['bsp_proj_name']}",
                shell=True, capture_output=True, cwd=self.bsp_tool_p.parent, check=True, text=True)
            subprocess.run(f"{self.bsp_tool_p} dr1x90_tool generate_app_sources -bsp_loc {entry['location']}/{entry['bsp_proj_name']} -name {app_name} -app_loc {entry['location']}/{entry['app_proj_name']}",
                shell=True, capture_output=True, cwd=self.bsp_tool_p.parent, check=True, text=True)
            logger.info(f'======> App created successfully: {app_name}')

            def make_rpu():
                COMPILE_PREFIX = RISCV_TOOLCHAIN_PATH + "/riscv-nuclei-elf-"
                if "rpc" not in entry["app_proj_name"]:
                    subprocess.run(
                        f"make -j8 COMPILE_PREFIX={COMPILE_PREFIX} WITH_PROXY=1 SDK_ROOT={entry['location']}/{entry['bsp_proj_name']} LINKER_SCRIPT={entry['location']}/{entry['bsp_proj_name']}/dr1v90_fpsoc_cpu0/chip/dr1x90/dr1v90/lds/gcc_dr1v90_ddr_lp64d.ld",
                        shell=True,
                        capture_output=True,
                        cwd=f"{entry['location']}/{entry['app_proj_name']}",
                        check=True,
                        text=True,
                    )
                    entry["result"] = "pass"
                else:
                    entry["result"] = "N/A "

            def make_apu_32():
                COMPILE_PREFIX = ARM_TOOLCHAIN_PATH + "/arm-none-eabi-"
                if "standalone" in entry["os_type"] and 'FSBL' not in entry["app_proj_name"] and 'OPENAMP' not in entry["app_proj_name"]:
                    subprocess.run(
                        f"make -j8 COMPILE_PREFIX={COMPILE_PREFIX} WITH_PROXY=1 SDK_ROOT={entry['location']}/{entry['bsp_proj_name']} ARMv8_STATE=32 LINKER_SCRIPT={entry['location']}/{entry['bsp_proj_name']}/dr1m90_fpsoc_cpu0/chip/dr1x90/dr1m90/lds/gcc_dr1m90_ddr_aarch32.ld",
                        shell=True,
                        capture_output=True,
                        cwd=f"{entry['location']}/{entry['app_proj_name']}",
                        check=True,
                        text=True,
                    )
                    entry["result"] = "pass"
                else:
                    entry["result"] = "N/A "

            def make_apu_64():
                COMPILE_PREFIX = AARCH64_TOOLCHAIN_PATH + "/aarch64-none-elf-"
                subprocess.run(
                    f"make -j8 COMPILE_PREFIX={COMPILE_PREFIX} WITH_PROXY=1 SDK_ROOT={entry['location']}/{entry['bsp_proj_name']} ARMv8_STATE=64 LINKER_SCRIPT={entry['location']}/{entry['bsp_proj_name']}/dr1m90_fpsoc_cpu0/chip/dr1x90/dr1m90/lds/gcc_dr1m90_ddr_aarch64.ld",
                    shell=True,
                    capture_output=True,
                    cwd=f"{entry['location']}/{entry['app_proj_name']}",
                    check=True,
                    text=True,
                )
                entry["result"] = "pass"
            

            logger.info(f"======> Start make project: {entry['app_proj_name']}")

            if 'rpu' in entry['proc_type']:
                make_rpu()
            if 'apu' in entry['proc_type'] and 32 != entry['ext']:
                make_apu_64()
            if 'apu' in entry['proc_type'] and 32 == entry['ext']:
                make_apu_32()
            
            self.table.append(entry)

        except subprocess.CalledProcessError as e:
            logger.error(e.stderr)
            faild_prj_app = Path(self.env['BSP_RESOURCE_PATH']).joinpath('log').joinpath(proc_type).joinpath(f"{entry['app_proj_name']}")
            faild_prj_app.mkdir(parents=True)
            faild_prj_bsp = Path(self.env['BSP_RESOURCE_PATH']).joinpath('log').joinpath(proc_type).joinpath(f'{os_type}')
            copytree(src=f"{entry['location']}/{entry['app_proj_name']}", dst=faild_prj_app, ignore=None, dirs_exist_ok=True)
            copytree(src=f"{entry['location']}/{entry['bsp_proj_name']}", dst=faild_prj_bsp, ignore=None, dirs_exist_ok=True)
            entry["result"] = f'failed: {e.cmd}'
            self.table.append(entry)
            return
        except Exception as e:
            logger.error(f'======> create app and run failed {e}')


def main():
    """main function"""

    parser = argparse.ArgumentParser(
        prog="build_with_bsp_tool",
        description="Using bsp_tool build master_sdk Engineering.",
        epilog="Copyright(r), 2023",
    )

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "-A", "--all", help="build all project", action="store_true", dest="build_all"
    )
    parser.add_argument("-t", "--bsp_tool", help="bsp_tool path")
    parser.add_argument("-b", "--bsp_resource_path", help="master_sdk path")

    args = parser.parse_args()

    proc_types = {'apu-0': 'dr1x90.hpf', 'rpu': 'dr1x90.hpf'}

    bsp_tool = Bsp_tool(
        bsp_tool_p=args.bsp_tool,
        BSP_RESOURCE_PATH=args.bsp_resource_path,
        AARCH64_TOOLCHAIN_PATH=os.path.abspath(AARCH64_TOOLCHAIN_PATH),
        RISCV_TOOLCHAIN_PATH=os.path.abspath(RISCV_TOOLCHAIN_PATH),
    )

    with Bsp_tool(
        bsp_tool_p=args.bsp_tool,
        BSP_RESOURCE_PATH=args.bsp_resource_path,
        AARCH64_TOOLCHAIN_PATH=os.path.abspath(AARCH64_TOOLCHAIN_PATH),
        RISCV_TOOLCHAIN_PATH=os.path.abspath(RISCV_TOOLCHAIN_PATH),
    ) as bsp_tool:
        bsp_tool

        bsp_tool.create_bsp(location=os.getcwd())

        with open(f'{args.bsp_resource_path}/docs/depend.json') as f:
            depend = json.load(f)

            for proc_type in proc_types.keys():

                for i in depend['app'].values():
                    os_type = i['1.0']['supportedOS'][0]
                    k = i['1.0']['name']
                    if ('DEMO_OPENAMP' in k and 'rpu' in proc_type):
                        pass
                    else:
                        bsp_tool.create_app_and_make(app_name=f'{k}', os_type=os_type, proc_type=proc_type)


                for i in depend['ps_driver_app'].values():
                    for c in i['1.0'].values():
                        os_type = c['supportedOS'][0]
                        k = c['name']
                        bsp_tool.create_app_and_make(app_name=f'{k}', os_type=os_type, proc_type=proc_type)

        Fail = False
        try:
            logger.info(f'\r\n============================> Statistics <============================')
            for i in bsp_tool.table:
                if i is not None:
                    if i['uuid'] is not None:
                        for k, v in i.items():
                            if 'app_proj_name' == k:
                                app_proj_name = v;
                                continue;
                            if k == 'result':
                                if'failed' in v:
                                    logger.info(f"| {app_proj_name:<60} |\033[31m{v:^40}\033[0m |")
                                    Fail = True
                                else:
                                    logger.info(f"| {app_proj_name:<60} |\033[32m{v:^40}\033[0m |")
        except Exception as e:
            logger.info(e)
            pass


        if Fail:
            exit(1)

if __name__ == '__main__':
    main()
