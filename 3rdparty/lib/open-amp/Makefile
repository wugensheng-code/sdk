# (0) include config file

# (1.1) add submodule resource file
C_SRCS += $(wildcard ./lib/*.c) \
		    $(wildcard ./lib/rpmsg/*.c) \
			$(wildcard ./lib/virtio/*.c) \
			$(wildcard ./lib/remoteproc/*.c) \
			$(wildcard ./lib/service/rpmsg/rpc/*.c) \
			$(wildcard ./lib/proxy/*.c)

# (1.2) add submodule include file to /bsp/inc for calling
INC_DIR += lib/include \
			lib/rpmsg \
			include/include/generated \
			../libmetal/include/lib/include \
			apps/machine/dr1m90

CFLAGS += -DMACHINE="dr1m90" \
			-DCMAKE_SYSTEM_PROCESSOR="arm" \
			-DCMAKE_SYSTEM_NAME="Generic" \
			-DCMAKE_C_COMPILER_FORCED=ON \
			-DWITH_PROXY=ON \
			-DWITH_APPS=OFF \
			-DWITH_DCACHE_VRINGS=ON \
			-DVIRTIO_CACHED_VRINGS

# (3) set specificity FLAGS
LIBNAME := openamp

# (2) compile and return obj-y

include ../../../tools/make/rules.mk
