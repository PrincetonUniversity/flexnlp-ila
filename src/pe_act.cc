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
#include <vector>

namespace ilang {
// declare a child ILA to organize the child instructions of PE Activation unit.
void AddChildPEAct(Ila& m, const int& pe_idx, const uint64_t& base);

// helper functions
// help declare a vector of ExprRef holding the next state variables of PE registers
std::vector<ExprRef> PEActGetRegNext(Ila& child, const int& pe_idx);
// function to load the data from the PE Act Register
ExprRef PEActRegLoad_v(Ila& child, const int& pe_idx, const ExprRef& reg_idx,
                        const ExprRef& addr);
// function to link store ast
void PEActRegStore_v(std::vector<ExprRef>& reg_next_v, const ExprRef& reg_idx,
                          const ExprRef& addr, const ExprRef& data);
// function to set instruction update for the corresponding memory states
void PEActRegUpdate_v(Ila& child, const int& pe_idx, InstrRef& instr,
                        std::vector<ExprRef>& reg_next_v);


// declare uninterpreted function here;
auto uf_act_in1 = SortRef::BV(PE_CORE_ACT_VECTOR_BITWIDTH);
auto uf_act_in2 = SortRef::BV(PE_CORE_ACT_VECTOR_BITWIDTH);
auto uf_act_out = SortRef::BV(PE_CORE_ACT_VECTOR_BITWIDTH);

auto uf_scalar_type = SortRef::BV(TOP_DATA_IN_WIDTH);
auto uf_act_type = SortRef::BV(PE_CORE_ACT_VECTOR_BITWIDTH);
auto uf_adpbias_type = SortRef::BV(ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS_WIDTH);

FuncRef Adptflow2Fixed("Adptflow2Fixed", uf_act_type, uf_scalar_type, uf_adpbias_type);
FuncRef Fixed2Adptflow("Fixed2Adaptflow", uf_scalar_type, uf_act_type, uf_adpbias_type);

FuncRef PEActEadd("PEActEadd", uf_act_out, uf_act_in1, uf_act_in2);
FuncRef PEActEmul("PEActEmul", uf_act_out, uf_act_in1, uf_act_in2);
FuncRef PEActSigmoid("PEActSigmoid", uf_act_out, uf_act_in1);
FuncRef PEActTanh("PEActTanh", uf_act_out, uf_act_in1);
FuncRef PEActRelu("PEActRelu", uf_act_out, uf_act_in1);
FuncRef PEActOnex("PEActOnex", uf_act_out, uf_act_in1);


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

  // define the register array as memory here
  for (auto i = 0; i < PE_ACT_REGS_NUM; i++) {
    child.NewMemState(PEGetVarNameVector(pe_idx, i, ACT_REGS), 
                      PE_ACT_REGS_ADDR_WIDTH, PE_CORE_ACT_VECTOR_BITWIDTH);
  }
  
  // parent states
  auto pe_start = m.state(PE_START_SIGNAL_SHARED);

  // child initial conditions
  child.AddInit(is_start_reg == PE_ACT_INVALID);
  child.AddInit(is_init_reg == PE_ACT_INVALID);
  child.AddInit(state == PE_ACT_STATE_IDLE);

  { // instr0 ---- initiate the act unit when receiving pe_start, reset states
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_initiate"));
    // auto not_init_cond = (is_init_reg == PE_ACT_INVALID);
    auto not_start_cond = (is_start_reg == PE_ACT_INVALID);
    auto pe_start_active = (pe_start == PE_ACT_VALID);
    auto state_idle = (state == PE_ACT_STATE_IDLE);
    
    instr.SetDecode(config_valid & not_start_cond & pe_start_active & state_idle);

    // state updates
    instr.SetUpdate(w_out, BvConst(PE_ACT_INVALID, PE_ACT_FLAG_BITWIDTH));
    instr.SetUpdate(w_load, BvConst(PE_ACT_INVALID, PE_ACT_FLAG_BITWIDTH));
    instr.SetUpdate(w_done, BvConst(PE_ACT_INVALID, PE_ACT_FLAG_BITWIDTH));
    instr.SetUpdate(is_incr, BvConst(PE_ACT_VALID, PE_ACT_FLAG_BITWIDTH));

    instr.SetUpdate(is_start_reg, BvConst(PE_ACT_VALID, PE_ACT_IS_START_REG_BITWIDTH));

    // reset counter
    instr.SetUpdate(instr_cntr, BvConst(0, PE_ACT_INSTR_COUNTER_BITWIDTH));
    instr.SetUpdate(output_cntr, BvConst(0, PE_ACT_OUTPUT_COUNTER_BITWIDTH));

    // // reset the act vector register
    // reset the act vector memory
    for (auto i = 0; i < PE_ACT_REGS_NUM; i++) {
      auto reg = child.state(PEGetVarNameVector(pe_idx, i, ACT_REGS));
      auto reg_next = reg;
      for (auto j = 0; j < ACT_SCALAR; j++) {
        auto addr = BvConst(j, PE_ACT_REGS_ADDR_WIDTH);
        reg_next = Store(reg_next, addr, BvConst(0, PE_CORE_ACT_VECTOR_BITWIDTH));
      }
      instr.SetUpdate(reg, reg_next);
    }

    // set the FSM state to fetch
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_FETCH, PE_ACT_STATE_BITWIDTH));
  }  

  { // instr1 ---- fetch instruction in the act unit
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_run_instr"));
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

  { // instr 2 ---- incr state, increment the counter
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_do_incr"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_incr = (state == PE_ACT_STATE_INCR);
    
    instr.SetDecode(is_start & state_incr);

    auto num_instr = m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_NUM_INST));
    auto num_output = m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_NUM_OUTPUT));

    auto is_last_instr = (instr_cntr == num_instr - 1);
    auto is_last_output = (output_cntr == num_output - 1);
    auto is_end = is_last_instr & is_last_output;

    auto instr_cntr_next = Ite(is_last_instr, BvConst(0, PE_ACT_INSTR_COUNTER_BITWIDTH),
                                                instr_cntr + 1);
    auto output_cntr_next = Ite(is_end, BvConst(0, PE_ACT_OUTPUT_COUNTER_BITWIDTH),
                                Ite(is_last_instr & !is_last_output, output_cntr + 1, 
                                      output_cntr));
    auto is_zero_next = Ite(is_end, BvConst(0, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST_WIDTH),
                              m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST)));

    instr.SetUpdate(instr_cntr, instr_cntr_next);
    instr.SetUpdate(output_cntr, output_cntr_next);
    instr.SetUpdate(m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST)),
                      is_zero_next);
    
    auto next_state = Ite(is_end, BvConst(PE_ACT_STATE_SEND_DONE, PE_ACT_STATE_BITWIDTH),
                                  BvConst(PE_ACT_STATE_FETCH, PE_ACT_STATE_BITWIDTH));
    auto is_start_reg_next = Ite(is_end, BvConst(PE_ACT_INVALID, PE_ACT_IS_START_REG_BITWIDTH),
                                          BvConst(PE_ACT_VALID, PE_ACT_IS_START_REG_BITWIDTH));
    
    instr.SetUpdate(state, next_state);
    instr.SetUpdate(is_start_reg, is_start_reg_next);
    
  }

  { // instr 3 ---- schedule the pe_done, send pe_done after all the pe are done.
    // use a scheduler here to force sequential access to the shared states
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_send_done"));
    auto state_sd = (state == PE_ACT_STATE_SEND_DONE);
    auto cntr_valid = (m.state(PE_ACT_DONE_CNTR) == pe_idx);

    instr.SetDecode(state_sd & cntr_valid & (is_start_reg == PE_ACT_INVALID));

    auto all_pe_cond = (m.state(PE_ACT_DONE_CNTR) >= 3);
    auto pe_cntr_next = Ite(all_pe_cond, BvConst(0, PE_ACT_DONE_CNTR_BITWIDTH),
                                          m.state(PE_ACT_DONE_CNTR) + 1);

    auto next_state = BvConst(PE_ACT_STATE_IDLE, PE_ACT_STATE_BITWIDTH);

    instr.SetUpdate(state, next_state);
    instr.SetUpdate(m.state(PE_ACT_DONE_CNTR), pe_cntr_next);
    // push pe_done when all of the four PEs are finish
    instr.SetUpdate(m.state(PE_DONE_SIGNAL_SHARED),
                      Ite(all_pe_cond, BvConst(PE_ACT_VALID, PE_DONE_SIGNAL_SHARED_BITWIDTH),
                                        BvConst(PE_ACT_INVALID, PE_DONE_SIGNAL_SHARED_BITWIDTH)));
  }

  { // instr 4 ---- op 0x1, LOAD instruction with is_zero_first == 1
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_load_zero"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_load = (op == PE_ACT_OP_LOAD);
    auto is_zero_first = 
          (m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST)) == PE_ACT_VALID);

    instr.SetDecode(is_start & state_exec & op_load & is_zero_first);

    // next state
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH));

    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data = BvConst(0, PE_CORE_ACT_VECTOR_BITWIDTH);
      PEActRegStore_v(reg_next_v, a2, addr, data);
    }
    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v); 
  }

  { // instr 5 ---- op 0x1, LOAD instruction with is_zero_first == 0
    // LOAD SRAM -> A2 load address is determined by the output_counter + buffer_addr_base
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_load"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_load = (op == PE_ACT_OP_LOAD);
    auto no_zero_first = 
          (m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST)) == PE_ACT_INVALID);
    
    instr.SetDecode(is_start & state_exec & op_load & no_zero_first);

    // next state updates
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    auto buffer_addr_base = m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_BUFFER_ADDR_BASE));
    // set the rd_addr to 32 bit
    auto rd_addr = Concat(BvConst(0, TOP_ADDR_IN_WIDTH - output_cntr.bit_width()), output_cntr)
         + Concat(BvConst(0, TOP_ADDR_IN_WIDTH - buffer_addr_base.bit_width()), buffer_addr_base);
    // act buffer
    auto buffer = m.state(PEGetVarName(pe_idx, ACT_BUFFER));
    auto adptflow_bias = m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS));

    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto data_buf = Load(buffer, rd_addr + i);
      auto data_fixed = Adptflow2Fixed(data_buf, adptflow_bias);
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      PEActRegStore_v(reg_next_v, a2, reg_addr, data_fixed);
    }

    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v);


  }

  { // instr 6 ---- op 0x2, STORE instruction
    // STORE SRAM <- A2 Store address is determined by the output_counter + buffer_addr_base
    auto instr = child.NewInstr(PEGetVarName(pe_idx, "act_child_op_store"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_store = (op == PE_ACT_OP_STORE);
    
    instr.SetDecode(is_start & state_exec & op_store);

    // next FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    auto buffer_addr_base = m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_BUFFER_ADDR_BASE));
    auto wr_addr = Concat(BvConst(0, TOP_ADDR_IN_WIDTH - output_cntr.bit_width()), output_cntr)
         + Concat(BvConst(0, TOP_ADDR_IN_WIDTH - buffer_addr_base.bit_width()), buffer_addr_base);
    
    // get the ExprRef of the register
    auto buffer = m.state(PEGetVarName(pe_idx, ACT_BUFFER));
    auto buffer_next = buffer;
    auto adptflow_bias = m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS));

    // don't need to use the vector method for it only reads from the registers
    for (auto i = 0; i < ACT_SCALAR; i++) {
      // TODO: implement the Fixed2Adptflow function
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data_reg = PEActRegLoad_v(child, pe_idx, a2, reg_addr);
      auto data_adptflow = Fixed2Adptflow(data_reg, adptflow_bias);
      buffer_next = Store(buffer_next, wr_addr + i, data_adptflow);
    }

    instr.SetUpdate(buffer, buffer_next);
  }

  { // instr 7 ---- op 0x3, INPE instruction
    // INPE act_port -> A2 Do not increment instruction if nothing recieved
    auto instr = child.NewInstr(PEGetVarName(pe_idx, "act_child_op_inpe"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_inpe = (op == PE_ACT_OP_INPE);
    // halt the instruction when there is no data pushed to the port
    auto port_valid = (m.state(PEGetVarName(pe_idx, CORE_ACT_REG_PORT_VALID)) == PE_ACT_VALID);

    instr.SetDecode(is_start & state_exec & op_inpe & port_valid);

    // next state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data = m.state(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECOTR));
      PEActRegStore_v(reg_next_v, a2, reg_addr, data);
    }
    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v);
  }
   
  { // instr 8 ---- op 0x4, OUTGB, output result to GB
    // OUTGB A2 -> Output A2 to GB
    // TODO: Need to make sequential access to the shared states
    auto instr = child.NewInstr(PEGetVarName(pe_idx, "act_child_op_outgb"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_outgb = (op == PE_ACT_OP_OUTGB);
    // need to schedule the sequential access 
    // need to check whether gb has popped the previous data
    auto data_out_invalid = (m.state(GB_CONTROL_DATA_IN_VALID) == PE_ACT_INVALID);
    auto cntr_valid = (m.state(PE_ACT_OUTGB_CNTR) == pe_idx);

    instr.SetDecode(is_start & state_exec & op_outgb & data_out_invalid & cntr_valid);

    auto all_pe_cond = (m.state(PE_ACT_OUTGB_CNTR) >= 3);
    auto pe_cntr_next = Ite(all_pe_cond, BvConst(0, PE_ACT_OUTGB_CNTR_BITWIDTH),
                                          m.state(PE_ACT_OUTGB_CNTR) + 1);
    // update pe counter
    instr.SetUpdate(m.state(PE_ACT_OUTGB_CNTR), pe_cntr_next);
    // update FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);
        
    auto output_base_addr = m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_OUTPUT_ADDR_BASE));
    auto data_out_addr = output_cntr + output_base_addr;

    instr.SetUpdate(m.state(GB_CONTROL_DATA_IN_ADDR), data_out_addr);

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto result = PEActRegLoad_v(child, pe_idx, a2, reg_addr);
      instr.SetUpdate(m.state(PEGetGBDataInName(i, GB_CONTROL_DATA_IN)), result);
    }
  }

  { // instr 9 ---- op 0x7, COPY
    // COPY A1 -> A2
    auto instr = child.NewInstr(PEGetVarName(pe_idx, "act_child_op_copy"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_copy = (op == PE_ACT_OP_COPY);

    instr.SetDecode(is_start & state_exec & op_copy);

    // FSM Update
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a1, reg_addr);
      PEActRegStore_v(reg_next_v, a2, reg_addr, data);
    }
    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v);
  }

  { // instr 10 ---- op 0x8, EADD, A2 = A2 + A1
    auto instr = child.NewInstr(PEGetVarName(pe_idx, "act_child_op_eadd"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_eadd = (op == PE_ACT_OP_EADD);

    instr.SetDecode(is_start & state_exec & op_eadd);

    // Next FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data_a1 = PEActRegLoad_v(child, pe_idx, a1, reg_addr);
      auto data_a2 = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActEadd(data_a1, data_a2);
      
      PEActRegStore_v(reg_next_v, a2, reg_addr, result);
    }
    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v);

  }

  { // instr 11 ---- op 0x9, EMUL, A2 = A2 * A1
    auto instr = child.NewInstr(PEGetVarName(pe_idx, "act_child_op_emul"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_emul = (op == PE_ACT_OP_EADD);

    instr.SetDecode(is_start & state_exec & op_emul);
    // Next FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data_a1 = PEActRegLoad_v(child, pe_idx, a1, reg_addr);
      auto data_a2 = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActEmul(data_a1, data_a2);

      PEActRegStore_v(reg_next_v, a2, reg_addr, result);
    }
    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v);
  }

  { // instr 12 ---- op 0xA, SIGM, A2 = Sigmoid(A2)
    auto instr = child.NewInstr(PEGetVarName(pe_idx, "act_child_op_sigm"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_sigm = (op == PE_ACT_OP_SIGM);

    instr.SetDecode(is_start & state_exec & op_sigm);

    // Next FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    // sigmoid is also element-wise
    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActSigmoid(data);

      PEActRegStore_v(reg_next_v, a2, reg_addr, result);
    }
    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v);

  }

  { // instr 13 ---- op 0xB, TANH, A2 = TANH(A2)
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_tanh"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_tanh = (op == PE_ACT_OP_TANH);

    instr.SetDecode(is_start & state_exec & op_tanh);

    // Next FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    // tanh is also element-wise
    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActTanh(data);

      PEActRegStore_v(reg_next_v, a2, reg_addr, result);
    }
    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v);
  }

  { // instr 14 ---- op 0xC, RELU, A2 = RELU(A2)
    auto instr = child.NewInstr(PEGetVarName(pe_idx, "act_child_op_relu"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_relu = (op == PE_ACT_OP_RELU);

    instr.SetDecode(is_start & state_exec & op_relu);

    // Next FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    // Relu is also element wise
    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActRelu(data);

      PEActRegStore_v(reg_next_v, a2, reg_addr, result);
    }
    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v);

  }

  { // instr 15 ---- op 0xD, ONEX, A2 = 1 - A2
    auto instr = child.NewInstr(PEGetVarName(pe_idx, "act_child_op_onex"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_onex = (op == PE_ACT_OP_ONEX);

    instr.SetDecode(is_start & state_exec & op_onex);

    // Next FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    // Onex is also element wise
    auto reg_next_v = PEActGetRegNext(child, pe_idx);
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActOnex(data);

      PEActRegStore_v(reg_next_v, a2, reg_addr, result);
    }
    PEActRegUpdate_v(child, pe_idx, instr, reg_next_v);
  }

}

/****** helper function for PE Act child instructions **********/
// using uninterpreted functions for now
// challenge is how to deal with signed operations

// This function help return the ExprRef pointer to the register memory state
ExprRef PEActGetReg(Ila& child, const int& pe_idx, const ExprRef& reg_idx) {
  auto result = Ite(reg_idx == 0, child.state(PEGetVarNameVector(pe_idx, 0, ACT_REGS)),
                  Ite(reg_idx == 1, child.state(PEGetVarNameVector(pe_idx, 1, ACT_REGS)),
                  Ite(reg_idx == 2, child.state(PEGetVarNameVector(pe_idx, 2, ACT_REGS)),
                                    child.state(PEGetVarNameVector(pe_idx, 3, ACT_REGS)))));
  return result;
}

std::vector<ExprRef> PEActGetRegNext(Ila& child, const int& pe_idx) {
  std::vector<ExprRef> reg_next_v;
  for (auto i = 0; i < PE_ACT_REGS_NUM; i++) {
    auto reg = child.state(PEGetVarNameVector(pe_idx, i, ACT_REGS));
    reg_next_v.push_back(reg);
  }
  return reg_next_v;
}

ExprRef PEActRegLoad_v(Ila& child, const int& pe_idx, const ExprRef& reg_idx,
                        const ExprRef& addr) {
  auto result = BvConst(0, PE_CORE_ACT_VECTOR_BITWIDTH);
  for (auto i = 0; i < PE_ACT_REGS_NUM; i++) {
    auto reg = child.state(PEGetVarNameVector(pe_idx, i, ACT_REGS));
    result = Ite(reg_idx == i, Load(reg, addr), result);
  }
  return result;
}

void PEActRegStore_v(std::vector<ExprRef>& reg_next_v, const ExprRef& reg_idx,
                          const ExprRef& addr, const ExprRef& data) {
  ILA_ASSERT(reg_next_v.size() == PE_ACT_REGS_NUM) 
    << "PEActRegStore: PE_ACT reg vector number is not equal to PE_ACT_REGS_NUM";
  for (auto i = 0; i < reg_next_v.size(); i++) {
    reg_next_v[i] = Ite(reg_idx == i, Store(reg_next_v[i], addr, data), reg_next_v[i]);
  }
}

void PEActRegUpdate_v(Ila& child, const int& pe_idx, InstrRef& instr,
                        std::vector<ExprRef>& reg_next_v) {
  ILA_ASSERT(reg_next_v.size() == PE_ACT_REGS_NUM) 
    << "PEActRegUpdate_v: PE_ACT reg vector number is not equal to PE_ACT_REGS_NUM";
  for (auto i = 0; i < reg_next_v.size(); i++) {
    auto reg = child.state(PEGetVarNameVector(pe_idx, i, ACT_REGS));
    auto reg_next = reg_next_v[i];
    instr.SetUpdate(reg, reg_next);
  }
}

}; // namespace ilang
