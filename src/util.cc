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

#include <flex/util.h>
#include <flex/pe_config.h>
#include <flex/uninterpreted_func.h>

#include <ilang/ilang++.h>
#include <ilang/util/log.h>

namespace ilang {

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
  auto result = 
    Ite(index == 0, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY0)),
      Ite(index == 1, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY1)),
      Ite(index == 2, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY2)), 
      Ite(index == 3, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY3)),
      Ite(index == 4, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY4)),
      Ite(index == 5, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY5)),
      Ite(index == 6, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY6)),
      Ite(index == 7, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY7)),
      Ite(index == 8, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY8)),
      Ite(index == 9, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY9)),
      Ite(index == 10, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY10)), 
      Ite(index == 11, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY11)),
      Ite(index == 12, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY12)),
      Ite(index == 13, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY13)),
      Ite(index == 14, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY14)),
                       m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY15))
                       )))))))))))))));
  return result;
}

ExprRef FetchClusterLUT_Second(Ila& m, const int& pe_idx, const ExprRef& index) {
  auto result = 
    Ite(index == 0, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY0)),
      Ite(index == 1, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY1)),
      Ite(index == 2, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY2)), 
      Ite(index == 3, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY3)),
      Ite(index == 4, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY4)),
      Ite(index == 5, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY5)),
      Ite(index == 6, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY6)),
      Ite(index == 7, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY7)),
      Ite(index == 8, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY8)),
      Ite(index == 9, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY9)),
      Ite(index == 10, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY10)), 
      Ite(index == 11, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY11)),
      Ite(index == 12, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY12)),
      Ite(index == 13, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY13)),
      Ite(index == 14, m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY14)),
                       m.state(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY15))
                       )))))))))))))));
  return result;
}

ExprRef PEActInstrFetch(Ila&m, const int& pe_idx, const ExprRef& instr_cntr) {
  auto result = 
    Ite(instr_cntr == 0, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST0)),
      Ite(instr_cntr == 1, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST1)),
      Ite(instr_cntr == 2, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST2)),
      Ite(instr_cntr == 3, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST3)),
      Ite(instr_cntr == 4, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST4)),
      Ite(instr_cntr == 5, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST5)),
      Ite(instr_cntr == 6, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST6)),
      Ite(instr_cntr == 7, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST7)),
      Ite(instr_cntr == 8, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST8)),
      Ite(instr_cntr == 9, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST9)),
      Ite(instr_cntr == 10, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST10)),
      Ite(instr_cntr == 11, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST11)),
      Ite(instr_cntr == 12, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST12)),
      Ite(instr_cntr == 13, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST13)),
      Ite(instr_cntr == 14, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST14)),
      Ite(instr_cntr == 15, m.state(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST15)),
      Ite(instr_cntr == 16, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST0)),
      Ite(instr_cntr == 17, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST1)),
      Ite(instr_cntr == 18, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST2)),
      Ite(instr_cntr == 19, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST3)),
      Ite(instr_cntr == 20, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST4)),
      Ite(instr_cntr == 21, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST5)),
      Ite(instr_cntr == 22, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST6)),
      Ite(instr_cntr == 23, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST7)),
      Ite(instr_cntr == 24, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST8)),
      Ite(instr_cntr == 25, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST9)),
      Ite(instr_cntr == 26, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST10)),
      Ite(instr_cntr == 27, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST11)),
      Ite(instr_cntr == 28, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST12)),
      Ite(instr_cntr == 29, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST13)),
      Ite(instr_cntr == 30, m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST14)),
                            m.state(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST15))
                            )))))))))))))))))))))))))))))));
  return result;
}

// update 06022020: add a template function call for pe_core_run_mac update result

void PECoreRunMacOut(Ila& m, const int& pe_idx, const int& idx) {
  auto child_pe_core = m.child(PEGetChildName(pe_idx, "CORE_CHILD"));
  auto child_run_mac = child_pe_core.child(PEGetChildName(pe_idx, "CORE_RUN_MAC_CHILD"));

  auto is_cluster = 
    (m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER)) == PE_CORE_VALID);
  
  auto state = child_pe_core.state(PEGetVarName(pe_idx, CORE_RUN_MAC_CHILD_STATE));

  auto run_mac_flag = child_pe_core.state(PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_FLAG));
  auto run_mac_cntr = child_pe_core.state(PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_CNTR));
  auto result_tmp = child_run_mac.state(PEGetVarName(pe_idx, CORE_RUN_MAC_CHILD_RESULT_TEMP));

  auto child_valid = ((run_mac_flag == PE_CORE_VALID) & (run_mac_cntr < 16));
  auto state_out = (state == PE_CORE_RUN_MAC_STATE_OUT);

  {
    auto instr = child_run_mac.NewInstr(
                  PEGetVarNameVector(pe_idx, idx, "core_run_mac_out"));

    // one individual instruction for each register
    instr.SetDecode(child_valid & state_out & (run_mac_cntr == idx));

    auto accum_state = child_pe_core.state(PEGetVarNameVector(pe_idx, idx, CORE_ACCUM_VECTOR));
    auto tmp = AccumAdd2(accum_state, result_tmp);

    // state updates
    instr.SetUpdate(accum_state, tmp);
    auto next_state = Ite(is_cluster,
                            BvConst(PE_CORE_RUN_MAC_STATE_FETCH_CLUSTER,
                                    PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH),
                            BvConst(PE_CORE_RUN_MAC_STATE_FETCH_NON_CLUSTER,
                                    PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH));
    
    auto run_mac_flag_next = Ite(run_mac_cntr >= 15, 
                                  BvConst(PE_CORE_INVALID, PE_CORE_CHILD_RUN_MAC_FLAG_BITWIDTH),
                                  BvConst(PE_CORE_VALID, PE_CORE_CHILD_RUN_MAC_FLAG_BITWIDTH));
    
    // control state updates
    instr.SetUpdate(state, next_state);
    instr.SetUpdate(run_mac_cntr, run_mac_cntr + 1);
    instr.SetUpdate(run_mac_flag, run_mac_flag_next);
  }
  
}

}; // namespace ilang
