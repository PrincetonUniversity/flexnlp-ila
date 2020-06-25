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

// File: gb_zero_padding.h

#ifndef FLEX_GB_ZERO_PADDING_H__
#define FLEX_GB_ZERO_PADDING_H__

namespace ilang {

namespace flex {

// config register: is_valid
#define GB_ZERO_PADDING_CONFIG_REG_IS_VALID "gb_zero_padding_is_valid"
#define GB_ZERO_PADDING_CONFIG_REG_IS_VALID_WIDTH 1

// config register: memory_index_1
#define GB_ZERO_PADDING_CONFIG_REG_MEMORY_INDEX_1                              \
  "gb_zero_padding_memory_index_1"
#define GB_ZERO_PADDING_CONFIG_REG_MEMORY_INDEX_1_WIDTH 3

// config register: num_vector_1
#define GB_ZERO_PADDING_CONFIG_REG_NUM_VECTOR_1 "gb_zero_padding_num_vector_1"
#define GB_ZERO_PADDING_CONFIG_REG_NUM_VECTOR_1_WIDTH 8

// config register: num_timestep_1
#define GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_1                              \
  "gb_zero_padding_num_timestep_1"
#define GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_1_WIDTH 16

// config register: num_timestep_2
#define GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_2                              \
  "gb_zero_padding_num_timestep_2"
#define GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_2_WIDTH 16

// internal states
#define GB_ZERO_PADDING_CHILD_VALID_FLAG "gb_zero_padding_child_valid_flag"
#define GB_ZERO_PADDING_CHILD_VALID_FLAG_BITWIDTH 1
#define GB_ZERO_PADDING_FLAG_ON 1
#define GB_ZERO_PADDING_FLAG_OFF 0

#define GB_ZERO_PADDING_CHILD_STATE "gb_zero_padding_child_state"
#define GB_ZERO_PADDING_CHILD_STATE_BITWIDTH 3

#define GB_ZERO_PADDING_CHILD_STATE_PREP 0
#define GB_ZERO_PADDING_CHILD_STATE_TIMESTEP 1
#define GB_ZERO_PADDING_CHILD_STATE_VECTOR 2
#define GB_ZERO_PADDING_CHILD_STATE_BYTE 3
#define GB_ZERO_PADDING_CHILD_STATE_NEXT 4
#define GB_ZERO_PADDING_CHILD_STATE_DONE 5

#define GB_ZERO_PADDING_TIMESTEP_CNTR "gb_zero_padding_timestep_cntr"
#define GB_ZERO_PADDING_TIMESTEP_CNTR_BITWIDTH                                 \
  GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_1_WIDTH

#define GB_ZERO_PADDING_VECTOR_CNTR "gb_zero_padding_vector_cntr"
#define GB_ZERO_PADDING_VECTOR_CNTR_BITWIDTH                                   \
  GB_ZERO_PADDING_CONFIG_REG_NUM_VECTOR_1_WIDTH

#define GB_ZERO_PADDING_MEM_BASE_ADDR "gb_zero_padding_mem_base_addr"
#define GB_ZERO_PADDING_MEM_BASE_ADDR_BITWIDTH GB_CORE_STORE_LARGE_BITWIDTH

} // namespace flex

} // namespace ilang

#endif // FLEX_GB_ZERO_PADDING_H__
