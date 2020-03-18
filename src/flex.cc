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

// File: flex.cc

#include <flex/flex.h>

namespace ilang {

Ila GetFlexIla(const std::string& model_name) {
  auto m = Ila(model_name);

  //
  // define interface (I/O) and architectural states
  //
  DefineTopInput(m);

  DefineSharedState(m);
  
  DefineGBConfigState(m);
  DefineGBOtherState(m);

  // DefinePEConfigState(m, 0);
  // DefinePEConfigState(m, 1);
  // DefinePEConfigState(m, 2);
  // DefinePEConfigState(m, 3);

  // DefinePEOtherState(m, 0);
  // DefinePEOtherState(m, 1);
  // DefinePEOtherState(m, 2);
  // DefinePEOtherState(m, 3);

  // GB internal states
  DefineGBInternalStates(m);
  
  // define initial conditions
  DefineInitConditons(m);

  //
  // define valid function
  //
  auto is_wr_or_rd = m.input(TOP_IF_WR) | m.input(TOP_IF_RD);
  auto is_mmio_addr = (m.input(TOP_ADDR_IN) >= TOP_GB_ADDR_MIN) &
                      (m.input(TOP_ADDR_IN) <= TOP_PE3_ADDR_MAX);
  m.SetValid(is_wr_or_rd & is_mmio_addr);

  //
  // define instructions
  //

  // configuration setting
  DefineGBConfigInstr(m);

  // DefinePEConfigInstr(m, 0, TOP_PE0_ADDR_MIN);
  // DefinePEConfigInstr(m, 1, TOP_PE1_ADDR_MIN);
  // DefinePEConfigInstr(m, 2, TOP_PE2_ADDR_MIN);
  // DefinePEConfigInstr(m, 3, TOP_PE3_ADDR_MIN);

  // store instructions
  // This instruction have conflicts with other buffer write instructions.
  DefineGBCoreStore(m);

  // DefinePECoreStore(m, 0, TOP_PE0_ADDR_MIN);
  // DefinePECoreStore(m, 1, TOP_PE1_ADDR_MIN);
  // DefinePECoreStore(m, 2, TOP_PE2_ADDR_MIN);
  // DefinePECoreStore(m, 3, TOP_PE3_ADDR_MIN);
  // DefinePEActStore(m, 0, TOP_PE0_ADDR_MIN);
  // DefinePEActStore(m, 1, TOP_PE1_ADDR_MIN);
  // DefinePEActStore(m, 2, TOP_PE2_ADDR_MIN);
  // DefinePEActStore(m, 3, TOP_PE3_ADDR_MIN);


  // GB specific start instructions
 DefineStartGBAttention(m);
 DefineStartGBControl(m);
 DefineStartGBLayerNorm(m);
  DefineStartGBLayerReduce(m);
 DefineStartGBZeroPadding(m);

  // PE specific start instruction
  // DefineStartPERnnLayerSizing(m, 0, TOP_PE0_ADDR_MIN);
  // DefineStartPERnnLayerSizing(m, 1, TOP_PE1_ADDR_MIN);
  // DefineStartPERnnLayerSizing(m, 2, TOP_PE2_ADDR_MIN);
  // DefineStartPERnnLayerSizing(m, 3, TOP_PE3_ADDR_MIN);

  return m;
}

}; // namespace ilang
