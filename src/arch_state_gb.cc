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

// File: arch_state_gb.cc

#include <ilang/ilang++.h>

#include <flex/gb_config.h>
#include <flex/top_config.h>

namespace ilang {

namespace flex {

void DefineGBOtherState(Ila& m) {

  // GB core large buffer
  // XXX one load/store unit is 128-bit; modeled using 8-bit
  auto gb_core_large_buffer =
      m.NewMemState(GB_CORE_LARGE_BUFFER, TOP_ADDR_IN_WIDTH, TOP_DATA_IN_WIDTH);
  gb_core_large_buffer.SetEntryNum(GB_CORE_LARGE_BUFFER_SIZE);

  // GB core small buffer
  // XXX one load/store unit is 128-bit; modeled using 8-bit
  auto gb_core_small_buffer =
      m.NewMemState(GB_CORE_SMALL_BUFFER, TOP_ADDR_IN_WIDTH, TOP_DATA_IN_WIDTH);
  gb_core_small_buffer.SetEntryNum(GB_CORE_SMALL_BUFFER_SIZE);
}

} // namespace flex

} // namespace ilang
