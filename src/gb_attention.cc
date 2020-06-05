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

// File: gb_attention.cc

#include <flex/flex.h>

namespace ilang {

void AddChild_GB_Attention(Ila& m);

void DefineStartGBAttention(Ila& m) {
  auto instr = m.NewInstr("GB_ATTENTION_START");
  // decode condition
  auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
  auto start_addr = TOP_GB_ADDR_MIN + START_ADDR_GB_ATTENTION;
  auto is_valid = m.state(GB_ATTENTION_CONFIG_REG_IS_VALID);

  instr.SetDecode(is_write & is_valid & (m.input(TOP_ADDR_IN) == start_addr));

  auto flag_child = m.state(GB_ATTENTION_CHILD_VALID_FLAG);
  auto child_state = m.state(GB_ATTENTION_CHILD_STATE);

  instr.SetUpdate(flag_child, 
                  BvConst(GB_ATTENTION_VALID, GB_ATTENTION_CHILD_VALID_FLAG_BITWIDTH));
  instr.SetUpdate(child_state,
                  BvConst(GB_ATTENTION_CHILD_STATE_IDLE, GB_ATTENTION_CHILD_STATE_BITWIDTH));

  AddChild_GB_Attention(m);
}

void AddChild_GB_Attention(Ila& m) {
  auto child = m.NewChild("GB_ATTENTION_CHILD");
  auto flag_child = m.state(GB_ATTENTION_CHILD_VALID_FLAG);
  auto child_valid = (flag_child == GB_ATTENTION_VALID);

  child.SetValid(child_valid);

  /********* states needed for gb_attention child **********/
  auto state = m.state(GB_ATTENTION_CHILD_STATE); 
  auto num_vector = m.state(GB_ATTENTION_CONFIG_REG_NUM_VECTOR_1);
  auto num_timestep = m.state(GB_ATTENTION_CONFIG_REG_NUM_TIMESTEP_1); 

  // declare child states
  auto sum_exp = child.NewBvState(GB_ATTENTION_SUM_EXP, GB_ATTENTION_SUM_EXP_BITWIDTH);
  auto max_val = child.NewBvState(GB_ATTENTION_MAX_VALUE, GB_ATTENTION_MAX_VALUE_BITWIDTH);

  auto vector_cntr = 
        child.NewBvState(GB_ATTENTION_VECTOR_CNTR, GB_ATTENTION_VECTOR_CNTR_BITWIDTH);
  auto timestep_cntr = 
        child.NewBvState(GB_ATTENTION_TIMESTEP_CNTR, GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH);
  auto bmm_cntr = 
        child.NewBvState(GB_ATTENTION_BMM_CNTR, GB_ATTENTION_BMM_CNTR_BITWIDTH);
  auto softmax_cntr = 
        child.NewBvState(GB_ATTENTION_SOFTMAX_CNTR, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH);

  // vector states
  for (auto i = 0; i < GB_CORE_SCALAR; i++) {
    child.NewBvState(GBGetVectorName(i, GB_ATTENTION_ACCUM_VECTOR),
                      GB_ATTENTION_ACCUM_VECTOR_BITWIDTH);
    // states for holding input vector data
    child.NewBvState(GBGetVectorName(i, GB_ATTENTION_DP1), GB_ATTENTION_DP1_BITWIDTH);
    // declare states for holding the input matrix data
    for (auto j = 0; j < GB_CORE_SCALAR; j++) {
      child.NewBvState(GBGetVectorName2D(i, j, GB_ATTENTION_DP0),
                        GB_ATTENTION_DP0_BITWIDTH);
    }
  }

  /********** gb_attention child instructions ***************/
  { // instruction 0 ---- handle the idle state
    auto instr = child.NewInstr("gb_attention_child_idle");
    auto state_idle = (state == GB_ATTENTION_CHILD_STATE_IDLE);

    instr.SetDecode(child_valid & state_idle);

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_PREP,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);                          
    // states initilization
    instr.SetUpdate(child.state(GB_ATTENTION_SUM_EXP),
                      BvConst(0, GB_ATTENTION_SUM_EXP_BITWIDTH));
    instr.SetUpdate(child.state(GB_ATTENTION_MAX_VALUE),
                      BvConst(ATTENTION_WORD_MIN, GB_ATTENTION_MAX_VALUE_BITWIDTH));
    
    instr.SetUpdate(vector_cntr, BvConst(0, GB_ATTENTION_VECTOR_CNTR_BITWIDTH));
    instr.SetUpdate(timestep_cntr, BvConst(0, GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH));
    instr.SetUpdate(bmm_cntr, BvConst(0, GB_ATTENTION_BMM_CNTR_BITWIDTH));
    instr.SetUpdate(softmax_cntr, BvConst(0, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH));
  }

  { // instruction 1 ---- set preparation for gb_attention
    auto instr = child.NewInstr("gb_attention_child_prep");
    auto state_prep = (state == GB_ATTENTION_CHILD_STATE_PREP);

    instr.SetDecode(child_valid & state_prep);

    // initiate the accum vector
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      instr.SetUpdate(child.state(GBGetVectorName(i, GB_ATTENTION_ACCUM_VECTOR)),
                      BvConst(0, GB_ATTENTION_ACCUM_VECTOR_BITWIDTH));
    }

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_BMM, GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 2 ---- state BMM
    auto instr = child.NewInstr("gb_attention_child_bmm");
    auto state_bmm = (state == GB_ATTENTION_CHILD_STATE_BMM);

    instr.SetDecode(child_valid & state_bmm);

    auto mem_large_index_enc = m.state(GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1);
    // TODO: See whether current ILAtor support concatenation of specified length constant
    auto mem_large_base_enc = 
      Ite(mem_large_index_enc == 0, 
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0), BvConst(0, 4)),
      Ite(mem_large_index_enc == 1, 
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1), BvConst(0, 4)),
      Ite(mem_large_index_enc == 2,
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2), BvConst(0, 4)),
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3), BvConst(0,4)))));
    
    // calculating the address for the large memory
    auto num_vector_32 = Concat(BvConst(0, 32-num_vector.bit_width()), num_vector);
    auto timestep_size = num_vector_32 * GB_CORE_SCALAR;
    auto group_size = timestep_size * GB_CORE_LARGE_NUM_BANKS;

    auto group_index = timestep_cntr/BvConst(GB_CORE_SCALAR, timestep_cntr.bit_width());
    auto group_offset = URem(timestep_cntr, BvConst(GB_CORE_SCALAR, timestep_cntr.bit_width()));
    auto group_index_32 = 
          Concat(BvConst(0, 32 - group_index.bit_width()), group_index);
    auto group_offset_32 = 
          Concat(BvConst(0, 32 - group_offset.bit_width()), group_offset);
    
    auto ts_base_addr_offset = group_index_32*group_size + group_offset_32*GB_CORE_SCALAR;
    auto ts_base_addr = Concat(BvConst(0, 32-mem_large_base_enc.bit_width()), mem_large_base_enc)
                        + ts_base_addr_offset;

    // fetch the data encoder data from large memory buffer
    auto mem_large = m.state(GB_CORE_LARGE_BUFFER);
    auto row_size = GB_CORE_SCALAR * GB_CORE_LARGE_NUM_BANKS;

    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto addr_large_v = ts_base_addr + row_size * i;
      for (auto j = 0; j < GB_CORE_SCALAR; j++) {
        auto addr_large_b = addr_large_v + j;
        auto reg = child.state(GBGetVectorName2D(i,j,GB_ATTENTION_DP0));
        instr.SetUpdate(reg, Load(mem_large, addr_large_b));
      }
    }

    // calculating the address for the small buffer
    auto index_dec = m.state(GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_2);
    auto index_softmax = BvConst(GB_ATTENTION_SOFTMAX_INDEX,
                                  GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1_WIDTH);
    auto vector_cntr_16 = Concat(BvConst(0, 16-vector_cntr.bit_width()), vector_cntr);

    auto mem_small_index = Ite(bmm_cntr == 0, index_dec, index_softmax);
    auto vector_index = Ite(bmm_cntr == 0, vector_cntr_16, timestep_cntr >> 4);
    
    // fetch data from the small buffer
    auto is_zero = BoolConst(true); // determine whether dp1 is zero vector

    auto mem_small = m.state(GB_CORE_SMALL_BUFFER);
    auto mem_small_base = 
      Ite(mem_small_index == 0,
        Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_0), BvConst(0,4)),
      Ite(mem_small_index == 1,
        Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_1), BvConst(0,4)),
      Ite(mem_small_index == 2,
        Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_2), BvConst(0,4)),
      Ite(mem_small_index == 3,
        Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_3), BvConst(0,4)),
      Ite(mem_small_index == 4,
        Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_4), BvConst(0,4)),
      Ite(mem_small_index == 5,
        Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_5), BvConst(0,4)),
      Ite(mem_small_index == 6,
        Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_6), BvConst(0,4)),
        Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_7), BvConst(0,4))
        )))))));

    auto addr_small_v = Concat(BvConst(0, 32-mem_small_base.bit_width()), mem_small_base) + 
          Concat(BvConst(0, 32-vector_index.bit_width()), vector_index) * GB_CORE_SCALAR;
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto addr_small_b = addr_small_v + i;
      auto reg = child.state(GBGetVectorName(i, GB_ATTENTION_DP1));
      auto data = Load(mem_small, addr_small_b);
      auto data_zero = (data == 0);

      is_zero = is_zero & data_zero;
      instr.SetUpdate(reg, data);
    }

    // next state
    auto next_state = 
      Ite(is_zero, BvConst(GB_ATTENTION_CHILD_STATE_NEXT, GB_ATTENTION_CHILD_STATE_BITWIDTH),
        Ite(bmm_cntr == 1, 
            BvConst(GB_ATTENTION_CHILD_STATE_BMM_TP, GB_ATTENTION_CHILD_STATE_BITWIDTH),
            BvConst(GB_ATTENTION_CHILD_STATE_BMM_MV, GB_ATTENTION_CHILD_STATE_BITWIDTH)));
    
    instr.SetUpdate(state, next_state);
  }
}

}; // namespace ilang
