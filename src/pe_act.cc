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

// File: pe_act.cc

#include <flex/flex.h>
#include <flex/util.h>

namespace ilang {
// declare a child ILA to organize the child instructions of PE Activation unit.
void AddChildPEAct(Ila& m, const int& pe_idx, const uint64_t& base);

// helper functions

void DefinePEAct(Ila& m, const int& pe_idx, const uint64_t& base) {
  // TODO
  AddChildPEAct(m, pe_idx, base);
}

void AddChildPEAct(Ila& m, const int& pe_idx, const uint64_t& base) {
  auto child = m.NewChild(PEGetChildName(pe_idx, "ACT_CHILD"));
  auto config_valid = 
        (m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_VALID)) == PE_ACT_VALID);
  child.SetValid(config_valid);

  // child states
  auto is_start_reg = 
        child.NewBvState(PEGetVarName(pe_idx, ACT_IS_START_REG), PE_ACT_IS_START_REG_BITWIDTH);
  auto is_init_reg = 
        child.NewBvState(PEGetVarName(pe_idx, ACT_IS_INIT_REG), PE_ACT_IS_INIT_REG_BITWIDTH);
  auto instr_cntr = 
        child.NewBvState(PEGetVarName(pe_idx, ACT_INSTR_COUNTER), PE_ACT_INSTR_COUNTER_BITWIDTH);
  auto output_cntr = 
        child.NewBvState(PEGetVarName(pe_idx, ACT_OUTPUT_COUNTER), PE_ACT_OUTPUT_COUNTER_BITWIDTH);

  auto state = 
        child.NewBvState(PEGetVarName(pe_idx, ACT_STATE), PE_ACT_STATE_BITWIDTH);
  
  auto a1 = child.NewBvState(PEGetVarName(pe_idx, ACT_REG_A1), PE_ACT_REG_IDX_BITWIDTH);
  auto a2 = child.NewBvState(PEGetVarName(pe_idx, ACT_REG_A2), PE_ACT_REG_IDX_BITWIDTH);
  auto op = child.NewBvState(PEGetVarName(pe_idx, ACT_OP), PE_ACT_OP_BITWIDTH);
  
  auto w_out = child.NewBvState(PEGetVarName(pe_idx, ACT_W_OUT), PE_ACT_FLAG_BITWIDTH);
  auto w_load = child.NewBvState(PEGetVarName(pe_idx, ACT_W_LOAD), PE_ACT_FLAG_BITWIDTH);
  auto w_done = child.NewBvState(PEGetVarName(pe_idx, ACT_W_DONE), PE_ACT_FLAG_BITWIDTH);
  auto is_incr = child.NewBvState(PEGetVarName(pe_idx, ACT_IS_INCR), PE_ACT_FLAG_BITWIDTH);

  // act register 20-bit scaler * 16, 4 set
  // naming example: PE0_ACT_REGS_0_1: mean the 2nd entry in of the vector register 0 in PE0
  for (auto i = 0; i < ACT_REGS_NUM; i++) {
    for (auto j = 0; j < ACT_SCALAR; j++) {
      child.NewBvState(PEGetActRegName(pe_idx, i, j, ACT_REGS), PE_CORE_ACT_VECTOR_BITWIDTH);
    }
  }
  
  // parent states
  auto pe_start = m.state(PE_START_SIGNAL_SHARED);

  // child initial conditions
  child.AddInit(is_start_reg == PE_ACT_INVALID);
  child.AddInit(is_init_reg == PE_ACT_INVALID);

  { // instr0 ---- initiate the act unit when receiving pe_start, reset states
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_initiate"));
    // auto not_init_cond = (is_init_reg == PE_ACT_INVALID);
    auto not_start_cond = (is_start_reg == PE_ACT_INVALID);
    auto pe_start_active = (pe_start == PE_ACT_VALID);
    
    instr.SetDecode(config_valid & not_start_cond & pe_start_active);

    // state updates
    instr.SetUpdate(w_out, BvConst(PE_ACT_INVALID, PE_ACT_FLAG_BITWIDTH));
    instr.SetUpdate(w_load, BvConst(PE_ACT_INVALID, PE_ACT_FLAG_BITWIDTH));
    instr.SetUpdate(w_done, BvConst(PE_ACT_INVALID, PE_ACT_FLAG_BITWIDTH));
    instr.SetUpdate(is_incr, BvConst(PE_ACT_VALID, PE_ACT_FLAG_BITWIDTH));

    instr.SetUpdate(is_start_reg, BvConst(PE_ACT_VALID, PE_ACT_IS_START_REG_BITWIDTH));

    // reset counter
    instr.SetUpdate(instr_cntr, BvConst(0, PE_ACT_INSTR_COUNTER_BITWIDTH));
    instr.SetUpdate(output_cntr, BvConst(0, PE_ACT_OUTPUT_COUNTER_BITWIDTH));

    // reset the act vector register
    for (auto i = 0; i < ACT_REGS_NUM; i++) {
      for (auto j = 0; j < ACT_SCALAR; j++) {
        auto entry = child.state(PEGetActRegName(pe_idx, i, j, ACT_REGS));
        instr.SetUpdate(entry, BvConst(0, PE_CORE_ACT_VECTOR_BITWIDTH));
      }
    }

    // set the FSM state to fetch
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_FETCH, PE_ACT_STATE_BITWIDTH));
  }  

  { // instr1 ---- run instruction in the act unit
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_run_instr"));
    // TODO: Think about how to separate the instructions decode conditions
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_fetch = (state == PE_ACT_STATE_FETCH);
    
    instr.SetDecode(is_start & state_fetch);

    // fetch instruction from the instruction register
    auto current_instr = PEActInstrFetch(m, pe_idx, instr_cntr);
    auto a1_next = Extract(current_instr, PE_ACT_REG_A1_HI_IDX, PE_ACT_REG_A1_LO_IDX);
    auto a2_next = Extract(current_instr, PE_ACT_REG_A2_HI_IDX, PE_ACT_REG_A2_LO_IDX);
    auto op_next = Extract(current_instr, PE_ACT_OP_HI_IDX, PE_ACT_OP_LO_IDX);

    // update opcode and registers
    instr.SetUpdate(a1, a1_next);
    instr.SetUpdate(a2, a2_next);
    instr.SetUpdate(op, op_next);

    // update FSM state
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_EXEC, PE_ACT_STATE_BITWIDTH));
  }
}





}; // namespace ilang
