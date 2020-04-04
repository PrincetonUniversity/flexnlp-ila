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

#include <ilang/util/log.h>

#include <iostream>
// #include <simple/lib.h>
#include <ilang/ila/instr_lvl_abs.h>
#include <ilang/target-sc/ila_sim.h>

// for debug
#include <queue>

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
  std::string sim_gen_dir = "./sim_model/";
  std::string systemc_path = "/u/yl29/local";
  bool cpp_gen = false;
  // ILA_INFO << "test";
  simulator_generator.set_instr_lvl_abs(model);
  simulator_generator.set_systemc_path(systemc_path);
  // ILA_INFO << "before debug";
  // simulator_generator.sim_gen_decode_d();
  simulator_generator.enable_cmake_support();
  simulator_generator.sim_gen(sim_gen_dir, false, true, cpp_gen);

  return 0;
}

// these two functions are used for debugging only.
// void IlaSim::sim_gen_decode_d() {
//   std::queue<InstrLvlAbsPtr> ila_queue;
//   ila_queue.push(model_ptr_);
//   DebugLog::Enable("ILA hierarchy");
//   while (!ila_queue.empty()) {
//     auto current_ila = ila_queue.front();
//     ila_queue.pop();
//     ILA_INFO << "current_ila_name:" << current_ila->name();
//     for (unsigned int i = 0; i < current_ila->child_num(); i++) {
//       ILA_INFO << "current_ila_child:" << current_ila->child(i)->name();
//       ila_queue.push(current_ila->child(i));
//     }
//     for (unsigned int i = 0; i < current_ila->instr_num(); i++) {
//       ILA_INFO << "current_ila_instr:" << current_ila->instr(i)->name();
//       create_decode_d(current_ila->instr(i));
//     }
//     ILA_INFO << std::endl;
//   }
//   DebugLog::Disable("ILA hierarchy");
// }

// void IlaSim::create_decode_d(const InstrPtr& instr_expr) {
//   std::stringstream decode_function;
//   std::string indent = "";
//   std::string decode_func_name;
//   ILA_INFO << "cd_0";
//   if (readable_)
//     decode_func_name = "decode_" + instr_expr->host()->name().str() + "_" +
//                        instr_expr->name().str();
//   else
//     decode_func_name =
//         "decode_" + std::to_string(instr_expr->decode()->name().id());
//   ILA_INFO << "cd_1";
//   auto decode_expr = instr_expr->decode();
//   decode_decl(decode_function, indent, decode_func_name);
//   auto valid_expr = instr_expr->host()->valid();
//   auto DfsKernel = [this, &decode_function, &indent](const ExprPtr& e) {
//     dfs_kernel(decode_function, indent, e);
//   };

//   valid_expr->DepthFirstVisit(DfsKernel);
//   decode_check_valid(decode_function, indent, valid_expr, instr_expr);
//   ILA_INFO << "cd2";
//   decode_expr->DepthFirstVisit(DfsKernel);
//   ILA_INFO << "cd3";
//   decode_return(decode_function, indent, decode_expr, instr_expr);
//   ILA_INFO << "cd4";
//   decrease_indent(indent);
//   decode_function << indent << "};" << std::endl;

//   decode_export(decode_function, decode_func_name);
//   decode_mk_file(decode_func_name);
//   return;
// }
