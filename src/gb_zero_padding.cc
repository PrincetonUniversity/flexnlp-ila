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

// File: gb_zero_padding.cc

#include <flex/flex.h>

namespace ilang {

namespace flex {

void AddChild_ZeroPadding(Ila& m);

void DefineStartGBZeroPadding(Ila& m) {

  auto instr = m.NewInstr("GB_ZEROPADDING_START");

  // decode conditions
  auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
  auto start_addr = TOP_GB_ADDR_MIN + START_ADDR_GB_ZERO_PADDING;
  auto is_valid = m.state(GB_ZERO_PADDING_CONFIG_REG_IS_VALID);

  instr.SetDecode(is_write & is_valid & (m.input(TOP_ADDR_IN) == start_addr));

  // state updates
  auto flag_child = m.state(GB_ZERO_PADDING_CHILD_VALID_FLAG);
  auto state = m.state(GB_ZERO_PADDING_CHILD_STATE);

  instr.SetUpdate(flag_child,
                  BvConst(GB_ZERO_PADDING_FLAG_ON,
                          GB_ZERO_PADDING_CHILD_VALID_FLAG_BITWIDTH));
  instr.SetUpdate(state, BvConst(GB_ZERO_PADDING_CHILD_STATE_PREP,
                                 GB_ZERO_PADDING_CHILD_STATE_BITWIDTH));

  AddChild_ZeroPadding(m);
}

void AddChild_ZeroPadding(Ila& m) {
  auto child = m.NewChild("Child_GBZeroPadding");
  auto flag_child = m.state(GB_ZERO_PADDING_CHILD_VALID_FLAG);
  auto child_valid = (flag_child == GB_ZERO_PADDING_FLAG_ON);

  child.SetValid(child_valid);

  // new child states
  auto cntr_timestep = child.NewBvState(GB_ZERO_PADDING_TIMESTEP_CNTR,
                                        GB_ZERO_PADDING_TIMESTEP_CNTR_BITWIDTH);
  auto cntr_vector = child.NewBvState(GB_ZERO_PADDING_VECTOR_CNTR,
                                      GB_ZERO_PADDING_VECTOR_CNTR_BITWIDTH);
  auto start_addr = child.NewBvState(GB_ZERO_PADDING_MEM_BASE_ADDR,
                                     GB_ZERO_PADDING_MEM_BASE_ADDR_BITWIDTH);

  // other states
  auto mem = m.state(GB_CORE_LARGE_BUFFER);
  auto state = m.state(GB_ZERO_PADDING_CHILD_STATE);

  auto num_vector = m.state(GB_ZERO_PADDING_CONFIG_REG_NUM_VECTOR_1);
  auto index_timestep_1 = m.state(GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_1);
  auto index_timestep_2 = m.state(GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_2);
  auto num_timestep = index_timestep_2 - index_timestep_1;

  auto g_scalar =
      BvConst(GB_CORE_SCALAR, GB_ZERO_PADDING_CONFIG_REG_NUM_TIMESTEP_1_WIDTH);

  { // instruction prep, set the configuration states for the child instructions
    auto instr = child.NewInstr("gb_zero_padding_child_prep");
    instr.SetDecode(child_valid & (state == GB_ZERO_PADDING_CHILD_STATE_PREP));

    auto next_state = BvConst(GB_ZERO_PADDING_CHILD_STATE_TIMESTEP,
                              GB_ZERO_PADDING_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(cntr_timestep,
                    BvConst(0, GB_ZERO_PADDING_TIMESTEP_CNTR_BITWIDTH));
    instr.SetUpdate(state, next_state);
  }

  { // instruction setting parameters for timestep level op
    auto instr = child.NewInstr("gb_zero_padding_child_timestep");
    instr.SetDecode(child_valid &
                    (state == GB_ZERO_PADDING_CHILD_STATE_TIMESTEP));

    auto current_timestep = index_timestep_1 + cntr_timestep;

    auto memory_index = m.state(GB_ZERO_PADDING_CONFIG_REG_MEMORY_INDEX_1);

    auto memory_base_addr = Ite(
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

    auto num_vector_20 = Concat(BvConst(0, 12), num_vector);
    auto start_addr_offset = GetGBLargeBaseAddr(
      Concat(BvConst(0, 4), current_timestep), num_vector_20) + memory_base_addr;
    auto next_state = BvConst(GB_ZERO_PADDING_CHILD_STATE_VECTOR,
                              GB_ZERO_PADDING_CHILD_STATE_BITWIDTH);

    // updated start address should not include previous value
    instr.SetUpdate(start_addr, start_addr_offset);

    instr.SetUpdate(cntr_vector,
                    BvConst(0, GB_ZERO_PADDING_VECTOR_CNTR_BITWIDTH));
    instr.SetUpdate(cntr_timestep, cntr_timestep + 1);
    instr.SetUpdate(state, next_state);
  }

  { // instruction setting parameters for vector level op
    auto instr = child.NewInstr("gb_zero_padding_child_vector");
    instr.SetDecode(child_valid &
                    (state == GB_ZERO_PADDING_CHILD_STATE_VECTOR));

    auto row_size = GB_CORE_SCALAR * GB_CORE_LARGE_NUM_BANKS;
    auto cntr_vector_20 = Concat(BvConst(0, 12), cntr_vector);
    auto start_addr_offset = cntr_vector_20 * row_size;
    auto next_state = BvConst(GB_ZERO_PADDING_CHILD_STATE_BYTE,
                              GB_ZERO_PADDING_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(start_addr, start_addr + start_addr_offset);
    instr.SetUpdate(cntr_vector, cntr_vector + 1);
    instr.SetUpdate(state, next_state);
  }

  { // instruction do zero padding at the byte
    auto instr = child.NewInstr("gb_zero_padding_child_byte");
    instr.SetDecode(child_valid & (state == GB_ZERO_PADDING_CHILD_STATE_BYTE));
    auto zero_0 = BvConst(0, TOP_DATA_IN_WIDTH);
    auto zero_1 = BvConst(0, TOP_DATA_IN_WIDTH);
    auto zero = zero_0 + zero_1;
    auto addr = Concat(BvConst(0, 12), start_addr);

    auto next_state = BvConst(GB_ZERO_PADDING_CHILD_STATE_NEXT,
                              GB_ZERO_PADDING_CHILD_STATE_BITWIDTH);

    auto mem_tmp = Store(mem, addr, zero);
    for (auto i = 0; i < CORE_SCALAR; i++) {
      mem_tmp = Store(mem_tmp, addr+i, zero);
    }
    instr.SetUpdate(mem, mem_tmp);
    instr.SetUpdate(state, next_state);
  }

  { // instruction set the next step for the state machine
    auto instr = child.NewInstr("gb_zero_padding_child_state_next");
    instr.SetDecode(child_valid & (state == GB_ZERO_PADDING_CHILD_STATE_NEXT));

    auto vector_next = cntr_vector < num_vector;
    auto timestep_next = cntr_timestep < num_timestep;
    auto done = (!vector_next) & (!timestep_next);

    auto next_state = Ite(vector_next,
                          BvConst(GB_ZERO_PADDING_CHILD_STATE_VECTOR,
                                  GB_ZERO_PADDING_CHILD_STATE_BITWIDTH),
                          Ite(timestep_next,
                              BvConst(GB_ZERO_PADDING_CHILD_STATE_TIMESTEP,
                                      GB_ZERO_PADDING_CHILD_STATE_BITWIDTH),
                              BvConst(GB_ZERO_PADDING_CHILD_STATE_DONE,
                                      GB_ZERO_PADDING_CHILD_STATE_BITWIDTH)));
    instr.SetUpdate(state, next_state);
    instr.SetUpdate(flag_child,
                    Ite(done,
                        BvConst(GB_ZERO_PADDING_FLAG_OFF,
                                GB_ZERO_PADDING_CHILD_VALID_FLAG_BITWIDTH),
                        BvConst(GB_ZERO_PADDING_FLAG_ON,
                                GB_ZERO_PADDING_CHILD_VALID_FLAG_BITWIDTH)));
    // add modeling of the interrupt signal
    auto irq_next = 
        Ite(done, BvConst(1, TOP_IRQ_BITWIDTH), BvConst(0, TOP_IRQ_BITWIDTH));
    instr.SetUpdate(m.state(TOP_IRQ), irq_next);  
  }
}

} // namespace flex

} // namespace ilang
