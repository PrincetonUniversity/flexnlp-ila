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

namespace ilang {

void AddChild_GB_LayerNorm_Timestep_Level(Ila& m);
void AddChild_GB_LayerNorm_Vector_Level_Sum(Ila& m);
void AddChild_GB_LayerNorm_Timestep_Level_Mean(Ila& m);
void AddChild_GB_LayerNorm_Vector_Level_Norm(Ila& m);
void AddChild_GB_LayerNorm_Byte_Level_Sum(Ila& m);
void AddChild_GB_LayerNorm_Byte_Level_Norm(Ila& m);

void DefineStartGBLayerNorm(Ila& m) {
  // TODO 
  auto instr = m.NewInstr("GB_LAYER_NORM_START");
  // Decode condition
  auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
  auto start_addr = TOP_GB_ADDR_MIN + START_ADDR_GB_LAYER_NORM;
  auto is_valid = m.state(GB_LAYER_NORM_CONFIG_REG_IS_VALID);

  instr.SetDecode(is_write & is_valid & (m.input(TOP_ADDR_IN) == start_addr));

  // parameters from the GB_LAYER_NORM configurations
  auto memory_index = m.state(GB_LAYER_NORM_CONFIG_REG_MEMORY_INDEX_1);
  auto num_timestep = m.state(GB_LAYER_NORM_CONFIG_REG_NUM_TIMESTEP_1);
  auto adpbias_enc = m.state(GB_LAYER_NORM_CONFIG_REG_ADPBIAS_1);
  auto adpbias_gamma = m.state(GB_LAYER_NORM_CONFIG_REG_ADPBIAS_3);
  auto adpbias_beta = m.state(GB_LAYER_NORM_CONFIG_REG_ADPBIAS_4);

  // calcuate the address range for the selected memory index
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

  // states update for child instructions
  instr.SetUpdate(m.state(GB_LAYER_NORM_ITERATIONS), num_timestep);
  instr.SetUpdate(m.state(GB_LAYER_NORM_CNTR_TIMESTEP), BvConst(0, GB_LAYER_NORM_CNTR_TIMESTEP_WIDTH));
  instr.SetUpdate(m.state(GB_LAYER_NORM_MEM_BLOCK_SIZE), block_size);
  instr.SetUpdate(m.state(GB_LAYER_NORM_MEM_MIN_ADDR_OFFSET), memory_min_addr_offset);
  
  AddChild_GB_LayerNorm_Timestep_Level(m); // add child model to handle timestep level instructions
}

void AddChild_GB_LayerNorm_Timestep_Level(Ila& m) {
  auto child_ts = m.NewChild("GBLayerNorm_Timestep_Level");
  
  auto counter_ts = m.state(GB_LAYER_NORM_CNTR_TIMESTEP);

  child_ts.SetValid(counter_ts < m.state(GB_LAYER_NORM_ITERATIONS));
  child_ts.SetFetch(BvConst(1, 1));

  // states for timestep level operations
  child_ts.NewBvState(GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR, GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_VECTOR_BASE_ADDR, GB_LAYER_NORM_VECTOR_BASE_ADDR_WIDTH);

  child_ts.NewBvState(GB_LAYER_NORM_SUM_X, GB_LAYER_NORM_SUM_X_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_SUM_X_SQ, GB_LAYER_NORM_SUM_X_SQ_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_MEAN, GB_LAYER_NORM_MEAN_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_INV_STD, GB_LAYER_NORM_INV_STD_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_VAR, GB_LAYER_NORM_VAR_WIDTH);

  // control related states  
  child_ts.NewBvState(GB_LAYER_NORM_CNTR_VECTOR, GB_LAYER_NORM_CNTR_VECTOR_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_SUM_DONE_FLAG, GB_LAYER_NORM_SUM_DONE_FLAG_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_MEAN_DONE_FLAG, GB_LAYER_NORM_MEAN_DONE_FLAG_WIDTH);

  // common variables
  auto num_vector = m.state(GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1);
  
  auto counter_v = child_ts.state(GB_LAYER_NORM_CNTR_VECTOR);
  auto base_addr_ts = child_ts.state(GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR);
  auto base_addr_v = child_ts.state(GB_LAYER_NORM_VECTOR_BASE_ADDR);

  auto sum_x = child_ts.state(GB_LAYER_NORM_SUM_X);
  auto sum_x_sq = child_ts.state(GB_LAYER_NORM_SUM_X_SQ);

  auto flag_sum = child_ts.state(GB_LAYER_NORM_SUM_DONE_FLAG);
  auto flag_mean = child_ts.state(GB_LAYER_NORM_MEAN_DONE_FLAG);

  {
    auto instr = child_ts.NewInstr("gb_layer_norm_timestep_level_op");
    instr.SetDecode(counter_ts < m.state(GB_LAYER_NORM_ITERATIONS));

    // state updates
    auto timestep_size = Concat(BvConst(0, 8), num_vector) * GB_CORE_SCALAR;
    auto base_addr_offset = Concat(BvConst(0,4), timestep_size * counter_ts);

    // update the base addr for the current timestep
    instr.SetUpdate(base_addr_ts, m.state(GB_LAYER_NORM_MEM_MIN_ADDR_OFFSET) + base_addr_offset);

    // clear the sum register for current timestep
    instr.SetUpdate(sum_x, BvConst(0, GB_LAYER_NORM_SUM_X_WIDTH));
    instr.SetUpdate(sum_x_sq, BvConst(0, GB_LAYER_NORM_SUM_X_SQ_WIDTH));

    // initiate the control signals
    instr.SetUpdate(counter_v, BvConst(0, GB_LAYER_NORM_CNTR_VECTOR_WIDTH));
    instr.SetUpdate(flag_sum, BvConst(0, GB_LAYER_NORM_SUM_DONE_FLAG_WIDTH));
    instr.SetUpdate(flag_mean, BvConst(0, GB_LAYER_NORM_MEAN_DONE_FLAG_WIDTH));

    // add 3rd level child here for computing the sum of the data for the whole timestep
    AddChild_GB_LayerNorm_Vector_Level_Sum(m);

    // add child to compute the variance and mean of the data 
    AddChild_GB_LayerNorm_Timestep_Level_Mean(m);

    // add child to compute the norm of the data;
    AddChild_GB_LayerNorm_Vector_Level_Norm(m);
  }
}

// child that compute the sum of the current timestep
void AddChild_GB_LayerNorm_Vector_Level_Sum(Ila& m) {
  auto child_ts = m.child("GBLayerNorm_Timestep_Level");
  auto child_v_sum = child_ts.NewChild("GBLayerNorm_Vector_Level_sum");

  auto counter_v = child_ts.state(GB_LAYER_NORM_CNTR_VECTOR);
  auto num_vector = m.state(GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1);
  auto flag_sum = child_ts.state(GB_LAYER_NORM_SUM_DONE_FLAG);
  auto flag_mean = child_ts.state(GB_LAYER_NORM_MEAN_DONE_FLAG);

  child_v_sum.SetValid((counter_v < num_vector) & (flag_sum == 0) & (flag_mean == 0));
  child_v_sum.SetFetch(BvConst(1,1));

  // states for Vector level model
  child_v_sum.NewBvState(GB_LAYER_NORM_CNTR_BYTE, GB_LAYER_NORM_CNTR_BYTE_WIDTH);
  auto counter_byte = child_v_sum.state(GB_LAYER_NORM_CNTR_BYTE);
  auto base_addr_ts = child_ts.state(GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR);
  auto base_addr_v = child_ts.state(GB_LAYER_NORM_VECTOR_BASE_ADDR);

  {
    auto instr = child_v_sum.NewInstr("gb_layer_norm_vector_level_sum");
    instr.SetDecode(counter_v < num_vector);

    auto counter_v_20 = Concat(BvConst(0, 12), counter_v);
    auto base_addr_v_tmp = base_addr_ts + counter_v_20 * GB_CORE_SCALAR;

    instr.SetUpdate(counter_v, counter_v + 1);
    instr.SetUpdate(counter_byte, BvConst(0, GB_LAYER_NORM_CNTR_BYTE_WIDTH));
    instr.SetUpdate(base_addr_v, base_addr_v_tmp);

    AddChild_GB_LayerNorm_Byte_Level_Sum(m);
  }
}

void AddChild_GB_LayerNorm_Byte_Level_Sum(Ila& m) {
  auto child_ts = m.child("GBLayerNorm_Timestep_Level");
  auto child_v_sum = child_ts.child("GBLayerNorm_Vector_Level_sum");
  auto child_b_sum = child_v_sum.NewChild("GBLayerNorm_Byte_Level_sum");

  auto counter_byte = child_v_sum.state(GB_LAYER_NORM_CNTR_BYTE);
  
  child_b_sum.SetValid(counter_byte < GB_CORE_SCALAR);
  child_b_sum.SetFetch(BvConst(1,1));

  auto counter_v = child_ts.state(GB_LAYER_NORM_CNTR_VECTOR);
  auto num_vector = m.state(GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1);
  auto flag_sum = child_ts.state(GB_LAYER_NORM_SUM_DONE_FLAG);

  auto mem = m.state(GB_CORE_LARGE_BUFFER);
  auto sum_x = child_ts.state(GB_LAYER_NORM_SUM_X);
  auto sum_x_sq = child_ts.state(GB_LAYER_NORM_SUM_X_SQ);
  auto base_addr_v = child_ts.state(GB_LAYER_NORM_VECTOR_BASE_ADDR);

  {
    auto instr = child_b_sum.NewInstr("gb_layer_norm_byte_level_sum");
    instr.SetDecode(counter_byte < GB_CORE_SCALAR);

    // control states update
    instr.SetUpdate(counter_byte, counter_byte + 1);

    // data updates
    
    auto counter_byte_20 = Concat(BvConst(0, 15), counter_byte);
    auto addr = base_addr_v + counter_byte_20;
    auto addr_32 = Concat(BvConst(0, 12), addr);
    auto data = Load(mem, addr_32);
    // TODO: implement the conversion from data to adpfloat type.
    // TODO: Also need to extend the bit width the temp result in case of overflow.
    auto adpfloat_data = Concat(BvConst(0, 16), data);
    // TODO: check the implementation of EMUL in the flexNLP code
    auto adpfloat_data_sq = (adpfloat_data * adpfloat_data) >> K_ACT_NUM_FRAC;

    instr.SetUpdate(sum_x, sum_x + adpfloat_data);
    instr.SetUpdate(sum_x_sq, sum_x_sq + adpfloat_data_sq);

    auto f_cond = (counter_v == num_vector - 1) & (counter_byte == GB_CORE_SCALAR - 1);
    instr.SetUpdate(flag_sum, Ite(f_cond, 
                                    BvConst(1, GB_LAYER_NORM_SUM_DONE_FLAG_WIDTH),
                                    BvConst(0, GB_LAYER_NORM_SUM_DONE_FLAG_WIDTH)));
  }
  
}

// child that compute the mean, variance and inv_std of current timestep
void AddChild_GB_LayerNorm_Timestep_Level_Mean(Ila& m) {
  auto child_ts = m.child("GBLayerNorm_Timestep_Level");
  auto child_ts_mean = child_ts.NewChild("GBLayerNorm_Timestep_Level_Mean");

  auto counter_v = child_ts.state(GB_LAYER_NORM_CNTR_VECTOR);
  auto flag_sum = child_ts.state(GB_LAYER_NORM_SUM_DONE_FLAG);
  auto flag_mean = child_ts.state(GB_LAYER_NORM_MEAN_DONE_FLAG);

  child_ts_mean.SetValid((flag_sum == 1) & (flag_mean == 0));
  child_ts_mean.SetFetch(BvConst(1, 1));

  auto mean_x = child_ts.state(GB_LAYER_NORM_MEAN);
  auto var_x = child_ts.state(GB_LAYER_NORM_VAR);
  auto inv_std_x = child_ts.state(GB_LAYER_NORM_INV_STD);

  auto sum_x = child_ts.state(GB_LAYER_NORM_SUM_X);
  auto sum_x_sq = child_ts.state(GB_LAYER_NORM_SUM_X_SQ);
  auto num_vector = m.state(GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1);

  {
    auto instr = child_ts_mean.NewInstr("gb_layer_norm_timestep_level_mean");
    instr.SetDecode((flag_sum == 1) & (flag_mean == 0));

    // control condition update
    instr.SetUpdate(flag_mean, BvConst(1, GB_LAYER_NORM_MEAN_DONE_FLAG_WIDTH));
    instr.SetUpdate(counter_v, BvConst(0, GB_LAYER_NORM_CNTR_VECTOR_WIDTH));

    // calcuate the mean, variance and inverted standard variance
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

  }
}

// child taht compute the norm of current timestep.
void AddChild_GB_LayerNorm_Vector_Level_Norm(Ila& m) {
  auto child_ts = m.child("GBLayerNorm_Timestep_Level");
  auto child_v_norm = child_ts.NewChild("GBLayerNorm_Vector_Level_Norm");

  auto counter_v = child_ts.state(GB_LAYER_NORM_CNTR_VECTOR);
  auto num_vector = m.state(GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1);
  auto flag_sum = child_ts.state(GB_LAYER_NORM_SUM_DONE_FLAG);
  auto flag_mean = child_ts.state(GB_LAYER_NORM_MEAN_DONE_FLAG);

  child_v_norm.SetValid(counter_v < num_vector & (flag_sum == 1) & (flag_mean == 1));
  child_v_norm.SetFetch(BvConst(1,1));

  auto mean_x = child_ts.state(GB_LAYER_NORM_MEAN);
  auto var_x = child_ts.state(GB_LAYER_NORM_VAR);
  auto inv_std_x = child_ts.state(GB_LAYER_NORM_INV_STD);

  // states for Vector level model
  child_v_norm.NewBvState(GB_LAYER_NORM_CNTR_BYTE, GB_LAYER_NORM_CNTR_BYTE_WIDTH);

  // states for Vector level base addresses for gamma and beta in small buffer
  child_v_norm.NewBvState(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA,
                            GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA_WIDTH);
  child_v_norm.NewBvState(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_BETA,
                            GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDT_BETA_WIDTH);

  auto counter_byte = child_v_norm.state(GB_LAYER_NORM_CNTR_BYTE);
  auto base_addr_ts = child_ts.state(GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR);
  auto base_addr_v = child_ts.state(GB_LAYER_NORM_VECTOR_BASE_ADDR);
  auto base_addr_gamma = child_v_norm.state(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA);
  auto base_addr_beta = child_v_norm.state(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_BETA);

  {
    auto instr = child_v_norm.NewInstr("gb_layer_norm_vector_level_norm");
    instr.SetDecode(counter_v < num_vector);

    auto counter_v_20 = Concat(BvConst(0, 12), counter_v);
    auto base_addr_v_tmp = base_addr_ts + counter_v_20 * GB_CORE_SCALAR;

    auto counter_v_16 = Concat(BvConst(0, 8), counter_v);
    auto base_addr_gamma_tmp = m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_5) +    \
                                counter_v_16 * GB_CORE_SCALAR;
    auto base_addr_beta_tmp = m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_6) +     \
                                counter_v_16 * GB_CORE_SCALAR;

    instr.SetUpdate(counter_v, counter_v + 1);
    instr.SetUpdate(counter_byte, BvConst(0, GB_LAYER_NORM_CNTR_BYTE_WIDTH));
    instr.SetUpdate(base_addr_v, base_addr_v_tmp);
    instr.SetUpdate(base_addr_gamma, base_addr_gamma_tmp);
    instr.SetUpdate(base_addr_beta, base_addr_beta_tmp);

    AddChild_GB_LayerNorm_Byte_Level_Norm(m);
  }
}

void AddChild_GB_LayerNorm_Byte_Level_Norm(Ila& m){
  auto child_ts = m.child("GBLayerNorm_Timestep_Level");
  auto child_v_norm = child_ts.child("GBLayerNorm_Vector_Level_Norm");
  auto child_b_norm = child_v_norm.NewChild("GBLayerNorm_Byte_Level_Norm");

  auto counter_byte = child_v_norm.state(GB_LAYER_NORM_CNTR_BYTE);
  
  child_b_norm.SetValid(counter_byte < GB_CORE_SCALAR);
  child_b_norm.SetFetch(BvConst(1,1));

  // statistics
  auto mean_x = child_ts.state(GB_LAYER_NORM_MEAN);
  auto var_x = child_ts.state(GB_LAYER_NORM_VAR);
  auto inv_std_x = child_ts.state(GB_LAYER_NORM_INV_STD);

  // memory state
  auto base_addr_v = child_ts.state(GB_LAYER_NORM_VECTOR_BASE_ADDR);
  auto base_addr_gamma = child_v_norm.state(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_GAMMA);
  auto base_addr_beta = child_v_norm.state(GB_LAYER_NORM_VECTOR_LEVEL_BASE_ADDR_BETA);
  
  auto large_buf = m.state(GB_CORE_LARGE_BUFFER);
  auto small_buf = m.state(GB_CORE_SMALL_BUFFER);

  {
    auto instr = child_b_norm.NewInstr("gb_layer_norm_byte_level_norm");
    instr.SetDecode(counter_byte < GB_CORE_SCALAR);

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

    // TODO: implement adpfloat2fixed, both data, gamma, beta.
    auto data = Load(large_buf, addr_d_32);
    auto gamma = Load(small_buf, addr_g_32);
    auto beta = Load(small_buf, addr_b_32);
    // TODO: check the EMUL operations
    auto tmp1 = ((data - mean_x) * inv_std_x) >> K_ACT_NUM_FRAC;
    auto tmp2 = (tmp1 * gamma) >> K_ACT_NUM_FRAC;
    auto result = tmp2 + beta;

    instr.SetUpdate(large_buf, Store(large_buf, addr_d_32, result));
  }


}

}; // namespace ilang
