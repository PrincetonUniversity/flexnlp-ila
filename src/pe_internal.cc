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

// File: pe_internal.cc

#include <ilang/ilang++.h>

#include <flex/pe_config.h>
#include <flex/top_config.h>

namespace ilang {

namespace flex {

void DefinePEInternalStates(Ila& m) {
  // define PE shared internal states here
  // counter for recording pe
  m.NewBvState(PE_CORE_CNTR, PE_CORE_CNTR_BIWTDTH);
  // counter for pe sending back data to GB
  m.NewBvState(PE_ACT_OUTGB_CNTR, PE_ACT_OUTGB_CNTR_BITWIDTH);
  // counter for pe seding pe_done to GB
  m.NewBvState(PE_ACT_DONE_CNTR, PE_ACT_DONE_CNTR_BITWIDTH);
  // number of valid pe
  m.NewBvState(PE_VALID_NUM, PE_VALID_NUM_BITWIDTH);
}

void DefinePEInternalStates(Ila& m, const int& pe_idx) {
  // Define PE own internal states here
}

} // namespace flex

} // namespace ilang
