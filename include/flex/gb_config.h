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

// File: gb_config.h

#ifndef FLEX_GB_CONFIG_H__
#define FLEX_GB_CONFIG_H__

#include <flex/gb_addr_map.h>
#include <flex/gb_attention.h>
#include <flex/gb_control.h>
#include <flex/gb_core.h>
#include <flex/gb_layer_norm.h>
#include <flex/gb_layer_reduce.h>
#include <flex/gb_zero_padding.h>

namespace ilang {

namespace flex {

inline std::string GBGetVectorName(const int& idx,
                                   const std::string& var_name) {
  auto suffix = std::to_string(idx);
  return var_name + "_" + suffix;
}

inline std::string GBGetVectorName2D(const int& i, const int& j,
                                     const std::string& var_name) {
  auto i_string = std::to_string(i);
  auto j_string = std::to_string(j);
  return var_name + "_" + i_string + "_" + j_string;
}

} // namespace flex

} // namespace ilang

#endif // FLEX_GB_CONFIG_H__
