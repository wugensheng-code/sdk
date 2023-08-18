import rtconfiga as rtconfig_soc
import os

# toolchains options
ARCH        = rtconfig_soc.ARCH
CPU         = rtconfig_soc.CPU
CROSS_TOOL  = rtconfig_soc.CROSS_TOOL

CHIP_ROOT = rtconfig_soc.CHIP_ROOT
SDK_ROOT = rtconfig_soc.SDK_ROOT
ARCH_ROOT = rtconfig_soc.ARCH_ROOT

if os.getenv('RTT_ROOT'):
    RTT_ROOT = os.getenv('RTT_ROOT')
else:
    RTT_ROOT = os.path.join(os.getcwd(), '..', '..')

if os.getenv('RTT_CC'):
    CROSS_TOOL = os.getenv('RTT_CC')

# cross_tool provides the cross compiler
# EXEC_PATH is the compiler execute path, for example, CodeSourcery,

if  CROSS_TOOL == 'gcc':
    PLATFORM    = 'gcc'
    EXEC_PATH   = rtconfig_soc.EXEC_PATH

if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD = 'debug'

if PLATFORM == 'gcc':
    # toolchains
    PREFIX = rtconfig_soc.PREFIX
    CC      = PREFIX + 'gcc'
    CXX     = PREFIX + 'g++'
    AS      = PREFIX + 'gcc'
    AR      = PREFIX + 'ar'
    LINK    = PREFIX + 'gcc'
    GDB     = PREFIX + 'gdb'
    TARGET_EXT = 'elf'
    SIZE    = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY  = PREFIX + 'objcopy'

    # DEVICE  = rtconfig_soc.DEVICE
    CFLAGS  = rtconfig_soc.CFLAGS
    AFLAGS  = rtconfig_soc.AFLAGS
    LFLAGS  = rtconfig_soc.LFLAGS
    CPATH   = rtconfig_soc.CPATH
    LPATH   = rtconfig_soc.LPATH
    LIBS    = rtconfig_soc.LIBS

    if BUILD == 'debug':
        CFLAGS += ' -O0 -gdwarf-2'
        AFLAGS += ' -gdwarf-2'
    else:
        CFLAGS += ' -O2'

    CXXFLAGS = CFLAGS

DUMP_ACTION = ''
POST_ACTION = ''
