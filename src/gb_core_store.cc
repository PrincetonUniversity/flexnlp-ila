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

// File: gb_core_store.cc

#include <flex/flex.h>

namespace ilang {

namespace flex {

void DefineGBCoreStore(Ila& m) {
  // TODO:

  // Instruction for storing data into the GB large buffer
  {
    auto instr = m.NewInstr("GB_CORE_STORE_LARGE");
    // decode condition
    auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
    auto valid_addr = (m.input(TOP_ADDR_IN) >=
                       (TOP_ADDR_BASE + GB_CORE_STORE_LARGE_ADDR_MIN)) &
                      (m.input(TOP_ADDR_IN) <=
                       (TOP_ADDR_BASE + GB_CORE_STORE_LARGE_ADDR_MAX));
    instr.SetDecode(is_write & valid_addr);

    // state updates, data come in by 16 bytes
    // translate the input address to the entry number of the buffer states
    auto base_addr =
        m.input(TOP_ADDR_IN) - TOP_ADDR_BASE - GB_CORE_STORE_LARGE_ADDR_MIN;
    auto mem = m.state(GB_CORE_LARGE_BUFFER);

    auto mem_next = Store(mem, (base_addr + 0), m.input(TOP_DATA_IN_0));
    mem_next = Store(mem_next, (base_addr + 1), m.input(TOP_DATA_IN_1));
    mem_next = Store(mem_next, (base_addr + 2), m.input(TOP_DATA_IN_2));
    mem_next = Store(mem_next, (base_addr + 3), m.input(TOP_DATA_IN_3));
    mem_next = Store(mem_next, (base_addr + 4), m.input(TOP_DATA_IN_4));
    mem_next = Store(mem_next, (base_addr + 5), m.input(TOP_DATA_IN_5));
    mem_next = Store(mem_next, (base_addr + 6), m.input(TOP_DATA_IN_6));
    mem_next = Store(mem_next, (base_addr + 7), m.input(TOP_DATA_IN_7));
    mem_next = Store(mem_next, (base_addr + 8), m.input(TOP_DATA_IN_8));
    mem_next = Store(mem_next, (base_addr + 9), m.input(TOP_DATA_IN_9));
    mem_next = Store(mem_next, (base_addr + 10), m.input(TOP_DATA_IN_10));
    mem_next = Store(mem_next, (base_addr + 11), m.input(TOP_DATA_IN_11));
    mem_next = Store(mem_next, (base_addr + 12), m.input(TOP_DATA_IN_12));
    mem_next = Store(mem_next, (base_addr + 13), m.input(TOP_DATA_IN_13));
    mem_next = Store(mem_next, (base_addr + 14), m.input(TOP_DATA_IN_14));
    instr.SetUpdate(mem,
                    Store(mem_next, (base_addr + 15), m.input(TOP_DATA_IN_15)));
  }

  // Instructions for storing data into the GB small buffer
  {
    auto instr = m.NewInstr("GB_CORE_STORE_SMALL");
    // decode condition
    auto is_write = m.input(TOP_IF_WR) & ~m.input(TOP_IF_RD);
    auto valid_addr = (m.input(TOP_ADDR_IN) >=
                       (TOP_ADDR_BASE + GB_CORE_STORE_SMALL_ADDR_MIN)) &
                      (m.input(TOP_ADDR_IN) <=
                       (TOP_ADDR_BASE + GB_CORE_STORE_SMALL_ADDR_MAX));
    instr.SetDecode(is_write & valid_addr);

    // state updates, data come in by 16 bytes
    // translate the input address to the entry number of the buffer states
    auto base_addr =
        m.input(TOP_ADDR_IN) - TOP_ADDR_BASE - GB_CORE_STORE_SMALL_ADDR_MIN;
    auto mem = m.state(GB_CORE_SMALL_BUFFER);

    auto mem_next = Store(mem, (base_addr + 0), m.input(TOP_DATA_IN_0));
    mem_next = Store(mem_next, (base_addr + 1), m.input(TOP_DATA_IN_1));
    mem_next = Store(mem_next, (base_addr + 2), m.input(TOP_DATA_IN_2));
    mem_next = Store(mem_next, (base_addr + 3), m.input(TOP_DATA_IN_3));
    mem_next = Store(mem_next, (base_addr + 4), m.input(TOP_DATA_IN_4));
    mem_next = Store(mem_next, (base_addr + 5), m.input(TOP_DATA_IN_5));
    mem_next = Store(mem_next, (base_addr + 6), m.input(TOP_DATA_IN_6));
    mem_next = Store(mem_next, (base_addr + 7), m.input(TOP_DATA_IN_7));
    mem_next = Store(mem_next, (base_addr + 8), m.input(TOP_DATA_IN_8));
    mem_next = Store(mem_next, (base_addr + 9), m.input(TOP_DATA_IN_9));
    mem_next = Store(mem_next, (base_addr + 10), m.input(TOP_DATA_IN_10));
    mem_next = Store(mem_next, (base_addr + 11), m.input(TOP_DATA_IN_11));
    mem_next = Store(mem_next, (base_addr + 12), m.input(TOP_DATA_IN_12));
    mem_next = Store(mem_next, (base_addr + 13), m.input(TOP_DATA_IN_13));
    mem_next = Store(mem_next, (base_addr + 14), m.input(TOP_DATA_IN_14));
    instr.SetUpdate(mem,
                    Store(mem_next, (base_addr + 15), m.input(TOP_DATA_IN_15)));
  }
}

} // namespace flex

} // namespace ilang
