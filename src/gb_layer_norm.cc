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
void AddChild_GB_LayerNorm_Vector_Level_Norm(Ila& m);

void DefineStartGBLayerNorm(Ila& m) {
  // TODO 
  auto instr = m.NewInstr("Start_GBLayer_Norm");
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

void AddChild_LayerNorm_Timestep_Level(Ila& m) {
  auto child_ts = m.NewChild("GBLayerNorm_Timestep_Level");

  auto counter_ts = m.state(GB_LAYER_NORM_CNTR_TIMESTEP);

  child_ts.SetValid(counter_ts < m.state(GB_LAYER_NORM_ITERATIONS));
  child_ts.SetFetch(BvConst(1, 1));

  // states for timestep level operations
  child_ts.NewBvState(GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR, GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_CNTR_VECTOR, GB_LAYER_NORM_CNTR_VECTOR_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_SUM_X, GB_LAYER_NORM_SUM_X_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_SUM_X_SQ, GB_LAYER_NORM_SUM_X_SQ_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_MEAN, GB_LAYER_NORM_MEAN_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_INV_STD, GB_LAYER_NORM_INV_STD_WIDTH);
  child_ts.NewBvState(GB_LAYER_NORM_VAR, GB_LAYER_NORM_VAR_WIDTH);

  // common variables
  auto num_vector = m.state(GB_LAYER_NORM_CONFIG_REG_NUM_VECTOR_1);
  auto counter_v = child_ts.state(GB_LAYER_NORM_CNTR_VECTOR);
  auto base_addr_ts = child_ts.state(GB_LAYER_NORM_TIMESTEP_LEVEL_BASE_ADDR);
  auto sum_x = child_ts.state(GB_LAYER_NORM_SUM_X);
  auto sum_x_sq = child_ts.state(GB_LAYER_NORM_SUM_X_SQ);
  auto mean_x = child_ts.state(GB_LAYER_NORM_MEAN);

  auto inv_std_x = child_ts.state(GB_LAYER_NORM_INV_STD);

  {
    auto instr = child_ts.NewInstr("gb_layer_norm_timestep_level_op");
    instr.SetDecode(counter_ts < m.state(GB_LAYER_NORM_ITERATIONS));

    // state updates
    auto timestep_size = num_vector * GB_CORE_SCALAR;
    auto base_addr_offset = timestep_size * counter_ts;

    // update the base addr for the current timestep
    instr.SetUpdate(base_addr_ts, m.state(GB_LAYER_NORM_MEM_MIN_ADDR_OFFSET) + base_addr_offset);

    // clear the sum register for current timestep
    instr.SetUpdate(sum_x, BvConst(0, GB_LAYER_NORM_SUM_X_WIDTH));
    instr.SetUpdate(sum_x_sq, BvConst(0, GB_LAYER_NORM_SUM_X_SQ_WIDTH));

    // add 3rd level child here for computing the sum of the data for the whole timestep
    AddChild_GB_LayerNorm_Vector_Level_Sum(m);

    // calculate Variance and Mean of the data in the whole timestep
    
    instr.SetUpdate()


  }

}

}; // namespace ilang
