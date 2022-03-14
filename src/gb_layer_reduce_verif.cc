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

  // Parameters form the GB_CORE_MEM_MNGR_LARGE
  auto memory_min_addr_offset = ZExt(
    Ite(memory_index == 0,
      m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0),
      Ite(memory_index == 1,
        m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1),
        Ite(memory_index == 2,
          m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2),
          m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3))
    )), TOP_ADDR_IN_WIDTH) * GB_CORE_SCALAR;

  // child states
  // cntr & iteraions
  auto ctr_ts_grp = child.NewBvState("gb_layer_reduce_tgrp_ctr", TOP_ADDR_IN_WIDTH);
  auto ctr_ts = child.NewBvState("gb_layer_reduce_ts_ctr", TOP_ADDR_IN_WIDTH);
  auto ctr_hs_vec = child.NewBvState("gb_layer_reduce_hvec_ctr", TOP_ADDR_IN_WIDTH);
  auto ctr_hs = child.NewBvState("gb_layer_reduce_hs_ctr", TOP_ADDR_IN_WIDTH);

  auto ts_max = child.NewBvState("gb_layer_reduce_ts_max", TOP_ADDR_IN_WIDTH);
  auto hs_vec_max = child.NewBvState("gb_layer_reduce_hs_vec_max", TOP_ADDR_IN_WIDTH);

  // addresss related state variables
  auto base_addr_ts_0 =
      child.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0,
                       TOP_ADDR_IN_WIDTH);
  auto base_addr_ts_1 =
      child.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1,
                       TOP_ADDR_IN_WIDTH);
  auto base_addr_ts_out =
      child.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT,
                       TOP_ADDR_IN_WIDTH);
  auto base_addr_v_0 =
      child.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0,
                       TOP_ADDR_IN_WIDTH);
  auto base_addr_v_1 =
      child.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1,
                       TOP_ADDR_IN_WIDTH);
  auto base_addr_v_out =
      child.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT,
                       TOP_ADDR_IN_WIDTH);

  auto ZERO = BvConst(0, TOP_ADDR_IN_WIDTH);
  auto ONE = BvConst(1, TOP_ADDR_IN_WIDTH);

  { // instruction 0 ---- initiate all the parameters
    auto instr = child.NewInstr("gb_layer_reduce_prep");
    auto state_prep = (state == GB_LAYER_REDUCE_CHILD_STATE_PREP);

    instr.SetDecode(child_valid & state_prep);

    instr.SetUpdate(ts_max, ZExt(num_timestep, TOP_ADDR_IN_WIDTH));
    instr.SetUpdate(hs_vec_max, ZExt(num_vector, TOP_ADDR_IN_WIDTH));

    instr.SetUpdate(ctr_ts_grp, ZERO);
    instr.SetUpdate(ctr_ts, ZERO);

    auto next_state = BvConst(GB_LAYER_REDUCE_CHILD_STATE_TIMESTEP_OP,
                              GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 1 ---- setting up timestep level parameters
    auto instr = child.NewInstr("gb_layer_reduce_timestep_level_op");
    auto state_ts = (state == GB_LAYER_REDUCE_CHILD_STATE_TIMESTEP_OP);

    instr.SetDecode(child_valid & state_ts);

    auto timestep_size = hs_vec_max * GB_CORE_SCALAR;
    auto group_size = timestep_size * GB_CORE_LARGE_NUM_BANKS;

    auto carry = Ite(ctr_ts * 2 < GB_CORE_LARGE_NUM_BANKS, 
                     ZERO, ONE);
    auto rem = ctr_ts * 2 - carry * GB_CORE_LARGE_NUM_BANKS;

    auto base_addr_offset_0 =
        (ctr_ts_grp * 2 + carry) * group_size + rem * GB_CORE_SCALAR;
    auto base_addr_offset_1 =
        (ctr_ts_grp * 2 + carry) * group_size + (rem + 1) * GB_CORE_SCALAR;
    auto base_addr_offset_out =
        ctr_ts_grp * group_size + ctr_ts * GB_CORE_SCALAR;

    auto base_addr_ts_0_next = base_addr_offset_0 + memory_min_addr_offset;
    auto base_addr_ts_1_next = base_addr_offset_1 + memory_min_addr_offset;
    auto base_addr_ts_out_next = base_addr_offset_out + memory_min_addr_offset;

    // state updates
    instr.SetUpdate(base_addr_ts_0, base_addr_ts_0_next);
    instr.SetUpdate(base_addr_ts_1, base_addr_ts_1_next);
    instr.SetUpdate(base_addr_ts_out, base_addr_ts_out_next);

    auto ts_end = (ctr_ts == GB_CORE_LARGE_NUM_BANKS - 1);
    instr.SetUpdate(ctr_ts, Ite(ts_end, ZERO, ctr_ts + 1));
    instr.SetUpdate(ctr_ts_grp, Ite(ts_end, ctr_ts_grp + 1, ctr_ts_grp));
    instr.SetUpdate(ctr_hs_vec, ZERO);

    // next state
    auto next_state = BvConst(GB_LAYER_REDUCE_CHILD_STATE_VECTOR_OP,
                              GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 2 ---- setting up vector level parameters
    auto instr = child.NewInstr("gb_layer_reduce_vector_level_op");
    auto state_v = (state == GB_LAYER_REDUCE_CHILD_STATE_VECTOR_OP);

    instr.SetDecode(child_valid & state_v);

    auto v_addr_offset = ctr_hs_vec * GB_CORE_LARGE_NUM_BANKS * GB_CORE_SCALAR;

    auto v_addr_0 = base_addr_ts_0 + v_addr_offset;
    auto v_addr_1 = base_addr_ts_1 + v_addr_offset;
    auto v_addr_out = base_addr_ts_out + v_addr_offset;

    instr.SetUpdate(ctr_hs_vec, ctr_hs_vec + 1);
    instr.SetUpdate(base_addr_v_0, v_addr_0);
    instr.SetUpdate(base_addr_v_1, v_addr_1);
    instr.SetUpdate(base_addr_v_out, v_addr_out);

    instr.SetUpdate(ctr_hs, ZERO);

    // next state
    auto next_state = BvConst(GB_LAYER_REDUCE_CHILD_STATE_BYTE_OP,
                              GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 3 ---- byte level reduce operation
    // [Par2Seq]: FlexNLP perform pooling for the whole vector (16 bytes)
    auto instr = child.NewInstr("gb_layer_reduce_byte_level_op");
    auto state_byte = (state == GB_LAYER_REDUCE_CHILD_STATE_BYTE_OP);

    instr.SetDecode(child_valid & state_byte);

    auto addr_0 = base_addr_v_0 + ctr_hs;
    auto addr_1 = base_addr_v_1 + ctr_hs;
    auto addr_out = base_addr_v_out + ctr_hs;

    auto mem = m.state(GB_CORE_LARGE_BUFFER);
    auto op_mode = m.state(GB_LAYER_REDUCE_CONFIG_REG_MODE);

    auto data_0 = Load(mem, addr_0);
    auto data_1 = Load(mem, addr_1);

    // TODO: add pooling is not correct!
    // update 05272020: use uninterpreted functions to implement the algorithms
    auto result = Ite(
        (op_mode == GB_LAYER_REDUCE_OP_MAX), GBAdpfloat_max(data_0, data_1),
        Ite((op_mode == GB_LAYER_REDUCE_OP_MEAN),
            GBAdpfloat_mean(data_0, data_1), GBAdpfloat_add(data_0, data_1)));

    instr.SetUpdate(mem, Store(mem, addr_out, result));
    instr.SetUpdate(ctr_hs, ctr_hs + 1);

    auto timesteps_done = (
      ctr_ts_grp * 2 * GB_CORE_LARGE_NUM_BANKS + ctr_ts * 2 > ts_max - 2);
    auto cur_ts_done = (ctr_hs_vec > hs_vec_max - 1);
    auto vec_done = (ctr_hs >= (GB_CORE_SCALAR - 1));

    auto next_state =
        Ite(timesteps_done & cur_ts_done & vec_done,
            BvConst(GB_LAYER_REDUCE_CHILD_STATE_DONE,
                    GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH),
            Ite(cur_ts_done & vec_done,
                BvConst(GB_LAYER_REDUCE_CHILD_STATE_TIMESTEP_OP,
                        GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH),
                Ite(vec_done,
                    BvConst(GB_LAYER_REDUCE_CHILD_STATE_VECTOR_OP,
                            GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH),
                    BvConst(GB_LAYER_REDUCE_CHILD_STATE_BYTE_OP,
                            GB_LAYER_REDUCE_CHILD_STATE_BITWIDTH))));

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
