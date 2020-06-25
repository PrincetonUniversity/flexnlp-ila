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

// File: gb_core.h

#ifndef FLEX_GB_CORE_H__
#define FLEX_GB_CORE_H__

namespace ilang {

namespace flex {

// read 128-bits (16 bytes) at a time
#define GB_CORE_SCALAR 16
#define GB_CORE_SCALAR_BITWIDTH 4
#define GB_CORE_BYTE_BITWIDTH 8

//
// large buffer (1 MB)
//
#define GB_CORE_LARGE_BUFFER "gb_core_large_buffer"
#define GB_CORE_LARGE_NUM_BANKS 16
#define GB_CORE_LARGE_ENTRIES_PER_BANK 4096
#define GB_CORE_LARGE_BUFFER_SIZE                                              \
  (GB_CORE_SCALAR * GB_CORE_LARGE_NUM_BANKS * GB_CORE_LARGE_ENTRIES_PER_BANK)

// config register: num_vector_0
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_0                         \
  "gb_core_mem_mngr_large_num_vector_0"
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_0_WIDTH 8

// config register: base_large_0
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0                         \
  "gb_core_mem_mngr_large_base_large_0"
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0_WIDTH 16

// config register: num_vector_1
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_1                         \
  "gb_core_mem_mngr_large_num_vector_1"
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_1_WIDTH 8

// The base address for large buffer is vector size, thus need to concat 4bit 0
// into byte address config register: base_large_1
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1                         \
  "gb_core_mem_mngr_large_base_large_1"
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1_WIDTH 16

// config register: num_vector_2
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_2                         \
  "gb_core_mem_mngr_large_num_vector_2"
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_2_WIDTH 8

// config register: base_large_2
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2                         \
  "gb_core_mem_mngr_large_base_large_2"
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2_WIDTH 16

// config register: num_vector_3
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_3                         \
  "gb_core_mem_mngr_large_num_vector_3"
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_3_WIDTH 8

// config register: base_large_3
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3                         \
  "gb_core_mem_mngr_large_base_large_3"
#define GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3_WIDTH 16

//
// small buffer (16 KB)
//
#define GB_CORE_SMALL_BUFFER "gb_core_small_buffer"
#define GB_CORE_SMALL_NUM_BANKS 1
#define GB_CORE_SMALL_ENTRIES_PER_BANK 1024
#define GB_CORE_SMALL_BUFFER_SIZE                                              \
  (GB_CORE_SCALAR * GB_CORE_SMALL_NUM_BANKS * GB_CORE_SMALL_ENTRIES_PER_BANK)

// config register: base_small_0
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_0                         \
  "gb_core_mem_mngr_small_base_small_0"
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_0_WIDTH 16

// config register: base_small_1
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_1                         \
  "gb_core_mem_mngr_small_base_small_1"
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_1_WIDTH 16

// config register: base_small_2
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_2                         \
  "gb_core_mem_mngr_small_base_small_2"
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_2_WIDTH 16

// config register: base_small_3
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_3                         \
  "gb_core_mem_mngr_small_base_small_3"
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_3_WIDTH 16

// config register: base_small_4
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_4                         \
  "gb_core_mem_mngr_small_base_small_4"
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_4_WIDTH 16

// config register: base_small_5
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_5                         \
  "gb_core_mem_mngr_small_base_small_5"
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_5_WIDTH 16

// config register: base_small_6
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_6                         \
  "gb_core_mem_mngr_small_base_small_6"
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_6_WIDTH 16

// config register: base_small_7
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_7                         \
  "gb_core_mem_mngr_small_base_small_7"
#define GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_7_WIDTH 16

} // namespace flex

} // namespace ilang

#endif // FLEX_GB_CORE_H__
