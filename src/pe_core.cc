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

#include <vector>

#include <flex/flex.h>
#include <flex/uninterpreted_func.h>
#include <flex/util.h>

namespace ilang {

namespace flex {

void AddChild_PECore(Ila& m, const int& pe_idx, const uint64_t& base);
void AddChild_PECoreRunMac(Ila& m, const int& pe_idx);

void DefinePECore(Ila& m, const int& pe_idx, const uint64_t& base) {

  // PE Core instructions are not visible at the top interface, we need to use
  // child model to implement PE core Thus this DefinePECore function will only
  // contain the declaration of the child model of PE Core
  AddChild_PECore(m, pe_idx, base);
}

void AddChild_PECore(Ila& m, const int& pe_idx, const uint64_t& base) {
  auto child = m.NewChild(PEGetChildName(pe_idx, "CORE_CHILD"));
  auto pe_config_valid_bit =
      m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_VALID));
  auto pe_config_is_valid = (pe_config_valid_bit == PE_CORE_VALID);
  // child valid when instructions layer sizing is set valid.
  child.SetValid(pe_config_is_valid);

  // declare new child states
  auto is_start_reg = child.NewBvState(PEGetVarName(pe_idx, CORE_IS_START),
                                       PE_CORE_IS_START_BITWIDTH);
  auto mngr_cntr = child.NewBvState(PEGetVarName(pe_idx, CORE_MNGR_CNTR),
                                    PE_CORE_MNGR_CNTR_BITWIDTH);
  auto input_cntr = child.NewBvState(PEGetVarName(pe_idx, CORE_INPUT_CNTR),
                                     PE_CORE_INPUT_CNTR_BITWIDTH);
  auto output_cntr = child.NewBvState(PEGetVarName(pe_idx, CORE_OUTPUT_CNTR),
                                      PE_CORE_OUTPUT_CNTR_BITWIDTH);
  // states for run mac child
  auto run_mac_flag =
      child.NewBvState(PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_FLAG),
                       PE_CORE_CHILD_RUN_MAC_FLAG_BITWIDTH);
  auto run_mac_cntr =
      child.NewBvState(PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_CNTR),
                       PE_CORE_CHILD_RUN_MAC_CNTR_BITWIDTH);
  auto run_mac_state =
      child.NewBvState(PEGetVarName(pe_idx, CORE_RUN_MAC_CHILD_STATE),
                       PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH);
  auto weight_base_v = child.NewBvState(
      PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_WEIGHT_BASE_VECTOR),
      PE_CORE_CHILD_RUN_MAC_WEIGHT_BASE_VECTOR_BITWIDTH);
  auto input_base_v = child.NewBvState(
      PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_INPUT_BASE_VECTOR),
      PE_CORE_CHILD_RUN_MAC_INPUT_BASE_VECTOR_BITWIDTH);
  // states for run bias child
  auto run_bias_flag =
      child.NewBvState(PEGetVarName(pe_idx, CORE_RUN_BIAS_CHILD_FLAG),
                       PE_CORE_RUN_BIAS_CHILD_FLAG_BITWIDTH);
  auto run_bias_cntr =
      child.NewBvState(PEGetVarName(pe_idx, CORE_RUN_BIAS_CHILD_CNTR),
                       PE_CORE_RUN_BIAS_CHILD_CNTR_BITWIDTH);

  // common states used in this child instructions
  auto state = m.state(PEGetVarName(pe_idx, CORE_STATE));

  // state holding valid pe numbers
  auto pe_valid_num = m.state(PE_VALID_NUM);

  // core accumulator registers
  for (auto i = 0; i < PE_CORE_ACCUM_VECTOR_LANES; i++) {
    child.NewBvState(PEGetVarNameVector(pe_idx, i, CORE_ACCUM_VECTOR),
                     PE_CORE_ACCUM_VECTOR_BITWIDTH);
    child.NewBvState(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECTOR_REG),
                     PE_CORE_ACT_VECTOR_REG_BITWIDTH);
  }

  // add initial condition
  child.AddInit(is_start_reg == PE_CORE_INVALID);
  child.AddInit(mngr_cntr == PE_CORE_MNGR_0);
  child.AddInit(input_cntr == 0);
  child.AddInit(output_cntr == 0);

  { // instructions 0 ---- read the data from GB
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "core_read_gb"));

    auto pe_not_start = (is_start_reg == PE_CORE_INVALID);
    auto gb_data_valid = (m.state(GB_CONTROL_DATA_OUT_VALID) == PE_CORE_VALID);

    auto cntr_valid = (m.state(PE_CORE_CNTR) == pe_idx);
    auto state_idle = (state == PE_CORE_STATE_IDLE);

    instr.SetDecode(pe_config_is_valid & pe_not_start & gb_data_valid &
                    cntr_valid & state_idle);

    auto m_index = m.state(GB_CONTROL_DATA_OUT_INDEX);
    auto base_addr = Ite(
        (m_index == PE_CORE_MNGR_0),
        m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_INPUT)),
        m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_INPUT)));
    auto data_in_addr = m.state(GB_CONTROL_DATA_OUT_ADDR);
    auto data_in_addr_16 = Concat(BvConst(0, 8), data_in_addr);
    auto input_wr_addr =
        base_addr + data_in_addr_16; // this address is vector level (16 byte);

    input_wr_addr = (Concat(BvConst(0, 16), input_wr_addr))
                    << 4; // change the address to byte

    // fetch the data from GB and store them into the input buffer
    auto mem = m.state(PEGetVarName(pe_idx, CORE_INPUT_BUFFER));

    auto mem_next =
        Store(mem, input_wr_addr + 0, m.state(GB_CONTROL_DATA_OUT_0));
    mem_next =
        Store(mem_next, input_wr_addr + 1, m.state(GB_CONTROL_DATA_OUT_1));
    mem_next =
        Store(mem_next, input_wr_addr + 2, m.state(GB_CONTROL_DATA_OUT_2));
    mem_next =
        Store(mem_next, input_wr_addr + 3, m.state(GB_CONTROL_DATA_OUT_3));
    mem_next =
        Store(mem_next, input_wr_addr + 4, m.state(GB_CONTROL_DATA_OUT_4));
    mem_next =
        Store(mem_next, input_wr_addr + 5, m.state(GB_CONTROL_DATA_OUT_5));
    mem_next =
        Store(mem_next, input_wr_addr + 6, m.state(GB_CONTROL_DATA_OUT_6));
    mem_next =
        Store(mem_next, input_wr_addr + 7, m.state(GB_CONTROL_DATA_OUT_7));
    mem_next =
        Store(mem_next, input_wr_addr + 8, m.state(GB_CONTROL_DATA_OUT_8));
    mem_next =
        Store(mem_next, input_wr_addr + 9, m.state(GB_CONTROL_DATA_OUT_9));
    mem_next =
        Store(mem_next, input_wr_addr + 10, m.state(GB_CONTROL_DATA_OUT_10));
    mem_next =
        Store(mem_next, input_wr_addr + 11, m.state(GB_CONTROL_DATA_OUT_11));
    mem_next =
        Store(mem_next, input_wr_addr + 12, m.state(GB_CONTROL_DATA_OUT_12));
    mem_next =
        Store(mem_next, input_wr_addr + 13, m.state(GB_CONTROL_DATA_OUT_13));
    mem_next =
        Store(mem_next, input_wr_addr + 14, m.state(GB_CONTROL_DATA_OUT_14));
    mem_next =
        Store(mem_next, input_wr_addr + 15, m.state(GB_CONTROL_DATA_OUT_15));

    instr.SetUpdate(mem, mem_next);

    // update the control parameters
    auto all_pe_cond = BoolConst(pe_idx >= 3);
    auto pe_cntr_next = Ite(all_pe_cond, BvConst(0, PE_CORE_CNTR_BIWTDTH),
                            BvConst(pe_idx + 1, PE_CORE_CNTR_BIWTDTH));
    auto gb_control_data_out_valid_next =
        Ite(all_pe_cond,
            BvConst(PE_CORE_INVALID, GB_CONTROL_DATA_OUT_VALID_BITWIDTH),
            BvConst(PE_CORE_VALID, GB_CONTROL_DATA_OUT_VALID_BITWIDTH));

    instr.SetUpdate(m.state(PE_CORE_CNTR), pe_cntr_next);
    instr.SetUpdate(m.state(GB_CONTROL_DATA_OUT_VALID),
                    gb_control_data_out_valid_next);
  }

  { // instruction 2 ---- helper instructions for is_start condition
    // updates on the GB_Control: the pe_start valid only after all the PE have
    // read the last piece of data.
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "core_is_start"));

    auto pe_start_valid = (m.state(PE_CORE_START_SIGNAL) == PE_CORE_VALID);
    auto state_idle = (state == PE_CORE_STATE_IDLE);
    auto is_start = pe_config_is_valid & pe_start_valid;
    // this instruction access the shared states, thus needs scheduling
    auto cntr_valid = (m.state(PE_CORE_CNTR) == pe_idx);
    // only need the pe_start_valid here because the d
    instr.SetDecode(is_start & state_idle & cntr_valid);

    auto next_state = BvConst(PE_CORE_STATE_PRE, PE_CORE_STATE_BITWIDTH);
    auto all_pe_cond = BoolConst(pe_idx >= 3);
    auto pe_cntr_next = Ite(all_pe_cond, BvConst(0, PE_CORE_CNTR_BIWTDTH),
                            BvConst(pe_idx + 1, PE_CORE_CNTR_BIWTDTH));
    auto pe_start_next = Ite(
        all_pe_cond, BvConst(PE_CORE_INVALID, PE_START_SIGNAL_SHARED_BITWIDTH),
        m.state(PE_CORE_START_SIGNAL));

    instr.SetUpdate(state, next_state);
    // set the is_start_reg to be valid, and set the shared start state to be
    // invalid, immitate the behavior of the pop operation NEED TO BE CAREFUL
    // ABOUT THE 4 PE CORES ACCESSES TO THE SHARED STATES!!!!!
    instr.SetUpdate(is_start_reg,
                    BvConst(PE_CORE_VALID, PE_CORE_IS_START_BITWIDTH));
    instr.SetUpdate(m.state(PE_CORE_CNTR), pe_cntr_next);
    instr.SetUpdate(m.state(PE_CORE_START_SIGNAL), pe_start_next);

    // update 04132020: initialize the shared valid flag for act_port_register
    instr.SetUpdate(
        m.state(PEGetVarName(pe_idx, CORE_ACT_REG_PORT_VALID)),
        BvConst(PE_CORE_INVALID, PE_CORE_ACT_REG_PORT_VALID_BITWIDTH));
  }

  { // instruction 3 ---- select next state
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "core_prep"));

    auto is_start = pe_config_is_valid & is_start_reg;
    auto state_pre = (state == PE_CORE_STATE_PRE);

    instr.SetDecode(is_start & state_pre);

    auto is_zero_first = m.state(
        PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_ZERO_FIRST));
    auto zero_active = Ite(
        mngr_cntr == PE_CORE_MNGR_0,
        m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ZERO_ACTIVE)),
        m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ZERO_ACTIVE)));
    auto zero_first_cond =
        (is_zero_first == PE_CORE_VALID) & (zero_active == PE_CORE_VALID);
    auto next_state = Ite(zero_first_cond,
                          BvConst(PE_CORE_STATE_BIAS, PE_CORE_STATE_BITWIDTH),
                          BvConst(PE_CORE_STATE_MAC, PE_CORE_STATE_BITWIDTH));

    // states updates
    // reset the accumulate registers
    for (auto i = 0; i < CORE_SCALAR; i++) {
      instr.SetUpdate(
          child.state(PEGetVarNameVector(pe_idx, i, CORE_ACCUM_VECTOR)),
          BvConst(0, PE_CORE_ACCUM_VECTOR_BITWIDTH));
      instr.SetUpdate(
          child.state(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECTOR_REG)),
          BvConst(0, PE_CORE_ACT_VECTOR_REG_BITWIDTH));
    }

    instr.SetUpdate(state, next_state);

    // reset the run mac child valid flag
    // when run_mac_flag is valid, it means it is still calculating the results,
    // and instr 4 shouldn't run
    instr.SetUpdate(run_mac_flag,
                    BvConst(PE_CORE_INVALID,
                            PE_CORE_CHILD_RUN_MAC_INPUT_BASE_VECTOR_BITWIDTH));
  }

  { // instruction 4 ---- MAC state
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "core_mac"));

    auto is_start = pe_config_is_valid & is_start_reg;
    auto state_mac = (state == PE_CORE_STATE_MAC);
    auto run_mac_invalid = (run_mac_flag == PE_CORE_INVALID);

    instr.SetDecode(is_start & state_mac & run_mac_invalid);

    // state transition control signal
    auto num_input = Ite(
        mngr_cntr == PE_CORE_MNGR_0,
        m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_NUM_INPUT)),
        m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_NUM_INPUT)));
    auto is_input_end = (input_cntr >= num_input - 1);
    auto next_state =
        Ite(is_input_end, BvConst(PE_CORE_STATE_BIAS, PE_CORE_STATE_BITWIDTH),
            BvConst(PE_CORE_STATE_MAC, PE_CORE_STATE_BITWIDTH));
    auto input_cntr_next = Ite(
        is_input_end, BvConst(0, PE_CORE_INPUT_CNTR_BITWIDTH), input_cntr + 1);

    instr.SetUpdate(state, next_state);
    // update input counter
    instr.SetUpdate(input_cntr, input_cntr_next);

    // calculate the addresses
    auto is_cluster =
        m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER));

    auto config_base_weight = Ite(
        mngr_cntr == PE_CORE_MNGR_0,
        m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_WEIGHT)),
        m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_WEIGHT)));
    auto config_base_input = Ite(
        mngr_cntr == PE_CORE_MNGR_0,
        m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_INPUT)),
        m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_INPUT)));

    auto num_input_16 =
        Concat(BvConst(0, 16 - num_input.bit_width()), num_input);
    auto input_cntr_16 =
        Concat(BvConst(0, 16 - input_cntr.bit_width()), input_cntr);
    auto output_cntr_16 =
        Concat(BvConst(0, 16 - output_cntr.bit_width()), output_cntr);

    auto weight_base_v_next =
        Ite(is_cluster == PE_CORE_VALID,
            (output_cntr_16 * num_input_16 + input_cntr_16) * 8 +
                config_base_weight,
            (output_cntr_16 * num_input_16 + input_cntr_16) * 16 +
                config_base_weight);
    auto input_base_v_next = input_cntr_16 + config_base_input;

    // update 06022020: seperate clustered and non clustered weight fetch for
    // run mac child
    auto run_mac_state_next =
        Ite(is_cluster == PE_CORE_VALID,
            BvConst(PE_CORE_RUN_MAC_STATE_FETCH_CLUSTER,
                    PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH),
            BvConst(PE_CORE_RUN_MAC_STATE_FETCH_NON_CLUSTER,
                    PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH));

    // auto run_mac_state_fetch = BvConst(PE_CORE_RUN_MAC_STATE_FETCH,
    //                                     PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH);

    instr.SetUpdate(weight_base_v, weight_base_v_next);
    instr.SetUpdate(input_base_v, input_base_v_next);

    instr.SetUpdate(run_mac_flag, BvConst(PE_CORE_VALID,
                                          PE_CORE_CHILD_RUN_MAC_FLAG_BITWIDTH));
    instr.SetUpdate(run_mac_cntr,
                    BvConst(0, PE_CORE_CHILD_RUN_MAC_CNTR_BITWIDTH));
    instr.SetUpdate(run_mac_state, run_mac_state_next);

    // Add child to do the run mac function
    // weight: 16 x 16, input vector 16
    // child do the MAC by 16 steps, do a weight vector and input vector
    // multiplication each step.
    AddChild_PECoreRunMac(m, pe_idx);
  }

  { // instruction 5 ---- BIAS state
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "core_bias"));

    auto is_start = pe_config_is_valid & is_start_reg;
    auto run_mac_invalid = (run_mac_flag == PE_CORE_INVALID);
    auto state_bias = (state == PE_CORE_STATE_BIAS);

    instr.SetDecode(is_start & state_bias & run_mac_invalid);

    auto is_bias =
        m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_BIAS));
    auto bias_w = Ite(mngr_cntr == 0,
                      m.state(PEGetVarName(
                          pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_W)),
                      m.state(PEGetVarName(
                          pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_W)));
    auto bias_i = Ite(mngr_cntr == 0,
                      m.state(PEGetVarName(
                          pe_idx, MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_I)),
                      m.state(PEGetVarName(
                          pe_idx, MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_I)));

    // Hand over to the uninterpreted function to deal with the right shrift
    // values auto bias_w_32 = Concat(BvConst(0, 32 - bias_w.bit_width()),
    // bias_w); auto bias_i_32 = Concat(BvConst(0, 32 - bias_i.bit_width()),
    // bias_i);
    // // NVUINT5 right_shift = -2*spec::kAdpfloatOffset +
    // 2*spec::kAdpfloatManWidth - spec::kActNumFrac
    // //                         - pe_manager[m_index].adplfloat_bias_weight
    // //                         - pe_manager[m_index].adplfloat_bias_input;
    // auto right_shift = BvConst(ADPTFLOW_MAN_WIDTH, 32) * 2 +
    // BvConst(ADPTFLOW_OFFSET_NEG, 32) * 2
    //                     - BvConst(ACT_NUM_FRAC, 32) - bias_w_32 - bias_i_32;

    auto base_bias = Ite(
        mngr_cntr == 0,
        m.state(PEGetVarName(pe_idx, MEM_MNGR_FIRST_CONFIG_REG_BASE_BIAS)),
        m.state(PEGetVarName(pe_idx, MEM_MNGR_SECOND_CONFIG_REG_BASE_BIAS)));
    // extend the address to 32 bits
    auto base_bias_32 =
        Concat(BvConst(0, 32 - base_bias.bit_width()), base_bias);
    auto output_cntr_32 =
        Concat(BvConst(0, 32 - output_cntr.bit_width()), output_cntr);
    auto bias_addr_base = (base_bias_32 + output_cntr_32)
                          << 4; // turn the address into byte unit

    auto input_mem = m.state(PEGetVarName(pe_idx, CORE_INPUT_BUFFER));

    auto adpfloat_bias_bias =
        Ite(mngr_cntr == 0,
            m.state(PEGetVarName(pe_idx,
                                 MEM_MNGR_FIRST_CONFIG_REG_ADPFLOAT_BIAS_B)),
            m.state(PEGetVarName(pe_idx,
                                 MEM_MNGR_SECOND_CONFIG_REG_ADPFLOAT_BIAS_B)));

    for (auto i = 0; i < CORE_SCALAR; i++) {
      auto accum_vector =
          child.state(PEGetVarNameVector(pe_idx, i, CORE_ACCUM_VECTOR));
      std::vector<ExprRef> rs_input = {accum_vector, bias_w, bias_i};
      // TODO: Uninterpreted function: Right Shift, Get Bias
      auto accum_vector_shifted = PECoreAccumRightShift(rs_input);
      // auto accum_vector_out = PECoreAccumRightShift(accum_vector);
      auto bias = Load(input_mem, bias_addr_base + i);
      std::vector<ExprRef> get_bias_input = {accum_vector_shifted, bias,
                                             adpfloat_bias_bias};
      auto accum_vector_out_with_bias = PECoreAccumGetBiasOut(get_bias_input);

      auto accum_vector_out =
          Ite(is_bias == 1, accum_vector_out_with_bias, accum_vector_shifted);
      accum_vector_out = PECoreAccumOverflowCheck(accum_vector_out);

      auto act_reg_tmp = PECoreAccum2ActReg(accum_vector_out);
      auto act_vector_reg =
          child.state(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECTOR_REG));

      // update the activation vector registers
      instr.SetUpdate(act_vector_reg, act_reg_tmp);
    }

    auto next_state = BvConst(PE_CORE_STATE_OUT, PE_CORE_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);
  }

  { // instruction 6 ---- OUT state
    auto instr = child.NewInstr(PEGetInstrName(pe_idx, "core_out"));

    auto is_start = pe_config_is_valid & is_start_reg;
    auto state_out = (state == PE_CORE_STATE_OUT);
    // update 04162020: flexnlp use blocking push on act port, thus if port
    // valid is high, it should halt this instruction
    auto act_port_invalid =
        (m.state(PEGetVarName(pe_idx, CORE_ACT_REG_PORT_VALID)) ==
         PE_CORE_INVALID);

    instr.SetDecode(is_start & state_out & act_port_invalid);

    auto num_mngr =
        m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_NUM_MANAGER));
    auto num_output =
        m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_NUM_OUTPUT));
    auto mngr_cntr_4 = Concat(BvConst(0, 4 - mngr_cntr.bit_width()), mngr_cntr);
    auto last_mngr = (mngr_cntr_4 == (num_mngr - 1));
    auto last_output = (output_cntr == (num_output - 1));
    auto is_output_end = last_mngr & last_output;

    auto is_zero_first = m.state(
        PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_ZERO_FIRST));

    auto mngr_cntr_next =
        Ite(last_mngr, BvConst(0, PE_CORE_MNGR_CNTR_BITWIDTH), mngr_cntr + 1);
    auto output_cntr_next =
        Ite(last_mngr & last_output, BvConst(0, PE_CORE_OUTPUT_CNTR_BITWIDTH),
            Ite(last_mngr & !last_output, output_cntr + 1, output_cntr));
    // when is_zero_first & zero_active is valid, the output for the first
    // timestep should all be zero.
    auto is_zero_first_next =
        Ite(last_mngr & last_output,
            BvConst(0, RNN_LAYER_SIZING_CONFIG_REG_IS_ZERO_FIRST_WIDTH),
            is_zero_first);

    auto next_state =
        Ite(is_output_end, BvConst(PE_CORE_STATE_IDLE, PE_CORE_STATE_BITWIDTH),
            BvConst(PE_CORE_STATE_PRE, PE_CORE_STATE_BITWIDTH));
    // use the is_start_reg to end the run mac when there is no new pe_start
    // signal pushed into the channel
    auto is_start_reg_next =
        Ite(is_output_end, BvConst(PE_CORE_INVALID, PE_CORE_STATE_BITWIDTH),
            BvConst(PE_CORE_VALID, PE_CORE_STATE_BITWIDTH));

    instr.SetUpdate(is_start_reg, is_start_reg_next);

    instr.SetUpdate(mngr_cntr, mngr_cntr_next);
    instr.SetUpdate(output_cntr, output_cntr_next);
    instr.SetUpdate(is_zero_first, is_zero_first_next);
    instr.SetUpdate(state, next_state);

    // update 04132020: set the act_port_reg valid bit at output stage
    // FlexNLP use blocking push on act_port, thus if the port valid is high, it
    // should halt this out instruction
    instr.SetUpdate(
        m.state(PEGetVarName(pe_idx, CORE_ACT_REG_PORT_VALID)),
        BvConst(PE_CORE_VALID, PE_CORE_ACT_REG_PORT_VALID_BITWIDTH));

    // update 05012020: add act_port_reg to hold the result from RunBias for
    // act_port, in accordance to the FlexNLP implementation
    for (auto i = 0; i < PE_CORE_ACT_VECTOR_LANES; i++) {
      auto act_vector_reg =
          child.state(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECTOR_REG));
      auto act_vector = m.state(PEGetVarNameVector(pe_idx, i, CORE_ACT_VECTOR));
      instr.SetUpdate(act_vector, act_vector_reg);
    }
  }
}

// *************************************************************//
// ************ child model : PECoreRunMac *********************//
// *************************************************************//
void AddChild_PECoreRunMac(Ila& m, const int& pe_idx) {
  auto child_pe_core = m.child(PEGetChildName(pe_idx, "CORE_CHILD"));
  auto child_run_mac =
      child_pe_core.NewChild(PEGetChildName(pe_idx, "CORE_RUN_MAC_CHILD"));
  auto run_mac_flag =
      child_pe_core.state(PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_FLAG));
  auto run_mac_cntr = child_pe_core.state(PEGetVarName(
      pe_idx, CORE_CHILD_RUN_MAC_CNTR)); // check cntr_bitwidth > 4 !!

  auto child_valid = ((run_mac_flag == PE_CORE_VALID) & (run_mac_cntr < 16));

  child_run_mac.SetValid(child_valid);

  // common states
  auto state =
      child_pe_core.state(PEGetVarName(pe_idx, CORE_RUN_MAC_CHILD_STATE));
  auto is_cluster =
      (m.state(PEGetVarName(pe_idx, RNN_LAYER_SIZING_CONFIG_REG_IS_CLUSTER)) ==
       PE_CORE_VALID);

  auto weight_base_v = child_pe_core.state(
      PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_WEIGHT_BASE_VECTOR));
  auto input_base_v = child_pe_core.state(
      PEGetVarName(pe_idx, CORE_CHILD_RUN_MAC_INPUT_BASE_VECTOR));
  // transfering vector address into byte address, in 32 bits.
  auto weight_base_b =
      (Concat(BvConst(0, TOP_ADDR_IN_WIDTH - weight_base_v.bit_width()),
              weight_base_v))
      << 4;
  auto input_base_b =
      (Concat(BvConst(0, TOP_ADDR_IN_WIDTH - input_base_v.bit_width()),
              input_base_v))
      << 4;
  auto weight_buffer = m.state(PEGetVarName(pe_idx, CORE_WEIGHT_BUFFER));
  auto input_buffer = m.state(PEGetVarName(pe_idx, CORE_INPUT_BUFFER));

  // declare child states
  for (auto i = 0; i < 16; i++) {
    // declare states for holding weight vector values
    child_run_mac.NewBvState(
        PEGetVarNameVector(pe_idx, i, CORE_RUN_MAC_CHILD_WEIGHT_BYTE),
        PE_CORE_RUN_MAC_CHILD_WEIGHT_BYTE_BITWIDTH);
    // declare states for holding input vector values
    child_run_mac.NewBvState(
        PEGetVarNameVector(pe_idx, i, CORE_RUN_MAC_CHILD_INPUT_BYTE),
        PE_CORE_RUN_MAC_CHILD_INPUT_BYTE_BITWIDTH);
  }

  auto result_tmp = child_run_mac.NewBvState(
      PEGetVarName(pe_idx, CORE_RUN_MAC_CHILD_RESULT_TEMP),
      PE_CORE_RUN_MAC_CHILD_RESULT_TEMP_BITWIDTH);

  // update 02062020: seperate the cluster and non-clustered mode data fetch
  // into two different instructions
  { // instruction 0 -- get data from the memory, non-cluster
    auto instr =
        child_run_mac.NewInstr(PEGetInstrName(pe_idx, "core_run_mac_data_nc"));
    auto state_fnc = (state == PE_CORE_RUN_MAC_STATE_FETCH_NON_CLUSTER);

    instr.SetDecode(child_valid & state_fnc & !is_cluster);

    // fetch the non-clustered weigth values.
    // update: 05012020: extend the run_mac_cntr bitwidth to hold values larger
    // than 31.. Always keep in mind that the result of muliplication is bounded
    // by the parameter bitwidth!!!
    auto run_mac_cntr_32 =
        Concat(BvConst(0, weight_base_b.bit_width() - run_mac_cntr.bit_width()),
               run_mac_cntr);

    for (auto i = 0; i < 16; i++) {
      // weight data
      auto addr_offset = run_mac_cntr_32 * CORE_SCALAR + i;
      auto addr = weight_base_b + addr_offset;
      // auto addr = weight_base_b +
      //       Concat(BvConst(0, weight_base_b.bit_width() -
      //       addr_offset.bit_width()), addr_offset);
      auto data = Load(weight_buffer, addr);
      instr.SetUpdate(child_run_mac.state(PEGetVarNameVector(
                          pe_idx, i, CORE_RUN_MAC_CHILD_WEIGHT_BYTE)),
                      data);
      // input data
      auto input_addr_offset = i;
      auto input_addr = input_base_b + i;

      instr.SetUpdate(child_run_mac.state(PEGetVarNameVector(
                          pe_idx, i, CORE_RUN_MAC_CHILD_INPUT_BYTE)),
                      Load(input_buffer, input_addr));
    }

    auto next_state = BvConst(PE_CORE_RUN_MAC_STATE_MUL,
                              PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);
  }

  { // instructions 1 ---- get the data from the memory, clustered
    auto instr =
        child_run_mac.NewInstr(PEGetInstrName(pe_idx, "core_run_mac_data_c"));
    auto state_fc = (state == PE_CORE_RUN_MAC_STATE_FETCH_CLUSTER);

    instr.SetDecode(child_valid & state_fc & is_cluster);

    // fetch the non-clustered weigth values.
    // update: 05012020: extend the run_mac_cntr bitwidth to hold values larger
    // than 31.. Always keep in mind that the result of muliplication is bounded
    // by the parameter bitwidth!!!
    auto run_mac_cntr_32 =
        Concat(BvConst(0, weight_base_b.bit_width() - run_mac_cntr.bit_width()),
               run_mac_cntr);

    // fetch the clustered weight values
    for (auto i = 0; i < 16; i++) {
      // Update 04302020, the clustering fetching index is different: it will
      // get the lower half of all the weight in the weight matrix than get the
      // upper half.
      auto addr_offset = run_mac_cntr_32 * (CORE_SCALAR / 2) + i / 2;
      auto addr = weight_base_b + addr_offset;
      // auto addr = weight_base_b +
      //       Concat(BvConst(0, weight_base_b.bit_width() -
      //       addr_offset.bit_width()), addr_offset);
      auto data = Load(weight_buffer, addr);
      // get the index after splitting the data for clustered mode.
      auto ind =
          Ite(BoolConst(i % 2 == 0), Extract(data, 3, 0), Extract(data, 7, 4));
      // auto ind = Ite(URem(run_mac_cntr, BvConst(2, run_mac_cntr.bit_width()))
      // == 0,
      //                Extract(data, 3, 0), Extract(data, 7, 4));

      // get the cluster lut result
      auto mgnr_cntr =
          child_pe_core.state(PEGetVarName(pe_idx, CORE_MNGR_CNTR));
      // FetchClusterLut is defined in utils.cc
      auto result = Ite(mgnr_cntr == 0, FetchClusterLUT_First(m, pe_idx, ind),
                        FetchClusterLUT_Second(m, pe_idx, ind));

      instr.SetUpdate(child_run_mac.state(PEGetVarNameVector(
                          pe_idx, i, CORE_RUN_MAC_CHILD_WEIGHT_BYTE)),
                      result);

      // load the input data
      auto input_addr_offset = i;
      auto input_addr = input_base_b + i;

      instr.SetUpdate(child_run_mac.state(PEGetVarNameVector(
                          pe_idx, i, CORE_RUN_MAC_CHILD_INPUT_BYTE)),
                      Load(input_buffer, input_addr));
    }

    auto next_state = BvConst(PE_CORE_RUN_MAC_STATE_MUL,
                              PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH);
    instr.SetUpdate(state, next_state);
  }

  { // instruction 2 ---- multiply the weight vector and input vector
    auto instr =
        child_run_mac.NewInstr(PEGetInstrName(pe_idx, "core_run_mac_mul"));
    auto state_mul = (state == PE_CORE_RUN_MAC_STATE_MUL);

    instr.SetDecode(child_valid & state_mul);

    auto accum = BvConst(0, PE_CORE_ACCUM_VECTOR_BITWIDTH);

    // do adaptive flow multiplication
    for (auto i = 0; i < CORE_SCALAR; i++) {
      auto weight_byte = child_run_mac.state(
          PEGetVarNameVector(pe_idx, i, CORE_RUN_MAC_CHILD_WEIGHT_BYTE));
      auto input_byte = child_run_mac.state(
          PEGetVarNameVector(pe_idx, i, CORE_RUN_MAC_CHILD_INPUT_BYTE));

      // Need two uninterpreted functions here, one for product sum, one for
      // signed add
      auto result = ProductSum(weight_byte, input_byte);
      accum = AccumAdd(accum, result);
    }

    instr.SetUpdate(result_tmp, accum);

    auto next_state = BvConst(PE_CORE_RUN_MAC_STATE_OUT,
                              PE_CORE_RUN_MAC_CHILD_STATE_BITWIDTH);
    // update the control parameters
    instr.SetUpdate(state, next_state);
  }

  for (auto i = 0; i < CORE_SCALAR; i++) {
    PECoreRunMacOut(m, pe_idx, i);
  }
}

} // namespace flex

} // namespace ilang
