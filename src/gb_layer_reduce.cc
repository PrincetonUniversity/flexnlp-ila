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

// File: gb_layer_reduce.cc

#include <flex/flex.h>

namespace ilang {

void AddChild_gb_lr_ts(Ila& m);
void AddChild_gb_lr_v(Ila& m, Ila& child_ts);

void DefineStartGBLayerReduce(Ila& m) {
  auto instr = m.NewInstr("Start_GBLayer_Reduce");
  // Decode condition
  auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
  auto start_addr = TOP_GB_ADDR_MIN + START_ADDR_GB_LAYER_REDUCE;
  auto is_valid = m.state(GB_LAYER_REDUCE_CONFIG_REG_IS_VALID);

  instr.SetDecode(is_write & is_valid & (m.input(TOP_ADDR_IN) == start_addr));

  // Parameters from the GB_LAYER_REDUCE configurations

  auto mode = m.state(GB_LAYER_REDUCE_CONFIG_REG_MODE);
  auto memory_index = m.state(GB_LAYER_REDUCE_CONFIG_REG_MEMORY_INDEX_1);
  auto num_vector = m.state(GB_LAYER_REDUCE_CONFIG_REG_NUM_VECTOR_1);
  auto num_timestep = m.state(GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1);

  // Parameters form the GB_CORE_MEM_MNGR_LARGE
  auto memory_min_addr_offset =
      Ite((memory_index == 0),
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0), BvConst(0, 4)),
          Ite((memory_index == 1),
              Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1), BvConst(0, 4)),
              Ite((memory_index == 2),
                  Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2), BvConst(0, 4)),
                  Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3), BvConst(0, 4)))));

  auto memory_max_addr_offset =
      Ite((memory_index == 0),
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1) - 16, BvConst(0, 4)),
          Ite((memory_index == 1),
              Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2) - 16, BvConst(0, 4)),
              Ite((memory_index == 2),
                  Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3) - 16, BvConst(0, 4)),
                  BvConst(GB_CORE_STORE_LARGE_SIZE, GB_CORE_STORE_LARGE_BITWIDTH))));
  // FIXME: use GB_CORE_STORE_LARGE_ADDR_MAX/MIN instead of concrete number
  // same as bit-width (done)

  // Parameter preprocessing, translating parameters used in ILA model
  
  // how many iterations should the layer reduce performs in pairs
  auto pair_num = Ite((SelectBit(num_timestep, 0) == 0), 
                    num_timestep / BvConst(2, GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH),
                    (num_timestep - 1) / BvConst(2, GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH));

  auto timestep_size = num_vector * 16;

  // the size of the targeted memory block
  auto block_size =
      memory_max_addr_offset - memory_min_addr_offset; 

// The over_flow should be detected elsewhere?
// the overwrite situation happens when num_of_timestep is larger than given.
// Thus the additional operations will do the pooling on time_step which are already
// been pooled.

//   auto is_overflow =
//       Ite((timestep_size * num_timestep > block_size), 1,
//           0); // check whether the data has exceeded the block size
 
  auto iterations = pair_num;

  // layer reduce operations
  // memory layout in ILA, fixed 128bit length for a vector, modeled using 8
  // bit, 16 entries per vector.
  // **************************
  // **   entry 0, 8bit wide **
  // **************************
  // **   entry 1, 8bit      **
  // **************************
  //          ... ...

  // operations, 2 layers of loops
  // 1. loop of iterations: block level
  // 2. loop of vector_entries: vector level
  // operations are implemented by 3 level of ILA model
  // "start_gb_reduce" -> "gb_layer_reduce_timestep_level" -> "gb_layer_reduce_vector_level"

  instr.SetUpdate(m.state(GB_LAYER_REDUCE_ITERATIONS), iterations);
  instr.SetUpdate(m.state(GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET), memory_min_addr_offset);
  instr.SetUpdate(m.state(GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE), block_size); 
  instr.SetUpdate(m.state(GB_LAYER_REDUCE_TIMESTEP_SIZE), timestep_size);   
  instr.SetUpdate(m.state(GB_LAYER_REDUCE_TIME_STEP_OP_CNTR), 
                    BvConst(0, GB_LAYER_REDUCE_TIME_STEP_OP_CNTR_WIDTH));


  AddChild_gb_lr_ts(m);
}

// TODO: complete the time_step layer loop
void AddChild_gb_lr_ts(Ila& m){

    auto child_ts = m.NewChild("GB_LAYER_REDUCE_TIMESTEP_LEVEL");

    auto counter = m.state(GB_LAYER_REDUCE_TIME_STEP_OP_CNTR); //16
    
    child_ts.SetValid(counter < m.state(GB_LAYER_REDUCE_ITERATIONS));
    child_ts.SetFetch(BvConst(0x1, 1));

    // internal states for this child
    child_ts.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0,
                        GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0_WIDTH);
    child_ts.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1,
                        GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1_WIDTH);
    child_ts.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT,
                        GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT_WIDTH);

    child_ts.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR,
                    GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR_WIDTH);
    
    // states
    auto ts_base_addr_0 = child_ts.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0); //20
    auto ts_base_addr_1 = child_ts.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1); //20
    auto ts_base_addr_result = child_ts.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT); //20

    auto start_addr = m.state(GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET); //20
    auto block_size = m.state(GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE); //20
    auto timestep_size = m.state(GB_LAYER_REDUCE_TIMESTEP_SIZE); //16

    // only 1 instructions, doing time_step level layer reduction,
    // calculating timestep base address for sub_instructions
    {
        auto instr = child_ts.NewInstr("gb_layer_reduce_timestep_level_op");

        instr.SetUpdate(counter, counter + 1);
        // assumption: block size is of even multiple size of timestep_size
        auto counter_20 = Concat(BvConst(0,4), counter);
        auto timestep_size_20 = Concat(BvConst(0,4), timestep_size);

        auto base_addr_0 = start_addr + URem((counter_20*2*timestep_size_20), block_size);
        auto base_addr_1 = start_addr + URem(((counter_20*2+1)*timestep_size_20), block_size);
        auto base_addr_result = start_addr + URem((counter_20*timestep_size_20), block_size);

        // states update for sub-operations
        instr.SetUpdate(ts_base_addr_0, base_addr_0);
        instr.SetUpdate(ts_base_addr_1, base_addr_1);
        instr.SetUpdate(ts_base_addr_result, base_addr_result);

        instr.SetUpdate(child_ts.state(GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR), 
                                        BvConst(0, GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR_WIDTH));
        
        // TODO: implement the child instruction for the inner loop
        AddChild_gb_lr_v(m, child_ts);
    }
}

void AddChild_gb_lr_v(Ila& m, Ila& child_ts) {

    auto child_v = child_ts.NewChild("GB_LAYER_REDUCE_VECTOR_LEVEL");

    auto counter = child_ts.state(GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR); //16
    auto timestep_size = m.state(GB_LAYER_REDUCE_TIMESTEP_SIZE); //16

    child_v.SetValid(counter < timestep_size);
    child_v.SetFetch(BvConst(0x1, 1));

    // states
    child_v.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0,
                        GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0_WIDTH);
    child_v.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1,
                        GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1_WIDTH);
    child_v.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT,
                        GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT_WIDTH);

    auto ts_base_addr_0 = child_ts.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0); //20
    auto ts_base_addr_1 = child_ts.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1); //20
    auto ts_base_addr_result = child_ts.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT); //20

    auto v_addr_0 = child_v.state(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0); //20
    auto v_addr_1 = child_v.state(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1); //20
    auto v_addr_result = child_v.state(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT); //20
    auto op_mode = m.state(GB_LAYER_REDUCE_CONFIG_REG_MODE);

    auto mem = m.state(GB_CORE_LARGE_BUFFER);

    {
        auto instr = child_ts.NewInstr("gb_layer_reduce_vector_level_op");

        instr.SetUpdate(counter, counter+1);

        auto addr_0 = Concat(BvConst(0, 12), Ite((counter == 0), ts_base_addr_0, v_addr_0));
        auto addr_1 = Concat(BvConst(0, 12), Ite((counter == 0), ts_base_addr_1, v_addr_1));
        auto addr_result = Concat(BvConst(0, 12), Ite(counter == 0, ts_base_addr_result, v_addr_result));

        auto data_0 = Load(mem, addr_0);
        auto data_1 = Load(mem, addr_1);

        auto result = Ite((op_mode == GB_LAYER_REDUCE_OP_MAX),
                        Ite((data_0 > data_1), data_0, data_1),
                            Ite((op_mode == GB_LAYER_REDUCE_OP_MEAN),
                                (data_0 + data_1) / BvConst(2, TOP_DATA_IN_WIDTH), 
                                    data_0 + data_1)); 
        

        instr.SetUpdate(mem, Store(mem, addr_result, result));
        instr.SetUpdate(v_addr_0, Ite(counter == 0, ts_base_addr_0 + 1, v_addr_0 + 1));
        instr.SetUpdate(v_addr_1, Ite(counter == 0, ts_base_addr_1 + 1, v_addr_1 + 1));
        instr.SetUpdate(v_addr_result, Ite(counter == 0, ts_base_addr_result + 1, 
                                                         v_addr_result + 1));
        
    }

    
}

}; // namespace ilang
