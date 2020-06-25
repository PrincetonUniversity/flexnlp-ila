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

#include <ilang/ilang++.h>

#include <flex/pe_config.h>
#include <flex/top_config.h>

namespace ilang {

namespace flex {

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

  //
  // Define child states in the PE Core
  //

  // state machine for PE Core module
  m.NewBvState(PEGetVarName(pe_idx, CORE_STATE), PE_CORE_STATE_BITWIDTH);

  // core activation vector
  // act register needs to be defined at the top level, which are accessed by
  // act child model.
  for (auto i = 0; i < PE_CORE_ACT_VECTOR_LANES; i++) {
    m.NewBvState(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECTOR),
                 PE_CORE_ACT_VECTOR_BITWIDTH);
  }
  // valid bit for core act register, which are accessed by both pe_core and
  // pe_act use it to immitate push function
  m.NewBvState(PEGetVarName(pe_idx, CORE_ACT_REG_PORT_VALID),
               PE_CORE_ACT_REG_PORT_VALID_BITWIDTH);
}

} // namespace flex

} // namespace ilang
