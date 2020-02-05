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

namespace ilang {

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

/****** gb_layer_norm instructions internal states **********/
// internal states for LayerNorm instruction
#define GB_LAYER_NORM_ITERATIONS "gb_layer_norm_iterations"
#define GB_LAYER_NORM_ITERATIONS_WIDTH GB_LAYER_NORM_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

// 2nd level model counter
#define GB_LAYER_NORM_CNTR_TIMESTEP "gb_layer_norm_cntr_timestep"
#define GB_LAYER_NORM_CNTR_TIMESTEP_WIDTH GB_LAYER_NORM_ITERATIONS_WIDTH

// state register, memory block size
#define GB_LAYER_NORM_MEM_BLOCK_SIZE "gb_layer_norm_mem_block_size"
#define GB_LAYER_NORM_MEM_BLOCK_SIZE_WIDTH GB_CORE_STORE_LARGE_BITWIDTH

// state register, memory block min address
#define GB_LAYER_NORM_MEM_MIN_ADDR_OFFSET "gb_layer_norm_mem_min_addr_offset"
#define GB_LAYER_NORM_MEM_MIN_ADDR_OFFSET_WIDTH GB_CORE_STORE_LARGE_BITWIDTH

// timestep level state, timestep base address
#define GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR "gb_layer_norm_timestep_level_base_addr"
#define GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR_WIDTH GB_CORE_STORE_LARGE_BITWIDTH

// timestep level state, counter for vector level instructions
#define GB_LAYER_NORM_CNTR_VECTOR "gb_layer_norm_cntr_vector"
#define GB_LAYER_NORM_CNTR_VECTOR_WIDTH GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1_WIDTH

// TODO: some specs for flexNLP, temporally put here
#define GB_LAYER_NORM_SUM_TYPE_BITWIDTH 24
#define K_ACT_WORD_WIDTH 20
#define K_ACT_NUM_FRAC 14
#define K_ADPFLOAT_WORD_WIDTH 8
#define K_ADPFLOAT_EXP_WIDTH 3
#define K_ADPFLOAT_MAN_WIDTH K_ADPFLOAT_WORD_WIDTH - K_ADPFLOAT_EXP_WIDTH - 1

// timestep level state, sum for elements' values
#define GB_LAYER_NORM_SUM_X "gb_layer_norm_sum_X"
#define GB_LAYER_NORM_SUM_X_WIDTH GB_LAYER_NORM_SUM_TYPE_BITWIDTH

// timestep level state, sum for elements' square values
#define GB_LAYER_NORM_SUM_X_SQ "gb_layer_norm_sum_X_SQ"
#define GB_LAYER_NORM_SUM_X_SQ_WIDTH GB_LAYER_NORM_SUM_TYPE_BITWIDTH

// timestep level state, mean value of data in the timestep
#define GB_LAYER_NORM_MEAN "gb_layer_norm_mean"
#define GB_LAYER_NORM_MEAN_WIDTH K_ADPFLOAT_WORD_WIDTH

// timestep level state, variance of the data in the timestep
#define GB_LAYER_NORM_VAR "gb_layer_norm_var"
#define GB_LAYER_NORM_VAR_WIDTH K_ACT_WORD_WIDTH

// timestep level state, standard variance of the data in the timestep
#define GB_LAYER_NORM_INV_STD "gb_layer_norm_inv_std"
#define GB_LAYER_NORM_INV_STD_WIDTH K_ADPFLOAT_WORD_WIDTH



// timestep level state, 
}; // namespace ilang

#endif // FLEX_GB_LAYER_NORM_H__
