----------------------------------------------------
FSBL简介：
FSBL，全程First Stage Bootloader（第一阶段启动程序）。用于从非易失存储器（QSPI、SD、EMMC、NAND等）加载码流到PL部分，配置PL部分的逻辑功能，
以及加载第二阶段的启动程序（例如linux uboot）或裸机程序到可执行地址空间（如DDR，OCM等）并使其开始运行。
码流数据、裸机程序、二阶段启动程序可使用工具软件FD打包成启动镜像文件。
如若有安全需求，打包时对码流数据、裸机程序等内容指定是否进行加密和签名验证。
系统上电后，首先由CSU运行BootROM，BootROM加载FSBL到OCM，执行解密和签名验证（如有配置安全功能），
然后释放目标处理器的复位，使其开始运行FSBL。

----------------------------------------------------
如何在FD软件中创建FSBL：
1，	在TD中新建RTL工程，根据需要，配置时钟、输入输出、外设等参数；
2，	在TD中点击Project->Export Hardware Platform File，导出HPF配置文件（*.hpf）；
3，	在FD中点击File->New->Platform Project，导入刚才到处的HPF配置文件，填写一个工程名，选择需要运行的操作系统（Operating system，支持rtthread，freertos，Linux），如仅需使用裸机程序，不需操作系统，则选择standalone，选择Processor（apu-0或rpu），新建一个Platform工程；
4，	点击File->New->Application Project，在Plaform选项框中选择刚才新建的Platform工程，填写一个工程名，点击Finish，完成FSBL工程创建。

----------------------------------------------------
FSBL App Setting：
可通过App Setting配置某些功能，或关闭/开启部分功能。

配置方法：
在FD的Project Explorer窗口中右单击fsbl工程名，
在弹出菜单中点击Properties，
在弹出窗口中点击C/C++ Build->Settings，
选择Tool Settings标签页，
若使用的是DR1M90，进入GNU Arm Cross C Compiler项目下的Preprocessor标签页，
若使用的是DR1V90，进入GNU RISC-V Cross C Compiler项目下的Preprocessor标签页，
在Defined symbols项目框中可以看到App Setting的各个项目：

DOWNLOAD_MODE=ocm：  配置FSBL的加载和运行地址，必须为ocm；

ENABLE_MMU=1：  对于DR1M90，默认开启MMU，如需关闭，将此参数改为0；

ARMv8_EL=EL3：  若使用的是DR1M90，则必须将EL等级设置为EL3；

DDR_2M_MAPPING=0：  配置DDR Mapping选项，在FSBL编译时必须配置为0；

ALFSBL_WDT_EXCLUDE_VAL=0：  配置FSBL中是否开启WDT功能，默认开始，若需要不开启WDT，则将此参数改为1；

ALFSBL_PMU_EXCLUDE_VAL=0：  配置FSBL中是否开启PMU功能，开启后，若发生WDT超时错误、总线超时错误、OCM ECC校验错误，
                            则会触发系统复位。如果需要不开启PMU的这些功能，则将此参数改为1；

----------------------------------------------------
FSBL可在哪些处理器上运行？
DR1M90：FSBL运行在APU Core0（ARM-V8）；
DR1V90：FSBL运行在RPU（RISCV-64）；

----------------------------------------------------
FSBL的运行地址？
FSBL被加载到OCM运行，运行起始地址为0x61000000。

----------------------------------------------------
FSBL运行信息输出
FSBL运行过程中可通过串口输出信息供调试和参考，可通过AL_LOG_LEVEL配置输出信息的等级：
打开fpsoc/inc/al_utils_def.h文件，找到AL_LOG的宏定义，修改输出消息的LEVEL等级；
AL_LOG_LEVEL_DEBUG：	输出全部信息，包含调试用的信息
AL_LOG_LEVEL_INFO：	    输出全部信息，但不包含调试用的信息
AL_LOG_LEVEL_NOTICE：	仅输出较重要的信息
AL_LOG_LEVEL_WARNING：	仅输出警告信息
AL_LOG_LEVEL_ERROR：	仅输出错误信息
AL_LOG_LEVEL_CRIT：	    仅输出重要信息
AL_LOG_LEVEL_ALERT：	仅输出必须立即解决的错误信息
若配置输出更多的信息，信息输出将会消耗额外的启动时间。

----------------------------------------------------
FSBL的编译优化等级
FSBL默认的编译优化等级为-O2，亦可通过FD配置或修改Makefile，修改优化等级。

----------------------------------------------------
FSBL运行的地址空间
系统启动时，FSBL被加载至OCM中运行，地址空间为0x61000000~0x6103FFFF，总长度为256K字节。
另外，DDR的0x0~0xFFFFF地址空间（长度为1M字节），BootROM无法直接访问，因此建议这段地址空间保留不使用。

----------------------------------------------------
BIF文件（用于创建启动镜像）中对Partition是否有顺序要求？
在BIF文件中，第一个Partition必须为FSBL；如需加载PL码流，则必须放在第二个；MCU应用程序则依次向后排列。
需要注意的是，运行在执行FSBL的处理器核的程序，应当放在BIF文件中最后一个Partition上。

----------------------------------------------------
FSBL支持哪些启动模式：
根据保存启动镜像的非易失存储器的不同，FSBL支持如下不同的启动模式：
JTAG：BootROM启动后直接进入JTAG调试模式；
QSPI：从QSPI Nor Flash中读取启动镜像并启动，可以配置FSBL以XIP模式或非XIP模式读取启动镜像，启动镜像不使用文件系统；
SD：从SD卡中读取启动镜像并启动，启动镜像以FATFS的文件格式保存在SD卡；
EMMC：从EMMC中读取启动镜像并启动，启动镜像以FATFS的文件格式保存在EMMC；
EMMC Raw：从EMMC中读取启动镜像并启动，启动镜像以原始数据的模式保存在EMMC，启动镜像不使用文件系统；
NAND：从NAND Flash中读取启动镜像并启动，启动镜像不使用文件系统。
FSBL不支持USB、Ethernet启动。

FSBL Fallback：
若启动时发生错误，FSBL支持Fallback模式，由FSBL修改MultiBoot寄存器后（寄存器值加1），触发系统复位，
由BootROM重新检索一个启动镜像文件重新加载并启动。

