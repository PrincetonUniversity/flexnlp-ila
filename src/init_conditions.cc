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

// File: init_conditions.cc

#include <flex/flex.h>

namespace ilang {

namespace flex {

void DefineInitConditons(Ila& m) {

  // gb_layer_reduce_initial conditions
  m.AddInit(m.state(GB_LAYER_REDUCE_START_FLAG) == 0);

  // pe_core instruction initial conditions
  // initial conditions for PE cntr
  m.AddInit(m.state(PE_CORE_CNTR) == 0);
  m.AddInit(m.state(PE_ACT_OUTGB_CNTR) == 0);
  m.AddInit(m.state(PE_ACT_DONE_CNTR) == 0);
  // initial conditions for the PE core state machine
  m.AddInit(m.state(PEGetVarName(0, CORE_STATE)) == PE_CORE_STATE_IDLE);
  m.AddInit(m.state(PEGetVarName(1, CORE_STATE)) == PE_CORE_STATE_IDLE);
  m.AddInit(m.state(PEGetVarName(2, CORE_STATE)) == PE_CORE_STATE_IDLE);
  m.AddInit(m.state(PEGetVarName(3, CORE_STATE)) == PE_CORE_STATE_IDLE);
}

} // namespace flex

} // namespace ilang
