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

// File: pe_config.h

#ifndef FLEX_PE_CONFIG_H__
#define FLEX_PE_CONFIG_H__

#include <flex/pe_act_unit.h>
#include <flex/pe_addr_map.h>
#include <flex/pe_core.h>

namespace ilang {

namespace flex {

inline std::string PEGetVarName(const int& idx, const std::string& var_name) {
  ILA_ASSERT(idx >= 0 && idx < 4) << "Invalid PE index " << idx;
  auto prefix = std::to_string(idx);
  return "pe" + prefix + "_" + var_name;
}

inline std::string PEGetInstrName(const int& idx,
                                  const std::string& instr_name) {
  ILA_ASSERT(idx >= 0 && idx < 4) << "Invalid PE index " << idx;
  auto prefix = std::to_string(idx);
  return "PE" + prefix + "_" + instr_name;
}

inline std::string PEGetChildName(const int& idx,
                                  const std::string& child_name) {
  ILA_ASSERT(idx >= 0 && idx < 4) << "Invalid PE index " << idx;
  auto prefix = std::to_string(idx);
  return "PE" + prefix + "_" + child_name;
}

inline std::string PEGetVarNameVector(const int& pe_idx, const int& v_idx,
                                      const std::string& var_name) {
  ILA_ASSERT(pe_idx >= 0 && pe_idx < 4) << "Invalid PE index" << pe_idx;
  ILA_ASSERT(v_idx >= 0 && v_idx < CORE_SCALAR)
      << "Invalid PE vector index" << v_idx;
  auto prefix = std::to_string(pe_idx);
  auto suffix = std::to_string(v_idx);
  return "PE" + prefix + "_" + var_name + "_" + suffix;
}

inline std::string PEGetActRegName(const int& pe_idx, const int& reg_idx,
                                   const int& v_idx,
                                   const std::string& var_name) {
  return PEGetVarNameVector(pe_idx, reg_idx, var_name) + "_" +
         std::to_string(v_idx);
}

inline std::string PEGetGBDataInName(const int& reg_idx,
                                     const std::string& var_name) {
  ILA_ASSERT(reg_idx >= 0 && reg_idx < ACT_SCALAR)
      << "Invalid GB data in index" << reg_idx;
  auto suffix = std::to_string(reg_idx);
  return var_name + "_" + suffix;
}

inline std::string PEGetClusterData(const int& pe_idx, const int& byte_idx,
                                    const std::string& var_name) {
  ILA_ASSERT(pe_idx >= 0 && pe_idx < 4) << "Invalid PE index" << pe_idx;
  ILA_ASSERT(byte_idx >= 0 && byte_idx < CORE_SCALAR)
      << "Invalid PE byte index" << byte_idx;
  auto prefix = std::to_string(pe_idx);
  auto suffix = std::to_string(byte_idx);
  return "PE" + prefix + "_" + var_name + suffix;
}

} // namespace flex

} // namespace ilang

#endif // FLEX_PE_CONFIG_H__
