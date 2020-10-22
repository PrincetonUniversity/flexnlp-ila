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

#include <ilang/ilang++.h>
#include <ilang/util/log.h>

using namespace ilang;

int main(int argc, char* argv[]) {
  SetToStdErr(1);

  // get the ILA model
  auto flex = flex::GetFlexIla("flex");

  ILA_INFO << "#input: " << flex.input_num();
  ILA_INFO << "#state: " << flex.state_num();
  ILA_INFO << "#instr: " << flex.instr_num();

  for (auto i = 0; i < flex.instr_num(); i++) {
    ILA_INFO << flex.instr(i);
  }

  for (auto i = 0; i < flex.child_num(); i++) {
      ILA_INFO << "";
      ILA_INFO << "flex child No." << i << ": " << flex.child(i);
      ILA_INFO << "child #instr: " << flex.child(i).instr_num();
      ILA_INFO << "child #state: " << flex.child(i).state_num() - flex.state_num();
      for (auto j = 0; j < flex.child(i).instr_num(); j++) {
          ILA_INFO << flex.child(i).instr(j);
      }
  }

  // simulation generation
  ExportSysCSim(flex, "./sim_model");

  return 0;
}
