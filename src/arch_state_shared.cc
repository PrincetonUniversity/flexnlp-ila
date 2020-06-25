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

// File: arch_state_shared.cc
// This file defines the shared states between PE and GB

#include <ilang/ilang++.h>

#include <flex/flex.h>
#include <flex/gb_config.h>
#include <flex/pe_config.h>
#include <flex/top_config.h>

namespace ilang {

namespace flex {

void DefineSharedState(Ila& m) {

  // shared states that are used in gb_control instructions between PE and GB
  // update 04182020: Need to have two copy of pe_start, one for pe_act
  m.NewBvState(PE_CORE_START_SIGNAL, PE_START_SIGNAL_SHARED_BITWIDTH);
  // can define one start signal for each PE cores to avoid using scheduler
  for (auto i = 0; i < PE_ACT_REGS_NUM; i++) {
    m.NewBvState(PEGetVarName(i, ACT_START_SIGNAL),
                 PE_START_SIGNAL_SHARED_BITWIDTH);
  }

  // m.NewBvState(PE_ACT_START_SIGNAL, PE_START_SIGNAL_SHARED_BITWIDTH);

  m.NewBvState(PE_DONE_SIGNAL_SHARED, PE_DONE_SIGNAL_SHARED_BITWIDTH);

  m.NewBvState(GB_CONTROL_DATA_IN_0, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_1, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_2, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_3, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_4, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_5, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_6, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_7, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_8, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_9, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_10, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_11, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_12, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_13, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_14, GB_CONTROL_DATA_IN_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_IN_15, GB_CONTROL_DATA_IN_BITWIDTH);

  m.NewBvState(GB_CONTROL_DATA_IN_ADDR, GB_CONTROL_DATA_IN_ADDR_BITWIDTH);

  m.NewBvState(GB_CONTROL_DATA_OUT_0, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_1, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_2, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_3, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_4, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_5, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_6, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_7, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_8, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_9, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_10, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_11, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_12, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_13, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_14, GB_CONTROL_DATA_OUT_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_15, GB_CONTROL_DATA_OUT_BITWIDTH);

  m.NewBvState(GB_CONTROL_DATA_OUT_ADDR, GB_CONTROL_DATA_OUT_ADDR_BITWIDTH);
  // data out index
  m.NewBvState(GB_CONTROL_DATA_OUT_INDEX, GB_CONTROL_DATA_OUT_INDEX_BITWIDTH);

  m.NewBvState(GB_CONTROL_DATA_IN_VALID, GB_CONTROL_DATA_IN_VALID_BITWIDTH);
  m.NewBvState(GB_CONTROL_DATA_OUT_VALID, GB_CONTROL_DATA_OUT_VALID_BITWIDTH);
}

} // namespace flex

} // namespace ilang
