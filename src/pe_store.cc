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

// File: pe_store.cc

#include <flex/flex.h>

namespace ilang {

namespace flex {

void DefinePEStore(Ila& m, const int& pe_idx, const uint64_t& base) {

  { // instruction 0 ---- PE Core Store in Weight buffer
    auto instr = m.NewInstr(PEGetInstrName(pe_idx, "CORE_STORE_WEIGHT_BUFFER"));

    auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
    auto addr_base = TOP_ADDR_BASE + (pe_idx + 1) * PE_PARTITION_SIZE;
    auto valid_addr =
        (m.input(TOP_ADDR_IN) >= (addr_base + PE_CORE_STORE_WEIGHT_ADDR_MIN)) &
        (m.input(TOP_ADDR_IN) <= (addr_base + PE_CORE_STORE_WEIGHT_ADDR_MAX));
    // ILA_INFO << "PE " << pe_idx << " base addr: " << std::hex << addr_base;

    instr.SetDecode(is_write & valid_addr);

    auto mem = m.state(PEGetVarName(pe_idx, CORE_WEIGHT_BUFFER));
    auto addr =
        m.input(TOP_ADDR_IN) - addr_base - PE_CORE_STORE_WEIGHT_ADDR_MIN;

    auto mem_next = Store(mem, (addr + 0), m.input(TOP_DATA_IN_0));
    mem_next = Store(mem_next, (addr + 1), m.input(TOP_DATA_IN_1));
    mem_next = Store(mem_next, (addr + 2), m.input(TOP_DATA_IN_2));
    mem_next = Store(mem_next, (addr + 3), m.input(TOP_DATA_IN_3));
    mem_next = Store(mem_next, (addr + 4), m.input(TOP_DATA_IN_4));
    mem_next = Store(mem_next, (addr + 5), m.input(TOP_DATA_IN_5));
    mem_next = Store(mem_next, (addr + 6), m.input(TOP_DATA_IN_6));
    mem_next = Store(mem_next, (addr + 7), m.input(TOP_DATA_IN_7));
    mem_next = Store(mem_next, (addr + 8), m.input(TOP_DATA_IN_8));
    mem_next = Store(mem_next, (addr + 9), m.input(TOP_DATA_IN_9));
    mem_next = Store(mem_next, (addr + 10), m.input(TOP_DATA_IN_10));
    mem_next = Store(mem_next, (addr + 11), m.input(TOP_DATA_IN_11));
    mem_next = Store(mem_next, (addr + 12), m.input(TOP_DATA_IN_12));
    mem_next = Store(mem_next, (addr + 13), m.input(TOP_DATA_IN_13));
    mem_next = Store(mem_next, (addr + 14), m.input(TOP_DATA_IN_14));

    instr.SetUpdate(mem, Store(mem_next, (addr + 15), m.input(TOP_DATA_IN_15)));
  }

  { // instruction 1 ---- PE Core Store for input buffer
    auto instr = m.NewInstr(PEGetInstrName(pe_idx, "CORE_STORE_INPUT_BUFFER"));

    auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
    auto addr_base = TOP_ADDR_BASE + (pe_idx + 1) * PE_PARTITION_SIZE;
    auto valid_addr =
        (m.input(TOP_ADDR_IN) >= (addr_base + PE_CORE_STORE_INPUT_ADDR_MIN)) &
        (m.input(TOP_ADDR_IN) <= (addr_base + PE_CORE_STORE_INPUT_ADDR_MAX));

    instr.SetDecode(is_write & valid_addr);

    auto mem = m.state(PEGetVarName(pe_idx, CORE_INPUT_BUFFER));
    auto addr = m.input(TOP_ADDR_IN) - addr_base - PE_CORE_STORE_INPUT_ADDR_MIN;

    auto mem_next = Store(mem, (addr + 0), m.input(TOP_DATA_IN_0));
    mem_next = Store(mem_next, (addr + 1), m.input(TOP_DATA_IN_1));
    mem_next = Store(mem_next, (addr + 2), m.input(TOP_DATA_IN_2));
    mem_next = Store(mem_next, (addr + 3), m.input(TOP_DATA_IN_3));
    mem_next = Store(mem_next, (addr + 4), m.input(TOP_DATA_IN_4));
    mem_next = Store(mem_next, (addr + 5), m.input(TOP_DATA_IN_5));
    mem_next = Store(mem_next, (addr + 6), m.input(TOP_DATA_IN_6));
    mem_next = Store(mem_next, (addr + 7), m.input(TOP_DATA_IN_7));
    mem_next = Store(mem_next, (addr + 8), m.input(TOP_DATA_IN_8));
    mem_next = Store(mem_next, (addr + 9), m.input(TOP_DATA_IN_9));
    mem_next = Store(mem_next, (addr + 10), m.input(TOP_DATA_IN_10));
    mem_next = Store(mem_next, (addr + 11), m.input(TOP_DATA_IN_11));
    mem_next = Store(mem_next, (addr + 12), m.input(TOP_DATA_IN_12));
    mem_next = Store(mem_next, (addr + 13), m.input(TOP_DATA_IN_13));
    mem_next = Store(mem_next, (addr + 14), m.input(TOP_DATA_IN_14));

    instr.SetUpdate(mem, Store(mem_next, (addr + 15), m.input(TOP_DATA_IN_15)));
  }

  // update 04132020: change the function name from PECoreStore to PEStore to
  // include the store function to activation buffer, which is visible at the
  // AXI interface.
  { // instructions 2 ---- PE Act Store for Act Buffer
    auto instr = m.NewInstr(PEGetInstrName(pe_idx, "ACT_STORE_BUFFER"));

    auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
    auto valid_addr = (m.input(TOP_ADDR_IN) >= (base + PE_ACT_STORE_ADDR_MIN)) &
                      (m.input(TOP_ADDR_IN) <= (base + PE_ACT_STORE_ADDR_MAX));

    instr.SetDecode(is_write & valid_addr);

    auto mem = m.state(PEGetVarName(pe_idx, ACT_BUFFER));
    auto addr = m.input(TOP_ADDR_IN) - base - PE_ACT_STORE_ADDR_MIN;

    auto mem_next = Store(mem, (addr + 0), m.input(TOP_DATA_IN_0));
    mem_next = Store(mem_next, (addr + 1), m.input(TOP_DATA_IN_1));
    mem_next = Store(mem_next, (addr + 2), m.input(TOP_DATA_IN_2));
    mem_next = Store(mem_next, (addr + 3), m.input(TOP_DATA_IN_3));
    mem_next = Store(mem_next, (addr + 4), m.input(TOP_DATA_IN_4));
    mem_next = Store(mem_next, (addr + 5), m.input(TOP_DATA_IN_5));
    mem_next = Store(mem_next, (addr + 6), m.input(TOP_DATA_IN_6));
    mem_next = Store(mem_next, (addr + 7), m.input(TOP_DATA_IN_7));
    mem_next = Store(mem_next, (addr + 8), m.input(TOP_DATA_IN_8));
    mem_next = Store(mem_next, (addr + 9), m.input(TOP_DATA_IN_9));
    mem_next = Store(mem_next, (addr + 10), m.input(TOP_DATA_IN_10));
    mem_next = Store(mem_next, (addr + 11), m.input(TOP_DATA_IN_11));
    mem_next = Store(mem_next, (addr + 12), m.input(TOP_DATA_IN_12));
    mem_next = Store(mem_next, (addr + 13), m.input(TOP_DATA_IN_13));
    mem_next = Store(mem_next, (addr + 14), m.input(TOP_DATA_IN_14));

    instr.SetUpdate(mem, Store(mem_next, (addr + 15), m.input(TOP_DATA_IN_15)));
  }
}

} // namespace flex

} // namespace ilang
