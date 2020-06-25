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
#include <flex/util.h>

namespace ilang {

namespace flex {

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

  instr.SetUpdate(flag_child, BvConst(GB_ATTENTION_VALID,
                                      GB_ATTENTION_CHILD_VALID_FLAG_BITWIDTH));
  instr.SetUpdate(child_state, BvConst(GB_ATTENTION_CHILD_STATE_IDLE,
                                       GB_ATTENTION_CHILD_STATE_BITWIDTH));

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
  auto sum_exp =
      child.NewBvState(GB_ATTENTION_SUM_EXP, GB_ATTENTION_SUM_EXP_BITWIDTH);
  auto max_val =
      child.NewBvState(GB_ATTENTION_MAX_VALUE, GB_ATTENTION_MAX_VALUE_BITWIDTH);

  auto vector_cntr = child.NewBvState(GB_ATTENTION_VECTOR_CNTR,
                                      GB_ATTENTION_VECTOR_CNTR_BITWIDTH);
  auto timestep_cntr = child.NewBvState(GB_ATTENTION_TIMESTEP_CNTR,
                                        GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH);
  auto bmm_cntr =
      child.NewBvState(GB_ATTENTION_BMM_CNTR, GB_ATTENTION_BMM_CNTR_BITWIDTH);
  auto softmax_cntr = child.NewBvState(GB_ATTENTION_SOFTMAX_CNTR,
                                       GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH);

  // vector states
  for (auto i = 0; i < GB_CORE_SCALAR; i++) {
    // states for holding accumulation results
    child.NewBvState(GBGetVectorName(i, GB_ATTENTION_ACCUM_VECTOR),
                     GB_ATTENTION_ACCUM_VECTOR_BITWIDTH);
    // states for holding input vector data
    child.NewBvState(GBGetVectorName(i, GB_ATTENTION_DP1),
                     GB_ATTENTION_DP1_BITWIDTH);
    // states for holding softmax results
    child.NewBvState(GBGetVectorName(i, GB_ATTENTION_SFM_RESULT_VECTOR),
                     GB_ATTENTION_SFM_RESULT_VECTOR_BITWIDTH);
    // states for holding output in the FSM out
    child.NewBvState(GBGetVectorName(i, GB_ATTENTION_OUT_VECTOR),
                     GB_ATTENTION_OUT_VECTOR_BITWIDTH);
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
    // states for holding exp vectors
    child.NewBvState(GBGetVectorName(i, GB_ATTENTION_EXP_VECTOR),
                     GB_ATTENTION_EXP_VECTOR_BITWIDTH);
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
    instr.SetUpdate(sum_exp, BvConst(0, GB_ATTENTION_SUM_EXP_BITWIDTH));
    instr.SetUpdate(
        max_val, BvConst(ATTENTION_WORD_MIN, GB_ATTENTION_MAX_VALUE_BITWIDTH));

    instr.SetUpdate(vector_cntr, BvConst(0, GB_ATTENTION_VECTOR_CNTR_BITWIDTH));
    instr.SetUpdate(timestep_cntr,
                    BvConst(0, GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH));
    instr.SetUpdate(bmm_cntr, BvConst(0, GB_ATTENTION_BMM_CNTR_BITWIDTH));
    instr.SetUpdate(softmax_cntr,
                    BvConst(0, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH));
  }

  { // instruction 1 ---- set preparation for gb_attention
    auto instr = child.NewInstr("gb_attention_child_prep");
    auto state_prep = (state == GB_ATTENTION_CHILD_STATE_PREP);

    instr.SetDecode(child_valid & state_prep);

    // initiate the accum vector
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      instr.SetUpdate(
          child.state(GBGetVectorName(i, GB_ATTENTION_ACCUM_VECTOR)),
          BvConst(0, GB_ATTENTION_ACCUM_VECTOR_BITWIDTH));
    }

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_BMM_RD,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 2 ---- state BMM
    auto instr = child.NewInstr("gb_attention_child_bmm_rd");
    auto state_bmm = (state == GB_ATTENTION_CHILD_STATE_BMM_RD);

    instr.SetDecode(child_valid & state_bmm);

    auto mem_large_index_enc = m.state(GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1);

    auto mem_large_base_enc = Ite(
        mem_large_index_enc == 0,
        Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0),
               BvConst(0, 4)),
        Ite(mem_large_index_enc == 1,
            Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1),
                   BvConst(0, 4)),
            Ite(mem_large_index_enc == 2,
                Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2),
                       BvConst(0, 4)),
                Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3),
                       BvConst(0, 4)))));

    // // calculating the address for the large memory
    auto num_vector_32 =
        Concat(BvConst(0, 32 - num_vector.bit_width()), num_vector);
    auto timestep_size = num_vector_32 * GB_CORE_SCALAR;
    auto group_size = timestep_size * GB_CORE_LARGE_NUM_BANKS;

    auto group_index =
        timestep_cntr / BvConst(GB_CORE_SCALAR, timestep_cntr.bit_width());
    // auto group_offset = URem(timestep_cntr, BvConst(GB_CORE_SCALAR,
    // timestep_cntr.bit_width()));
    auto group_index_32 =
        Concat(BvConst(0, 32 - group_index.bit_width()), group_index);
    // auto group_offset_32 =
    //       Concat(BvConst(0, 32 - group_offset.bit_width()), group_offset);

    auto ts_base_addr_offset = group_index_32 * group_size;
    auto ts_base_addr = Concat(BvConst(0, 32 - mem_large_base_enc.bit_width()),
                               mem_large_base_enc) +
                        ts_base_addr_offset;

    // fetch the data encoder data from large memory buffer
    auto mem_large = m.state(GB_CORE_LARGE_BUFFER);
    auto vector_cntr_32 =
        Concat(BvConst(0, 32 - vector_cntr.bit_width()), vector_cntr);
    auto row_size = GB_CORE_SCALAR * GB_CORE_LARGE_NUM_BANKS;
    auto vector_addr_offset = vector_cntr_32 * row_size;

    auto mem_large_addr_base = ts_base_addr + vector_addr_offset;

    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto addr_large_v = mem_large_addr_base + GB_CORE_SCALAR * i;
      for (auto j = 0; j < GB_CORE_SCALAR; j++) {
        auto addr_large_b = addr_large_v + j;
        auto reg = child.state(GBGetVectorName2D(i, j, GB_ATTENTION_DP0));
        instr.SetUpdate(reg, Load(mem_large, addr_large_b));
      }
    }

    // calculating the address for the small buffer
    auto index_dec = m.state(GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_2);
    auto index_softmax = BvConst(GB_ATTENTION_SOFTMAX_INDEX,
                                 GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1_WIDTH);
    auto vector_cntr_16 =
        Concat(BvConst(0, 16 - vector_cntr.bit_width()), vector_cntr);

    auto mem_small_index = Ite(bmm_cntr == 0, index_dec, index_softmax);
    auto vector_index = Ite(bmm_cntr == 0, vector_cntr_16, timestep_cntr >> 4);

    // fetch data from the small buffer
    auto is_zero = BoolConst(true); // determine whether dp1 is zero vector

    auto mem_small = m.state(GB_CORE_SMALL_BUFFER);
    auto mem_small_base = GBGetSmallBufBase(m, mem_small_index);

    auto addr_small_v =
        Concat(BvConst(0, 32 - mem_small_base.bit_width()), mem_small_base) +
        Concat(BvConst(0, 32 - vector_index.bit_width()), vector_index) *
            GB_CORE_SCALAR;

    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto addr_small_b = addr_small_v + i;
      auto reg = child.state(GBGetVectorName(i, GB_ATTENTION_DP1));
      auto data = Load(mem_small, addr_small_b);
      auto data_zero = (data == 0);

      is_zero = is_zero & data_zero;
      instr.SetUpdate(reg, data);
    }

    // next state
    auto next_state = Ite(is_zero,
                          BvConst(GB_ATTENTION_CHILD_STATE_BMM_NEXT,
                                  GB_ATTENTION_CHILD_STATE_BITWIDTH),
                          Ite(bmm_cntr == 1,
                              BvConst(GB_ATTENTION_CHILD_STATE_BMM_TP,
                                      GB_ATTENTION_CHILD_STATE_BITWIDTH),
                              BvConst(GB_ATTENTION_CHILD_STATE_BMM_MV,
                                      GB_ATTENTION_CHILD_STATE_BITWIDTH)));

    instr.SetUpdate(state, next_state);
  }

  { // instr 3 ---- get the transpose of the input matrix
    auto instr = child.NewInstr("gb_attention_child_bmm_tp");
    auto state_bmmtp = (state == GB_ATTENTION_CHILD_STATE_BMM_TP);

    instr.SetDecode(child_valid & state_bmmtp);

    // do the transpose of the matrix
    // don't need the intermediate temp reg because it is non-blocking
    // assignment
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      for (auto j = 0; j < GB_CORE_SCALAR; j++) {
        auto reg_i_j = child.state(GBGetVectorName2D(i, j, GB_ATTENTION_DP0));
        auto reg_j_i = child.state(GBGetVectorName2D(j, i, GB_ATTENTION_DP0));
        instr.SetUpdate(reg_i_j, reg_j_i);
      }
    }

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_BMM_MV,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instr 4 ---- matrix-vector multiplication
    auto instr = child.NewInstr("gb_attention_child_bmm_mv");
    auto state_bmm_mv = (state == GB_ATTENTION_CHILD_STATE_BMM_MV);

    instr.SetDecode(child_valid & state_bmm_mv);

    // do the matrix-vector multiplication
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto accum_reg_i =
          child.state(GBGetVectorName(i, GB_ATTENTION_ACCUM_VECTOR));
      auto accum_tmp = BvConst(0, ACCUM_WORD_WIDTH);

      for (auto j = 0; j < GB_CORE_SCALAR; j++) {
        auto dp0_byte = child.state(GBGetVectorName2D(i, j, GB_ATTENTION_DP0));
        auto dp1_byte = child.state(GBGetVectorName(j, GB_ATTENTION_DP1));
        auto tmp = ProductSum(dp0_byte, dp1_byte);
        accum_tmp = AccumAdd(accum_tmp, tmp);
      }

      auto accum_reg_i_next = AccumAdd(accum_reg_i, accum_tmp);

      instr.SetUpdate(accum_reg_i, accum_reg_i_next);
    }

    // move the control states updates to a independent instruction to handle
    // the is_zero case
    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_BMM_NEXT,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instr 19 ---- FSM BMM control state updates
    auto instr = child.NewInstr("gb_attention_child_bmm_next");
    auto state_bmm_next = (state == GB_ATTENTION_CHILD_STATE_BMM_NEXT);

    instr.SetDecode(child_valid & state_bmm_next);

    auto is_end_vector = (vector_cntr >= num_vector - 1);
    auto is_end_timestep = (timestep_cntr >= num_timestep - 16);

    auto vector_cntr_next =
        Ite(bmm_cntr == 0,
            Ite(is_end_vector, BvConst(0, GB_ATTENTION_VECTOR_CNTR_BITWIDTH),
                vector_cntr + 1),
            vector_cntr);

    auto timestep_cntr_next = Ite(
        bmm_cntr == 1,
        Ite(is_end_timestep, BvConst(0, GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH),
            timestep_cntr + 16),
        timestep_cntr);

    auto next_state = Ite(bmm_cntr == 0,
                          Ite(is_end_vector,
                              BvConst(GB_ATTENTION_CHILD_STATE_NEXT_SH,
                                      GB_ATTENTION_CHILD_STATE_BITWIDTH),
                              BvConst(GB_ATTENTION_CHILD_STATE_BMM_RD,
                                      GB_ATTENTION_CHILD_STATE_BITWIDTH)),
                          Ite(is_end_timestep,
                              BvConst(GB_ATTENTION_CHILD_STATE_OUT1,
                                      GB_ATTENTION_CHILD_STATE_BITWIDTH),
                              BvConst(GB_ATTENTION_CHILD_STATE_BMM_RD,
                                      GB_ATTENTION_CHILD_STATE_BITWIDTH)));

    instr.SetUpdate(state, next_state);
    instr.SetUpdate(vector_cntr, vector_cntr_next);
    instr.SetUpdate(timestep_cntr, timestep_cntr_next);
  }

  { // instr 5 ---- FSM next instruction
    auto instr = child.NewInstr("gb_attention_child_next_shift");
    auto state_next = (state == GB_ATTENTION_CHILD_STATE_NEXT_SH);

    instr.SetDecode(child_valid & state_next);

    // right shrift amout
    auto adpbias_matrix = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_1);
    auto adpbias_input = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_2);

    auto shift_const =
        2 * ADPTFLOW_MAN_WIDTH + 2 * ADPTFLOW_OFFSET_NEG - ATTENTION_NUM_FRAC;
    auto shift_bias =
        Concat(BvConst(0, 32 - adpbias_matrix.bit_width()), adpbias_matrix) +
        Concat(BvConst(0, 32 - adpbias_input.bit_width()), adpbias_input);

    auto is_left_shift = (shift_bias > shift_const);
    auto shift_amout = Ite(is_left_shift, shift_bias - shift_const,
                           BvConst(shift_const, 32) - shift_bias);

    for (auto i = 0; i < GB_ATTENTION_VECTOR_NUM; i++) {
      auto atten_vector = child.state(GBGetVectorName(i, GB_ATTENTION_VECTOR));
      auto accum_vector = Ite(
          softmax_cntr == 0,
          child.state(GBGetVectorName(i, GB_ATTENTION_ACCUM_VECTOR)),
          Ite(softmax_cntr == 1,
              child.state(GBGetVectorName(4 + i, GB_ATTENTION_ACCUM_VECTOR)),
              Ite(softmax_cntr == 2,
                  child.state(
                      GBGetVectorName(8 + i, GB_ATTENTION_ACCUM_VECTOR)),
                  child.state(
                      GBGetVectorName(12 + i, GB_ATTENTION_ACCUM_VECTOR)))));

      // shift
      auto atten_vector_next =
          Ite(accum_vector == 0,
              BvConst(ATTENTION_HIGH_NEG, GB_ATTENTION_ACCUM_VECTOR_BITWIDTH),
              Ite(is_left_shift, GBAttentionLSH(accum_vector, shift_amout),
                  GBAttentionRSH(accum_vector, shift_amout)));

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

    auto mem_small_index =
        BvConst(GB_ATTENTION_SOFTMAX_INDEX,
                GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1_WIDTH);

    auto mem_small = m.state(GB_CORE_SMALL_BUFFER);
    auto mem_small_base = GBGetSmallBufBase(m, mem_small_index);

    auto vector_index =
        (timestep_cntr >> 2) +
        Concat(BvConst(0, timestep_cntr.bit_width() - softmax_cntr.bit_width()),
               softmax_cntr);

    auto vector_index_b = Concat(vector_index, BvConst(0, 4));

    auto addr_base_v =
        Concat(BvConst(0, 32 - mem_small_base.bit_width()), mem_small_base) +
        Concat(BvConst(0, 32 - vector_index_b.bit_width()), vector_index_b);

    auto mem_small_next = mem_small;

    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto accum_scalar =
          child.state(GBGetVectorName(i / 4, GB_ATTENTION_VECTOR));
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

    auto softmax_cntr_next =
        Ite(is_end1, BvConst(0, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH),
            softmax_cntr + 1);

    auto timestep_cntr_next =
        Ite(is_end1,
            Ite(is_end2, BvConst(0, GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH),
                timestep_cntr + 16),
            timestep_cntr);

    auto bmm_cntr_next =
        Ite(is_end1 & is_end2, BvConst(1, GB_ATTENTION_BMM_CNTR_BITWIDTH),
            bmm_cntr);
    auto next_state = Ite(is_end1 & is_end2,
                          BvConst(GB_ATTENTION_CHILD_STATE_SFM1_RD,
                                  GB_ATTENTION_CHILD_STATE_BITWIDTH),
                          Ite(is_end1 & !is_end2,
                              BvConst(GB_ATTENTION_CHILD_STATE_PREP,
                                      GB_ATTENTION_CHILD_STATE_BITWIDTH),
                              BvConst(GB_ATTENTION_CHILD_STATE_NEXT_SH,
                                      GB_ATTENTION_CHILD_STATE_BITWIDTH)));

    instr.SetUpdate(softmax_cntr, softmax_cntr_next);
    instr.SetUpdate(timestep_cntr, timestep_cntr_next);
    instr.SetUpdate(bmm_cntr, bmm_cntr_next);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 8 ---- FSM SFM read
    auto instr = child.NewInstr("gb_attention_child_sfm1_rd");
    auto state_sfm1_rd = (state == GB_ATTENTION_CHILD_STATE_SFM1_RD);

    instr.SetDecode(child_valid & state_sfm1_rd);

    auto mem_small_index =
        BvConst(GB_ATTENTION_SOFTMAX_INDEX,
                GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1_WIDTH);
    auto mem_small_base = GBGetSmallBufBase(m, mem_small_index);

    auto vector_index =
        (timestep_cntr >> 2) +
        Concat(BvConst(0, timestep_cntr.bit_width() - softmax_cntr.bit_width()),
               softmax_cntr);
    auto vector_index_b = Concat(vector_index, BvConst(0, 4));

    auto addr_base_v =
        Concat(BvConst(0, 32 - mem_small_base.bit_width()), mem_small_base) +
        Concat(BvConst(0, 32 - vector_index_b.bit_width()), vector_index_b);

    auto mem_small = m.state(GB_CORE_SMALL_BUFFER);

    for (auto i = 0; i < 4; i++) {
      auto exp_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_EXP_VECTOR));
      auto mem_addr_base = addr_base_v + i * 4;
      auto data_0 = Load(mem_small, mem_addr_base + 0);
      auto data_1 = Load(mem_small, mem_addr_base + 1);
      auto data_2 = Load(mem_small, mem_addr_base + 2);
      auto data_3 = Load(mem_small, mem_addr_base + 3);
      auto exp_vector_next =
          Concat(data_3, Concat(data_2, Concat(data_1, data_0)));

      instr.SetUpdate(exp_vector, exp_vector_next);
    }

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_SFM1_COMP,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 9 ---- FSM SFM computation
    auto instr = child.NewInstr("gb_attention_child_sfm1_comp");
    auto state_sfm1_sub = (state == GB_ATTENTION_CHILD_STATE_SFM1_COMP);

    instr.SetDecode(child_valid & state_sfm1_sub);

    auto tmp_sum = BvConst(0, ATTENTION_WORD_WIDTH);

    for (auto i = 0; i < 4; i++) {
      auto exp_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_EXP_VECTOR));
      auto exp_vector_next = GBAttentionSub(exp_vector, max_val);
      exp_vector_next = GBAttentionExp(exp_vector_next);

      tmp_sum = tmp_sum + exp_vector_next;
    }

    auto sum_exp_next = sum_exp + tmp_sum;

    instr.SetUpdate(sum_exp, sum_exp_next);

    // constrol state updates
    auto is_end1 = (softmax_cntr >= GB_ATTENTION_VECTOR_NUM - 1);
    auto is_end2 = (timestep_cntr >= num_timestep - 16);

    auto softmax_cntr_next =
        Ite(is_end1, BvConst(0, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH),
            softmax_cntr + 1);

    auto timestep_cntr_next =
        Ite(is_end1,
            Ite(is_end2, BvConst(0, GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH),
                timestep_cntr + 16),
            timestep_cntr);
    auto next_state = Ite(is_end1 & is_end2,
                          BvConst(GB_ATTENTION_CHILD_STATE_SFM2_RD,
                                  GB_ATTENTION_CHILD_STATE_BITWIDTH),
                          BvConst(GB_ATTENTION_CHILD_STATE_SFM1_RD,
                                  GB_ATTENTION_CHILD_STATE_BITWIDTH));

    instr.SetUpdate(softmax_cntr, softmax_cntr_next);
    instr.SetUpdate(timestep_cntr, timestep_cntr_next);
    instr.SetUpdate(state, next_state);
  }

  { // instruction 10 ---- FSM SFM2 read
    auto instr = child.NewInstr("gb_attention_sfm2_rd");
    auto state_sfm2_rd = (state == GB_ATTENTION_CHILD_STATE_SFM2_RD);

    instr.SetDecode(child_valid & state_sfm2_rd);

    auto mem_small_index =
        BvConst(GB_ATTENTION_SOFTMAX_INDEX,
                GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1_WIDTH);
    auto mem_small_base = GBGetSmallBufBase(m, mem_small_index);

    auto vector_index =
        (timestep_cntr >> 2) +
        Concat(BvConst(0, timestep_cntr.bit_width() - softmax_cntr.bit_width()),
               softmax_cntr);
    auto vector_index_b = Concat(vector_index, BvConst(0, 4));

    auto addr_base_v =
        Concat(BvConst(0, 32 - mem_small_base.bit_width()), mem_small_base) +
        Concat(BvConst(0, 32 - vector_index_b.bit_width()), vector_index_b);

    auto mem_small = m.state(GB_CORE_SMALL_BUFFER);

    for (auto i = 0; i < 4; i++) {
      auto exp_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_EXP_VECTOR));
      auto mem_addr_base = addr_base_v + i * 4;
      auto data_0 = Load(mem_small, mem_addr_base + 0);
      auto data_1 = Load(mem_small, mem_addr_base + 1);
      auto data_2 = Load(mem_small, mem_addr_base + 2);
      auto data_3 = Load(mem_small, mem_addr_base + 3);
      auto exp_vector_next =
          Concat(data_3, Concat(data_2, Concat(data_1, data_0)));

      instr.SetUpdate(exp_vector, exp_vector_next);
    }

    auto next_state = Ite(softmax_cntr == 0,
                          BvConst(GB_ATTENTION_CHILD_STATE_SFM2_COMP0,
                                  GB_ATTENTION_CHILD_STATE_BITWIDTH),
                          Ite(softmax_cntr == 1,
                              BvConst(GB_ATTENTION_CHILD_STATE_SFM2_COMP1,
                                      GB_ATTENTION_CHILD_STATE_BITWIDTH),
                              Ite(softmax_cntr == 2,
                                  BvConst(GB_ATTENTION_CHILD_STATE_SFM2_COMP2,
                                          GB_ATTENTION_CHILD_STATE_BITWIDTH),
                                  BvConst(GB_ATTENTION_CHILD_STATE_SFM2_COMP3,
                                          GB_ATTENTION_CHILD_STATE_BITWIDTH))));

    instr.SetUpdate(state, next_state);
  }

  { // instruction 11 ---- FSM SFM2 computation0
    auto instr = child.NewInstr("gb_attention_sfm2_comp0");
    auto state_sfm2_comp = (state == GB_ATTENTION_CHILD_STATE_SFM2_COMP0);

    instr.SetDecode(child_valid & state_sfm2_comp);

    auto adpbias_softmax = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_3);

    for (auto i = 0; i < 4; i++) {
      auto exp_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_EXP_VECTOR));
      auto exp_vector_sub = GBAttentionSub(exp_vector, max_val); // X - Max[X]
      auto exp_vector_exp = GBAttentionExp(exp_vector_sub);      // (X-Max[X])^2
      auto exp_vector_div =
          GBAttentionDiv(exp_vector_exp, sum_exp); // (X-Max[X])^2 / sum_exp
      auto tmp = GBAttentionCompress(exp_vector_div, adpbias_softmax);

      auto sfm_result_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_SFM_RESULT_VECTOR));
      instr.SetUpdate(sfm_result_vector, tmp);
    }

    auto softmax_cntr_next =
        Ite(softmax_cntr >= 3, BvConst(0, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH),
            softmax_cntr + 1);
    instr.SetUpdate(softmax_cntr, softmax_cntr_next);

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_SFM2_RD,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 12 ---- FSM SFM2 computation1
    auto instr = child.NewInstr("gb_attention_sfm2_comp1");
    auto state_sfm2_comp = (state == GB_ATTENTION_CHILD_STATE_SFM2_COMP1);

    instr.SetDecode(child_valid & state_sfm2_comp);

    auto adpbias_softmax = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_3);

    for (auto i = 0; i < 4; i++) {
      auto exp_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_EXP_VECTOR));
      auto exp_vector_sub = GBAttentionSub(exp_vector, max_val); // X - Max[X]
      auto exp_vector_exp = GBAttentionExp(exp_vector_sub);      // (X-Max[X])^2
      auto exp_vector_div =
          GBAttentionDiv(exp_vector_exp, sum_exp); // (X-Max[X])^2 / sum_exp
      auto tmp = GBAttentionCompress(exp_vector_div, adpbias_softmax);

      auto sfm_result_vector =
          child.state(GBGetVectorName(i + 4, GB_ATTENTION_SFM_RESULT_VECTOR));
      instr.SetUpdate(sfm_result_vector, tmp);
    }

    auto softmax_cntr_next =
        Ite(softmax_cntr >= 3, BvConst(0, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH),
            softmax_cntr + 1);
    instr.SetUpdate(softmax_cntr, softmax_cntr_next);

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_SFM2_RD,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 13 ---- FSM SFM2 computation2
    auto instr = child.NewInstr("gb_attention_sfm2_comp2");
    auto state_sfm2_comp = (state == GB_ATTENTION_CHILD_STATE_SFM2_COMP2);

    instr.SetDecode(child_valid & state_sfm2_comp);

    auto adpbias_softmax = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_3);

    for (auto i = 0; i < 4; i++) {
      auto exp_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_EXP_VECTOR));
      auto exp_vector_sub = GBAttentionSub(exp_vector, max_val); // X - Max[X]
      auto exp_vector_exp = GBAttentionExp(exp_vector_sub);      // (X-Max[X])^2
      auto exp_vector_div =
          GBAttentionDiv(exp_vector_exp, sum_exp); // (X-Max[X])^2 / sum_exp
      auto tmp = GBAttentionCompress(exp_vector_div, adpbias_softmax);

      auto sfm_result_vector =
          child.state(GBGetVectorName(i + 8, GB_ATTENTION_SFM_RESULT_VECTOR));
      instr.SetUpdate(sfm_result_vector, tmp);
    }

    auto softmax_cntr_next =
        Ite(softmax_cntr >= 3, BvConst(0, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH),
            softmax_cntr + 1);
    instr.SetUpdate(softmax_cntr, softmax_cntr_next);

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_SFM2_RD,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 14 ---- FSM SFM2 computation3
    auto instr = child.NewInstr("gb_attention_sfm2_comp3");
    auto state_sfm2_comp = (state == GB_ATTENTION_CHILD_STATE_SFM2_COMP3);

    instr.SetDecode(child_valid & state_sfm2_comp);

    auto adpbias_softmax = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_3);

    for (auto i = 0; i < 4; i++) {
      auto exp_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_EXP_VECTOR));
      auto exp_vector_sub = GBAttentionSub(exp_vector, max_val); // X - Max[X]
      auto exp_vector_exp = GBAttentionExp(exp_vector_sub);      // (X-Max[X])^2
      auto exp_vector_div =
          GBAttentionDiv(exp_vector_exp, sum_exp); // (X-Max[X])^2 / sum_exp
      auto tmp = GBAttentionCompress(exp_vector_div, adpbias_softmax);

      auto sfm_result_vector =
          child.state(GBGetVectorName(i + 12, GB_ATTENTION_SFM_RESULT_VECTOR));
      instr.SetUpdate(sfm_result_vector, tmp);
    }

    auto softmax_cntr_next =
        Ite(softmax_cntr >= 3, BvConst(0, GB_ATTENTION_SOFTMAX_CNTR_BITWIDTH),
            softmax_cntr + 1);
    instr.SetUpdate(softmax_cntr, softmax_cntr_next);

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_SFM3,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 15 ---- FSM state SFM3, write result back into small buffer
    auto instr = child.NewInstr("gb_attention_sfm3");
    auto state_sfm3 = (state == GB_ATTENTION_CHILD_STATE_SFM3);

    instr.SetDecode(child_valid & state_sfm3);

    auto mem_small_index =
        BvConst(GB_ATTENTION_SOFTMAX_INDEX,
                GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_1_WIDTH);
    auto mem_small_base = GBGetSmallBufBase(m, mem_small_index);

    auto vector_index = timestep_cntr >> 4;
    auto vector_index_v = Concat(vector_index, BvConst(0, 4));

    auto addr_base_b =
        Concat(BvConst(0, 32 - mem_small_base.bit_width()), mem_small_base) +
        Concat(BvConst(0, 32 - vector_index_v.bit_width()), vector_index_v);

    auto mem_small = m.state(GB_CORE_SMALL_BUFFER);
    auto mem_small_next = mem_small;

    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto sfm_result =
          child.state(GBGetVectorName(i, GB_ATTENTION_SFM_RESULT_VECTOR));
      auto addr = addr_base_b + i;
      mem_small_next = Store(mem_small_next, addr, sfm_result);
    }

    instr.SetUpdate(mem_small, mem_small_next);

    // control state update
    auto is_end = (timestep_cntr >= num_timestep - 16);
    auto bmm_cntr_next =
        Ite(is_end, BvConst(1, GB_ATTENTION_BMM_CNTR_BITWIDTH), bmm_cntr);
    auto timestep_cntr_next =
        Ite(is_end, BvConst(0, GB_ATTENTION_TIMESTEP_CNTR_BITWIDTH),
            timestep_cntr + 16);

    auto next_state = Ite(is_end,
                          BvConst(GB_ATTENTION_CHILD_STATE_PREP,
                                  GB_ATTENTION_CHILD_STATE_BITWIDTH),
                          BvConst(GB_ATTENTION_CHILD_STATE_SFM2_RD,
                                  GB_ATTENTION_CHILD_STATE_BITWIDTH));

    instr.SetUpdate(state, next_state);
    instr.SetUpdate(bmm_cntr, bmm_cntr_next);
    instr.SetUpdate(timestep_cntr, timestep_cntr_next);
  }

  { // instruction 16 ---- FSM OUT1
    auto instr = child.NewInstr("gb_attention_child_out1");
    auto state_out1 = (state == GB_ATTENTION_CHILD_STATE_OUT1);

    instr.SetDecode(child_valid & state_out1);

    // right shrift amout
    auto adpbias_matrix = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_1);
    auto adpbias_softmax = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_3);
    auto adpbias_out = m.state(GB_ATTENTION_CONFIG_REG_ADPBIAS_4);

    auto shift_const =
        2 * ADPTFLOW_MAN_WIDTH + 2 * ADPTFLOW_OFFSET_NEG - ATTENTION_NUM_FRAC;
    auto shift_bias =
        Concat(BvConst(0, 32 - adpbias_matrix.bit_width()), adpbias_matrix) +
        Concat(BvConst(0, 32 - adpbias_softmax.bit_width()), adpbias_softmax);

    auto is_left_shift = (shift_bias > shift_const);
    auto shift_amout = Ite(is_left_shift, shift_bias - shift_const,
                           BvConst(shift_const, 32) - shift_bias);

    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto accum_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_ACCUM_VECTOR));
      auto tmp = Ite(is_left_shift, GBAttentionLSH(accum_vector, shift_amout),
                     GBAttentionRSH(accum_vector, shift_amout));
      tmp = GBAttentionCompress(tmp, adpbias_out);

      auto out_data_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_OUT_VECTOR));
      instr.SetUpdate(out_data_vector, tmp);
    }

    auto next_state = BvConst(GB_ATTENTION_CHILD_STATE_OUT2,
                              GB_ATTENTION_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 17 ---- FSM OUT2
    auto instr = child.NewInstr("gb_attention_child_out2");
    auto state_out2 = (state == GB_ATTENTION_CHILD_STATE_OUT2);

    instr.SetDecode(child_valid & state_out2);

    auto mem_small_index = m.state(GB_ATTENTION_CONFIG_REG_MEMORY_INDEX_2);
    auto mem_small_base = GBGetSmallBufBase(m, mem_small_index);

    auto vector_index = vector_cntr;
    auto vector_index_b = Concat(vector_cntr, BvConst(0, 4));

    auto addr_small_base =
        Concat(BvConst(0, 32 - mem_small_base.bit_width()), mem_small_base) +
        Concat(BvConst(0, 32 - vector_index_b.bit_width()), vector_index_b);

    auto mem_small = m.state(GB_CORE_SMALL_BUFFER);
    auto mem_small_next = mem_small;

    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
      auto out_data_vector =
          child.state(GBGetVectorName(i, GB_ATTENTION_OUT_VECTOR));
      auto addr = addr_small_base + i;
      mem_small_next = Store(mem_small_next, addr, out_data_vector);
    }

    instr.SetUpdate(mem_small, mem_small_next);

    // control state update
    auto is_end = (vector_cntr >= num_vector - 1);
    auto vector_cntr_next = Ite(
        is_end, BvConst(0, GB_ATTENTION_VECTOR_CNTR_BITWIDTH), vector_cntr + 1);
    auto bmm_cntr_next =
        Ite(is_end, BvConst(0, GB_ATTENTION_BMM_CNTR_BITWIDTH), bmm_cntr);

    auto next_state = Ite(is_end,
                          BvConst(GB_ATTENTION_CHILD_STATE_FIN,
                                  GB_ATTENTION_CHILD_STATE_BITWIDTH),
                          BvConst(GB_ATTENTION_CHILD_STATE_PREP,
                                  GB_ATTENTION_CHILD_STATE_BITWIDTH));

    instr.SetUpdate(vector_cntr, vector_cntr_next);
    instr.SetUpdate(bmm_cntr, bmm_cntr_next);
    instr.SetUpdate(state, next_state);
  }

  { // instruction 18 ---- FSM FIN
    auto instr = child.NewInstr("gb_attention_child_fin");
    auto state_fin = (state == GB_ATTENTION_CHILD_STATE_FIN);

    instr.SetDecode(child_valid & state_fin);

    instr.SetUpdate(state, BvConst(GB_ATTENTION_CHILD_STATE_IDLE,
                                   GB_ATTENTION_CHILD_STATE_BITWIDTH));
    instr.SetUpdate(
        m.state(GB_ATTENTION_CHILD_VALID_FLAG),
        BvConst(GB_ATTENTION_INVALID, GB_ATTENTION_CHILD_VALID_FLAG_BITWIDTH));
  }

  ILA_INFO << "gb_attention summary: #state: "
           << child.state_num() - m.state_num() << '\t'
           << "#instr: " << child.instr_num();
}

} // namespace flex

} // namespace ilang
