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
#include <flex/uninterpreted_func.h>

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
    // states for holding accumulation results
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

  for (auto i = 0; i < GB_ATTENTION_VECTOR_NUM; i++) {
    // states for holding attention values
    child.NewBvState(GBGetVectorName(i, GB_ATTENTION_VECTOR),
                      GB_ATTENTION_VECTOR_BITWIDTH);
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

  { // instr 3 ---- get the transpose of the input matrix
    auto instr = child.NewInstr("gb_attention_child_bmm_tp");
    auto state_bmmtp = (state == GB_ATTENTION_CHILD_STATE_BMM_TP);

    instr.SetDecode(child_valid & state_bmmtp);

    // do the transpose of the matrix
    // don't need the intermediate temp reg because it is non-blocking assignment
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      for (auto j = 0; j < GB_CORE_SCALAR; j++) {
        auto reg_i_j = child.state(GBGetVectorName2D(i, j, GB_ATTENTION_DP0));
        auto reg_j_i = child.state(GBGetVectorName2D(j, i, GB_ATTENTION_DP0));
        instr.SetUpdate(reg_i_j, reg_j_i);
      }
    }

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_BMM_MV,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);
                              
  }

  { // instr 4 ---- matrix-vector multiplication
    auto instr = child.NewInstr("gb_attention_child_bmm_mv");
    auto state_bmm_mv = (state == GB_ATTENTION_CHILD_STATE_BMM_MV);

    instr.SetDecode(child_valid & state_bmm_mv);

    // do the matrix-vector multiplication
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto accum_reg_i = child.state(GBGetVectorName(i, GB_ATTENTION_ACCUM_VECTOR));
      auto accum_tmp = BvConst(0, ACCUM_WORD_WIDTH);

      for (auto j = 0; j < GB_CORE_SCALAR; j++) {
        auto dp0_byte = child.state(GBGetVectorName2D(i, j, GB_ATTENTION_DP0));
        auto dp1_byte = child.state(GBGetVectorName(j, GB_ATTENTION_DP1));
        auto tmp = ProductSum(dp0_byte, dp1_byte);
        accum_tmp = AccumAdd(accum_tmp, tmp);
      }

      instr.SetUpdate(accum_reg_i, accum_tmp);
    }
    
    auto is_end_vector = (vector_cntr >= num_vector - 1);
    auto is_end_timestep = (timestep_cntr >= num_timestep - 1);

    auto vector_cntr_next = 
      Ite(bmm_cntr == 0,
          Ite(is_end_vector, BvConst(0, GB_ATTENTION_VECTOR_CNTR_BITWIDTH), vector_cntr + 1),
          vector_cntr);
    
    auto timestep_cntr_next = 
      Ite(bmm_cntr == 0,
          Ite(is_end_timestep, BvConst(0, GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH), timestep_cntr + 1),
          timestep_cntr);

    auto next_state = 
      Ite(bmm_cntr == 0,
        Ite(is_end_vector,
            BvConst(GB_ATTENTION_CHILD_STATE_NEXT, GB_ATTENTION_CHILD_STATE_BITWIDTH),
            BvConst(GB_ATTENTION_CHILD_STATE_BMM, GB_ATTENTION_CHILD_STATE_BITWIDTH)),
        Ite(is_end_timestep,
            BvConst(GB_ATTENTION_CHILD_STATE_OUT, GB_ATTENTION_CHILD_STATE_BITWIDTH),
            BvConst(GB_ATTENTION_CHILD_STATE_BMM, GB_ATTENTION_CHILD_STATE_BITWIDTH)));

    instr.SetUpdate(state, next_state);
    instr.SetUpdate(vector_cntr, vector_cntr_next);
    instr.SetUpdate(timestep_cntr, timestep_cntr_next);
  }

  { // instr 5 ---- FSM next instruction
    auto instr = child.NewInstr("gb_attention_child_next");
    auto state_next = (state == GB_ATTENTION_CHILD_STATE_NEXT);

    instr.SetDecode(child_valid & state_next);

    // right shrift amout
    auto adpbias_matrix = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_1);
    auto adpbias_input = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_2);

    auto shift_const = 2*ADPTFLOW_MAN_WIDTH + 2*ADPTFLOW_OFFSET_NEG - ATTENTION_NUM_FRAC;
    auto shift_bias = Concat(BvConst(0, 32-adpbias_matrix.bit_width()), adpbias_matrix) +
                      Concat(BvConst(0, 32-adpbias_input.bit_width()), adpbias_input);
    
    auto is_left_shift = (shift_bias > shift_const);
    auto shift_amout = Ite(is_left_shift,
                            shift_bias - shift_const,
                            BvConst(shift_const,32) - shift_bias);
    
    for (auto i = 0; i < GB_ATTENTION_VECTOR_NUM; i++) {
      auto atten_vector = child.state(GBGetVectorName(i, GB_ATTENTION_VECTOR));
      auto accum_vector = 
        Ite(softmax_cntr == 0,
            child.state(GBGetVectorName(i, GB_ATTENTION_ACCUM_VECTOR)),
        Ite(softmax_cntr == 1,
            child.state(GBGetVectorName(4+i, GB_ATTENTION_ACCUM_VECTOR)),
        Ite(softmax_cntr == 2,
            child.state(GBGetVectorName(8+i, GB_ATTENTION_ACCUM_VECTOR)),
            child.state(GBGetVectorName(12+i, GB_ATTENTION_ACCUM_VECTOR)))));
      
      // shift
      auto atten_vector_next = 
        Ite(accum_vector == 0, 
            BvConst(ATTENTION_HIGH_NEG, GB_ATTENTION_ACCUM_VECTOR_BITWIDTH),
            Ite(is_left_shift, accum_vector << shift_amout,
                                accum_vector >> shift_amout));

      instr.SetUpdate(atten_vector, atten_vector_next);
    }

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_NEXT_MAX,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);
  }

  { // instr 6 ---- FSM NEXT, update max value
    auto instr = child.NewInstr("gb_attention_child_next_max");
    auto state_next_max = (state == GB_ATTENTION_CHILD_STATE_NEXT_MAX);

    instr.SetDecode(child_valid & state_next_max);

    auto max_val_next = max_val;

    for (auto i = 0; i < GB_ATTENTION_VECTOR_NUM; i++) {
      auto accum_scalar = child.state(GBGetVectorName(i, GB_ATTENTION_VECTOR));
      max_val_next = GBAttentionMax(max_val_next, accum_scalar);
    }

    instr.SetUpdate(max_val, max_val_next);

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_NEXT_WR,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);
    
    instr.SetUpdate(state, next_state);
  }

  { // instr 7 ---- FSM NEXT write data into small buffer
    // write softmax values into the small buffer 
    auto instr = child.NewInstr("gb_attention_child_next_wr");
    auto state_next_wr = (state == GB_ATTENTION_CHILD_STATE_NEXT_WR);

    instr.SetDecode(child_valid & state_next_wr);

    auto mem_small_index = BvConst(GB_ATTENTION_SOFTMAX_INDEX,
                                    GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1_WIDTH);

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

    auto vector_index = (timestep_cntr >> 2) + 
          Concat(BvConst(0, timestep_cntr.bit_width() - softmax_cntr.bit_width()), softmax_cntr);
          
    auto vector_index_b = Concat(vector_index, BvConst(0, 4));

    auto addr_base_v = Concat(BvConst(0, 32-mem_small_base.bit_width()), mem_small_base) +
                        Concat(BvConst(0, 32-vector_index_b.bit_width()), vector_index_b);
    
    auto mem_small_next = mem_small;

    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto accum_scalar = child.state(GBGetVectorName(i/4, GB_ATTENTION_VECTOR));
      auto addr_b = addr_base_v + i;
      auto hi = 8 * (i % 4) + 7;
      auto lo = 8 * (i % 4);
      auto data = Extract(accum_scalar, hi, lo);
      mem_small_next = Store(mem_small_next, addr_b, data);
    }

    instr.SetUpdate(mem_small, mem_small_next);

    // constrol state updates
    auto is_end1 = (softmax_cntr >= GB_ATTENTION_VECTOR_NUM - 1);
    auto is_end2 = (timestep_cntr >= num_timestep - 16);

    auto softmax_cntr_next = Ite(is_end1, BvConst(0, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH),
                                          softmax_cntr + 1);
    auto timestep_cntr_next = Ite(is_end2, BvConst(0, GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH),
                                            timestep_cntr + 16);
    auto bmm_cntr_next = Ite(is_end1 & is_end2, BvConst(1, GB_ATTENTION_BMM_CNTR_BITWIDTH),
                                                bmm_cntr);
    auto next_state = 
          Ite(is_end1 & is_end2,
            BvConst(GB_ATTENTION_CHILD_STATE_SFM, GB_ATTENTION_CHILD_STATE_BITWIDTH),
            Ite(is_end1 & !is_end2,
                BvConst(GB_ATTENTION_CHILD_STATE_PREP, GB_ATTENTION_CHILD_STATE_BITWIDTH),
                BvConst(GB_ATTENTION_CHILD_STATE_NEXT, GB_ATTENTION_CHILD_STATE_BITWIDTH)));
    
    instr.SetUpdate(softmax_cntr, softmax_cntr_next);
    instr.SetUpdate(timestep_cntr, timestep_cntr_next);
    instr.SetUpdate(bmm_cntr, bmm_cntr_next);

    instr.SetUpdate(state, next_state);   
  }
}

}; // namespace ilang
