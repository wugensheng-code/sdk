# Embedded-sw for fpsoc

---
### Directory Structure

Here is the directory structure.

├── docs\
├── fpsoc\
│   ├── arch\
│   │   ├── arm\
│   │   │   ├── armv8\
│   │   │   └── common\
│   │   └── riscv\
│   │   │   └── rv64\
│   │   └── common \
│   ├── chip\
│   │   └── dr1x90 \
│   ├── driver\
│   │   ├── pl_driver \
│   │   └── ps_driver \
│   │&emsp;&emsp;&ensp;└── uart\
│   │&emsp;&emsp;&emsp;&emsp;  ├── example\
│   │&emsp;&emsp;&emsp;&emsp;  ├── inc\
│   │&emsp;&emsp;&emsp;&emsp;  └── src\
│   └── lib\
│&emsp;&emsp;&nbsp;&nbsp;&nbsp;└── osal\
│&emsp;&emsp;&nbsp;&nbsp;&nbsp;└── cipher\
│&emsp;&emsp;&nbsp;&nbsp;&nbsp;└── cli\
│&emsp;&emsp;&nbsp;&nbsp;&nbsp;└── log\
│&emsp;&emsp;&nbsp;&nbsp;&nbsp;└── newlib\
│&emsp;&emsp;&nbsp;&nbsp;&nbsp;└── npu_runtime\
├── license.txt\
├── solutions\
│   ├── board\
│   │   ├── board_dr1x90_ad101_v10\
│   │   └── board_dr1x90_emulation\
│   └── demo\
└── third_party \
&emsp;&nbsp;├── lwip\
&emsp;&nbsp;├── lib\
&emsp;&emsp;&nbsp;&nbsp;&nbsp; ├── CherryUSB\
&emsp;&emsp;&nbsp;&nbsp;&nbsp; ├── jpu_driver\
&emsp;&emsp;&nbsp;&nbsp;&nbsp; ├── libmetal\
&emsp;&emsp;&nbsp;&nbsp;&nbsp; ├── open-amp\
&emsp;&emsp;&nbsp;&nbsp;&nbsp; └── FATFS\
&emsp;&nbsp;└── os\
&emsp;&emsp;&nbsp;&nbsp;&nbsp; ├── freertos\
&emsp;&emsp;&nbsp;&nbsp;&nbsp; └── rtthread

---
### How to build

For example :

- Download toolchain

    Please download the [arm](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)    or [riscv](https://www.nucleisys.com/download.php) toolchain first.

- build

    ```shell
    cd ./solutions/demo/baremetal/helloworld
    make
    ```

- Modify the compilation configuration

    All configurable variables and default values are in the config.mk, if you need to modify them, please modify the variables in the makefile of the current project.
