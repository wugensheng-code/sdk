# (0) include config file

# (1.1) add submodule resource file
C_SRCS += $(wildcard ./*.c)
C_SRCS += $(wildcard lagecy/drivers/*.c)
C_SRCS += $(wildcard lagecy/qspi/*.c)
C_SRCS += $(wildcard lagecy/sd_emmc/*.c)


# (1.2) add submodule include
INC_DIR += ./ \
           lagecy/include \
           lagecy/qspi \
           lagecy/sd_emmc


# (3) set specificity FLAGS
LIBNAME := fatfs


include ../../../tools/make/rules.mk
