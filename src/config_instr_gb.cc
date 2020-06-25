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

// File: config_instr_gb.cc

#include <ilang/ilang++.h>

#include <flex/flex.h>
#include <flex/util.h>

namespace ilang {

namespace flex {

void DefineGBConfigInstr(Ila& m) {
  auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);

  // GB specific
  // - GB attention
  // - GB control
  // - GB core memory manager large
  // - GB core memory manager small
  // - GB layer norm
  // - GB layer reduce
  // - GB zero padding

  { // GB_ATTENTION
    auto instr = m.NewInstr("CONFIG_GB_ATTENTION");
    SetDecodeForConfigWr(CONFIG_ADDR_GB_ATTENTION);

    // update for is_valid
    auto is_valid_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(m, instr, is_valid_next,
                         GB_ATTENTION_CONFIG_REG_IS_VALID);

    // update for memory_index_1
    auto memory_index_1_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(m, instr, memory_index_1_next,
                         GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1);

    // update for memory_index_2
    auto memory_index_2_next = m.input(TOP_DATA_IN_5);
    SetUpdateForConfigWr(m, instr, memory_index_2_next,
                         GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_2);

    // update for num_vector_1
    auto num_vector_1_next = m.input(TOP_DATA_IN_6);
    SetUpdateForConfigWr(m, instr, num_vector_1_next,
                         GB_ATTENTION_CONFIG_REG_NUM_VECTOR_1);

    // update for num_timestep_1
    auto num_timestep_1_next =
        Concat(m.input(TOP_DATA_IN_9), m.input(TOP_DATA_IN_8));
    SetUpdateForConfigWr(m, instr, num_timestep_1_next,
                         GB_ATTENTION_CONFIG_REG_NUM_TIMESTEP_1);

    // update for adpbias_1
    auto adpbias_1_next = m.input(TOP_DATA_IN_12);
    SetUpdateForConfigWr(m, instr, adpbias_1_next,
                         GB_ATTENTION_CONFIG_REG_ADPBIAS_1);

    // update for adpbias_2
    auto adpbias_2_next = m.input(TOP_DATA_IN_13);
    SetUpdateForConfigWr(m, instr, adpbias_2_next,
                         GB_ATTENTION_CONFIG_REG_ADPBIAS_2);

    // update for adpbias_3
    auto adpbias_3_next = m.input(TOP_DATA_IN_14);
    SetUpdateForConfigWr(m, instr, adpbias_3_next,
                         GB_ATTENTION_CONFIG_REG_ADPBIAS_3);

    // update for adpbias_4
    auto adpbias_4_next = m.input(TOP_DATA_IN_15);
    SetUpdateForConfigWr(m, instr, adpbias_4_next,
                         GB_ATTENTION_CONFIG_REG_ADPBIAS_4);
  }

  { // GB_CONTROL
    auto instr = m.NewInstr("CONFIG_GB_CONTROL");
    SetDecodeForConfigWr(CONFIG_ADDR_GB_CONTROL);

    // update for is_valid
    auto is_valid_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(m, instr, is_valid_next,
                         GB_CONTROL_CONFIG_REG_IS_VALID);

    // update for mode
    auto mode_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(m, instr, mode_next, GB_CONTROL_CONFIG_REG_MODE);

    // update for is_rnn
    auto is_rnn_next = m.input(TOP_DATA_IN_2);
    SetUpdateForConfigWr(m, instr, is_rnn_next, GB_CONTROL_CONFIG_REG_IS_RNN);

    // update for memory_index_1
    auto memory_index_1_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(m, instr, memory_index_1_next,
                         GB_CONTROL_CONFIG_REG_MEMORY_INDEX_1);

    // update for memory_index_2
    auto memory_index_2_next = m.input(TOP_DATA_IN_5);
    SetUpdateForConfigWr(m, instr, memory_index_2_next,
                         GB_CONTROL_CONFIG_REG_MEMORY_INDEX_2);

    // update for num_vector_1
    auto num_vector_1_next = m.input(TOP_DATA_IN_6);
    SetUpdateForConfigWr(m, instr, num_vector_1_next,
                         GB_CONTROL_CONFIG_REG_NUM_VECTOR_1);

    // update for num_vector_2
    auto num_vector_2_next = m.input(TOP_DATA_IN_7);
    SetUpdateForConfigWr(m, instr, num_vector_2_next,
                         GB_CONTROL_CONFIG_REG_NUM_VECTOR_2);

    // update for num_timestep_1
    auto num_timestep_1_next =
        Concat(m.input(TOP_DATA_IN_9), m.input(TOP_DATA_IN_8));
    SetUpdateForConfigWr(m, instr, num_timestep_1_next,
                         GB_CONTROL_CONFIG_REG_NUM_TIMESTEP_1);
  }

  { // GB_CORE_MEM_MNGR_LARGE
    auto instr = m.NewInstr("CONFIG_GB_CORE_MEM_MNGR_LARGE");
    SetDecodeForConfigWr(CONFIG_ADDR_GB_CORE_MEM_MNGR_LARGE);

    // update for num_vector_0
    auto num_vector_0_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(m, instr, num_vector_0_next,
                         GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_0);

    // update for base_large_0
    auto base_large_0_next =
        Concat(m.input(TOP_DATA_IN_3), m.input(TOP_DATA_IN_2));
    SetUpdateForConfigWr(m, instr, base_large_0_next,
                         GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0);

    // update for num_vector_1
    auto num_vector_1_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(m, instr, num_vector_1_next,
                         GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_1);

    // update for base_large_1
    auto base_large_1_next =
        Concat(m.input(TOP_DATA_IN_7), m.input(TOP_DATA_IN_6));
    SetUpdateForConfigWr(m, instr, base_large_1_next,
                         GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1);

    // update for num_vector_2
    auto num_vector_2_next = m.input(TOP_DATA_IN_8);
    SetUpdateForConfigWr(m, instr, num_vector_2_next,
                         GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_2);

    // update for base_large_2
    auto base_large_2_next =
        Concat(m.input(TOP_DATA_IN_11), m.input(TOP_DATA_IN_10));
    SetUpdateForConfigWr(m, instr, base_large_2_next,
                         GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2);

    // update for num_vector_3
    auto num_vector_3_next = m.input(TOP_DATA_IN_12);
    SetUpdateForConfigWr(m, instr, num_vector_3_next,
                         GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_NUM_VECTOR_3);

    // update for base_large_3
    auto base_large_3_next =
        Concat(m.input(TOP_DATA_IN_15), m.input(TOP_DATA_IN_14));
    SetUpdateForConfigWr(m, instr, base_large_3_next,
                         GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3);
  }

  { // GB_CORE_MEM_MNGR_SMALL
    auto instr = m.NewInstr("CONFIG_GB_CORE_MEM_MNGR_SMALL");
    SetDecodeForConfigWr(CONFIG_ADDR_GB_CORE_MEM_MNGR_SMALL);

    // update for base_small_0
    auto base_small_0_next =
        Concat(m.input(TOP_DATA_IN_1), m.input(TOP_DATA_IN_0));
    SetUpdateForConfigWr(m, instr, base_small_0_next,
                         GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_0);

    // update for base_small_1
    auto base_small_1_next =
        Concat(m.input(TOP_DATA_IN_3), m.input(TOP_DATA_IN_2));
    SetUpdateForConfigWr(m, instr, base_small_1_next,
                         GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_1);

    // update for base_small_2
    auto base_small_2_next =
        Concat(m.input(TOP_DATA_IN_5), m.input(TOP_DATA_IN_4));
    SetUpdateForConfigWr(m, instr, base_small_2_next,
                         GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_2);

    // update for base_small_3
    auto base_small_3_next =
        Concat(m.input(TOP_DATA_IN_7), m.input(TOP_DATA_IN_6));
    SetUpdateForConfigWr(m, instr, base_small_3_next,
                         GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_3);

    // update for base_small_4
    auto base_small_4_next =
        Concat(m.input(TOP_DATA_IN_9), m.input(TOP_DATA_IN_8));
    SetUpdateForConfigWr(m, instr, base_small_4_next,
                         GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_4);

    // update for base_small_5
    auto base_small_5_next =
        Concat(m.input(TOP_DATA_IN_11), m.input(TOP_DATA_IN_10));
    SetUpdateForConfigWr(m, instr, base_small_5_next,
                         GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_5);

    // update for base_small_6
    auto base_small_6_next =
        Concat(m.input(TOP_DATA_IN_13), m.input(TOP_DATA_IN_12));
    SetUpdateForConfigWr(m, instr, base_small_6_next,
                         GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_6);

    // update for base_small_7
    auto base_small_7_next =
        Concat(m.input(TOP_DATA_IN_15), m.input(TOP_DATA_IN_14));
    SetUpdateForConfigWr(m, instr, base_small_7_next,
                         GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_7);
  }

  { // GB_LAYER_NORM
    auto instr = m.NewInstr("CONFIG_GB_LAYER_NORM");
    SetDecodeForConfigWr(CONFIG_ADDR_GB_LAYER_NORM);

    // update for is_valid
    auto is_valid_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(m, instr, is_valid_next,
                         GB_LAYER_NORM_CONFIG_REG_IS_VALID);

    // update for mode
    auto mode_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(m, instr, mode_next, GB_LAYER_NORM_CONFIG_REG_MODE);

    // update for memory_index_1
    auto memory_index_1_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(m, instr, memory_index_1_next,
                         GB_LAYER_NORM_CONFIG_REG_MEMORY_INDEX_1);

    // update for num_vector_1
    auto num_vector_1_next = m.input(TOP_DATA_IN_6);
    SetUpdateForConfigWr(m, instr, num_vector_1_next,
                         GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1);

    // update for num_timestep_1
    auto num_timestep_1_next =
        Concat(m.input(TOP_DATA_IN_9), m.input(TOP_DATA_IN_8));
    SetUpdateForConfigWr(m, instr, num_timestep_1_next,
                         GB_LAYER_NORM_CONFIG_REG_NUM_TIMESTEP_1);

    // update for adpbias_1
    auto adpbias_1_next = m.input(TOP_DATA_IN_12);
    SetUpdateForConfigWr(m, instr, adpbias_1_next,
                         GB_LAYER_NORM_CONFIG_REG_ADPBIAS_1);

    // update for adpbias_3
    auto adpbias_3_next = m.input(TOP_DATA_IN_14);
    SetUpdateForConfigWr(m, instr, adpbias_3_next,
                         GB_LAYER_NORM_CONFIG_REG_ADPBIAS_3);

    // update for adpbias_4
    auto adpbias_4_next = m.input(TOP_DATA_IN_15);
    SetUpdateForConfigWr(m, instr, adpbias_4_next,
                         GB_LAYER_NORM_CONFIG_REG_ADPBIAS_4);
  }

  { // GB_LAYER_REDUCE
    auto instr = m.NewInstr("CONFIG_GB_LAYER_REDUCE");
    SetDecodeForConfigWr(CONFIG_ADDR_GB_LAYER_REDUCE);

    // update for is_valid
    auto is_valid_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(m, instr, is_valid_next,
                         GB_LAYER_REDUCE_CONFIG_REG_IS_VALID);

    // update for mode
    auto mode_next = m.input(TOP_DATA_IN_1);
    SetUpdateForConfigWr(m, instr, mode_next, GB_LAYER_REDUCE_CONFIG_REG_MODE);

    // update for memory_index_1
    auto memory_index_1_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(m, instr, memory_index_1_next,
                         GB_LAYER_REDUCE_CONFIG_REG_MEMORY_INDEX_1);

    // update for num_vector_1
    auto num_vector_1_next = m.input(TOP_DATA_IN_6);
    SetUpdateForConfigWr(m, instr, num_vector_1_next,
                         GB_LAYER_REDUCE_CONFIG_REG_NUM_VECTOR_1);

    // update for num_timestep_1
    auto num_timestep_1_next =
        Concat(m.input(TOP_DATA_IN_9), m.input(TOP_DATA_IN_8));
    SetUpdateForConfigWr(m, instr, num_timestep_1_next,
                         GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1);
  }

  { // GB_ZERO_PADDING
    auto instr = m.NewInstr("CONFIG_GB_ZERO_PADDING");
    SetDecodeForConfigWr(CONFIG_ADDR_GB_ZERO_PADDING);

    // update for is_valid
    auto is_valid_next = m.input(TOP_DATA_IN_0);
    SetUpdateForConfigWr(m, instr, is_valid_next,
                         GB_ZERO_PADDING_CONFIG_REG_IS_VALID);

    // update for memory_index_1
    auto memory_index_1_next = m.input(TOP_DATA_IN_4);
    SetUpdateForConfigWr(m, instr, memory_index_1_next,
                         GB_ZERO_PADDING_CONFIG_REG_MEMORY_INDEX_1);

    // update for num_vector_1
    auto num_vector_1_next = m.input(TOP_DATA_IN_6);
    SetUpdateForConfigWr(m, instr, num_vector_1_next,
                         GB_ZERO_PADDING_CONFIG_REG_NUM_VECTOR_1);

    // update for num_timestep_1
    auto num_timestep_1_next =
        Concat(m.input(TOP_DATA_IN_9), m.input(TOP_DATA_IN_8));
    SetUpdateForConfigWr(m, instr, num_timestep_1_next,
                         GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_1);

    // update for num_timestep_2
    auto num_timestep_2_next =
        Concat(m.input(TOP_DATA_IN_11), m.input(TOP_DATA_IN_10));
    SetUpdateForConfigWr(m, instr, num_timestep_2_next,
                         GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_2);
  }
}

} // namespace flex

} // namespace ilang
