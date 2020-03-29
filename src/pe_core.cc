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
#include <vector>

namespace ilang {

void AddChild_PECore(Ila& m, const int& pe_idx, const uint64_t& base);
// helper functions
void PECoreRunMac(Ila& m, const int& pe_idx,
                          ExprRef& is_cluster,
                          ExprRef& weight_base_v,
                          ExprRef& input_base_v);

void DefinePECore(Ila& m, const int& pe_idx, const uint64_t& base) {

// PE Core instructions are not visible at the top interface, we need to use child model to implement PE core
// Thus this DefinePECore function will only contain the declaration of the child model of PE Core
  AddChild_PECore(m, pe_idx, base);
}

void AddChild_PECore(Ila& m, const int& pe_idx, const uint64_t& base) {
  auto child = m.NewChild(PEGetChildName(pe_idx, "CORE_CHILD"));
  auto pe_config_valid_bit = m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_VALID));
  auto pe_config_is_valid = (pe_config_valid_bit == PE_CORE_VALID);
  // child valid when instructions layer sizing is set valid.
  child.SetValid(pe_config_is_valid);
  
  // declare new child states
  auto mngr_cntr = child.NewBvState(PEGetVarName(pe_idx, CORE_MNGR_CNTR), PE_CORE_MNGR_CNTR_BITWIDTH);
  auto input_cntr = child.NewBvState(PEGetVarName(pe_idx, CORE_INPUT_CNTR), PE_CORE_INPUT_CNTR_BITWIDTH);
  auto output_cntr = child.NewBvState(PEGetVarName(pe_idx, CORE_OUTPUT_CNTR), PE_CORE_OUTPUT_CNTR_BITWIDTH);
  
  // common states used in this child instructions
  auto state = m.state(PEGetVarName(pe_idx, CORE_STATE));

  // core accumulator registers
  for (auto i = 0; i < PE_CORE_ACCUM_VECTOR_LANES; i++) {
    child.NewBvState(PEGetVarNameVector(pe_idx, i, CORE_ACCUM_VECTOR), PE_CORE_ACCUM_VECTOR_BITWIDTH);
  }
  // core activation vector
  for (auto i = 0; i < PE_CORE_ACT_VECTOR_LANES; i++) {
    child.NewBvState(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECOTR), PE_CORE_ACT_VECTOR_BITWIDTH);
  }

  // add initial condition
  child.AddInit(mngr_cntr == PE_CORE_MNGR_0);
  child.AddInit(input_cntr == 0);
  child.AddInit(output_cntr == 0);
  
  
  { // instructions 0 ---- read the data from GB
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "CORE_READ_GB_0"));
  
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
    // TODO: verify the correctness of the input addr !!!!
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
    // updates on the GB_Control: the pe_start valid only after all the PE have read 
    // the last piece of data.
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "CORE_IS_START_PREP"));

    auto pe_start_valid = (m.state(GB_CONTROL_CHILD_STATE_PE_START) == PE_CORE_VALID);
    auto state_idle = (state == PE_CORE_STATE_IDLE);
    auto is_start = pe_config_is_valid & pe_start_valid;
    // only need the pe_start_valid here because the d
    instr.SetDecode(is_start & state_idle);

    auto next_state = BvConst(PE_CORE_STATE_PRE, PE_CORE_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
  }

  { // instruction 3 ---- select next state
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "CORE_STATE_PRE"));

    auto pe_start_valid = (m.state(GB_CONTROL_CHILD_STATE_PE_START) == PE_CORE_VALID);
    auto is_start = pe_config_is_valid & pe_start_valid;
    auto state_pre = (state == PE_CORE_STATE_PRE);

    instr.SetDecode(is_start & state_pre);

    auto is_zero_first = m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_ZERO_FIRST));
    auto zero_active = Ite(mngr_cntr == PE_CORE_MNGR_0, 
                            m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ZERO_ACTIVE)),
                            m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ZERO_ACTIVE)));
    auto zero_first_cond = (is_zero_first == PE_CORE_VALID) & (zero_active == PE_CORE_VALID);
    auto next_state = Ite(zero_first_cond, BvConst(PE_CORE_STATE_BIAS, PE_CORE_STATE_BITWIDTH),
                                            BvConst(PE_CORE_STATE_MAC, PE_CORE_STATE_BITWIDTH));
    
    // states updates
    // reset the accumulate registers
    for (auto i = 0; i < CORE_SCALAR; i++) {
      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, CORE_ACCUM_VECTOR)),
                        BvConst(0, PE_CORE_ACCUM_VECTOR_BITWIDTH));
      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECOTR)),
                        BvConst(0, PE_CORE_ACT_VECTOR_BITWIDTH));
    }
    
    instr.SetUpdate(state, next_state);
  }

  { // instruction 4 ---- MAC state
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "CORE_STATE_MAC"));
    
    auto pe_start_valid = (m.state(GB_CONTROL_CHILD_STATE_PE_START) == PE_CORE_VALID);
    auto is_start = pe_config_is_valid & pe_start_valid;
    auto state_mac = (state == PE_CORE_STATE_MAC);

    instr.SetDecode(is_start & state_mac);

    // state transition control signal
    auto num_input = Ite(mngr_cntr == PE_CORE_MNGR_0,
                          m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_NUM_INPUT)),
                          m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_NUM_INPUT)));
    auto is_input_end = (input_cntr >= num_input - 1);
    auto next_state = Ite(is_input_end, BvConst(PE_CORE_STATE_BIAS, PE_CORE_STATE_BITWIDTH),
                                        BvConst(PE_CORE_STATE_MAC, PE_CORE_STATE_BITWIDTH));

    instr.SetUpdate(state, next_state);

    // calculate the addresses
    auto is_cluster = m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER));

    auto config_base_weight = Ite(mngr_cntr == PE_CORE_MNGR_0,
                                m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_WEIGHT)),
                                m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_WEIGHT)));
    auto config_base_input = Ite(mngr_cntr == PE_CORE_MNGR_0,
                                m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_INPUT)),
                                m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_INPUT)));

    auto weight_base_v = Ite(is_cluster == 1,
                              (output_cntr*num_input + input_cntr)*8 + config_base_weight,
                              (output_cntr*num_input + input_cntr)*16 + config_base_weight);
    auto input_base_v = input_cntr + config_base_input;
    
    PECoreRunMac(m, pe_idx, is_cluster, weight_base_v, input_base_v);
  }
}

void PECoreRunMac(Ila& m, const int& pe_idx,
                          ExprRef& is_cluster,
                          ExprRef& weight_base_v,
                          ExprRef& input_base_v) {
  // TODO: implement the matrix multiplication here.
  auto child = m.child(PEGetChildName(pe_idx, "CORE_CHILD"));
  std::vector<ExprRef> weights_data_byte;
  std::vector<std::vector<ExprRef>> weights_data_vector;
  // current weight vector base addr
  auto weights_addr_current_v = Concat(BvConst(0, TOP_ADDR_IN_WIDTH - weight_base_v.bit_width()),
                                        weight_base_v);
  weights_addr_current_v = weights_addr_current_v << 4;; // changed into byte units

  // auto index_max = Ite(is_cluster, BvConst(8, 16), BvConst(16, 16));

  // for (auto i = BvConst(0, 16); i < index_max; i++) {
  //   weights_addr_current_v = weights_addr_current_v + i*CORE_SCALAR;
    
  // }

  

  
}


}; // namespace ilang
