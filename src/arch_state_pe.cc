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

// File: arch_state_pe.cc

#include <flex/pe_config.h>
#include <flex/top_config.h>

#include <ilang/ilang++.h>

namespace ilang {

void DefinePEOtherState(Ila& m, const int& pe_idx) {

  // ActUnit buffer
  // XXX load/store unit may not be atomic
  auto pe_act_buffer = m.NewMemState(PEGetVarName(pe_idx, ACT_BUFFER),
                                     TOP_ADDR_IN_WIDTH, TOP_DATA_IN_WIDTH);
  pe_act_buffer.SetEntryNum(ACT_BUFFER_BUFFER_SIZE);

  // Core weight buffer
  // XXX load/store unit may not be atomic
  auto pe_core_weight_buffer =
      m.NewMemState(PEGetVarName(pe_idx, CORE_WEIGHT_BUFFER), TOP_ADDR_IN_WIDTH,
                    TOP_DATA_IN_WIDTH);
  pe_core_weight_buffer.SetEntryNum(CORE_WEIGHT_BUFFER_SIZE);

  // Core input buffer
  // XXX load/store unit may not be atomic
  auto pe_core_input_buffer =
      m.NewMemState(PEGetVarName(pe_idx, CORE_INPUT_BUFFER), TOP_ADDR_IN_WIDTH,
                    TOP_DATA_IN_WIDTH);
  pe_core_input_buffer.SetEntryNum(CORE_INPUT_BUFFER_SIZE);
}

}; // namespace ilang
