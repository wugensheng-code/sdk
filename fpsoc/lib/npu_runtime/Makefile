# (0) include config file

# (1.1) add submodule resource file
SRC_DIR += image_process \
          uio_driver \
          npu_arm_op \
          npu_rt_graph/src \
          npu_driver \
          npu_util \
          npu_init \
          fm_mem_alloc \
          npu_scheduler \
          jpu_driver \
          elog/src \
          elog/port \
          api/src \
          port/cma_mem/nonos \
          port/file_ops/nonos \
          port/intr/nonos \
          port/npu_driver/nonos \
          port/npu_lock/nonos \
          port/rg_main/nonos \
          cpu_aclr_op/src \
          cpu_op/src


ifeq ($(CHIP_ARCH),armv8-a)
SRC_DIR += cpu_aclr_op/armv8
endif

# (1.2) add submodule includes
INC_DIR += . \
           npu_rt_graph/inc \
           fm_mem_alloc \
           npu_arm_op \
           npu_init \
           npu_scheduler \
           npu_util \
           image_process \
           uio_driver \
           jpu_driver \
           elog/inc \
           api/inc \
           port/cma_mem/inc \
           port/cma_mem/inc \
           port/file_ops/inc \
           port/intr/inc \
           port/npu_driver/inc \
           port/npu_lock/inc \
           port/rg_main/inc \
           npu_driver \
           cpu_aclr_op/inc \
           cpu_op/inc

# (3) set specificity FLAGS
LIBNAME := npu_runtime

CFLAGS := -O2

include ../../../tools/make/rules.mk
