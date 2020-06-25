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

// File: config_instr_pe.cc

#include <ilang/ilang++.h>

#include <flex/flex.h>
#include <flex/util.h>

namespace ilang {

namespace flex {

void DefinePEConfigInstr(Ila& m, const int& pe_idx, const uint64_t& host_base) {
  auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);

  // PE specific
  // - ActUnit manager
  // - ActUnit vector 0-15
  // - ActUnit vector 16-31
  // - Core cluster table first
  // - Core cluster table second
  // - Core memory manager first
  // - Core memory manager second
  // - Core RNN layer sizing

  { // ACT_MNGR
    auto instr = m.NewInstr(PEGetInstrName(pe_idx, "CONFIG_ACT_MNGR"));
    SetDecodeForConfigWr(host_base + CONFIG_ADDR_PE_ACT_MNGR);

    // update for is_valid
    auto is_valid_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(m, instr, is_valid_next,
                         PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_VALID));

    // update for is_zero_first
    auto is_zero_first_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(
        m, instr, is_zero_first_next,
        PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST));

    // update for adpfloat_bias
    auto adpfloat_bias_next = m.input(TOP_DATA_IN_2);
    SetUpdateForConfigWr(
        m, instr, adpfloat_bias_next,
        PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS));

    // update for num_inst
    auto num_inst_next = m.input(TOP_DATA_IN_3);
    SetUpdateForConfigWr(m, instr, num_inst_next,
                         PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_NUM_INST));

    // update for num_output
    auto num_output_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(m, instr, num_output_next,
                         PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_NUM_OUTPUT));

    // update for buffer_addr_base
    auto buffer_addr_base_next =
        Concat(m.input(TOP_DATA_IN_7), m.input(TOP_DATA_IN_6));
    SetUpdateForConfigWr(
        m, instr, buffer_addr_base_next,
        PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_BUFFER_ADDR_BASE));

    // update for output_addr_base
    auto output_addr_base_next = m.input(TOP_DATA_IN_8);
    SetUpdateForConfigWr(
        m, instr, output_addr_base_next,
        PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_OUTPUT_ADDR_BASE));
  }

  { // ACT_VECTOR_0_15
    auto instr = m.NewInstr(PEGetInstrName(pe_idx, "CONFIG_ACT_VECTOR_0_15"));
    SetDecodeForConfigWr(host_base + CONFIG_ADDR_PE_ACT_VECTOR_0_15);

    // update for Inst0
    auto Inst0_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(
        m, instr, Inst0_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST0));

    // update for Inst1
    auto Inst1_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(
        m, instr, Inst1_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST1));

    // update for Inst2
    auto Inst2_next = m.input(TOP_DATA_IN_2);
    SetUpdateForConfigWr(
        m, instr, Inst2_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST2));

    // update for Inst3
    auto Inst3_next = m.input(TOP_DATA_IN_3);
    SetUpdateForConfigWr(
        m, instr, Inst3_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST3));

    // update for Inst4
    auto Inst4_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(
        m, instr, Inst4_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST4));

    // update for Inst5
    auto Inst5_next = m.input(TOP_DATA_IN_5);
    SetUpdateForConfigWr(
        m, instr, Inst5_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST5));

    // update for Inst6
    auto Inst6_next = m.input(TOP_DATA_IN_6);
    SetUpdateForConfigWr(
        m, instr, Inst6_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST6));

    // update for Inst7
    auto Inst7_next = m.input(TOP_DATA_IN_7);
    SetUpdateForConfigWr(
        m, instr, Inst7_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST7));

    // update for Inst8
    auto Inst8_next = m.input(TOP_DATA_IN_8);
    SetUpdateForConfigWr(
        m, instr, Inst8_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST8));

    // update for Inst9
    auto Inst9_next = m.input(TOP_DATA_IN_9);
    SetUpdateForConfigWr(
        m, instr, Inst9_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST9));

    // update for Inst10
    auto Inst10_next = m.input(TOP_DATA_IN_10);
    SetUpdateForConfigWr(
        m, instr, Inst10_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST10));

    // update for Inst11
    auto Inst11_next = m.input(TOP_DATA_IN_11);
    SetUpdateForConfigWr(
        m, instr, Inst11_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST11));

    // update for Inst12
    auto Inst12_next = m.input(TOP_DATA_IN_12);
    SetUpdateForConfigWr(
        m, instr, Inst12_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST12));

    // update for Inst13
    auto Inst13_next = m.input(TOP_DATA_IN_13);
    SetUpdateForConfigWr(
        m, instr, Inst13_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST13));

    // update for Inst14
    auto Inst14_next = m.input(TOP_DATA_IN_14);
    SetUpdateForConfigWr(
        m, instr, Inst14_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST14));

    // update for Inst15
    auto Inst15_next = m.input(TOP_DATA_IN_15);
    SetUpdateForConfigWr(
        m, instr, Inst15_next,
        PEGetVarName(pe_idx, ACT_VECTOR_0_15_CONFIG_REG_INST15));
  }

  { // ACT_VECTOR_16_31
    auto instr = m.NewInstr(PEGetInstrName(pe_idx, "CONFIG_ACT_VECTOR_16_31"));
    SetDecodeForConfigWr(host_base + CONFIG_ADDR_PE_ACT_VECTOR_16_31);

    // update for Inst0
    auto Inst0_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(
        m, instr, Inst0_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST0));

    // update for Inst1
    auto Inst1_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(
        m, instr, Inst1_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST1));

    // update for Inst2
    auto Inst2_next = m.input(TOP_DATA_IN_2);
    SetUpdateForConfigWr(
        m, instr, Inst2_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST2));

    // update for Inst3
    auto Inst3_next = m.input(TOP_DATA_IN_3);
    SetUpdateForConfigWr(
        m, instr, Inst3_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST3));

    // update for Inst4
    auto Inst4_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(
        m, instr, Inst4_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST4));

    // update for Inst5
    auto Inst5_next = m.input(TOP_DATA_IN_5);
    SetUpdateForConfigWr(
        m, instr, Inst5_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST5));

    // update for Inst6
    auto Inst6_next = m.input(TOP_DATA_IN_6);
    SetUpdateForConfigWr(
        m, instr, Inst6_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST6));

    // update for Inst7
    auto Inst7_next = m.input(TOP_DATA_IN_7);
    SetUpdateForConfigWr(
        m, instr, Inst7_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST7));

    // update for Inst8
    auto Inst8_next = m.input(TOP_DATA_IN_8);
    SetUpdateForConfigWr(
        m, instr, Inst8_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST8));

    // update for Inst9
    auto Inst9_next = m.input(TOP_DATA_IN_9);
    SetUpdateForConfigWr(
        m, instr, Inst9_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST9));

    // update for Inst10
    auto Inst10_next = m.input(TOP_DATA_IN_10);
    SetUpdateForConfigWr(
        m, instr, Inst10_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST10));

    // update for Inst11
    auto Inst11_next = m.input(TOP_DATA_IN_11);
    SetUpdateForConfigWr(
        m, instr, Inst11_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST11));

    // update for Inst12
    auto Inst12_next = m.input(TOP_DATA_IN_12);
    SetUpdateForConfigWr(
        m, instr, Inst12_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST12));

    // update for Inst13
    auto Inst13_next = m.input(TOP_DATA_IN_13);
    SetUpdateForConfigWr(
        m, instr, Inst13_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST13));

    // update for Inst14
    auto Inst14_next = m.input(TOP_DATA_IN_14);
    SetUpdateForConfigWr(
        m, instr, Inst14_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST14));

    // update for Inst15
    auto Inst15_next = m.input(TOP_DATA_IN_15);
    SetUpdateForConfigWr(
        m, instr, Inst15_next,
        PEGetVarName(pe_idx, ACT_VECTOR_16_31_CONFIG_REG_INST15));
  }

  { // CLUSTER_TABLE_FIRST
    auto instr =
        m.NewInstr(PEGetInstrName(pe_idx, "CONFIG_CLUSTER_TABLE_FIRST"));
    SetDecodeForConfigWr(host_base + CONFIG_ADDR_PE_CLUSTER_TABLE_FIRST);

    // update for Entry0
    auto Entry0_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(
        m, instr, Entry0_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY0));

    // update for Entry1
    auto Entry1_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(
        m, instr, Entry1_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY1));

    // update for Entry2
    auto Entry2_next = m.input(TOP_DATA_IN_2);
    SetUpdateForConfigWr(
        m, instr, Entry2_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY2));

    // update for Entry3
    auto Entry3_next = m.input(TOP_DATA_IN_3);
    SetUpdateForConfigWr(
        m, instr, Entry3_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY3));

    // update for Entry4
    auto Entry4_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(
        m, instr, Entry4_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY4));

    // update for Entry5
    auto Entry5_next = m.input(TOP_DATA_IN_5);
    SetUpdateForConfigWr(
        m, instr, Entry5_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY5));

    // update for Entry6
    auto Entry6_next = m.input(TOP_DATA_IN_6);
    SetUpdateForConfigWr(
        m, instr, Entry6_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY6));

    // update for Entry7
    auto Entry7_next = m.input(TOP_DATA_IN_7);
    SetUpdateForConfigWr(
        m, instr, Entry7_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY7));

    // update for Entry8
    auto Entry8_next = m.input(TOP_DATA_IN_8);
    SetUpdateForConfigWr(
        m, instr, Entry8_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY8));

    // update for Entry9
    auto Entry9_next = m.input(TOP_DATA_IN_9);
    SetUpdateForConfigWr(
        m, instr, Entry9_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY9));

    // update for Entry10
    auto Entry10_next = m.input(TOP_DATA_IN_10);
    SetUpdateForConfigWr(
        m, instr, Entry10_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY10));

    // update for Entry11
    auto Entry11_next = m.input(TOP_DATA_IN_11);
    SetUpdateForConfigWr(
        m, instr, Entry11_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY11));

    // update for Entry12
    auto Entry12_next = m.input(TOP_DATA_IN_12);
    SetUpdateForConfigWr(
        m, instr, Entry12_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY12));

    // update for Entry13
    auto Entry13_next = m.input(TOP_DATA_IN_13);
    SetUpdateForConfigWr(
        m, instr, Entry13_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY13));

    // update for Entry14
    auto Entry14_next = m.input(TOP_DATA_IN_14);
    SetUpdateForConfigWr(
        m, instr, Entry14_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY14));

    // update for Entry15
    auto Entry15_next = m.input(TOP_DATA_IN_15);
    SetUpdateForConfigWr(
        m, instr, Entry15_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY15));
  }

  { // CLUSTER_TABLE_SECOND
    auto instr =
        m.NewInstr(PEGetInstrName(pe_idx, "CONFIG_CLUSTER_TABLE_SECOND"));
    SetDecodeForConfigWr(host_base + CONFIG_ADDR_PE_CLUSTER_TABLE_SECOND);

    // update for Entry0
    auto Entry0_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(
        m, instr, Entry0_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY0));

    // update for Entry1
    auto Entry1_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(
        m, instr, Entry1_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY1));

    // update for Entry2
    auto Entry2_next = m.input(TOP_DATA_IN_2);
    SetUpdateForConfigWr(
        m, instr, Entry2_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY2));

    // update for Entry3
    auto Entry3_next = m.input(TOP_DATA_IN_3);
    SetUpdateForConfigWr(
        m, instr, Entry3_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY3));

    // update for Entry4
    auto Entry4_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(
        m, instr, Entry4_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY4));

    // update for Entry5
    auto Entry5_next = m.input(TOP_DATA_IN_5);
    SetUpdateForConfigWr(
        m, instr, Entry5_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY5));

    // update for Entry6
    auto Entry6_next = m.input(TOP_DATA_IN_6);
    SetUpdateForConfigWr(
        m, instr, Entry6_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY6));

    // update for Entry7
    auto Entry7_next = m.input(TOP_DATA_IN_7);
    SetUpdateForConfigWr(
        m, instr, Entry7_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY7));

    // update for Entry8
    auto Entry8_next = m.input(TOP_DATA_IN_8);
    SetUpdateForConfigWr(
        m, instr, Entry8_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY8));

    // update for Entry9
    auto Entry9_next = m.input(TOP_DATA_IN_9);
    SetUpdateForConfigWr(
        m, instr, Entry9_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY9));

    // update for Entry10
    auto Entry10_next = m.input(TOP_DATA_IN_10);
    SetUpdateForConfigWr(
        m, instr, Entry10_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY10));

    // update for Entry11
    auto Entry11_next = m.input(TOP_DATA_IN_11);
    SetUpdateForConfigWr(
        m, instr, Entry11_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY11));

    // update for Entry12
    auto Entry12_next = m.input(TOP_DATA_IN_12);
    SetUpdateForConfigWr(
        m, instr, Entry12_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY12));

    // update for Entry13
    auto Entry13_next = m.input(TOP_DATA_IN_13);
    SetUpdateForConfigWr(
        m, instr, Entry13_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY13));

    // update for Entry14
    auto Entry14_next = m.input(TOP_DATA_IN_14);
    SetUpdateForConfigWr(
        m, instr, Entry14_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY14));

    // update for Entry15
    auto Entry15_next = m.input(TOP_DATA_IN_15);
    SetUpdateForConfigWr(
        m, instr, Entry15_next,
        PEGetVarName(pe_idx, CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY15));
  }

  { // MEM_MNGR_FIRST
    auto instr = m.NewInstr(PEGetInstrName(pe_idx, "CONFIG_MEM_MNGR_FIRST"));
    SetDecodeForConfigWr(host_base + CONFIG_ADDR_PE_MEM_MNGR_FIRST);

    // update for zero_active
    auto zero_active_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(
        m, instr, zero_active_next,
        PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ZERO_ACTIVE));

    // update for adpfloat_bias_w
    auto adpfloat_bias_w_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(
        m, instr, adpfloat_bias_w_next,
        PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_W));

    // update for adpfloat_bias_b
    auto adpfloat_bias_b_next = m.input(TOP_DATA_IN_2);
    SetUpdateForConfigWr(
        m, instr, adpfloat_bias_b_next,
        PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_B));

    // update for adpfloat_bias_i
    auto adpfloat_bias_i_next = m.input(TOP_DATA_IN_3);
    SetUpdateForConfigWr(
        m, instr, adpfloat_bias_i_next,
        PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_I));

    // update for num_input
    auto num_input_next =
        Concat(m.input(TOP_DATA_IN_5), m.input(TOP_DATA_IN_4));
    SetUpdateForConfigWr(
        m, instr, num_input_next,
        PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_NUM_INPUT));

    // update for base_weight
    auto base_weight_next =
        Concat(m.input(TOP_DATA_IN_7), m.input(TOP_DATA_IN_6));
    SetUpdateForConfigWr(
        m, instr, base_weight_next,
        PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_WEIGHT));

    // update for base_bias
    auto base_bias_next =
        Concat(m.input(TOP_DATA_IN_9), m.input(TOP_DATA_IN_8));
    SetUpdateForConfigWr(
        m, instr, base_bias_next,
        PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_BIAS));

    // update for base_input
    auto base_input_next =
        Concat(m.input(TOP_DATA_IN_11), m.input(TOP_DATA_IN_10));
    SetUpdateForConfigWr(
        m, instr, base_input_next,
        PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_INPUT));
  }

  { // MEM_MNGR_SECOND
    auto instr = m.NewInstr(PEGetInstrName(pe_idx, "CONFIG_MEM_MNGR_SECOND"));
    SetDecodeForConfigWr(host_base + CONFIG_ADDR_PE_MEM_MNGR_SECOND);

    // update for zero_active
    auto zero_active_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(
        m, instr, zero_active_next,
        PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ZERO_ACTIVE));

    // update for adpfloat_bias_w
    auto adpfloat_bias_w_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(
        m, instr, adpfloat_bias_w_next,
        PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_W));

    // update for adpfloat_bias_b
    auto adpfloat_bias_b_next = m.input(TOP_DATA_IN_2);
    SetUpdateForConfigWr(
        m, instr, adpfloat_bias_b_next,
        PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_B));

    // update for adpfloat_bias_i
    auto adpfloat_bias_i_next = m.input(TOP_DATA_IN_3);
    SetUpdateForConfigWr(
        m, instr, adpfloat_bias_i_next,
        PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_I));

    // update for num_input
    auto num_input_next =
        Concat(m.input(TOP_DATA_IN_5), m.input(TOP_DATA_IN_4));
    SetUpdateForConfigWr(
        m, instr, num_input_next,
        PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_NUM_INPUT));

    // update for base_weight
    auto base_weight_next =
        Concat(m.input(TOP_DATA_IN_7), m.input(TOP_DATA_IN_6));
    SetUpdateForConfigWr(
        m, instr, base_weight_next,
        PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_WEIGHT));

    // update for base_bias
    auto base_bias_next =
        Concat(m.input(TOP_DATA_IN_9), m.input(TOP_DATA_IN_8));
    SetUpdateForConfigWr(
        m, instr, base_bias_next,
        PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_BIAS));

    // update for base_input
    auto base_input_next =
        Concat(m.input(TOP_DATA_IN_11), m.input(TOP_DATA_IN_10));
    SetUpdateForConfigWr(
        m, instr, base_input_next,
        PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_INPUT));
  }

  { // RNN_LAYER_SIZING
    auto instr = m.NewInstr(PEGetInstrName(pe_idx, "CONFIG_RNN_LAYER_SIZING"));
    SetDecodeForConfigWr(host_base + CONFIG_ADDR_PE_RNN_LAYER_SIZING);

    // update for is_valid
    auto is_valid_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(
        m, instr, is_valid_next,
        PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_VALID));

    // update for is_zero_first
    auto is_zero_first_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(
        m, instr, is_zero_first_next,
        PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_ZERO_FIRST));

    // update for is_cluster
    auto is_cluster_next = m.input(TOP_DATA_IN_2);
    SetUpdateForConfigWr(
        m, instr, is_cluster_next,
        PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER));

    // update for is_bias
    auto is_bias_next = m.input(TOP_DATA_IN_3);
    SetUpdateForConfigWr(
        m, instr, is_bias_next,
        PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_BIAS));

    // update for num_manager
    auto num_manager_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(
        m, instr, num_manager_next,
        PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_NUM_MANAGER));

    // update for num_output
    auto num_output_next = m.input(TOP_DATA_IN_5);
    SetUpdateForConfigWr(
        m, instr, num_output_next,
        PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_NUM_OUTPUT));
  }
}

} // namespace flex

} // namespace ilang
