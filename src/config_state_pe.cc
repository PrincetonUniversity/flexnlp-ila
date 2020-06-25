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

// File: config_state_pe.cc

#include <ilang/ilang++.h>

#include <flex/pe_config.h>

namespace ilang {

namespace flex {

void DefinePEConfigState(Ila& m, const int& pe_idx) {

  // PE specific
  // - ActUnit manager
  // - ActUnit vector 0-15
  // - ActUnit vector 16-31
  // - Core cluster table first
  // - Core cluster table second
  // - Core memory manager first
  // - Core memory manager second
  // - Core RNN layer sizing

  // ActUnit manager
  m.NewBvState(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_VALID),
               ACT_MNGR_CONFIG_REG_IS_VALID_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST),
               ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS),
               ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_NUM_INST),
               ACT_MNGR_CONFIG_REG_NUM_INST_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_NUM_OUTPUT),
               ACT_MNGR_CONFIG_REG_NUM_OUTPUT_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_BUFFER_ADDR_BASE),
               ACT_MNGR_CONFIG_REG_BUFFER_ADDR_BASE_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_OUTPUT_ADDR_BASE),
               ACT_MNGR_CONFIG_REG_OUTPUT_ADDR_BASE_WIDTH);

  // ActUnit vector 0-15
  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST0),
               ACT_VECTOR_0_15_CONFIG_REG_INST0_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST1),
               ACT_VECTOR_0_15_CONFIG_REG_INST1_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST2),
               ACT_VECTOR_0_15_CONFIG_REG_INST2_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST3),
               ACT_VECTOR_0_15_CONFIG_REG_INST3_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST4),
               ACT_VECTOR_0_15_CONFIG_REG_INST4_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST5),
               ACT_VECTOR_0_15_CONFIG_REG_INST5_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST6),
               ACT_VECTOR_0_15_CONFIG_REG_INST6_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST7),
               ACT_VECTOR_0_15_CONFIG_REG_INST7_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST8),
               ACT_VECTOR_0_15_CONFIG_REG_INST8_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST9),
               ACT_VECTOR_0_15_CONFIG_REG_INST9_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST10),
               ACT_VECTOR_0_15_CONFIG_REG_INST10_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST11),
               ACT_VECTOR_0_15_CONFIG_REG_INST11_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST12),
               ACT_VECTOR_0_15_CONFIG_REG_INST12_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST13),
               ACT_VECTOR_0_15_CONFIG_REG_INST13_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST14),
               ACT_VECTOR_0_15_CONFIG_REG_INST14_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST15),
               ACT_VECTOR_0_15_CONFIG_REG_INST15_WIDTH);

  // ActUnit vector 16-31
  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST0),
               ACT_VECTOR_16_31_CONFIG_REG_INST0_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST1),
               ACT_VECTOR_16_31_CONFIG_REG_INST1_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST2),
               ACT_VECTOR_16_31_CONFIG_REG_INST2_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST3),
               ACT_VECTOR_16_31_CONFIG_REG_INST3_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST4),
               ACT_VECTOR_16_31_CONFIG_REG_INST4_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST5),
               ACT_VECTOR_16_31_CONFIG_REG_INST5_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST6),
               ACT_VECTOR_16_31_CONFIG_REG_INST6_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST7),
               ACT_VECTOR_16_31_CONFIG_REG_INST7_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST8),
               ACT_VECTOR_16_31_CONFIG_REG_INST8_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST9),
               ACT_VECTOR_16_31_CONFIG_REG_INST9_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST10),
               ACT_VECTOR_16_31_CONFIG_REG_INST10_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST11),
               ACT_VECTOR_16_31_CONFIG_REG_INST11_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST12),
               ACT_VECTOR_16_31_CONFIG_REG_INST12_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST13),
               ACT_VECTOR_16_31_CONFIG_REG_INST13_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST14),
               ACT_VECTOR_16_31_CONFIG_REG_INST14_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST15),
               ACT_VECTOR_16_31_CONFIG_REG_INST15_WIDTH);

  // Core cluster table first
  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY0),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY0_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY1),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY1_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY2),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY2_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY3),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY3_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY4),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY4_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY5),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY5_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY6),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY6_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY7),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY7_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY8),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY8_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY9),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY9_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY10),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY10_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY11),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY11_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY12),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY12_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY13),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY13_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY14),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY14_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY15),
               CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY15_WIDTH);

  //  Core cluster table second
  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY0),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY0_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY1),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY1_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY2),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY2_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY3),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY3_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY4),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY4_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY5),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY5_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY6),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY6_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY7),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY7_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY8),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY8_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY9),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY9_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY10),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY10_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY11),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY11_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY12),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY12_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY13),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY13_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY14),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY14_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY15),
               CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY15_WIDTH);

  // Core memory manager first
  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ZERO_ACTIVE),
               MEM_MNGR_FIRST_CONFIG_REG_ZERO_ACTIVE_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_W),
               MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_W_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_B),
               MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_B_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_I),
               MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_I_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_NUM_INPUT),
               MEM_MNGR_FIRST_CONFIG_REG_NUM_INPUT_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_WEIGHT),
               MEM_MNGR_FIRST_CONFIG_REG_BASE_WEIGHT_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_BIAS),
               MEM_MNGR_FIRST_CONFIG_REG_BASE_BIAS_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_INPUT),
               MEM_MNGR_FIRST_CONFIG_REG_BASE_INPUT_WIDTH);

  // Core memory manager second
  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ZERO_ACTIVE),
               MEM_MNGR_SECOND_CONFIG_REG_ZERO_ACTIVE_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_W),
               MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_W_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_B),
               MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_B_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_I),
               MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_I_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_NUM_INPUT),
               MEM_MNGR_SECOND_CONFIG_REG_NUM_INPUT_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_WEIGHT),
               MEM_MNGR_SECOND_CONFIG_REG_BASE_WEIGHT_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_BIAS),
               MEM_MNGR_SECOND_CONFIG_REG_BASE_BIAS_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_INPUT),
               MEM_MNGR_SECOND_CONFIG_REG_BASE_INPUT_WIDTH);

  // Core RNN layer sizing
  m.NewBvState(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_VALID),
               RNN_LAYER_SIZING_CONFIG_REG_IS_VALID_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_ZERO_FIRST),
               RNN_LAYER_SIZING_CONFIG_REG_IS_ZERO_FIRST_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER),
               RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_BIAS),
               RNN_LAYER_SIZING_CONFIG_REG_IS_BIAS_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_NUM_MANAGER),
               RNN_LAYER_SIZING_CONFIG_REG_NUM_MANAGER_WIDTH);

  m.NewBvState(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_NUM_OUTPUT),
               RNN_LAYER_SIZING_CONFIG_REG_NUM_OUTPUT_WIDTH);
};

} // namespace flex

} // namespace ilang
