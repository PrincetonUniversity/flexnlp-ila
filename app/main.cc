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

// File: main.cc

#include <flex/flex.h>

#include <iostream>
#include <queue>

#include <ilang/ila/instr_lvl_abs.h>
#include <ilang/target-sc/ila_sim.h>
#include <ilang/util/log.h>

using namespace ilang;

int main(int argc, char* argv[]) {
  SetToStdErr(1);

  // get the ILA model
  auto flex = GetFlexIla("flex_sim");

  ILA_INFO << "#input: " << flex.input_num();
  ILA_INFO << "#state: " << flex.state_num();
  ILA_INFO << "#instr: " << flex.instr_num();

  for (auto i = 0; i < flex.instr_num(); i++) {
    ILA_INFO << flex.instr(i);
  }

  auto model = flex.get();

  ILA_INFO << "before calling sim gen function";
  // simulation generation
  IlaSim simulator_generator;
  std::string sim_gen_dir = "./sim_model";

  simulator_generator.set_instr_lvl_abs(model);
  simulator_generator.enable_cmake_support();
  simulator_generator.sim_gen(sim_gen_dir, false, true, false);

  return 0;
}
