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

// File: gb_layer_norm.cc

#include <flex/flex.h>
#include <flex/uninterpreted_func.h>

namespace ilang {

void AddChild_GB_LayerNorm_Child(Ila& m);

void DefineStartGBLayerNorm(Ila& m) {
  // TODO 
  auto instr = m.NewInstr("GB_LAYER_NORM_START");
  // Decode condition
  auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
  auto start_addr = TOP_GB_ADDR_MIN + START_ADDR_GB_LAYER_NORM;
  auto is_valid = m.state(GB_LAYER_NORM_CONFIG_REG_IS_VALID);

  instr.SetDecode(is_write & is_valid & (m.input(TOP_ADDR_IN) == start_addr));

  // update 04082020, using state machine instead of multilevel child to organize the child instructions
  auto state = m.state(GB_LAYER_NORM_CHILD_STATE);
  auto next_state = BvConst(GB_LAYER_NORM_CHILD_STATE_TIMESTEP_PREP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH);
  auto child_valid_flag = m.state(GB_LAYER_NORM_CHILD_VALID_FLAG);

  auto counter_ts = m.NewBvState(GB_LAYER_NORM_CNTR_TIMESTEP, GB_LAYER_NORM_CNTR_TIMESTEP_WIDTH);

  instr.SetUpdate(state, next_state);
  instr.SetUpdate(child_valid_flag, BvConst(GB_LAYER_NORM_VALID, GB_LAYER_NORM_CHILD_VALID_FLAG_BITWIDTH));
  instr.SetUpdate(counter_ts, BvConst(0, GB_LAYER_NORM_CNTR_TIMESTEP_WIDTH));
  
  AddChild_GB_LayerNorm_Child(m); // add child model to handle timestep level instructions
}

void AddChild_GB_LayerNorm_Child(Ila& m) {
  auto child = m.NewChild("Child_GBLayerNorm");
  auto child_valid = (m.state(GB_LAYER_NORM_CHILD_VALID_FLAG) == GB_LAYER_NORM_VALID);

  child.SetValid(child_valid);

  // child states

  // calcuate the address range for the selected memory index
  auto memory_index = m.state(GB_LAYER_NORM_CONFIG_REG_MEMORY_INDEX_1);
  // memory_min_addr_offset is 20-bit
  auto memory_min_addr_offset =
      Ite((memory_index == 0),
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0), BvConst(0, 4)),
          Ite((memory_index == 1),
              Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1), BvConst(0, 4)),
              Ite((memory_index == 2),
                  Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2), BvConst(0, 4)),
                  Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3), BvConst(0, 4)))));
  
  // FIXME: uncertain about the max addr offset for some memory managing instructions
  auto memory_max_addr_offset =
      Ite((memory_index == 0),
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1), BvConst(0, 4)) - 16,
          Ite((memory_index == 1),
              Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2), BvConst(0, 4)) - 16,
              Ite((memory_index == 2),
                  Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3), BvConst(0, 4)) - 16,
                  BvConst(GB_CORE_STORE_LARGE_SIZE, GB_CORE_STORE_LARGE_BITWIDTH))));
  // the size of the targeted memory block
  // FIXME: for now, I assume memory manager will not skip memory index, if max_addr_offset == 0, assume the 
  // max addr for the block is the largest address.
  auto block_size = 
    Ite((memory_max_addr_offset == 0),
        BvConst(GB_CORE_STORE_LARGE_SIZE, GB_CORE_STORE_LARGE_BITWIDTH) - memory_min_addr_offset,
        memory_max_addr_offset - memory_min_addr_offset);


  child.NewBvState(GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR, GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR_WIDTH);
  child.NewBvState(GB_LAYER_NORM_VECTOR_BASE_ADDR, GB_LAYER_NORM_VECTOR_BASE_ADDR_WIDTH);
  child.NewBvState(GB_LAYER_NORM_CNTR_BYTE, GB_LAYER_NORM_CNTR_BYTE_WIDTH);

  child.NewBvState(GB_LAYER_NORM_SUM_X, GB_LAYER_NORM_SUM_X_WIDTH);
  child.NewBvState(GB_LAYER_NORM_SUM_X_SQ, GB_LAYER_NORM_SUM_X_SQ_WIDTH);
  child.NewBvState(GB_LAYER_NORM_MEAN, GB_LAYER_NORM_MEAN_WIDTH);
  child.NewBvState(GB_LAYER_NORM_INV_STD, GB_LAYER_NORM_INV_STD_WIDTH);
  child.NewBvState(GB_LAYER_NORM_VAR, GB_LAYER_NORM_VAR_WIDTH);

  child.NewBvState(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA,
                        GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA_WIDTH);
  child.NewBvState(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_BETA,
                        GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDT_BETA_WIDTH);

  // control related states  
  child.NewBvState(GB_LAYER_NORM_CNTR_VECTOR, GB_LAYER_NORM_CNTR_VECTOR_WIDTH);
  child.NewBvState(GB_LAYER_NORM_SUM_DONE_FLAG, GB_LAYER_NORM_SUM_DONE_FLAG_WIDTH);
  child.NewBvState(GB_LAYER_NORM_MEAN_DONE_FLAG, GB_LAYER_NORM_MEAN_DONE_FLAG_WIDTH);


  // common variables
  auto num_timestep = m.state(GB_LAYER_NORM_CONFIG_REG_NUM_TIMESTEP_1);
  auto num_vector = m.state(GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1);
  
  // timestep counter is at the parent level, initilized at the parent instruction.
  auto counter_ts = m.state(GB_LAYER_NORM_CNTR_TIMESTEP);
  auto counter_v = child.state(GB_LAYER_NORM_CNTR_VECTOR);

  auto base_addr_ts = child.state(GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR);
  auto base_addr_v = child.state(GB_LAYER_NORM_VECTOR_BASE_ADDR);

  auto sum_x = child.state(GB_LAYER_NORM_SUM_X);
  auto sum_x_sq = child.state(GB_LAYER_NORM_SUM_X_SQ);

  // update
  auto state = m.state(GB_LAYER_NORM_CHILD_STATE);

  { // instr 0 ---- setting timestep level preparation
    auto instr = child.NewInstr("gb_layer_norm_timestep_level_op");
    auto state_ts = (state == GB_LAYER_NORM_CHILD_STATE_TIMESTEP_PREP);

    instr.SetDecode(child_valid & state_ts);

    // state updates

    // fixed all the parameters to 20 bit
    auto num_vector_20 = Concat(BvConst(0, 20-num_vector.bit_width()), num_vector);
    auto timestep_size = num_vector_20 * GB_CORE_SCALAR;
    auto group_size = timestep_size * GB_CORE_LARGE_NUM_BANKS;

    auto cntr_ts_20 = Concat(BvConst(0, 20-counter_ts.bit_width()), counter_ts);
    auto group_index = cntr_ts_20 / BvConst(GB_CORE_SCALAR, 20);
    auto group_offset = URem(cntr_ts_20, BvConst(GB_CORE_SCALAR, 20));

    auto base_addr_offset = group_index * group_size + group_offset * GB_CORE_SCALAR;

    // update the base addr for the current timestep
    instr.SetUpdate(base_addr_ts, memory_min_addr_offset + base_addr_offset);

    // clear the sum register for current timestep
    instr.SetUpdate(sum_x, BvConst(0, GB_LAYER_NORM_SUM_X_WIDTH));
    instr.SetUpdate(sum_x_sq, BvConst(0, GB_LAYER_NORM_SUM_X_SQ_WIDTH));

    // initiate the control signals
    instr.SetUpdate(counter_v, BvConst(0, GB_LAYER_NORM_CNTR_VECTOR_WIDTH));

    // update state machine
    auto next_state = BvConst(GB_LAYER_NORM_CHILD_STATE_SUM_VECTOR_PREP,
                                GB_LAYER_NORM_CHILD_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);
  }

  { // instr 1 ---- setting vector level sum preparation
    auto instr = child.NewInstr("gb_layer_norm_sum_vector_level_prep");
    auto state_svp = (state == GB_LAYER_NORM_CHILD_STATE_SUM_VECTOR_PREP);

    instr.SetDecode(child_valid & state_svp);

    // extend the counter parameter to 20 bit
    auto counter_v_20 = Concat(BvConst(0, 20 - counter_v.bit_width()), counter_v);
    auto row_size = GB_CORE_SCALAR * GB_CORE_LARGE_NUM_BANKS;
    auto base_addr_v_tmp = base_addr_ts + counter_v_20 * row_size;

    auto counter_byte = child.state(GB_LAYER_NORM_CNTR_BYTE);

    instr.SetUpdate(counter_v, counter_v + 1);
    instr.SetUpdate(counter_byte, BvConst(0, GB_LAYER_NORM_CNTR_BYTE_WIDTH));
    instr.SetUpdate(base_addr_v, base_addr_v_tmp); 

    auto next_state = 
          Ite(counter_v >= num_vector - 1,
                BvConst(GB_LAYER_NORM_CHILD_STATE_MEAN_OP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH),
                BvConst(GB_LAYER_NORM_CHILD_STATE_SUM_BYTE_OP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH));
    instr.SetUpdate(state, next_state);
  }

  { // instr 2 ---- compute sum of the current vector
    auto instr = child.NewInstr("gb_layer_norm_sum_byte_op");
    auto state_sb = (state == GB_LAYER_NORM_CHILD_STATE_SUM_BYTE_OP);

    instr.SetDecode(child_valid & state_sb);

    auto counter_byte = child.state(GB_LAYER_NORM_CNTR_BYTE);
    auto mem = m.state(GB_CORE_LARGE_BUFFER);

    // control states update
    instr.SetUpdate(counter_byte, counter_byte + 1);

    // data updates
    auto counter_byte_20 = Concat(BvConst(0, 20-counter_byte.bit_width()), counter_byte);
    auto addr_20 = base_addr_v + counter_byte_20;
    auto addr_32 = Concat(BvConst(0, 32-addr_20.bit_width()), addr_20);

    auto data = Load(mem, addr_32);
    // TODO: implement the conversion from data to adpfloat type.
    // TODO: Also need to extend the bit width the temp result in case of overflow.

    auto adpfloat_data = Concat(BvConst(0, 16), data);
    // TODO: check the implementation of EMUL in the flexNLP code
    auto adpfloat_data_sq = (adpfloat_data * adpfloat_data) >> ACT_NUM_FRAC;

    instr.SetUpdate(sum_x, sum_x + adpfloat_data);
    instr.SetUpdate(sum_x_sq, sum_x_sq + adpfloat_data_sq);

    auto next_state = 
      Ite(counter_byte >= (GB_CORE_SCALAR - 1),
            BvConst(GB_LAYER_NORM_CHILD_STATE_SUM_VECTOR_PREP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH),
            BvConst(GB_LAYER_NORM_CHILD_STATE_SUM_BYTE_OP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH));
    instr.SetUpdate(state, next_state);
  }

  { // instr 3 ---- compute the mean, variance and inv_std of current timestep
    auto instr = child.NewInstr("gb_layer_norm_mean_op");
    auto state_mean = (state == GB_LAYER_NORM_CHILD_STATE_MEAN_OP);

    instr.SetDecode(child_valid & state_mean);

    // control condition update
    instr.SetUpdate(counter_v, BvConst(0, GB_LAYER_NORM_CNTR_VECTOR_WIDTH));

    // calcuate the mean, variance and inverted standard variance
    auto mean_x = child.state(GB_LAYER_NORM_MEAN);
    auto var_x = child.state(GB_LAYER_NORM_VAR);
    auto inv_std_x = child.state(GB_LAYER_NORM_INV_STD);
    // E[X], E[X^2]
    auto num_vector_24 = Concat(BvConst(0, 16), num_vector);
    auto divisor = num_vector_24 * GB_CORE_SCALAR;
    auto mean_tmp = sum_x / divisor; 
    auto sqmean_tmp = sum_x_sq / divisor;

    // E[X]^2
    // TODO: EMUL
    auto mean_sq_tmp = (mean_tmp * mean_tmp) >> K_ACT_NUM_FRAC;
    // Var[X] = E[X^2] - E[X]^2
    auto var_tmp = sqmean_tmp - mean_sq_tmp;
    // inv_std = 1/sqrt(VAR[X])
    // TODO: implement the uninterpretted function sqaure root
    auto inv_std_tmp = var_tmp;

    // states updates
    instr.SetUpdate(mean_x, mean_tmp);
    instr.SetUpdate(var_x, var_tmp);
    instr.SetUpdate(inv_std_x, inv_std_tmp);

    auto next_state = 
          BvConst(GB_LAYER_NORM_CHILD_STATE_NORM_VECTOR_PREP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);
    
  }

  { // instr 4 ---- setting vector level parameters for calculating norm of current timestep
    auto instr = child.NewInstr("gb_layer_norm_norm_vector_prep");
    auto state_nvp = (state == GB_LAYER_NORM_CHILD_STATE_NORM_VECTOR_PREP);

    instr.SetDecode(child_valid & state_nvp);

    auto counter_v_20 = Concat(BvConst(0, 12), counter_v);
    auto base_addr_v_tmp = base_addr_ts + counter_v_20 * GB_CORE_SCALAR;

    auto counter_v_16 = Concat(BvConst(0, 8), counter_v);
    auto base_addr_gamma_tmp = m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_5) +    
                                counter_v_16 * GB_CORE_SCALAR;
    auto base_addr_beta_tmp = m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_6) +     
                                counter_v_16 * GB_CORE_SCALAR;

    auto counter_byte = child.state(GB_LAYER_NORM_CNTR_BYTE);
    auto base_addr_gamma = child.state(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA);
    auto base_addr_beta = child.state(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_BETA);

    instr.SetUpdate(counter_v, counter_v + 1);
    instr.SetUpdate(counter_byte, BvConst(0, GB_LAYER_NORM_CNTR_BYTE_WIDTH));
    instr.SetUpdate(base_addr_v, base_addr_v_tmp);
    instr.SetUpdate(base_addr_gamma, base_addr_gamma_tmp);
    instr.SetUpdate(base_addr_beta, base_addr_beta_tmp);

    auto next_state = 
      Ite(counter_v >= num_vector - 1, 
            BvConst(GB_LAYER_NORM_CHILD_STATE_NEXT, GB_LAYER_NORM_CHILD_STATE_BITWIDTH),
            BvConst(GB_LAYER_NORM_CHILD_STATE_NORM_BYTE_OP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH));
    instr.SetUpdate(state, next_state);
  }

  { // instr 5 ---- calculating norm for each byte in current vector
    auto instr = child.NewInstr("gb_layer_norm_norm_byte_op");
    auto state_nb = (state == GB_LAYER_NORM_CHILD_STATE_NORM_BYTE_OP);

    instr.SetDecode(child_valid & state_nb);

    auto counter_byte = child.state(GB_LAYER_NORM_CNTR_BYTE);
    auto base_addr_gamma = child.state(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA);
    auto base_addr_beta = child.state(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_BETA);
    // control signals update
    instr.SetUpdate(counter_byte, counter_byte + 1);
    // computation
    auto counter_byte_20 = Concat(BvConst(0, 15), counter_byte);
    auto counter_byte_14 = Concat(BvConst(0, 9), counter_byte);

    auto addr_d = base_addr_v + counter_byte_20;
    auto addr_g = base_addr_gamma + counter_byte_14;
    auto addr_b = base_addr_beta + counter_byte_14;

    auto addr_d_32 = Concat(BvConst(0, 12), addr_d);
    auto addr_g_32 = Concat(BvConst(0, 18), addr_g);
    auto addr_b_32 = Concat(BvConst(0, 18), addr_b);

    auto large_buf = m.state(GB_CORE_LARGE_BUFFER);
    auto small_buf = m.state(GB_CORE_SMALL_BUFFER);
    auto mean_x = child.state(GB_LAYER_NORM_MEAN);
    auto var_x = child.state(GB_LAYER_NORM_VAR);
    auto inv_std_x = child.state(GB_LAYER_NORM_INV_STD);

    // TODO: implement adpfloat2fixed, both data, gamma, beta.
    auto data = Load(large_buf, addr_d_32);
    auto gamma = Load(small_buf, addr_g_32);
    auto beta = Load(small_buf, addr_b_32);
    // TODO: check the EMUL operations
    auto tmp1 = ((data - mean_x) * inv_std_x) >> K_ACT_NUM_FRAC;
    auto tmp2 = (tmp1 * gamma) >> K_ACT_NUM_FRAC;
    auto result = tmp2 + beta;

    instr.SetUpdate(large_buf, Store(large_buf, addr_d_32, result));

    auto next_state = 
      Ite(counter_byte >= (GB_CORE_SCALAR - 1),
            BvConst(GB_LAYER_NORM_CHILD_STATE_NORM_VECTOR_PREP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH),
            BvConst(GB_LAYER_NORM_CHILD_STATE_NORM_BYTE_OP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH));
    instr.SetUpdate(state, next_state);
  }

  { // instr 6 ---- determing whether move to next timestep or finished
    auto instr = child.NewInstr("gb_layer_norm_child_next");
    auto state_valid = (state == GB_LAYER_NORM_CHILD_STATE_NEXT);

    instr.SetDecode(child_valid & state_valid);

    auto next_state = BvConst(GB_LAYER_NORM_CHILD_STATE_TIMESTEP_PREP, GB_LAYER_NORM_CHILD_STATE_BITWIDTH);
    auto is_done = (counter_ts >= (m.state(GB_LAYER_NORM_ITERATIONS) - 1));

    instr.SetUpdate(state, next_state);
    instr.SetUpdate(m.state(GB_LAYER_NORM_CHILD_VALID_FLAG),
                    Ite(is_done, BvConst(GB_LAYER_NORM_INVALID,GB_LAYER_NORM_CHILD_VALID_FLAG_BITWIDTH),
                                  BvConst(GB_LAYER_NORM_VALID, GB_LAYER_NORM_CHILD_VALID_FLAG_BITWIDTH)));

  }
  
}

}; // namespace ilang
