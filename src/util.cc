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


}; // namespace ilang
