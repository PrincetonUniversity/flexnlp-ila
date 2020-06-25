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

// File: gb_addr_map.h

#ifndef FLEX_GB_ADDR_MAP_H__
#define FLEX_GB_ADDR_MAP_H__

namespace ilang {

namespace flex {

// GB start signals (1-bit, write only)
#define START_ADDR_GB_CONTROL 0x10
#define START_ADDR_GB_LAYER_REDUCE 0x20
#define START_ADDR_GB_LAYER_NORM 0x30
#define START_ADDR_GB_ZERO_PADDING 0x40
#define START_ADDR_GB_ATTENTION 0x50

// GBCore store instruction
#define GB_CORE_STORE_LARGE_ADDR_MIN 0x00500000
#define GB_CORE_STORE_LARGE_ADDR_MAX 0x005FFFF0
#define GB_CORE_STORE_LARGE_SIZE                                               \
  (GB_CORE_STORE_LARGE_ADDR_MAX - GB_CORE_STORE_LARGE_ADDR_MIN)
#define GB_CORE_STORE_LARGE_BITWIDTH 20

#define GB_CORE_STORE_SMALL_ADDR_MIN 0x00600000
#define GB_CORE_STORE_SMALL_ADDR_MAX 0x00603FF0
#define GB_CORE_STORE_SMALL_BITWIDTH 14 // 16KB small buffer size.

// GBCore memory manager instructions
#define CONFIG_ADDR_GB_CORE_MEM_MNGR_PHY_SRAM 0x00300000
#define CONFIG_ADDR_GB_CORE_MEM_MNGR_LARGE (0x00400000 + 0x10)
#define CONFIG_ADDR_GB_CORE_MEM_MNGR_SMALL (0x00400000 + 0x20)

// GB Control instruction
#define CONFIG_ADDR_GB_CONTROL (0x00700000 + 0x10)

// LayerReduce instruction
#define CONFIG_ADDR_GB_LAYER_REDUCE (0x00800000 + 0x10)

// LayerNorm instruction
#define CONFIG_ADDR_GB_LAYER_NORM (0x00900000 + 0x10)

// ZeroPadding instruction
#define CONFIG_ADDR_GB_ZERO_PADDING (0x00A00000 + 0x10)

// Attention instruction
#define CONFIG_ADDR_GB_ATTENTION (0x00B00000 + 0x10)

} // namespace flex

} // namespace ilang

#endif // FLEX_GB_ADDR_MAP_H__
