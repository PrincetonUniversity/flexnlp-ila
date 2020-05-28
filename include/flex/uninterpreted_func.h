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

// File: uninterpreted_func.h

// This file declares the uninterpreted functions used in the model

#ifndef FLEX_UN_FUNC_H__
#define FLEX_UN_FUNC_H__

#include <ilang/ilang++.h>
#include <vector>

namespace ilang {
  // uninterpreted functions
  static auto uf_out = SortRef::BV(TOP_DATA_IN_WIDTH);
  static auto uf_in1 = SortRef::BV(TOP_DATA_IN_WIDTH);
  static auto uf_in2 = SortRef::BV(TOP_DATA_IN_WIDTH);

  // uninterpreted functions used in LayerReduce
  static FuncRef adpfloat_max("adpfloat_max", uf_out, uf_in1, uf_in2);
  static FuncRef adpfloat_mean("adpfloat_mean", uf_out, uf_in1, uf_in2);
  static FuncRef adpfloat_add("adpfloat_add", uf_out, uf_in1, uf_in2);

  // uninterpreted functions used in pe_core
  static auto uf_accum_scalar = SortRef::BV(PE_CORE_ACCUMSCALAR_BITWIDTH);
  static auto uf_accum_bias = SortRef::BV(PE_CORE_ADPFLOAT_BIAS_BITWIDTH);
  static std::vector<SortRef> right_shift_input = {uf_accum_scalar, uf_accum_bias, uf_accum_bias};
  static FuncRef PECoreAccumRightShift("PECoreAccumRightShift", uf_accum_scalar, right_shift_input);

  static auto uf_accum_bias_input = SortRef::BV(PE_CORE_SCALAR_BITWIDTH);
  static auto uf_accum_bias_bias = SortRef::BV(PE_CORE_ADPFLOAT_BIAS_B_BITWIDTH);
  static std::vector<SortRef> get_bias_input = {uf_accum_scalar, uf_accum_bias_input, 
                                          uf_accum_bias_bias};
  static FuncRef PECoreAccumGetBiasOut("PECoreAccumGetBiasOut", uf_accum_scalar, get_bias_input);

  static FuncRef PECoreAccumOverflowCheck("PECoreAccumOverflowCheck", uf_accum_scalar, uf_accum_scalar);

  static auto uf_act_reg_scalar = SortRef::BV(PE_CORE_ACTSCALAR_BITWIDTH);
  static FuncRef PECoreAccum2ActReg("PECoreAccum2ActReg", uf_act_reg_scalar, uf_accum_scalar);

  // uninterpreted functions in pe_act.cc
  static auto uf_act_in1 = SortRef::BV(PE_CORE_ACT_VECTOR_BITWIDTH);
  static auto uf_act_in2 = SortRef::BV(PE_CORE_ACT_VECTOR_BITWIDTH);
  static auto uf_act_out = SortRef::BV(PE_CORE_ACT_VECTOR_BITWIDTH);

  static auto uf_scalar_type = SortRef::BV(TOP_DATA_IN_WIDTH);
  static auto uf_act_type = SortRef::BV(PE_CORE_ACT_VECTOR_BITWIDTH);
  static auto uf_adpbias_type = SortRef::BV(ACT_MNGR_CONFIG_REG_ADPFLOAT_BIAS_WIDTH);

  static FuncRef Adptfloat2Fixed("Adptfloat2Fixed", uf_act_type, uf_scalar_type, uf_adpbias_type);
  static FuncRef Fixed2Adptfloat("Fixed2Adaptfloat", uf_scalar_type, uf_act_type, uf_adpbias_type);

  static FuncRef PEActEadd("PEActEadd", uf_act_out, uf_act_in1, uf_act_in2);
  static FuncRef PEActEmul("PEActEmul", uf_act_out, uf_act_in1, uf_act_in2);
  static FuncRef PEActSigmoid("PEActSigmoid", uf_act_out, uf_act_in1);
  static FuncRef PEActTanh("PEActTanh", uf_act_out, uf_act_in1);
  static FuncRef PEActRelu("PEActRelu", uf_act_out, uf_act_in1);
  static FuncRef PEActOnex("PEActOnex", uf_act_out, uf_act_in1);

  // uninterpreted functions used in gb_layernorm
  // layer normalization shares several same uninterpreted functions with pe_act
  // signed divide
  static auto uf_norm_sum_type = SortRef::BV(GB_LAYER_NORM_SUM_TYPE_BITWIDTH);
  static FuncRef GBNormAdd_24("GBNormAdd_24", uf_norm_sum_type, uf_norm_sum_type, uf_norm_sum_type);
  static FuncRef GBNormDivide_24("GBNormDivide_24", uf_act_out, uf_norm_sum_type, uf_norm_sum_type);
  // signed minus for ActScalar type
  static FuncRef ActSignedMinus("ActSignedMinus", uf_act_out, uf_act_in1, uf_act_in2);
  // inv_std = 1/sqrt(VAR[X])
  static FuncRef SInvSqrt("SInvSqrt", uf_act_out, uf_act_in1);
}; // namespace ilang

#endif // UN_FUNC_H__