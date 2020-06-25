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
#include <flex/uninterpreted_func.h>

namespace ilang {

namespace flex {

void AddChild_Turnoff_Flag(Ila& m);

void AddChild_Group_Level(Ila& m);
void AddChild_Timestep_Level(Ila& m);
void AddChild_Vector_Level(Ila& m);
void AddChild_Byte_Level(Ila& m);

void DefineStartGBLayerReduce(Ila& m) {
  auto instr = m.NewInstr("GB_LAYER_REDUCE_START");
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

  // Parameter preprocessing, translating parameters used in ILA model
  // how many iterations should the layer reduce performs in pairs
  auto pair_num =
      Ite((SelectBit(num_timestep, 0) == 0),
          num_timestep /
              BvConst(2, GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH),
          (num_timestep - 1) /
              BvConst(2, GB_LAYER_REDUCE_CONFIG_REG_NUM_TIMESTEP_1_WIDTH));

  auto timestep_size = num_vector * 16;

  // The over_flow should be detected elsewhere?
  // the overwrite situation happens when num_of_timestep is larger than given.
  // Thus the additional operations will do the pooling on time_step which are
  // already been pooled.

#if 0
  auto is_overflow =
      Ite((timestep_size * num_timestep > block_size), 1,
          0); // check whether the data has exceeded the block size
#endif

  auto iterations = pair_num;

  // Update: data are organized as grouping in the large buffer.
  auto g_scalar = BvConst(GROUPING_SCALAR, GB_LAYER_REDUCE_GROUPING_NUM_WIDTH);
  // the remainder timestep in the last timestep.
  auto grouping_rem = URem(num_timestep, g_scalar);
  auto grouping_num = Ite((grouping_rem > 0), num_timestep / g_scalar + 1,
                          num_timestep / g_scalar);

  // flag variables
  auto flag_start = m.state(GB_LAYER_REDUCE_START_FLAG);
  auto flag_group = m.state(GB_LAYER_REDUCE_GROUP_LEVEL_FLAG);
  auto flag_timestep = m.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_FLAG);
  auto flag_vector = m.state(GB_LAYER_REDUCE_VECTOR_LEVEL_FLAG);
  auto flag_byte = m.state(GB_LAYER_REDUCE_BYTE_LEVEL_FLAG);

  // memory boundary states
  instr.SetUpdate(m.state(GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET),
                  memory_min_addr_offset);
  instr.SetUpdate(m.state(GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE), block_size);
  // grouping states
  instr.SetUpdate(m.state(GB_LAYER_REDUCE_GROUPING_NUM), grouping_num);
  instr.SetUpdate(m.state(GB_LAYER_REDUCE_GROUPING_REM), grouping_rem);
  instr.SetUpdate(m.state(GB_LAYER_REDUCE_GROUPING_INDEX),
                  BvConst(0, GB_LAYER_REDUCE_GROUPING_INDEX_WIDTH));

  // flag variables updates
  instr.SetUpdate(flag_start, BvConst(ON, FLAG_BITWIDTH));
  instr.SetUpdate(flag_group,
                  BvConst(UNDONE, FLAG_BITWIDTH)); // top child should be undone
  instr.SetUpdate(flag_timestep, BvConst(DONE, FLAG_BITWIDTH));
  instr.SetUpdate(flag_vector, BvConst(DONE, FLAG_BITWIDTH));
  instr.SetUpdate(flag_byte, BvConst(DONE, FLAG_BITWIDTH));

  AddChild_Group_Level(m);
  AddChild_Turnoff_Flag(m);
}

void AddChild_Turnoff_Flag(Ila& m) {
  auto flag_start = m.state(GB_LAYER_REDUCE_START_FLAG);
  auto flag_group = m.state(GB_LAYER_REDUCE_GROUP_LEVEL_FLAG);
  auto flag_timestep = m.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_FLAG);
  auto flag_vector = m.state(GB_LAYER_REDUCE_VECTOR_LEVEL_FLAG);
  auto flag_byte = m.state(GB_LAYER_REDUCE_BYTE_LEVEL_FLAG);

  auto cond = (flag_start == ON) & (flag_group == DONE) &
              (flag_timestep == DONE) & (flag_vector == DONE) &
              (flag_byte == DONE);

  auto child_done = m.NewChild("GBLayerReduce_Done_Flag");

  child_done.SetValid(cond);
  child_done.SetFetch(BvConst(1, 1));

  {
    auto instr = child_done.NewInstr("gb_layer_reduce_done");
    instr.SetDecode(cond);

    instr.SetUpdate(flag_start, BvConst(OFF, GB_LAYER_REDUCE_START_FLAG_WIDTH));
  }
}

// Child model that set grouping level parameters
void AddChild_Group_Level(Ila& m) {
  auto child_group = m.NewChild("GBLayerReduce_Group_Level");
  auto group_index = m.state(GB_LAYER_REDUCE_GROUPING_INDEX);
  auto group_num = m.state(GB_LAYER_REDUCE_GROUPING_NUM);

  // flags
  auto flag_start = m.state(GB_LAYER_REDUCE_START_FLAG);

  auto flag_group = m.state(GB_LAYER_REDUCE_GROUP_LEVEL_FLAG);
  auto flag_timestep = m.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_FLAG);
  auto flag_vector = m.state(GB_LAYER_REDUCE_VECTOR_LEVEL_FLAG);
  auto flag_byte = m.state(GB_LAYER_REDUCE_BYTE_LEVEL_FLAG);

  auto flag_cond = (flag_start == ON) & (flag_group == UNDONE) &
                   (flag_timestep == DONE) & (flag_vector == DONE) &
                   (flag_byte == DONE);

  child_group.SetValid((group_index < group_num) & flag_cond);
  child_group.SetFetch(BvConst(1, 1));

  // grouping level parameters
  auto g_base_addr =
      child_group.NewBvState(GB_LAYER_REDUCE_GROUP_LEVEL_BASE_ADDR,
                             GB_LAYER_REDUCE_GROUP_LEVEL_BASE_ADDR_WIDTH);
  auto g_out_addr =
      child_group.NewBvState(GB_LAYER_REDUCE_GROUP_LEVEL_OUT_ADDR,
                             GB_LAYER_REDUCE_GROUP_LEVEL_OUT_ADDR_WIDTH);
  auto ts_cntr =
      child_group.NewBvState(GB_LAYER_REDUCE_TIMESTEP_LEVEL_CNTR,
                             GB_LAYER_REDUCE_TIMESTEP_LEVEL_CNTR_WIDTH);

  // parent level parameters
  auto num_vector = m.state(GB_LAYER_REDUCE_CONFIG_REG_NUM_VECTOR_1);
  auto block_size = m.state(GB_LAYER_REDUCE_MEMORY_BLOCK_SIZE);
  auto mem_min_addr = m.state(GB_LAYER_REDUCE_MEMORY_MIN_ADDR_OFFSET);

  // here we consider the last grouping, which may not be a factor of 16
  auto group_rem = m.state(GB_LAYER_REDUCE_GROUPING_REM);
  auto ts_num_group =
      child_group.NewBvState(GB_LAYER_REDUCE_GROUP_LEVEL_TS_NUM,
                             GB_LAYER_REDUCE_GROUP_LEVEL_TS_NUM_WIDTH);

  // instruction
  {
    auto instr = child_group.NewInstr("gb_layer_reduce_group_level_op");

    instr.SetDecode((group_index < group_num) & flag_cond);

    auto g_scalar =
        BvConst(GROUPING_SCALAR, GB_LAYER_REDUCE_GROUPING_NUM_WIDTH);
    auto v_scalar = BvConst(GB_CORE_SCALAR, GB_LAYER_REDUCE_GROUPING_NUM_WIDTH);
    auto num_vector_16 = Concat(BvConst(0, 8), num_vector);
    auto group_size = g_scalar * num_vector_16 * v_scalar;

    auto base_addr_offset = group_size * group_index;                   // 16
    auto base_addr_offset_20 = Concat(BvConst(0, 4), base_addr_offset); // 20

    auto const_2 = BvConst(2, GB_LAYER_REDUCE_GROUPING_INDEX_WIDTH);
    auto half_row_size = (GROUPING_SCALAR / 2) * GB_CORE_SCALAR;
    auto out_addr_offset =
        Ite((URem(group_index, const_2) > 0),
            group_size * (group_index / const_2) + half_row_size,
            group_size * (group_index / const_2));
    auto out_addr_offset_20 = Concat(BvConst(0, 4), out_addr_offset);

    // deal with overflow situation
    auto base_addr_fixed =
        Ite((base_addr_offset_20 < block_size), base_addr_offset_20,
            base_addr_offset_20 - block_size);
    auto base_addr_final = base_addr_fixed + mem_min_addr;

    auto out_addr_fixed =
        Ite((out_addr_offset_20 < block_size), out_addr_offset_20,
            out_addr_offset_20 - block_size);
    auto out_addr_final = out_addr_fixed + mem_min_addr;

    // state updates
    instr.SetUpdate(group_index, group_index + 1);
    instr.SetUpdate(g_base_addr, base_addr_final);
    instr.SetUpdate(g_out_addr, out_addr_final);

    instr.SetUpdate(ts_cntr,
                    BvConst(0, GB_LAYER_REDUCE_TIMESTEP_LEVEL_CNTR_WIDTH));
    // update the number of timesteps remaining in the current group
    instr.SetUpdate(
        ts_num_group,
        Ite((group_index < (group_num - 1)),
            BvConst(GROUPING_SCALAR, GB_LAYER_REDUCE_GROUP_LEVEL_TS_NUM_WIDTH),
            Ite(group_rem == 0,
                BvConst(GROUPING_SCALAR,
                        GB_LAYER_REDUCE_GROUP_LEVEL_TS_NUM_WIDTH),
                Extract(group_rem, 4, 0))));
    // flags update
    instr.SetUpdate(flag_timestep, BvConst(UNDONE, FLAG_BITWIDTH));
    instr.SetUpdate(flag_group, Ite(group_index < (group_num - 1),
                                    BvConst(UNDONE, FLAG_BITWIDTH),
                                    BvConst(DONE, FLAG_BITWIDTH)));

    // Add child to set timestep level parameters in the current group
    AddChild_Timestep_Level(m);
  }
}

void AddChild_Timestep_Level(Ila& m) {
  auto child_group = m.child("GBLayerReduce_Group_Level");
  auto child_timestep = child_group.NewChild("GBLayerReduce_Timestep_Level");

  auto ts_cntr =
      child_group.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_CNTR); // 5-bit
  auto ts_num = child_group.state(GB_LAYER_REDUCE_GROUP_LEVEL_TS_NUM);

  // flags states
  auto flag_start = m.state(GB_LAYER_REDUCE_START_FLAG);

  auto flag_timestep = m.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_FLAG);
  auto flag_vector = m.state(GB_LAYER_REDUCE_VECTOR_LEVEL_FLAG);
  auto flag_byte = m.state(GB_LAYER_REDUCE_BYTE_LEVEL_FLAG);

  auto flag_cond = (flag_start == ON) & (flag_timestep == UNDONE) &
                   (flag_vector == DONE) & (flag_byte == DONE);

  child_timestep.SetValid((ts_cntr < ts_num) & flag_cond);
  child_timestep.SetFetch(BvConst(1, 1));

  // timestep level parameters
  auto vector_cntr =
      child_timestep.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR,
                                GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR_WIDTH);
  auto timestep_base_addr_0 = child_timestep.NewBvState(
      GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0,
      GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0_WIDTH);
  auto timestep_base_addr_1 = child_timestep.NewBvState(
      GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1,
      GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1_WIDTH);
  auto timestep_base_addr_out = child_timestep.NewBvState(
      GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT,
      GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT_WIDTH);
  // paraent states
  auto group_base_addr =
      child_group.state(GB_LAYER_REDUCE_GROUP_LEVEL_BASE_ADDR);
  auto group_out_addr = child_group.state(GB_LAYER_REDUCE_GROUP_LEVEL_OUT_ADDR);

  // instruction
  {
    auto instr = child_timestep.NewInstr("gb_layer_timestep_level_op");

    instr.SetDecode((ts_cntr < ts_num) & flag_cond);

    auto ts_cntr_20 = Concat(BvConst(0, 15), ts_cntr);            // 20
    auto const_2_cntr = BvConst(2, GB_CORE_STORE_LARGE_BITWIDTH); // 20

    auto ts_base_addr_0 = group_base_addr + ts_cntr_20 * GB_CORE_SCALAR;
    auto ts_base_addr_1 = group_base_addr + (ts_cntr_20 + 1) * GB_CORE_SCALAR;
    auto ts_base_addr_out =
        group_out_addr + (ts_cntr_20 / const_2_cntr) * GB_CORE_SCALAR;

    // state upates
    instr.SetUpdate(ts_cntr, ts_cntr + 2);
    instr.SetUpdate(timestep_base_addr_0, ts_base_addr_0);
    instr.SetUpdate(timestep_base_addr_1, ts_base_addr_1);
    instr.SetUpdate(timestep_base_addr_out, ts_base_addr_out);
    instr.SetUpdate(vector_cntr,
                    BvConst(0, GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR_WIDTH));

    // flag updates
    instr.SetUpdate(flag_timestep, Ite((ts_cntr < (ts_num - 2)),
                                       BvConst(UNDONE, FLAG_BITWIDTH),
                                       BvConst(DONE, FLAG_BITWIDTH)));
    instr.SetUpdate(flag_vector, BvConst(UNDONE, FLAG_BITWIDTH));

    // add child to set the vector level parameters
    AddChild_Vector_Level(m);
  }
}

void AddChild_Vector_Level(Ila& m) {
  auto child_group = m.child("GBLayerReduce_Group_Level");
  auto child_timestep = child_group.child("GBLayerReduce_Timestep_Level");
  auto child_vector = child_timestep.NewChild("GBLayerReduce_Vector_Level");

  auto num_vector = m.state(GB_LAYER_REDUCE_CONFIG_REG_NUM_VECTOR_1); // 8
  auto num_vector_16 = Concat(BvConst(0, 8), num_vector);
  auto vector_cntr =
      child_timestep.state(GB_LAYER_REDUCE_VECTOR_LEVEL_OP_CNTR); // 16

  // flag states
  auto flag_start = m.state(GB_LAYER_REDUCE_START_FLAG);
  auto flag_vector = m.state(GB_LAYER_REDUCE_VECTOR_LEVEL_FLAG);
  auto flag_byte = m.state(GB_LAYER_REDUCE_BYTE_LEVEL_FLAG);
  auto flag_cond =
      (flag_vector == UNDONE) & (flag_byte == DONE) & (flag_start == ON);

  child_vector.SetValid((vector_cntr < num_vector_16) & flag_cond);
  child_vector.SetFetch(BvConst(1, 1));

  // vector level parameters
  auto byte_cntr = child_vector.NewBvState(
      GB_LAYER_REDUCE_BYTE_LEVEL_CNTR, GB_LAYER_REDUCE_BYTE_LEVEL_CNTR_WIDTH);
  auto vector_base_0 =
      child_vector.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0,
                              GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0_WIDTH);
  auto vector_base_1 =
      child_vector.NewBvState(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1,
                              GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1_WIDTH);
  auto vector_base_out = child_vector.NewBvState(
      GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT,
      GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT_WIDTH);

  // parent level parameters
  auto timestep_base_addr_0 =
      child_timestep.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_0);
  auto timestep_base_addr_1 =
      child_timestep.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_1);
  auto timestep_base_addr_out =
      child_timestep.state(GB_LAYER_REDUCE_TIMESTEP_LEVEL_BASE_ADDR_RESULT);

  // instruction
  {
    auto instr = child_vector.NewInstr("gb_layer_vector_level_op");

    instr.SetDecode((vector_cntr < num_vector_16) & flag_cond);

    auto v_addr_offset = vector_cntr * GROUPING_SCALAR * GB_CORE_SCALAR; // 16
    auto v_addr_offset_20 = Concat(BvConst(0, 4), v_addr_offset);        // 20

    auto v_addr_0 = timestep_base_addr_0 + v_addr_offset_20;
    auto v_addr_1 = timestep_base_addr_1 + v_addr_offset_20;
    auto v_addr_out = timestep_base_addr_out + v_addr_offset_20;

    instr.SetUpdate(vector_cntr, vector_cntr + 1);
    instr.SetUpdate(vector_base_0, v_addr_0);
    instr.SetUpdate(vector_base_1, v_addr_1);
    instr.SetUpdate(vector_base_out, v_addr_out);
    instr.SetUpdate(byte_cntr,
                    BvConst(0, GB_LAYER_REDUCE_BYTE_LEVEL_CNTR_WIDTH));

    // flag updates
    instr.SetUpdate(flag_vector, Ite((vector_cntr < (num_vector_16 - 1)),
                                     BvConst(UNDONE, FLAG_BITWIDTH),
                                     BvConst(DONE, FLAG_BITWIDTH)));

    instr.SetUpdate(flag_byte, BvConst(UNDONE, FLAG_BITWIDTH));

    // add child to do the pooling
    AddChild_Byte_Level(m);
  }
}

void AddChild_Byte_Level(Ila& m) {
  auto child_group = m.child("GBLayerReduce_Group_Level");
  auto child_timestep = child_group.child("GBLayerReduce_Timestep_Level");
  auto child_vector = child_timestep.child("GBLayerReduce_Vector_Level");
  auto child_byte = child_vector.NewChild("GBLayerReduce_Byte_Level");

  auto byte_cntr = child_vector.state(GB_LAYER_REDUCE_BYTE_LEVEL_CNTR);
  // flag state
  auto flag_byte = m.state(GB_LAYER_REDUCE_BYTE_LEVEL_FLAG);
  auto flag_start = m.state(GB_LAYER_REDUCE_START_FLAG);
  auto flag_cond = (flag_byte == UNDONE) & (flag_start == ON);

  child_byte.SetValid((byte_cntr < GB_CORE_SCALAR) & flag_cond);

  // parent states
  auto vector_base_0 = child_vector.state(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_0);
  auto vector_base_1 = child_vector.state(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_1);
  auto vector_base_out =
      child_vector.state(GB_LAYER_REDUCE_VECTOR_LEVEL_ADDR_RESULT);

  auto mem = m.state(GB_CORE_LARGE_BUFFER);
  auto op_mode = m.state(GB_LAYER_REDUCE_CONFIG_REG_MODE);

  // instruction
  {
    auto instr = child_byte.NewInstr("gb_layer_byte_level_op");
    instr.SetDecode((byte_cntr < GB_CORE_SCALAR) & flag_cond);

    auto byte_cntr_20 = Concat(BvConst(0, 15), byte_cntr); // 20

    auto addr_0 = vector_base_0 + byte_cntr_20;
    auto addr_1 = vector_base_1 + byte_cntr_20;
    auto addr_out = vector_base_out + byte_cntr_20;

    auto addr_0_32 = Concat(BvConst(0, 12), addr_0);
    auto addr_1_32 = Concat(BvConst(0, 12), addr_1);
    auto addr_out_32 = Concat(BvConst(0, 12), addr_out);

    auto data_0 = Load(mem, addr_0_32);
    auto data_1 = Load(mem, addr_1_32);

    // TODO: add pooling is not correct!
    // update 05272020: use uninterpreted functions to implement the algorithms
    auto result = Ite(
        (op_mode == GB_LAYER_REDUCE_OP_MAX), GBAdpfloat_max(data_0, data_1),
        Ite((op_mode == GB_LAYER_REDUCE_OP_MEAN),
            GBAdpfloat_mean(data_0, data_1), GBAdpfloat_add(data_0, data_1)));

    instr.SetUpdate(byte_cntr, byte_cntr + 1);
    instr.SetUpdate(mem, Store(mem, addr_out_32, result));
    // flag update
    instr.SetUpdate(flag_byte, Ite(byte_cntr < (GB_CORE_SCALAR - 1),
                                   BvConst(UNDONE, FLAG_BITWIDTH),
                                   BvConst(DONE, FLAG_BITWIDTH)));
  }
}

} // namespace flex

} // namespace ilang
