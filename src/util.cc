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

#include <flex/pe_config.h>
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

static const std::vector<std::string> idx2entry_1 = {
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY0,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY1,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY2,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY3,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY4,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY5,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY6,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY7,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY8,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY9,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY10,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY11,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY12,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY13,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY14,
    CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY15};

ExprRef FetchClusterLUT_First(Ila& m, const int& pe_idx, const ExprRef& index) {
  auto result = m.state(PEGetVarName(pe_idx, idx2entry_1.front()));

  for (auto i = 1; i < idx2entry_1.size(); i++) {
    auto state_name = PEGetVarName(pe_idx, idx2entry_1.at(i));
    result = Ite(index == i, m.state(state_name), result);
  }

  return result;
}

static const std::vector<std::string> idx2entry_2 = {
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY0,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY1,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY2,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY3,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY4,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY5,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY6,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY7,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY8,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY9,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY10,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY11,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY12,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY13,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY14,
    CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY15};

ExprRef FetchClusterLUT_Second(Ila& m, const int& pe_idx,
                               const ExprRef& index) {

  auto result = m.state(PEGetVarName(pe_idx, idx2entry_2.front()));

  for (auto i = 1; i < idx2entry_2.size(); i++) {
    auto state_name = PEGetVarName(pe_idx, idx2entry_2.at(i));
    result = Ite(index == i, m.state(state_name), result);
  }

  return result;
}

static const std::vector<std::string> idx2act = {
    ACT_VECTOR_0_15_CONFIG_REG_INST0,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST1,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST2,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST3,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST4,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST5,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST6,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST7,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST8,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST9,   //
    ACT_VECTOR_0_15_CONFIG_REG_INST10,  //
    ACT_VECTOR_0_15_CONFIG_REG_INST11,  //
    ACT_VECTOR_0_15_CONFIG_REG_INST12,  //
    ACT_VECTOR_0_15_CONFIG_REG_INST13,  //
    ACT_VECTOR_0_15_CONFIG_REG_INST14,  //
    ACT_VECTOR_0_15_CONFIG_REG_INST15,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST0,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST1,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST2,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST3,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST4,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST5,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST6,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST7,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST8,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST9,  //
    ACT_VECTOR_16_31_CONFIG_REG_INST10, //
    ACT_VECTOR_16_31_CONFIG_REG_INST11, //
    ACT_VECTOR_16_31_CONFIG_REG_INST12, //
    ACT_VECTOR_16_31_CONFIG_REG_INST13, //
    ACT_VECTOR_16_31_CONFIG_REG_INST14, //
    ACT_VECTOR_16_31_CONFIG_REG_INST15  //
};

ExprRef PEActInstrFetch(Ila& m, const int& pe_idx, const ExprRef& instr_cntr) {

  auto result = m.state(PEGetVarName(pe_idx, idx2act.front()));

  for (auto i = 1; i < idx2act.size(); i++) {
    auto state_name = PEGetVarName(pe_idx, idx2act.at(i));
    result = Ite(instr_cntr == i, m.state(state_name), result);
  }

  return result;
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

// update 06022020: add a template function call for pe_core_run_mac update
// result

void PECoreRunMacOut(Ila& m, const int& pe_idx, const int& idx) {
  auto child_pe_core = m.child(PEGetChildName(pe_idx, "CORE_CHILD"));
  auto child_run_mac =
      child_pe_core.child(PEGetChildName(pe_idx, "CORE_RUN_MAC_CHILD"));

  auto is_cluster =
      (m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER)) ==
       PE_CORE_VALID);

  auto state =
      child_pe_core.state(PEGetVarName(pe_idx, CORE_RUN_MAC_CHILD_STATE));

  auto run_mac_flag =
      child_pe_core.state(PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_FLAG));
  auto run_mac_cntr =
      child_pe_core.state(PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_CNTR));
  auto result_tmp =
      child_run_mac.state(PEGetVarName(pe_idx, CORE_RUN_MAC_CHILD_RESULT_TEMP));

  auto child_valid = ((run_mac_flag == PE_CORE_VALID) & (run_mac_cntr < 16));
  auto state_out = (state == PE_CORE_RUN_MAC_STATE_OUT);

  {
    auto instr = child_run_mac.NewInstr(
        PEGetVarNameVector(pe_idx, idx, "core_run_mac_out"));

    // one individual instruction for each register
    instr.SetDecode(child_valid & state_out & (run_mac_cntr == idx));

    auto accum_state =
        child_pe_core.state(PEGetVarNameVector(pe_idx, idx, CORE_ACCUM_VECTOR));
    auto tmp = AccumAdd2(accum_state, result_tmp);

    // state updates
    instr.SetUpdate(accum_state, tmp);
    auto next_state = Ite(is_cluster,
                          BvConst(PE_CORE_RUN_MAC_STATE_FETCH_CLUSTER,
                                  PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH),
                          BvConst(PE_CORE_RUN_MAC_STATE_FETCH_NON_CLUSTER,
                                  PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH));

    auto run_mac_flag_next =
        Ite(run_mac_cntr >= 15,
            BvConst(PE_CORE_INVALID, PE_CORE_CHILD_RUN_MAC_FLAG_BITWIDTH),
            BvConst(PE_CORE_VALID, PE_CORE_CHILD_RUN_MAC_FLAG_BITWIDTH));

    // control state updates
    instr.SetUpdate(state, next_state);
    instr.SetUpdate(run_mac_cntr, run_mac_cntr + 1);
    instr.SetUpdate(run_mac_flag, run_mac_flag_next);
  }
}

} // namespace flex

} // namespace ilang
