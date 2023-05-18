#include <stdio.h>
#include "npu_init.h"
#include "npu_scheduler.h"
#include "fm_mem_alloc.h"
#include "gic.h"
#include "npu_util_func.h"
#include "npu_arm_op.h"
#include <math.h>
#include <new_op_func.h>

int8_t npu_reshape_node(npu_reshape_param_t *param)
{
  uint8_t o_dt = param->output_tensor->data_type;

  if (o_dt == NPU_DATA_INT8)
    return npu_reshape_node_int8(param);
  else if (o_dt == NPU_DATA_UINT8)
    return npu_reshape_node_uint8(param);
  else
  {
    printf("Unsupported datatype : %d", o_dt);
    return -1;
  }
}

int8_t npu_reshape_node_int8(npu_reshape_param_t *param)
{
  int8_t *input_buf = NULL;
  int8_t *output_buf = NULL;

  // input_buf = (int8_t *)param->input_tensor[0]->addr;
  // output_data = (int8_t *)param->output_tensor[0].addr;

  tensor_param_t* reshape_input = param->input_tensor;
  tensor_param_t* reshape_output = param->output_tensor;
  input_buf = (int8_t *)reshape_input->addr;
  output_buf = (int8_t *)reshape_output->addr;

  int32_t dim_in = reshape_input->dim_num;
  int32_t dim_out = reshape_output->dim_num;
  int size = 1;
  for (int i = 0; i < dim_in; i++)
  {
    size *= reshape_input->dims[i];
  }
  if (size <= 0)
  {
    return -1;
  }

  if (reshape_input->layout != 0)
  {
    if (dim_in == 4 && dim_out == 4)
    {
      int in_ch = reshape_input->dims[1];
      int in_h = reshape_input->dims[2];
      int in_w = reshape_input->dims[3];

      int out_ch = reshape_output->dims[1];
      int out_h = reshape_output->dims[2];
      int out_w = reshape_output->dims[3];

      int8_t *data_int8_temp = (int8_t *)malloc(size);

      int index = 0;
      for (int h = 0; h < in_h; h++)
        for (int w = 0; w < in_w; w++)
          for (int c = 0; c < in_ch; c++)
            data_int8_temp[index++] = input_buf[c * in_h * in_w + h * in_w + w];

      /* transpose nhwc to nchw */
      index = 0;
      for (int c = 0; c < out_ch; c++)
        for (int h = 0; h < out_h; h++)
          for (int w = 0; w < out_w; w++)
          {
            output_buf[index] = data_int8_temp[h * out_w * out_ch + w * out_ch + c];
            index++;
          }

      free(data_int8_temp);
      return 0;
    }
    else if (dim_in == 4 && dim_out == 3)
    {
      int in_ch = reshape_input->dims[1];
      int in_h = reshape_input->dims[2];
      int in_w = reshape_input->dims[3];

      int out_ch = reshape_output->dims[1];
      int out_h = reshape_output->dims[2];
      int out_w = reshape_output->dims[3];

      int8_t *data_int8_temp = (int8_t *)malloc(size);

      int index = 0;
      for (int h = 0; h < in_h; h++)
        for (int w = 0; w < in_w; w++)
          for (int c = 0; c < in_ch; c++)
            data_int8_temp[index++] = input_buf[c * in_h * in_w + h * in_w + w];

      /* transpose nhwc to nchw */
      index = 0;
      for (int c = 0; c < out_ch; c++)
        for (int w = 0; w < out_w; w++)
        {
          output_buf[index] = data_int8_temp[w * out_ch + c];
          index++;
        }

      free(data_int8_temp);
      return 0;
    }
  }
  memmove(output_buf, input_buf, size);
  return 0;
}

int8_t npu_reshape_node_uint8(npu_reshape_param_t *param)
{

  return 0;
}

//sigmoid
#define SIGMOID_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SIGMOID_MIN(a, b) ((a) < (b) ? (a) : (b))
double myexp(double x)
{
    int m = 1;
    int t = 10;
    int x_int = (int)x;
    double sum_dec = 1;
    double sum_int = 1;
    double e = 2.718281;
    double z = 1;
    double x_dec = x - x_int;
    //x<0:e^x=1/e^|x|
    if (x_int < 0)
    {
        x_int = (-x_int);
        for (int k = 0; k < x_int; k++)
        {
            sum_int *= e;
        }
        sum_int = 1 / e;
    }
    //x>0:e^x
    else
    {
        for (int k = 0; k < x_int; k++)
        {
            sum_int *= e;
        }
    }
    for (int i = 1; i < t; i++)
    {
        m *= i;
        z *= x_dec;
        sum_dec += z / m;
    }
    //e^x=e^(x=x_dec)*e^(x=x_int)
    return sum_int * sum_dec;
}

int8_t npu_sigmoid_node(npu_sigmoid_param_t *param)
{
  uint8_t o_dt = param->output_tensor->data_type;

  if (o_dt == NPU_DATA_INT8)
    return npu_sigmoid_node_int8(param);
  else if (o_dt == NPU_DATA_UINT8)
    return npu_sigmoid_node_uint8(param);
  else
  {
    printf("Unsupported datatype : %d", o_dt);
    return -1;
  }
}

int8_t npu_sigmoid_node_int8(npu_sigmoid_param_t *param)
{
  int8_t *input_buf = NULL;
  int8_t *output_buf = NULL;

  // input_buf = (int8_t *)param->input_tensor[0]->addr;
  // output_data = (int8_t *)param->output_tensor[0].addr;

  tensor_param_t* sigmoid_input = param->input_tensor;
  tensor_param_t* sigmoid_output = param->output_tensor;
  input_buf = (int8_t *)sigmoid_input->addr;
  output_buf = (int8_t *)sigmoid_output->addr;

  float input_scale = param->scale_i;
  float output_scale = param->scale_o;
  int32_t input_zero = param->zp_i;
  int32_t output_zero = param->zp_o;

  int size = 1;
  for (int i = 0; i < sigmoid_input->dim_num; i++)
  {
    size *= sigmoid_input->dims[i];
  }
  if (size <= 0)
  {
    return -1;
  }

  float *input_fp32 = (float *)0x40000000;
  float *output_fp32 = (float *)0x41000000;

  for (int i = 0; i < size; i++)
  {
    input_fp32[i] = ((float)input_buf[i] - (float)input_zero) * input_scale;
    output_fp32[i] = SIGMOID_MIN(input_fp32[i], 30.0f);
    output_fp32[i] = SIGMOID_MAX(input_fp32[i], -30.0f);
    output_fp32[i] = 1 / (1 + myexp(-output_fp32[i]));
  }

  for (int i = 0; i < size; i++)
  {
    int idata = round(output_fp32[i] / output_scale + output_zero);
    if (idata > 127)
      idata = 127;
    else if (idata < -127)
      idata = -127;
    output_buf[i] = idata;
  }

  return 0;
}

int8_t npu_sigmoid_node_uint8(npu_sigmoid_param_t *param)
{
 
  return 0;
}