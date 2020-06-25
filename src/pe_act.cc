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

#include <vector>

#include <flex/flex.h>
#include <flex/uninterpreted_func.h>
#include <flex/util.h>

namespace ilang {

namespace flex {

// declare a child ILA to organize the child instructions of PE Activation unit.
void AddChildPEAct(Ila& m, const int& pe_idx, const uint64_t& base);

// helper functions
// help declare a vector of ExprRef holding the next state variables of PE
// registers std::vector<ExprRef> PEActGetRegNext(Ila& child, const int&
// pe_idx); function to load the data from the PE Act Register
ExprRef PEActRegLoad_v(Ila& child, const int& pe_idx, const ExprRef& reg_idx,
                       const ExprRef& addr);

void DefinePEAct(Ila& m, const int& pe_idx, const uint64_t& base) {
  // TODO
  AddChildPEAct(m, pe_idx, base);
}

void AddChildPEAct(Ila& m, const int& pe_idx, const uint64_t& base) {
  auto child = m.NewChild(PEGetChildName(pe_idx, "ACT_CHILD"));
  auto config_valid =
      (m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_VALID)) ==
       PE_ACT_VALID);
  child.SetValid(config_valid);

  // child states
  auto is_start_reg = child.NewBvState(PEGetVarName(pe_idx, ACT_IS_START_REG),
                                       PE_ACT_IS_START_REG_BITWIDTH);
  auto instr_cntr = child.NewBvState(PEGetVarName(pe_idx, ACT_INSTR_COUNTER),
                                     PE_ACT_INSTR_COUNTER_BITWIDTH);
  auto output_cntr = child.NewBvState(PEGetVarName(pe_idx, ACT_OUTPUT_COUNTER),
                                      PE_ACT_OUTPUT_COUNTER_BITWIDTH);

  auto state =
      child.NewBvState(PEGetVarName(pe_idx, ACT_STATE), PE_ACT_STATE_BITWIDTH);

  auto a1 = child.NewBvState(PEGetVarName(pe_idx, ACT_REG_A1),
                             PE_ACT_REG_IDX_BITWIDTH);
  auto a2 = child.NewBvState(PEGetVarName(pe_idx, ACT_REG_A2),
                             PE_ACT_REG_IDX_BITWIDTH);
  auto op = child.NewBvState(PEGetVarName(pe_idx, ACT_OP), PE_ACT_OP_BITWIDTH);

  // define the register array as memory here
  for (auto i = 0; i < PE_ACT_REGS_NUM; i++) {
    child.NewMemState(PEGetVarNameVector(pe_idx, i, ACT_REGS),
                      PE_ACT_REGS_ADDR_WIDTH, PE_CORE_ACT_VECTOR_BITWIDTH);
  }

  // define the temp register array for holding the data to be stored in to the
  // data
  for (auto i = 0; i < ACT_SCALAR; i++) {
    child.NewBvState(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP),
                     PE_CORE_ACT_VECTOR_BITWIDTH);
  }

  // parent states
  // auto pe_start = m.state(PE_ACT_START_SIGNAL);
  auto pe_start = m.state(PEGetVarName(pe_idx, ACT_START_SIGNAL));

  // child initial conditions
  child.AddInit(is_start_reg == PE_ACT_INVALID);
  child.AddInit(state == PE_ACT_STATE_IDLE);

  { // instr0 ---- initiate the act unit when receiving pe_start, reset states
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_initiate"));

    auto not_start_cond = (is_start_reg == PE_ACT_INVALID);
    auto pe_start_active = (pe_start == PE_ACT_VALID);
    auto state_idle = (state == PE_ACT_STATE_IDLE);

    instr.SetDecode(config_valid & not_start_cond & pe_start_active &
                    state_idle);

    // state updates
    instr.SetUpdate(is_start_reg,
                    BvConst(PE_ACT_VALID, PE_ACT_IS_START_REG_BITWIDTH));
    instr.SetUpdate(pe_start,
                    BvConst(PE_ACT_INVALID, PE_START_SIGNAL_SHARED_BITWIDTH));

    // reset counter
    instr.SetUpdate(instr_cntr, BvConst(0, PE_ACT_INSTR_COUNTER_BITWIDTH));
    instr.SetUpdate(output_cntr, BvConst(0, PE_ACT_OUTPUT_COUNTER_BITWIDTH));

    // // reset the act vector register
    // reset the act vector memory
    for (auto i = 0; i < PE_ACT_REGS_NUM; i++) {
      auto reg = child.state(PEGetVarNameVector(pe_idx, i, ACT_REGS));
      auto reg_next = reg;
      for (auto j = 0; j < ACT_SCALAR; j++) {
        auto zero = BvConst(0, PE_CORE_ACT_VECTOR_BITWIDTH);
        auto addr = BvConst(j, PE_ACT_REGS_ADDR_WIDTH) +
                    BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
        reg_next = Store(reg_next, addr, zero + zero);
      }
      instr.SetUpdate(reg, reg_next);
    }

    // set the FSM state to fetch
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_FETCH, PE_ACT_STATE_BITWIDTH));
  }

  { // instr1 ---- fetch instruction in the act unit
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_fetch"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_fetch = (state == PE_ACT_STATE_FETCH);

    instr.SetDecode(is_start & state_fetch);

    // fetch instruction from the instruction register
    auto current_instr = PEActInstrFetch(m, pe_idx, instr_cntr);
    auto a1_next =
        Extract(current_instr, PE_ACT_REG_A1_HI_IDX, PE_ACT_REG_A1_LO_IDX);
    auto a2_next =
        Extract(current_instr, PE_ACT_REG_A2_HI_IDX, PE_ACT_REG_A2_LO_IDX);
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

    auto num_instr =
        m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_NUM_INST));
    auto num_output =
        m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_NUM_OUTPUT));

    auto is_last_instr = (instr_cntr == num_instr - 1);
    auto is_last_output = (output_cntr == num_output - 1);
    auto is_end = is_last_instr & is_last_output;

    auto instr_cntr_next =
        Ite(is_last_instr, BvConst(0, PE_ACT_INSTR_COUNTER_BITWIDTH),
            instr_cntr + 1);
    auto output_cntr_next =
        Ite(is_end, BvConst(0, PE_ACT_OUTPUT_COUNTER_BITWIDTH),
            Ite(is_last_instr & !is_last_output, output_cntr + 1, output_cntr));
    auto is_zero_next =
        Ite(is_end, BvConst(0, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST_WIDTH),
            m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST)));

    instr.SetUpdate(instr_cntr, instr_cntr_next);
    instr.SetUpdate(output_cntr, output_cntr_next);
    instr.SetUpdate(
        m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST)),
        is_zero_next);

    auto next_state =
        Ite(is_end, BvConst(PE_ACT_STATE_SEND_DONE, PE_ACT_STATE_BITWIDTH),
            BvConst(PE_ACT_STATE_FETCH, PE_ACT_STATE_BITWIDTH));
    auto is_start_reg_next =
        Ite(is_end, BvConst(PE_ACT_INVALID, PE_ACT_IS_START_REG_BITWIDTH),
            BvConst(PE_ACT_VALID, PE_ACT_IS_START_REG_BITWIDTH));

    instr.SetUpdate(state, next_state);
    instr.SetUpdate(is_start_reg, is_start_reg_next);
  }

  { // instr 3 ---- schedule the pe_done, send pe_done after all the pe are
    // done.
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
    instr.SetUpdate(
        m.state(PE_DONE_SIGNAL_SHARED),
        Ite(all_pe_cond, BvConst(PE_ACT_VALID, PE_DONE_SIGNAL_SHARED_BITWIDTH),
            BvConst(PE_ACT_INVALID, PE_DONE_SIGNAL_SHARED_BITWIDTH)));
  }

  { // instr 4 ---- op 0x1, LOAD instruction with is_zero_first == 1
    auto instr =
        child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_load_zero"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_load = (op == PE_ACT_OP_LOAD);
    auto is_zero_first =
        (m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST)) ==
         PE_ACT_VALID);

    instr.SetDecode(is_start & state_exec & op_load & is_zero_first);

    // next state
    // 0417 update: add a MEM FSM state to deal with memory store operation
    // instr.SetUpdate(state, BvConst(PE_ACT_STATE_INCR,
    // PE_ACT_STATE_BITWIDTH));
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto zero = BvConst(0, PE_CORE_ACT_VECTOR_BITWIDTH);
      auto data = zero + zero;
      auto reg_tmp = child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP));
      instr.SetUpdate(reg_tmp, data);
    }
  }

  { // instr 5 ---- op 0x1, LOAD instruction with is_zero_first == 0
    // LOAD SRAM -> A2 load address is determined by the output_counter +
    // buffer_addr_base
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_load"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_load = (op == PE_ACT_OP_LOAD);
    auto no_zero_first =
        (m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_IS_ZERO_FIRST)) ==
         PE_ACT_INVALID);

    instr.SetDecode(is_start & state_exec & op_load & no_zero_first);

    // next state updates
    // auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    // instr.SetUpdate(state, next_state);
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    auto buffer_addr_base =
        m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_BUFFER_ADDR_BASE));
    // set the rd_addr to 32 bit
    // 05032020: This rd_addr is vector level address!!, need to left shift 4
    // bits to get byte addr!!!
    auto rd_addr =
        Concat(BvConst(0, TOP_ADDR_IN_WIDTH - output_cntr.bit_width()),
               output_cntr) +
        Concat(BvConst(0, TOP_ADDR_IN_WIDTH - buffer_addr_base.bit_width()),
               buffer_addr_base);
    rd_addr = rd_addr << 4;
    // act buffer
    auto buffer = m.state(PEGetVarName(pe_idx, ACT_BUFFER));
    auto adptflow_bias =
        m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto data_buf = Load(buffer, rd_addr + i);
      auto data_fixed = Adptfloat2Fixed(data_buf, adptflow_bias);
      auto reg_tmp = child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP));
      instr.SetUpdate(reg_tmp, data_fixed);
    }
  }

  { // instr 6 ---- op 0x2, STORE instruction
    // STORE SRAM <- A2 Store address is determined by the output_counter +
    // buffer_addr_base
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_store"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_store = (op == PE_ACT_OP_STORE);

    instr.SetDecode(is_start & state_exec & op_store);

    // next FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);

    auto buffer_addr_base =
        m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_BUFFER_ADDR_BASE));

    // update 05032020: The wr_addr here is vector level addr, need to left
    // shift 4 bits to get byte level addr
    auto wr_addr =
        Concat(BvConst(0, TOP_ADDR_IN_WIDTH - output_cntr.bit_width()),
               output_cntr) +
        Concat(BvConst(0, TOP_ADDR_IN_WIDTH - buffer_addr_base.bit_width()),
               buffer_addr_base);
    wr_addr = wr_addr << 4;

    // get the ExprRef of the register
    auto buffer = m.state(PEGetVarName(pe_idx, ACT_BUFFER));
    auto buffer_next = buffer;
    auto adptflow_bias =
        m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS));

    // don't need to use the vector method for it only reads from the registers
    for (auto i = 0; i < ACT_SCALAR; i++) {
      // TODO: implement the Fixed2Adptflow function
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH) +
                      BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
      auto data_reg = PEActRegLoad_v(child, pe_idx, a2, reg_addr);
      auto data_adptflow = Fixed2Adptfloat(data_reg, adptflow_bias);
      buffer_next = Store(buffer_next, wr_addr + i, data_adptflow);
    }

    instr.SetUpdate(buffer, buffer_next);
  }

  { // instr 7 ---- op 0x3, INPE instruction
    // INPE act_port -> A2 Do not increment instruction if nothing recieved
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_inpe"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_inpe = (op == PE_ACT_OP_INPE);
    // halt the instruction when there is no data pushed to the port
    auto port_valid = (m.state(PEGetVarName(pe_idx, CORE_ACT_REG_PORT_VALID)) ==
                       PE_ACT_VALID);

    instr.SetDecode(is_start & state_exec & op_inpe & port_valid);

    // set the port from pe core to be invalid
    instr.SetUpdate(
        m.state(PEGetVarName(pe_idx, CORE_ACT_REG_PORT_VALID)),
        BvConst(PE_ACT_INVALID, PE_CORE_ACT_REG_PORT_VALID_BITWIDTH));

    // next state
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto data = m.state(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECTOR));
      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP)),
                      data);
    }
  }

  { // instr 8 ---- op 0x4, OUTGB, output result to GB
    // OUTGB A2 -> Output A2 to GB
    // TODO: Need to make sequential access to the shared states
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_outgb"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_outgb = (op == PE_ACT_OP_OUTGB);
    // need to schedule the sequential access
    // need to check whether gb has popped the previous data
    auto data_out_invalid =
        (m.state(GB_CONTROL_DATA_IN_VALID) == PE_ACT_INVALID);
    auto cntr_valid = (m.state(PE_ACT_OUTGB_CNTR) == pe_idx);

    instr.SetDecode(is_start & state_exec & op_outgb & data_out_invalid &
                    cntr_valid);

    auto all_pe_cond = (m.state(PE_ACT_OUTGB_CNTR) >= 3);
    auto pe_cntr_next = Ite(all_pe_cond, BvConst(0, PE_ACT_OUTGB_CNTR_BITWIDTH),
                            m.state(PE_ACT_OUTGB_CNTR) + 1);
    // update pe counter
    instr.SetUpdate(m.state(PE_ACT_OUTGB_CNTR), pe_cntr_next);
    // update FSM state
    auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);
    // set the gb_control_data_in to be valid
    instr.SetUpdate(m.state(GB_CONTROL_DATA_IN_VALID),
                    BvConst(PE_ACT_VALID, GB_CONTROL_DATA_IN_VALID_BITWIDTH));

    auto output_base_addr =
        m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_OUTPUT_ADDR_BASE));
    auto data_out_addr = output_cntr + output_base_addr;

    instr.SetUpdate(m.state(GB_CONTROL_DATA_IN_ADDR), data_out_addr);

    auto adptflow_bias =
        m.state(PEGetVarName(pe_idx, ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH) +
                      BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
      auto result = PEActRegLoad_v(child, pe_idx, a2, reg_addr);
      // update 05012020: Need to transform the result from fixed point to
      // adaptive float when sending back to gb.
      auto result_adpfloat = Fixed2Adptfloat(result, adptflow_bias);
      instr.SetUpdate(m.state(PEGetGBDataInName(i, GB_CONTROL_DATA_IN)),
                      result_adpfloat);
    }
  }

  { // instr 9 ---- op 0x7, COPY
    // COPY A1 -> A2
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_copy"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_copy = (op == PE_ACT_OP_COPY);

    instr.SetDecode(is_start & state_exec & op_copy);

    // FSM Update
    // auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    // instr.SetUpdate(state, next_state);
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH) +
                      BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a1, reg_addr);
      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP)),
                      data);
    }
  }

  { // instr 10 ---- op 0x8, EADD, A2 = A2 + A1
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_eadd"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_eadd = (op == PE_ACT_OP_EADD);

    instr.SetDecode(is_start & state_exec & op_eadd);

    // Next FSM state
    // auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    // instr.SetUpdate(state, next_state);
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH) +
                      BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
      auto data_a1 = PEActRegLoad_v(child, pe_idx, a1, reg_addr);
      auto data_a2 = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActEadd(data_a1, data_a2);
      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP)),
                      result);
    }
  }

  { // instr 11 ---- op 0x9, EMUL, A2 = A2 * A1
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_emul"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_emul = (op == PE_ACT_OP_EMUL);

    instr.SetDecode(is_start & state_exec & op_emul);
    // Next FSM state
    // auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    // instr.SetUpdate(state, next_state);
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH) +
                      BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
      auto data_a1 = PEActRegLoad_v(child, pe_idx, a1, reg_addr);
      auto data_a2 = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActEmul(data_a1, data_a2);

      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP)),
                      result);
    }
  }

  { // instr 12 ---- op 0xA, SIGM, A2 = Sigmoid(A2)
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_sigm"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_sigm = (op == PE_ACT_OP_SIGM);

    instr.SetDecode(is_start & state_exec & op_sigm);

    // Next FSM state
    // auto next_state = BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH);
    // instr.SetUpdate(state, next_state);
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH) +
                      BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActSigmoid(data);

      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP)),
                      result);
    }
  }

  { // instr 13 ---- op 0xB, TANH, A2 = TANH(A2)
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_tanh"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_tanh = (op == PE_ACT_OP_TANH);

    instr.SetDecode(is_start & state_exec & op_tanh);

    // Next FSM state
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH) +
                      BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActTanh(data);

      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP)),
                      result);
    }
  }

  { // instr 14 ---- op 0xC, RELU, A2 = RELU(A2)
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_relu"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_relu = (op == PE_ACT_OP_RELU);

    instr.SetDecode(is_start & state_exec & op_relu);

    // Next FSM state
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH) +
                      BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActRelu(data);

      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP)),
                      result);
    }
  }

  { // instr 15 ---- op 0xD, ONEX, A2 = 1 - A2
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_op_onex"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_exec = (state == PE_ACT_STATE_EXEC);
    auto op_onex = (op == PE_ACT_OP_ONEX);

    instr.SetDecode(is_start & state_exec & op_onex);

    // Next FSM state
    instr.SetUpdate(state, BvConst(PE_ACT_STATE_MEM, PE_ACT_STATE_BITWIDTH));

    // Onex is also element wise
    for (auto i = 0; i < ACT_SCALAR; i++) {
      auto reg_addr = BvConst(i, PE_ACT_REGS_ADDR_WIDTH) +
                      BvConst(0, PE_ACT_REGS_ADDR_WIDTH);
      auto data = PEActRegLoad_v(child, pe_idx, a2, reg_addr);

      auto result = PEActOnex(data);

      instr.SetUpdate(child.state(PEGetVarNameVector(pe_idx, i, ACT_REG_TEMP)),
                      result);
    }
  }

  // 04172020 update: add a new state for register file store, decrease the time
  // to generate systemc models
  { // instr 16 ---- store data into the register file
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "act_child_reg_store"));
    auto is_start = (is_start_reg == PE_ACT_VALID);
    auto state_mem = (state == PE_ACT_STATE_MEM);

    instr.SetDecode(is_start & state_mem);

    instr.SetUpdate(state, BvConst(PE_ACT_STATE_INCR, PE_ACT_STATE_BITWIDTH));

    for (auto i = 0; i < PE_ACT_REGS_NUM; i++) {
      auto reg = child.state(PEGetVarNameVector(pe_idx, i, ACT_REGS));
      auto reg_next = reg;
      for (auto j = 0; j < ACT_SCALAR; j++) {
        auto data = child.state(PEGetVarNameVector(pe_idx, j, ACT_REG_TEMP));
        auto addr = BvConst(j, PE_ACT_REGS_ADDR_WIDTH) +
                    BvConst(0, PE_ACT_REGS_ADDR_WIDTH);

        reg_next = Ite(a2 == i, Store(reg_next, addr, data), reg_next);
      }
      instr.SetUpdate(reg, reg_next);
    }
  }
}

/****** helper function for PE Act child instructions **********/
ExprRef PEActRegLoad_v(Ila& child, const int& pe_idx, const ExprRef& reg_idx,
                       const ExprRef& addr) {
  auto result = BvConst(0, PE_CORE_ACT_VECTOR_BITWIDTH);
  for (auto i = 0; i < PE_ACT_REGS_NUM; i++) {
    auto reg = child.state(PEGetVarNameVector(pe_idx, i, ACT_REGS));
    result = Ite(reg_idx == i, Load(reg, addr), result);
  }
  return result;
}

} // namespace flex

} // namespace ilang
