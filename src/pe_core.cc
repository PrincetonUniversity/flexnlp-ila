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

// File: pe_core.cc

#include <flex/flex.h>

namespace ilang {

void AddChild_PECore(Ila& m, const int& pe_idx, const uint64_t& base);

void DefinePECore(Ila& m, const int& pe_idx, const uint64_t& base) {

// PE Core instructions are not visible at the top interface, we need to use child model to implement PE core
// Thus this DefinePECore function will only contain the declaration of the child model of PE Core
  AddChild_PECore(m, pe_idx, base);
}

void AddChild_PECore(Ila& m, const int& pe_idx, const uint64_t& base) {
  auto child = m.NewChild(PEGetChildName(pe_idx, "CORE_CHILD"));
  //currently set the child valid function as always true.
  child.SetValid(BoolConst(true));
  
  // declare new child states
  auto mngr_cntr = m.NewBvState(PEGetVarName(pe_idx, CORE_MNGR_CNTR), PE_CORE_MNGR_CNTR_BITWIDTH);
  // common states used in this child instructions
  auto state = m.state(PEGetVarName(pe_idx, CORE_STATE));

  // add initial condition
  child.AddInit(mngr_cntr == PE_CORE_MNGR_0);
  
  
  { // instructions 0 ---- read the data from GB
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "CORE_READ_GB_0"));
    
    auto pe_config_valid_bit = m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_VALID));
    auto pe_config_is_valid = (pe_config_valid_bit == PE_CORE_VALID);
    auto pe_not_start = (m.state(PE_START_SIGNAL_SHARED) == PE_CORE_INVALID);
    auto gb_data_valid = (m.state(GB_CONTROL_DATA_OUT_VALID) == PE_CORE_VALID);

    auto cntr_valid = (m.state(PE_CNTR) == pe_idx);
    auto state_idle = (state == PE_CORE_STATE_IDLE);

    instr.SetDecode(pe_config_is_valid & pe_not_start & gb_data_valid & cntr_valid & state_idle);

    auto m_index = m.state(GB_CONTROL_DATA_OUT_INDEX);
    auto base_addr = Ite((m_index == PE_CORE_MNGR_0),
                            m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_INPUT)),
                            m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_INPUT)));
    auto data_in_addr = m.state(GB_CONTROL_DATA_OUT_ADDR);
    auto data_in_addr_16 = Concat(BvConst(0, 8), data_in_addr);
    auto input_wr_addr = base_addr + data_in_addr_16; // this address is vector level (16 byte);
    input_wr_addr = (Concat(BvConst(0, 16), input_wr_addr)) << 4; // change the address to byte

    // fetch the data from GB and store them into the input buffer
    auto mem = m.state(PEGetVarName(pe_idx, CORE_INPUT_BUFFER));

    auto mem_next = Store(mem, input_wr_addr + 0, m.state(GB_CONTROL_DATA_OUT_0));
    mem_next = Store(mem_next, input_wr_addr + 1, m.state(GB_CONTROL_DATA_OUT_1));
    mem_next = Store(mem_next, input_wr_addr + 2, m.state(GB_CONTROL_DATA_OUT_2));
    mem_next = Store(mem_next, input_wr_addr + 3, m.state(GB_CONTROL_DATA_OUT_3));
    mem_next = Store(mem_next, input_wr_addr + 4, m.state(GB_CONTROL_DATA_OUT_4));
    mem_next = Store(mem_next, input_wr_addr + 5, m.state(GB_CONTROL_DATA_OUT_5));
    mem_next = Store(mem_next, input_wr_addr + 6, m.state(GB_CONTROL_DATA_OUT_6));
    mem_next = Store(mem_next, input_wr_addr + 7, m.state(GB_CONTROL_DATA_OUT_7));
    mem_next = Store(mem_next, input_wr_addr + 8, m.state(GB_CONTROL_DATA_OUT_8));
    mem_next = Store(mem_next, input_wr_addr + 9, m.state(GB_CONTROL_DATA_OUT_9));
    mem_next = Store(mem_next, input_wr_addr + 10, m.state(GB_CONTROL_DATA_OUT_10));
    mem_next = Store(mem_next, input_wr_addr + 11, m.state(GB_CONTROL_DATA_OUT_11));
    mem_next = Store(mem_next, input_wr_addr + 12, m.state(GB_CONTROL_DATA_OUT_12));
    mem_next = Store(mem_next, input_wr_addr + 13, m.state(GB_CONTROL_DATA_OUT_13));
    mem_next = Store(mem_next, input_wr_addr + 14, m.state(GB_CONTROL_DATA_OUT_14));
    mem_next = Store(mem_next, input_wr_addr + 15, m.state(GB_CONTROL_DATA_OUT_15));
    
    instr.SetUpdate(mem, mem_next);

    // update the control parameters
    auto all_pe_cond = BoolConst(pe_idx >= 3);
    auto pe_cntr_next = Ite(all_pe_cond, BvConst(0, PE_CNTR_BIWTDTH),
                                         BvConst(pe_idx + 1, PE_CNTR_BIWTDTH));

    instr.SetUpdate(m.state(PE_CNTR), pe_cntr_next);    
    instr.SetUpdate(m.state(GB_CONTROL_DATA_OUT_VALID), 
                      BvConst(PE_CORE_INVALID, GB_CONTROL_DATA_OUT_VALID_BITWIDTH));
    
  }

  { // instruction 2 ---- helper instructions for is_start condition
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "CORE_IS_START_PREP"));

    auto pe_config_valid_bit = m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_VALID));
    auto pe_config_is_valid = (pe_config_valid_bit == PE_CORE_VALID);
    auto pe_start_valid = (m.state(GB_CONTROL_CHILD_STATE_PE_START) == PE_CORE_VALID);

    auto is_start = pe_config_is_valid & pe_start_valid;

    instr.SetDecode(is_start);

    auto next_state = BvConst(PE_CORE_STATE_PRE, PE_CORE_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }
}


}; // namespace ilang
