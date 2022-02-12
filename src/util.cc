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

// File: util.cc

#include <ilang/ilang++.h>
#include <ilang/util/log.h>

#include <flex/flex.h>
#include <flex/uninterpreted_func.h>
#include <flex/util.h>

namespace ilang {

namespace flex {

void SetUpdateForConfigWr(Ila& m, InstrRef& instr, const ExprRef& next,
                          const std::string& state_name) {
  auto state = m.state(state_name);

  ILA_ASSERT(state.bit_width() <= next.bit_width())
      << "Bit-width error " << state;

  if (state.bit_width() == next.bit_width()) {
    instr.SetUpdate(state, next);
  } else if (state.bit_width() == 1) {
    instr.SetUpdate(state, next(0));
  } else {
    instr.SetUpdate(state, next(state.bit_width() - 1, 0));
  }
}

ExprRef FetchClusterLUT_First(Ila& m, const int& pe_idx, const ExprRef& index) {
    auto lut = m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_0_STATE_MEM));
    auto addr = Concat(BvConst(0, 32-index.bit_width()), index);
    return Load(lut, addr);
}

ExprRef FetchClusterLUT_Second(Ila& m, const int& pe_idx,
                               const ExprRef& index) {
    auto lut = m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_1_STATE_MEM));
    auto addr = Concat(BvConst(0, 32-index.bit_width()), index);
    return Load(lut, addr);
}


ExprRef PEActInstrFetch(Ila& m, const int& pe_idx, const ExprRef& instr_cntr) {
    auto act_v_table = m.state(PEGetVarName(pe_idx, ACT_VECTOR_STATE_MEM));
    auto addr = Concat(BvConst(0, 32 - instr_cntr.bit_width()), instr_cntr);
    return Load(act_v_table, addr);
}

// function help get the gb small buffer base address given the memory index
ExprRef GBGetSmallBufBase(Ila& m, const ExprRef& mem_small_index) {
  auto mem_small_base = Ite(
      mem_small_index == 0,
      Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_0),
             BvConst(0, 4)),
      Ite(mem_small_index == 1,
          Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_1),
                 BvConst(0, 4)),
          Ite(mem_small_index == 2,
              Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_2),
                     BvConst(0, 4)),
              Ite(mem_small_index == 3,
                  Concat(
                      m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_3),
                      BvConst(0, 4)),
                  Ite(mem_small_index == 4,
                      Concat(
                          m.state(
                              GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_4),
                          BvConst(0, 4)),
                      Ite(mem_small_index == 5,
                          Concat(
                              m.state(
                                  GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_5),
                              BvConst(0, 4)),
                          Ite(mem_small_index == 6,
                              Concat(
                                  m.state(
                                      GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_6),
                                  BvConst(0, 4)),
                              Concat(
                                  m.state(
                                      GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_7),
                                  BvConst(0, 4)))))))));

  return mem_small_base;
}

// add a function to calculate the base address of GB Large given the timestep index
ExprRef GetGBLargeBaseAddr(const ExprRef& timestep_idx, const ExprRef& num_vector) {
    // assume the argument and return values are all 20-bit
    // lower timestep idx is the URem of the timestep groups
    auto lower_timestep_idx = Concat(BvConst(0, 16), Extract(timestep_idx, 3, 0));
    // number of the timestep groups of 16
    auto upper_timestep_idx = Concat(BvConst(0, 4), Extract(timestep_idx, 19, 4));
    auto base_addr_offset = (
        lower_timestep_idx + 
        upper_timestep_idx * num_vector * GB_CORE_LARGE_NUM_BANKS
    ) * GB_CORE_SCALAR;
    return base_addr_offset;
}

} // namespace flex

} // namespace ilang
