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

namespace flex {

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
#define GROUPING_SCALAR GB_CORE_LARGE_NUM_BANKS

#define GB_LAYER_REDUCE_GROUPING_NUM "gb_layer_reduce_grouping_num"
#define GB_LAYER_REDUCE_GROUPING_NUM_WIDTH                                     \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

#define GB_LAYER_REDUCE_GROUPING_INDEX "gb_layer_reduce_grouping_index"
#define GB_LAYER_REDUCE_GROUPING_INDEX_WIDTH GB_LAYER_REDUCE_GROUPING_NUM_WIDTH

#define GB_LAYER_REDUCE_GROUPING_REM "gb_layer_reduce_grouping_rem"
#define GB_LAYER_REDUCE_GROUPING_REM_WIDTH                                     \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

#define GB_LAYER_REDUCE_ITERATIONS "gb_layer_reduce_iterations"
#define GB_LAYER_REDUCE_ITERATIONS_WIDTH                                       \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

#define GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET                                 \
  "gb_layer_reduce_memory_min_addr_offset"
#define GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET_WIDTH                           \
  GB_CORE_STORE_LARGE_BITWIDTH

#define GB_LAYER_REDUCE_MEMORY_MAX_ADDR_OFFSET                                 \
  "gb_layer_reduce_memory_max_addr_offset"

#define GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE "gb_layer_reduce_memory_block_size"
#define GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE_WIDTH GB_CORE_STORE_LARGE_BITWIDTH

#define GB_LAYER_REDUCE_TIMESTEP_SIZE "gb_layer_timestep_size"
#define GB_LAYER_REDUCE_TIMESTEP_SIZE_WIDTH                                    \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

#define GB_LAYER_REDUCE_TIME_STEP_OP_CNTR "gb_layer_reduce_time_step_op_cntr"

#define GB_LAYER_REDUCE_TIME_STEP_OP_CNTR_WIDTH                                \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

/* grouping level child parameters */
#define GB_LAYER_REDUCE_GROUP_LEVEL_BASE_ADDR                                  \
  "gb_layer_reduce_group_level_base_addr"
#define GB_LAYER_REDUCE_GROUP_LEVEL_BASE_ADDR_WIDTH GB_CORE_STORE_LARGE_BITWIDTH

#define GB_LAYER_REDUCE_GROUP_LEVEL_OUT_ADDR                                   \
  "gb_layer_reduce_group_level_out_addr"
#define GB_LAYER_REDUCE_GROUP_LEVEL_OUT_ADDR_WIDTH GB_CORE_STORE_LARGE_BITWIDTH

#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_CNTR                                    \
  "gb_layer_reduce_timestep_level_cntr"
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_CNTR_WIDTH GB_CORE_SCALAR_BITWIDTH + 1

#define GB_LAYER_REDUCE_GROUP_LEVEL_TS_NUM "gb_layer_reduce_group_level_ts_num"
#define GB_LAYER_REDUCE_GROUP_LEVEL_TS_NUM_WIDTH                               \
  GB_LAYER_REDUCE_TIMESTEP_LEVEL_CNTR_WIDTH

/*  timestep level child parameters  */
#define GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR                                   \
  "gb_layer_reduce_vector_level_op_cntr"
#define GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR_WIDTH                             \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

/* time step level child instructions states parameters*/
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0                             \
  "gb_layer_reduce_timestep_level_base_addr_0"
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0_WIDTH                       \
  GB_CORE_STORE_LARGE_BITWIDTH
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1                             \
  "gb_layer_reduce_timestep_level_base_addr_1"
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1_WIDTH                       \
  GB_CORE_STORE_LARGE_BITWIDTH
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT                        \
  "gb_layer_reduce_timestep_level_base_addr_result"
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT_WIDTH                  \
  GB_CORE_STORE_LARGE_BITWIDTH

/* vector level child instruction states parameter*/
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0                                    \
  "gb_layer_reduce_vector_level_addr_0"
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0_WIDTH GB_CORE_STORE_LARGE_BITWIDTH
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1                                    \
  "gb_layer_reduce_vector_level_addr_1"
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1_WIDTH GB_CORE_STORE_LARGE_BITWIDTH
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT                               \
  "gb_layer_reduce_vector_level_addr_0_result"
#define GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT_WIDTH                         \
  GB_CORE_STORE_LARGE_BITWIDTH

#define GB_LAYER_REDUCE_BYTE_LEVEL_CNTR "gb_layer_reduce_byte_level_cntr"
#define GB_LAYER_REDUCE_BYTE_LEVEL_CNTR_WIDTH GB_CORE_SCALAR_BITWIDTH + 1

// flag variables for deciding child sequences
#define ON 1
#define OFF 0
#define DONE 1
#define UNDONE 0
#define FLAG_BITWIDTH 1

#define GB_LAYER_REDUCE_START_FLAG "gb_layer_reduce_start_flag"
#define GB_LAYER_REDUCE_START_FLAG_WIDTH FLAG_BITWIDTH

#define GB_LAYER_REDUCE_GROUP_LEVEL_FLAG "gb_layer_reduce_group_level_flag"
#define GB_LAYER_REDUCE_GROUP_LEVEL_FLAG_WIDTH FLAG_BITWIDTH

#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_FLAG                                    \
  "gb_layer_reduce_timestep_level_flag"
#define GB_LAYER_REDUCE_TIMESTEP_LEVEL_FLAG_WIDTH FLAG_BITWIDTH

#define GB_LAYER_REDUCE_VECTOR_LEVEL_FLAG "gb_layer_reduce_vector_level_flag"
#define GB_LAYER_REDUCE_VECTOR_LEVEL_FLAG_WIDTH FLAG_BITWIDTH

#define GB_LAYER_REDUCE_BYTE_LEVEL_FLAG "gb_layer_reduce_byte_level_flag"
#define GB_LAYER_REDUCE_BYTE_LEVEL_FLAG_WIDTH FLAG_BITWIDTH

//
#define GB_LAYER_REDUCE_VALID 1
#define GB_LAYER_REDUCE_INVALID 0
// GB LAYER REDUCE valid flag
#define GB_LAYER_REDUCE_CHILD_VALID_FLAG "gb_layer_reduce_child_valid_flag"
#define GB_LAYER_REDUCE_CHILD_VALID_FLAG_BITWIDTH 1

// gb layer reduce FSM
#define GB_LAYER_REDUCE_CHILD_STATE "gb_layer_reduce_child_state"
#define GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH 3
#define GB_LAYER_REDUCE_CHILD_STATE_PREP 0
#define GB_LAYER_REDUCE_CHILD_STATE_GROUP_OP 1
#define GB_LAYER_REDUCE_CHILD_STATE_TIMESTEP_OP 2
#define GB_LAYER_REDUCE_CHILD_STATE_VECTOR_OP 3
#define GB_LAYER_REDUCE_CHILD_STATE_BYTE_OP 4
#define GB_LAYER_REDUCE_CHILD_STATE_DONE 5

// iteration cntr
#define GB_LAYER_REDUCE_TIMESTEP_CNTR "gb_layer_reduce_timestep_cntr"
#define GB_LAYER_REDUCE_TIMESTEP_CNTR_BITWIDTH                                 \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

// vector cntr
#define GB_LAYER_REDUCE_VECTOR_CNTR "gb_layer_reduce_vector_cntr"
#define GB_LAYER_REDUCE_VECTOR_CNTR_BITWIDTH                                   \
  GB_LAYER_REDUCE_CONFIG_REG_NUM_VECTOR_1_WIDTH

} // namespace flex

} // namespace ilang

#endif // FLEX_GB_LAYER_REDUCE_H__
