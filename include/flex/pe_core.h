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

// File: pe_core.h

#ifndef FLEX_PE_CORE_H__
#define FLEX_PE_CORE_H__

namespace ilang {

// read 128-bits (16 bytes) at a time
#define CORE_SCALAR 16

// weight buffer (1 MB)
#define CORE_WEIGHT_BUFFER "core_weight_buffer"
#define CORE_WEIGHT_NUM_BANKS 16
#define CORE_WEIGHT_ENTRIES_PER_BANK 4096
#define CORE_WEIGHT_BUFFER_SIZE                                                \
  (CORE_SCALAR * CORE_WEIGHT_NUM_BANKS * CORE_WEIGHT_ENTRIES_PER_BANK)

// input buffer (4 KB)
#define CORE_INPUT_BUFFER "core_input_buffer"
#define CORE_INPUT_NUM_BANKS 16
#define CORE_INPUT_ENTRIES_PER_BANK 4096
#define CORE_INPUT_BUFFER_SIZE                                                 \
  (CORE_SCALAR * CORE_INPUT_NUM_BANKS * CORE_INPUT_ENTRIES_PER_BANK)

//
// PE core cluster table first
//

// config register: Entry0
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY0 "cluster_table_first_Entry0"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY0_WIDTH 8

// config register: Entry1
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY1 "cluster_table_first_Entry1"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY1_WIDTH 8

// config register: Entry2
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY2 "cluster_table_first_Entry2"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY2_WIDTH 8

// config register: Entry3
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY3 "cluster_table_first_Entry3"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY3_WIDTH 8

// config register: Entry4
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY4 "cluster_table_first_Entry4"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY4_WIDTH 8

// config register: Entry5
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY5 "cluster_table_first_Entry5"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY5_WIDTH 8

// config register: Entry6
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY6 "cluster_table_first_Entry6"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY6_WIDTH 8

// config register: Entry7
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY7 "cluster_table_first_Entry7"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY7_WIDTH 8

// config register: Entry8
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY8 "cluster_table_first_Entry8"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY8_WIDTH 8

// config register: Entry9
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY9 "cluster_table_first_Entry9"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY9_WIDTH 8

// config register: Entry10
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY10 "cluster_table_first_Entry10"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY10_WIDTH 8

// config register: Entry11
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY11 "cluster_table_first_Entry11"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY11_WIDTH 8

// config register: Entry12
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY12 "cluster_table_first_Entry12"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY12_WIDTH 8

// config register: Entry13
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY13 "cluster_table_first_Entry13"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY13_WIDTH 8

// config register: Entry14
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY14 "cluster_table_first_Entry14"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY14_WIDTH 8

// config register: Entry15
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY15 "cluster_table_first_Entry15"
#define CLUSTER_TABLE_FIRST_CONFIG_REG_ENTRY15_WIDTH 8

//
// PE core cluster table second
//

// config register: Entry0
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY0 "cluster_table_second_Entry0"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY0_WIDTH 8

// config register: Entry1
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY1 "cluster_table_second_Entry1"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY1_WIDTH 8

// config register: Entry2
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY2 "cluster_table_second_Entry2"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY2_WIDTH 8

// config register: Entry3
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY3 "cluster_table_second_Entry3"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY3_WIDTH 8

// config register: Entry4
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY4 "cluster_table_second_Entry4"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY4_WIDTH 8

// config register: Entry5
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY5 "cluster_table_second_Entry5"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY5_WIDTH 8

// config register: Entry6
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY6 "cluster_table_second_Entry6"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY6_WIDTH 8

// config register: Entry7
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY7 "cluster_table_second_Entry7"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY7_WIDTH 8

// config register: Entry8
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY8 "cluster_table_second_Entry8"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY8_WIDTH 8

// config register: Entry9
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY9 "cluster_table_second_Entry9"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY9_WIDTH 8

// config register: Entry10
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY10 "cluster_table_second_Entry10"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY10_WIDTH 8

// config register: Entry11
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY11 "cluster_table_second_Entry11"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY11_WIDTH 8

// config register: Entry12
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY12 "cluster_table_second_Entry12"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY12_WIDTH 8

// config register: Entry13
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY13 "cluster_table_second_Entry13"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY13_WIDTH 8

// config register: Entry14
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY14 "cluster_table_second_Entry14"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY14_WIDTH 8

// config register: Entry15
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY15 "cluster_table_second_Entry15"
#define CLUSTER_TABLE_SECOND_CONFIG_REG_ENTRY15_WIDTH 8

//
// PE core memory manager first
//

// config register: zero_active
#define MEM_MNGR_FIRST_CONFIG_REG_ZERO_ACTIVE "mem_mngr_first_zero_active"
#define MEM_MNGR_FIRST_CONFIG_REG_ZERO_ACTIVE_WIDTH 1

// config register: adpfloat_bias_w
#define MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_W                              \
  "mem_mngr_first_adpfloat_bias_w"
#define MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_W_WIDTH 3

// config register: adpfloat_bias_b
#define MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_B                              \
  "mem_mngr_first_adpfloat_bias_b"
#define MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_B_WIDTH 3

// config register: adpfloat_bias_i
#define MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_I                              \
  "mem_mngr_first_adpfloat_bias_i"
#define MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_I_WIDTH 3

// config register: num_input
#define MEM_MNGR_FIRST_CONFIG_REG_NUM_INPUT "mem_mngr_first_num_input"
#define MEM_MNGR_FIRST_CONFIG_REG_NUM_INPUT_WIDTH 8

// config register: base_weight
#define MEM_MNGR_FIRST_CONFIG_REG_BASE_WEIGHT "mem_mngr_first_base_weight"
#define MEM_MNGR_FIRST_CONFIG_REG_BASE_WEIGHT_WIDTH 16

// config register: base_bias
#define MEM_MNGR_FIRST_CONFIG_REG_BASE_BIAS "mem_mngr_first_base_bias"
#define MEM_MNGR_FIRST_CONFIG_REG_BASE_BIAS_WIDTH 16

// config register: base_input
#define MEM_MNGR_FIRST_CONFIG_REG_BASE_INPUT "mem_mngr_first_base_input"
#define MEM_MNGR_FIRST_CONFIG_REG_BASE_INPUT_WIDTH 16

//
// PE core memory manager second
//

// config register: zero_active
#define MEM_MNGR_SECOND_CONFIG_REG_ZERO_ACTIVE "mem_mngr_second_zero_active"
#define MEM_MNGR_SECOND_CONFIG_REG_ZERO_ACTIVE_WIDTH 1

// config register: adpfloat_bias_w
#define MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_W                             \
  "mem_mngr_second_adpfloat_bias_w"
#define MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_W_WIDTH 3

// config register: adpfloat_bias_b
#define MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_B                             \
  "mem_mngr_second_adpfloat_bias_b"
#define MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_B_WIDTH 3

// config register: adpfloat_bias_i
#define MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_I                             \
  "mem_mngr_second_adpfloat_bias_i"
#define MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_I_WIDTH 3

// config register: num_input
#define MEM_MNGR_SECOND_CONFIG_REG_NUM_INPUT "mem_mngr_second_num_input"
#define MEM_MNGR_SECOND_CONFIG_REG_NUM_INPUT_WIDTH 8

// config register: base_weight
#define MEM_MNGR_SECOND_CONFIG_REG_BASE_WEIGHT "mem_mngr_second_base_weight"
#define MEM_MNGR_SECOND_CONFIG_REG_BASE_WEIGHT_WIDTH 16

// config register: base_bias
#define MEM_MNGR_SECOND_CONFIG_REG_BASE_BIAS "mem_mngr_second_base_bias"
#define MEM_MNGR_SECOND_CONFIG_REG_BASE_BIAS_WIDTH 16

// config register: base_input
#define MEM_MNGR_SECOND_CONFIG_REG_BASE_INPUT "mem_mngr_second_base_input"
#define MEM_MNGR_SECOND_CONFIG_REG_BASE_INPUT_WIDTH 16

//
// PE core RNN layer sizing
//

// config register: is_valid
#define RNN_LAYER_SIZING_CONFIG_REG_IS_VALID "rnn_layer_sizing_is_valid"
#define RNN_LAYER_SIZING_CONFIG_REG_IS_VALID_WIDTH 1

// config register: is_zero_first
#define RNN_LAYER_SIZING_CONFIG_REG_IS_ZERO_FIRST                              \
  "rnn_layer_sizing_is_zero_first"
#define RNN_LAYER_SIZING_CONFIG_REG_IS_ZERO_FIRST_WIDTH 1

// config register: is_cluster
#define RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER "rnn_layer_sizing_is_cluster"
#define RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER_WIDTH 1

// config register: is_bias
#define RNN_LAYER_SIZING_CONFIG_REG_IS_BIAS "rnn_layer_sizing_is_bias"
#define RNN_LAYER_SIZING_CONFIG_REG_IS_BIAS_WIDTH 1

// config register: num_manager
#define RNN_LAYER_SIZING_CONFIG_REG_NUM_MANAGER "rnn_layer_sizing_num_manager"
#define RNN_LAYER_SIZING_CONFIG_REG_NUM_MANAGER_WIDTH 4

// config register: num_output
#define RNN_LAYER_SIZING_CONFIG_REG_NUM_OUTPUT "rnn_layer_sizing_num_output"
#define RNN_LAYER_SIZING_CONFIG_REG_NUM_OUTPUT_WIDTH 8

}; // namespace ilang

#endif // FLEX_PE_CORE_H__
