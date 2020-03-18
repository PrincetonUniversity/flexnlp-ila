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

// File: gb_control.cc

#include <flex/flex.h>

namespace ilang {

void AddChild_GB_Control(Ila& m);

void DefineStartGBControl(Ila& m) {

  auto instr = m.NewInstr("GB_CONTROL_START");
  // Decode condition
  auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
  auto start_addr = TOP_GB_ADDR_MIN + START_ADDR_GB_CONTROL;
  auto is_valid = m.state(GB_CONTROL_CONFIG_REG_IS_VALID);

  instr.SetDecode(is_write & is_valid & (m.input(TOP_ADDR_IN) == start_addr));
  
  auto flag_child = m.state(GB_CONTROL_CHILD_VALID_FLAG);
  auto child_state = m.state(GB_CONTROL_CHILD_STATE);

  instr.SetUpdate(flag_child, 
                    BvConst(GB_CONTROL_CHILD_ON, GB_CONTROL_CHILD_VALID_FLAG_BITWIDTH));
  instr.SetUpdate(child_state,
                    BvConst(GB_CONTROL_CHILD_STATE_PREP, GB_CONTROL_CHILD_STATE_BITWIDTH));

  ILA_INFO << "before calling child";
  AddChild_GB_Control(m);

}

void AddChild_GB_Control(Ila& m) {
  auto child = m.NewChild("GB_Control_child");
  auto flag_child = m.state(GB_CONTROL_CHILD_VALID_FLAG);
  auto child_valid = (flag_child == GB_CONTROL_CHILD_ON);

  child.SetValid(child_valid);
  
  // child level states
  auto cntr_timestep = child.NewBvState(GB_CONTROL_CHILD_TIMESTEP_CNTR,
                                          GB_CONTROL_CHILD_TIMESTEP_CNTR_BITWIDTH);  
  auto cntr_vector = child.NewBvState(GB_CONTROL_CHILD_VECTOR_CNTR,
                                        GB_CONTROL_CHILD_VECTOR_CNTR_BITWIDTH);
  auto timestep_index = child.NewBvState(GB_CONTROL_CHILD_TIMESTEP_INDEX,
                                            GB_CONTROL_CHILD_TIMESTEP_INDEX_BITWIDTH);
  auto timestep_base_addr = child.NewBvState(GB_CONTROL_CHILD_TIMESTEP_BASE_ADDR,
                                              GB_CONTROL_CHILD_TIMESTEP_BASE_ADDR_BITWIDTH);

  // parent config states
  auto mode = m.state(GB_CONTROL_CONFIG_REG_MODE);
  auto is_rnn = m.state(GB_CONTROL_CONFIG_REG_IS_RNN);
  auto mem_index_1 = m.state(GB_CONTROL_CONFIG_REG_MEMORY_INDEX_1);
  auto mem_index_2 = m.state(GB_CONTROL_CONFIG_REG_MEMORY_INDEX_2);
  auto num_vector_1 = m.state(GB_CONTROL_CONFIG_REG_NUM_VECTOR_1);
  auto num_vector_2 = m.state(GB_CONTROL_CONFIG_REG_NUM_VECTOR_2);
  auto num_timestep = m.state(GB_CONTROL_CONFIG_REG_NUM_TIMESTEP_1);
  // parent shared state
  auto pe_start = m.state(PE_START_SIGNAL_SHARED);
  auto pe_done = m.state(PE_DONE_SIGNAL_SHARED);

  auto data_in_0 = m.state(GB_CONTROL_DATA_IN_0);
  auto data_in_1 = m.state(GB_CONTROL_DATA_IN_1);
  auto data_in_2 = m.state(GB_CONTROL_DATA_IN_2);
  auto data_in_3 = m.state(GB_CONTROL_DATA_IN_3);
  auto data_in_4 = m.state(GB_CONTROL_DATA_IN_4);
  auto data_in_5 = m.state(GB_CONTROL_DATA_IN_5);
  auto data_in_6 = m.state(GB_CONTROL_DATA_IN_6);
  auto data_in_7 = m.state(GB_CONTROL_DATA_IN_7);
  auto data_in_8 = m.state(GB_CONTROL_DATA_IN_8);
  auto data_in_9 = m.state(GB_CONTROL_DATA_IN_9);
  auto data_in_10 = m.state(GB_CONTROL_DATA_IN_10);
  auto data_in_11 = m.state(GB_CONTROL_DATA_IN_11);
  auto data_in_12 = m.state(GB_CONTROL_DATA_IN_12);
  auto data_in_13 = m.state(GB_CONTROL_DATA_IN_13);
  auto data_in_14 = m.state(GB_CONTROL_DATA_IN_14);
  auto data_in_15 = m.state(GB_CONTROL_DATA_IN_15);

  auto data_in_addr = m.state(GB_CONTROL_DATA_IN_ADDR);

  auto data_out_0 = m.state(GB_CONTROL_DATA_OUT_0);
  auto data_out_1 = m.state(GB_CONTROL_DATA_OUT_1);
  auto data_out_2 = m.state(GB_CONTROL_DATA_OUT_2);
  auto data_out_3 = m.state(GB_CONTROL_DATA_OUT_3);
  auto data_out_4 = m.state(GB_CONTROL_DATA_OUT_4);
  auto data_out_5 = m.state(GB_CONTROL_DATA_OUT_5);
  auto data_out_6 = m.state(GB_CONTROL_DATA_OUT_6);
  auto data_out_7 = m.state(GB_CONTROL_DATA_OUT_7);
  auto data_out_8 = m.state(GB_CONTROL_DATA_OUT_8);
  auto data_out_9 = m.state(GB_CONTROL_DATA_OUT_9);
  auto data_out_10 = m.state(GB_CONTROL_DATA_OUT_10);
  auto data_out_11 = m.state(GB_CONTROL_DATA_OUT_11);
  auto data_out_12 = m.state(GB_CONTROL_DATA_OUT_12);
  auto data_out_13 = m.state(GB_CONTROL_DATA_OUT_13);
  auto data_out_14 = m.state(GB_CONTROL_DATA_OUT_14);
  auto data_out_15 = m.state(GB_CONTROL_DATA_OUT_15);

  auto data_out_addr = m.state(GB_CONTROL_DATA_OUT_ADDR);
  auto data_in_valid_bit = m.state(GB_CONTROL_DATA_IN_VALID);
  auto data_out_valid_bit = m.state(GB_CONTROL_DATA_OUT_VALID);

  // parent child control state
  auto state = m.state(GB_CONTROL_CHILD_STATE);

  // parent memory states
  auto mem_large = m.state(GB_CORE_LARGE_BUFFER);
  auto mem_small = m.state(GB_CORE_SMALL_BUFFER);

  // memory addresses
  // large buffer
  auto memory_base_addr_1 =
          Ite((mem_index_1 == 0),
              Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0), BvConst(0, 4)),
              Ite((mem_index_1 == 1),
                  Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1), BvConst(0, 4)),
                  Ite((mem_index_1 == 2),
                      Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2), BvConst(0, 4)),
                      Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3), BvConst(0, 4)))));
  
  auto memory_base_addr_2 =
          Ite((mem_index_2 == 0),
              Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_0), BvConst(0, 4)),
              Ite((mem_index_2 == 1),
                  Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_1), BvConst(0, 4)),
                  Ite((mem_index_2 == 2),
                      Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_2), BvConst(0, 4)),
                      Concat(m.state(GB_CORE_MEM_MNGR_LARGE_CONFIG_REG_BASE_LARGE_3), BvConst(0, 4)))));
  // small buffer
  auto mem_base_addr_small = 
        Ite((mem_index_1 == 0),
          Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_0), BvConst(0, 4)),
          Ite((mem_index_1 == 1),
            Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_1), BvConst(0, 4)),
            Ite((mem_index_1 == 2),
              Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_2), BvConst(0, 4)),
              Ite((mem_index_1 == 3),
                Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_3), BvConst(0, 4)),
                Ite((mem_index_1 == 4),
                  Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_4), BvConst(0, 4)),
                  Ite((mem_index_1 == 5),
                    Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_5), BvConst(0, 4)),
                    Ite((mem_index_1 == 6),
                    Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_6), BvConst(0, 4)),
                    Concat(m.state(GB_CORE_MEM_MNGR_SMALL_CONFIG_REG_BASE_SMALL_7), BvConst(0, 4)))))))));

  auto g_scalar = BvConst(GB_CORE_SCALAR, GB_CONTROL_CONFIG_REG_NUM_TIMESTEP_1_WIDTH);

  {// child instruction 0 ---- initialize the parameters for gb control child instruction
    auto instr = child.NewInstr("gb_control_child_prep");
    auto state_prep = (state == GB_CONTROL_CHILD_STATE_PREP);
    
    instr.SetDecode(child_valid & state_prep);

    auto next_state = BvConst(GB_CONTROL_CHILD_STATE_SEND_PREP, GB_CONTROL_CHILD_STATE_BITWIDTH);

    // reset the counter value
    instr.SetUpdate(cntr_timestep, BvConst(0, GB_CONTROL_CHILD_TIMESTEP_CNTR_BITWIDTH));
    instr.SetUpdate(cntr_vector, BvConst(0, GB_CONTROL_CHILD_VECTOR_CNTR_BITWIDTH));
    // reset the valid bits for the shared states with PE
    instr.SetUpdate(data_in_valid_bit, BvConst(GB_CONTROL_INVALID, GB_CONTROL_DATA_IN_VALID_BITWIDTH));
    instr.SetUpdate(data_out_valid_bit, BvConst(GB_CONTROL_INVALID, GB_CONTROL_DATA_OUT_VALID_BITWIDTH));
    instr.SetUpdate(pe_start, BvConst(GB_CONTROL_INVALID, PE_START_SIGNAL_SHARED_BITWIDTH));

    instr.SetUpdate(state, next_state);
  }

  {// child instruction 1 ---- set the parameters for sending timestep
    auto instr = child.NewInstr("gb_control_child_send_prep");
    auto state_send_prep = (state == GB_CONTROL_CHILD_STATE_SEND_PREP);

    instr.SetDecode(child_valid & state_send_prep);

    auto timestep_index_tmp = Ite(mode == GB_CONTROL_MODE_UNI, cntr_timestep,
                                Ite(mode == GB_CONTROL_MODE_BI_FWD, cntr_timestep * 2, 
                                  Ite(mode == GB_CONTROL_MODE_BI_BWD, (num_timestep - cntr_timestep) * 2 + 1,
                                    Ite(mode == GB_CONTROL_MODE_DECODE, 
                                        BvConst(0, GB_CONTROL_CHILD_TIMESTEP_CNTR_BITWIDTH),
                                          BvConst(0, GB_CONTROL_CHILD_TIMESTEP_CNTR_BITWIDTH)))));

    auto cntr_vector_tmp = BvConst(0, GB_CONTROL_CHILD_VECTOR_CNTR_BITWIDTH);
    
    // calculating the starting address of the target timestep
    auto num_vector_20 = Concat(BvConst(0, 12), num_vector_1);
    auto timestep_size = num_vector_20 * GB_CORE_SCALAR;
    auto group_size = timestep_size * GB_CORE_LARGE_NUM_BANKS;

    auto group_index = timestep_index_tmp / g_scalar;
    auto group_offset = URem(timestep_index_tmp, g_scalar);

    auto group_index_20 = Concat(BvConst(0, 4), group_index);
    auto group_offset_20 = Concat(BvConst(0, 4), group_offset);

    auto timestep_base_addr_offset = group_index_20 * group_size + group_offset_20;
    auto timestep_base_addr_tmp = memory_base_addr_1 + timestep_base_addr_offset;

    auto next_state = BvConst(GB_CONTROL_CHILD_STATE_SEND, GB_CONTROL_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
    instr.SetUpdate(timestep_index, timestep_index_tmp);
    instr.SetUpdate(cntr_vector, cntr_vector_tmp);
    instr.SetUpdate(cntr_timestep, cntr_timestep + 1);
    instr.SetUpdate(timestep_base_addr, timestep_base_addr_offset);
  }

  {// instruction 2 ---- sending the current vector into the data out

    auto instr = child.NewInstr("gb_control_child_send");
    auto state_send = (state == GB_CONTROL_CHILD_STATE_SEND);
    auto data_out_invalid = (data_out_valid_bit == GB_CONTROL_INVALID);

    instr.SetDecode(child_valid & state_send & data_out_invalid);

    // memory address for reading from large buffer
    auto row_size = GB_CORE_SCALAR * GB_CORE_LARGE_NUM_BANKS;
    auto cntr_vector_20 = Concat(BvConst(0, 12), cntr_vector);
    auto vector_addr_offset = cntr_vector_20 * row_size;
    auto addr_large = timestep_base_addr + vector_addr_offset;
    auto addr_large_32 = Concat(BvConst(0, 32 - GB_CORE_STORE_LARGE_BITWIDTH), addr_large);

    // memory address for reading from small buffer
    auto vector_addr_offset_small = cntr_vector_20 * GB_CORE_SCALAR;
    auto addr_small = mem_base_addr_small + vector_addr_offset_small;
    auto addr_small_32 = Concat(BvConst(0, 32 - GB_CORE_STORE_LARGE_BITWIDTH), addr_small);


    auto data_out_0_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 0),
                                        Load(mem_small, addr_small_32 + 0));
    auto data_out_1_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 1),
                                        Load(mem_small, addr_small_32 + 1));
    auto data_out_2_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 2),
                                        Load(mem_small, addr_small_32 + 2));
    auto data_out_3_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 3),
                                        Load(mem_small, addr_small_32 + 3));
    auto data_out_4_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 4),
                                        Load(mem_small, addr_small_32 + 4));
    auto data_out_5_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 5),
                                        Load(mem_small, addr_small_32 + 5));
    auto data_out_6_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 6),
                                        Load(mem_small, addr_small_32 + 6));
    auto data_out_7_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 7),
                                        Load(mem_small, addr_small_32 + 7));                                    
    auto data_out_8_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 8),
                                        Load(mem_small, addr_small_32 + 8));
    auto data_out_9_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 9),
                                        Load(mem_small, addr_small_32 + 9));
    auto data_out_10_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 10),
                                        Load(mem_small, addr_small_32 + 10));
    auto data_out_11_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 11),
                                        Load(mem_small, addr_small_32 + 11));
    auto data_out_12_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 12),
                                        Load(mem_small, addr_small_32 + 12));
    auto data_out_13_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 13),
                                        Load(mem_small, addr_small_32 + 13));
    auto data_out_14_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 14),
                                        Load(mem_small, addr_small_32 + 14));
    auto data_out_15_tmp = Ite(mode < 3, Load(mem_large, addr_large_32 + 15),
                                        Load(mem_small, addr_small_32 + 15));
    
    auto cntr_vector_tmp = cntr_vector + 1;
    auto next_state = Ite(cntr_vector_tmp == num_vector_1,
                            BvConst(GB_CONTROL_CHILD_STATE_PE_START, GB_CONTROL_CHILD_STATE_BITWIDTH),
                            BvConst(GB_CONTROL_CHILD_STATE_SEND, GB_CONTROL_CHILD_STATE_BITWIDTH));
    
    instr.SetUpdate(state, next_state);
    instr.SetUpdate(cntr_vector, cntr_vector_tmp);
    // send data and address to the output port
    instr.SetUpdate(data_out_0, data_out_0_tmp);
    instr.SetUpdate(data_out_1, data_out_1_tmp);
    instr.SetUpdate(data_out_2, data_out_2_tmp);
    instr.SetUpdate(data_out_3, data_out_3_tmp);
    instr.SetUpdate(data_out_4, data_out_4_tmp);
    instr.SetUpdate(data_out_5, data_out_5_tmp);
    instr.SetUpdate(data_out_6, data_out_6_tmp);
    instr.SetUpdate(data_out_7, data_out_7_tmp);
    instr.SetUpdate(data_out_8, data_out_8_tmp);
    instr.SetUpdate(data_out_9, data_out_9_tmp);
    instr.SetUpdate(data_out_10, data_out_10_tmp);
    instr.SetUpdate(data_out_11, data_out_11_tmp);
    instr.SetUpdate(data_out_12, data_out_12_tmp);
    instr.SetUpdate(data_out_13, data_out_13_tmp);
    instr.SetUpdate(data_out_14, data_out_14_tmp);
    instr.SetUpdate(data_out_15, data_out_15_tmp);

    instr.SetUpdate(data_out_addr, cntr_vector);

  }

  { // instruction 3 ---- finishing sending the whole timestep, sending pe_start
    auto instr = child.NewInstr("gb_control_pe_start");
    auto state_pe_start = (state == GB_CONTROL_CHILD_STATE_PE_START);
    
    instr.SetDecode(child_valid & state_pe_start);

    auto next_state = BvConst(GB_CONTROL_CHILD_STATE_RECV_PREP, GB_CONTROL_CHILD_STATE_BITWIDTH);
    
    instr.SetUpdate(pe_start, BvConst(GB_CONTROL_VALID, PE_START_SIGNAL_SHARED_BITWIDTH));
    instr.SetUpdate(state, next_state);
  }


}

}; // namespace ilang
