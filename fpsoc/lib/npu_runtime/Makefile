# (0) include config file
include $(core)/config.mk

# (1.1) add submodule resource file
src-$(CONFIG_NPU_RUNTIME) += image_process/blas.c
src-$(CONFIG_NPU_RUNTIME) += image_process/list.c
src-$(CONFIG_NPU_RUNTIME) += image_process/image.c
src-$(CONFIG_NPU_RUNTIME) += uio_driver/bare_metal_io.c
src-$(CONFIG_NPU_RUNTIME) += uio_driver/uio_npu_driver.c
src-$(CONFIG_NPU_RUNTIME) += npu_arm_op/armv8/dw_k3s1p1_int8_a72.S
src-$(CONFIG_NPU_RUNTIME) += npu_arm_op/armv8/dw_k3s2p1_int8_a72.S
src-$(CONFIG_NPU_RUNTIME) += npu_arm_op/armv8/dw_k7s1p0_int8_a72.S
src-$(CONFIG_NPU_RUNTIME) += npu_arm_op/armv8/dw_k3s1p1_int16_a72.S
src-$(CONFIG_NPU_RUNTIME) += npu_arm_op/armv8/dw_k3s2p1_int16_a72.S
src-$(CONFIG_NPU_RUNTIME) += npu_arm_op/armv8/dw_k7s1p0_int16_a72.S
src-$(CONFIG_NPU_RUNTIME) += npu_arm_op/armv8/dw_k5s1.S
src-$(CONFIG_NPU_RUNTIME) += npu_arm_op/npu_arm_op.c
src-$(CONFIG_NPU_RUNTIME) += npu_rt_graph/src/npu_rt_graph.c
src-$(CONFIG_NPU_RUNTIME) += npu_rt_graph/src/npu_rt_graph_util.c
src-$(CONFIG_NPU_RUNTIME) += npu_rt_graph/src/rgb_deserializer.c
src-$(CONFIG_NPU_RUNTIME) += npu_util/npu_util.c
src-$(CONFIG_NPU_RUNTIME) += npu_util/npu_util_func.c
src-$(CONFIG_NPU_RUNTIME) += npu_util/npu_util_test.c
src-$(CONFIG_NPU_RUNTIME) += npu_util/new_op_func.c
src-$(CONFIG_NPU_RUNTIME) += npu_util/fifo.c
src-$(CONFIG_NPU_RUNTIME) += fm_mem_alloc/buddy_allocator.c
src-$(CONFIG_NPU_RUNTIME) += fm_mem_alloc/fm_mem_alloc.c
src-$(CONFIG_NPU_RUNTIME) += npu_init/npu_init.c
src-$(CONFIG_NPU_RUNTIME) += npu_scheduler/npu_scheduler.c
src-$(CONFIG_NPU_RUNTIME) += $(wildcard jpu_driver/*.c)



# (1.2) add submodule include file to /bsp/inc for calling
inc-$(CONFIG_NPU_RUNTIME) += npu_rt_graph/inc
inc-$(CONFIG_NPU_RUNTIME) += fm_mem_alloc
inc-$(CONFIG_NPU_RUNTIME) += npu_arm_op
inc-$(CONFIG_NPU_RUNTIME) += npu_init
inc-$(CONFIG_NPU_RUNTIME) += npu_scheduler
inc-$(CONFIG_NPU_RUNTIME) += npu_util
inc-$(CONFIG_NPU_RUNTIME) += image_process
inc-$(CONFIG_NPU_RUNTIME) += uio_driver
inc-$(CONFIG_NPU_RUNTIME) += jpu_driver
inc-$(CONFIG_NPU_RUNTIME) += ./

# (3) set specificity FLAGS
# LIBNAME := libnpu_runtime.a

# (2) compile and return obj-y
include $(core)/build.mk



