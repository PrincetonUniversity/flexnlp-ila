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

// File: gb_layer_reduce_fsm.cc

// This file is a variant of the original gb_layer_reduce file
// merging all the child instructions under only one child model, and using FSM
// states instead of flag variables as decode conditions

#include <flex/flex.h>
#include <flex/uninterpreted_func.h>

namespace ilang {

namespace flex {

void AddChild_LayerReduce(Ila& m);

void DefineStartGBLayerReduce(Ila& m) {
  auto instr = m.NewInstr("GB_LAYER_REDUCE_START");
  // decode condition
  auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
  auto start_addr = TOP_GB_ADDR_MIN + START_ADDR_GB_LAYER_REDUCE;
  auto is_valid = m.state(GB_LAYER_REDUCE_CONFIG_REG_IS_VALID);

  instr.SetDecode(is_write & is_valid & (m.input(TOP_ADDR_IN) == start_addr));

  auto flag_child = m.state(GB_LAYER_REDUCE_CHILD_VALID_FLAG);
  auto child_state = m.state(GB_LAYER_REDUCE_CHILD_STATE);

  instr.SetUpdate(flag_child,
                  BvConst(GB_LAYER_REDUCE_VALID,
                          GB_LAYER_REDUCE_CHILD_VALID_FLAG_BITWIDTH));
  instr.SetUpdate(child_state, BvConst(GB_LAYER_REDUCE_CHILD_STATE_PREP,
                                       GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH));

  AddChild_LayerReduce(m);
}

void AddChild_LayerReduce(Ila& m) {
  auto child = m.NewChild("GB_LAYER_REDUCE");
  auto child_valid =
      (m.state(GB_LAYER_REDUCE_CHILD_VALID_FLAG) == GB_LAYER_REDUCE_VALID);

  child.SetValid(child_valid);

  // parent states
  auto state = m.state(GB_LAYER_REDUCE_CHILD_STATE);

  auto memory_index = m.state(GB_LAYER_REDUCE_CONFIG_REG_MEMORY_INDEX_1);
  auto num_vector = m.state(GB_LAYER_REDUCE_CONFIG_REG_NUM_VECTOR_1);
  auto num_timestep = m.state(GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1);

  auto op_mode = m.state(GB_LAYER_REDUCE_CONFIG_REG_MODE);

  // Parameters form the GB_CORE_MEM_MNGR_LARGE
  auto memory_min_addr_offset = Ite(
      (memory_index == 0),
      Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0),
             BvConst(0, 4)),
      Ite((memory_index == 1),
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1),
                 BvConst(0, 4)),
          Ite((memory_index == 2),
              Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2),
                     BvConst(0, 4)),
              Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3),
                     BvConst(0, 4)))));

  // uncertain about the max addr offset for some memory managing instructions
  auto memory_max_addr_offset = Ite(
      (memory_index == 0),
      Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1),
             BvConst(0, 4)) -
          16,
      Ite((memory_index == 1),
          Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2),
                 BvConst(0, 4)) -
              16,
          Ite((memory_index == 2),
              Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3),
                     BvConst(0, 4)) -
                  16,
              BvConst(GB_CORE_STORE_LARGE_SIZE,
                      GB_CORE_STORE_LARGE_BITWIDTH))));

  // the size of the targeted memory block
  // for now, I assume memory manager will not skip memory index, if
  // max_addr_offset == 0, assume the max addr for the block is the largest
  // address.
  auto block_size =
      Ite((memory_max_addr_offset == 0),
          BvConst(GB_CORE_STORE_LARGE_SIZE, GB_CORE_STORE_LARGE_BITWIDTH) -
              memory_min_addr_offset,
          memory_max_addr_offset - memory_min_addr_offset);

  // child states
  // cntr & iteraions
  auto cntr_timestep = child.NewBvState(GB_LAYER_REDUCE_TIMESTEP_CNTR,
                                        GB_LAYER_REDUCE_TIMESTEP_CNTR_BITWIDTH);
  auto cntr_vector = child.NewBvState(GB_LAYER_REDUCE_VECTOR_CNTR,
                                      GB_LAYER_REDUCE_VECTOR_CNTR_BITWIDTH);
  // addresss related state variables
  auto base_addr_ts_0 =
      child.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0,
                       GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0_WIDTH);
  auto base_addr_ts_1 =
      child.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1,
                       GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1_WIDTH);
  auto base_addr_ts_out =
      child.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT,
                       GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT_WIDTH);
  auto base_addr_v_0 =
      child.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0,
                       GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0_WIDTH);
  auto base_addr_v_1 =
      child.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1,
                       GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1_WIDTH);
  auto base_addr_v_out =
      child.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT,
                       GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT_WIDTH);

  { // instruction 0 ---- initiate all the parameters
    auto instr = child.NewInstr("gb_layer_reduce_prep");
    auto state_prep = (state == GB_LAYER_REDUCE_CHILD_STATE_PREP);

    instr.SetDecode(child_valid & state_prep);

    instr.SetUpdate(cntr_timestep,
                    BvConst(0, GB_LAYER_REDUCE_TIMESTEP_CNTR_BITWIDTH));

    auto next_state = BvConst(GB_LAYER_REDUCE_CHILD_STATE_TIMESTEP_OP,
                              GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 1 ---- setting up timestep level parameters
    auto instr = child.NewInstr("gb_layer_reduce_timestep_level_op");
    auto state_ts = (state == GB_LAYER_REDUCE_CHILD_STATE_TIMESTEP_OP);

    instr.SetDecode(child_valid & state_ts);

    auto num_vector_20 =
        Concat(BvConst(0, 20 - num_vector.bit_width()), num_vector);

    auto cntr_timestep_20 =
        Concat(BvConst(0, 20 - cntr_timestep.bit_width()), cntr_timestep);
    auto ts_index_0 = cntr_timestep_20;
    auto ts_index_1 = cntr_timestep_20 + 1;
    auto ts_index_out = cntr_timestep_20 >> 1;

    auto base_addr_offset_0 = GetGBLargeBaseAddr(ts_index_0, num_vector_20);
    auto base_addr_offset_1 = GetGBLargeBaseAddr(ts_index_1, num_vector_20);
    auto base_addr_offset_out = GetGBLargeBaseAddr(ts_index_out, num_vector_20);

    auto base_addr_ts_0_next = base_addr_offset_0 + memory_min_addr_offset;
    auto base_addr_ts_1_next = base_addr_offset_1 + memory_min_addr_offset;
    auto base_addr_ts_out_next = base_addr_offset_out + memory_min_addr_offset;

    // state updates
    instr.SetUpdate(base_addr_ts_0, base_addr_ts_0_next);
    instr.SetUpdate(base_addr_ts_1, base_addr_ts_1_next);
    instr.SetUpdate(base_addr_ts_out, base_addr_ts_out_next);

    instr.SetUpdate(cntr_timestep, cntr_timestep + 2);
    instr.SetUpdate(cntr_vector,
                    BvConst(0, GB_LAYER_REDUCE_VECTOR_CNTR_BITWIDTH));

    // next state
    auto next_state = BvConst(GB_LAYER_REDUCE_CHILD_STATE_VECTOR_OP,
                              GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 2 ---- setting up vector level parameters
    auto instr = child.NewInstr("gb_layer_reduce_vector_level_op");
    auto state_v = (state == GB_LAYER_REDUCE_CHILD_STATE_VECTOR_OP);

    instr.SetDecode(child_valid & state_v);

    auto cntr_vector_20 =
        Concat(BvConst(0, 20 - cntr_vector.bit_width()), cntr_vector);
    auto v_addr_offset = cntr_vector_20 * GROUPING_SCALAR * GB_CORE_SCALAR;

    auto v_addr_0 = base_addr_ts_0 + v_addr_offset;
    auto v_addr_1 = base_addr_ts_1 + v_addr_offset;
    auto v_addr_out = base_addr_ts_out + v_addr_offset;

    instr.SetUpdate(cntr_vector, cntr_vector + 1);
    instr.SetUpdate(base_addr_v_0, v_addr_0);
    instr.SetUpdate(base_addr_v_1, v_addr_1);
    instr.SetUpdate(base_addr_v_out, v_addr_out);

    // next state
    auto next_state = BvConst(GB_LAYER_REDUCE_CHILD_STATE_VECTOR_REDUCE,
                              GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 3 --- maxpooling operation for the given vector
    auto instr = child.NewInstr("gb_layer_reduce_vector_max");
    auto state_v = (state == GB_LAYER_REDUCE_CHILD_STATE_VECTOR_REDUCE &
                    op_mode == GB_LAYER_REDUCE_OP_MAX);
    instr.SetDecode(child_valid & state_v);

    auto mem = m.state(GB_CORE_LARGE_BUFFER);
    auto mem_next = mem;

    // perform reduction on the vector level
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
        auto addr_0 = Concat(BvConst(0, 32 - base_addr_v_0.bit_width()), base_addr_v_0) + 
                        BvConst(i, 32);
        auto addr_1 = Concat(BvConst(0, 32 - base_addr_v_1.bit_width()), base_addr_v_1) + 
                        BvConst(i, 32);
        auto addr_out = Concat(BvConst(0, 32 - base_addr_v_out.bit_width()), base_addr_v_out) +
                            BvConst(i, 32);
        // reduce the value and store back into the large buffer
        auto result = GBAdpfloat_max(Load(mem, addr_0), Load(mem, addr_1));
        mem_next = Store(mem_next, addr_out, result);
    }

    instr.SetUpdate(mem, mem_next);
    
    auto timestep_done = (cntr_timestep > num_timestep - 2);
    auto vector_done = (cntr_vector > num_vector - 1);
    // next state logic
    auto next_state = 
        Ite(timestep_done & vector_done,
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_DONE, GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH),
        Ite(vector_done,
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_TIMESTEP_OP, GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH),
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_VECTOR_OP, GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH)));
    instr.SetUpdate(state, next_state);
  }

  { // instruction 4 --- meanpooling operation for the given vector
    auto instr = child.NewInstr("gb_layer_reduce_vector_mean");
    auto state_v = (state == GB_LAYER_REDUCE_CHILD_STATE_VECTOR_REDUCE &
                    op_mode == GB_LAYER_REDUCE_OP_MEAN);
    instr.SetDecode(child_valid & state_v);

    auto mem = m.state(GB_CORE_LARGE_BUFFER);
    auto mem_next = mem;

    // perform reduction on the vector level
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
        auto addr_0 = Concat(BvConst(0, 32 - base_addr_v_0.bit_width()), base_addr_v_0) + 
                        BvConst(i, 32);
        auto addr_1 = Concat(BvConst(0, 32 - base_addr_v_1.bit_width()), base_addr_v_1) + 
                        BvConst(i, 32);
        auto addr_out = Concat(BvConst(0, 32 - base_addr_v_out.bit_width()), base_addr_v_out) +
                            BvConst(i, 32);
        // reduce the value and store back into the large buffer
        auto result = GBAdpfloat_mean(Load(mem, addr_0), Load(mem, addr_1));
        mem_next = Store(mem_next, addr_out, result);
    }

    instr.SetUpdate(mem, mem_next);
    
    auto timestep_done = (cntr_timestep > num_timestep - 2);
    auto vector_done = (cntr_vector > num_vector - 1);
    // next state logic
    auto next_state = 
        Ite(timestep_done & vector_done,
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_DONE, GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH),
        Ite(vector_done,
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_TIMESTEP_OP, GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH),
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_VECTOR_OP, GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH)));
    instr.SetUpdate(state, next_state);
  }

  { // instruction 5 --- add-pooling operation for the given vector
    auto instr = child.NewInstr("gb_layer_reduce_vector_add");
    auto state_v = (state == GB_LAYER_REDUCE_CHILD_STATE_VECTOR_REDUCE &
                    op_mode == GB_LAYER_REDUCE_OP_ADD);
    instr.SetDecode(child_valid & state_v);

    auto mem = m.state(GB_CORE_LARGE_BUFFER);
    auto mem_next = mem;

    // perform reduction on the vector level
    for (auto i = 0; i < GB_CORE_SCALAR; i++) {
        auto addr_0 = Concat(BvConst(0, 32 - base_addr_v_0.bit_width()), base_addr_v_0) + 
                        BvConst(i, 32);
        auto addr_1 = Concat(BvConst(0, 32 - base_addr_v_1.bit_width()), base_addr_v_1) + 
                        BvConst(i, 32);
        auto addr_out = Concat(BvConst(0, 32 - base_addr_v_out.bit_width()), base_addr_v_out) +
                            BvConst(i, 32);
        // reduce the value and store back into the large buffer
        auto result = GBAdpfloat_add(Load(mem, addr_0), Load(mem, addr_1));
        mem_next = Store(mem_next, addr_out, result);
    }

    instr.SetUpdate(mem, mem_next);
    
    auto timestep_done = (cntr_timestep > num_timestep - 2);
    auto vector_done = (cntr_vector > num_vector - 1);
    // next state logic
    auto next_state = 
        Ite(timestep_done & vector_done,
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_DONE, GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH),
        Ite(vector_done,
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_TIMESTEP_OP, GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH),
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_VECTOR_OP, GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH)));
    instr.SetUpdate(state, next_state);
  }

  { // instruction 4 ---- set child done
    auto instr = child.NewInstr("gb_layer_reduce_done");
    auto state_done = (state == GB_LAYER_REDUCE_CHILD_STATE_DONE);

    instr.SetDecode(child_valid & state_done);

    instr.SetUpdate(m.state(GB_LAYER_REDUCE_CHILD_VALID_FLAG),
                    BvConst(GB_LAYER_REDUCE_INVALID,
                            GB_LAYER_REDUCE_CHILD_VALID_FLAG_BITWIDTH));
    // add modeling of the interrupt signal
    instr.SetUpdate(m.state(TOP_IRQ), BvConst(1, TOP_IRQ_BITWIDTH));
  }
}

} // namespace flex

} // namespace ilang
