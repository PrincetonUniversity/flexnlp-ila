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

// File: gb_layer_norm.h

#ifndef FLEX_GB_LAYER_NORM_H__
#define FLEX_GB_LAYER_NORM_H__

#include <flex/adpfloat_spec.h>

namespace ilang {

namespace flex {

// config register: is_valid
#define GB_LAYER_NORM_CONFIG_REG_IS_VALID "gb_layer_norm_is_valid"
#define GB_LAYER_NORM_CONFIG_REG_IS_VALID_WIDTH 1

// config register: mode
#define GB_LAYER_NORM_CONFIG_REG_MODE "gb_layer_norm_mode"
#define GB_LAYER_NORM_CONFIG_REG_MODE_WIDTH 3

// config register: memory_index_1
#define GB_LAYER_NORM_CONFIG_REG_MEMORY_INDEX_1 "gb_layer_norm_memory_index_1"
#define GB_LAYER_NORM_CONFIG_REG_MEMORY_INDEX_1_WIDTH 3

// config register: num_vector_1
#define GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1 "gb_layer_norm_num_vector_1"
#define GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1_WIDTH 8

// config register: num_timestep_1
#define GB_LAYER_NORM_CONFIG_REG_NUM_TIMESTEP_1 "gb_layer_norm_num_timestep_1"
#define GB_LAYER_NORM_CONFIG_REG_NUM_TIMESTEP_1_WIDTH 16

// config register: adpbias_1
#define GB_LAYER_NORM_CONFIG_REG_ADPBIAS_1 "gb_layer_norm_adpbias_1"
#define GB_LAYER_NORM_CONFIG_REG_ADPBIAS_1_WIDTH 3

// config register: adpbias_3
#define GB_LAYER_NORM_CONFIG_REG_ADPBIAS_3 "gb_layer_norm_adpbias_3"
#define GB_LAYER_NORM_CONFIG_REG_ADPBIAS_3_WIDTH 3

// config register: adpbias_4
#define GB_LAYER_NORM_CONFIG_REG_ADPBIAS_4 "gb_layer_norm_adpbias_4"
#define GB_LAYER_NORM_CONFIG_REG_ADPBIAS_4_WIDTH 3

#define GB_LAYER_NORM_CONFIG_GAMMA_INDEX 5
#define GB_LAYER_NORM_CONFIG_BETA_INDEX 6

/****** gb_layer_norm instructions internal states **********/

// 2nd level model counter
#define GB_LAYER_NORM_CNTR_TIMESTEP "gb_layer_norm_cntr_timestep"
#define GB_LAYER_NORM_CNTR_TIMESTEP_WIDTH                                      \
  GB_LAYER_NORM_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

// state register, memory block min address
#define GB_LAYER_NORM_MEM_MIN_ADDR_OFFSET "gb_layer_norm_mem_min_addr_offset"
#define GB_LAYER_NORM_MEM_MIN_ADDR_OFFSET_WIDTH GB_CORE_STORE_LARGE_BITWIDTH

// timestep level state, timestep base address
#define GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR                                 \
  "gb_layer_norm_timestep_level_base_addr"
#define GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR_WIDTH                           \
  GB_CORE_STORE_LARGE_BITWIDTH

// timestep level state, vector base address (put at timestep level to be shared
// by both vector level childs)
#define GB_LAYER_NORM_VECTOR_BASE_ADDR "gb_layer_norm_vector_base_addr"
#define GB_LAYER_NORM_VECTOR_BASE_ADDR_WIDTH GB_CORE_STORE_LARGE_BITWIDTH

// timestep level state, counter for vector level instructions
#define GB_LAYER_NORM_CNTR_VECTOR "gb_layer_norm_cntr_vector"
#define GB_LAYER_NORM_CNTR_VECTOR_WIDTH                                        \
  GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1_WIDTH

// bitwidth for normalization sum registers
#define GB_LAYER_NORM_SUM_TYPE_BITWIDTH 24

// timestep level state, sum for elements' values
#define GB_LAYER_NORM_SUM_X "gb_layer_norm_sum_X"
#define GB_LAYER_NORM_SUM_X_WIDTH GB_LAYER_NORM_SUM_TYPE_BITWIDTH

// timestep level state, sum for elements' square values
#define GB_LAYER_NORM_SUM_X_SQ "gb_layer_norm_sum_X_SQ"
#define GB_LAYER_NORM_SUM_X_SQ_WIDTH GB_LAYER_NORM_SUM_TYPE_BITWIDTH

// need a 20 bit vector accumulator registers for mean and mean_sq
#define GB_LAYER_NORM_VECTOR_SUM_X "gb_layer_norm_vector_sum_x"
#define GB_LAYER_NORM_VECTOR_SUM_X_WIDTH ACT_WORD_WIDTH

#define GB_LAYER_NORM_VECTOR_SUM_X_SQ "gb_layer_norm_vector_sum_x_sq"
#define GB_LAYER_NORM_VECTOR_SUM_X_SQ_WIDTH ACT_WORD_WIDTH

// timestep level state, mean value of data in the timestep
#define GB_LAYER_NORM_MEAN "gb_layer_norm_mean"
#define GB_LAYER_NORM_MEAN_WIDTH ACT_WORD_WIDTH

// timestep level state, standard variance of the data in the timestep
#define GB_LAYER_NORM_INV_STD "gb_layer_norm_inv_std"
#define GB_LAYER_NORM_INV_STD_WIDTH ACT_WORD_WIDTH

// vector level state, counter for byte level sum in the current vector
#define GB_LAYER_NORM_CNTR_BYTE "gb_layer_norm_cntr_byte"
#define GB_LAYER_NORM_CNTR_BYTE_WIDTH (GB_CORE_SCALAR_BITWIDTH + 1)

// vector level state, base address for gamma and beta in the small buffer
// bitwidth use 20 here, in order to hold the byte level address for the small
// buffer
#define GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA                             \
  "gb_layer_norm_vector_level_base_addr_gamma"
#define GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA_WIDTH                       \
  GB_CORE_STORE_LARGE_BITWIDTH

#define GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_BETA                              \
  "gb_layer_norm_vector_level_base_addr_beta"
#define GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDT_BETA_WIDTH                        \
  GB_CORE_STORE_LARGE_BITWIDTH

// update: 04082020: use only one child to organize the child instructions of
// GBLayerNorm
#define GB_LAYER_NORM_VALID 1
#define GB_LAYER_NORM_INVALID 0

#define GB_LAYER_NORM_CHILD_VALID_FLAG "gb_layer_norm_child_valid_flag"
#define GB_LAYER_NORM_CHILD_VALID_FLAG_BITWIDTH 1

#define GB_LAYER_NORM_CHILD_STATE "gb_layer_norm_child_state"
#define GB_LAYER_NORM_CHILD_STATE_BITWIDTH 4
#define GB_LAYER_NORM_CHILD_STATE_TIMESTEP_PREP 0
#define GB_LAYER_NORM_CHILD_STATE_SUM_VECTOR_PREP 1
#define GB_LAYER_NORM_CHILD_STATE_SUM_BYTE_OP 2
#define GB_LAYER_NORM_CHILD_STATE_MEAN_OP 3
#define GB_LAYER_NORM_CHILD_STATE_NORM_VECTOR_PREP 4
#define GB_LAYER_NORM_CHILD_STATE_NORM_BYTE_OP 5
#define GB_LAYER_NORM_CHILD_STATE_NEXT 6

} // namespace flex

} // namespace ilang

#endif // FLEX_GB_LAYER_NORM_H__
