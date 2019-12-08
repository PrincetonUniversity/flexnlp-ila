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

void DefineStartGBLayerReduce(Ila& m) {
  // TODO
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
  auto memory_min_addr_offset = Ite((memory_index == 0), m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0),
                                Ite((memory_index == 1), m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1),
                                Ite((memory_index == 2), m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2),
                                                         m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3))));

  auto memory_max_addr_offset = Ite((memory_index == 0), m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1) - 16,
                                Ite((memory_index == 1), m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2) - 16,
                                Ite((memory_index == 2), m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3) - 16,
                                                         BvConst(0xFFFF0, 20))));

  // Parameter preprocessing, translating parameters used in ILA model
  
  // how many iterations should the layer reduce performs in pairs
  auto pair_num = Ite((SelectBit(num_timestep, 0) == 0), num_timestep/2, (num_timestep - 1)/2);
  auto timestep_size = num_vector * 16; 
  auto block_size = memory_max_addr_offset - memory_min_addr_offset; // the size of the targeted memory block
  auto is_overflow = Ite((timestep_size * num_timestep > block_size), 1, 0); // check whether the data has exceeded the block size
  auto iterations = Ite((is_overflow == 1), block_size/(2*timestep_size), pair_num);

  // layer reduce operations
  // memory layout in ILA, fixed 128bit length for a vector, modeled using 8 bit, 16 entries per vector.
  // **************************
  // **   entry 0, 8bit wide **
  // **************************
  // **   entry 1, 8bit      **
  // **************************
  //          ... ...

  // operations, 3 layers of loops
  // 1. loop of iterations: block level
  // 2. loop of vector_entries: vector level
  
  // ILA entry number = 2*iterations + timestep(0 or 1)*num_vector + vector_num*byte_num
  auto mem = m.state(GB_CORE_LARGE_BUFFER);

  for (auto i = 0; i < iterations; i = i+1) {
    auto entry_base_0 = i*2*timestep_size; // byte level address for loading data
    auto entry_base_1 = (i*2+1)*timestep_size;
    auto store_base = i*2*timestep_size; // byte level address for storing the pooling result

    for (auto j = 0; j < timestep_size; j = j+1) {
      auto store_addr = store_base + j;
      auto data_0_addr = entry_base_0 + j;
      auto data_1_addr = entry_base_1 + j;

      auto data_0 = Load(mem, data_0_addr);
      auto data_1 = Load(mem, data_1_addr);
      
      auto result = Ite((mode == OP_MAX), Ite((data_0 > data_1), data_0, data_1), 
                    Ite((mode == OP_MEAN), (data_0 + data_1)/2,
                                            data_0 + data_1));

      Store(mem, store_addr, result);                                            
    }
  }

}

}; // namespace ilang
