// =============================================================================
// MIT License
//
// Copyright (c) 2019 Princeton University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// =============================================================================

// File: gb_layer_reduce.h

#ifndef FLEX_GB_LAYER_REDUCE_H__
#define FLEX_GB_LAYER_REDUCE_H__

namespace ilang {

// config register: is_valid
#define GB_LAYER_REDUCE_CONFIG_REG_IS_VALID "gb_layer_reduce_is_valid"
#define GB_LAYER_REDUCE_CONFIG_REG_IS_VALID_WIDTH 1

// config register: mode
#define GB_LAYER_REDUCE_CONFIG_REG_MODE "gb_layer_reduce_mode"
#define GB_LAYER_REDUCE_CONFIG_REG_MODE_WIDTH 3

// config register: memory_index_1
#define GB_LAYER_REDUCE_CONFIG_REG_MEMORY_INDEX_1                              \
  "gb_layer_reduce_memory_index_1"
#define GB_LAYER_REDUCE_CONFIG_REG_MEMORY_INDEX_1_WIDTH 3

// config register: num_vector_1
#define GB_LAYER_REDUCE_CONFIG_REG_NUM_VECTOR_1 "gb_layer_reduce_num_vector_1"
#define GB_LAYER_REDUCE_CONFIG_REG_NUM_VECTOR_1_WIDTH 8

// config register: num_timestep_1
#define GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1                              \
  "gb_layer_reduce_num_timestep_1"
#define GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH 16

// mode value
#define GB_LAYER_REDUCE_OP_MAX 0
#define GB_LAYER_REDUCE_OP_MEAN 1
#define GB_LAYER_REDUCE_OP_ADD 2

// states for sub-instructions
#define GB_LAYER_REDUCE_ITERATIONS "gb_layer_reduce_iterations"
#define GB_LAYER_REDUCE_ITERATIONS_WIDTH                                      \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

#define GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET                                \
  "gb_layer_reduce_memory_min_addr_offset"
#define GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET_WIDTH                          \
  GB_CORE_STORE_LARGE_BITWIDTH

#define GB_LAYER_REDUCE_MEMORY_MAX_ADDR_OFFSET                                \
  "gb_layer_reduce_memory_max_addr_offset"

#define GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE                                     \
  "gb_layer_reduce_memory_block_size"
#define GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE_WIDTH                               \
  GB_CORE_STORE_LARGE_BITWIDTH

#define GB_LAYER_REDUCE_TIMESTEP_SIZE "gb_layer_timestep_size"                
#define GB_LAYER_REDUCE_TIMESTEP_SIZE_WIDTH GB_CORE_STORE_LARGE_BITWIDTH

#define GB_LAYER_REDUCE_TIME_STEP_OP_CNTR                                     \
  "gb_layer_reduce_time_step_op_cntr"

#define GB_LAYER_REDUCE_TIME_STEP_OP_CNTR_WIDTH                               \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

/* time step level child instructions states parameters*/
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0                            \
  "gb_layer_reduce_timestep_level_base_addr_0"
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0_WIDTH                      \
  TOP_ADDR_IN_WIDTH
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1                            \
  "gb_layer_reduce_timestep_level_base_addr_1"
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1_WIDTH                      \
  TOP_ADDR_IN_WIDTH
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT                       \
  "gb_layer_reduce_timestep_level_base_addr_result"
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT_WIDTH                 \
  TOP_ADDR_IN_WIDTH

#define GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR                                  \
  "gb_layer_reduce_vector_level_op_cntr"
#define GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR_WIDTH                            \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

/* vector level child instruction states parameter*/
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0                                   \
  "gb_layer_reduce_vector_level_addr_0"
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0_WIDTH                             \
  TOP_ADDR_IN_WIDTH
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1                                   \
  "gb_layer_reduce_vector_level_addr_1"
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1_WIDTH                             \
  TOP_ADDR_IN_WIDTH
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT                              \
  "gb_layer_reduce_vector_level_addr_0"
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT_WIDTH                        \
  TOP_ADDR_IN_WIDTH

}; // namespace ilang

#endif // FLEX_GB_LAYER_REDUCE_H__
